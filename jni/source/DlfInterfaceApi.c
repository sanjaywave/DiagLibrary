#include <stdio.h>
#include "interfaceapi.h"
#include "DlfInterfaceApi.h"
#include "LteLogMask.h"
#include "DlfFileWriter.h"
#include "errors.h"

#define TRUE 1
#define FALSE 0
//int IsDMOpen = FALSE;
//int RadioTechnologyType = 0;

/*
 * Function to enable layer wise logging
 * Return 0 success  ,(-1) failed
 */

int openISFDM(int devicetype, int systype , char * configFilePath){
	u_int32_t returnValue = -1;
	switch(systype)
	{
		case  DLF_GSM  :
		{
			returnValue = init_diag_port(devicetype,DLF_GSM,NULL,configFilePath);
		}
		break;
		case  DLF_WCDMA :
		{
			returnValue = init_diag_port(devicetype,DLF_WCDMA,NULL,configFilePath);
		}
		break;
		case   DLF_LTE :
		{
			returnValue = init_diag_port(devicetype,DLF_LTE,NULL,configFilePath);
		}
		break;
		case DLF_CDMA :
		{
			returnValue = init_diag_port(devicetype,DLF_CDMA,NULL,configFilePath);
		}
		break;
	}
	return returnValue;
}

int closeISFDM()
{
	is_DLF_allowed(FALSE);
	stopDlfFileWrite();
	return close_diag_port();
}

/*
 * Function to start Dlf file Writing
 * Return  0 - Success, -1 failed
 */
int startDlfFileWrite( char* imei,char* testid,u_int32_t imeilen,u_int32_t testidlen)
{
	int returnValue = -1;
	is_DLF_allowed(TRUE);
	d_warning("\n Start of DLF File writing");
	returnValue = DlfFileWriter_startDlfFileWrite( imei,testid,imeilen,testidlen);
	Log_Trace (__FILE__,__LINE__,__func__,LOGL_DEBUG,"Return Value  = %d",returnValue);
	return returnValue;
}

/*
 * Function to stop Dlf File Writing
 * Return  0 - Success, 1 - failed
 */

int stopDlfFileWrite()
{
   is_DLF_allowed(FALSE);
   d_warning("\n DLF File write stopped");
   return  DlfFileWriter_stopDlfFileWrite();
}

/*
 * Function to get  number of Dlf file formed during this test
 * Return :- No of DLF file formed
 */
int  numOfDlfFileFormed()
{
   return DlfFileWriter_numOfDlfFileFormed();
}

int  set_DlfGeoLocationInfomation(float Latitude,float Longitude,u_int32_t GPS_Confidence,
		u_int32_t Data_Position_Confidence)
{
	return Dlffilewriter_set_DlfGeoLocationInfomation(Latitude, Longitude, GPS_Confidence, Data_Position_Confidence);
}


