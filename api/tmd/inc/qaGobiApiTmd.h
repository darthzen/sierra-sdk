/**
 * \ingroup tmd
 *
 * \file    qaGobiApiTmd.h
 *
 * \brief   Thermal Mitigation Device API function prototypes
 */

/*
 * Copyright: © 2011-2016 Sierra Wireless, Inc. all rights reserved
 */
#ifndef __GOBI_API_TMD_H__
#define __GOBI_API_TMD_H__

#ifdef __cplusplus
extern "C" {
#endif
#define MAX_MITIGATION_DEV_LIST_LEN 255
#define MAX_MITIGATION_DEV_ID_LEN 255

/**
 * This structure contains mitigation devices list
 *
 * \param mitigationDevIdLen
 *        - Number of sets of the following elements
 *          - mitigation_dev_id
 *
 * \param mitigationDevId
 *        - Mitigation device ID.
 *
 * \param maxMitigationLevel
 *        - Maximum valid mitigation level.
 *        - Valid range - 0 to max_mitigation_level.
 */
typedef struct
{
    BYTE mitigationDevIdLen;
    CHAR mitigationDevId[MAX_MITIGATION_DEV_LIST_LEN];
    BYTE maxMitigationLevel;
}mitigationDevList;

/**
 * This structure contains mitigation devices list from the remote endpoint
 *
 * \param  pMitigationDevListLen
 *         - Mitigation Device List Length (Optional)
 *         - Number of sets of the following elements
 *         - pMitigationDevList
 *
 * \param pMitigationDevList
 *        - Mitigation Device List (Optional)
 *        - See \ref mitigationDevList for more information.
 *
 *
 */
typedef struct
{
    BYTE              *pMitigationDevListLen;
    mitigationDevList *pMitigationDevList;
} TmdGetMitigationDevListResp;

/**
 *  Used by the control point to gets the list of mitigation devices from the remote endpoint.
 *
 *  \param  pTmdGetMitigationDevListResp[OUT]
 *          - See \ref TmdGetMitigationDevListResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 */
ULONG SLQSTmdGetMitigationDevList(TmdGetMitigationDevListResp *pTmdGetMitigationDevListResp);

/**
 * This structure contains mitigation devices Level request parameters
 *
 * \param mitigationDevIDLen
 *        - Number of sets of the following elements
 *          - mitigation_dev_id
 *
 * \param mitigationDevID
 *        - Mitigation device ID
 *
 */
typedef struct
{
    BYTE              mitigationDevIDLen;
    CHAR              mitigationDevID[MAX_MITIGATION_DEV_ID_LEN];
} TmdGetMitigationLvlReq;

/**
 * This structure contains mitigation devices Level request parameters
 *
 * \param pCurrentmitigationLvl
 *        - Current thermal mitigation level (Optional)
 *
 * \param pReqMitigationLvl
 *        - Requested Thermal Mitigation Level (Optional)
 *        - The requested thermal mitigation level from the client.
 *          The default is zero if the client has not previously set
 *          the mitigation level.
 *
 */
typedef struct
{
    BYTE              *pCurrentmitigationLvl;
    BYTE              *pReqMitigationLvl;
} TmdGetMitigationLvlResp;
/**
 *  Gets the thermal mitigation level for the mitigation device
 *
 *  \param  pTmdGetMitigationLvlReq[IN]
 *          - See \ref TmdGetMitigationLvlReq for more information
 *
 *  \param  pTmdGetMitigationLvlResp[OUT]
 *          - See \ref TmdGetMitigationLvlResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 */
ULONG SLQSTmdGetMitigationLvl(TmdGetMitigationLvlReq *pTmdGetMitigationLvlReq,
                              TmdGetMitigationLvlResp *pTmdGetMitigationLvlResp);

/**
 * This structure contains mitigation devices Level register request parameters
 *
 * \param mitigationDevIDLen
 *        - Number of sets of the following elements
 *          - mitigation_dev_id
 *
 * \param mitigationDevID
 *        - Mitigation device ID
 *
 */
typedef struct
{
    BYTE              mitigationDevIDLen;
    CHAR              mitigationDevID[MAX_MITIGATION_DEV_ID_LEN];
} TmdRegNotMitigationLvlReq;

/**
 *  Registers for notification of mitigation device level changes.
 *
 *  \param  pTmdRegNotMitigationLvlReq[IN]
 *          - See \ref TmdRegNotMitigationLvlReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 */
ULONG SLQSTmdRegNotMitigationLvl ( TmdRegNotMitigationLvlReq *pTmdRegNotMitigationLvlReq);

/**
 * This structure contains mitigation devices Level deregister request parameters
 *
 * \param mitigationDevIDLen
 *        - Number of sets of the following elements
 *          - mitigation_dev_id
 *
 * \param mitigationDevID
 *        - Mitigation device ID
 *
 */
typedef struct
{
    BYTE              mitigationDevIDLen;
    CHAR              mitigationDevID[MAX_MITIGATION_DEV_ID_LEN];
} TmdDeRegNotMitigationLvlReq;

/**
 *  Deregisters the notification for mitigation device level changes.
 *
 *  \param  pTmdDeRegNotMitigationLvlReq[IN]
 *          - See \ref TmdDeRegNotMitigationLvlReq for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 */
ULONG SLQSTmdDeRegNotMitigationLvl ( TmdDeRegNotMitigationLvlReq *pTmdDeRegNotMitigationLvlReq);

/**
 * This structure contains mitigation Level Indication request parameters
 *
 * \param mitigationDevIDLen
 *        - Number of sets of the following elements
 *          - mitigation_dev_id
 *
 * \param mitigationDevID
 *        - Mitigation device ID
 *
 */
typedef struct
{
    BYTE              mitigationDevIDLen;
    CHAR              mitigationDevID[MAX_MITIGATION_DEV_ID_LEN];
} TmdMitigationLvlIndReq;

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_TMD_H__ */
