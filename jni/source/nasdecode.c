#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <sys/types.h>
#include "nasdecode.h"
#include "interfaceapi.h"
#include "extmsgid.h"
#include "errors.h"
#ifndef HOST
#include "nasDecoderWrapper.h"
#include "csnRRDecoder.h"
#endif
#include "filewriter.h"

extern bool getCCEventEnableStatus();

void  nas_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8])
{

	switch(code)
	{
    	case UMTS_NAS_GMM_STATE :
    	{
    		d_log("\n -----GMM State LOGGING -------");
    		d_log("\nNAS Command code :-  0x%x",code);
    		d_log("\nTime :- %s", ctime(&time));
    	}
    	break;
    	case UMTS_NAS_MM_STATE :
    	{
    		d_log("\n -----MM State LOGGING-------");
    		d_log("\nNAS Command code :-  0x%x",code);
    		d_log("\nTime :- %s", ctime(&time));
    	}
    	break;
    	case UMTS_NAS_SIGNALING :
    	{
			if(getCCEventEnableStatus()) { 
				/* send cc events only if they are enabled */
				u_int64_t cmdmillSeconds = 0;
				cmdTimestamp *cts = (cmdTimestamp *)&timestamp[0];
				cmdmillSeconds = ((unsigned long long)cts->counter*125)/100;
				d_log("\nNAS Command code :-  %d",code);
				d_log("\nTime :- %s", ctime(&time));
				/* Check for incoming buffer length */
				VERIFY_LENGTH(umts_nas_signaling , length , 0);
				umts_nas_signaling *uns = (umts_nas_signaling*)&buf[0];
			#ifndef HOST		//added by me to test compilation
                        #ifndef MINIMUM_DM
				NASDecoderCCevents(buf, length, cmdmillSeconds);
  			#endif
			#endif
    		}
    	}
    	break;
    	case UMTS_UE_DYNAMIC_ID:
    	{
    		umts_ue_dynamic_id *uudi = (umts_ue_dynamic_id *)&buf[0];
    		if(uudi->tmsi_ptmsi == 0){
    			//function to set tmsi in decoder;
			#ifndef HOST		//added by me to test compilation
			#ifndef MINIMUM_DM
    			setTMSI(uudi->p_tmsi);
			#endif
			#endif
    		}
    		break;
    	}
    	default:
    		d_log("\nUMTS Unknown packet = %d \n",code);
    		break;
	}
}




