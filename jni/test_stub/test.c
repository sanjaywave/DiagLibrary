#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <time.h>
#include <signal.h>
#include <linux/random.h>
//#include <execinfo.h>
#include "interfaceapi.h"
#include "errors.h"
#include "diag-cmd.h"
#include "decode.h"
#include "gsmdecode.h"
#include "wcdmadecode.h"
#include "ltedecode.h"
#include "nasdecode.h"
# include "DlfInterfaceApi.h"
#include "CdmaContext.h"
#include "lte_parameters.h"
#include "ccEvents.h"
#define log  printf
int stop_count=0;
int logging_mask;
extern void exit();
void initport();
int networktype;
FILE *malloc_filep;

void printRatType (u_int8_t rat);
void printInfo();
void printRatType (u_int8_t rat)
{
  switch (rat)
  {
  	  case   0x00 :   log("CDMA-HRPD\n"); break;
  	  case   0x01 :   log("CDMA-1X\n"); break;
  	  case   0x02 :   log("GSM\n");  break;
  	  case   0x03 :   log("WCDMA\n");break;
  	  case   0x04 :   log("LTE\n"); break;
  	  case   0x05 :   log("TDSCDMA \n"); break;
  	  default :       log("Unknown\n");
  	  break;
  }
}

void mycallback (int notifierType, int identifier , char *buf , unsigned int length)
{
  if (notifierType == LOGGING_DATA){
	  	long *packettime = (long*)&buf[0];
	    time_t time_span =time(packettime);// convert the long time format into human readable format
	    log("\nTimeaa  :- %s", ctime(&time_span));
	    switch (identifier){
        case LOG_LTE_RRC_MESSAGES :
			log("LTE RRC logging Received");
			log("\nLTE OTA Message  ");
			lte_ota_messages * otacmd =(lte_ota_messages *)&buf[8];
			log("Version number: %d",otacmd->version);
			log("\nRRC release: %d",otacmd->rrc_rel);
			log("\nRRC version: %d",otacmd->rrc_ver);
			log("\nRadio bearer ID: %d",otacmd->rb_id);
			log("\nPhysical cell ID: %d",otacmd->physical_cell_id);
			log("\nE-ARFCN frequency: %d",otacmd->freq);
			log("\nSFN-SubFrameNumber: %d",otacmd->sfn);
			log("\nPDU-ProtocolDataUnit :-");
			  switch(otacmd->pdu_num){
			  case 0x2: log("BCCH_DL_SCH Message");break;
			  case 0x3: log("MCCH Message "); break;
			  case 0x4: log("PCCH Message "); break;
			  case 0x5: log("DL_CCCH Message "); break;
			  case 0x6: log("DL_DCCH Message "); break;
			  case 0x7: log("UL_CCCH Message "); break;
			  case 0x8: log("UL_DCCH Message"); break;
			  case 0x9: log("UE EUTRA Capability Message"); break;
			  case 0xA: log("VarShortMAC_Input "); break;
			  case 0xB: log("UE EUTRA Capability v9a0_IEs  "); break;
			  case 0xC: log("System Information Block Type1 Message"); break;
			  default : log("Unknown message type");
			  break;
			  }
           log("\nEncoded message length: %d",otacmd->encoded_msg_len);
           log("\nASN Encoded Message Start ");
           log("\n");
           if(otacmd->encoded_msg_len < 6156){
         	  //myarrayprint(otacmd->encoded_data,otacmd->encoded_msg_len);
           }
           break;
        	  default: log ("\n Unknown Logging ");
        	  break;
	    }
  }
  if (notifierType == EVENT_DATA){
  	  switch (identifier){
	  case EVENT_SYSTEM_BLOCK :
		 log ("$$$$$ Reading Thread Blocked $$$$$$$$$");
		 break;
	  case EVENT_RRC_STATE :
		  log ("\nEVENT_RRC_STATE"); break;
	  case EVENT_MM_STATE :
		  log ("\nEVENT_MM_STATE "); break;
	  case EVENT_GMM_STATE :
		  log ("\nEVENT_GMM_STATE");  break;
	  case EVENT_InterRat_Handover_Start :
		  log ("\nEVENT_InterRat_Handover_Star");  break;
	  case EVENT_InterRat_Handover_End :
		  log("\nEVENT_InterRat_Handover_End");  break;
	  case EVENT_CallDrop :
		  log ("\nEVENT_CallDrop"); break;
	  case EVENT_RB_Setup_Failure :
		  log ("\nEVENT_RB_Setup_Failure");  break;
	  case EVENT_RRC_Phy_Channel_Failure :
		  log ("\nEVENT_RRC_Phy_Channel_Failure");  break;
	  case EVENT_RRC_WCDMA_Rat_Change :
		  log ("\nEVENT_RRC_WCDMA_Rat_Change");  break;
	  case EVENT_RRC_TIMER_EXPIRY :
		  log ("\nEVENT_RRC_TIMER_EXPIRY");  break;
	  case EVENT_CELL_SELECTION_FAILURE :
		  log ("\nEVENT_CELL_SELECTION_FAILURE"); break;
	  case EVENT_LTE_RRC_STATE_CHANGE :
		  log ("\nEVENT_LTE-RRC Event"); break;
	  case EVENT_LTE_INTERAT_RESEL_START :
	  {
		  log ("\n---Inter-RAT reselection from E-UTRAN start--Moving to ");
		  inter_rat_reselection_from_e_utran_start *irrfeus =(inter_rat_reselection_from_e_utran_start *) &buf[0];
		  printRatType (irrfeus->rat);
		  log("Freq-Band indicator  = %d",irrfeus->freq_band_indicator);
		  log("\nFrequency  = %d",irrfeus->frequency );
		  log("\nCell ID   = %d",irrfeus->cell_id);
	  }
	  break;

	  case EVENT_LTE_INTERAT_RESEL_END :
	  {
		  log ("\n--Inter-RAT reselection from E-UTRAN end----Moved ");
		  inter_rat_reselection_from_e_utran_end *irrfeue =(inter_rat_reselection_from_e_utran_end *) &buf[0];
			printRatType(irrfeue->rat);
		}
		break;
	  case EVENT_LTE_INTERAT_REDIR_START :
	  {
		  log ("\n--Inter-RAT redirection from E-UTRAN--Moving to ");
		  inter_rat_redirection_from_e_utran_start *irredfeus =(inter_rat_redirection_from_e_utran_start*)  &buf[0];
		  printRatType(irredfeus->rat);
	  }
	  break;
	  case EVENT_LTE_INTERAT_REDIR_END :
	  {
		  int status;
		  log ("\n--Inter-RAT redirection from E-UTRAN end--Moved  ");
		  inter_rat_redirection_from_e_utran_end *irredfeue =(inter_rat_redirection_from_e_utran_end *) &buf[0];
		  printRatType(irredfeue->rat);
		  status = irredfeue->status_casue;
		  switch(status){
		  case  0x0 :
			  log("Status-cause :- Success");
			  break;
		  case  0x01 :
			  log("Status-cause :- Failed");
			  break;
		  case  0x02 :
			  log("Status-cause :-Aborted-Service Request ");
			  break;
		  case  0x03 :
			  log("Status-cause :-Aborted-Manual PLMN Search Request  ");
			  break;
		  case  0x04 :
			  log("Status-cause :-Aborted-Deactivate  ");
			  break;
		  default:
			  log("\nUnknown Status cause");
		  }
		  break;
	  case EVENT_LTE_HANDOVER_FAILURE:
	  {
		handover_failure *hf =(handover_failure*)&buf[0];
		log("\nFrequency = %d",hf->frequency);
		log("\nCell Id = %d",hf->cell_id);
		log("\n Cause= %d",hf->cause);
	  }
	  break;

	  case EVENT_LTE_MOBILITY_E_UTRA_STARTED:
	  {
		 mobility_from_e_ultra_started *mfeus = ( mobility_from_e_ultra_started*)&buf[0];
		log("\nRAT - Procedure = %d",mfeus->rat_procedure);
	  }
	  break;
	  case EVENT_LTE_MOBILITY_E_UTRA_FAILED:
	  {
		mobility_from_e_ultra_failed *mfeuf =(mobility_from_e_ultra_failed*)&buf[0];
		log("\nStatus- cause =0x%x",mfeuf->status_cause);
	  }
	  break;
	  case EVENT_IRAT_TECHNOLOGY_CHANGE :
	  {
		 event_irat_technology_change *eitc =(event_irat_technology_change *)&buf[0];
		 log ("\nLast Technology = %d ",eitc->Last_technology);
		 log ("\nCurrent Technology =%d ",eitc->Current_technology);
	  }
	  break;
	  case EVENT_LTE_OUT_OF_SERVICE  :
	  {
		log ("\nEVENT_LTE_OUT_OF_SERVICE ");
	  }
	  break;
	  case EVENT_LTE_RADIO_LINK_FAILURE :
	  {
		log ("\nEVENT_LTE_RADIO_LINK_FAILURE");
	  }
	  break;
	  case EVENT_LTE_RADIO_LINK_FAILURE_STAT :
	  {
		log ("\nEVENT_LTE_RADIO_LINK_FAILURE_STAT");
	  }
	  break;
	  }
	  default:    log ("\n Unknown Event ");
	  break;
	  }
  }
  if (notifierType == EVENT_CC){
	  switch(identifier){
	  case CALL_START_INDICATION:
		  log("call start indication\n");
		  log("timestamp : %llu\n", *(u_int64_t *)&buf[0]);
		  log("ismt : %d\n\n\n", *(int32_t *)&buf[8]);
		  break;
	  case SETUP:
		  log("SETUP\n");
		  log("timestamp : %llu\n", *(u_int64_t *)&buf[0]);
		  log("number : %s\n\n\n", &buf[8]);
		  break;
	  case CALL_PROCEEDING:
		  log("CALL_PROCEEDING\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  case PROGRESS:
		  log("PROGRESS\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  case ALERTING:
		  log("ALERTING\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  case CONNECT:
		  log("CONNECT\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  case CONNECT_ACK:
		  log("CONNECT_ACK\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  case DISCONNECT:
		  log("DISCONNECT\n");
		  log("timestamp : %llu\n", *(u_int64_t *)&buf[0]);
		  log("cause value : %d\n\n\n", *(int *)&buf[8]);
		  break;
	  case RELEASE:
		  log("RELEASE\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  case RELEASE_COMPLETE:
		  log("RELEASE_COMPLETE\n");
		  log("timestamp : %llu\n\n\n", *(u_int64_t *)&buf[0]);
		  break;
	  }
  }
}

#if 0
void
print_trace (void)
{
	void *array[1000];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 1000);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
	  printf ("%s\n", strings[i]);

	free (strings);
}
#endif

volatile sig_atomic_t fatal_error_in_progress = 0;

void
fatal_error_signal_handler (int sig)
{
	/* Since this handler is established for more than one kind of signal,
	   it might still get invoked recursively by delivery of some other kind
	   of signal.  Use a static variable to keep track of that. */
	if (fatal_error_in_progress)
		raise (sig);
	fatal_error_in_progress = 1;

	void *p = __builtin_return_address(0);
	printf("p : %x\n", p);
	void *q = __builtin_return_address(1);
	printf("q : %x\n", q);

	//fclose(malloc_filep);

	/* Now reraise the signal.  We reactivate the signal's
	   default handling, which is to terminate the process.
	   We could just call exit or abort,
	   but reraising the signal sets the return status
	   from the process correctly. */
	signal (sig, SIG_DFL);
	raise (sig);
}
         

int main(int argc , char *argv[])
{
  char device[8];
  int gsm=1;
  char * ptr;
  int testcode =0;
  int DLFtestCode = 0;
  int status1=-1;
  u_int16_t frequency;
  char imsi[]="TXF1212TU#24LT";
  char DLFimsi[]= "DLF1212TU#24LT";
  int imsilen= sizeof(imsi);
  int DLFimsilen=sizeof(DLFimsi);
  char testid[]="TXT123";
  char DLFtestid[]="DLF123";
  int network;
  int cmdType = 0;
  u_int64_t freqBand;
  char OptionBuff[5]= {0};
  int urfcnselect;

  //malloc_filep = fopen("/mnt/sdcard/diagApp/malloc_file", "w+");

  signal(SIGABRT, fatal_error_signal_handler);
  signal(SIGSEGV, fatal_error_signal_handler);
  signal(SIGTERM, fatal_error_signal_handler);
  signal(SIGINT, fatal_error_signal_handler);
  signal(SIGQUIT, fatal_error_signal_handler);
  signal(SIGHUP, fatal_error_signal_handler);

  if (argc == 2){
	  ptr = argv[1];
	  frequency = atoi(ptr);
  }else{
	  printf ("Expected Single Argument  \n");
	  return 0;
  }
  initport();
  while (1)
   {
	  cmdType = 0;
	  printf("\nEnter 1 : Select for Enabling\n"
   			"Enter 2 : Start to Writefile\n"
   			"Enter 3 : Stop to Writefile\n"
   			"Enter 4 : View Value\n"
   			"Enter 5 : Stop Test\n"
   			"Enter 6  :Set Urarfn_value\n"
			"Enter 7  :Get Urarfn_value\n"
			"Enter 8  :Get Version\n"
	  		"Enter 9  :Set Wcdma Events\n"
   			"Enter 10 :InitDiag\n"
	  		"Enter 11 :SetBandFrequency\n"
	  		"Enter 12 :ReadBandFrequency\n"
	  		"Enter 13 :Get Current Technology\n"
	  		"Enter 14 :Enable ISF file for WCDMA\n"
	  		"Enter 15 :Enable ISF file for LTE\n"
	  		"Enter 16 :Enable ISF file for GSM\n"
			"Enter 17 :Enable ISF file for CDMA\n"
			"Enter 18 :Start Dlf File Write\n"
			"Enter 19 :Stop Dlf file write\n"
			"Enter 20 :close ISFDM\n"
			"Enter 21 :Get Gps Last Location\n"
			"Enter 22 :Enable LTE MCS info\n"
			"Enter 23 :Disable LTE MCS info\n"
			"Enter 24 to print LTe values \n"
			"Enter 25 to change debuf file path \n" );
	      scanf("%s",OptionBuff);
	  /*gets(OptionBuff,3,stdin);*/
	  /*gets(OptionBuff);*/
	  sscanf(OptionBuff,"%d",&cmdType);
	  switch(cmdType){
	  case 1:{
			  if (networktype >3 && networktype < 7){
				  printf("\nIs this for GSM-1 or WCDMA-2 or LTE-3 or 4-GSM and WCDMA 0r 5 -WCDMA and LTE or 6:CDMA:");
				  scanf("%d",&network);
				  if(network==1){
					  printf("\nEnter the logging value GSM:");
					  scanf("%x",&logging_mask);
					  printf("\nLogging mask =%x",logging_mask);
					  set_logging_mask(GSM,logging_mask);
				  }
				  else if (network==2){
					  printf("\nEnter the logging value WCDMA:");
					  scanf("%x",&logging_mask);
					  printf("\nLogging mask =%x",logging_mask);
					  set_logging_mask(WCDMA,logging_mask);
				  }else if(network==3){
					  printf("\nEnter the logging value LTE :");
					  scanf("%x",&logging_mask);
					  ("\nLogging mask =%x",logging_mask);
					  set_logging_mask(LTE,logging_mask);
				  }else if(network==4){
					  printf("\nEnter the logging value GSM:");
					  scanf("%x",&logging_mask);
					  printf("\nLogging mask =%x",logging_mask);
					  set_logging_mask(GSM,logging_mask);
					  printf("\nEnter the logging value WCDMA:");
					  scanf("%x",&logging_mask);
					  printf("\nLogging mask =%x",logging_mask);
					  set_logging_mask(WCDMA,logging_mask);
				  }else if(network==5){
				  	 printf("\nEnter the logging value WCDMA:");
				  	 scanf("%x",&logging_mask);
				  	 printf("\nLogging mask =%x",logging_mask);
				  	 set_logging_mask(WCDMA,logging_mask);
				  	 printf("\nEnter the logging value LTE:");
				  	 scanf("%x",&logging_mask);
				  	 printf("\nLogging mask =%x",logging_mask);
				  	 set_logging_mask(LTE,logging_mask);
				  }else if(network==6){
				  	 printf("\nEnter the logging value CDMA:");
				  	 scanf("%x",&logging_mask);
				  	 printf("\nLogging mask =%x",logging_mask);
				  	 set_logging_mask(CDMA,logging_mask);
				  }
			  }
			  else if(networktype<=3){
				printf("\nEnter the logging value :");
				scanf("%x",&logging_mask);
				set_logging_mask(networktype,logging_mask);
			  }
			  else if(networktype==10){
				printf("\nEnter the logging value :");
				scanf("%x",&logging_mask);
				set_logging_mask(CDMA,logging_mask);

	  }
   	  break;
	  case 2:
	  {
		  int returnstatus  =-1;
		  char FilePath[360] = {0};
		  testcode++;
		  sprintf(testid,"Test%03d", testcode);
		  printf("TestId = %s",testid);
		  printf("Enter path for txt file : ");
		  scanf ("%s", FilePath);
		  setTXTFilePath(FilePath ,strlen(FilePath));
		  returnstatus=start_file_write(imsi,testid,imsilen,7);
		  if(returnstatus == -1){
			printf("\nFile Name Error");
		  }
	  }
   	  break;
	 case 3:
			{
			  int status=-1;
			  status=stop_file_write();
			  log("Close Status= %d",status);
			}
			break;
	 case 4 :
			printInfo();
			break;
	case 5 :
			{
				int closeStatus=-1;
				closeStatus=close_diag_port();
			//	printf_packetStats();
				log("\n Everything is Close Status= %d",closeStatus);
				printf("To reInitialise Select InitDiag");
			}
			 break;
	case 6 :
			{
				printf("Select 1 : Airtel\n"
					 "Select 2 : Idea\n"
					 "Select 3 : Relience");
			scanf("%d",&urfcnselect);
			switch(urfcnselect)
			{
				case 1:
					set_uarfn_value(10807);
					break;
				case 2:
					set_uarfn_value(10707);
					break;
				case 3:
					set_uarfn_value(10833);
					break;
				default:
					 printf("\n Invalid option");
					 break;
			 }
   }
	break;

	case 7 :
			{
				printf("\n WCDMA UARFCN = %d " ,get_uarfn_value());
			}
			break;
	case 8 :
	{
		 char Version[20];
		 get_DM_version(Version);
		 printf("Version=%s",Version);
	}
	break;

	case 9:
	{
		/*int event = EVENT_RRC_STATE |EVENT_MM_STATE|EVENT_GMM_STATE|EVENT_InterRat_Handover_Start|
				EVENT_InterRat_Handover_End | EVENT_CallDrop| EVENT_LTE_MOBILITY_E_UTRA_FAILED |
				EVENT_LTE_MOBILITY_E_UTRA_STARTED |  EVENT_LTE_HANDOVER_FAILURE | EVENT_LTE_INTERAT_REDIR_END
				| EVENT_LTE_INTERAT_REDIR_START | EVENT_LTE_RRC_STATE_CHANGE ;*/
		int event = 0xffffffff;
		set_event_mask(0xffffffff);
	}
	break;
	case 10:
		initport();
		break;
	case 11:
	{
		if (networktype==3)
		{
			int device8690 ;
			printf ("\nIf device is S3 enter 1, otherwise 0");
			scanf("%d",&device8690);
			printf("\n Enter the frequency band to be set :-");
			scanf("%llx",&freqBand);
			set_lte_freqband(freqBand,device8690);
		}
		else {
			printf("\nThis feature is only for LTE device");
		}
	}
	break;
	case 12:
	{
		if (networktype==3)
		{

			u_int64_t freqBand;
			int device8690 ;
			printf ("\nIf device is S3 enter 1 ,otherwise 0");
			scanf("%d",&device8690);
			freqBand = get_lte_freqband(device8690);
			printf("\nLTE Band  = %llx \n",freqBand);
		}
		else printf("\nThis feature is only for LTE device");
	}
	break;
	case 13 :
	{
				printf("\nCurrent Radio Tech = %d",get_state_info());
	}
	break;
	case 14 :
	{
				 printf("Enable for WCDMA Dlf File Write");
				 setWCDMALogMask(WCDMA_LOG_ENABLE_ALL);
	}
	break;
	case 15 :{
				 printf("Enable for LTE Dlf File Write");
				 setLTELogMask(LTE_LOG_ENABLE_ALL);
	}
	break;
	case 16 :{
				printf("Enable for GSM Dlf File Write");
				setGSMLogMask(GSM_LOG_ENABLE_ALL);
	}
	break;
	case 17 :{
				printf("Enable for CDMA Dlf File Write");
				setCDMALogMask(CDMA_LOG_ENABLE_ALL);
	}
	break;
	case 18 :{
				char FilePath[5000] = {0};
				 DLFtestCode++;
				 sprintf(DLFtestid,"DLFT%03d",DLFtestCode);
				 printf("DLFTestId = %s",DLFtestid);
				 printf("Enter path for dlf file : ");
				 scanf ("%s", FilePath);
				 setDLFFilePath(FilePath ,strlen(FilePath));
				 set_DlfGeoLocationInfomation(0.0,345.1234,100000,33333333);
				 startDlfFileWrite( DLFimsi,DLFtestid,DLFimsilen,7);
	}
	break;
	case 19 :{
			printf("\nDLF FILE STOP = %d",stopDlfFileWrite());
	}
	break;
	case 20 :{
			printf("\n close ISFDM = %d",closeISFDM());
	}
	break;
	case 21 :{
				printf("GpsCommandwriteStatus = %d",set_gps_last_position());
	}
	break;

	case 22:{
		if (networktype==3)
		{
			unsigned int interval;
			printf("Enter interval value as 1=500ms OR 65535 to display at end \n");
			scanf("%d", &interval);
			SetLtePhysicalLayerReporting (ENABLE,interval);
		}
		else printf("\nThis feature is only for LTE device");
	}
	break;
	case 23:{
		if (networktype==3)
		{
			SetLtePhysicalLayerReporting (DISABLE ,0);
		}
		else printf("\nThis feature is only for LTE device");
	}
	break;
	case 24:{
		if (networktype==3)
		{
		//	printf("code 24 selected \n ");
			LTEDownLinkPhyStatus downlinkstatus;
			LTEUplinkPhyStatus uplinkstatus;
			get_lte_downlink_phy_status(&downlinkstatus);
			get_lte_uplink_phy_status(&uplinkstatus);
			print_lte_phy_status();
		}
				else printf("\nThis feature is only for LTE device");
		}
		break;
	case 25:{
		char FilePath[5000] = {0};
		printf("Enter path for debug file : ");
		scanf ("%s", FilePath);
		setDebugFilePath(FilePath, strlen(FilePath));
	}
	break;

	default :
		printf("Incorrect Input \n");
	   break;
	  }
   }
}
}
void initport(){

	  int devicetype;
      char GUIDInformation[]="ABCDEFGHIJK";
	  int guidlen=11;
	  int testcode =0;
	  int status1=-1;
	  double lat= 1234567.12345;
	  double longitude=1234567.1234;
	  double altitude=1234567.1234;
	  int speed=100;
	  int direction=1231;
	  int locationAccurancy=89;
	  printf("\nSelect Your Device\n"
			   "Enter 0 : For HTC\n"
	     		"Enter 1 : For SAMSUNG\n");
	  scanf("%d",&devicetype);
	  printf("\nSelect Your Network :\n"
			  "Enter 1 : For Gsm\n"
			  "Enter 2 : For Wcdma\n"
			  "Enter 3 : For Lte\n"
			  "Enter 4 : For Gsm+Wcdma\n"
			  "Enter 5 : For Wcdma+Lte\n"
			  "Enter 6 : For Gsm+Wcdma+Lte\n"
			  "Enter 7 : For DLF_Wcdma\n"
	  	  "Enter 8 : For DLF_LTE\n"
        "Enter 9 : For DLF_GSM\n"
        "Enter 10: For CDMA\n"
        "Enter 11: For DLF_CDMA\n");

      scanf("%d",&networktype);
	  switch(networktype){
		  case 1:
			  // Enable GSM  Network.
			   status1 = init_diag_port(devicetype,GSM, mycallback,"/data/config.txt");
			   set_GUIDInformation(GUIDInformation,guidlen);
			   set_GeoLocationInfomation (lat,longitude,altitude,speed,direction,locationAccurancy);
		  break;
		  case 2:
			  // Enable WCDMA  Network
			   status1 = init_diag_port(devicetype,WCDMA, mycallback,"/data/config.txt");
			   set_GUIDInformation(GUIDInformation,guidlen);
			   set_GeoLocationInfomation (lat, longitude,altitude,speed,direction,locationAccurancy);
		  break;
		  case 3:
				// Enable LTE  Network.
			   status1 = init_diag_port(devicetype,LTE, mycallback,"/data/config.txt");
				set_GUIDInformation(GUIDInformation,guidlen);
				set_GeoLocationInfomation (lat, longitude,altitude,speed,direction,locationAccurancy);
		  break;
		  case 4 :
				// Enable GSM+WCDMA  Network.
			   status1 = init_diag_port(devicetype,GSM, mycallback,"/data/config.txt");
				status1 = init_diag_port(devicetype,WCDMA, mycallback,"/data/config.txt");
				set_GUIDInformation(GUIDInformation,guidlen);
				set_GeoLocationInfomation (lat, longitude,altitude,speed,direction,locationAccurancy);
		  break;
		  case 5:
				// Enable WCDMA+LTE  Network
				status1 = init_diag_port(devicetype,WCDMA, mycallback,"/data/config.txt");
				set_GUIDInformation(GUIDInformation,guidlen);
				set_GeoLocationInfomation (lat, longitude,altitude,speed,direction,locationAccurancy);
				status1 = init_diag_port(devicetype,LTE, mycallback,"/data/config.txt");
		  break;
		  case 6:
				// Enable GSM+WCDMA+LTE  Network
			    status1 = init_diag_port(devicetype,GSM, mycallback,"/data/config.txt");
				status1 = init_diag_port(devicetype,WCDMA, mycallback,"/data/config.txt");
				status1 = init_diag_port(devicetype,LTE, mycallback,"/data/config.txt");
				set_GUIDInformation(GUIDInformation,guidlen);
				set_GeoLocationInfomation (lat, longitude,altitude,speed,direction,locationAccurancy);
          break;
		  case 7:
		  	  {
		  		 openISFDM(devicetype,DLF_WCDMA ,"/data/config.txt");
		  	  }
		  break;
		  case 8 :
			  {
				  openISFDM(devicetype,DLF_LTE ,"/data/config.txt");
			  }
		  break;
		  case 9 :
			  {
				  openISFDM(devicetype,DLF_GSM,"/data/config.txt");
			  }
		  break;
      case 10 :
      	  	  {
      	  		    status1 = init_diag_port(devicetype,CDMA, mycallback,"/data/config.txt");
					set_GUIDInformation(GUIDInformation,guidlen);
					set_GeoLocationInfomation (lat, longitude,altitude,speed,direction,locationAccurancy);
			  }
		  break;
		  case 11 :
			  {
				  openISFDM(devicetype,DLF_CDMA,"/data/config.txt");
			  }
		break;
		default :
			printf("\nNetwork Type not found ");
			break;
	  }
}



void printInfo()
{

	  if((networktype==1)||(networktype==4)||(networktype==6)){
   	     GSMCellInfo gGSMCellInfo;
		  get_GSMCellInfo(&gGSMCellInfo);
		  log("\n**********************GSM Information**************************");
		  long *packettime = (long*)&(gGSMCellInfo.timestamp[0]);
		  //time_t time_span =time(packettime);// convert the long time format into human readable format
		  log("\n Time  :- %s", ctime((const time_t*)packettime));
		  log(" MCC = %s ",gGSMCellInfo.MCC);
		  log("\n MNC = %s ",gGSMCellInfo.MNC);
		  log(" \nPhysical_Cell_Id = %d ",gGSMCellInfo.cell_id);
		  log("\n LAC = %d",gGSMCellInfo.LAC);
		  log("\n CGI = %s ",gGSMCellInfo.CGI);
		  log("\n BcchARFCN = %d ",gGSMCellInfo.bcchARFCN);
		  log("\n Bsic_plmn_color = %d ",gGSMCellInfo.BSIC[0]);
		  log("\n Bsic_bs_color = %d",gGSMCellInfo.BSIC[1]);
		  log("\n LAI[1] = %d",gGSMCellInfo.LAI[0]);
		  log("\n LAI[2] = %d",gGSMCellInfo.LAI[1]);
		  log("\n LAI[3] = %d",gGSMCellInfo.LAI[2]);
		  log("\n LAI[4] = %d",gGSMCellInfo.LAI[3]);
		  log("\n LAI[5] = %d",gGSMCellInfo.LAI[4]);
		  log("\n Cell_Selection_Priority = %d",gGSMCellInfo.Cell_Selection_Priority);
		  log("\n NCCPermit = %d ",gGSMCellInfo.NCCPermit);
		  log("\n Rxlev_arg = %d ",gGSMCellInfo.rxlev_arg);
		  log("\n  c1= %d ",gGSMCellInfo.c1);
		  log("\n  c2= %d ",gGSMCellInfo.c2);
		  log("\n  RXLEV_FULL_AVG = %d ",gGSMCellInfo.Rxlev_Full_Avg);
		  log("\n  RXLEV_SUB_AVG = %d ",gGSMCellInfo.Rxlev_Sub_Avg);
		  log("\n  RXQUAL_FULL_AVG  = %d ",gGSMCellInfo.Rxqual_Full_Avg);
		  log("\n  RXQUAL_SUB_AVG  = %d ",gGSMCellInfo.Rxqual_Sub_Avg);
		  log("\n  No_of_neigbourcell= %d ",gGSMCellInfo.no_of_neigbourcell);
		  int count = 0;
		  for (count = 0 ;count<gGSMCellInfo.no_of_neigbourcell;count++){
			  log("\n  BccnNeigARFCN= %d ",gGSMCellInfo.neigCellInfo[count].bccnNeigARFCN);
			  log("\n  Rxlev_arg= %d ",gGSMCellInfo.neigCellInfo[count].rxlev_arg);
			  log("\n  C1= %d ",gGSMCellInfo.neigCellInfo[count].c1);
			  log("\n  C2= %d ",gGSMCellInfo.neigCellInfo[count].c2);
			  log("\n serving_LAI= %d ",gGSMCellInfo.neigCellInfo[count].serving_LAI);
		  }
		  log("\n*****************************************************************");
	  }
   	  if ((networktype==3)||(networktype==5)||(networktype==6)||(networktype==8)){
		  LTECellInfo mLTE_CellInfo;
		  LTEThroughput mLTEThroughput;
		  get_LTECellInfo(&mLTE_CellInfo);
		  get_LTEThroughput (&mLTEThroughput);
		  log("\n**********************LTE Information **************************");
		  long *packettime = (long*)&(mLTE_CellInfo.timestamp[0]);
		  //time_t time_span =time(packettime);// convert the long time format into human readable format
		  log("\n Time  :- %s", ctime((const time_t*)packettime));
		  log(" Physical_Cell_Id = %d ",mLTE_CellInfo.physical_cell_id);
		  log("\n EARFCN_Downlink = %d ",mLTE_CellInfo.dl_freq);
		  log("\n EARFCN_Uplink = %d ",mLTE_CellInfo.ul_freq);
		  log("\n Downlink_bandwidth = %d MHz",mLTE_CellInfo.dl_bandwidth);
		  log("\n Uplink_bandwidth = %d MHz",mLTE_CellInfo.ul_bandwidth);
		  log("\n Cell_identity = %d ",mLTE_CellInfo.cell_identity);
		  log("\n Tracking_area_code = %d ",mLTE_CellInfo.tracking_area_code);
		  log("\n Frequency_band_indicator = %d ",mLTE_CellInfo.frequency_band_indicator);
		  log("\n MCC = %d ",mLTE_CellInfo.mcc);
		  log("\n No of MNC Digits = %d ",mLTE_CellInfo.num_of_mnc_digits);
		  log("\n MNC = %d ",mLTE_CellInfo.mnc);
		  log("\n No of Tx Antennas = %d ",mLTE_CellInfo.num_tx_antennas);
		  log("\n Cell_timingRx0 = %d ",mLTE_CellInfo.cell_timingRx0);
		  log("\n Cell_timingRx1 = %d ",mLTE_CellInfo.cell_timingRx1);
		  log("\n Inst_rsrp_rx0  = %f dbm ",mLTE_CellInfo.inst_rsrp_rx0);
		  log("\n Inst_rsrp_rx1 = %f dbm ",mLTE_CellInfo.inst_rsrp_rx1);
		  log("\n Inst_rsrp = %f dbm ",mLTE_CellInfo.inst_rsrp);
		  log("\n Inst_rsrq_rx0  = %f db",mLTE_CellInfo.inst_rsrq_rx0);
		  log("\n Inst_rsrq_rx1 = %f db",mLTE_CellInfo.inst_rsrq_rx1);
		  log("\n Inst_rsrq = %f db",mLTE_CellInfo.inst_rsrq);
		  log("\n Inst_rssi_rx0  = %f dbm",mLTE_CellInfo.inst_rssi_rx0);
		  log("\n Inst_rssi_rx1 = %f dbm",mLTE_CellInfo.inst_rssi_rx1);
		  log("\n Inst_rssi = %f dbm ",mLTE_CellInfo.inst_rssi);
		  log("\n sinr_rx0 = %f db",mLTE_CellInfo.sinr_rx0);
		  log("\n sinr_rx1 = %f db",mLTE_CellInfo.sinr_rx1);
		  log("\n*********************************************************************");
		  log("\n********************** LTE Throughput *********************************");
		  log("\n Down LinkRLC Throughput = %f",mLTEThroughput.downlinkRLCThroughput);
		  log("\n Up LinkRLC Throughput = %f",mLTEThroughput.upLinkRLCThroughput);
		  log("\n*********************************************************************");

		/*  log("\n UL_MSC_Index_Average 	: %d ", mLteUplinkPhyStatus.UL_MSC_Index_Average);
		  log("\n  DL_MSC_Index_Average 	: %d ", mlte_downlink_phy_status.DL_MSC_Index_Average);
		  log("\n\n\n");

		  log("\n ****************""Lte Uplink Phy Status""****************************");
		  log("\n Lte Uplink Phy Status : UL_MSC_Index_Average 	: %d ", mLteUplinkPhyStatus.UL_MSC_Index_Average);
		  log("\n Lte Uplink Phy Status : UL_NUM_SubBand 			: %d ", mLteUplinkPhyStatus.UL_NUM_SubBand);
		  log("\n Lte Uplink Phy Status : UL_SubBand_CQI_CW_0 		: %d ", mLteUplinkPhyStatus.UL_SubBand_CQI_CW_0);
		  log("\n Lte Uplink Phy Status : UL_SubBand_CQI_CW_1 		: %d ", mLteUplinkPhyStatus.UL_SubBand_CQI_CW_1);
		  log("\n Lte Uplink Phy Status : Uplink_Rank_Index  		: %d ", mLteUplinkPhyStatus.Uplink_Rank_Index);
		  log("\n Lte Uplink Phy Status : Uplink_WideBand_CQI_CW_0	: %d ", mLteUplinkPhyStatus.Uplink_WideBand_CQI_CW_0);
		  log("\n Lte Uplink Phy Status : Uplink_Wideband_CQI_CW_1 : %d ", mLteUplinkPhyStatus.Uplink_Wideband_CQI_CW_1);
		  log("\n Lte Uplink Phy Status : Uplink_Wideband_PMI		: %d ", mLteUplinkPhyStatus.Uplink_Wideband_PMI);*/


	  }
	  if((networktype==2)||(networktype==4)||(networktype==5)||(networktype==6)||(networktype==7)){
		  WCDMACellInfo cellInfo;
		  get_WCDMACellInfo (&cellInfo);
		  int i,j,k,l,num_of_wcdma_cell,num_of_gsm_cell;
		  int NO_RANK =0;
		  //log ("\ncheck_freq_isset = %d",is_freq_set_success());
		  log("\n*******************WCDMA Information *******************************");
		  long *packettime = (long*)&(cellInfo.timestamp[0]);
		  //time_t time_span =time(packettime);// convert the long time format into human readable format
		  log("\n Time  :- %s", ctime((const time_t*)packettime));
		  log(" URAFCN_Downlink = %d ",cellInfo.URAFCN_Downlink);
		  log("\n URAFCN_Uplink = %d ",cellInfo.URAFCN_Uplink);
		  log("\n Cell_Id = %d ",cellInfo.cell_id);
		  log("\n Max_Tx_Power = %d ",cellInfo.Max_Tx_Power);
		  log("\n PRIM_SC_CODE = %d ",cellInfo.PRIM_SC_CODE);
		  log("\n Num_of_activeSet = %d ",cellInfo.Num_of_activeSet);
		  for(l=0;l<cellInfo.Num_of_activeSet;l++)
		  {
			  log("\n IS_reference_cell = %d ",cellInfo.activeSetInfo[l].is_reference_cell);
			  log("\n PRIM_SC_CODE = %d ",cellInfo.activeSetInfo[l].PRIM_SC_CODE);
			  log("\n ACELL_POS = %d ",cellInfo.activeSetInfo[l].ACELL_POS);
			  log("\n CHAN_CODE_IDX = %d ",cellInfo.activeSetInfo[l].CHAN_CODE_IDX);
			  log("\n CELL_TPC = %d ",cellInfo.activeSetInfo[l].CELL_TPC);
			  log("\n Diversity = %d ",cellInfo.activeSetInfo[l].Diversity);
			  log("\n ECIO = %f ",cellInfo.activeSetInfo[l].ECIO);
			  log("\n RSCP = %d ",cellInfo.activeSetInfo[l].RSCP);
		  }
		  log("\n Num_of_neigbourSet = %d ",cellInfo.Num_of_neigbourSet);
		  for(k=0;k <cellInfo.Num_of_neigbourSet;k++)
		  {
			  log("\n URAFCN_Downlink = %d ",cellInfo.neigbourSetInfo[k].URAFCN_Downlink);
			  log("\n PRIM_SC_CODE = %d ",cellInfo.neigbourSetInfo[k].PRIM_SC_CODE);
			  log("\n Diversity = %d ",cellInfo.neigbourSetInfo[k].Diversity);
			  log("\n NCELL_POS = %d ",cellInfo.neigbourSetInfo[k].NCELL_POS);
		  }
		  num_of_wcdma_cell=cellInfo.neigCellInfo.num_of_wcdma_cell;
		  log("\n Num_of_Wcdma_cell = %d ", num_of_wcdma_cell);
		  for (i=0;i<num_of_wcdma_cell;i++){
			  log("\n URAFCN_Downlink = %d ",cellInfo.neigCellInfo.wcdmaNeigbourData[i].URAFCN_Downlink);
			  log("\n psc = %d ",cellInfo.neigCellInfo.wcdmaNeigbourData[i].psc);
			  log("\n rscp = %d ",cellInfo.neigCellInfo.wcdmaNeigbourData[i].rscp);
			  if (cellInfo.neigCellInfo.wcdmaNeigbourData[i].rank_rscp == NO_RANK){
				  log("\n rank_rscp= No rank");
			  }else{
				  log("\n rank_rscp= %d ",cellInfo.neigCellInfo.wcdmaNeigbourData[i].rank_rscp);
			  }
			  log("\n ecio= %f ",cellInfo.neigCellInfo.wcdmaNeigbourData[i].ecio);
			  if (cellInfo.neigCellInfo.wcdmaNeigbourData[i].rank_ecio == NO_RANK){
				  log("\n rank_ecio= No rank  ");
			  }else
				  log("\n rank_ecio= %d ",cellInfo.neigCellInfo.wcdmaNeigbourData[i].rank_ecio);
			  }
		  num_of_gsm_cell=cellInfo.neigCellInfo.num_of_gsm_cell;
		  log("\n Num_of_Gsm_cell = %d ",num_of_gsm_cell);
		  for(j=0;j< num_of_gsm_cell;j++){
			  log("\n Arfcn = %d ",cellInfo.neigCellInfo.gsmNeigbourData[j].arfcn);
			  log("\n Bsic = %d ",cellInfo.neigCellInfo.gsmNeigbourData[j].bsic);
			  log("\n Rssi = %d ",cellInfo.neigCellInfo.gsmNeigbourData[j]. rssi);
			  if (cellInfo.neigCellInfo.gsmNeigbourData[i].rank == NO_RANK){
				  log("\n Rank= No rank");
			  }else
				  log("\n Rank = %d ",cellInfo.neigCellInfo.gsmNeigbourData[j].rank);
			  }
		  log("\n***********************************************************************");

	   }
	  if(networktype == 10){
		  int j;
		  Kpi_CDMA kpi_cdma;
		  if(!get_CDMA_Info(&kpi_cdma)){
			  log("\n ::::::::::::::::::CDMAKPI values :::::::::::::::::::::\n");
			  log("Active Pilot PN : %d \n",kpi_cdma.ActivePilotPN );
			  log("TxTotalPwr : %d \n",kpi_cdma.TxTotalPwr);
			  log("RxPwr_0 : %d \n",kpi_cdma.RxPwr_0);
			  log("RxPwr_1 : %d \n",kpi_cdma.RxPwr_1);
			  log("SNR : %f \n",kpi_cdma.SNR);
			  log("TxADJ : %d \n",kpi_cdma.TxADJ);
			  log("Frame_err_rate : %f \n",kpi_cdma.Frame_err_rate);
			  log("MAX Pilot : %d \n",kpi_cdma.maxPilot);

			  for (j = 0; j < kpi_cdma.maxPilot; j++)
			  {
				  log(" Pilot Info [%d] : PilotPN : %d \n", j, kpi_cdma.pilotInfo[j].pilotPN);
				  log(" Pilot Info [%d] : PilotECIO : %f \n", j, kpi_cdma.pilotInfo[j].EcIO);
				  log(" Pilot Info [%d] : C_I : %f \n", j, kpi_cdma.pilotInfo[j].C_I);
			  }
	  }
	  }


}



/*void test_filewrite(){

	int file_write(u_int16_t command_code,char *time,int timelen,unsigned char *buf,int length);
*/
