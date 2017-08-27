/*************
 *
 * Filename:    common.h
 *
 * Purpose:     Contains functionality common to all the files in RAM Dump
 *              application.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/****************************************************************
*                       #DEFINES
****************************************************************/
//#define                                 DBG
#define TRUE                            1
#define FALSE                           0

#define ASYNC_HDLC_FLAG                 0x7E
#define ASYNC_HDLC_ESC                  0x7D
#define ASYNC_HDLC_ESC_MASK             0x20

typedef unsigned char                   uint8;  /* 8 bit integer unsigned */
typedef unsigned short                  uint16; /* 16 bit integer unsigned */
typedef unsigned long                   uint32; /* 32 bit integer unsigned */
typedef signed long                     int32;  /* 32 bit integer signed */

/* Maximum Packet Size */
#define DM_MAX_PACKET_LEN               4096

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/


/****************************************************************
*                       FUNCTIONS
****************************************************************/
/* Serial Communication functions */
extern int OpenDMPort( const char * );
extern void CloseDMPort( );
extern void WriteOnDMPort(
    char *pBuf,
    int  len );
extern int ReadFromDMPort(
    char *pBuf,
    int  len );

/* Generic functionality function */
extern void dmput16(
    uint16 field,
    uint8  **packetpp );
extern void dmput32(
    uint32 field,
    uint8  **packetpp );
extern uint16 dmcrc(
    uint8  *pBuf,
    uint16 len );
extern uint16 dmunescape(
    uint8  *pBuf,
    uint16 len );
extern void dmescape(
    uint8  *pBuf,
    uint16 inlen,
    uint16 outlen );


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __COMMON_H__ */
