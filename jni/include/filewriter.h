#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#define MAX_FILE_NAME 550
#define MAX_DIR_PATH_LENGTH 360
#define MAX_GUIDE_BUFFER_SIZE   50
#define GEO_LOCATION_BUFFER_SIZE 550
#define SIZE_OF_TOKEN_BUFFER 360
#define MARGIN 50
#define DEFAULT_FILE_SIZE 500000
#define TIME_STAMP 8

struct Test_parameter
{
	u_int8_t guidInfo[MAX_GUIDE_BUFFER_SIZE];
	u_int8_t  timestamp[TIME_STAMP];
	u_int32_t Guidlength;
	double  lat;
	double  longitude;
	double  altitude;
	u_int32_t speed;
	u_int32_t direction;
	u_int32_t locationAccurancy;
	u_int16_t cmdCode;
	u_int32_t length;
}__attribute__ ((packed));
typedef struct Test_parameter Test_parameter;




typedef struct DlfFileParameter{
   u_int8_t DirPath[MAX_FILE_NAME];
   u_int32_t  MaxDlfFileSize;
}DlfFileParameter;

typedef  struct  DebugFileParameter
{
	u_int8_t LogFilePath [MAX_FILE_NAME];

}DebugFileParameter;

int  stop_filewriter();
int  filewriter_start_file_write(char* imsi,char* testid,int imsilen,int testidlen );
int  filewriter_stop_file_write();
int  open_config_file();
int  filewriter_set_GUIDInformation(char *guidInfo, int guidInfolen);
int  filewriter_set_GeoLocationInfomation (double lat,double longitude,double altitude,
		int speed,int direction,int locationAccurancy);

int DlfConfigParameter(DlfFileParameter *DlfFileParameter);
int LogConfigParameter(DebugFileParameter *debugFileParameter);

struct cmdTimestamp{
	u_int64_t  unused : 16;
	u_int64_t  counter : 48;
}__attribute__ ((packed));
typedef struct  cmdTimestamp  cmdTimestamp;

#endif /* FILEWRITER_H_ */
