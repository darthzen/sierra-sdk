#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ba.h"
#include "sqf.h"

#define SQF_FILE_HEADER 0x7D
#define SQF_FILE_FOOTER 0x7e

extern sqf_t sqf;

dispatch(uint8_t *cmd, unsigned len)
{
    int log_idx = 0;
    static event_idx = 0;
    static msg_idx = 0;
    unsigned i=0,j=0;
    int iOffsetStart=0;
    int iOffsetEnd=0;
    event_hdr_t *local_eHdr;
    log_hdr_t   *local_logHdr;
    switch(cmd[0])
    {
        case LOG_HEADER_CMD_CODE: //0x73
            //TODO log_idx bound check
            local_logHdr = (log_hdr_t*) cmd;
            log_idx = local_logHdr->equip_id;
            if(log_idx>=MAX_EQUIP_ID)
            {
	            fprintf(stderr,"!EQUIP ID QUT OF RANGE\n");fflush(stderr);
	            exit(1);
            }
            memcpy(sqf.logs[log_idx].hdr,local_logHdr,sizeof(log_hdr_t));

            if(sqf.logs_enable[log_idx]!=0)
            {
                fprintf(stderr,"!DUPICATE ENTRY NOT SUPPORT :%d %d\n",log_idx,sqf.logs_enable[log_idx]);fflush(stderr);
                exit(1);
            }
            
            sqf.logs[log_idx].ba.sz = sqf.logs[log_idx].hdr->nitem;
            
            for(i=0;i<sqf.logs[log_idx].ba.sz;i++)
            {
                sqf.logs[log_idx].ba.buf[i]=cmd[sizeof(log_hdr_t)+i];
                if (bitarray_get(&sqf.logs[log_idx].ba, i))
                    printf("%d ", i);
            }
            sqf.logs_enable[log_idx] = 1;
            sqf.nlogs++;
            break;
        case MSG_HEADER_CMD_CODE://0x7D:
            iOffsetStart = ((msg_hdr_t*) cmd)->first;
            iOffsetEnd   = ((msg_hdr_t*) cmd)->last;
            i  = iGetMsgOffset(iOffsetStart);
            if ((sqf.msgs[i].hdr->last-sqf.msgs[i].hdr->first) < (iOffsetEnd - iOffsetStart))
            {
                fprintf(stderr,"LOAD : MSG OFFSET NOT IN RANGE!");
                exit(1);
            }

            if( i < MAX_MSG_TYPE_OFFSET )
            {
                for(j=0;j<(iOffsetEnd-iOffsetStart+1);j++)
                {
                    sqf.msgs[i].fields[j] = (uint32_t*) &cmd[sizeof(msg_hdr_t)+(j*4)];
                }
                sqf.nmsgs = 1;
                sqf.msg_enable = 1;
            }else{
                fprintf(stderr,"LOAD : MSG OFFSET ERR!");
                exit(1);
            }
            break;
        case EVENT_HEADER_CMD_CODE://0x82:
            local_eHdr = (event_hdr_t*) cmd;
            if(local_eHdr->nitem > sqf.events[0].ba.sz*8)
            {
                iResizeEventArray(local_eHdr->nitem);
            }
            for(i=0;i<sqf.events[0].ba.sz;i++)
            {
                sqf.events[0].ba.buf[i]=cmd[sizeof(event_hdr_t)+i];
                if (bitarray_get(&sqf.events[0].ba, i))
                    printf("%d ", i);
            }
            printf("\n");
            event_idx=1;
            sqf.nevents = event_idx;
            break;
    }
}

int sqf_load_file(char *szFilePath){
    unsigned idx, start;
    unsigned oidx;
    unsigned n_escaped = 0;
    size_t sz;
    FILE* fd = fopen(szFilePath, "r");
    if(fd==NULL)
    {
        fprintf(stderr,"OPEN FILE FAIL!\n");
        return 0;
    }
    uint8_t *buf = malloc(MAX_SZ);
    uint8_t *escaped = malloc(MAX_SZ);
    printf("fd %p\n", fd);
    sz = fread(buf, 1, MAX_SZ, fd);
    printf("sz %zu\n", sz);
    fclose(fd);
    printf("%x:%x:%x\n", buf[0], buf[1], buf[2]);

    //unescape
    idx = 3;
    oidx = 0;
    while(idx<sz)
    {
        if (buf[idx] != SQF_FILE_HEADER)
        {
            escaped[oidx++] = buf[idx++];
        }
        else
        {
            escaped[oidx++] = buf[idx+1] ^ 0x20;
            idx+=2;
        }
    }
    sz = oidx;
    printf("oidx %d\n", oidx);

    idx = 0;
    start = 0;
    while(idx < sz)
    {
        if (escaped[idx] == SQF_FILE_FOOTER)
        {
            dispatch(&escaped[start], idx-start+1);
            //printf("%d %d\n", start, idx-start+1);
            start = idx+1;
        }
        idx++;
    }
    free(buf);
    free(escaped);
	return 1;
}
