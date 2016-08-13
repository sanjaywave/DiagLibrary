#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>         
#include <errno.h>
#include <unistd.h>
#include "errors.h"
#include "filewriter.h"

#define LOG_FILE_TYPE ".txt"
#define MAX_FILE_SIZE 5000000  /*(~100KB FILE)*/
static char ChangedDmLogPath[SIZE_OF_TOKEN_BUFFER] = {'\0'};

pthread_mutex_t logging_mutex;

int dm_initialise_logfile_mutex()
{
	return pthread_mutex_init(&logging_mutex, NULL);
}

void dm_destroy_logfile_mutex()
{
	pthread_mutex_destroy(&logging_mutex);
}

static unsigned int current_file_size = 0;
static unsigned int LogFileCounter = 1;
bool make_new_file= false;

FILE *filefdInternal = NULL;
char LogFileName[MAX_FILE_NAME] ={'\0'};
int  LogFileCreatedFirstTime = 1;

void dm_log (const char *file,int line, const char *func, int level, int domain, const char *format, ...)
{
    va_list args;
    char *message = NULL;
    int n;
    const char *prefix = "info";
    dm_return_if_fail (format != NULL);
    dm_return_if_fail (format[0] != '\0');
    /* level & domain ignored for now */

    if (getenv ("DM_DEBUG") == NULL)
        return;

    va_start (args, format);
    n = vasprintf (&message, format, args);
    va_end (args);

    if (level & DM_LOGL_ERR)
        prefix = "err";
    else if (level & DM_LOGL_WARN)
        prefix = "warn";
    else if (level & DM_LOGL_DEBUG)
        prefix = "dbg";

    if (n >= 0) {
        fprintf (stderr, "<%s> [%s:%u] %s(): %s\n", prefix, file, line, func, message);
        FREE (message);
    }
}

void Log_Trace(const char *file,int line,const char *func, int level,const char *format, ...){
    va_list args;
    char *message = NULL;
    int n;
    const char *prefix = "Info";
     va_start (args, format);
    n = vasprintf (&message, format, args);
    va_end (args);
    if (level & LOGL_ERR)
        prefix = "Error";
    else if (level & LOGL_WARN)
        prefix = "Warning";
    else if (level & LOGL_DEBUG)
        prefix = "Debug";
    else if(level & LOGL_SUCCESS)
    	 prefix = "Success";
    if (n >= 0) {
        fprintf (stderr, "< %s > [FileName = %s : Function = %s() : Line no = %u]: %s\n", prefix, file, func,line,message);
        FREE (message);
    }
}

/* checks wether directory specified by path exist */
int isDirPresent(char *path) {
	struct stat s;
	int err = stat((const char *)path, &s);
	if(-1 == err) {
		if(ENOENT == errno) {
			/* does not exist */
			return -1;
		}
	} else {
		if(S_ISDIR(s.st_mode)) {
			/* it's a dir */
		} else {
			/* exists but is no dir */
			return -1;
		}
	}
	return 0;
}


void makeNewfile(unsigned short count){
   current_file_size += count;
   if(current_file_size > MAX_FILE_SIZE){
      closeLog(); /* close the file writing*/
      enableLog(); /* make a new file */
   }
}

void d_my_log(const char *format, ...)
{
   va_list arg;
   va_start (arg, format);
   if(filefdInternal == NULL)
   	vprintf(format, arg);
   else
  		vfprintf(filefdInternal,format, arg);
   va_end (arg);
}

void d_log_func(const char *format, ...)
{
	u_int32_t count = 0;
	va_list arg;
	pthread_mutex_lock(&logging_mutex);
    va_start (arg, format);
    if(filefdInternal == NULL)
   	vprintf(format, arg);
   else
	   count = vfprintf(filefdInternal,format, arg);
   va_end (arg);
   makeNewfile(count);
   pthread_mutex_unlock(&logging_mutex);

}

void d_arraylog_func(char *buf , int size)
{
  	u_int32_t i;
  	d_log(" \n");
  	for (i = 0; i<size ;i++){
  		d_log_func(" %2x " , buf[i]);
        if (i%16 == 0 && i!= 0)
        	d_log_func("\n");
   	}
  	d_log_func("\n");
}

void
d_flush()
{
	pthread_mutex_lock(&logging_mutex);
	fflush(filefdInternal);
	pthread_mutex_unlock(&logging_mutex);
}

void d_warning_func(const char *format, ...)
{
   u_int32_t count = 0;
   va_list arg;
   pthread_mutex_lock(&logging_mutex);
   va_start (arg, format);
   if(filefdInternal == NULL)
   	vprintf(format, arg);
   else {
	   count = vfprintf(filefdInternal,format, arg);
//#ifdef MEMORY_CORRUPTION_REPORT
	   fflush(filefdInternal);
//#endif //MEMORY_CORRUPTION_REPORT
   }
   va_end (arg);
   makeNewfile(count);
   pthread_mutex_unlock(&logging_mutex);
}

int enableLog(){
  time_t t;
  time(&t);
  DebugFileParameter  debugFileParameter;
  current_file_size = 0;
  char tempFileName[MAX_FILE_NAME] = {'\0'};
  if(LogFileCreatedFirstTime)
  {
	  memset(LogFileName,'\0',sizeof(LogFileName));
	  /* This section will execute first time only */
	  if(LogFileParameter(&debugFileParameter))
	  {
		  printf ("LogFile path not found in Config.txt file");
		  return -1;
	  }
	  strcpy(LogFileName,debugFileParameter.LogFilePath);
	  strcat(LogFileName,"/");
	   strcat(LogFileName,"Log");
	  LogFileCreatedFirstTime = 0;
  }else if(ChangedDmLogPath[0] != '\0') {
	  memset(LogFileName,'\0',sizeof(LogFileName));
	  strcpy(LogFileName,ChangedDmLogPath);
	  strcat(LogFileName,"/");
	   strcat(LogFileName,"Log");
	  LogFileCounter = 1;
  }

  strcpy(tempFileName,LogFileName); 
  sprintf(tempFileName+strlen(LogFileName),"%03d",LogFileCounter);
  strcat(tempFileName,LOG_FILE_TYPE);
  printf("\nLogFile = %s",tempFileName);
  filefdInternal =fopen(tempFileName,"a");
  if (filefdInternal == NULL)
  {
	  Log_Trace (__FILE__,__LINE__,__func__,LOGL_ERR,"Error while making Log file");
	  exit(-1);
  }
  printf("\nLogfile created");
  fprintf(filefdInternal,"\n //************************************************************// ");
  fprintf(filefdInternal,"\nDate:-%s",ctime(&t));
  fflush(filefdInternal);
  LogFileCounter++;
  return 0;
}

int closeLog()
{
  int fclose_status=-1;
  if(filefdInternal != NULL)
	  fclose_status=fclose(filefdInternal);
  if (fclose_status)
   	return -1;
  else
     	return 0;
}

/*
 *This function is added to close the LOG file when CloseDm is called,
 * So in this siutation we have to wait for file writing and then will close
 */
int closeDMLogFile(){
	int fclose_status=-1;
	LogFileCreatedFirstTime = 1;
	pthread_mutex_lock(&logging_mutex);
	if(filefdInternal != NULL)
		fclose_status=fclose(filefdInternal);
	pthread_mutex_unlock(&logging_mutex);
	if (fclose_status)
		return -1;
	else
		return 0;
}
	
int setDebugFilePath(char *debugFilePath ,unsigned int length){

	VALIDATE_CONFIG_PARAMETER(debugFilePath,length,SIZE_OF_TOKEN_BUFFER,"DM LOG");
	if(isDirPresent(debugFilePath)){
		d_warning ("Directory not exist : %s",debugFilePath);
		return -1;
	} 
	pthread_mutex_lock(&logging_mutex);
	closeLog();
	memset(ChangedDmLogPath,'\0',sizeof(ChangedDmLogPath));
	strncpy(ChangedDmLogPath,debugFilePath,length);
	enableLog();
	pthread_mutex_unlock(&logging_mutex);
	return 0;

}

