/*************
*
* $Id: registry.cpp,v 1.1 2009/04/08 22:15:08 rkirk Exp $
*
* Filename:  registry.cpp
*
* Purpose:   This module handles registry accesses on Windows. 
*
* Copyright: © 2010 Sierra Wireless, Inc.
*            All rights reserved
*
**************/


/* Include Files */
#include "common.h"
#include "registry_windows.h"
#include "devcon.h"

/* Local Constants */
#define REGKEYBASELEN 34
#define REGKEYTAILLEN 22
#define REGKEYLEN     (MAX_DEVICE_ID_LEN + REGKEYBASELEN + REGKEYTAILLEN)

LPCSTR RegKeyBase = "SYSTEM\\CurrentControlSet\\Enum\\";
LPCSTR RegKeyTail = "\\Device Parameters\\";
LPCSTR RegConnectedCOM = "HARDWARE\\DEVICEMAP\\SERIALCOMM\\";

/*************
* 
* Name:     GetDeviceStringProperty
* 
* Purpose:  Return a string property for a device, otherwise NULL
*       
* Parms:    Devs    - [IN] uniquely identify device (along with DevInfo)
*           DevInfo - [IN] uniquely identify device (along with Devs)
*           Prop    - [IN] string property to obtain
*           desc  - [OUT] String containing device property
*           size  - [IN] Size of string containing device property
*
* Return:   none
*
* Abort:    none
*
* Notes:    none
*
**************/
void GetDeviceStringProperty(HDEVINFO Devs,PSP_DEVINFO_DATA DevInfo,DWORD Prop, char *desc, DWORD &size)

{
  DWORD reqSize;
  DWORD dataType;
  DWORD szChars;

  SetupDiGetDeviceRegistryProperty(Devs,DevInfo,Prop,&dataType,(LPBYTE)desc,size,&reqSize);
    
  szChars = reqSize/sizeof(TCHAR);
  desc[szChars] = TEXT('\0');
}


/*************
* 
* Name:     GetDeviceDescription
* 
* Purpose:  Return a string containing a description of the device, otherwise NULL
*           Always try friendly name first
*       
* Parms:    Devs    - [IN] uniquely identify device (along with DevInfo)
*           DevInfo - [IN] uniquely identify device (along with Devs)
*           desc  - [OUT] String Containing Device Description
*
* Return:   none
*
* Abort:    none
*
* Notes:    none
*
**************/
void GetDeviceDescription(HDEVINFO Devs,PSP_DEVINFO_DATA DevInfo, char *desc)

{
  DWORD size = 1024;

  GetDeviceStringProperty(Devs,DevInfo,SPDRP_FRIENDLYNAME, desc, size);
  
  /* If the return string is empty, try searching for the Device Descrip. */
  if(*desc == '\0')
  {
    GetDeviceStringProperty(Devs,DevInfo,SPDRP_DEVICEDESC, desc, size);
  }
}


/*************
* 
* Name:     registry_read_swidiag
* 
* Purpose:  Determine a comport, if active, of a Sierra Wireless device.
*       
* Parms:    friendlyName  - [OUT] Friendly Name of the device which is found
*           ACcomport     - [OUT] Com port name
*           ACcomportsz   - [IN] Maximum length of com port name acceptable in buffer
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
bool registry_read_swidiag(
  char *friendlyname, 
  char *name,
  unsigned long namelen
)
{
  DWORD reqGuids = 16;
  LPGUID guids = NULL;
  HDEVINFO devs = INVALID_HANDLE_VALUE;
  DWORD numGuids;
  DWORD index;
  long lStatus = 0;
  HKEY hkey;
  DWORD dwType;
  char device_desc[1024];
  char RegKey[REGKEYLEN];
  char devID[MAX_DEVICE_ID_LEN];
  SP_DEVINFO_LIST_DETAIL_DATA devInfoListDetail;
  TCHAR className[MAX_CLASS_NAME_LEN];
  TCHAR classDesc[LINE_LEN];
  DWORD devCount = 0;
  SP_DEVINFO_DATA devInfo;
  DWORD devIndex;
  bool result = false;

  guids = new GUID[reqGuids];
  if(!guids) {
      goto final;
  }

  while(!SetupDiClassGuidsFromNameEx("Ports",guids,reqGuids,&numGuids,NULL,NULL)) 
  {
    if(GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
    {
      goto final;
    }

    delete [] guids;
    reqGuids = numGuids;
    guids = new GUID[reqGuids];
    if(!guids) 
    {
      goto final;
    }
  }
    
  for(index = 0;index<numGuids;index++) 
  {
    devs = SetupDiGetClassDevsEx(&guids[index],NULL,NULL,DIGCF_PRESENT,NULL,NULL,NULL);
    if(devs != INVALID_HANDLE_VALUE) 
    {
      /* count number of devices */
      devInfo.cbSize = sizeof(devInfo);
      while(SetupDiEnumDeviceInfo(devs,devCount,&devInfo)) 
      {
        devCount++;
      }
    }

    if(!SetupDiClassNameFromGuidEx(&guids[index],className,MAX_CLASS_NAME_LEN,NULL,NULL,NULL)) 
    {
      lstrcpyn(className,TEXT("?"),MAX_CLASS_NAME_LEN);
    }
    if(!SetupDiGetClassDescriptionEx(&guids[index],classDesc,LINE_LEN,NULL,NULL,NULL)) 
    {
      lstrcpyn(classDesc,className,LINE_LEN);
    }

    /* if there are devices to process */
    if (devCount > 0) 
    {
      for(devIndex=0;SetupDiEnumDeviceInfo(devs,devIndex,&devInfo);devIndex++) 
      {
        GetDeviceDescription(devs,&devInfo, &device_desc[0]);
        if (strstr(device_desc, friendlyname) != NULL )
        {
          devInfoListDetail.cbSize = sizeof(devInfoListDetail);
          if((!SetupDiGetDeviceInfoListDetail(devs,&devInfoListDetail)) ||
             (CM_Get_Device_ID_Ex(devInfo.DevInst,devID,MAX_DEVICE_ID_LEN,0,devInfoListDetail.RemoteMachineHandle)!=CR_SUCCESS)) 
          {
            lstrcpy(devID,TEXT("?"));
          }

          memset(RegKey, 0, REGKEYLEN);
          strncpy(RegKey, RegKeyBase, REGKEYLEN - 1);
          strncat(RegKey, devID, REGKEYLEN - strlen(RegKey) - 1);
          strncat(RegKey, RegKeyTail, REGKEYLEN - strlen(RegKey) - 1);
          SWI_PRINT_INFO("%s", RegKey);

          lStatus = 0;

          lStatus = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, RegKey, 0, KEY_READ, &hkey);
          if (ERROR_SUCCESS == lStatus)
          {
            ::RegQueryValueEx(hkey, TEXT("Portname"), 0, &dwType,
                              (unsigned char *)name, &namelen);
            ::RegCloseKey(hkey);

            result = true;
            break;
          }
        }
      } 
    }
    if(devs != INVALID_HANDLE_VALUE)
    {
      SetupDiDestroyDeviceInfoList(devs);
      devs = INVALID_HANDLE_VALUE;
    }
  }

final:
  if(guids) 
  {
    delete [] guids;
  }

  if(devs != INVALID_HANDLE_VALUE) 
  {
    SetupDiDestroyDeviceInfoList(devs);
  }

  return result;
}

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

bool registry_check_com(char *comportname)
{
  HKEY hkey;
  bool result = FALSE;
  DWORD index, count;
  DWORD maxNameLen, maxValLen;
  DWORD namelen, comfoundlen;
  char* name;
  char* comfound;

  /* Check for NULL pointer */
  if(comportname == NULL)
  {
    return FALSE;
  }

  if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, RegConnectedCOM, 0,
       KEY_READ, &hkey) != ERROR_SUCCESS)
  {
    return FALSE;
  }

  if (::RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL,
         &count, &maxNameLen, &maxValLen, NULL, NULL) == ERROR_SUCCESS)
  {
    /* Initialize reg name and data variable*/
    name = new char[++maxNameLen];
    comfound = new char[++maxValLen];
  
    for(index = 0 ; index < count; index++)
    {
      namelen = maxNameLen;
      comfoundlen = maxValLen;
      if(::RegEnumValue (hkey, index, name, &namelen, NULL, 
           NULL, (unsigned char *)comfound, &comfoundlen) != ERROR_SUCCESS)
      {
        return FALSE;
      }

      if(strcmp(comportname, comfound) == 0)
      {
        result = TRUE;
        break;
      }
    }

    ::RegCloseKey(hkey);

    delete [] name;
    delete [] comfound;
  }
  return result;
}

