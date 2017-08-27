#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ba.h"
#include "sqf.h"
#include "crc.h"

#define BIT_TO_BYTE(x) ((x+7)/8)

typedef struct
{
    uint8_t buf[1024];
    unsigned sz;
} cmd_t;

extern sqf_t sqf;

void
cmd_append(
        cmd_t *cmd,
        uint8_t *ptr,
        unsigned len
        )
{
    memcpy( &cmd->buf[cmd->sz],
            ptr, len);
    cmd->sz += len;
}

unsigned escape(
        uint8_t *buf,
        unsigned sz)
{
    uint8_t cmd[1024]; //TODO limit
    unsigned idx = 0;
    unsigned oidx = 0;

    while(idx<sz)
    {
        if ( (buf[idx] != 0x7d) && (buf[idx] != 0x7e) )
        {
            cmd[oidx] = buf[idx];
            oidx++;
        }
        else
        {
            cmd[oidx] = 0x7d;
            cmd[oidx+1] = buf[idx] ^ 0x20;
            oidx+=2;
        }
        idx++;
    }
    memcpy(buf, cmd, oidx);
    return oidx-sz;
}

void
cmd_end(
        cmd_t *cmd
       )
{
    crc_t crc;
    crc = crc_init();
    crc = crc_update(crc, (void*)cmd->buf, cmd->sz);
    crc = crc_finalize(crc);
    cmd->buf[cmd->sz] = crc & 0xff;
    cmd->buf[cmd->sz+1] = crc >> 8;
    cmd->sz+=2;
    cmd->sz += escape(cmd->buf, cmd->sz);
    cmd->buf[cmd->sz] = 0x7e;
    cmd->sz+=1;
}

void sqf_save_file(char *szFilePath)
{
    unsigned payload_len;
    uint8_t *buf = (void*)malloc(MAX_SZ);
    unsigned cmd_start;
    unsigned idx = 0;
    unsigned oidx;
    unsigned sz;
    uint8_t terminator = 0x7e;
    crc_t crc;
    cmd_t cmd;
    unsigned i;
    FILE* ofd = fopen(szFilePath, "w");
    uint8_t hdr[] = {0x76, 0, 1};

    memcpy(buf, hdr, sizeof(hdr));
    idx+=sizeof(hdr);
    //logs
    if(sqf.nlogs>0)
    {
        for(i=0;i<MAX_EQUIP_ID;i++)
        {
            if(sqf.logs_enable[i]==0)
                continue;
	        log_t *cur = &sqf.logs[i];
	        cmd.sz=0;
	        cmd_append(&cmd, (uint8_t*)cur->hdr, sizeof(log_hdr_t));
	        cmd_append(&cmd, cur->ba.buf, BIT_TO_BYTE(cur->hdr->nitem));
	        cmd_end(&cmd);
	        memcpy(&buf[idx], cmd.buf, cmd.sz);
	        idx+=cmd.sz;
        }
    }
    //events
    for(i=0;i<sqf.nevents;i++)
    {
        payload_len = sqf.events[i].ba.sz;
        event_t *cur = &sqf.events[i];
        cmd.sz=0;
        cmd_append(&cmd, (uint8_t*)cur->hdr, sizeof(event_hdr_t));
        cmd_append(&cmd, cur->ba.buf, payload_len);
        cmd_end(&cmd);
        memcpy(&buf[idx], cmd.buf, cmd.sz);
        idx+=cmd.sz;
    }
    //msgs
    if(sqf.msg_enable==1)
    {
        for(i=0;i<MAX_MSG_TYPE_OFFSET;i++)
        {
	        payload_len = (sqf.msgs[i].hdr->last - 
	                sqf.msgs[i].hdr->first + 1) * 4;
	        msg_t *cur = &sqf.msgs[i];
	        cmd.sz=0;
	        cmd_append(&cmd, (uint8_t*)cur->hdr, sizeof(msg_hdr_t));
	        cmd_append(&cmd, (uint8_t*)cur->fields, payload_len);
	        cmd_end(&cmd);
	        memcpy(&buf[idx], cmd.buf, cmd.sz);
	        idx+=cmd.sz;
        }
    }
    //msgs
    fwrite(buf, 1, idx, ofd);
    free(buf);
    fclose(ofd);
}
