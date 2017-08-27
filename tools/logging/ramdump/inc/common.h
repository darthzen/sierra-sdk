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
// #define                                 DBG
// #define                                 SERCOMM_DBG
// #define                                 RAMDUMP_DBG
// #define                                 SWI_DEBUG
// #define                                 SWI_DEBUG_INFO
#define bool                            int
#define TRUE                            1
#define FALSE                           0

#define ASYNC_HDLC_FLAG                 0x7E
#define ASYNC_HDLC_ESC                  0x7D
#define ASYNC_HDLC_ESC_MASK             0x20

typedef unsigned char                   uint8;  /* 8 bit integer unsigned */
typedef unsigned short                  uint16; /* 16 bit integer unsigned */

/* This section is for building for 64-bit Linux hosts */

#ifdef __LP64__                     /* GCC macro indicating longs are 64-bits */
typedef unsigned int       uint32;  /* 32 bit integer unsigned  */
typedef signed int         int32;   /* 32 bit integer signed    */
#else                               /* longs are 32-bits (default) */
typedef unsigned long      uint32;  /* 32 bit integer unsigned  */
typedef signed long        int32;   /* 32 bit integer signed    */
#endif /* __LP64__ */

/* Maximum Packet Size */
#define DM_MAX_PACKET_LEN               4096

#define MAX_PATH_NAME 256

/* define struct packing */
#ifndef _PACKED
#ifdef __GNUC__
#define _PACKED __attribute__ ((packed))
#else
#define _PACKED
#endif
#endif /* _PACKED */

/* Debug utility */
#ifdef SWI_DEBUG
#define SWI_PRINTF(...)     fprintf(stderr, __VA_ARGS__)
#else
#define SWI_PRINTF(...)
#endif

#ifdef SWI_DEBUG_INFO
#define SWI_PRINT_INFO(...) fprintf(stderr, __VA_ARGS__)
#else
#define SWI_PRINT_INFO(...)
#endif

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
/* Serial communication function for windows */
extern int serialOpen( const char *);
extern void serialClose();
extern void serialWrite(
    char *pbuf,
    int len);
extern int serialRead(
    char *pbuf,
    int len);

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
