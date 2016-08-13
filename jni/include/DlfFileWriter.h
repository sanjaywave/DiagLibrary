#ifndef DLFFILEWRITER_H_
# define DLFFILEWRITER_H_
#include <sys/types.h>

# define UNDERBAR "_"
# define FILE_EXTENSION ".dlf"
# define FILE_OPERATION_MODE   "wb+"

struct DlfTest_parameter
{
	float      latitude;
	float      longitude;
	u_int32_t  gps_confidence;
	u_int32_t  data_position_confidence;
}__attribute__ ((packed));
typedef struct DlfTest_parameter DlfTest_parameter;

void is_DLF_allowed(u_int32_t DLFState);
int InputDlfDataBuff(char *databuff,u_int32_t datalength);
int DlfFileWriter_startDlfFileWrite( char* imei,char* testid,u_int32_t imeilen,u_int32_t testidlen);
int  DlfFileWriter_stopDlfFileWrite() ;
int Dlffilewriter_set_DlfGeoLocationInfomation(float Latitude,float Longitude,u_int32_t GPS_Confidence, u_int32_t Data_Position_Confidence);
#endif /* DLFFILEWRITER_H_ */
