/**
 * Adapter to make Sierra Wireless standard Device Management Tree (version 2) variables
 * available to the Air Vantage Communications Agent.
 *
 * Copyright (C) Sierra Wireless, Inc. 2012. All rights reserved.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <malloc.h>
#include <dlfcn.h>
#include "swi_status.h"
#include "pointer_list.h"
#include "extvars_hdl.h"
#include "lua.h"
#include "lauxlib.h"
#include "devtree_v2.h"


/**
 * The "soname" of the Device Management Tree Specification version 2 API implementation library.
 * The API functions defined in devtree_v2.h are implemented by a shared object library with this
 * name.
 */
#define DEVTREE_LIB_NAME "libdevtree.so.2"

/**
 * Computes the number of entries in a static-sized array.
 */
#define NUM_ARRAY_ENTRIES(x) (sizeof(x)/sizeof(x[0]))

/**
 * Maximum number of characters (excluding null terminator) in an accessor function name.
 */
#define MAX_FUNC_NAME_LEN 99

/**
 * Maximum number of bytes (including null terminator) that can be held in a string
 * value fetched from libdevtree.so.2.
 */
#define MAX_STRING_VALUE_LEN 256

/**
 * Handle of the libdevtree.so.2 library file.
 */
static void* LibHandle = NULL;


/** 
 * Our variable list contains entries that have this structure.
 * Essentially, it contains all the information needed in order to provide the Comm Agent
 * with access to one Device Tree variable.
 * 
 * @note    The name of the "Get" and "Set" accessor functions will be constructed based
 *          on the name of the variable by prefixing the variable name with "dt2_Get" or
 *          "dt2_Set".  E.g., if the variable is called "Foo", then this adapter will look
 *          in "libdevtree.so.2" for the functions "dt2_GetFoo" and "dt2_SetFoo" for use
 *          in getting and setting that variable, respectively.
 */
typedef struct
{
    ExtVars_id_t        id;         /**< The ID that this variable's Dev Tree path is mapped to. */
    const char*         name;       /**< The name of the variable. */
    ExtVars_type_t      type;       /**< The type of the variable. */
}
VarListEntry_t;


/** The list of Variables that this library can provide to the Comm Agent.
 * 
 * This is where the translation from identifiers back to names occurs.
 * This MUST be kept synchronized with devtree2.map in the Comm Agent and
 * the devtree_v2.h (Device Management Tree Specification, version 2).
 */
static const VarListEntry_t VarList[] =
{
    /*  ID,         FIELD NAME,             DATA TYPE */
    {   1,          "DeviceId",             EXTVARS_TYPE_STR},

    {   900,        "RadioTemp",            EXTVARS_TYPE_INT},
    {   902,        "DateTime",             EXTVARS_TYPE_DOUBLE},
    
    /* Cellular Parameters */
    
    /*  ID,         FIELD NAME,             DATA TYPE */
    {   1000,       "ApnOverride",          EXTVARS_TYPE_STR},
    {   1001,       "Apn",                  EXTVARS_TYPE_STR},

    /* CDMA */
    {   1002,       "CdmaEcio"   ,          EXTVARS_TYPE_DOUBLE},
    {   1010,       "CdmaOperator",         EXTVARS_TYPE_STR},
    {   1011,       "CdmaPnOff",            EXTVARS_TYPE_STR},
    {   1012,       "CdmaSid",              EXTVARS_TYPE_STR},
    {   1013,       "CdmaNid",              EXTVARS_TYPE_STR},

    /* GSM */
    {   1020,       "GsmCellId",            EXTVARS_TYPE_STR},
    {   1021,       "GsmEcio",              EXTVARS_TYPE_DOUBLE},
    {   1022,       "Operator",             EXTVARS_TYPE_STR},

    /* Subscriber */
    {   1025,       "PhoneNum",             EXTVARS_TYPE_STR},

    /* Sw info */
    {   1026,       "FwVer",                EXTVARS_TYPE_STR},
    {   1027,       "FwName",               EXTVARS_TYPE_STR},

    /* Link */
    {   1040,       "BytesRcvd",            EXTVARS_TYPE_DOUBLE},
    {   1041,       "BytesSent",            EXTVARS_TYPE_DOUBLE},
    {   1042,       "IP",                   EXTVARS_TYPE_STR},
    {   1043,       "PktRcvd",              EXTVARS_TYPE_DOUBLE},
    {   1044,       "PktSent",              EXTVARS_TYPE_DOUBLE},
    {   1045,       "RoamStatus",           EXTVARS_TYPE_STR},
    {   1046,       "Tech",                 EXTVARS_TYPE_STR},
    {   1050,       "Rssi",                 EXTVARS_TYPE_INT},

    /* LTE */
    {   1060,       "Rsrp",                 EXTVARS_TYPE_INT},
    {   1061,       "Rsrq",                 EXTVARS_TYPE_INT},

    /* GPS Parameters */
    /*  ID,         FIELD NAME,             DATA TYPE */
    {   2000,       "Altitude",             EXTVARS_TYPE_DOUBLE},
    {   2001,       "Latitude",             EXTVARS_TYPE_DOUBLE},
    {   2002,       "Longitude",            EXTVARS_TYPE_DOUBLE},

    {   1030,       "Imei",                 EXTVARS_TYPE_STR},

    /* SIM Parameters */
    {   1031,       "Iccid",                EXTVARS_TYPE_STR},
    {   1032,       "Imsi",                 EXTVARS_TYPE_STR},

    /* Aleos Cellular Parameters */
    {   3000,       "PinStatus",           EXTVARS_TYPE_STR},

    /* internal use */
    {   4000,       "SerialNumber",        EXTVARS_TYPE_STR},
};


/**
 * Computes the number of variables in the variable list.
 */
#define NUM_VARS  NUM_ARRAY_ENTRIES(VarList)


/** List of functions used to get variable values.
 * 
 * This list is indexed according to index into the Var List.  That is, entry i in this
 * list contains the address of the "Get" accessor function for the variable described by
 * entry i in the VarList.
 */
static void* GetFuncList[NUM_VARS];

/** List of functions used to set variable values.
 * 
 * This list is indexed according to index into the Var List.  That is, entry i in this
 * list contains the address of the "Set" accessor function for the variable described by
 * entry i in the VarList.
 */
static void* SetFuncList[NUM_VARS];

static void* ResolveSymbol
(
    const char* name
);

typedef int (* GetWirlessTechFunc_t) ();

/**
 * Search the list of variables for an entry with a particular ID.
 * 
 * @return  The index of the Var List Entry or -1 if not found.
 */
static const int FindVarListEntry
(
    ExtVars_id_t    id  /**< [IN] The ExtVars mapped variable identifier. */
)
{
    int i;
    void* func;

    func = ResolveSymbol("getWirelessTech");

    //return nil handler for technology not in use
    switch( ((GetWirlessTechFunc_t) func)() )
    {
        /*
         * Radio interface currently in use. Values:
         * • 0x00 – RADIO_IF_NO_SVC – None (no service)
         * • 0x01 – RADIO_IF_CDMA_1X – cdma2000® 1X
         * • 0x02 – RADIO_IF_CDMA_1XEVDO – cdma2000® HRPD (1xEV-DO)
         * • 0x03 – RADIO_IF_AMPS – AMPS
         * • 0x04 – RADIO_IF_GSM – GSM
         * • 0x05 – RADIO_IF_UMTS – UMTS
         * • 0x08 – RADIO_IF_LTE – LTE
         */
        case 1:
        case 2:
            if (
                    ((id >= 1020) && (id <=1022)) || //GSM
                    ((id >= 1060) && (id <=1061))    //LTE
               )
                return -1;
            break;

        case 4:
        case 5:
            if (
                    ((id >= 1002) && (id <=1013)) || //CDMA
                    ((id >= 1060) && (id <=1061))    //LTE
               )
                return -1;
            break;

        case 8:
            if (
                    (((id >= 1002) && (id <=1013))    //CDMA
                    || (id == 1021))                  //ECIO
               )
                return -1;
            break;

        case 0:
        default:
            if (
                    ((id >= 1002) && (id <=1013)) || //CDMA
                    ((id >= 1020) && (id <=1022)) || //GSM
                    ((id >= 1060) && (id <=1061))    //LTE
               )
                return -1;
            break;

    }

    for (i = 0; i < NUM_VARS; i++)
    {
        if (VarList[i].id == id)
        {
            return i;
        }
    }
    
    return -1;
}


/**
 * Resolves a symbol from the dynamically loaded library.
 * 
 * @return  the address of the symbol or NULL if not found.
 */
static void* ResolveSymbol
(
    const char* name
)
{
    void* symbolPtr = NULL;
    const char* errStrPtr;
    
    //printf("---- Resolving '%s'...", name);
    
    dlerror();  /* Clear out the error status */
    
    symbolPtr = dlsym(LibHandle, name);
    
    if (symbolPtr)
    {
        printf(" FOUND (%p)\n", symbolPtr);
    }
    else
    {
        errStrPtr = dlerror();
        if (errStrPtr == NULL)
        {
            errStrPtr = "-";
        }
        printf(" NOT FOUND (%s)\n", errStrPtr);
    }
    
    return symbolPtr;
}


/**
 * Fetches the address of a "Get" accessor function for a given variable.
 * 
 * @return  The address of the function or NULL if not found.
 */
static void* ResolveGetFunc
(
    int i   /**< [IN] Index of variable in VarList */
)
{
    if ((GetFuncList[i] == NULL) && (LibHandle != NULL))
    {
        #define GET_PREFIX "dt2_Get"
        
        char funcName[MAX_FUNC_NAME_LEN + 1] = GET_PREFIX; /* Note: the +1 is for the null char */
        
        strncat(funcName, VarList[i].name, sizeof(funcName) - sizeof(GET_PREFIX));

        GetFuncList[i] = ResolveSymbol(funcName);
    }
    
    return GetFuncList[i];
}


/**
 * Fetches the address of a "Set" accessor function for a given variable.
 * 
 * @return  The address of the function or NULL if not found.
 */
static void* ResolveSetFunc
(
    int i   /**< [IN] Index of variable in VarList */
)
{
    if ((SetFuncList[i] == NULL) && (LibHandle != NULL))
    {
        #define SET_PREFIX "dt2_Set"
        
        char funcName[MAX_FUNC_NAME_LEN + 1] = SET_PREFIX; /* Note: the +1 is for the null char */
        
        strncat(funcName, VarList[i].name, sizeof(funcName) - sizeof(SET_PREFIX));
        
        SetFuncList[i] = ResolveSymbol(funcName);
    }
    
    return SetFuncList[i];
}

/**
 * Convert a Device Management Tree Version 2 result code into an swi_status_t code.
 *
 * @return The swi_status_t code.
 */
static swi_status_t DevTreeResultToAwtStatus
(
    dt2_ResultCode_t    dt2Code /**< [IN] The Device Management Tree result code. */
)
{
    swi_status_t awtCode = SWI_STATUS_UNKNOWN_ERROR;
    
    switch (dt2Code)
    {
        case DT2_RESULT_OK:                 /* Success! */
            awtCode = SWI_STATUS_OK;
            break;
            
        case DT2_RESULT_TRUNCATED:          /* The value could not fit in the buffer provided. */
            awtCode = SWI_STATUS_NOT_ENOUGH_MEMORY;
            break;
            
        case DT2_RESULT_DEVICE_NOT_FOUND:   /* The device is not connected or is not working. */
        case DT2_RESULT_WRONG_MODE:         /* The device is in a mode that does not allow this operation. */
            awtCode = SWI_STATUS_SERVICE_UNAVAILABLE;
            break;

        case DT2_RESULT_ERROR:              /* The device reported an error. */
            awtCode = SWI_STATUS_UNKNOWN_ERROR;
            break;
    }

    return awtCode;
}


/**
 * Calls a "get" accessor function for a string typed variable.
 * 
 * @return  A result code (SWI_STATUS_OK on success).
 */
static swi_status_t GetString
(
    void* func,         /**< [IN] Address of the accessor function to be called. */
    void** valuePtrPtr  /**< [OUT] Ptr to where the address of the value will be stored. */
)
{
    static char stringBuff[MAX_STRING_VALUE_LEN];

    *valuePtrPtr = &stringBuff;
    
    /* Clear out the string buffer */
    memset(stringBuff, 0, sizeof(stringBuff));
    
    return DevTreeResultToAwtStatus(((dt2_StringGetFunc_t)func)(stringBuff, sizeof(stringBuff)));
}


/**
 * Calls a "get" accessor function for an int typed variable.
 * 
 * @return  A result code (SWI_STATUS_OK on success).
 */
static swi_status_t GetInt
(
    void* func,         /**< [IN] Address of the accessor function to be called. */
    void** valuePtrPtr  /**< [OUT] Ptr to where the address of the value will be stored. */
)
{
    static int value;
    
    *valuePtrPtr = &value;
    
    return DevTreeResultToAwtStatus(((dt2_IntegerGetFunc_t)func)(&value));
}


/**
 * Calls a "get" accessor function for a double typed variable.
 * 
 * @return  A result code (SWI_STATUS_OK on success).
 */
static swi_status_t GetDouble
(
    void* func,         /**< [IN] Address of the accessor function to be called. */
    void** valuePtrPtr  /**< [OUT] Ptr to where the address of the value will be stored. */
)
{
    static double value;
    
    *valuePtrPtr = &value;
    
    return DevTreeResultToAwtStatus(((dt2_DoubleGetFunc_t)func)(&value));
}


/**
 * Calls a "set" accessor function for a string typed variable.
 * 
 * @return  A result code (SWI_STATUS_OK on success).
 */
static swi_status_t SetString
(
    void* func,     /**< [IN] Address of the accessor function to be called. */
    void* valuePtr  /**< [IN] Ptr to the value to be set */
)
{
    return DevTreeResultToAwtStatus(((dt2_StringSetFunc_t)func)((char*)valuePtr));
}


/**
 * Calls a "set" accessor function for an int typed variable.
 * 
 * @return  A result code (SWI_STATUS_OK on success).
 */
static swi_status_t SetInt
(
    void* func,     /**< [IN] Address of the accessor function to be called. */
    void* valuePtr  /**< [IN] Ptr to the value to be set */
)
{
    return DevTreeResultToAwtStatus(((dt2_IntegerSetFunc_t)func)(*((int*)valuePtr)));
}


/**
 * Calls a "set" accessor function for a double typed variable.
 * 
 * @return  A result code (SWI_STATUS_OK on success).
 */
static swi_status_t SetDouble
(
    void* func,     /**< [IN] Address of the accessor function to be called. */
    void* valuePtr  /**< [IN] Ptr to the value to be set */
)
{
    return DevTreeResultToAwtStatus(((dt2_DoubleSetFunc_t)func)(*((double*)valuePtr)));
}



/**
 * Function that is called by Comm Agent to fetch the value of a variable.
 * 
 * @return  The result code to pass back to the Comm Agent.
 *          SWI_STATUS_OK on success.
 */
static swi_status_t GetVar
(
    void *user_ctx,
    ExtVars_id_t id,
    void** valuePtrPtr,
    ExtVars_type_t* typePtr
)
{
    swi_status_t result = SWI_STATUS_DA_NOT_FOUND;

    int i = FindVarListEntry(id);
    
    fprintf(stderr, "---- GetVar ---- %d -> %d\n", id, i);
    
    if (i >= 0)
    {
        void* getFunc = ResolveGetFunc(i);
        
        if (getFunc != NULL)
        {
            *typePtr = VarList[i].type;
            
            switch (*typePtr)
            {
                case EXTVARS_TYPE_STR:
                    result = GetString(getFunc, valuePtrPtr);
                    if (result == SWI_STATUS_OK)
                    {
                        printf("  Got string value '%s'.\n", *(char**)valuePtrPtr);
                    }
                    break;
                    
                case EXTVARS_TYPE_INT:
                case EXTVARS_TYPE_BOOL:
                    result = GetInt(getFunc, valuePtrPtr);
                    if (result == SWI_STATUS_OK)
                    {
                        printf("  Got integer value '%d'.\n", **(int**)valuePtrPtr);
                    }
                    break;
                    
                case EXTVARS_TYPE_DOUBLE:
                    result = GetDouble(getFunc, valuePtrPtr);
                    if (result == SWI_STATUS_OK)
                    {
                        printf("  Got floating-point value '%.10f'.\n", **(double**)valuePtrPtr);
                    }
                    break;
                    
                case EXTVARS_TYPE_NIL:
                case EXTVARS_TYPE_END:
                    fprintf(stderr, "**** ERROR(devtree_v2::GetVar): Invalid type (%d) at index %d.\n", *typePtr, i);
                    break;
            }
        }
    }
    else
    {
        *typePtr = EXTVARS_TYPE_NIL;
        result = SWI_STATUS_OK;
    }

    return result;
}


/**
 * Sets the value of a single variable.
 * 
 * @return  The result code to pass back to the Comm Agent.
 *          SWI_STATUS_OK on success.
 */
static swi_status_t SetVar
(
    ExtVars_id_t    id,         /**< [IN] The variable ID */
    void*           valuePtr,   /**< [IN] Pointer to the value to be set */ 
    ExtVars_type_t  type        /**< [IN] The value's type */
)
{
    int i = FindVarListEntry(id);
    
    fprintf(stderr, "---- SetVar ---- %d -> %d\n", id, i);
    
    if (i >= 0)
    {
        void* setFunc = ResolveSetFunc(i);
        
        if (setFunc != NULL)
        {
            if (type != VarList[i].type)
            {
                fprintf(stderr,
                        "**** ERROR(devtree_v2): Var with ID %d (at index %d) set to wrong type %d (should be %d).\n",
                        id,
                        i,
                        type,
                        VarList[i].type);
                        
                return SWI_STATUS_DA_BAD_TYPE;
            }
            
            switch (type)
            {
                case EXTVARS_TYPE_STR:
                    return SetString(setFunc, valuePtr);

                case EXTVARS_TYPE_INT:
                case EXTVARS_TYPE_BOOL:
                    return SetInt(setFunc, valuePtr);

                case EXTVARS_TYPE_DOUBLE:
                    return SetDouble(setFunc, valuePtr);
                
                case EXTVARS_TYPE_NIL:
                    fprintf(stderr, "**** ERROR(devtree_v2): Var with ID %d set to NIL.\n", id);
                    return SWI_STATUS_DA_BAD_TYPE;
                    
                case EXTVARS_TYPE_END:
                    break;
            }
            
            fprintf(stderr, "**** ERROR(devtree_v2): Var with ID %d has invalid type %d.\n", id, type);
            return SWI_STATUS_DA_BAD_TYPE;
        }
    }
    
    return SWI_STATUS_DA_NOT_FOUND;
}


/**
 * Function that is called by the Comm Agent to set the values of a list of variables.
 * 
 * @return  The result code to pass back to the Comm Agent.
 *          SWI_STATUS_OK on success.
 */
static swi_status_t SetVars
(
    void*           user_ctx,       /**< Not used */
    int             numVars,        /**< [IN] # of variables (# of entries in the three lists below) */
    ExtVars_id_t*   varIdListPtr,   /**< [IN] Pointer to the list of variable IDs */
    void**          valueListPtr,   /**< [IN] Pointer to the list of variable values */ 
    ExtVars_type_t* typeListPtr     /**< [IN] Pointer to the list of types */
)
{
    size_t i;

    if (numVars < 0)
    {
        printf("**** ERROR(devtree_v2::SetVars): numVars is negative (%d).\n", numVars);
        return SWI_STATUS_DA_NOT_FOUND;
    }
    
    for (i = 0; i < numVars; i++)
    {
        swi_status_t result = SetVar(varIdListPtr[i], valueListPtr[i], typeListPtr[i]);
            
        if (result != SWI_STATUS_OK)
        {
            return result;
        }
    }
    
    return SWI_STATUS_OK;
}


/**
 * Function called by the Comm Agent to fetch the list of variables that this library provides.
 * 
 * @return  The result code to pass back to the Comm Agent.
 *          SWI_STATUS_OK on success.
 */
static swi_status_t GetVarList
(
    void *user_ctx,     /**< Not used. */
    int *nvars,         /**< [OUT] Pointer to where the number of variables should be stored. */
    ExtVars_id_t **vars /**< [OUT] Pointer to where the address of the variable list should be stored. */
)
{
    static int IsIdListInitialized = 0;
    static ExtVars_id_t IdList[NUM_VARS];
    static unsigned int NumIdListEntries = 0;
    
    /* If the list of variable ID numbers hasn't been initialized yet, do so now. */
    if (!IsIdListInitialized)
    {
        unsigned int i;
        for (i = 0; i < NUM_VARS; i++)
        {
            /* If there's is a "Get" accessor for this variable, then
             * this variable must be available. */
            if (GetFuncList[i] != NULL)
            {
                IdList[NumIdListEntries++] = VarList[i].id;
            }
        }
        IsIdListInitialized = 1;
    }
    
    /* Pass back a pointer to the list of IDs for available variables
     * and the number of IDs in that list.  */
    *vars = IdList;
    *nvars = NumIdListEntries;
    
    return SWI_STATUS_OK;
}

/**
 * This is the structure that the Comm Agent's ExtVars interface requires us to provide it.
 * It contains all the parameters we pass to the ExtVars interface when it calls our
 * "luaopen_..." function to fetch them.
 */
static struct ExtVars_API_t ExtVarsParams = {
        .user_ctx       = NULL,         /**< Value passed to callbacks when they are called. */
        .initialize     = NULL,         /**< Called when the library was just loaded. */
        .destroy        = NULL,         /**< Called when the library is about to be unloaded. */
        .get            = GetVar,       /**< Gets the value of a variable. */
        .get_release    = NULL,         /**< Releases any memory used by the "get" function. */
        .set            = SetVars,      /**< Sets the value of a variable. */
        .set_notifier   = NULL,         /**< Sets the "notifier" function to be called on var changes. */
        .list           = GetVarList,   /**< Fetch list of variables provided by this library. */
        .list_release   = NULL,         /**< Releases any memory used by the "list" function. */
        .register_var   = NULL,         /**< Tells us to call the notifier when a specific var changes. */
        .register_all   = NULL,         /**< Tells us to call the notifier when any variable changes. */
};

/**
 * This is called when the Comm Agent opens our library within the Lua environment.
 * All this should do is ask the ExtVars interface to package up our parameters along
 * with our Lua package name and return the result back to the Comm Agent.
 */
int luaopen_agent_devman_extvars_devtree2
(
    lua_State *L
)
{
    /* Open the Device Management Tree (version 2) library. */
    LibHandle = dlopen(DEVTREE_LIB_NAME, RTLD_NOW);
    if (LibHandle == NULL)
    {
        return luaL_error(L, "Failed to open %s [%s]", DEVTREE_LIB_NAME, dlerror());
    }
    else
    {
        printf("---- FOUND %s ----\n", DEVTREE_LIB_NAME);
    }

    /* As the final step, register with the Comm Agent's "ExtVars" API. */
    return ExtVars_return_handler( L, "agent.devman.extvars.devtree2", &ExtVarsParams);
}
