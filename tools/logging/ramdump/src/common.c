/************
 *
 * Filename:  common.c
 *
 * Purpose:   File containing implementation for generic DM utilities
 *
 * Copyright: © 2011 Sierra Wireless Inc., All rights reserved
 *
 ************/
#include "common.h"
#include "crc.h"

/* 
 * Name:     dmput16
 *
 * Purpose:  To put a 16 bit value into a packet in little-endian order and
 *           increment the packet pointer beyond the written field.
 *
 * Parms:    field          - 16 bit field value
 *           packetpp [OUT] - memory location of the pointer to the packet
 *                            into which the 16 bit field will be written
 *
 * Return:   NONE
 *
 * Notes:    This function performs no pointer validation
 */
void dmput16(
  uint16 field,
  uint8 ** packetpp)
{
  uint8 *packetp;

  packetp = *packetpp;

  *packetp++ = (uint8)(field & 0x000000FF);
  *packetp++ = (uint8)((field & 0x0000FF00) >> 8);

  *packetpp = packetp;
}

/* 
 * Name:     dmput32
 *
 * Purpose:  To put a 32 bit value into a packet in little-endian order and
 *           increment the packet pointer beyond the written field.
 *
 * Parms:    field          - 32 bit field value
 *           packetpp [OUT] - memory location of the pointer to the packet
 *                            into which the 32 bit field will be written
 *
 * Return:   NONE
 *
 * Notes:    This function performs no pointer validation
 */
void dmput32(
  uint32 field,
  uint8 ** packetpp)
{
  uint8 *packetp;

  packetp = *packetpp;

  *packetp++ = (uint8)(field & 0xFF);
  *packetp++ = (uint8)((field >> 8) & 0xFF);
  *packetp++ = (uint8)((field >> 16) & 0xFF);
  *packetp++ = (uint8)((field >> 24) & 0xFF);

  *packetpp = packetp;
}

/* 
 * Name:     dmcrc
 *
 * Purpose:  To calculate the 16 bit CRC value for a DM packet
 *
 * Parms:    pBuf [IN] - Pointer to beginning of the DM packet
 *           len       - Length of DM packet, including CRC & escape character.
 *
 * Return:   crc - 16 bit CRC value
 *
 * Notes:    Abort on invalid length or buffer results in return value 0
 */
uint16 dmcrc(
  uint8 * pBuf,
  uint16 len)
{
  uint16 crc;

  if ((len <= 3) || (pBuf == NULL))
  {
    return 0;
  }

  if (ASYNC_HDLC_FLAG == *pBuf)
  {
    /* Start after the ASYNC header, and length omits the header, crc, and
     * end character. */
    crc = crc_16_l_calc((unsigned char *)(pBuf + 1), (len - 4) * 8);
  }
  else
  {
    /* Start at beginning, and length omits the crc and end character */
    crc = crc_16_l_calc((unsigned char *)pBuf, (len - 3) * 8);
  }
  return crc;
}

/* 
 * Name:     dmunescape
 *
 * Purpose:  To remove escape characters and unescape data from a DM packet
 *
 * Parms:    pBuf [IN/OUT] - Pointer to beginning of the DM packet
 *           len           - Length of DM packet, including CRC and escape
 *                           character.
 *
 * Return:   left - New packet length after escape characters removed
 *
 * Notes:    Abort on invalid length or buffer results in return value 0
 */
uint16 dmunescape(
  uint8 * pBuf,
  uint16 len)
{
  uint8 *pCopyStart = NULL, *pData = NULL;
  uint16 idx1 = 0, idx2 = 0;
  uint16 left;

  if ((len <= 3) || (NULL == pBuf))
  {
    return 0;
  }

  pData = pBuf;
  left = len;

  /* skip the last byte */
  for (idx1 = 1; idx1 < left - 1; idx1++)
  {
    if (ASYNC_HDLC_ESC == *pData)
    {
      *(pData + 1) |= ASYNC_HDLC_ESC_MASK;
      pCopyStart = pData;

      for (idx2 = idx1; idx2 < left; idx2++)
      {
        *pCopyStart = *(pCopyStart + 1);
        pCopyStart++;
      }
      left--;
    }
    pData++;
  }
  return left;
}

/* 
 *
 * Name:     dmescape
 *
 * Purpose:  To add escape characters to a DM packet
 *
 * Parms:    pBuf [IN/OUT] - Pointer to beginning of the DM packet
 *           inlen         - Length of DM packet, including CRC and escape
 *                           character.
 *           outlen        - Length of escaped packet
 *
 * Return:   None
 *
 * Notes :   Abort on invalid length or buffer results in return value 0
 */
void dmescape(
  uint8 * pBuf,
  uint16 inlen,
  uint16 outlen)
{
  uint8 outBuf[DM_MAX_PACKET_LEN];
  uint8 *pOut = outBuf;
  uint8 *pInBuf;
  uint16 idx;

  if ((inlen <= 3) || (NULL == pBuf) || (NULL == pOut))
  {
    outlen = 0;
    return;
  }

  outlen = inlen;
  pInBuf = pBuf;

  /* Escape the data up to the expected end character */
  for (idx = 0; idx < (inlen - 1); idx++)
  {
    if (ASYNC_HDLC_ESC == *pBuf)
    {
      *pOut++ = *pBuf++;
      *pOut++ = (ASYNC_HDLC_ESC - ASYNC_HDLC_ESC_MASK);
      outlen++;
    }
    else if (ASYNC_HDLC_FLAG == *pBuf)
    {
      *pOut++ = ASYNC_HDLC_ESC;
      *pOut++ = (*pBuf++ - ASYNC_HDLC_ESC_MASK);
      outlen++;
    }
    else
    {
      *pOut++ = *pBuf++;
    }
  }

  /* Add the ENDCHAR character at the end of the packet */
  *pOut++ = *pBuf++;

  memcpy(pInBuf, outBuf, outlen);
}
