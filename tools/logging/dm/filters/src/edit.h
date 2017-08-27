#ifndef _EDIT_H_
#define _EDIT_H_
int iInitMsgBuffer();
int iInitLogBuffer();
int iInitEventBuffer();
void DestoryLogBuffer();
void DestoryEventBuffer();
void DestoryMsgBuffer();
int iSetLog(uint32_t iNo);
int iSetEvent(uint32_t iNo);
int iSetMsg(uint32_t iNo);
int iClearLog(uint32_t iNo);
int iClearEvent(uint32_t iNo);
int iClearMsg(uint32_t iNo);
int iLogItem(uint32_t ,int );
int iGetMsgOffset(int iNo);
int iMsgItem(uint32_t ,int );
int iEventItem(uint32_t iNo,int iOperation);
int iResizeEventArray(int iSize);
int iGetLogIndex(uint32_t iEqid);
#endif
