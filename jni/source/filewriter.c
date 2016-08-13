# include <stdio.h>
# include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include "filewriter.h"
#include "errors.h"
#include "extmsgid.h"

#define SIZE_OF_IMEI  16
#define SIZE_OF_TESTID 10
#define UNDERBAR "_"
#define TXT_FILE_EXTENSION ".txt"
#define TXT_FILE_OPERATION_MODE  "a"
#define FILE_OPERATION_MODE   "wb+"

static u_int64_t NiltimeDiff = 0;
extern pthread_mutex_t fileReleaseMutex;

static char DebugFilePath[SIZE_OF_TOKEN_BUFFER] = {'\0'};
static char g_ParentDirPath[SIZE_OF_TOKEN_BUFFER] = {'\0'};
static char CurrentTxtFileFullName[MAX_FILE_NAME] = {'\0'};
static char TxtFileName[MAX_FILE_NAME] = {'\0'};
static u_int32_t  g_fileSize;
static u_int32_t g_DlfFileSize = 0;
FILE *g_file_des = NULL;
static bool g_fileWriteStatus = false;
static int  numOfBytesinFile=0;
static bool openconfigFile= false;
static int TxtFileCounter=1;
static Test_parameter test_parameter;
u_int64_t TotalTestCaseBytes = 0;
static isNewTest = 0;

//extern unsigned long long getTimeDiff();

int NILDiffTime(char *buff, unsigned int packetlength);


void getParaValues(Test_parameter *para)
{
	memcpy(para, &test_parameter, sizeof(Test_parameter));
}


/* Function to extract the value for key value pair */
int get_valuefromKey(char *inputBuffer , char *Key , char *tokenValue)
{
if( strstr(inputBuffer,Key)!= NULL)
   {
      int tokensize;
      char *value;
      int bufferSize;
      tokensize = strlen(Key);
      bufferSize = strlen(inputBuffer);
      value= strncpy(tokenValue,inputBuffer+tokensize+2,bufferSize-(tokensize+4));
      return strlen(tokenValue);
   }
   else return -1;
}


/* Function to open the config.txt file and get the key value pair */
int open_config_file(char *filename){
  d_warning("\nFunction : open_config_file");
  int Errno = 0;
  char inputBuffer[SIZE_OF_TOKEN_BUFFER]= {'\0'};
  char tokenValue [SIZE_OF_TOKEN_BUFFER] = {'\0'} ;
  int tokenvalueSize;
  FILE *cfp=NULL;
  memset(g_ParentDirPath,'\0',sizeof(g_ParentDirPath));
  cfp=fopen(filename,"r");
  if (cfp == 0){
	 Errno = errno;
	 printf("\nUnable to open Config.txt file,Configuring default , Errno = %d",errno);
     strcpy(g_ParentDirPath,"/mnt/sdcard/");
     printf("\nFilePath=%s",g_ParentDirPath);
     g_fileSize = DEFAULT_FILE_SIZE;
     printf("\nFileSize=%d",g_fileSize);
     g_DlfFileSize = DEFAULT_FILE_SIZE;
     strcpy(DebugFilePath,"/mnt/sdcard/");
     return 0;
  }
  while (fgets(inputBuffer,SIZE_OF_TOKEN_BUFFER,cfp)!=NULL)
  {
	  tokenvalueSize=get_valuefromKey(inputBuffer,"Path",tokenValue);
	  if(tokenvalueSize > 0)
	  {
		  strncpy(g_ParentDirPath,tokenValue,tokenvalueSize);
		  memset(inputBuffer,0,sizeof(inputBuffer));
		  memset(tokenValue,0,sizeof(tokenValue));
		  printf("\n FilePath = %s",g_ParentDirPath);
	  }
	  tokenvalueSize=get_valuefromKey(inputBuffer,"Log",tokenValue);
	  if(tokenvalueSize>0)
	  {
		 strncpy(DebugFilePath,tokenValue,tokenvalueSize);
		 memset(inputBuffer,0,sizeof(inputBuffer));
		 memset(tokenValue,0,sizeof(tokenValue));
		 printf("\n DebugLog = %s",DebugFilePath);
	  }
	  tokenvalueSize=get_valuefromKey(inputBuffer,"Size",tokenValue);
	  if(tokenvalueSize>0)
	  {
		  g_fileSize = atoi(tokenValue);
		  memset(inputBuffer,0,sizeof(inputBuffer));
		  memset(tokenValue,0,sizeof(tokenValue));
		  printf("\n FileSize = %d",g_fileSize);
	  }
	  tokenvalueSize=get_valuefromKey(inputBuffer,"DlfFile",tokenValue);
	  if(tokenvalueSize>0)
	  {
		 g_DlfFileSize = atoi(tokenValue);
		 memset(inputBuffer,0,sizeof(inputBuffer));
		 memset(tokenValue,0,sizeof(tokenValue));
		 printf("\n DlfFileSize = %d",g_DlfFileSize);
	  }
 }
  fclose(cfp);
  printf("\nlength of %d",strlen(g_ParentDirPath));
  if((g_ParentDirPath[SIZE_OF_TOKEN_BUFFER-1] !='\0') || (DebugFilePath[SIZE_OF_TOKEN_BUFFER-1] != '\0')){
	  printf("\nConfiguration file's Path or Log parameter is longer than as expected ");
	  exit(-1);
  }
  return 0;
}

/* Function to create the file */
int open_file()
{
	int Errno = 0;
	d_warning("\nFunction : open_file");
	d_warning("\n Value in CurrentTxtFileFullName before memset : %s", CurrentTxtFileFullName);
	memset(CurrentTxtFileFullName,'\0',sizeof(CurrentTxtFileFullName));
	d_warning("\nValue in CurrentTxtFileFullName after memset = %s",CurrentTxtFileFullName);
	d_warning("\nTxtFileName value = %s",TxtFileName);
  	strcpy(CurrentTxtFileFullName,TxtFileName);
  	strcat(CurrentTxtFileFullName,UNDERBAR);
  	sprintf(CurrentTxtFileFullName + strlen(CurrentTxtFileFullName),"%03d",TxtFileCounter);
    strcat(CurrentTxtFileFullName,TXT_FILE_EXTENSION);
    d_warning("\nComplete FilePath = %s",CurrentTxtFileFullName);
    g_file_des = fopen(CurrentTxtFileFullName,FILE_OPERATION_MODE);
  	if(g_file_des != NULL){
  		TxtFileCounter++;
		d_warning("\n Normal End of function : open_file");
  		return 0;
  	}else{
  		Errno = errno;
  		d_warning("\n Unable to Open the File = %s, Errno = %d ",CurrentTxtFileFullName,Errno);
		d_warning("\n Errorneous End of function: open_file");
  		exit(-1);
  }
}


/* Function to write the data in required format in the file.*/
int file_write(u_int16_t command_code,char *time,int timelen,unsigned char *buf,int length)
{
	d_warning("\n Function: file_write");

	d_warning("\n Writing  in file : %s", CurrentTxtFileFullName);
	u_int32_t fwriteReturn = 0;
	u_int32_t NumOfBytesWritten = 0;
	/*struct timeval enter;*/
	/*struct timeval exit;*/
	/*int rc;*/
	/* rc=gettimeofday(&enter, NULL);*/
	//d_warning("\n Command length %d \n",length);

	if((buf != NULL) && (time !=NULL) && length){
		test_parameter.cmdCode = command_code ;
		test_parameter.length = (unsigned int)length;
		if(timelen != TIME_STAMP)
		memset(&(test_parameter.timestamp[0]),0,sizeof(test_parameter.timestamp));
		memcpy(&(test_parameter.timestamp),time,timelen);

		fwriteReturn = fwrite(&test_parameter, sizeof(char), sizeof(test_parameter), g_file_des);
		if(fwriteReturn != sizeof(test_parameter))
		{
			Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"Test_parameter structure error");
		}
		fwriteReturn = fwrite(buf, sizeof(char), length, g_file_des);
		if(fwriteReturn != length){
			Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"No of bytes drops = %d,Error = %d,EOF = %d",(sizeof(test_parameter)-fwriteReturn),ferror(g_file_des),feof(g_file_des));
		}
		NumOfBytesWritten = (length + sizeof(test_parameter));
		/*rc=gettimeofday(&exit, NULL);*/
	   // d_my_log("\nfile_write Diff sec = %d  %d \n",(exit.tv_sec - enter.tv_sec) ,(exit.tv_usec - enter.tv_usec));
	}
	d_warning("\n End of Function : file_write");
	return NumOfBytesWritten;

}

/* Function to close the file */
int close_file(FILE *fp)
{
	d_warning("\n function : close_file ");
 	int file_status=-1;
  	file_status=fclose(fp);
	d_warning("\n Closing file : %s ", CurrentTxtFileFullName);
	d_warning("\n End of Function: close_file");
  return file_status;
}


int NILDiffTime(char *buff, unsigned int packetlength)
{
	u_int64_t cmdmillSeconds = 0, sysmilisecond = 0;
	int retval = -1;
	//int i = 0;
	struct timeval enter;
	NiltimeDiff = 0; /* Reset NilTimeDiff */
	//printf("\nRecevied");
	//for(i = 0 ; i < packetlength; i++) printf (" %2x", buff[i]);
	cmdTimestamp *cts = (cmdTimestamp *)&buff[0];
	//printf("\nCounter = %llx ", cts->counter);
	cmdmillSeconds = ((unsigned long long)cts->counter*125)/100; /* This time is in millisecond */
	/* Get the system time */
	retval = gettimeofday(&enter, NULL);
	if(!retval){
		sysmilisecond = ((unsigned long long)enter.tv_sec*1000) + (enter.tv_usec/1000);
		NiltimeDiff = sysmilisecond - cmdmillSeconds;
		//printf("TIME DIFF = %llu", NiltimeDiff);
		retval = 0 ;
	}else{
		d_warning ("\nDmTimeResponse : GetSystem time failed");
		retval = -1;
	}
	return retval;
}

/* function to write the buffer in file */
int  buffer_to_file_write(char *timebuff,int timelength,u_int16_t command_code,char *buff, int len)
{
	d_warning("\n Function : buffer_to_file_write");
	int i = 0 ;
	unsigned int outerCmd = 0;
	outerCmd = convert_into_external_code(command_code);
	if(outerCmd == 0){
	/* No need to write this command in file */
		return ;
	}
	if(g_fileWriteStatus){
	  /* lock the mutex */
	  pthread_mutex_lock(&fileReleaseMutex);
	  if (g_file_des !=NULL){
		  d_warning("\n buffer_to_file_write : case 1 write to file");
		  /* file writing is already started.so write the data byte in existing file */
		  numOfBytesinFile += file_write(outerCmd,timebuff,timelength,buff,len);
		  if(g_fileSize != 0) {
			  if(numOfBytesinFile > g_fileSize){
				  TotalTestCaseBytes += numOfBytesinFile;
				  close_file(g_file_des);
				  g_file_des=NULL;
				  numOfBytesinFile = 0;
			  }
		  }
	  }
	  else if (g_file_des == NULL)
	  {
		  /* condition when file writing is just started,so creat a new file to write */
		  d_warning("\n buffer_to_file_write : case 2 open new file and  write to  file");
		  if (open_file()==-1){
			  pthread_mutex_unlock(&fileReleaseMutex);
			  return -1;
		  }
		  if(isNewTest)
		  {
			      /* Function to calculate the constant NilServer time diff */
				  d_warning("\n Start of New Test");
			  	  NILDiffTime(timebuff, timelength);
		  		  numOfBytesinFile += file_write(SYSTEM_TIME_CMD,timebuff,timelength,(unsigned char *)&NiltimeDiff,sizeof(NiltimeDiff));
		  		  isNewTest = 0;
		  }
		  d_warning("\n Continued test");
		  numOfBytesinFile += file_write(outerCmd,timebuff,timelength,buff,len);
		  if(g_fileSize != 0) {
			  if(numOfBytesinFile > g_fileSize)
			  {
				  TotalTestCaseBytes += numOfBytesinFile;
				  close_file(g_file_des);
				  g_file_des = NULL;
				  numOfBytesinFile = 0;
			  }
		  }
	  }
	  /*Unlock the mutex */
	  pthread_mutex_unlock(&fileReleaseMutex);
  }
	d_warning("\n End of Function: buffer_to_file_write");
}


int stop_filewriter()
{
		d_warning("\n function : stop_filewriter");
		int close_status=-1;
		char lastfile[MAX_FILE_NAME]={'\0'};
		d_warning("\n Length of CurrentTxtFileFullName: %d",strlen(CurrentTxtFileFullName));
		d_warning(" \n Value in CurretTxtFileName : %s \n ", CurrentTxtFileFullName);	
		
		if(g_fileWriteStatus && (strlen(CurrentTxtFileFullName) > 0))
		{
			/* close the current txt file */
			if(g_file_des)
			{
				d_warning("\n Closing the current Text File");
				TotalTestCaseBytes += numOfBytesinFile;
				close_status =close_file(g_file_des);
				d_warning("\nClose File Status = %d",close_status);
				if(close_status!=0)
				{
					return -1;
				}
			}
			d_warning("\n Opening $ file");
			/* Open the $ file */
			if (open_file() == -1)
				 return -1;
			/* Write the total number of bytes for this test case in $ file */
			d_warning("\n TotalTestCaseBytes = %llu",TotalTestCaseBytes);
			TotalTestCaseBytes += sizeof(u_int64_t);
			fwrite(&TotalTestCaseBytes, sizeof(char),8, g_file_des);
			/* Append the $ at the end of file name */
			strncpy(lastfile,CurrentTxtFileFullName,strlen(CurrentTxtFileFullName)-4);
			d_warning("\nLast FileName = %s",lastfile);
			strcat(lastfile,"$");
			strcat(lastfile,TXT_FILE_EXTENSION);
			if(g_file_des)
			{
				close_status =close_file(g_file_des);
				d_warning("\nClose File Status of $ file = %d",close_status);
				if(close_status!=0)
				{
						return -1;
				}
			}
			if(rename(CurrentTxtFileFullName,lastfile) == 0)
			{
				d_warning("\n Current Txt File Name = %s", CurrentTxtFileFullName);
				d_warning("\nLast File=%s",lastfile);
			}
			else
			{
				d_warning("Last file renaming error");
			}
			TxtFileCounter = 1;
			g_file_des=NULL;
			numOfBytesinFile=0;
			TotalTestCaseBytes = 0;
	}
	d_warning("\n End of function: stop_filewriter");
	 return  0;
}


int  filewriter_stop_file_write(){

  d_warning("\n function : filewriter_stop_file_write");
  int retval=-1;
  /* lock the mutex */
  pthread_mutex_lock(&fileReleaseMutex);
  d_warning("\n Calling function stop_filewriter");
  retval = stop_filewriter();
  d_warning("\n File Name before resetting TXTFileName : %s \n", TxtFileName);
  memset(TxtFileName,'\0',MAX_FILE_NAME);

  d_warning("\n File Name after resetting TXTFileName : %s \n", TxtFileName);

  isNewTest = 1;
  g_fileWriteStatus=false;
  /* unlock the mutex */
  pthread_mutex_unlock(&fileReleaseMutex);
  d_warning("\n End of function : filewriter_stop_file_write");
  return retval;
}


int  filewriter_start_file_write(char* imei,char* testid,int imeilen,int testidlen )
{
   d_warning("\n function : filewriter_start_file_write");
   if((strrchr(imei,'_') != NULL) || ( strrchr(testid,'_') != NULL )){
	   /* Imei or Testid contains UNDERBAR */
	d_warning("\n IMEI and Testid  contains UNDERBAR");
		  return -1;
   }
   if((strlen(g_ParentDirPath) + imeilen + testidlen ) > MAX_FILE_NAME + MARGIN){
	   d_warning("Txt file directory path longer than expected ");
	   return -1;
   }
   pthread_mutex_lock(&fileReleaseMutex);
   isNewTest = 1;
   g_fileWriteStatus=true;
   strcpy(TxtFileName,g_ParentDirPath);
   strcat(TxtFileName,"/");
   strncat(TxtFileName,imei,imeilen);
   strcat(TxtFileName,UNDERBAR);
   strncat(TxtFileName,testid,testidlen);
   d_warning("\n Start File Write Name TxtFileName = %s\n",TxtFileName);
   pthread_mutex_unlock(&fileReleaseMutex);
   d_warning("\n End of function: filewriter_start_file_write");
   return 0;
}

int  filewriter_set_GUIDInformation(char *guidInfo, int guidInfolen){
	/* Guide info length should not be greater than 50 */
	if(guidInfolen > MAX_GUIDE_BUFFER_SIZE-1)
		return -1;
	memset(&test_parameter,0,sizeof(test_parameter));
	memcpy(&test_parameter.guidInfo,guidInfo,guidInfolen);
	test_parameter.Guidlength=guidInfolen;
	return 0;
}

int  filewriter_set_GeoLocationInfomation (double lat,double longitude,double altitude,
		int speed,int direction,int locationAccurancy)
{
	test_parameter.lat=lat;
	test_parameter.longitude=longitude;
	test_parameter.altitude=altitude;
	test_parameter.speed=speed;
	test_parameter.direction=direction;
	test_parameter.locationAccurancy=locationAccurancy;
	return 0;
}


int DlfConfigParameter(DlfFileParameter *DlfFileParameter)
{
	int retval =-1;
	if(g_ParentDirPath[0] != '\0')
	{
		strcpy(DlfFileParameter->DirPath,g_ParentDirPath);
		DlfFileParameter->MaxDlfFileSize = g_DlfFileSize ;
		if(!(strcmp(DlfFileParameter->DirPath,g_ParentDirPath)))
			retval =0;
	 }
	return retval;
}

int LogFileParameter(DebugFileParameter *debugFileParameter){
	int retval =-1;
	if(DebugFilePath[0] != '\0' && (strlen(DebugFilePath) <  SIZE_OF_TOKEN_BUFFER)){
	    strcpy(debugFileParameter->LogFilePath,DebugFilePath);
		if(!(strcmp(debugFileParameter->LogFilePath,DebugFilePath)))
			retval = 0;
	}else{
		d_warning(" Log File path is longer than expected ");
	}
	return retval;
}

int setTXTFilePath(char *txtFilePath ,unsigned int length){

	VALIDATE_CONFIG_PARAMETER(txtFilePath,length,SIZE_OF_TOKEN_BUFFER,"TXT File");
	if(isDirPresent(txtFilePath)){
		d_warning ("Directory not exist : %s",txtFilePath);
		return -1;
	}
	memset(g_ParentDirPath,'\0',sizeof(g_ParentDirPath));
	strncpy(g_ParentDirPath,txtFilePath,length);
	return 0;
}


#if 0
void main ()
{
    /*char  timebuff[] = {0x07,0x08};
    char buff[]={0x0,0x1,0x2,0x2,0xe,0xf};
    open_config_file("/home/ravi/c_pro/config.txt");
    set_GUIDInformation("ABCDEFGHI",9);
    set_GeoLocationInfomation (1.1,1.2,1.3,100,4,5);
    filewriter_start_file_write("TXIEST","TEST001",6,7);
    buffer_to_file_write(timebuff,2,0x51FA,buff,6);*/
	filewriter_set_GeoLocationInfomation (123.1111,-123.333,22.3232323,
			120,121,100);
}
#endif

