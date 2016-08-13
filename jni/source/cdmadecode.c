#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include "errors.h"
#include "interfaceapi.h"
#include "extmsgid.h"
#include "CdmaContext.h"
#include "cdmadecode.h"
#include "diag-init.h"


u_int32_t amss_target = 00;

#define MAX(a,b) (((a)>(b))?(a):(b))
#define antilog(x) pow(10,x)

typedef struct Version
{
	u_int32_t versionNo;
	u_int32_t numofsubPkt;
	u_int32_t index;

}Version;



/* This is dummy function */
void dummyfunc (CdmaKpi  *cdmaKpi,unsigned char *buf,int length ,int  index){

	d_log("\nNot need to decode");
}


/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:::::::::::::::::::::::::common commands function:::::::::::::::::::::::::::::*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void rf_subpacket_func (CdmaKpi *cdmaKpi,unsigned char *buf,int length,int  index )
{
	VERIFY_LENGTH(rf_subpacket ,length ,index);
	rf_subpacket *rfsp = (rf_subpacket*) &buf[index];
	index += sizeof(*rfsp);


	if(rfsp->tx_gain_adj<0x7F)
	{
		cdmaKpi->TxADJ=-((rfsp->tx_gain_adj)/2);
	}
	if(rfsp->tx_power > -32767)
	{
		cdmaKpi->TxTotalPwr = (rfsp->tx_power / 10);
	}
	cdmaKpi->RxPwr_0 = (-63.248 + ((rfsp->rx0_agc * 334 ) / 1000));
	cdmaKpi->RxPwr_1 = (-63.248 + ((rfsp->rx1_agc * 334 ) / 1000));
	cdmaKpi->bandClass = rfsp->rx0_band_class;

}

void active_subpacket_func (CdmaKpi *cdmaKpi,unsigned char *buf,int length,int  index )
{
	int i;
	u_int32_t *peak_pos;
	u_int16_t *peak_energy;
	VERIFY_LENGTH(active_info_subpacket ,length ,index);
	active_info_subpacket *aisp =(active_info_subpacket*) &buf[index];
	index += sizeof(*aisp);
	if ((aisp->max_num_peaks)%2==0)
	{
		VERIFY_LENGTH(active_information_even ,length ,index);
		active_information_even *aie = (active_information_even*) &buf[index];

		if ((aie->pilot_pn != 0xFFFF) && (aie->pilot_pn != 0))
				cdmaKpi->ActivePilotPN= aie->pilot_pn;

		index += sizeof(*aie);
		for (i=0; i < (aisp->max_num_peaks) ; i++)
		{
			{
				peak_pos = (u_int32_t*) &buf[index];
				index +=sizeof(*peak_pos);
			}
			for (i=0; i < (aisp->max_num_peaks) ; i++)
			{
				peak_energy = (u_int16_t*) &buf[index];
				index +=sizeof(*peak_energy);
			}
		}
	}
	else
	{
		VERIFY_LENGTH(active_information_odd ,length ,index);
		active_information_odd *aio = (active_information_odd*) &buf[index];
		if ((aio->pilot_pn != 0xFFFF) && (aio->pilot_pn != 0))
			cdmaKpi->ActivePilotPN= aio->pilot_pn;
		index += sizeof(*aio);
		for (i=0; i < (aisp->max_num_peaks) ; i++)
		{
			peak_pos = (u_int32_t*) &buf[index];
			index +=sizeof(*peak_pos);
		}
		for (i=0; i < (aisp->max_num_peaks) ; i++)
		{
			peak_energy = (u_int16_t*) &buf[index];
			index +=sizeof(*peak_energy);
		}
	}

}

void finger_subpacket_func (CdmaKpi *cdmaKpi,unsigned char *buf,int length,int  index )
{
	int i,pilotCount;
	unsigned int j;
	int rssi_value = 0 ;
	PilotInfo tempPilotInfo[MAX_PILOT];

	VERIFY_LENGTH(finger_info_subpacket ,length ,index);
	finger_info_subpacket *fisp = (finger_info_subpacket*)&buf[index];
	index += sizeof(*fisp);

	cdmaKpi->maxPilot = 0;
	pilotCount = 0;
	for(i=0;i<(fisp->num_fingers);i++)
	{
		finger_info *fi = (finger_info*) &buf[index];
		index += sizeof(*fi);

		if ((fi->pilot_pn != 0xFFFF) && (fi->pilot_pn != 0))
		{
				int pilotfound = 0;
				for (j = 0;j < pilotCount ; j++)
				{
					if(tempPilotInfo[j].pilotPN == fi->pilot_pn)
					{
						tempPilotInfo[j].EcIO = tempPilotInfo[j].EcIO + fi->rssi;
						pilotfound = 1;
						break;
					}
				}
			 	if (!pilotfound){
						tempPilotInfo[pilotCount].pilotPN= fi->pilot_pn;
						tempPilotInfo[pilotCount].EcIO = fi->rssi;
						pilotCount++;
				}
		 }
	}

	cdmaKpi->maxPilot= pilotCount;
	for (i = 0; i < pilotCount ; i++)
	{
		cdmaKpi->pilotInfo[i].pilotPN = tempPilotInfo[i].pilotPN;
		if ( cdmaKpi->bandClass == 0)
		{
			float ecio=0;
			ecio = ((tempPilotInfo[i].EcIO)/73728.0)-((1.0/64)*(0.125/1.875));
			cdmaKpi->pilotInfo[i].EcIO = 10*log10( MAX (0.00001, ecio ) );
		}
		else if (cdmaKpi->bandClass == 1)
		{
			float ecio=0;
			ecio = (((tempPilotInfo[i].EcIO)/73728.0)-((1.0/64)*(0.25/1.75)) );
			cdmaKpi->pilotInfo[i].EcIO = 10*log10 (MAX (0.00001, ecio ) );
		}
	}
}

void sleep_stats_subpacket_func (CdmaKpi *cdmaKpi,unsigned char *buf,int length,int  index )
{
	VERIFY_LENGTH(sleep_stats_subpacket,length ,index);
	sleep_stats_subpacket *sssp =(sleep_stats_subpacket*) &buf[index];
	index += sizeof(*sssp);

	if ((sssp->reacq_data_packet_id) == 1)
	{
		VERIFY_LENGTH(reacq_data_packet ,length ,index);
		reacq_data_packet *rdp = (reacq_data_packet*) &buf[index];
		int ecio;
		index += sizeof(*rdp);
		ecio = (log( (rdp->reacq_linear_ecio)/(9*(2^16))));
		if ((rdp->pilot_pn != 0xFFFF) && (rdp->pilot_pn != 0))
			cdmaKpi->ActivePilotPN = rdp->pilot_pn;
	}
	else if (((sssp->reacq_data_packet_id) == 2) || ((sssp->reacq_data_packet_id == 3)))
	{
		VERIFY_LENGTH(prio_data_packet ,length ,index);
		prio_data_packet *pdp =(prio_data_packet*) &buf[index];
		index += sizeof(*pdp);
	}

	if (((sssp->technology_data_packet_id)==1) || ((sssp->technology_data_packet_id)==2) || ((sssp->technology_data_packet_id)==3))
	{
		VERIFY_LENGTH(technology_data_packet_online ,length ,index);
		technology_data_packet_online *tdpon = (technology_data_packet_online*) &buf[index];
		index += sizeof(*tdpon);
	}
	else if ((sssp->technology_data_packet_id)==4)
	{
		VERIFY_LENGTH(technology_data_packet_offline ,length ,index);
		technology_data_packet_offline *tdpof = (technology_data_packet_offline*) &buf[index];
		index += sizeof(*tdpof);
	}
}

/*
 * Function to decode the version of Generalized_searcher
*/
void cdma_generalized_searcher(Version *version,unsigned char *buf ,int length)
{
	VERIFY_LENGTH(CdmapktVesion,length,version->index);
	CdmapktVesion *cdmapktVesion =(CdmapktVesion *) &buf[version->index];
	version->versionNo = cdmapktVesion->version;
	version->numofsubPkt = cdmapktVesion->numofsubPkt;
	version->index = sizeof(CdmapktVesion);
}

void  cdma_extract_subpkt_header(subpacket_header *Subpacket_header ,unsigned char *buf,int length,int  index)
{
		VERIFY_LENGTH(subpacket_header ,length ,index);
		subpacket_header *header = (subpacket_header *) &buf[index];
		Subpacket_header->subpacket_id = header->subpacket_id;
		Subpacket_header->subpacket_version = header->subpacket_version;
		Subpacket_header->subpacket_size = header->subpacket_size;
}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::::::::::::::::::::::::::::Evdo functions::::::::::::::::::::::::::::::::::::*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
#define MAX_CARRIER 2
typedef struct {
	int num_carrier;
	int dbmValues[MAX_CARRIER];
}LastValueInDB;


float convertIntoDBM(LastValueInDB LastValueDB )
{
	int i;
	float TotalLinearValue = 0, val=0;

	/******find anti log and add both*****/
	for (i=0; i<MAX_CARRIER; i++)
	{
		val =(LastValueDB.dbmValues[i])/10 ;
		TotalLinearValue = TotalLinearValue + antilog(val);
	}
	/******convert dbm again*******/
	val = (10 * log(val));
	return val;

}

void evdo_multicarrier_power_func(CdmaKpi  *cdmaKpi,unsigned char *buf,int length, int  index )
{
	int i=0;

	LastValueInDB TxAdjPower;

	VERIFY_LENGTH(evdo_multicarrier_power ,length ,index);
	evdo_multicarrier_power *emcp = (evdo_multicarrier_power*) &buf[index];
	index += sizeof(*emcp);
	TxAdjPower.num_carrier = emcp->num_carrier;

	for(i=0;i<emcp->num_carrier;i++)
	{
		carrier_info *ci = (carrier_info*) &buf[index];
		index += sizeof(*ci);

		cdmaKpi->TxADJ = ((emcp->carrier_data[i].tx_closed_loop_adj) / 256);
		TxAdjPower.dbmValues[i]= ((emcp->carrier_data[i].tx_closed_loop_adj) / 256);
		cdmaKpi->RxPwr_0 = ((emcp->carrier_data[i].rx_agc0) / 256);
		cdmaKpi->RxPwr_1 = ((emcp->carrier_data[i].rx_agc1) / 256);
	}
	if(emcp->total_tx_power != 0xFFFFFFFF) {
		cdmaKpi->TxTotalPwr = ((emcp->total_tx_power) / 256);
	}
}

void evdo_finger_func(CdmaKpi  *cdmaKpi,unsigned char *buf,int length, int  index )
{
	int i;
	VERIFY_LENGTH(evdo_finger ,length ,index);
	evdo_finger *ef = (evdo_finger*) &buf[index];
	index += sizeof(*ef);

	for(i=0;i<ef->num_fingers;i++)
	{
		finger_struct *fs = (finger_struct*) &buf[index];
		index += sizeof(*fs);

		if (fs->pilot_pn != 0xFFFF)
		{
			if((fs->pilot_pn) == (cdmaKpi->pilotInfo[i].pilotPN))
				cdmaKpi->pilotInfo[i].C_I = (10.0 * log(((float)fs->rssi)/512.0));
		}
	}

}

void evdo_power_func(CdmaKpi  *cdmaKpi,unsigned char *buf,int length, int  index )
{
	int i;
	LastValueInDB values_for_tx_power = {0} ;
	LastValueInDB values_for_adj_power = {0};
	float value_in_dbm;
	/*FIX ME FOR DUAL CARRIERs*/
	for (i=0;i<2;i++)
	{
		VERIFY_LENGTH(evdo_power ,length ,index);
		evdo_power *ep = (evdo_power*) &buf[index];
		index += sizeof(*ep);
		if(ep->tx_total_power > -32768)
			values_for_tx_power.dbmValues[i] = ((ep->tx_total_power) / 256);
		if(ep->tx_closed_loop_adj != 0x7F)
			values_for_adj_power.dbmValues[i] =  ((ep->tx_closed_loop_adj) / 256);
	}

		value_in_dbm = convertIntoDBM(values_for_tx_power);
		cdmaKpi->TxTotalPwr = value_in_dbm ;

		value_in_dbm = convertIntoDBM(values_for_tx_power);
		cdmaKpi->TxADJ =  value_in_dbm;
}


void evdo_air_link_summary_func(CdmaKpi  *cdmaKpi,unsigned char *buf,int length, int  index )
{
	int total_sinr = 0, i;
	float sinr_db = 0 , a=0;
	float per_long_old =0;
	VERIFY_LENGTH(evdo_air_link_summary ,length ,index);
	evdo_air_link_summary *eals = (evdo_air_link_summary*) &buf[index];
	index += sizeof(*eals);
	for(i=0;i<8;i++)
	{
		total_sinr = total_sinr + eals->Best_ASP_SINR_Buffer[i];
	}

	if(total_sinr != 0)
		sinr_db = 10.0*log((float)total_sinr / 512.0);
	cdmaKpi->SNR = sinr_db;

}


/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *:::::::::::::::::::::::Response functions:::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void temporal_analyser_graph_response_func(CdmaKpi  *cdmaKpi,unsigned char *buf,int length, int  index )
{
	int i;
	int old_error_frame = 0;
	int old_total_frame = 0;
	float avg_bad_frame = 0.0;

	VERIFY_LENGTH(temporal_analyser_graph_response ,length ,index);
	temporal_analyser_graph_response *tagr = (temporal_analyser_graph_response*) &buf[index];



	old_error_frame = cdmaKpi->bad_frames;
	old_total_frame = cdmaKpi->total_frames;

	if ((tagr->total_frames - old_total_frame) != 0)
		avg_bad_frame = ((((float)tagr->bad_frames - old_error_frame) / ((float)tagr->total_frames - old_total_frame)) *100.0) ;


	cdmaKpi->bad_frames = tagr->bad_frames;
	cdmaKpi->total_frames = tagr->total_frames;
	cdmaKpi->Frame_err_rate = avg_bad_frame;
}




/*************************Function pointer and array******************************************/
typedef void (*func)(CdmaKpi  *cdmaKpi,unsigned char *buf,int length, int  index );

const func funcArray[]=
{
		dummyfunc,dummyfunc, rf_subpacket_func, active_subpacket_func, finger_subpacket_func,dummyfunc,dummyfunc,dummyfunc, sleep_stats_subpacket_func, dummyfunc, dummyfunc,dummyfunc, dummyfunc, dummyfunc, dummyfunc, dummyfunc, dummyfunc, dummyfunc, dummyfunc,dummyfunc, dummyfunc, dummyfunc, dummyfunc
};


/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *:::::::::::::::::::::CDMA DECODE COMMANDS:::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void cdma_decode_log_msg(CdmaKpi *cdmaKpi, int code, unsigned char *buf , int length) //u_int8_t timestamp[8])
{
	switch(code)
	{
/******************************COMMON FUNCTION CALLS***************************************************/
	case SRCH_TNG_FINGER_STATUS :
	case SRCH_TNG_1X_SEARCHER_DUMP :
	case SRACH_TNG_GENERAL_STATUS :
	case SRCH_TNG_DEMOD_INFO :
	case SRCH_TNG_SLEEP_STATS :
	case SRCH_TNG_FINGER_CHANNEL_ESTIMATE :
	{
		int index = 0;
		Version  version = {0};
		subpacket_header subpacket_header = {0};
		/* get the version number */
		cdma_generalized_searcher(&version ,buf ,length);
		if(version.versionNo == 1)
		{
			index += version.index;
			while (version.numofsubPkt)
			{
				/* Get the subPkt and length */
				cdma_extract_subpkt_header(&subpacket_header ,buf ,length , index);
				index += sizeof(subpacket_header);
				if((subpacket_header.subpacket_id >= SEARCHER_STATUS_SUBPACKET)&&(subpacket_header.subpacket_id <= IDLE_DMOD_STATS_SUBPACKET))
				{
					funcArray[subpacket_header.subpacket_id](cdmaKpi ,buf ,length ,index);
					index += (subpacket_header.subpacket_size - 4);/*  */
					version.numofsubPkt -= 1;
				}
				else
				{
					index += (subpacket_header.subpacket_size -4);
					version.numofsubPkt -= 1;
				}
			}
		}
		break;
	}

/**************************EVDO FUNC CALLS*****************************/
	case EVDO_POWER_LOG :
		{
			int index = 0;
			evdo_power_func(cdmaKpi, buf,length,index );
			break;
		}
	case EVDO_FINGER_LOG :
		{
			int index = 0;
			evdo_finger_func(cdmaKpi, buf,length,index );
			break;
		}
	case EVDO_MULTICARRIER_POWER :
		{
			int index = 0;
			evdo_multicarrier_power_func(cdmaKpi, buf,length,index );
			break;
		}
	case EVDO_AIR_LINK_SUMMARY :
		{
			int index = 0;
			evdo_air_link_summary_func(cdmaKpi, buf,length,index);
			break;
		}


/**************************Response FUNC CALL*****************************/
	case  TEMPORAL_ANALYZER_GRAPH :
		{
			int index = 0;
			temporal_analyser_graph_response_func(cdmaKpi, buf,length,index);
			break;
		}




	default : d_log("\nCDMA Unknown Code = %d \n",code) ;
	break;

	}
}
