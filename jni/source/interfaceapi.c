#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "interfaceapi.h"
#include "errors.h"
#include "filewriter.h"
#include "decode.h"
#include "version.h"
#include "diag-cmd.h"
#include "ipcQueue.h"
#include "CdmaContext.h"
#include "internalEvent.h"

#define GSM_CMD  1
#define WCDMA_CMD 2
#define NAS_CMD 3
#define EVENT_CMD 4
#define LTE_CMD 5
#define CDMA_CMD 6

#define SIZE_OF_IMEI 15
#define SIZE_OF_TESTID 10
#define SYSTEM_SIZE 5
#define MAX_LOGGING_ITEM_COUNT 64

static int gEventMaskInternal;
int globalFD;
int init_done = 0;
int gEventMask;
int gLogMaskGSM;
int gLogMaskWCDMA;
int gLogMaskLTE;
bool IsGprsL2InfoEnable=false;
static bool isCCEventsEnabled = false;


extern int systemTypeDetected;
//extern pthread_mutex_t *watchDog_mutex;
extern short globalUrfcnFreq;
extern pthread_mutex_t logging_mutex;

WCDMACellInfo myCellInfo;
LTECellInfo mLTE_CellInfo;
GSMCellInfo mGSMCellInfo;
Kpi_CDMA mkpi_cdma;
pthread_mutex_t cellInfoMutex;

pthread_mutex_t fileReleaseMutex;
pthread_mutex_t fileDlfMutex;
pthread_mutex_t sychMutex;
pthread_t childthreadID;
pthread_t processthread;
pthread_t cdma_thread;

int gManufactureType ;

int index_event;

static queue_t diag_port_q = NULL;

void insertLargestItem(int index,int type);
void *diag_com_listen(void * arg) ;

struct init_diag{
	int sysType ;
	int logging_mask;
	int event_mask;
	void (*mycallback)(int, int , char * , unsigned int );
};
typedef struct init_diag init_diag;

static init_diag init_diag_info[SYSTEM_SIZE];
static u_int16_t GSM_Logging_item[MAX_LOGGING_ITEM_COUNT];
static u_int16_t WCDMA_Logging_item[MAX_LOGGING_ITEM_COUNT];
static u_int16_t LTE_Logging_item[MAX_LOGGING_ITEM_COUNT];
static u_int16_t NAS_item[MAX_LOGGING_ITEM_COUNT];
static u_int16_t Event_item[MAX_LOGGING_ITEM_COUNT];
static u_int16_t CDMA_Logging_item[MAX_LOGGING_ITEM_COUNT];

int dm_initialise_cellInfoMutex()
{
	return pthread_mutex_init(&cellInfoMutex, NULL);
}

void dm_destroy_cellInfoMutex()
{
	pthread_mutex_destroy(&cellInfoMutex);
}

bool getCCEventEnableStatus() {
	return isCCEventsEnabled;
}


void sig_handler(int signo)
{
	if (signo == SIGALRM) printf("received SIGALRM\n");
}

void *cdma_cmd_thread()
{
	printf("\nRequest thread created \n");
	while (1)
	{
		if (!init_done)
		{
			d_log("Exiting request thread \n");
			return ;
		}
		//print_lte_phy_status();
		//decode_get_state_info();
		if(systemTypeDetected == DETECT_CDMA) {
		//	printf("Sending the 0x19 command \n");
			diag_com_cdma_request(globalFD,0x19);
		}
		sleep(1);

	}
}

/*Function to init the DM */
int 
init_diag_port (unsigned int manufacture, unsigned int type, 
		void (*callback)(int, int , char *, unsigned int),
		char* fileName) 
{
	u_int16_t Disable_item[2];

	int thread_status= 1;
	int processing_thread_status=1;
	int cdma_thread_status = 1;
	int returnType = 0;
	if (type > CDMA)
	{
		returnType = -1;
		return returnType;
	}
	gManufactureType = manufacture;
	init_diag_info[type].sysType = type;
	init_diag_info[type].mycallback = callback;
	
	if(!init_done)
	{
		/*initialise cellinfo mutex*/
		if (dm_initialise_cellInfoMutex()){
			printf("\n cellInfo Mutex initialisation failed");
			exit(-1);
		}
		/* open the config file*/
		open_config_file(fileName);
		if (dm_initialise_logfile_mutex()){
			printf("\nLog File Mutex initialisation failed");
			exit(-1);
	    }
		   pthread_mutex_lock(&logging_mutex);

		enableLog(); /* Generate the log file*/
		   pthread_mutex_unlock(&logging_mutex);

		/*Enabling the signal handler*/
		if (signal(SIGALRM, sig_handler) == SIG_ERR) {
			d_warning("Can't catch SIGALRM\n");
		}
		globalFD=diag_com_open();
		printf("\nGlobal FD %d ",globalFD);
		init_done = 1;

		pthread_mutex_init(&fileReleaseMutex, NULL);
		pthread_mutex_init(&fileDlfMutex, NULL);
		pthread_mutex_init(&sychMutex,NULL);
		diag_port_q = createQueue();

		if (globalFD <= 0) return -1;

		/*Reading Thread*/
		thread_status = pthread_create(&childthreadID, NULL, 
					diag_com_listen, (void *) diag_port_q);
		usleep(20);
		/* packetProcessing Thread*/
		processing_thread_status = pthread_create(&processthread, NULL, 
							packetProcessing, 
									(void *) diag_port_q);
		usleep(20);
#ifdef CDMA_ENABLE
			pthread_create(&cdma_thread , NULL, cdma_cmd_thread,(void *) diag_port_q);
		   usleep(20);

#endif

		int ioctl_status = diag_com_ioctl(globalFD);
		if (ioctl_status >0) {
			if (thread_status) {
				fprintf(stderr, "Error creating thread\n");
			}  else if (thread_status == 0) {
				/*printf("Thread created");*/
			} else {
				printf("Thread error");
			}
		}
		Disable_item[0]=0x1599;
		Disable_item[1]=0x0;
		diag_com_write_command(globalFD,0x01,Disable_item);
		Disable_item[0]=0x5C5C;
		diag_com_write_command(globalFD,0x05,Disable_item);
		Disable_item[0]=0x4910;
		diag_com_write_command(globalFD,0x04,Disable_item);
		Disable_item[0]=0x7507;
		diag_com_write_command(globalFD,0x07,Disable_item);
		Disable_item[0]=0xB902;
		diag_com_write_command(globalFD,0x0B,Disable_item);
		Disable_item[0]=2112;
		Disable_item[1]=0;
		diag_com_write_event(globalFD, Disable_item);
#if 0
		sleep(1);
		/* DM command to get system time */
		Dm_cmd_time_request(globalFD);
		sleep(1);
		Dm_cmd_time_request(globalFD);
#endif

	}
	
	printf("Calling the Setting logging %d",type);
	set_logging_mask(type ,0);
	
	if (type == GSM) {
		memset(&myCellInfo,0,sizeof(mGSMCellInfo));
	} else if (type == WCDMA) {
		memset(&myCellInfo,0,sizeof(myCellInfo));
	} else if (type == LTE) {
		memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
	}else if (type == CDMA){
		memset(&mkpi_cdma,0,sizeof(mkpi_cdma));
	}
	return returnType;

}



/*Function to configure logging mask on device */
int set_logging_mask(int systype, int mask){
  int index_gsm = 0;
  int index_wcdma = 0;
  int index_nas = 0;
  int index_lte = 0;
  int index_cdma = 0;
  int status;
  IsGprsL2InfoEnable=false;
  pthread_mutex_lock(&sychMutex);
  printf("\nEnable Set Logging systemType :%d ,%d \n",systype ,mask);
  if(systype== GSM){
	  gLogMaskGSM = mask;
	  if (mask & LOG_GSM_RR_STATE_MESSAGES){
		    GSM_Logging_item[index_gsm++]=0x512C;
	  }

	  if (mask & LOG_GSM_RR_Message){
			 GSM_Logging_item[index_gsm++]=0x512F;
	  }

	  if (mask & LOG_GSM_CELL_INFORMATIOM){
		  GSM_Logging_item[index_gsm++]= 0x5133;
		  GSM_Logging_item[index_gsm++]= 0x5134;
		  GSM_Logging_item[index_gsm++]= 0x513A;
		  GSM_Logging_item[index_gsm++]= 0x51FC;
		  GSM_Logging_item[index_gsm++]= 0x51FE;
		  GSM_Logging_item[index_gsm++]= 0x5233;
	 }

	  if (mask &  LOG_GSM_CALL_INFORMATIOM){
		  GSM_Logging_item[index_gsm++]= 0x506C;
		  GSM_Logging_item[index_gsm++]= 0x512C;
		  GSM_Logging_item[index_gsm++]= 0x5135;
		  GSM_Logging_item[index_gsm++]= 0x5139;
		  GSM_Logging_item[index_gsm++]= 0x5079;
	  }

	  if (mask &  LOG_GPRS_L2_INFORMATION){
		  IsGprsL2InfoEnable=true;
		  GSM_Logging_item[index_gsm++]= 0x506C;
		  GSM_Logging_item[index_gsm++]= 0x5202;
		  GSM_Logging_item[index_gsm++]= 0x5205;
		  GSM_Logging_item[index_gsm++]= 0x5206;
		  GSM_Logging_item[index_gsm++]= 0x5207;
		  GSM_Logging_item[index_gsm++]= 0x520A;
		  GSM_Logging_item[index_gsm++]= 0x520F;
		  GSM_Logging_item[index_gsm++]= 0x5209;
		  GSM_Logging_item[index_gsm++]= 0x5210;
		  GSM_Logging_item[index_gsm++]= 0x5211;
		  GSM_Logging_item[index_gsm++]= 0x5212;
		  GSM_Logging_item[index_gsm++]= 0x5228;
		  GSM_Logging_item[index_gsm++]= 0x522A;
		  GSM_Logging_item[index_gsm++]= 0x5245;
		  GSM_Logging_item[index_gsm++]= 0x5079;
	  }

	  if (mask &  LOG_GSM_RACH_INFORMATION){
		  GSM_Logging_item[index_gsm++]= 0x5131;
	  }

	  if (mask &  LOG_GSM_UMTS_INFORMATION){
		  GSM_Logging_item[index_gsm++]= 0x51FA;
		  NAS_item[index_nas++]= 0x7130;
		  NAS_item[index_nas++]= 0x7131;
		  NAS_item[index_nas++]= 0x7133;
		  NAS_item[index_nas++]= 0x713A;
	  }

	  if (mask &  LOG_GSM_PAGING_INFORMATION){
		  GSM_Logging_item[index_gsm++]= 0x506F;
		  GSM_Logging_item[index_gsm++]= 0x5132;
	  }
	  insertLargestItem(index_gsm,GSM_CMD);
	  insertLargestItem(index_nas,NAS_CMD);

#ifdef GSM_ENABLE
	  diag_com_write_command(globalFD,0x05,GSM_Logging_item);
	  diag_com_write_command(globalFD,0x07,NAS_item);
#endif

	  memset(GSM_Logging_item,0,sizeof(GSM_Logging_item));
	  memset(NAS_item,0,sizeof(NAS_item));
  }
  else if(systype== WCDMA){
	  gLogMaskWCDMA = mask;

	  if (mask &  LOG_WCDMA_RRC_MESSAGES){

		  WCDMA_Logging_item[index_wcdma++]= 0x412F;
	  }


	  if (mask & LOG_WCDMA_RRC_STATE_MESSAGES){
	  		  WCDMA_Logging_item[index_wcdma++]= 0x4125;
	  }


	  if (mask &  LOG_WCDMA_CELL_INFORMATION){
		  WCDMA_Logging_item[index_wcdma++]= 0x4005;
		  WCDMA_Logging_item[index_wcdma++]= 0x4127;
		  WCDMA_Logging_item[index_wcdma++]= 0x4160;
	  }

	  if(mask & LOG_WCDMA_ACTIVE_AND_NEIGBOURING_INFORMATION){
		  WCDMA_Logging_item[index_wcdma++]=0x4005;
		  WCDMA_Logging_item[index_wcdma++]=0x4110;
		  WCDMA_Logging_item[index_wcdma++]=0x4111;
	  }


	  if(mask & LOG_WCDMA_BLER_REPORT ){
		  WCDMA_Logging_item[index_wcdma++]=0x4116;
	  }


	  if (mask & LOG_WCDMA_L2_INFORMATION){
		  WCDMA_Logging_item[index_wcdma++]=0x4138;
		  WCDMA_Logging_item[index_wcdma++]=0x414A;
		  WCDMA_Logging_item[index_wcdma++]=0x4208;
		  WCDMA_Logging_item[index_wcdma++]=0x4209;
		  WCDMA_Logging_item[index_wcdma++]=0x420B;
		  WCDMA_Logging_item[index_wcdma++]=0x4214;
		  WCDMA_Logging_item[index_wcdma++]=0x4222;
		  WCDMA_Logging_item[index_wcdma++]=0x4311;
	  }


	  if(mask & LOG_WCDMA_UMTS_INFORMATION) {
		  NAS_item[index_nas++]=0x7130;
		  NAS_item[index_nas++]=0x7131;
		  NAS_item[index_nas++]=0x7133;
		  NAS_item[index_nas++]= 0x713A;
	  }


	  if(mask & LOG_WCDMA_RACH_INFORMATION ){
		  WCDMA_Logging_item[index_wcdma++]=0x410D;
		  WCDMA_Logging_item[index_wcdma++]=0x4160;
	  }

     if(mask & LOG_WCDMA_SIR_LOGING){
	     WCDMA_Logging_item[index_wcdma++]=0x4010;
     }

	  insertLargestItem(index_wcdma,WCDMA_CMD);
	  insertLargestItem(index_nas,NAS_CMD);

#ifdef WCDMA_ENABLE
	  diag_com_write_command(globalFD,0x04,WCDMA_Logging_item);
	  diag_com_write_command(globalFD,0x07,NAS_item);
#endif

	  memset(WCDMA_Logging_item,0,sizeof(WCDMA_Logging_item));
	  memset(NAS_item,0,sizeof(NAS_item));

  }else if(systype== LTE)
  {
	  index_event =0;
	  if (mask & LOG_LTE_RRC_MESSAGES){
		  LTE_Logging_item[index_lte++]= 0xB0C0;
		  LTE_Logging_item[index_lte++]= 0xB0C1;
		  LTE_Logging_item[index_lte++]= 0xB0E2;
		  LTE_Logging_item[index_lte++]= 0xB0E3;
		  LTE_Logging_item[index_lte++]= 0xB0EC;
		  LTE_Logging_item[index_lte++]= 0xB0ED;
		  gEventMaskInternal |= LTE_RRC_DOWNLINK_MESSAGE;
		  gEventMaskInternal |= LTE_RRC_UPLINK_MESSAGE;
	  }
	  if (mask & LOG_LTE_CELL_INFORMATION){
		  /* Add only default parameter */
	  };

	  if (mask & LOG_LTE_EMM_INFORMATION){
		  LTE_Logging_item[index_lte++]= 0xB0EE;
		  gEventMaskInternal |= LTE_NAS_ESM_OTA_INCOMING_MESSAGE;
		  gEventMaskInternal |= LTE_NAS_ESM_OTA_OUTGOING_MESSAGE;
	  }
	  if (mask & LOG_LTE_PHYSICAL_LAYER){
		   LTE_Logging_item[index_lte++]= 0xB139;
		   LTE_Logging_item[index_lte++]= 0xB168;
		   LTE_Logging_item[index_lte++]= 0xB16F;
		   LTE_Logging_item[index_lte++]= 0xB171;
		   LTE_Logging_item[index_lte++]= 0xB173;
		   LTE_Logging_item[index_lte++]= 0xB14D;
		   LTE_Logging_item[index_lte++]= 0xB16D;
	  }
	  if (mask & LOG_LTE_SESSION_INFORMATION){
		   LTE_Logging_item[index_lte++]= 0xB0E5;
		   gEventMaskInternal |= LTE_RRC_STATE_CHANGE;
		   gEventMaskInternal |= LTE_NAS_EMM_OTA_INCOMING_MESSAGE;
		   gEventMaskInternal |= LTE_NAS_EMM_OTA_OUTGOING_MESSAGE;
	  }
	  if (mask & LOG_LTE_RACH_INFORMATION){
	        LTE_Logging_item[index_lte++] = 0xB060;
	        LTE_Logging_item[index_lte++] = 0xB062;
	        gEventMaskInternal |=  LTE__RACH_ACCESS_START;
	       	gEventMaskInternal |= LTE_RACH_ACCESS_RESULT;
	  }
	  if (mask & LOG_LTE_MOBILITY_INFORMATION){
	        LTE_Logging_item[index_lte++] = 0xB16C;
	        gEventMaskInternal |= LTE_RRC_DOWNLINK_MESSAGE;
			gEventMaskInternal |= LTE_RRC_UPLINK_MESSAGE;
			gEventMaskInternal |= LTE_NEW_CELL_INDICATION;
			gEventMaskInternal |= LTE_HANDOVER_FAILURE;
			gEventMaskInternal |= LTE_MOBILITY_FROM_EUTRA_STARTED;
			gEventMaskInternal |= LTE_MOBILITY_FROM_EUTRA_FAILED;
			gEventMaskInternal |= LTE_INTERRAT_RESELECTION_FROM_EUTRAN_START;
			gEventMaskInternal |= LTE_INTERRAT_RESELECTION_FROM_EUTRAN_END;
			gEventMaskInternal |= LTE_INTERRAT_REDIRECTION_FROM_EUTRAN_END;
			gEventMaskInternal |= LTE_INTERRAT_REDIRECTION_FROM_EUTRAN_END;
			gEventMaskInternal |= LTE_RADIO_LINK_FAILURE;
			gEventMaskInternal |= LTE_RADIO_LINK_FAILURE_STAT;
			gEventMaskInternal |= LTE_NAS_ESM_INCOMING_MESSAGE ;
			gEventMaskInternal |= LTE_NAS_ESM_OUTGOING_MESSAGE ;
	  }
	  if (mask &  LOG_LTE_RLC_INFORMATION){
	        LTE_Logging_item[index_lte++]= 0xB087;
	        LTE_Logging_item[index_lte++]= 0xB097;
	  }
	  if (mask & LOG_LTE_MAC_INFORMATION){
		  	LTE_Logging_item[index_lte++]= 0xB063;
		    LTE_Logging_item[index_lte++]= 0xB064;
		    LTE_Logging_item[index_lte++]= 0xB067;
		    LTE_Logging_item[index_lte++]= 0xB132;
		    LTE_Logging_item[index_lte++]= 0xB139;
		    LTE_Logging_item[index_lte++]= 0xB16B;
	  }
	  if (mask &  LOG_LTE_PDCP_INFORMATION){
			LTE_Logging_item[index_lte++]= 0xB0A4;
			LTE_Logging_item[index_lte++]= 0xB0B4;
	  }
	  insertLargestItem(index_lte,LTE_CMD);
	  insertLargestItem(index_event,EVENT_CMD);

#ifdef LTE_ENABLE

	  /* Enable the Logging */
	  diag_com_write_command(globalFD,0x0B,LTE_Logging_item);
	  /* Enable the Event */
	  pthread_mutex_unlock(&sychMutex); /* This is due to facts that set_event_mask is using same lock*/
	  set_event_mask(gEventMaskInternal);
	  pthread_mutex_lock(&sychMutex);
	  //diag_com_write_event(globalFD, Event_item);
#endif

	  memset(LTE_Logging_item,0,sizeof(LTE_Logging_item));
	  memset(Event_item,0,sizeof(Event_item));
  }
  else if(systype== CDMA){
	  	  CDMA_Logging_item[index_cdma++]= 0x1068;
	  	  CDMA_Logging_item[index_cdma++]= 0x1069;
	  	  CDMA_Logging_item[index_cdma++]= 0x107E;
	  	  CDMA_Logging_item[index_cdma++]= 0x1080;
	  	  CDMA_Logging_item[index_cdma++]= 0x108A;
	  	  CDMA_Logging_item[index_cdma++]= 0x108B;
	  	  CDMA_Logging_item[index_cdma++]= 0x1093;
	  	  CDMA_Logging_item[index_cdma++]= 0x119A;
	  	  CDMA_Logging_item[index_cdma++]= 0x119B;
	  	  CDMA_Logging_item[index_cdma++]= 0x119C;
	  	  CDMA_Logging_item[index_cdma++]= 0x119D;
	  	  CDMA_Logging_item[index_cdma++]= 0x119E;
	  	  CDMA_Logging_item[index_cdma++]= 0x119F;
	  	  CDMA_Logging_item[index_cdma++]= 0x11A1;
	  	  CDMA_Logging_item[index_cdma++]= 0x1183;
	  	  CDMA_Logging_item[index_cdma++]= 0x11A2;
	  	  CDMA_Logging_item[index_cdma++]= 0x119F;
	  	  CDMA_Logging_item[index_cdma++]= 0x127D;
	  	  CDMA_Logging_item[index_cdma++]= 0x129C;
	  	  CDMA_Logging_item[index_cdma++]= 0x12B6;
	  	 // CDMA_Logging_item[index_cdma++]= 0x1273;
	  	  CDMA_Logging_item[index_cdma++]= 0x1154,
	  	  CDMA_Logging_item[index_cdma++]= 0x1374;
	  	  CDMA_Logging_item[index_cdma++]= 0x1279;
	  	  CDMA_Logging_item[index_cdma++]= 0x1183;
	  	  insertLargestItem(index_cdma,CDMA_CMD);
	 #ifdef CDMA_ENABLE
	  	  //int interval = 500;
	  	  diag_com_write_command(globalFD,0x1,CDMA_Logging_item);

	 #endif
	  	  memset(CDMA_Logging_item,0,sizeof(CDMA_Logging_item));
  }
  else 
  {
	  printf("Unknown System type\n");
  }
  pthread_mutex_unlock(&sychMutex);
  return 0;
}

/* Function to configure event on the device */
int set_event_mask(int mask){

	index_event = 0;
	pthread_mutex_lock(&sychMutex);

	if(mask & EVENT_CC) {
		isCCEventsEnabled = true;
	} else {
		isCCEventsEnabled = false;
	}

	if (mask & EVENT_InterRat_Handover_Start ){
		Event_item[index_event]=447;
		index_event++;
	}
	if (mask &  EVENT_InterRat_Handover_End){
		Event_item[index_event]=448;
		index_event++;
	}
	if (mask & EVENT_CallDrop){
		Event_item[index_event]=570;
		index_event++;
	}
	if (mask & EVENT_RRC_STATE ){
		Event_item[index_event]=571;
		index_event++;
	}
	if (mask & EVENT_RB_Setup_Failure){
		Event_item[index_event]=627;
		index_event++;
	}
	if (mask & EVENT_RRC_Phy_Channel_Failure){
		Event_item[index_event]=628;
		index_event++;
	}
	if(mask & EVENT_MM_STATE){
		Event_item[index_event]=1022;
		index_event++;
	}
	if (mask & EVENT_GMM_STATE){
		Event_item[index_event]= 1023;
		index_event++;
	}
	if(mask & EVENT_RRC_WCDMA_Rat_Change ){
		Event_item[index_event]=1105;
		index_event++;
	}
	if(mask & EVENT_RRC_TIMER_EXPIRY){
		Event_item[index_event]=1541; 
		index_event++;
	}
	if(mask & EVENT_CELL_SELECTION_FAILURE){
		Event_item[index_event]=1548; 
		index_event++;
	}
	if (mask & EVENT_LTE_RRC_STATE_CHANGE)
	{
		Event_item[index_event++]=1606;
	}
	if (mask &  EVENT_LTE_INTERAT_RESEL_START){
		Event_item[index_event++]= 1617;
	}
	if (mask &  EVENT_LTE_INTERAT_RESEL_END){
		Event_item[index_event++]= 1618;
	}
	if (mask &  EVENT_LTE_INTERAT_REDIR_START){
		Event_item[index_event++]=1807 ;
	}
	if (mask &  EVENT_LTE_INTERAT_REDIR_END){
		Event_item[index_event++]= 1808;
	}
	if(mask & EVENT_LTE_HANDOVER_FAILURE){
		Event_item[index_event++]=1613 ;
	}
	if(mask & EVENT_LTE_MOBILITY_E_UTRA_STARTED){
		Event_item[index_event++]= 1615;
	}
	if(mask & EVENT_LTE_MOBILITY_E_UTRA_FAILED){
		Event_item[index_event++]=1616;
	}
	if(mask & EVENT_LTE_NEW_CELL_SELECTION){
		Event_item[index_event++]=1611;
	}
	if(mask & EVENT_LTE_OUT_OF_SERVICE){
		Event_item[index_event++]=1607;
	}
	if(mask & EVENT_LTE_RADIO_LINK_FAILURE){
		Event_item[index_event++]=1608;
	}
	if(mask & EVENT_LTE_RADIO_LINK_FAILURE_STAT){
		Event_item[index_event++]=1995;
	}

	if (gEventMaskInternal & LTE_RRC_DOWNLINK_MESSAGE){
		Event_item[index_event++] = 1609;
	}
	if (gEventMaskInternal & LTE_RRC_UPLINK_MESSAGE){
		Event_item[index_event++] = 1610;
	}
	if (gEventMaskInternal & LTE_NAS_ESM_OTA_INCOMING_MESSAGE){
		Event_item[index_event++] = 1968;
	}
	if (gEventMaskInternal & LTE_NAS_ESM_OTA_OUTGOING_MESSAGE){
		Event_item[index_event++] = 1969;
	}
	if (gEventMaskInternal & LTE_RRC_STATE_CHANGE){
		Event_item[index_event++] = 1606;
	}
	if (gEventMaskInternal & LTE_NAS_EMM_OTA_INCOMING_MESSAGE){
		Event_item[index_event++] = 1966;
	}
	if (gEventMaskInternal & LTE_NAS_EMM_OTA_OUTGOING_MESSAGE){
		Event_item[index_event++] = 1967;
	}
	if (gEventMaskInternal & LTE_NEW_CELL_INDICATION){
		Event_item[index_event++] = 1611;
	}
	if (gEventMaskInternal & LTE_HANDOVER_FAILURE){
		Event_item[index_event++] = 1613;
	}
	if (gEventMaskInternal & LTE_MOBILITY_FROM_EUTRA_STARTED){
		Event_item[index_event++] = 1615;
	}
	if (gEventMaskInternal & LTE_MOBILITY_FROM_EUTRA_FAILED){	if(gEventMaskInternal & LTE_NAS_ESM_INCOMING_MESSAGE){
		Event_item[index_event++]=1635;
	}
	if(gEventMaskInternal & LTE_NAS_ESM_OUTGOING_MESSAGE){
		Event_item[index_event++]=1636;
	}
	if(gEventMaskInternal & LTE__RACH_ACCESS_START){
		Event_item[index_event++]=1501;
	}
	if(gEventMaskInternal & LTE_RACH_ACCESS_RESULT){
		Event_item[index_event++]=1503;
	}
		Event_item[index_event++] = 1616;
	}
	if (gEventMaskInternal & LTE_INTERRAT_RESELECTION_FROM_EUTRAN_START){
		Event_item[index_event++] = 1617;
	}
	if (gEventMaskInternal & LTE_INTERRAT_RESELECTION_FROM_EUTRAN_END){
		Event_item[index_event++] = 1618;
	}
	if (gEventMaskInternal & LTE_INTERRAT_REDIRECTION_FROM_EUTRAN_START){
		Event_item[index_event++] = 1807;
	}
	if (gEventMaskInternal & LTE_INTERRAT_REDIRECTION_FROM_EUTRAN_END){
		Event_item[index_event++] = 1808;
	}
	if(gEventMaskInternal & EVENT_LTE_RADIO_LINK_FAILURE){
		Event_item[index_event++]=1608;
	}
	if(gEventMaskInternal & EVENT_LTE_RADIO_LINK_FAILURE_STAT){
		Event_item[index_event++]=1995;
	}
	if(gEventMaskInternal & LTE_NAS_ESM_INCOMING_MESSAGE){
		Event_item[index_event++]=1635;
	}
	if(gEventMaskInternal & LTE_NAS_ESM_OUTGOING_MESSAGE){
		Event_item[index_event++]=1636;
	}
	if(gEventMaskInternal & LTE__RACH_ACCESS_START){
		Event_item[index_event++]=1501;
	}
	if(gEventMaskInternal & LTE_RACH_ACCESS_RESULT){
		Event_item[index_event++]=1503;
	}


	insertLargestItem(index_event,EVENT_CMD);/* Event masking */
	diag_com_write_event(globalFD,Event_item);
	gEventMask = mask; 
	pthread_mutex_unlock(&sychMutex);
  return 0;
}

void insertLargestItem(int index,int type)
{
	switch (type){

		case GSM_CMD :
			{
				GSM_Logging_item[index++]= 0x5134;
				GSM_Logging_item[index++]= 0x513A;
				GSM_Logging_item[index++]= 0x51FC;
				GSM_Logging_item[index++]= 0x5C5C;
				GSM_Logging_item[index]	= 0x0;
			}
			break;

		case WCDMA_CMD :
			{
				WCDMA_Logging_item[index++]= 0x4005;
				WCDMA_Logging_item[index++]= 0x410C;
				WCDMA_Logging_item[index++]= 0x4125;
				WCDMA_Logging_item[index++]= 0x401C;
				WCDMA_Logging_item[index++]= 0x4179;
				WCDMA_Logging_item[index++]= 0x4186;
				WCDMA_Logging_item[index++]= 0x4110;
				WCDMA_Logging_item[index++]= 0x4111;
				WCDMA_Logging_item[index++]= 0x4127;
				WCDMA_Logging_item[index++]= 0x4910;
				WCDMA_Logging_item[index]= 0x0;
			}
			break;
		case NAS_CMD :
			{
				NAS_item[index++]=0x7138;
				NAS_item[index++]=0x7507;
				NAS_item[index]=0x0;
			}
			break;

		case EVENT_CMD :
			{
				Event_item[index++]=2112;
				Event_item[index]=0;
			}
			break;
		case LTE_CMD :
			{
				LTE_Logging_item[index++]= 0xB087;
		        LTE_Logging_item[index++]= 0xB097;
		        LTE_Logging_item[index++]= 0xB0C2;
		        LTE_Logging_item[index++]= 0xB0C1;
		        LTE_Logging_item[index++]= 0xB193;
		        LTE_Logging_item[index++]= 0xB902;
		        LTE_Logging_item[index]= 0x0;
			}
			break;
		case CDMA_CMD :
		{
			CDMA_Logging_item[index++]= 0x1068;
			CDMA_Logging_item[index++]= 0x1599;
			CDMA_Logging_item[index]= 0x0;
		}
		break;
		default:
			printf("Unknown type");
			break;
	}
}
/* function to set callback */
int sendInformation ( int systype, int notifier ,int identifier , char *buf , int length)
{
   if(init_diag_info[systype].sysType){
   	 if(init_diag_info[systype].mycallback != NULL)
   	 {
           init_diag_info[systype].mycallback(notifier,identifier,buf,length);
           return 0;
   	 }
    }
   return -1;
}


int get_GSMCellInfo (GSMCellInfo *gsmCellInfo)
{
  pthread_mutex_lock(&cellInfoMutex);
  *gsmCellInfo = mGSMCellInfo;
  pthread_mutex_unlock(&cellInfoMutex);
  return 0;
}

int get_WCDMACellInfo (WCDMACellInfo *wcdmacellInfo)
{
  pthread_mutex_lock(&cellInfoMutex);
  *wcdmacellInfo = myCellInfo;
#ifdef MEMORY_CORRUPTION_REPORT
  d_warning("get_WCDMACellInfo mycellinfo: ECIO : %f, PSC : %u\n\n", myCellInfo.activeSetInfo[0].ECIO, myCellInfo.activeSetInfo[0].PRIM_SC_CODE);
  d_warning("get_WCDMACellInfo wcdmacellinfo: ECIO : %f, PSC : %u\n\n", (*wcdmacellInfo).activeSetInfo[0].ECIO, (*wcdmacellInfo).activeSetInfo[0].PRIM_SC_CODE);
#endif //MEMORY_CORRUPTION_REPORT
  pthread_mutex_unlock(&cellInfoMutex);
  return 0;
}

int get_LTECellInfo (LTECellInfo *lteCellInfo){
  pthread_mutex_lock(&cellInfoMutex);
  *lteCellInfo = mLTE_CellInfo;
  pthread_mutex_unlock(&cellInfoMutex);
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  d_log("\nCurrentTime: %d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  d_log("\nget_LTECellInfo : PCI = %u",lteCellInfo->physical_cell_id);
  d_log("\nget_LTECellInfo : CellIdentity = %u",lteCellInfo->cell_identity);
  d_log("\nget_LTECellInfo : MCC = %u",lteCellInfo->mcc);
  d_log("\nget_LTECellInfo : UL_Bandwidth = %f",lteCellInfo->ul_bandwidth);
  d_log("\nget_LTECellInfo : UL_FREQ= %f",lteCellInfo->ul_freq);
  return 0;
}

/*Function to get Event masked on device*/
int get_event_mask()
{
  return gEventMask;
}

/*Function to Loging masked on device*/
int get_logging_mask( int systemType)
{
  if (systemType == GSM)
  return gLogMaskGSM;
  else if (systemType == WCDMA)
  return gLogMaskWCDMA;
  else if (systemType == LTE)
	return gLogMaskLTE;
}

/* Function to Stop DM */
int close_diag_port(){

  int status = -1;
  init_done = 0;
  pthread_kill(childthreadID,SIGALRM);
#ifdef CDMA_ENABLE
  pthread_kill(cdma_thread,SIGALRM);
#endif
  status = diag_com_close(globalFD);
  globalFD=0;
  gEventMask=0;
  gLogMaskGSM=0;
  systemTypeDetected=NOT_INIT;
  gLogMaskWCDMA=0;
  gLogMaskLTE=0;
  globalUrfcnFreq=0;
  gManufactureType=0;
  closeDMLogFile();
  stop_file_write();
  if (diag_port_q != NULL) destroyQueue(diag_port_q);
  pthread_kill(processthread,SIGALRM);
  memset(&mGSMCellInfo,0,sizeof(mGSMCellInfo));
  memset(&myCellInfo,0,sizeof(myCellInfo));
  memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
  memset(&GSM_Logging_item,0,sizeof(GSM_Logging_item));
  memset(&WCDMA_Logging_item,0,sizeof(WCDMA_Logging_item));
  memset(&LTE_Logging_item,0,sizeof(LTE_Logging_item));
  memset(&NAS_item,0,sizeof(NAS_item));
  memset(&Event_item,0,sizeof(Event_item));
  pthread_mutex_destroy(&sychMutex);
  pthread_mutex_destroy(&fileReleaseMutex);
  pthread_mutex_destroy(&fileDlfMutex);
  dm_destroy_logfile_mutex();
  dm_destroy_cellInfoMutex();
  return 0;
}

/*Function to set  UARFCN value */
int set_uarfn_value(unsigned short frequency)
{
	return Wcdma_set_uarfn_value(frequency,globalFD);
}

/*Function to get UARFCN value*/
unsigned short get_uarfn_value()
{
  return Wcdma_get_uarfn_value(globalFD);
}


/*Function to check required URAFCN is set or not*/
bool is_freq_set_success(){
  if (myCellInfo.URAFCN_Downlink == globalUrfcnFreq)
	  return true;
  else
	  return false;
}

/*Function to start file writing*/
int  start_file_write(char* imsi,char* testid,int imsilen,int testidlen )
{
	d_warning("\n Start of Text File writing TestID: %s, IMSI : %s, IMSILEN : %d, Testidlen: %d ", testid, imsi, imsilen, testidlen);
	return filewriter_start_file_write(imsi,testid,imsilen,testidlen);
}

/*Stop the file writing*/
int  stop_file_write()
{
    d_warning("\n Text file write stopped");
    return filewriter_stop_file_write();
}

/*Function to set various parameter of file */
int  set_GUIDInformation(char *guidInfo, int guidInfolen){
	return filewriter_set_GUIDInformation( guidInfo,guidInfolen);
}

/*Function to set various parameter of file */
int  set_GeoLocationInfomation (double lat,double longitude,double altitude,
		int speed,int direction,int locationAccurancy)
{
	return filewriter_set_GeoLocationInfomation (lat,longitude,altitude,
				 speed, direction,locationAccurancy);
}

/*Function to get the current radio technology running on device */
int  get_state_info()
{
	return decode_get_state_info();
}
/*Function to get DM current version */
int  get_DM_version(char *version)
{
	return version_get_DM_version(version);
}


/* Function to set lte band */
int set_lte_freqband(u_int64_t band , int device9660){
	return ltebandlock_set_lte_freqband(globalFD,band ,device9660);
}

/* Function to read lte band */
u_int64_t get_lte_freqband(int device9660)
{
	if(device9660){
		ltebandlock_get_uarfn_value(globalFD);
	}else
		ltebandlock_nv_read_cmd(globalFD,0x1AAC);
	sleep(1);/*Sleep to decode the response */
	return  ltebandlock_get_lte_freq_band(device9660);
}

u_int32_t set_gps_last_position()
{
	Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"Function called !");
	return diag_com_gps_write_command(globalFD);
}





#if 0
void main(){
	int event = EVENT_RRC_STATE | EVENT_MM_STATE | EVENT_GMM_STATE| EVENT_InterRat_Handover_Start|
	   					EVENT_InterRat_Handover_End  | EVENT_CallDrop | EVENT_LTE_MOBILITY_E_UTRA_FAILED |
	   					EVENT_LTE_MOBILITY_E_UTRA_STARTED |  EVENT_LTE_HANDOVER_FAILURE | EVENT_LTE_INTERAT_REDIR_END
	   					| EVENT_LTE_INTERAT_REDIR_START | EVENT_LTE_RRC_STATE_CHANGE | EVENT_LTE_NEW_CELL_SELECTION |  ;
	   			set_event_mask(EVENT_ALL);

}
#endif

