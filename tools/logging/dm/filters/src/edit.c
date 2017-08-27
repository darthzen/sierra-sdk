#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sqf.h"
#include "edit.h"

extern sqf_t sqf;

const uint16_t msg_first[MAX_MSG_TYPE_OFFSET]=
{
    0x0000,0x01F4,0x03E8,0x07D0,
    0x0BB8,0x0FA0,0x1194,0x11F8,
    0x1388,0x157C,0x1770,0x1964,
    0x1B58,0x1BBC,0x1C20,0x1F40,
    0x2134,0x2328,0x251C,0x27D8,
    0x280B,0x283C,0x286E,0xc000
};

const uint16_t msg_last[MAX_MSG_TYPE_OFFSET]=
{
    0x0060,0x01FA,0x03EF,0x07D8,
    0x0BC6,0x0FAA,0x11AE,0x1205,
    0x13A6,0x158C,0x17C0,0x1979,
    0x1B5B,0x1BC7,0x1C21,0x1F40,
    0x214B,0x2330,0x2525,0x27E2,
    0x280F,0x283C,0x2886,0xc054
};

int iResizeEventArray(int iNumberOfBits)
{
    uint8_t *tempBuf;
    int j = 0;
    div_t divresult = div(iNumberOfBits,8);
    int iNumberOfBytes= divresult.quot;
    if(divresult.rem>0)
    	iNumberOfBytes++;
    tempBuf = realloc(sqf.events[0].ba.buf,(sizeof(uint8_t)*iNumberOfBytes));
    if(tempBuf==NULL)
    {
        fprintf(stderr,"MEM ERROR!\n");
        exit(1);
    }
    sqf.events[0].ba.buf = tempBuf;
    for(j=sqf.events[0].ba.sz;j<iNumberOfBytes;j++)
        sqf.events[0].ba.buf[j]= 0;
    sqf.events[0].hdr->nitem = iNumberOfBits;
    sqf.iEventByteLength = iNumberOfBytes;
    sqf.events[0].ba.sz = iNumberOfBytes;
    return 0;
}




int iGetLogIndex(uint32_t iEqid)
{
    int i = 0;
    if(sqf.nlogs>0){
        for(i=0;i<sqf.nlogs;i++)
        {
            if(sqf.logs[i].hdr->equip_id==iEqid)
            {
                return i;
            }
        }
        return i;
    }
    return 0;
}

void DestoryLogBuffer()
{
    int i = 0;
    log_hdr_t *ptr ;
    for(i=0;i<MAX_EQUIP_ID;i++)
    {        
        if(sqf.logs[i].hdr) 
            free(sqf.logs[i].hdr);
        sqf.logs[i].hdr = NULL;
        sqf.logs_enable[i] = 0;
        if(sqf.logs[i].ba.buf) 
            free(sqf.logs[i].ba.buf);
        sqf.logs[i].ba.buf = NULL;
        sqf.logs[i].ba.sz = 0;
        sqf.logs_enable[i]=0;
    }
}

int iInitLogBuffer()
{
    int i = 0;
    int j = 0;
    for(i=0;i<MAX_EQUIP_ID;i++)
    {        
        sqf.logs[i].hdr = calloc(1,sizeof(log_hdr_t));
        sqf.logs[i].hdr->cmd = LOG_HEADER_CMD_CODE;
        for(j=0;j<LOG_RESERVED_BYTE;j++)
            sqf.logs[i].hdr->ign0[j]=0;
        sqf.logs[i].hdr->operand=3;
        sqf.logs[i].hdr->equip_id=i;
        sqf.logs[i].hdr->nitem = LOG_MAX_NUMBER_OF_ITEM_PLUS_ONE ;
        sqf.logs_enable[i] = 0;
        sqf.logs[i].ba.buf = calloc(LOG_MAX_NUMBER_OF_ITEM,sizeof(uint32_t));
        sqf.logs_enable[i]=0;
    }
    
    return 0;
}

int iGetMsgOffset(int iNo)
{
    int i=0;
    for(i=0;i<MAX_MSG_TYPE_OFFSET;i++)
    {
        if ( (sqf.msgs[i].hdr->first<=iNo)&&
             (sqf.msgs[i].hdr->last>=iNo)    )
        return i;
    }
    return MAX_MSG_TYPE_OFFSET;
}


void DestoryEventBuffer()
{
    int i = 0;
    log_hdr_t *ptr ;
    for(i=0;i<sqf.nevents;i++)
    {        
        if(sqf.events[i].hdr) 
            free(sqf.events[i].hdr);
        sqf.events[i].hdr = NULL;
        if(sqf.events[0].ba.buf)
            free(sqf.events[0].ba.buf);
        sqf.events[0].ba.buf = NULL;
        sqf.events[0].ba.sz = 0;
    }
}

int iInitEventBuffer()
{
    int i = 0;
    sqf.iEventByteLength=1;
    sqf.events[0].hdr = calloc(1,sizeof(event_hdr_t));
    sqf.events[0].hdr->cmd = EVENT_HEADER_CMD_CODE;
    sqf.events[0].hdr->nitem = 0x09bf;
    for(i=0;i<3;i++)
        sqf.events[0].hdr->ign0[i]=0;
    sqf.events[0].hdr->nitem = 1;
    sqf.events[0].ba.buf = calloc(1,sizeof(uint8_t));
    sqf.events[0].ba.sz = 1;
    sqf.nevents = 1;
}

void DestoryMsgBuffer()
{
    int i = 0;
    log_hdr_t *ptr ;
    for(i=0;i<MAX_MSG_TYPE_OFFSET;i++)
    {        
        if(sqf.msgs[i].hdr) 
            free(sqf.msgs[i].hdr);
        sqf.events[i].hdr = NULL;
        if(sqf.msgs[i].fields)
            free(sqf.msgs[i].fields);
        sqf.msgs[i].fields = NULL;
    }
}




int iInitMsgBuffer()
{
    int i = 0;
    int j = 0;
    sqf.msg_enable=0;
    for(i=0;i<MAX_MSG_TYPE_OFFSET;i++)
    {        
        sqf.msgs[i].hdr = calloc(1,sizeof(msg_hdr_t));
        sqf.nlogs++;
        sqf.msgs[i].hdr->cmd = MSG_HEADER_CMD_CODE;
        sqf.msgs[i].hdr->ign0[j]=0;
        for(j=0;j<2;j++)
        	sqf.msgs[i].hdr->ign1[j]=0;
        sqf.msgs[i].hdr->first=msg_first[i];
        sqf.msgs[i].hdr->last=msg_last[i];
        sqf.msgs[i].fields=calloc((sqf.msgs[i].hdr->last - sqf.msgs[i].hdr->first) +1,sizeof(uint32_t));
    }    
    return 0;
}

int iLogItem(uint32_t iNo,int iOperation)
{

    uint32_t eqid = (iNo&0xF000)>>12;
    uint32_t log_idx = iGetLogIndex(eqid);
    div_t divresult;
    uint32_t i = 0;

    i = iNo & 0xFFF;        
    divresult = div (LOG_MAX_NUMBER_OF_ITEM,8);
    divresult = div (i,8);

    if(iOperation==LOG_SET)        
    {    
       sqf.logs[log_idx].ba.buf[divresult.quot] |= 1<<divresult.rem;
    }
    else
    {        
       sqf.logs[log_idx].ba.buf[divresult.quot] &= ~(1 << divresult.rem) & 0xFF;        
    }
    if(sqf.logs_enable[log_idx]==0)
    {
	    sqf.nlogs++;
	    sqf.logs_enable[log_idx] = 1;
    }

}

int iMsgItem(uint32_t iNo,int iOperation)
{
    int iOffset = iGetMsgOffset(iNo);
    if(iOffset<MAX_MSG_TYPE_OFFSET)
    {
        if(iOperation==LOG_SET)
        	sqf.msgs[iOffset].fields[iNo-sqf.msgs[iOffset].hdr->first]=0x1F;
        else
        	sqf.msgs[iOffset].fields[iNo-sqf.msgs[iOffset].hdr->first]=0;    
        sqf.msg_enable = 1;
    }else{
        fprintf(stderr,"MSG ITEM OFFSET ERROR!");
        exit(1);
    }
}

int iEventItem(uint32_t iNo,int iOperation)
{
    
    div_t divresult = div (iNo,8);
    int iByteIndex = divresult.quot;
    if(sqf.events[0].ba.sz < iByteIndex+1)
    {
        iResizeEventArray(iNo);
    }
    if(iOperation==LOG_SET)
    	sqf.events[0].ba.buf[iByteIndex]|= 1<< divresult.rem;
    else
    	sqf.events[0].ba.buf[iByteIndex] &= ~(1 << divresult.rem) & 0xFF;
}

int iSetLog(uint32_t iNo)
{
    return iLogItem(iNo,LOG_SET);
}
int iSetEvent(uint32_t iNo)
{
    iEventItem(iNo,LOG_SET);
    return 0;
}

int iSetMsg(uint32_t iNo)
{
    if(iGetMsgOffset(iNo)<MAX_MSG_TYPE_OFFSET)
    {
        iMsgItem(iNo,LOG_SET);
    }
    return 0;
}

int iClearLog(uint32_t iNo)
{
    return iLogItem(iNo,LOG_CLEAR);
}
int iClearEvent(uint32_t iNo)
{
    iEventItem(iNo,LOG_CLEAR);
    return 0;
}

int iClearMsg(uint32_t iNo)
{
    if(iGetMsgOffset(iNo)<MAX_MSG_TYPE_OFFSET)
    {
        iMsgItem(iNo,LOG_CLEAR);
    }
    return 0;
}


void sqf_set(log_service_enum iLogService ,uint32_t iNo)
{
    switch(iLogService)
    {
    case LOG_MASK:
        iSetLog(iNo);
        break;
    case EVENT_MASK:
        iSetEvent(iNo);
        break;
    case MSG_MASK:
        iSetMsg(iNo);
        break;
    default:
        break;
    }
}

void sqf_clear(log_service_enum iLogService ,uint32_t iNo)
{
    switch(iLogService)
    {
    case LOG_MASK:
        iClearLog(iNo);
        break;
    case EVENT_MASK:
        iClearEvent(iNo);
        break;
    case MSG_MASK:
        iClearMsg(iNo);
        break;
    default:
        break;
    }
}

int sqf_createbuffer()
{
    iInitLogBuffer();
    iInitMsgBuffer();
    iInitEventBuffer();
    return 0;
}

void sqf_destroybuffer()
{
    DestoryLogBuffer();
    DestoryEventBuffer();
    DestoryMsgBuffer();
}

