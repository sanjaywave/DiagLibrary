#ifndef DLFINTERFACEAPI_H_
#define DLFINTERFACEAPI_H_


enum {
		DLF_GSM = 1,
        DLF_WCDMA = 2,
        DLF_LTE = 3,
        DLF_CDMA = 4,

};
enum
{
	LTE_LOG_DISABLE_ALL= 0x00000000,

	LTE_LOG_ENABLE_ALL= 0xFFFFFFFF,

	LTE_MANAGEMENT_LAYER_LOG = 0x00000001,

	LTE_MAC_LAYER_LOG = 0x00000002,

	LTE_RLC_LAYER_LOG = 0x00000004,

	LTE_PDCP_LAYER_LOG = 0x00000008,

	LTE_RRC_LAYER_LOG = 0x00000010,

	LTE_NAS_LAYER_LOG = 0x00000020,

	LTE_CALL_LAYER_LOG = 0x00000040,

	LTE_EVENT = 0x00000080,

	LTE_LOWER_LAYER_1 = 0x00000200,

  	OTHER= 0x00000100,/* For testing purpose */
};
enum
{
      WCDMA_LOG_DISABLE_ALL = 0x00000000,
      
      WCDMA_LOG_ENABLE_ALL = 0xFFFFFFFF,
     
      WCDMA_LAYER_1_LOG = 0x00000001,

      WCDMA_LAYER_2_LOG = 0x00000002,

      WCDMA_LAYER_3_LOG = 0x00000004,

      HSPDA_LAYER_1_LOG = 0x00000008,

      HSPDA_MAC_LAYER_LOG = 0x00000010,
    
      HSUPA_LAYER_1_LOG = 0x00000020,

      HSUPA_MAC_LAYER_LOG = 0x00000040,
      
      UTMS_NAS_LAYER_LOG = 0x00000080,

      WCDMA_GPS_LOG = 0x00000100,
};
enum
{
      GSM_LOG_DISABLE_ALL =0x00000000,
      
      GSM_LOG_ENABLE_ALL =0xFFFFFFFF,
     
      GSM_LAYER_1_LOG = 0x00000001,

      GSM_LAYER_2_LOG = 0x00000002,
      
      GSM_LAYER_3_LOG  = 0x00000004,

      GSM_CS_data_LOG  = 0x00000008,

      GSM_PHY_MDSP_IQ_LOG = 0x0000010,

      GSM_inter_RAT_LOG  = 0x00000020,

      GSM_NAS_LAYER_LOG = 0x00000040,

      GPRS_GRR_LAYER_LOG  = 0x00000080,

      GPRS_RLC_UL_LAYER_LOG  = 0x00000100,

      GPRS_RLC_DL_LAYER_LOG  = 0x00000200,

      GPRS_LLC_LAYER_LOG = 0x00000400,

      GPRS_SNDCP_LAYER_LOG = 0x00000800,

      GPRS_MAC_LAYER_LOG = 0x00001000,

      GPRS_LAYER_1_LOG = 0x00002000,

};
enum
{    
      GPS_LOG_DISABLE_ALL =0x00000000,
      
      GPS_LOG_ENABLE_ALL =0xFFFFFFFF,
     
      GPS_LAYER_1_LOG = 0x00000001,

      GPS_NAS_LAYER_LOG = 0x00000002,
};

//add
enum
{
	CDMA_LOG_DISABLE_ALL =0x00000000,

	CDMA_LOG_ENABLE_ALL =0xFFFFFFFF,

	CDMA_LOG_PACKET_DATA =0x00000001
}; //add
/*
 * Function to enable layer wise logging
 * Return 0 success  ,(-1) failed
 * This API only be used whenever , only ISF file has been initiated. 
 * If ISF and DM logging both are being used by application than don't use this AP
 * Dont call this API , DM is already initialised
 */

int openISFDM(int devicetype, int systype , char * configFilePath);

/*
 *This API is used only for the standalone mode (ISF mode).
 * Return 0
 */

int closeISFDM();

/*
 * Function to get  number of Dlf file formed during this test
 * Return :- No of DLF file formed
 */
int  numOfDlfFileFormed();

/*
 * Function to stop Dlf File Writing
 * Return  0 - Success, 1 - failed
 */

int stopDlfFileWrite();

/*
 * Function to start Dlf file Writing
 * Arg :- imeiNumber, testid, imeiNumberLength, testidLength, layerType(from above defined emun)
 * Return  0 - Success, -1 failed
 */
int startDlfFileWrite( char* imei,char* testid,u_int32_t imeilen,u_int32_t testidlen);

/*
 * This function is used to set the Logging for the LTE layer
 * The parameters are defined in enum
 * 
 */
int setLTELogMask(int layerType);
/* This function set to configure for the WCDMA
 * The below function is implemented. Will be provide in next release
 */
int setWCDMALogMask(int layerType);
/*
 * This function set to configure for the GSM
 *  Return  0 - Success, -1 failed
 */
int setGSMLogMask(int layerType);
/*
   This function set to configure for the GPS
*/
int setGPSLogMask(int layerType);

/*
 *  This function set the various .axf file parameter. Call this function before startDlfFileWrite()
 *  Return 0- Sucess, -1 failed
 */

int setCDMALogMask(int layerType);
/*
 *  This function set the various .axf file parameter. Call this function before startDlfFileWrite()
 *  Return 0- Sucess, -1 failed
 */
int  set_DlfGeoLocationInfomation(float Latitude,float Longitude,u_int32_t GPS_Confidence, u_int32_t Data_Position_Confidence);

#endif /* DLFINTERFACEAPI_H_ */
