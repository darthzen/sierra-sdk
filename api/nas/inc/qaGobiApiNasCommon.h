/**
 * \ingroup nas
 *
 * \file    qaGobiApiNasCommon.h
 *
 * \brief   Nas Service Common Pack and Unpack function prototypes.
 *
 * Copyright: © 2012-2013 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __GOBI_API_NAS_COMMON_H__
#define __GOBI_API_NAS_COMMON_H__

#include "qaGobiApiNas.h"

extern enum eQCWWANError UnpackTlvCommonSrvStatusInfo (
    BYTE          *pTlvData,
    SrvStatusInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonGSMSrvStatusInfo (
    BYTE             *pTlvData,
    GSMSrvStatusInfo *pResp );

extern enum eQCWWANError UnpackTlvSysInfoCommon (
    BYTE          *pTlvData,
    sysInfoCommon *pResp );

extern enum eQCWWANError UnpackTlvCommonCDMASysInfo (
    BYTE        *pTlvData,
    CDMASysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonHDRSysInfo (
    BYTE       *pTlvData,
    HDRSysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonGSMSysInfo (
    BYTE       *pTlvData,
    GSMSysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonWCDMASysInfo (
    BYTE         *pTlvData,
    WCDMASysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonLTESysInfo (
    BYTE       *pTlvData,
    LTESysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonAddCDMASysInfo (
    BYTE           *pTlvData,
    AddCDMASysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonAddSysInfo (
    BYTE       *pTlvData,
    AddSysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonCallBarringSysInfo (
    BYTE               *pTlvData,
    CallBarringSysInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonCDMASigInfo(
    BYTE       *pTlvData,
    CDMASSInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonHDRSigInfo(
    BYTE       *pTlvData,
    HDRSSInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonGSMSigInfo(
    BYTE *pTlvData,
    INT8 *pResp);

extern enum eQCWWANError UnpackTlvCommonLTESigInfo(
    BYTE      *pTlvData,
    LTESSInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonTDSCDMASigInfo(
    BYTE      *pTlvData,
    INT8      *pResp );

extern enum eQCWWANError UnpackTlvCommonTDSCDMAExtSigInfo(
    BYTE              *pTlvData,
    TDSCDMASigInfoExt *pResp );

enum eQCWWANError UnpackTlvCommonPersnltyLst(
    BYTE                   *pTlvData,
    protocolSubtypeElement *pResp,
    BYTE                   lPersLstLen );

#endif /* __GOBI_API_NAS_COMMON_H__ */
