/*******************************************************************************
 * Copyright (c) 2012 Sierra Wireless and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Sierra Wireless - initial API and implementation
 *******************************************************************************/

/*
 * This module is an example of a tree handler in C.
 * A tree handler is loaded by the tree manager when a tree is stocked/handled
 * in a special way and requires advanced customization.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "swi_status.h"
#include "swi_log.h"
#include "extvars_hdl.h"

/* Define this macro to enable asynchronous notifications when a leaf's value changes.
 * When it is not defined notifications are done synchronously */
//#define NOTIFY_IN_SEPARATE_THREAD

/* Define this macro to enable dynamic node storage, ie register a node not declared
 * in the map file. When it is not defined, only declared nodes are allowed */
//#define DYNAMIC_NODES_STORAGE

typedef struct
{
    ExtVars_id_t id;
    uint64_t value;
    ExtVars_type_t type;
    int registered:1;
}  treehdlvar_t;

#ifdef DYNAMIC_NODES_STORAGE
#define NVARS 8
#else
#define NVARS 4
#endif

static ExtVars_notify_t *notify;
static ExtVars_ctx_t    *notify_ctx;
static uint8_t all_vars_registered = 0;
static treehdlvar_t treehdlvars[NVARS] = {
  { .id = 1, .type = EXTVARS_TYPE_INT, .value = 0, .registered = 0 },
  { .id = 2, .type = EXTVARS_TYPE_DOUBLE, .value = 0, .registered = 0 },
  { .id = 4, .type = EXTVARS_TYPE_STR, .value = 0, .registered = 0 },
  { .id = 8, .type = EXTVARS_TYPE_BOOL, .value = 0, .registered = 0 },
};


#ifdef NOTIFY_IN_SEPARATE_THREAD
typedef struct {
    int inotified;
    ExtVars_id_t notified_ids[NVARS];
    ExtVars_type_t notified_types[NVARS];
    void *notified_values[NVARS];
} notifier_args_t;

static void *notify_async(void *arg)
{
    notifier_args_t *args = arg;

    notify(notify_ctx, args->inotified, args->notified_ids, args->notified_values, args->notified_types);
    free(args);
    return NULL;
}
#endif

/* Helper: retrieve a variable record from its id. */
static treehdlvar_t * get_treevar(ExtVars_id_t id)
{
    int i;
    for(i = 0; i < NVARS; i++)
        if(treehdlvars[i].id == id)
            return treehdlvars + i;
    SWI_LOG("TREEHDL", ERROR, "Variable %d not found\n", id);
    return NULL;
}

/* Helper: retrieve a variable record for its id if it exists, or find
 * an available entry */
static treehdlvar_t * register_treevar(ExtVars_id_t id)
{
#ifdef DYNAMIC_NODES_STORAGE
    int i;
    treehdlvar_t *var;

    var = get_treevar(id);

    if (var)
        return var;

    for (i = 0; i < NVARS; i++)
        if(treehdlvars[i].type == EXTVARS_TYPE_NIL)
            return treehdlvars + i;
    SWI_LOG("TREEHDL", ERROR, "No space left for variable %d\n", id);
    return NULL;
#else
    return get_treevar(id);
#endif
}

/* This function is called when treemgr registers a new notification callback */
static void treehdl_set_notifier(void *user_ctx, ExtVars_notify_t *f, ExtVars_ctx_t *ctx)
{
    SWI_LOG("TREEHDL", DEBUG, "%s: notify = %p, notify_ctx = %p\n", __FUNCTION__, f, ctx);
    notify = f;
    notify_ctx = ctx;
}

/* This function is called when treemgr get the value attached to a leaf */
static swi_status_t treehdl_get( void *user_ctx, ExtVars_id_t id, void** value, ExtVars_type_t* type)
{
    SWI_LOG("TREEHDL", DEBUG, "%s(%d)\n", __FUNCTION__, id);

    treehdlvar_t *treehdlvar = get_treevar(id);

    if(treehdlvar == NULL) return SWI_STATUS_DA_NOT_FOUND;
    if(value) *value = (treehdlvar->type == EXTVARS_TYPE_STR) ? (void *)treehdlvar->value : &treehdlvar->value;
    if(type)  *type  = treehdlvar->type;
    return SWI_STATUS_OK;
}

/* This function is called when treemgr changes the value attached to a leaf. It supports
 * changing a variable type on the fly, synchronous/asynchronous notifications and dynamic storage (according to the defined macro, see above)
 */
static swi_status_t treehdl_set(void *user_ctx, int nvars, ExtVars_id_t *vars, void **values, ExtVars_type_t *types)
{
    int inotified = 0, i = 0;
    static ExtVars_id_t notified_ids[NVARS];
    static ExtVars_type_t notified_types[NVARS];
    static void *notified_values[NVARS];

    for(i = 0; i < nvars; i++) {
        int notifiable = 0;
    treehdlvar_t *var = register_treevar(vars[i]);

        if(!var)
            return SWI_STATUS_ITEM_NOT_FOUND;

        var->id = vars[i];
        if(types[i] != var->type && var->type == EXTVARS_TYPE_STR) {
            free((void *)var->value);
            var->value = 0;
        }

        switch(types[i]) {
            case EXTVARS_TYPE_STR: {
        const char *newval = (const char *) values[i];
                SWI_LOG("TREEHDL", DEBUG, "%s: Pushing string value \"%s\" for var %d\n", __FUNCTION__, newval, var->id);
        // The type has changed or this is the same type and the value has changed
                if(types[i] != var->type || (var->type == EXTVARS_TYPE_STR && strcmp((const char *)var->value, newval))) {
                    free((void *)var->value);
                    var->value = (uint64_t)strdup(newval);
                    notifiable = 1;
                }
                break;
        }
            case EXTVARS_TYPE_INT: {
                int newval = *(int *) values[i];
                SWI_LOG("TREEHDL", DEBUG, "%s: Pushing int value %d for var %d\n", __FUNCTION__, newval, var->id);
        // The type has changed or this is the same type and the value has changed
                if(types[i] != var->type || (var->type == EXTVARS_TYPE_INT && (int)var->value != newval)) {
                    var->value = (uint64_t)newval;
                    notifiable = 1;
                }
                break;
        }
            case EXTVARS_TYPE_BOOL: {
                int newval = *(int *) values[i];
                SWI_LOG("TREEHDL", DEBUG, "%s: Pushing boolean value %d for var %d\n", __FUNCTION__, newval, var->id);
        // The type has changed or this is the same type and the value has changed
                if((types[i] != var->type) || (var->type == EXTVARS_TYPE_BOOL && ((int)var->value != newval))) {
                    var->value = (uint64_t)newval & 0x1;
                    notifiable = 1;
                }
                break;
            }
            case EXTVARS_TYPE_DOUBLE: {
                double newval = *(double *) values[i];
                SWI_LOG("TREEHDL", DEBUG, "%s: Pushing double value %lf for var %d\n", __FUNCTION__, newval, var->id);
        // The type has changed or this is the same type and the value has changed
                if(types[i] != var->type || (var->type == EXTVARS_TYPE_DOUBLE && (double)var->value != newval)) {
                    var->value = (uint64_t)newval;
                    notifiable = 1;
                }
                break;
            }
            case EXTVARS_TYPE_NIL:
                SWI_LOG("TREEHDL", DEBUG, "%s: Deleting var %d", __FUNCTION__, var->id);
                notifiable = 1;
            default:
                break;
        }

        var->type = types[i];
        if(notifiable && (var->registered || all_vars_registered) && notify) {
            SWI_LOG("TREEHDL", DEBUG, "%s: Notifications enabled, marking var %d for notification\n", __FUNCTION__, var->id);
            notified_ids[inotified] = var->id;
            notified_types[inotified]  = var->type;
            notified_values[inotified] = (var->type == EXTVARS_TYPE_STR) ? (void *)var->value : &var->value;
            inotified++;
        }
    }

    if (inotified > 0) {
#ifdef NOTIFY_IN_SEPARATE_THREAD
        pthread_t t;
        notifier_args_t *args = malloc(sizeof(*args));
		if(args == NULL)
		{
            SWI_LOG("TREEHDL", ERROR, "malloc fail\n");
			return SWI_STATUS_ALLOC_FAILED;
		}
        memcpy(args->notified_ids, notified_ids, sizeof(ExtVars_id_t) * NVARS);
        memcpy(args->notified_types, notified_types, sizeof(ExtVars_type_t) * NVARS);
        memcpy(args->notified_values, notified_values, sizeof(void *) * NVARS);
        args->inotified = inotified;
        pthread_create(&t, NULL, notify_async, args);
    pthread_detach(t);
#else
        notify(notify_ctx, inotified, notified_ids, notified_values, notified_types);
#endif
    }
    return SWI_STATUS_OK;
}

/* These functions are called when treemgr requires notifications for a specific node
   or for all existing nodes attached to the root node (treehdlsample) */
static swi_status_t treehdl_register_var(void *user_ctx, ExtVars_id_t id, int enable)
{
    SWI_LOG("TREEHDL", DEBUG, "%s: id=%d, enable=%d\n", __FUNCTION__, id, enable);

    treehdlvar_t *var = get_treevar(id);
    if(!var)
        return SWI_STATUS_DA_NOT_FOUND;
    var->registered = enable;
    return SWI_STATUS_OK;
}

static swi_status_t treehdl_register_all( void *user_ctx, int enable)
{
    SWI_LOG("TREEHDL", DEBUG, "%s\n", __FUNCTION__);
    all_vars_registered = enable;
    return SWI_STATUS_OK;
}

// This function is called when the tree manager needs to list all existing nodes
// attached to the root node (treehdlsample)
static swi_status_t treehdl_list( void *user_ctx, int *nvars, ExtVars_id_t **vars)
{
    static int initialized = 0;
    static ExtVars_id_t _vars[NVARS];

    if(!initialized)
    {
        int i;
        for(i = 0; i < NVARS; i++)
            _vars[i] = treehdlvars[i].id;
        initialized = 1;
    }
    if (vars) *vars = _vars;
    if (nvars) *nvars = NVARS;
    return SWI_STATUS_OK;
}

// This is the entry point for the Lua module and the function
// which initializes and export the public API to the extvars library
int luaopen_agent_devman_extvars_treehdlsample(lua_State *L)
{
    int i;
    struct ExtVars_API_t treehdl_api = {
        .user_ctx = NULL,
    .initialize = NULL,
    .destroy = NULL,
        .get = treehdl_get,
    .get_release = NULL,
        .set = treehdl_set,
        .set_notifier = treehdl_set_notifier,
        .list = treehdl_list,
    .list_release = NULL,
        .register_var = treehdl_register_var,
        .register_all = treehdl_register_all
    };
    treehdlvars[2].value = (uint64_t)strdup("");
    for (i = 4; i < NVARS; i++)
        treehdlvars[i].type = EXTVARS_TYPE_NIL;
    return ExtVars_return_handler(L, "agent.devman.extvars.treehdlsample", &treehdl_api);
}
