# include <stdio.h>
# include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "DlfFileWriter.h"
#include "filewriter.h"
#include "errors.h"

#define GeoLocationLteFIleExtension  ".Qualcomm LTE (0).axf"
#define GEOLOCATION_HEADER "#Message Number,Time,Latitude,Longitude,GPS_Confidence,Data_Position_Confidence\n"
#define GEOLOCATION_HEADER_SIZE 80
#define DATA_SEPRATER_COMMA ","


char DLFParentDirPath[SIZE_OF_TOKEN_BUFFER] = {'\0'};
char DlfFileName[MAX_FILE_NAME]= {'\0'};
u_int32_t NumOfBytesWritten = 0;
u_int32_t fileWriteStatus = 0;
static u_int32_t DlfFileCounter = 1;
static FILE*  DlfFd = NULL;
static FILE*  GeoLocationFileFd = NULL;
static FILE *dollarFp = NULL;
u_int32_t MaxDlfFileSize = 0;
u_int32_t DlfFileWriteStatus = 0;
u_int32_t GeoLocationfilewriteCounter = 0;
static u_int32_t total_NumOfBytesWritten = 0;


/*This mutex is initialize either by DM library or DLF library */
extern pthread_mutex_t fileDlfMutex;

DlfTest_parameter mDlfTest_parameter;
u_int32_t GeoLocationFileCounter = 1;

/*
 * Function to close GeoLocation file.
 * Return 0-Success ,other than 0 in case of failed
 */
int stopGeoLocationFile()
{
	int file_status=-1;
	if(GeoLocationFileFd != NULL)
	file_status=fclose(GeoLocationFileFd);
	if(!file_status)
	{
		GeoLocationFileFd = NULL;
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"GeoLocation File closed !");
	}else{
	    Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"GeoLocation File not closed !");
	}
	GeoLocationfilewriteCounter = 0;
	return file_status;
}

/*
 * Function to add the header in .axf file
 * Return:- Number of byte written .axf file on success,-1 on faliure.
 */
int addHeaderToGEOLocationFile()
{
	int retval = 0;
	retval = fwrite(GEOLOCATION_HEADER,sizeof(char),GEOLOCATION_HEADER_SIZE,GeoLocationFileFd);/* Five is added for 5 comma*/
	if(retval!= GEOLOCATION_HEADER_SIZE){
		d_warning(" Bytes drop ! No of bytes drop = %d ",(GEOLOCATION_HEADER_SIZE - retval));
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"Bytes drops while header write function !");
		retval = -1;
	}
	return retval;
}
	/*
	 * Function to close Dlf file.
	 * Return 0-Success ,other than 0 in case of failed
	 */

int stopDlfWrite (){
	int file_status=-1;
	int GeoLocationFileCloseStatus = -1;
	if(DlfFd != NULL)
	file_status=fclose(DlfFd);
	GeoLocationFileCloseStatus = stopGeoLocationFile();
	if(!(file_status & GeoLocationFileCloseStatus))
	{
		DlfFd = NULL;
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"File closed !");
	}else{
	    Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"File not closed !");
	}
	fprintf(dollarFp, "%u	%s_%03d%s\n", NumOfBytesWritten, DlfFileName, DlfFileCounter-1, FILE_EXTENSION);
	return file_status;
}

/*  Function to convert the time into second
 *  Return seconds
 */
u_int32_t ConvertTimeIntoSecond()
{
	struct tm *tt;
	u_int32_t TimeInSecond = 0;
    time_t  Current_Time = time(NULL);
    tt = localtime(&Current_Time);
    TimeInSecond += (tt->tm_hour*3600);
	TimeInSecond += (tt->tm_min*60);
	TimeInSecond += tt->tm_sec;
	d_log("Time in Second = %u",TimeInSecond);
	return TimeInSecond;
}

/*
 *  Function to write the data bytes in GeoLocation file
 *  Return void
 */
int GeoLocation_file_write(char *time)
  {
	int GeoLocationfilewriteReturn = 0;
	u_int32_t  TimeInSeconds = 0;
	u_int32_t  sprintfOutPut = 0;
	char GeoLocationParameterbuff[GEO_LOCATION_BUFFER_SIZE]={0};
	TimeInSeconds = ConvertTimeIntoSecond();
	sprintfOutPut +=sprintf(GeoLocationParameterbuff,"%u,%u,%f,%f,%d,%d\r\r\n",GeoLocationfilewriteCounter++,
			TimeInSeconds,mDlfTest_parameter.latitude,mDlfTest_parameter.longitude,
			mDlfTest_parameter.gps_confidence,mDlfTest_parameter.data_position_confidence);
	GeoLocationfilewriteReturn = fwrite(GeoLocationParameterbuff, sizeof(char),sprintfOutPut,GeoLocationFileFd);
	if(GeoLocationfilewriteReturn != (sprintfOutPut)){
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"Bytes drops while fwrite function !");
		GeoLocationfilewriteReturn = -1;
	}
	return GeoLocationfilewriteReturn;
}

/*
 *  Function to write the data bytes in dlf file
 *  Return void
 */
int file_write_isf(char *databuff, u_int32_t datalength)
{
	int fwriteReturn= 0;
	fwriteReturn = fwrite(databuff, sizeof(char), datalength, DlfFd);
#ifdef MEMORY_CORRUPTION_REPORT
	fflush(DlfFd);
#endif //MEMORY_CORRUPTION_REPORT
	if(fwriteReturn != datalength){

		Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"Bytes drops while fwrite function %d %d %d !",(datalength-fwriteReturn),ferror(DlfFd),feof(DlfFd));
	}
	NumOfBytesWritten += datalength;
	total_NumOfBytesWritten += datalength;
	/* Function to write in GeoLocation File */
	GeoLocation_file_write(databuff+4);
	if(MaxDlfFileSize != 0) {
		if(NumOfBytesWritten > MaxDlfFileSize)
		{
			fwriteReturn = stopDlfWrite();
			if(!(fwriteReturn))
				fwriteReturn = -1;
			NumOfBytesWritten = 0;
		}
	}
	return fwriteReturn;
}
/*
 * Function to create dollar file,
 * contains the details of all dlf files
 */
void open_dollar_file(char *DlfFileName) {
	char temFileName[MAX_FILE_NAME]= {'\0'};
	int tmp = 0;
	strcpy(temFileName,DlfFileName);
	strcat(temFileName,UNDERBAR);
	sprintf(temFileName + strlen(temFileName),"%03d$",tmp);
	strcat(temFileName,FILE_EXTENSION);
	dollarFp = fopen(temFileName,"w+");
}
/*
 * Function to open the file for writing
 * Return 0- Success, otherwise -1
 */
int file_open(char *DlfFileName)
{
	int retval = -1, Errno = 0;
	char temFileName[MAX_FILE_NAME]= {'\0'};
	strcpy(temFileName,DlfFileName);
	strcat(temFileName,UNDERBAR);
	sprintf(temFileName + strlen(temFileName),"%03d",DlfFileCounter);
    strcat(temFileName,FILE_EXTENSION);
    retval = GeoLocationfile_open(DlfFileName);/* Open GeoLocation File */
    if(retval){
    	d_warning("GeoLocation File Open failed");
    	exit(-1);
    }
    addHeaderToGEOLocationFile();/* Add the header */
	DlfFd = fopen(temFileName,FILE_OPERATION_MODE);
	if(DlfFd != NULL){
		fileWriteStatus = 1;
		DlfFileCounter++;
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"File open !");
		return 0;
	}else{
		Errno = errno;
		d_warning(" DLF File Open failed, Errno = %d ",Errno);
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"File open error !");
		exit(-1);
	}
}

/*
 * Function to open the Geo LOcation file for writing
 * Return 0- Success, otherwise -1
 */
int GeoLocationfile_open(char *DlfFileName)
{
	int Errno = 0;
	char temFileName[MAX_FILE_NAME]= {'\0'};
	strcpy(temFileName,DlfFileName);
	strcat(temFileName,UNDERBAR);
	sprintf(temFileName + strlen(temFileName),"%03d",GeoLocationFileCounter);
    strncat(temFileName,GeoLocationLteFIleExtension,21);
	GeoLocationFileFd = fopen(temFileName,FILE_OPERATION_MODE);
	if(GeoLocationFileFd != NULL)
	{
		GeoLocationFileCounter++;
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"GeoLocationfile open !");
		return 0;
	}else{
		 Errno = errno;
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"GeoLocationfile open error !");
		d_warning("GeoLocation file open error ,Errno =%d ",Errno);
		return -1;
	}
}

/*
 * Function to make .dlf file full selected path and open it.
 *  Return 0- Success, otherwise -1
 */
int makeDlfFileCompletePath(char *dirpath, char* imei,char* testid,u_int32_t imeilen,u_int32_t testidlen)
{
	/*Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS," Function Hit !");*/
    strcpy(DlfFileName,dirpath);
    strcat(DlfFileName,"/");
    strncat(DlfFileName,imei,imeilen);
    strcat(DlfFileName,UNDERBAR);
    strncat(DlfFileName,testid,testidlen);
    d_log("File Name = %s",DlfFileName);
    open_dollar_file(DlfFileName);
    return file_open(DlfFileName);
}

int InputDlfDataBuff(char *databuff,u_int32_t datalength)
{
   int returnValue = -1;
   pthread_mutex_lock(&fileDlfMutex);
   if(DlfFileWriteStatus)
   {
	   if(DlfFd != NULL)
	   {
		   /*write the input data buff in dlf file */
		   file_write_isf(databuff,datalength);
	   }else if(DlfFd == NULL)
	   {
		   /*Open new file*/
		   returnValue = file_open(DlfFileName);
		   if(!returnValue)
		   {
			   /*write the input data buff in dlf file */
			   file_write_isf(databuff,datalength);
		   }else{
		     /* Error while file open */
			d_warning("DLf File open error1");
			Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"File open error !");

		   }
	   }
   }
   pthread_mutex_unlock(&fileDlfMutex);
   return 0;
}

int DlfFileWriter_startDlfFileWrite( char* imei,char* testid,u_int32_t imeilen,u_int32_t testidlen)
{
	int returnValue = -1;
	DlfFileParameter mDlfFileParameter;
	memset(&mDlfFileParameter,'\0',sizeof(DlfFileParameter));
	/* call function to get the path and size of dlf file */
	returnValue = DlfConfigParameter(&mDlfFileParameter);

	if(returnValue == -1) {
		d_warning("Dlf file configuration parameters wrong : ");
		return returnValue;
	}

	returnValue = -1;

	if ((strlen(testid)+strlen(imei)+strlen(mDlfFileParameter.DirPath)) > (MAX_FILE_NAME - MARGIN))
		return returnValue;

	pthread_mutex_lock(&fileDlfMutex);
	/* This is for setDLFFilePath function */
	if(DLFParentDirPath[0] != '\0') 
		strncpy(mDlfFileParameter.DirPath,DLFParentDirPath,sizeof(DLFParentDirPath));
	DlfFileWriteStatus = 1;
	MaxDlfFileSize = mDlfFileParameter.MaxDlfFileSize;
	returnValue =  makeDlfFileCompletePath(mDlfFileParameter.DirPath,imei,testid,imeilen,testidlen);

	pthread_mutex_unlock(&fileDlfMutex);

	return returnValue;
}

int  DlfFileWriter_stopDlfFileWrite(){
	int returntype = -1;
	pthread_mutex_lock(&fileDlfMutex);
	returntype = stopDlfWrite();
	fprintf(dollarFp, "%d	%u\n", DlfFileCounter-1, total_NumOfBytesWritten);
	returntype = fclose(dollarFp);
	DlfFileWriteStatus = 0;
	DlfFileCounter = 1;
	total_NumOfBytesWritten = 0;
	GeoLocationFileCounter = 1;

	memset(DlfFileName,'\0',MAX_FILE_NAME);
	memset(DLFParentDirPath,'\0',MAX_FILE_NAME);
	pthread_mutex_unlock(&fileDlfMutex);
	return returntype;
}

int DlfFileWriter_numOfDlfFileFormed(){
	 return  (DlfFileCounter-1);
}

int Dlffilewriter_set_DlfGeoLocationInfomation(float Latitude,float Longitude,u_int32_t GPS_Confidence, u_int32_t Data_Position_Confidence)
{
	memset(&mDlfTest_parameter,'\0',sizeof(mDlfTest_parameter));
	mDlfTest_parameter.latitude = Latitude;
	mDlfTest_parameter.longitude = Longitude;
	mDlfTest_parameter.gps_confidence = GPS_Confidence;
	mDlfTest_parameter.data_position_confidence = Data_Position_Confidence;
	return 0;
}



int setDLFFilePath(char *dlfFilePath , unsigned int length){

	VALIDATE_CONFIG_PARAMETER(dlfFilePath,length,SIZE_OF_TOKEN_BUFFER,"DLF File");
	if(isDirPresent(dlfFilePath)){
		d_warning ("Directory not exist : %s",dlfFilePath);
		return -1;
	}
	memset(DLFParentDirPath,'\0',MAX_FILE_NAME);
	strncpy(DLFParentDirPath,dlfFilePath,length);
	return 0;
}


#if 0
void main ()
{

	char T2[] = {0xd8,0xb8,0x39,0xe5,0x2f,0x54,0x66,0x0};
	ConvertTimeIntoSecond(T2);
	/*char T2[] = {0x81, 0x5e, 0x76, 0x51, 0xcb, 0xaa, 0x65, 0x00};
	Dlffilewriter_set_DlfGeoLocationInfomation(0,345.1234,100,3);
	GeoLocationfile_open("/home/ravi/Desktop/IMSI12345_TESTID001");
	addHeaderToGEOLocationFile();
	GeoLocation_file_write(T2);
	GeoLocation_file_write(T2);
	GeoLocation_file_write(T2);
	stopGeoLocationFile();*/
}
#endif
