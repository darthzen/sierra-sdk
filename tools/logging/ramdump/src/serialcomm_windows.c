/*************
*
* Filename:  CSerial.cpp
*
* Purpose:   This module handles serial I/O. 
*
* Copyright: © 2010 Sierra Wireless, Inc.
*            All rights reserved
*
**************/


/* Include Files */
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "common.h"

/* Local Constants */
#define RECOMMENDED_BUFFER_LEN 4096
#define FLUSH_BUFFER_SIZE 512
#define MAX_COMPORT_NAME_LEN 20

/* Local variables */
static char flushBuffer[FLUSH_BUFFER_SIZE];
static HANDLE hCom = INVALID_HANDLE_VALUE;

/*************
*
* Name:     serialRead
* 
* Purpose:  Reads data from the serial Port
*           
* Parms:    iBuffer - [OUT] buffer to store data read from COM port
*           ilen    - [IN] Maximum iBuffer size
*
* Return:   int     - Number of bytes read from the COM port.
*
* Abort:    none
*
* Notes:    none
*
**************/
int serialRead(
  char *iBuffer,
  int ilen
)
{
  /* variables used with the com port */
  BOOL bReadRC;
  DWORD iBytesRead = 0;
  DWORD comError;
  COMSTAT comStat;

  ClearCommError(hCom, &comError, &comStat);
  if (comError)
  {
    SWI_PRINTF("\nRead error %lu!\n", comError);
    return 0;
  }
  
  bReadRC = ReadFile(hCom, iBuffer, ilen, &iBytesRead, NULL);
  if (!bReadRC)
  {
    ClearCommError(hCom, &comError, NULL);
    SWI_PRINTF("\nRead error %lu!\n", comError);
  }

  return(iBytesRead);
}


/*************
* 
* Name:     serialWrite
* 
* Purpose:  Write data to the serial Port
*           
* Parms:    oBuffer - [IN] data stream to write to COM port
*           olen    - [IN] length of data stream
*
* Return:   none
*
* Abort:    none
*
* Notes:    none
*
**************/
void serialWrite(
  char *oBuffer, 
  int olen
)
{
  /* variables used with the com port */
  BOOL     bWriteRC;
  DWORD iBytesWritten;

  /* First purge the com port: */
  PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR); 

  bWriteRC = WriteFile(hCom, oBuffer, olen, &iBytesWritten, NULL);
  if (!bWriteRC)
  {
    SWI_PRINTF("Could not write data (%lu)!\n", GetLastError());
  }
}


/*************
* 
* Name:     serialClose
* 
* Purpose:  Flush data from the input port until there is none left, and then close the port
*           
* Parms:    none
*
* Return:   none
*
* Abort:    none
*
* Notes:    This is a potential infinite loop!
*
**************/
void serialClose()
{
  COMMTIMEOUTS m_CommTimeouts;
  int bytesRead;

  if ((hCom != INVALID_HANDLE_VALUE) && (hCom != NULL))
  {
    /* We don't want to use long timeouts here */
    m_CommTimeouts.ReadIntervalTimeout = 10;
    m_CommTimeouts.ReadTotalTimeoutConstant = 50;
    m_CommTimeouts.ReadTotalTimeoutMultiplier = 1;
    m_CommTimeouts.WriteTotalTimeoutConstant = 50;
    m_CommTimeouts.WriteTotalTimeoutMultiplier = 10;
    
    (void)SetCommTimeouts(hCom, &m_CommTimeouts);

    /* Flush data out of the port */
    do
    {
      bytesRead = serialRead(flushBuffer, FLUSH_BUFFER_SIZE);
    }
    while(bytesRead);

    CloseHandle(hCom);

    /* Ensure the port is not closed twice */
    hCom = INVALID_HANDLE_VALUE;
  }
}


/*************
* 
* Name:     serialOpen
* 
* Purpose:  Initialize the serial port
*           
* Parms:    m_sComPort  - [IN] desired COM port to use
*
* Return:   HANDLE      - Handle to the open serial port
*
* Abort:    none
*
* Notes:    HANDLE is INVALID_HANDLE_VALUE if port cannot be opened
*
**************/
int serialOpen(
  const char *m_sComPort
)
{
  /* variables used with the com port */
  BOOL     m_bPortReady;
  DCB      m_dcb;
  COMMTIMEOUTS m_CommTimeouts;
  char comport[MAX_COMPORT_NAME_LEN];

  /* Adjust string so it works for COM ports >= COM10 */
  memset(comport, 0, MAX_COMPORT_NAME_LEN);    /* make sure the string is null terminated */
  strncpy(comport, "\\\\.\\", MAX_COMPORT_NAME_LEN - 1);
  strncat(comport, m_sComPort, MAX_COMPORT_NAME_LEN - strlen(comport) - 1);

  /* Check if the port is not already opened */
  if (hCom != INVALID_HANDLE_VALUE)
  {
    SWI_PRINTF("Comport is already opened\n");
    return TRUE;
  }

  hCom = CreateFile(comport, 
      GENERIC_READ | GENERIC_WRITE,
      0,        /* exclusive access */
      NULL,     /* no security */
      OPEN_EXISTING,
      0,        /* no overlapped I/O */
      NULL);    /* null template  */

  if (hCom != INVALID_HANDLE_VALUE)
  {
    m_bPortReady = SetupComm(hCom, RECOMMENDED_BUFFER_LEN, RECOMMENDED_BUFFER_LEN);
    if (m_bPortReady)
    {
        m_bPortReady = GetCommState(hCom, &m_dcb);
        if (m_bPortReady)
        {
            m_dcb.BaudRate = CBR_115200;
            m_dcb.ByteSize = 8;
            m_dcb.Parity = NOPARITY;
            m_dcb.StopBits = ONESTOPBIT;
            m_dcb.fAbortOnError = TRUE;
            m_dcb.EvtChar = 0x7E;
            m_dcb.fBinary = TRUE;
            m_dcb.fInX = FALSE;
            m_dcb.fOutX = FALSE;

            m_bPortReady = SetCommState(hCom, &m_dcb);
            if (m_bPortReady)
            {
                m_bPortReady = GetCommTimeouts(hCom, &m_CommTimeouts);
                if (m_bPortReady)
                {
                    /* 10/2011: Updated timeout values are taken from observing Qualcomm tools
                     * using Portcom.  Note that QXDM uses WC of 0, while Memory Debugger uses 
                     * WC of 500.  No issue was observed with SwiLog using the WC of 500.
                     */
                    m_CommTimeouts.ReadIntervalTimeout = -1;
                    m_CommTimeouts.ReadTotalTimeoutConstant = 30000;
                    m_CommTimeouts.ReadTotalTimeoutMultiplier = -1;
                    m_CommTimeouts.WriteTotalTimeoutConstant = 500;
                    m_CommTimeouts.WriteTotalTimeoutMultiplier = 0;
                    
                    m_bPortReady = SetCommTimeouts(hCom, &m_CommTimeouts);
                }
            }
        }
    }
  }
  return(hCom != INVALID_HANDLE_VALUE);
}



