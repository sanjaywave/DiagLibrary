#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <math.h>
#include <sys/types.h>
#include <math.h>
#include "wcdmadecode.h"
#include "interfaceapi.h"
#include "extmsgid.h"
#include "errors.h"
#ifndef HOST
#include "rrcDecoder.h"
#endif
#include "filewriter.h"

#define maxCount 10

#define CALCULATE_UL_UARFCN(_kpi,_dluarfcn)\
{\
	{\
		_kpi = ((_dluarfcn/5.0)-190)*5.0;\
	}\
}

static time_t wcdmatime;
#if CHECK_STATS
static int count_4005;
static int count_4110;
static int count_4111;
static int count_410c;
static int count_4125;
static int count_4127;
static int count_412f;
static int count_401c;
static int count_4222;
static int count_4311;
static int count_420b;
static int count_4214;
static int count_4213;
static int count_4212;
static int count_4211;
static int count_4208;
static int count_414a;
static int count_4160;
static int count_4138;
static int count_4116;
static int count_410d;
#endif
extern WCDMACellInfo myCellInfo;
static int connectionState = 0;   //1 for connected and 0 for disconnected
static int8_t numOfHits = 0;
static int is401cPresent;
static int8_t rxPower_4179;

struct perCellENGaccount {
	u_int8_t cell_index;
	u_int64_t ENG;
	u_int16_t nc;
	u_int8_t nn;
	u_int16_t psc;
	u_int8_t isActiveSet;
};
typedef struct perCellENGaccount  perCellENGaccount;

extern bool getCCEventEnableStatus();
/* 
 * This variable is updated only when ACtiveSetUpdate is comming
 * 1 means updated else 0
 */
//static IsActiveSetCmdUpdated = 0; 

struct localActiveSet {
	float ECIO;
	int8_t RSCP;
	u_int16_t PRIM_SC_CODE;
};
typedef struct localActiveSet  localActiveSet;

struct localNeighbourSet {
	float ECIO;
	int8_t RSCP;
	u_int16_t PRIM_SC_CODE;
	int8_t isInterFreq;
};
typedef struct localNeighbourSet  localNeighbourSet;

struct connectedWCDMApara {
	u_int8_t Num_of_activeSet;
	u_int8_t Num_of_neigbourSet;
	localActiveSet localactiveset[MAX_WCDMA_ACTIVE_SET];
	localNeighbourSet localneighbourset[MAX_WCDMA_NEIG_SET];
};
typedef struct connectedWCDMApara  connectedWCDMApara;

connectedWCDMApara connectedwcdmapara = {0};

float getECIO(u_int16_t N, u_int8_t M, u_int64_t Y, int isForTempAnalysisE2) {
	float ECIO = 0;
	if(isForTempAnalysisE2) {
		if((N == 2048) && (M == 1)) {
			ECIO = 10*log10f((Y - 0.00220 * M * N)/(0.00220 * M * N * N));
		} else if((N == 2048) && (M == 2)) {
			ECIO = 10*log10f((Y - 0.00110 * M * N)/(0.00110 * M * N * N));
		} else if(N == 128) {
			ECIO = 10*log10f((Y - 0.0175 * M * N)/(0.0273 * M * N * N));
		} else if(N == 1024) {
			ECIO = 10*log10f((Y - 0.00878 * M * N)/(0.00878 * M * N * N));
		} else if((N == 256) && (M == 16)) {
			ECIO = 10*log10f((Y - 0.00876 * M * N)/(0.00876 * M * N * N));
		} else {
			ECIO = 0;
			//printf("wrong ecio, nc : %u, nn : %u, energy : %u\n", N, M, Y);
		}
	} else {
		ECIO = 10*log10f((Y - 0.1406 * M * N)/(0.1406 * M * N * N));
	}
	//printf("\n\n\n\necio : %f\n", ECIO);
	return ECIO;
}

void insertLocalCellInfo(u_int32_t psc, float ECIO, int8_t RSCP, int8_t network_type) {
	int i = 0, activeSetFound = 0, neigbSetFound = 0;

	if(network_type == 0) {
		for (i = 0; i < connectedwcdmapara.Num_of_activeSet; i++){
			if(connectedwcdmapara.localactiveset[i].PRIM_SC_CODE == psc) {
				connectedwcdmapara.localactiveset[i].ECIO = ECIO;
				connectedwcdmapara.localactiveset[i].RSCP = RSCP;
				activeSetFound = 1;
				break;
			}
		}
		if(!activeSetFound) {
			connectedwcdmapara.localactiveset[connectedwcdmapara.Num_of_activeSet].ECIO = ECIO;
			connectedwcdmapara.localactiveset[connectedwcdmapara.Num_of_activeSet].PRIM_SC_CODE = psc;
			connectedwcdmapara.localactiveset[connectedwcdmapara.Num_of_activeSet].RSCP = RSCP;
			connectedwcdmapara.Num_of_activeSet++;
		}
	} else {

		for (i = 0; i < connectedwcdmapara.Num_of_neigbourSet; i++){
			if(connectedwcdmapara.localneighbourset[i].PRIM_SC_CODE == psc) {
				connectedwcdmapara.localneighbourset[i].ECIO = ECIO;
				connectedwcdmapara.localneighbourset[i].RSCP = RSCP;
				connectedwcdmapara.localneighbourset[i].isInterFreq = (network_type & 0x1);
				neigbSetFound = 1;
				break;
			}
		}
		if(!neigbSetFound) {
			connectedwcdmapara.localneighbourset[connectedwcdmapara.Num_of_neigbourSet].ECIO = ECIO;
			connectedwcdmapara.localneighbourset[connectedwcdmapara.Num_of_neigbourSet].PRIM_SC_CODE = psc;
			connectedwcdmapara.localneighbourset[connectedwcdmapara.Num_of_neigbourSet].RSCP = RSCP;
			connectedwcdmapara.localneighbourset[i].isInterFreq = (network_type & 0x1);
			connectedwcdmapara.Num_of_neigbourSet++;
		}
	}
}

void updateGlobalStruct() {
	int count = 0;
	myCellInfo.Num_of_activeSet = connectedwcdmapara.Num_of_activeSet;
	myCellInfo.neigCellInfo.num_of_wcdma_cell = connectedwcdmapara.Num_of_neigbourSet;
	for(count = 0;count < myCellInfo.Num_of_activeSet; count++) {
		myCellInfo.activeSetInfo[count].ECIO = connectedwcdmapara.localactiveset[count].ECIO;
		myCellInfo.activeSetInfo[count].PRIM_SC_CODE = connectedwcdmapara.localactiveset[count].PRIM_SC_CODE;
		myCellInfo.activeSetInfo[count].RSCP = connectedwcdmapara.localactiveset[count].RSCP;

	}
	for(count = 0;count < myCellInfo.neigCellInfo.num_of_wcdma_cell; count++) {
		myCellInfo.neigCellInfo.wcdmaNeigbourData[count].ecio = connectedwcdmapara.localneighbourset[count].ECIO;
		myCellInfo.neigCellInfo.wcdmaNeigbourData[count].psc = connectedwcdmapara.localneighbourset[count].PRIM_SC_CODE;
		myCellInfo.neigCellInfo.wcdmaNeigbourData[count].rscp = connectedwcdmapara.localneighbourset[count].RSCP;
		if(connectedwcdmapara.localneighbourset[count].isInterFreq)
			myCellInfo.neigCellInfo.wcdmaNeigbourData[count].URAFCN_Downlink =0;
		else
			myCellInfo.neigCellInfo.wcdmaNeigbourData[count].URAFCN_Downlink = myCellInfo.URAFCN_Downlink;
	}

}

void  wcdma_decode_log_msg(int code,unsigned char *buf ,int length, u_int8_t timestamp[8]){

#ifdef DEBUG
	time (&wcdmatime);/* This will get current system time */
#endif
	switch(code)
	{
			case WCDMA_CELL_RESELECTION_PACKET :
			{
#ifdef MEMORY_CORRUPTION_REPORT
				d_warning("4005 command hit\n");
#endif
#if CHECK_STATS
				count_4005++;
#endif

				u_int32_t i,j, ActiveSetCount = 0;
				u_int32_t index = 0;
				d_log("\nCommand code :-%d",code);
				d_log("\nTime  :- %s", ctime(&wcdmatime));
				VERIFY_LENGTH(wcdma_cell_reselection_packet,length,index);
				/* Update time stamp */ 
				memcpy(myCellInfo.timestamp,buf-8 ,8);
				wcdma_cell_reselection_packet *wcrp=(wcdma_cell_reselection_packet*)&buf[0];
				myCellInfo.neigCellInfo.num_of_wcdma_cell = wcrp->num_wcdma_cells;
				myCellInfo.neigCellInfo.num_of_gsm_cell=wcrp->num_gsm_cells;
				index += sizeof(*wcrp);
				/* 
 				 * check wethere WCDMA_ACTIVE_SET cmd is comming or not
 				 * if not then myCellInfo.Num_of_activeSet = 0. 
 				 */
				myCellInfo.Num_of_activeSet = 0;
				for(i = 0;i < wcrp->num_wcdma_cells ; i++){
					VERIFY_LENGTH(wcdma_cell_reselection_packet_info,length,index);
					wcdma_cell_reselection_packet_info *wcrpi=(wcdma_cell_reselection_packet_info*)&buf[index];
					if(i == 0) {
						myCellInfo.Num_of_activeSet = 1;
						myCellInfo.activeSetInfo[0].is_reference_cell =1;
						myCellInfo.activeSetInfo[0].PRIM_SC_CODE = wcrpi-> psc;
						myCellInfo.URAFCN_Downlink = wcrpi->rf_channel;
						myCellInfo.PRIM_SC_CODE  = myCellInfo.activeSetInfo[0].PRIM_SC_CODE; 
						/* Calculate the UL_UARFCN */
						CALCULATE_UL_UARFCN(myCellInfo.URAFCN_Uplink ,wcrpi->rf_channel);
						if((-63 <= wcrpi->ecio) && (wcrpi->ecio <= 0))
							/* -63 <= ECIO <= 0*/
							myCellInfo.activeSetInfo[0].ECIO=(float)wcrpi->ecio/2.0;
						if((-128 <= wcrpi->rscp) && (wcrpi->rscp <= 0))
							/* -128 <= RSCP <= 0 */
							myCellInfo.activeSetInfo[0].RSCP=(wcrpi->rscp-21);
#ifdef MEMORY_CORRUPTION_REPORT
						d_warning("4005 asupdate\n");
						d_warning("ECIO : %f, PSC : %u\n\n", myCellInfo.activeSetInfo[0].ECIO, myCellInfo.activeSetInfo[0].PRIM_SC_CODE);
#endif //MEMORY_CORRUPTION_REPORT
					} else {
					myCellInfo.neigCellInfo. wcdmaNeigbourData[i-1].URAFCN_Downlink=wcrpi->rf_channel;
					myCellInfo.neigCellInfo. wcdmaNeigbourData[i-1].psc=wcrpi-> psc;
					if((-128 <= wcrpi->rscp) && (wcrpi->rscp <= 0))
						myCellInfo.neigCellInfo. wcdmaNeigbourData[i-1].rscp = (wcrpi->rscp-21);
					myCellInfo.neigCellInfo. wcdmaNeigbourData[i-1].rank_rscp=wcrpi->rank_cell_rscp;
					if((-63 <= wcrpi->ecio) && (wcrpi->ecio <= 0))
						myCellInfo.neigCellInfo. wcdmaNeigbourData[i-1].ecio=(wcrpi->ecio/2);
					if((-200 <= wcrpi->rank_ecio) && (0 >= wcrpi->rank_ecio))
						myCellInfo.neigCellInfo. wcdmaNeigbourData[i-1].rank_ecio=wcrpi->rank_ecio;
					//Bug found
					for(ActiveSetCount = 1;ActiveSetCount < myCellInfo.Num_of_activeSet;ActiveSetCount++){
						if(myCellInfo.activeSetInfo[ActiveSetCount].PRIM_SC_CODE == wcrpi-> psc){
							if((-63 <= wcrpi->ecio) && (wcrpi->ecio <= 0))
								/* -63 <= ECIO <= 0*/
								myCellInfo.activeSetInfo[ActiveSetCount].ECIO=(float)wcrpi->ecio/2.0;
							if((-128 <= wcrpi->rscp) && (wcrpi->rscp <= 0)){
								/* -128 <= RSCP <= 0 */
								myCellInfo.activeSetInfo[ActiveSetCount].RSCP=(wcrpi->rscp-21);
							}
						}
					}
					//if((i == 0) && (myCellInfo.Num_of_activeSet == 0)){
						/* This is done for S4 */

					//}
					}
					index +=sizeof(*wcrpi) ;
				}
				for(j = 0;j < wcrp->num_gsm_cells;j++){
					VERIFY_LENGTH(gsm_cell_reselection_packet_info,length,index);
					gsm_cell_reselection_packet_info *gcrpi = (gsm_cell_reselection_packet_info*) &buf[index];
					myCellInfo.neigCellInfo.gsmNeigbourData[j].arfcn= gcrpi->arfcn;
					myCellInfo.neigCellInfo.gsmNeigbourData[j].bsic= gcrpi->bsic;
					myCellInfo.neigCellInfo.gsmNeigbourData[j].rssi= gcrpi->rssi;
					myCellInfo.neigCellInfo.gsmNeigbourData[j].rank= gcrpi->rank;
					index += sizeof(*gcrpi);

				}
			}
		break;
		case WCDMA_ACTIVE_SET :
		{
#if CHECK_STATS
			count_4110++;
#endif
			u_int32_t index = 0 ,ActiveSetCount;
			d_log("\nCommand code :-  %d",code);
			d_log("\nTime  :- %s",ctime(&wcdmatime));
			VERIFY_LENGTH(wcdma_active_set,length,index);
			/*Update Time Stamp*/
			memcpy(myCellInfo.timestamp,buf-8 ,8);
			wcdma_active_set *was = (wcdma_active_set *)&buf[index];
			myCellInfo.Num_of_activeSet = was->num_cells;
			index += sizeof(*was);
			for(ActiveSetCount = 0 ;ActiveSetCount< was->num_cells ;ActiveSetCount++){
				VERIFY_LENGTH(wcdma_active_set_info,length,index);
				wcdma_active_set_info *wasi = (wcdma_active_set_info *)&buf[index];
				myCellInfo.activeSetInfo[ActiveSetCount].is_reference_cell = wasi->ref_cell ;
				if (wasi->ref_cell){
					myCellInfo.PRIM_SC_CODE = wasi->prim_sc_code;
				}
				myCellInfo.activeSetInfo[ActiveSetCount].PRIM_SC_CODE= wasi->prim_sc_code;
				myCellInfo.activeSetInfo[ActiveSetCount].CELL_TPC=wasi->cell_tpc;
				myCellInfo.activeSetInfo[ActiveSetCount].Diversity=wasi->cpi_div_cell;
				myCellInfo.activeSetInfo[ActiveSetCount].CHAN_CODE_IDX=wasi->chan_code_idx;
				myCellInfo.activeSetInfo[ActiveSetCount].ACELL_POS=wasi->acell_pos;
				index += sizeof(*wasi);
			}
		}
		break;
		case WCDMA_NEIGHBOR_SET :
		{
#if CHECK_STATS
			count_4111++;
#endif

			u_int32_t FrqCount, NeighCellCount ;
			int index = 0 ;
			d_log("\n Command code :-  %d",code);
			d_log("\nTime  :- %s", ctime(&wcdmatime));
			VERIFY_LENGTH(wcdma_neighbor_set,length,index);
			/*Update Time Stamp*/
			memcpy(myCellInfo.timestamp,buf-8 ,8);
			wcdma_neighbor_set *wns = (wcdma_neighbor_set*)&buf[0];
			index += sizeof(*wns);
			for(FrqCount = 0 ; FrqCount < wns->num_freq ; FrqCount++){
				VERIFY_LENGTH(wcdma_neighbor_set_freq,length,index);
				wcdma_neighbor_set_freq *wnsf=(wcdma_neighbor_set_freq *)&buf[index];
				myCellInfo.Num_of_neigbourSet = wnsf->num_neigh_cell;
				index += sizeof(*wnsf);
				for (NeighCellCount = 0; NeighCellCount < wnsf->num_neigh_cell ; NeighCellCount++){
					 VERIFY_LENGTH(wcdma_neighbor_set_cell,length,index);
					 wcdma_neighbor_set_cell *wnsc =  (wcdma_neighbor_set_cell*) &buf[index];
				 	 myCellInfo.neigbourSetInfo[NeighCellCount].URAFCN_Downlink = wnsf->freq_dl;
				 	 myCellInfo.neigbourSetInfo[NeighCellCount].PRIM_SC_CODE = wnsc->prim_scr_code;
				 	 myCellInfo.neigbourSetInfo[NeighCellCount].Diversity = wnsc->cpl_div_cell;
				 	 myCellInfo.neigbourSetInfo[NeighCellCount].NCELL_POS = wnsc->ncell_pos;
				  	 index += sizeof(*wnsc);
			 	 }
			}
		}
		break;
		case WCDMA_PHYSICAL_CHANNELS_UPLINKS:
		{
#if CHECK_STATS
			count_410c++;
#endif
			d_log("\nCommand code :-  %d",code);
			d_log("\nTime  :- %s", ctime(&wcdmatime));
			VERIFY_LENGTH(wcdma_physical_channels_uplinks,length,0);
			/*Update Time Stamp*/
			memcpy(myCellInfo.timestamp,buf-8,8);
			wcdma_physical_channels_uplinks *wpcu = (wcdma_physical_channels_uplinks *)&buf[0];
			myCellInfo.Max_Tx_Power=wpcu->max_tx_pwr;
		}
		break;
		case WCDMA_RRC_STATES :
		{
#if CHECK_STATS
			count_4125++;
#endif
			d_log("\nRRC State Command :- %d",code);
			d_log("\nTime :- %s", ctime(&wcdmatime));
			VERIFY_LENGTH(wcdma_rrc_states,length,0);
			wcdma_rrc_states *wrs =(wcdma_rrc_states*)&buf[0];
			switch (wrs->rrc_state){
				case 0: d_log("-->Disconnected");
					connectionState = 0;
					break;
				case 1: d_log("-->Connecting");
					connectionState = 1;
					break;
				case 2: d_log("-->Cell FACH");
					connectionState = 1;
					break;
				case 3: d_log("-->Cell DCH");
					connectionState = 1;
				 	break;
				case 4: d_log("-->Cell PCH");
					connectionState = 1;
					break;
				case 5 : d_log("-->URA PCH");
					connectionState = 1;
					break;
				default:
					d_log("error found");
			}
		}
		break;
		case WCDMA_CELL_ID :
		{
#if CHECK_STATS
			count_4127++;
#endif
#ifdef MEMORY_CORRUPTION_REPORT
			d_warning("4127\n\n");
#endif //MEMORY_CORRUPTION_REPORT
			//d_warning("4127\n");
			d_log("\nCommand code :-  %d",code);
			d_log("\nTime  :- %s", ctime(&wcdmatime));
			VERIFY_LENGTH(wcdma_cell_id ,length,0);
			 /*Update Time Stamp*/
			memcpy(myCellInfo.timestamp,buf-8 ,8);
			//if()
			//myCellInfo.Num_of_activeSet = 0;
			//memset(&(myCellInfo.activeSetInfo[0]),0,sizeof(myCellInfo.activeSetInfo[0]));

			//if(length == sizeof(wcdma_cell_id)){
				wcdma_cell_id *wci = (wcdma_cell_id *)&buf[0];
				if(myCellInfo.cell_id != wci->cell_id){
					myCellInfo.Num_of_activeSet = 0;
					memset(&(myCellInfo.activeSetInfo[0]),0,sizeof(myCellInfo.activeSetInfo[0]));
				}
				myCellInfo.URAFCN_Uplink=wci->ul_uarfcn;
				myCellInfo.URAFCN_Downlink=wci->dl_uarfcn;
				myCellInfo.cell_id =wci->cell_id;
				d_log("\nCELL ID :-%d",wci->cell_id);
			//}
			/*else if(length == sizeof(wcdma_cell_id_new)){
				wcdma_cell_id_new *wnci = (wcdma_cell_id_new *)&buf[0];
				myCellInfo.URAFCN_Uplink=wnci->ul_uarfcn;
				myCellInfo.URAFCN_Downlink=wnci->dl_uarfcn;
				myCellInfo.cell_id =wnci->cell_id;
				d_log("\nNEW_CELL_ID:-%d",wnci->cell_id);
			}*/
			//else d_warning("Wrong 4127 length :- %u", length);
		}
		break;
		case WCDMA_RRC_SIGNALING_MESSAGE :
		{
#if CHECK_STATS
			count_412f++;
#endif
			VERIFY_LENGTH(wcdma_rrc_signaling_messages ,length,0);
			 /*Update Time Stamp*/
			memcpy(myCellInfo.timestamp,buf-8 ,8);
			wcdma_rrc_signaling_messages *wrsm =  ( wcdma_rrc_signaling_messages*)&buf[0];
			d_log("\nWcdma Rrc Signal code  :- %d",code);
			d_log("\nTime  :- %s",ctime(&wcdmatime));
			d_log("Logical Channel Type:- %d",wrsm->logical_channel_type);
			d_log("\nRB_Identifier :- %d",wrsm->rb_identifier);
			switch (wrsm->logical_channel_type)
			{
				case 0x0  : d_log("\nLogical Channel Type:- CCCH on RACH -Uplink");
							break;
				case 0x01 :	d_log("\nLogical Channel Type:- DCCH Uplink");
							break;
				case 0x02 :	d_log("\nLogical Channel Type:- CCCH Downlink");
							break;
				case 0x03 : d_log("\nLogical Channel Type:- DCCH Downlink");
							break;
				case 0x04 :	d_log("\nLogical Channel Type:- BCCH-BCH Downlink");
							break;
				case 0x06 : d_log("\nLogical Channel Type:-BCCH-PCCH Downlink");
							break;
				case 0x50 : d_log("\nLogical Channel Type:- MCCH over FACH");
							break;
				case 0x5F : d_log("\nLogical Channel Type:- MSCH over FACH");
							break;
				case 0x80 : d_log("\nLogical Channel Type:- CTCH over FACH");
							break;
				case 0x0FE :d_log("\nLogical Channel Type:- MASTER COMPLITE LIST/SYSTEM INFO");
							break;
				default:
					d_log("\nUnknown Logical Channel found");
			}
			if(wrsm->length < 2048)
			{
				d_log("\nData length:- %d",wrsm->length);
				d_log("\nData:-");
				d_arraylog(wrsm->wcdma_rrc_data,wrsm->length);
				//WCDMAmessageName(buf ,length);
				//decodeRRC(wrsm->wcdma_rrc_data, wrsm->length, wrsm->logical_channel_type, 0);
				if(getCCEventEnableStatus()) { //send cc events only if they are enabled
					u_int64_t cmdmillSeconds = 0;
					cmdTimestamp *cts = (cmdTimestamp *)&timestamp[0];
					cmdmillSeconds = ((unsigned long long)cts->counter*125)/100;
				#ifndef HOST				//to test compilation.. added by me
				#ifndef MINIMUM_DM
				 	WCDMADecoderCCevents(wrsm->wcdma_rrc_data, wrsm->length, wrsm->logical_channel_type, cmdmillSeconds);
				#endif
				#endif
				
				}
			}else{
				d_log("\nData packet is more than 2048 byte:-");
			}
		}
		break;
#if 0
		case WCDMA_FINGER_INFO :
		{
			u_int32_t index = 0, counter = 0;
			d_warning("Command Code ", code);
			printf("\nWCDMA_FINGER_INFO");
			wcdma_finger_info *wfi = (wcdma_finger_info *)&buf[index];
			d_warning ("Tx_Pos = %u",wfi->tx_pos);
			d_warning ("Coh_Int_length = %u",wfi->coh_int_length);
			d_warning ("Non_Coh_Int_length = %u",wfi->non_coh_int_length);
			d_warning ("No of Path = %u",wfi->num_path);
			index += sizeof(*wfi);
			for(;counter<= wfi->num_path; counter++){
				wcdma_finger_info_each_path *wfiep = (wcdma_finger_info_each_path*)&buf[index];
				d_warning ("Path_Enr = %u",wfiep->path_enr);
				d_warning ("ECIO = %f",(10.0*log10((wfiep->path_enr - (0.1406*wfi->non_coh_int_length*wfi->coh_int_length))/(0.1406*wfi->non_coh_int_length*wfi->coh_int_length*wfi->coh_int_length))));
				d_warning ("Pn_pos_path = %u",wfiep->pn_pos_path);
				d_warning ("P_CPICH_PSC = %u",wfiep->p_cpich_psc);
				d_warning ("S_CPICH_SSC = %u",wfiep->s_cpich_ssc);
				d_warning ("Fing_Chan_code_idx = %u",wfiep->fing_chan_code_idx);
				d_warning ("Fing_IDX = %u",wfiep->fing_idx);
				index += sizeof(*wfiep);
			}
			wcdma_finger_info_num_frig *wfinf = (wcdma_finger_info_num_frig *)&buf[index];
			d_warning ("Num of fring  = %u",wfinf->num_fing);
			index += sizeof(*wfinf);
			for(counter = 0; counter <= wfinf->num_fing; counter++){
				wcdma_finger_for_each_finger *wffef = (wcdma_finger_for_each_finger *)&buf[index];
				d_warning (" Fing_Id = %u",wffef->fing_id);
				d_warning (" LockStatus= %u",wffef->lock_status);
				d_warning (" Tpc_idx= %u",wffef->tpc_idx);
				d_warning (" Fing_Pos = %u",wffef->fing_pos);
				d_warning (" Fing Assign Status = %u",(wffef->fing_assign_state_or_fing_state & 0x01));
				d_warning (" Fing Status = %u",(wffef->fing_assign_state_or_fing_state & 0x02));
				d_warning (" Fing_Cpich_Div = %u",wffef->cpich_enr);
				d_warning (" Cpich_enr = %u",wffef->cpich_enr);
				d_warning (" Cpich_enr_Calcaulated  = %f",(10.0*log10(wffef->cpich_enr/36864)));
				d_warning (" Cpich_enr_div = %u",wffef->cpich_enr_div);
				d_warning (" Cpich_enr_div_Calcaulated  = %f",(10.0*log10(wffef->cpich_enr_div/36864)));
				d_warning (" Cpich_enr_tot = %u",wffef->cpich_enr_tot);
				d_warning (" Prim_Sc_code_idx = %u",wffef->prim_sc_code_idx);
				d_warning (" Cpich_chan_code = %u",wffef->cpich_chan_code);
				index += sizeof(*wffef);
			}
		}
#endif
#if 0
		case WCDMA_TEMP_ANALYSIS_EDITION_2 :
		{
			u_int32_t index = 0, cellCount = 0, pathCount = 0, fingCount = 0;
			d_warning("\n Command code = %d",code);
			wcdma_temp_analysis_Edition_2  *wtae2 = (wcdma_temp_analysis_Edition_2 *)&buf[index];
			d_warning("\n Version %d  ",wtae2->version_num);
			/*if(wtae2->version_num != 2) return;***/
			d_warning("\n Num cell = %d",wtae2->num_cell);
			index += sizeof(*wtae2);
			for( ; cellCount<= wtae2->num_cell; cellCount++){
				wcdma_temp_analysis_Edition_2_cell_struct *wtae2cs = (wcdma_temp_analysis_Edition_2_cell_struct *)&buf[index];
				d_warning("\n Cell_idx = %d ",wtae2cs->cell_idx);
				d_warning("\n Scr_code_idx = %d ",wtae2cs->scr_code_idx);
				d_warning("\n svc_mask = %d ",wtae2cs->svc_mask );
				d_warning("\n Index_ref = %d ",wtae2cs->index_ref );
				d_warning("\n Pri_ref_fing = %d ",wtae2cs->pri_ref_fing );
				d_warning("\n sec_ref_fing = %d ",wtae2cs->sec_ref_fing );
				d_warning("\n ref_pos = %d ",wtae2cs->ref_pos);
				d_warning("\n Sec_cpich_ovsf = %d ",wtae2cs->sec_cpich_ovsf);
				d_warning("\n Active_phy_channels = %d ",wtae2cs->active_phy_channels);
				index += sizeof(*wtae2cs);
			}
			wcdma_temp_analysis_Edition_2_path *wtae2p = (wcdma_temp_analysis_Edition_2_path *)&buf[index];
			d_warning("\n Num_path= %d ",wtae2p->num_path);
			index += sizeof(*wtae2p);
			for( ;pathCount <= wtae2p->num_path; pathCount++ ){
				wcdma_temp_analysis_Edition_2_path_struct  *wtae2ps = (wcdma_temp_analysis_Edition_2_path_struct *)&buf[index];
				d_warning("\n Eng = %d ",wtae2ps->eng );
				d_warning("\n Pos = %d ",wtae2ps->pos);
				d_warning("\n Cell_idx = %d ",wtae2ps->cell_idx );
				d_warning("\n Fing = %d ",wtae2ps->fing );
				d_warning("\n Nn = %d ",wtae2ps->nn );
				d_warning("\n Ant_and_vir_path_info = %d ",wtae2ps->ant_and_vir_path_info );
				index += sizeof( *wtae2ps );
			}
			wcdma_temp_analysis_Edition_2_fing *wtae2f = (wcdma_temp_analysis_Edition_2_fing*)&buf[index];
			d_warning("\n Fing = %d ",wtae2f->num_fing );
			index += sizeof(*wtae2f);
			for( ;fingCount <= wtae2f->num_fing ; fingCount++){
				wcdma_temp_analysis_Edition_2_fing_struct *wtae2fs = (wcdma_temp_analysis_Edition_2_fing_struct*)&buf[index];
				d_warning("\n cell_idx = %d ",wtae2fs->cell_idx );
				d_warning("\n fing = %d ",wtae2fs->fing );
				d_warning("\n lock = %d ",wtae2fs->lock );
				d_warning("\n fing_pos = %d ",wtae2fs->fing_pos );
				d_warning("\n fing_state = %d ",wtae2fs->fing_state );
				d_warning("\n cp_eng = %d ",wtae2fs->cp_eng );
				d_warning("\n txdiv_cp_eng = %d ",wtae2fs->txdiv_cp_eng );
				d_warning("\n tot_cp_eng = %d ",wtae2fs->tot_cp_eng );
				d_warning("\n reserved = %d ",wtae2fs->reserved );
				index += sizeof(*wtae2fs);
			}
		}
        break;
#endif
		case WCDMA_LIST_SEARCH_VER_5 :{
#if CHECK_STATS
			count_401c++;
#endif
			is401cPresent = 1;
			if(connectionState == 0) break; //disconnected state
			numOfHits++;
			u_int32_t psc = 0;
			u_int32_t totalFilterEng = 0;
			float ECIO = 0;
			u_int32_t index = 0, count = 0;
			u_int8_t ant_id = 0;
			u_int8_t pscfound = 0;
			u_int32_t tempPsc[4] = {0};
			u_int8_t pscCount = 0;
			u_int8_t iterationCount = 0;
			int8_t rxPower = 0;
			int8_t rscp = 0;

			VERIFY_LENGTH(wcdma_list_serarch_ver_5 ,length,index);
			 /*Update Time Stamp*/
			memcpy(myCellInfo.timestamp,buf-8 ,8);
			wcdma_list_serarch_ver_5 *wlsv5 = (wcdma_list_serarch_ver_5 *)&buf[0];
			index += sizeof(wcdma_list_serarch_ver_5);
			u_int32_t tmp = 0;

			rxPower = -106 + (wlsv5->rx_agc + 512)/12;

			for(count=0; count < wlsv5->num_task_spec; count++) {
				VERIFY_LENGTH(per_specified_input_task ,length,index);
				per_specified_input_task *psit = (per_specified_input_task*)&buf[index];
				tmp = (wlsv5->num_task_spec -count) *sizeof(per_specified_input_task)+count*sizeof(per_finished_input_task);
				VERIFY_LENGTH(per_finished_input_task ,length,index+tmp);
				per_finished_input_task *pfit = (per_finished_input_task*)&buf[index+tmp];

				psc = (psit->scr_code >> 4);

				for(iterationCount = 0; iterationCount < pscCount; iterationCount++) {
					if(tempPsc[iterationCount] == psc) {
						pscfound = 1;
						break;
					}
				}
				if(!pscfound) {
					tempPsc[pscCount] = psc;
					pscCount++;
					totalFilterEng = pfit->eng[0] + pfit->eng[1] + pfit->eng[2] + pfit->eng[3];
					if(totalFilterEng > 0){
						ECIO = getECIO(psit->nc, psit->nn, totalFilterEng, 0);
						rscp = rxPower + ECIO;
						insertLocalCellInfo(psc, ECIO, rscp, psit->network_type);
					}

				}
				if(numOfHits == maxCount) {
					updateGlobalStruct();
					memset(&connectedwcdmapara, 0, sizeof(connectedWCDMApara));
					numOfHits = 0;
				}
				index += sizeof(per_specified_input_task);
			}

			ant_id = *(u_int8_t *)&buf[index];
			break;
		}
		case WCDMA_TEMPORAL_ANALYSIS_ED2:
		{
#ifdef MEMORY_CORRUPTION_REPORT
			d_warning("4186 command hit\n");
#endif
			//if(connectionState == 0) break;
			if(is401cPresent) break;

			u_int8_t num_cell = 0, num_path = 0, num_finger = 0;
			int i = 0, index = 0, j = 0;
			float ECIO = 0;

			VERIFY_LENGTH(wcdma_temporal_analysis_ed2 ,length,index);

			myCellInfo.Num_of_activeSet = 0;
			myCellInfo.neigCellInfo.num_of_wcdma_cell = 0;
			//memset(myCellInfo.activeSetInfo, 0 , sizeof(ActiveSetInfo) * MAX_WCDMA_ACTIVE_SET);
			memcpy(myCellInfo.timestamp,buf-8 ,8);

			perCellENGaccount pcea[10] = {0}; //valid value of num_cell is [0..10]
			wcdma_temporal_analysis_ed2 *wtae2 = (wcdma_temporal_analysis_ed2 *)&buf[0];

			num_cell = wtae2->num_cell; //number of cells present in message
			if(num_cell >= 10) {
				d_warning("Wrong value received in Packet , %u \n", num_cell);
				break;
			}


			index += sizeof(wcdma_temporal_analysis_ed2);


			for(i = 0; i < num_cell; i++) {
				VERIFY_LENGTH(per_cell_structure ,length,index);
				per_cell_structure *pcs = (per_cell_structure *)&buf[index];
				pcea[i].cell_index = pcs->cell_index;
				pcea[i].psc = ((pcs->scr_code_index & 0x1ff0) >> 4); //bits [4..12]
				if(pcs->index_ref & 0x0100) {
					pcea[i].isActiveSet = 1;
				}
				index += sizeof(per_cell_structure);
			}

			num_path = *(u_int8_t *)&buf[index]; // total number of paths
			index += sizeof(u_int8_t);
			if(num_path >= 35) {
				d_warning("num path error in 4186, %u\n", num_path);
				break;
			}
			for(i = 0; i < num_path; i++) {
				VERIFY_LENGTH(per_path_structure ,length,index);
				per_path_structure *pps = (per_path_structure *)&buf[index];
				for(j = 0; j < num_cell; j++) {
					if(pps->cell_index == j) {  //pcea[j].cell_index) {
						pcea[j].nc = pps->nc;
						pcea[j].nn = pps->nn;
						pcea[j].ENG += pps->energy;
						break;
					}
				}
				index += sizeof(per_path_structure);
			}

			num_finger = *(u_int8_t *)&buf[index]; // total number of fingers
			index += sizeof(u_int8_t);
			if(num_finger >= 15) {
				d_warning("num finger error in 4186, %u\n", num_finger);
				break;
			}
			for(i = 0; i < num_finger; i++) {
				VERIFY_LENGTH(per_finger_structure ,length,index);
				per_finger_structure *pfs = (per_finger_structure *)&buf[index];
				index += sizeof(per_finger_structure);
			}
			index += 33; //for internal fields
			//update structure
			for(i = 0; i < num_cell; i++) {
				ECIO = getECIO(pcea[i].nc, pcea[i].nn, pcea[i].ENG, 1);
				if(ECIO == 0) {
					int abc;
					d_warning("wrong ecio\n");
					continue;
				}
				if(pcea[i].isActiveSet) {
#ifdef MEMORY_CORRUPTION_REPORT
					d_warning("4186 asupdate\n");
					d_warning("ECIO : %f, PSC : %u\n\n", ECIO, pcea[i].psc);
#endif //MEMORY_CORRUPTION_REPORT
					myCellInfo.activeSetInfo[myCellInfo.Num_of_activeSet].ECIO = ECIO;
					myCellInfo.activeSetInfo[myCellInfo.Num_of_activeSet].PRIM_SC_CODE = pcea[i].psc;
					myCellInfo.activeSetInfo[myCellInfo.Num_of_activeSet].RSCP = rxPower_4179 + ECIO;
					if(myCellInfo.Num_of_activeSet == 0) { //first entry of active set is reference cell
						myCellInfo.activeSetInfo[myCellInfo.Num_of_activeSet].is_reference_cell = 1;
						myCellInfo.PRIM_SC_CODE = myCellInfo.activeSetInfo[myCellInfo.Num_of_activeSet].PRIM_SC_CODE;

					} else {
						myCellInfo.activeSetInfo[myCellInfo.Num_of_activeSet].is_reference_cell = 0;
					}
					myCellInfo.Num_of_activeSet++;

					//printf("psc %d\n\n", pcea[i].psc);
				} else {
					myCellInfo.neigCellInfo.wcdmaNeigbourData[myCellInfo.neigCellInfo.num_of_wcdma_cell].ecio = ECIO;
					myCellInfo.neigCellInfo.wcdmaNeigbourData[myCellInfo.neigCellInfo.num_of_wcdma_cell].psc = pcea[i].psc;
					myCellInfo.neigCellInfo.wcdmaNeigbourData[myCellInfo.neigCellInfo.num_of_wcdma_cell].rscp = rxPower_4179 + ECIO;
					myCellInfo.neigCellInfo.num_of_wcdma_cell++;
				}
			}
		}
		break;
		case WCDMA_PN_SEARCH_LOG_PACKET_ED2 : {
			if(is401cPresent) break;
			//printf("4179 hit\n");
			//int index = 0, i = 0;
			//u_int32_t tempPsc[4] = {0};
			//u_int8_t pscCount = 0;
			//u_int32_t psc = 0;
			//u_int8_t iterationCount = 0;
			//u_int8_t pscfound = 0;
			//u_int64_t totalFilterEng = 0;
			//float ECIO = 0;
			//int8_t rscp = 0;
			//int8_t rxPower = 0;
			//u_int32_t tmp = 0;
			//int8_t received_power[2] = {0};

			wcdma_pn_search_log_packet_ed2 *wpslpe2 = (wcdma_pn_search_log_packet_ed2 *)&buf[0];
	//		index += sizeof(wcdma_pn_search_log_packet_ed2);

			rxPower_4179 = -106 + ((wpslpe2->rx_carr0_agc_0) + 512)/12;
		//	printf("rxPower : %d\n", rxPower);
#if 0
			for(i = 0; i < wpslpe2->num_task_spec; i++) {
				per_specified_ip_task *psit = (per_specified_ip_task *)&buf[index];
				printf("nc :%u ,nn :%u \n", psit->nc, psit->nn);
				tmp = (wpslpe2->num_task_spec -i) *sizeof(per_specified_ip_task)+i*sizeof(per_specified_ip_task);
				//index += sizeof(per_specified_ip_task);
				per_specified_result_task *psrt = (per_specified_result_task *)&buf[index+tmp];
				psc = ((psit->scr_code & 0x1ff0) >> 4);
				for(iterationCount = 0; iterationCount < pscCount; iterationCount++) {
					if(tempPsc[iterationCount] == psc) {
						pscfound = 1;
						break;
					}
				}
				if(!pscfound) {
					tempPsc[pscCount] = psc;
					pscCount++;
					totalFilterEng = psrt->energy[0] + psrt->energy[1] + psrt->energy[2] + psrt->energy[3] + psrt->energy[4] + psrt->energy[5];
					//printf("totalFilterEng = %llu\n", totalFilterEng);
					if(totalFilterEng > 0){
						ECIO = getECIO(psit->nc, psit->nn, totalFilterEng, 2);
						//printf("ECIO = %f\n", ECIO);
						rscp = rxPower + ECIO;
						insertLocalCellInfo(psc, ECIO, rscp, psit->set_type);
					}
				}
				if (num_WCDMA_TEMPORAL_ANALYSIS_ED2 == maxCount) {
					updateGlobalStruct();
					memset(&connectedwcdmapara, 0, sizeof(connectedWCDMApara));
					numOfHits = 0;
				}
				index += sizeof(per_specified_ip_task);
			}
#endif
		}
		break;
#if CHECK_STATS
		case HS_DECODE_STATUS_LOG_PKT_EDITION_3 :
				{
					count_4222++;
					VALIDATE_VERSION("HS_DECODE_STATUS_LOG_PKT_EDITION_3",5,buf[0]);
				}
					break;
				case EUL_L1_MAC_STATISTICS_LOG_PKT :
					count_4311++;
					break;
				case UL_HS_DPCCH_DEBUG_LOG_PkT :
					count_420b++;
					break;
				case HS_DECODE_STATUS_LOG_PKT_VER_2 :
					count_4214++;
					break;
				case MAC_HS_RESET_LOG_PKT :
					count_4213++;
					break;
				case MAC_HS_STATUS_LOG_PKT :
					count_4212++;
					break;
				case MAC_HS_HEADER_LOG_PKT :
					count_4211++;
					break;
				case HS_DSCH_HARQ_STA_LOG_PKT :
					count_4208++;
					break;
				case WCDMA_RLC_DL_AM_STATISTICS :
					count_414a++;
					break;
				case WCDMA_RACH_PARAMETERS :
					count_4160++;
					break;
				case WCDMA_RLC_UL_AM_STATISTICS_VER_2 :
					count_4138++;
					break;
				case WCDMA_BLER :
					count_4116++;
					break;
				case WCDMA_PRACH :
					count_410d++;
					break;
#endif

		default:
			d_log("\nUnknown packet type",code);
			break;
	}
}
#if CHECK_STATS
void print_count_wcdma()
{
	printf ("\n4005 : %d", count_4005);
	printf ("\n4110 : %d", count_4110);
	printf ("\n4111 : %d", count_4111);
	printf ("\n410c : %d", count_410c);
	printf ("\n4125 : %d", count_4125);
	printf ("\n4127 : %d", count_4127);
	printf ("\n412f : %d", count_412f);
	printf ("\n401c : %d", count_401c);
	printf ("\n4222 : %d", count_4222);
	printf ("\n4311 : %d", count_4311);
	printf ("\n420b : %d", count_420b);
	printf ("\n4214 : %d", count_4214);
	printf ("\n4213 : %d", count_4213);
	printf ("\n4212 : %d", count_4212);
	printf ("\n4211 : %d", count_4211);
	printf ("\n4208 : %d", count_4208);
	printf ("\n414a : %d", count_414a);
	printf ("\n4160 : %d", count_4160);
	printf ("\n4138 : %d", count_4138);
	printf ("\n4116 : %d", count_4116);
	printf ("\n410d : %d", count_410d);
	printf ("\n4005 : %d", count_4005);
}
#endif
