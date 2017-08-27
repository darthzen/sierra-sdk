/*************
*
* $Id: registry.h,v 1.1 2009/04/08 22:15:08 rkirk Exp $
*
* Filename:  registry.h
*
* Purpose:   This module handles registry accesses. 
*
* Copyright: © 2010 Sierra Wireless, Inc.
*            All rights reserved
*
**************/


#if !defined(REGISTRY_H)
#define REGISTRY_H


/*************
* 
* Name:     registry_read_swidiag
* 
* Purpose:  Determine a comport, if active, of a Sierra Wireless device.
*       
* Parms:    friendlyname  - [IN] Friendly Name of the device which is found
*           name          - [OUT] Com port name
*           namelen       - [IN] Maximum length of com port name acceptable in buffer
*
* Return:   TRUE  - Com Port was found, ACcomport is valid
*           FALSE - Com Port was not found, ACcomport is not valid
*
* Abort:    none
*
* Notes:    This function returns the first COM Port found, and returns TRUE when a com port has been found, 
*           and FALSE if none is found.  Subsequent calls to this function will return the next COM port found. 
*           If user is searching for all swidiag ports, they should call this function untill it returns a FALSE.
*
**************/
extern bool registry_read_swidiag(char *friendlyname, char *name, unsigned long namelen); 

/*************
* 
* Name:     registry_check_com
* 
* Purpose:  Determine whether a COM port is connected
*       
* Parms:    comportname  - [IN] COM port name to be found (Format: "COM#")
*
* Return:   TRUE  - Com Port was found
*           FALSE - Com Port was not found
*
* Abort:    none
*
* Notes:    This function checks HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM
*           registry location. 
*           Disconnect any serial port handle beforehand, otherwise the port will
*           still show up in the registry.
**************/
extern bool registry_check_com(char *comportname);


#endif /* REGISTRY_H */

