/*************
 *
 * Filename:    serialcomm.c
 *
 * Purpose:     Handle all the Serial communication in RAM Dump tool
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "common.h"

/****************************************************************
*                       DEFINES
****************************************************************/
#define MAX_PORT_NAME_LEN          20

/****************************************************************
*                    GLOBAL DATA
****************************************************************/
static int serialFileDescriptor = 0;

/****************************************************************
*                       FUNCTIONS
****************************************************************/
/*
 * Name:     OpenDMPort
 *
 * Purpose:  Opens the port specified by the user
 *
 * Params:   portName   - Port name e.g. /dev/ttyUSB<digit>
 *
 * Return:   TRUE       - If the port is opened successfully
 *           FALSE      - If port opening failed
 *
 * Notes:    None
 */
int OpenDMPort( const char *portName )

{
    serialFileDescriptor = open( portName, O_RDWR | O_NOCTTY );
    if( -1 == serialFileDescriptor )
    {
#ifdef SERCOMM_DBG
        fprintf( stderr, "Failed to open Port : %s\n", portName );
        fprintf( stderr, "Port open error %d %s\n", errno, strerror(errno) );
#endif
        return FALSE;
    }

    fcntl( serialFileDescriptor, F_SETFL, 0 );

    /* SIO port settings */
    struct termios portSettings;

    /* Set the Baud rate */
    cfsetispeed( &portSettings, B115200 );
    cfsetospeed( &portSettings, B115200 );

    /* Set Parity - NONE, Stop Bit - 1, Data Bits - 8 */
    portSettings.c_cflag &= ~PARENB;
    portSettings.c_cflag &= ~CSTOPB;
    portSettings.c_cflag &= ~CSIZE;
    portSettings.c_cflag |= CS8;

    portSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    portSettings.c_oflag &= ~OPOST;

    /* Apply the settings to the port */
    tcsetattr( serialFileDescriptor, TCSANOW, &portSettings );

    fprintf( stderr, "Port open Success : %s\n", portName );
    return TRUE;
}

/*
 * Name:     CloseDMPort
 *
 * Purpose:  Close the serial port opened for communication.
 *
 * Params:   None
 *
 * Return:   None
 *
 * Notes:    None
 */
void CloseDMPort( )
{
    /* Check if the port is opened */
    if ( 1 > serialFileDescriptor )
    {
#ifdef SERCOMM_DBG
        fprintf( stderr, "Port is not opened!\n" );
#endif
        return;
    }

    close( serialFileDescriptor );
    serialFileDescriptor = 0;
}

/*
 * Name:     WriteOnDMPort
 *
 * Purpose:  Write on the DM port opened for communication.
 *
 * Params:   pBuf [IN] - Data stream to write to COM port
 *           len       - Length of data stream
 *
 * Return:   None
 *
 * Notes:    None
 */
void WriteOnDMPort(
    char *pBuf,
    int  len )
{
    int returnValue = 0;

    /* Check if the port is opened */
    if ( 1 > serialFileDescriptor )
    {
        fprintf( stderr, "Port is not opened!\n" );
        return;
    }

    /* Flush both the input as well as output stream */
    tcflush( serialFileDescriptor, TCIOFLUSH );

    /* Write the data to the port */
    returnValue = write( serialFileDescriptor, pBuf, len );
    if( 0 > returnValue )
    {
#ifdef SERCOMM_DBG
        fprintf( stderr, "Write error %d %s\n", errno, strerror(errno) );
#endif
    }
}

/*
 * Name:     ReadFromDMPort
 *
 * Purpose:  Read data from DM port opened for communication.
 *
 * Params:   (OUT) pBuf - Buffer to read data from COM port
 *           len        - Length of buffer passed
 *
 * Return:   None
 *
 * Notes:    None
 */
int ReadFromDMPort(
    char *pBuf,
    int  len )
{
    int numBytesRead = 0;

    /* Check if the port is opened */
    if ( 1 > serialFileDescriptor )
    {
        fprintf( stderr, "Port is not opened!\n" );
        return numBytesRead;
    }

    /* Read the data from the port */
    numBytesRead = read( serialFileDescriptor, pBuf, len );
    if( 0 > numBytesRead )
    {
#ifdef SERCOMM_DBG
        fprintf( stderr, "Read error %d %s\n", errno, strerror(errno) );
#endif
    }
    return numBytesRead;
}
