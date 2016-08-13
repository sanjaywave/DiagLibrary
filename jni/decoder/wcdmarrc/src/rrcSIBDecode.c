#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>


#include "rrc.h"
#include "rtsrc/rtPrint.h"
#include "rtxsrc/rtxCommon.h"
#include "rtsrc/rtContext.h"
//#include "testContext.h"
//#include "utilities.h"
//#include "errors.h"


#define MAX_SIB_BLOCK_DATA 2048 //This is define in bits
#define MAX_SIB_DATA 10 //This is just for define purpose

#define SIB_DATA_SIZE_BLOCK 222
#define SIB_DATA_SIZE_COMPLETE 226
#define bool boolean


struct SIBDataStore
{
	u_int32_t length; // The length is define in bits
	SIB_Type sibType;
	u_int32_t seg_Count;
	u_int8_t sibData[MAX_SIB_BLOCK_DATA];
};
typedef struct SIBDataStore SIBDataStore;

void copybits( u_int8_t *srcData , u_int8_t *destData , u_int32_t len , u_int8_t dstBitPos);

void decodeSIBMessage(OSCTXT *p_lctxt,SIB_Type sibType , u_int8_t *data, u_int32_t length );

/*
//When you make linklist remove the fill validPartialSIB and isValid , we dont require the same.

int insertElement( TestContext *testContext, SIBDataStore *newSIBElement)
{	
	u_int32_t count =0;
	int32_t returnType = 0;
	while ((testContext->WcdmaRadioInfo.mWcdmaSIBEncodeData.sibDataPtr[count] !=NULL) && 
	  (count < MAX_TEMP_SIB_SEG))
		count++;
	if(count == (MAX_TEMP_SIB_SEG -1)){
		d_warning("Element cannot be inserted");
		returnType = -1;
		return returnType;
	}
	testContext->WcdmaRadioInfo.mWcdmaSIBEncodeData.sibDataPtr[count] =(void*)newSIBElement;
	return returnType;
} 
*/
/*
SIBDataStore *getElement(TestContext *testContext , SIB_Type sibType , u_int8_t markFree)
{
	u_int32_t count =0;
	SIBDataStore *element =NULL,*tempElement = NULL;
	while (count<MAX_TEMP_SIB_SEG)
	{
		tempElement = (SIBDataStore*)testContext->WcdmaRadioInfo.mWcdmaSIBEncodeData.sibDataPtr[count];
	        if(tempElement !=NULL){	
			if (tempElement->sibType == sibType){
				element = tempElement;
				break;
			}
		}
		count++;	
	}
	if(markFree)
	{
		testContext->WcdmaRadioInfo.mWcdmaSIBEncodeData.sibDataPtr[count] = NULL;
	}
	return element;
}
*/
/* This function will update the testContext with correponding SIB first Segment */
/*
void updateFirstSegment(OSCTXT * p_lctxt,TestContext *testContext ,FirstSegment *firstSegment )
{
	SIBDataStore *newSIBElement;
	newSIBElement = (SIBDataStore*)malloc(sizeof(SIBDataStore));
	memset(newSIBElement,0,sizeof(SIBDataStore));
	newSIBElement->sibType = firstSegment->sib_Type;
	newSIBElement->seg_Count = firstSegment->seg_Count-1;
	copybits( firstSegment->sib_Data_fixed.data , newSIBElement->sibData , SIB_DATA_SIZE_BLOCK ,0);
    newSIBElement->length = SIB_DATA_SIZE_BLOCK;
	insertElement(testContext,newSIBElement);
}
*/
/*
void updateFirstSegmentShort(OSCTXT * p_lctxt,TestContext *testContext ,FirstSegmentShort *firstSegmentShort )
{

	SIBDataStore *newSIBElement;
	newSIBElement = (SIBDataStore*)malloc(sizeof(SIBDataStore));
	memset(newSIBElement,0,sizeof(SIBDataStore));
	newSIBElement->sibType = firstSegmentShort->sib_Type;
	newSIBElement->seg_Count = firstSegmentShort->seg_Count-1;
	copybits( firstSegmentShort->sib_Data_variable.data , newSIBElement->sibData , firstSegmentShort->sib_Data_variable.numbits ,0);
    newSIBElement->length = firstSegmentShort->sib_Data_variable.numbits;
	insertElement(testContext,newSIBElement);
}*/

/* This function will update the TestContext *testContext with correponding SIB SubSegment */
/*
void updateSubSeqSegment(OSCTXT * p_lctxt,TestContext *testContext ,SubsequentSegment *segment )
{

	//If SIBType element is not found , means we need to discard that packet ... no`thing to be done. This can happen
	SIBDataStore *existSIBElement = getElement(testContext ,segment->sib_Type,FALSE); 
	if(existSIBElement){
		existSIBElement->seg_Count--; //reduce the 1 segment
		u_int8_t * dest = existSIBElement->sibData + existSIBElement->length/8 ;//Increment the pointer to byte position
		copybits( segment->sib_Data_fixed.data ,dest , SIB_DATA_SIZE_BLOCK ,existSIBElement->length%8 );
		existSIBElement->length += SIB_DATA_SIZE_BLOCK;
	}else {
		d_log("First Block is not receive , so discard the data\n");
	}
}
*/
/*
void updateLastSegmentShort(OSCTXT * p_lctxt,TestContext *testContext , LastSegmentShort *lastSegmentShort)
{

	//I have assume that below structure has 
	//If SIBType element is not found , means we need to discard that packet ... nothing to be done. This can happen
	SIBDataStore *existSIBElement = getElement(testContext ,lastSegmentShort->sib_Type,TRUE); 
	if(existSIBElement)
	{
		existSIBElement->seg_Count--; //reduce the 1 segment
    	u_int8_t *dest = existSIBElement->sibData + existSIBElement->length/8 ;//Increment the pointer to byte position
		copybits( lastSegmentShort->sib_Data_variable.data , dest ,lastSegmentShort->sib_Data_variable.numbits ,existSIBElement->length%8 );
		existSIBElement->length += lastSegmentShort->sib_Data_variable.numbits;
		if(existSIBElement->seg_Count)
		{
			d_warning  ("Something wrong in implementation");	
		}
		// Call here to SIB decode function 
		decodeSIBMessage(p_lctxt ,testContext ,existSIBElement->sibType ,existSIBElement->sibData,existSIBElement->length);
		free(existSIBElement);
	}

}
*/
/*
void updateLastAndFirst(OSCTXT * p_lctxt,TestContext *testContext ,SystemInformation_BCH_payload_lastAndFirst *lastAndFirst)
{
	updateLastSegmentShort(p_lctxt ,testContext ,&lastAndFirst->lastSegmentShort );//Update the last segment short
	updateFirstSegmentShort(p_lctxt ,testContext ,&lastAndFirst->firstSegment);//Create the new one

}
*/

/*
void updateCompleteSIB_List(OSCTXT * p_lctxt,TestContext *testContext ,CompleteSIB_List *completeSIB_List )
{
	unsigned int count =0;
	CompleteSIBshort *pCompleteSIBshort ;
	//Traverse the list
	OSRTDListNode *head = completeSIB_List->head;

	for(count=0;count< completeSIB_List->count;count++)
	{
		pCompleteSIBshort = (CompleteSIBshort*)head->data;
		decodeSIBMessage(p_lctxt,testContext ,pCompleteSIBshort->sib_Type ,pCompleteSIBshort->sib_Data_variable.data,
                      pCompleteSIBshort->sib_Data_variable.numbits);

		head = head->next;
	}

}
*/

/*
void updateLastAndCompleteAndFirst(OSCTXT * p_lctxt,TestContext *testContext ,SystemInformation_BCH_payload_lastAndCompleteAndFirst *lastAndCompleteAndFirst )
{
   updateLastSegmentShort(p_lctxt ,testContext ,&lastAndCompleteAndFirst->lastSegmentShort );//Update the last segment short
   updateCompleteSIB_List(p_lctxt ,testContext ,&lastAndCompleteAndFirst->completeSIB_List );
   updateFirstSegmentShort(p_lctxt ,testContext ,&lastAndCompleteAndFirst->firstSegment);//Create the new one with short...

}
*/
/*
void updateLastAndComplete(OSCTXT * p_lctxt,TestContext *testContext ,SystemInformation_BCH_payload_lastAndComplete *lastAndComplete)
{
	updateLastSegmentShort(p_lctxt ,testContext ,&lastAndComplete->lastSegmentShort );//Update the last segment short
	updateCompleteSIB_List(p_lctxt ,testContext ,&lastAndComplete->completeSIB_List );
}
*/
/*
void updateCompleteAndFirst(OSCTXT * p_lctxt,TestContext *testContext ,SystemInformation_BCH_payload_completeAndFirst *completeAndFirst )
{
   updateCompleteSIB_List(p_lctxt ,testContext ,&completeAndFirst->completeSIB_List );
   updateFirstSegmentShort(p_lctxt ,testContext ,&completeAndFirst->firstSegment);//Create the new one with short...
}
*/

#if 0
void updateCompleteSIB(OSCTXT * p_lctxt,TestContext *testContext ,CompleteSIB *completeSIB )
{
	//No need to store the data ... just decode directly
	decodeSIBMessage(p_lctxt ,testContext ,completeSIB->sib_Type , completeSIB->sib_Data_fixed,SIB_DATA_SIZE_COMPLETE );
}

void updateNoSegment(OSCTXT * p_lctxt,TestContext *testContext ,FirstSegment *firstSegment )
{
	//No need to store the data ... just decode directly
	//decodeSIBMessage(p_lctxt ,testContext ,firstSegment->sib_Type , (u_int8_t*)firstSegment->sib_Data_fixed,SIB_DATA_SIZE_COMPLETE );

}
void updateLastSegment(OSCTXT * p_lctxt,TestContext *testContext , LastSegment *lastSegment )
{

	SIBDataStore *existSIBElement = getElement(testContext ,lastSegment->sib_Type,TRUE);
	if(existSIBElement)
	{
		/*SIBDataStore *existSIBElement; //fetch Element*/
		existSIBElement->seg_Count--; //reduce the 1 segment
    	u_int8_t *dest = existSIBElement->sibData + existSIBElement->length/8 ;//Increment the pointer to byte position
		copybits( lastSegment->sib_Data_fixed.data , dest ,SIB_DATA_SIZE_BLOCK ,existSIBElement->length%8 );
		existSIBElement->length += SIB_DATA_SIZE_BLOCK;
		if(existSIBElement->seg_Count)
		{
			d_warning  ("Something wrong in implementation  last Segment");	
		}
		// Call here to SIB decode function 
	 	decodeSIBMessage(p_lctxt ,testContext ,existSIBElement->sibType ,existSIBElement->sibData,existSIBElement->length);
		free(existSIBElement);
	}

}

void decodeSystemInfoRRC ( OSCTXT * p_lctxt ,TestContext *testContext , BCCH_BCH_Message *p_rrc_bcch_bch_pdu)
{
	switch(p_rrc_bcch_bch_pdu->message.payload.t)
	{
	case T_SystemInformation_BCH_payload_noSegment :
			break;
	case T_SystemInformation_BCH_payload_firstSegment:
			updateFirstSegment(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.firstSegment );
			break;
	case	T_SystemInformation_BCH_payload_subsequentSegment:
			updateSubSeqSegment(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.subsequentSegment );
			break;
	case	T_SystemInformation_BCH_payload_lastSegmentShort:
			updateLastSegmentShort(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.lastSegmentShort );
			break;
	case	T_SystemInformation_BCH_payload_lastAndFirst:
			updateLastAndFirst(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.lastAndFirst );
			break;
	case	T_SystemInformation_BCH_payload_lastAndComplete:
			updateLastAndComplete(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.lastAndComplete ); 
			break;
	case	T_SystemInformation_BCH_payload_lastAndCompleteAndFirst:
			updateLastAndCompleteAndFirst(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.lastAndCompleteAndFirst );
			break;
	case	T_SystemInformation_BCH_payload_completeSIB_List :
			updateCompleteSIB_List(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.completeSIB_List );
			break;
	case	T_SystemInformation_BCH_payload_completeAndFirst :
			updateCompleteAndFirst(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.completeAndFirst );
			break;
	case	T_SystemInformation_BCH_payload_completeSIB :
			updateCompleteSIB(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.completeSIB );
			break;
	case	T_SystemInformation_BCH_payload_lastSegment :
			updateLastSegment(p_lctxt ,testContext ,p_rrc_bcch_bch_pdu->message.payload.u.lastSegment );
			break;
	case	T_SystemInformation_BCH_payload_spare5 :
	case	T_SystemInformation_BCH_payload_spare4 : 
	case	T_SystemInformation_BCH_payload_spare3 :
	case	T_SystemInformation_BCH_payload_spare2 :
	case	T_SystemInformation_BCH_payload_spare1 :
	default:
			break;
	}

}

void updateDetectedInfo(TestContext *testContext ,u_int32_t  primaryScramblingCode,u_int32_t count)
{
	u_int32_t counter = 0;

		if (count == 0)
		{
			for( counter= 0;counter<MAX_WCDMA_NEIGH_CELL; counter++ )
			{
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.Num_of_NeigbourSet=0;
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[counter].Rscp_Avg=0;
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[counter].PSC=0;
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[counter].Downlink_Freq=0;
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[counter].Position=0;
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[counter].Diversity= 0;
				testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[counter].isDetected=0;

			}
		}
		if(count < MAX_WCDMA_NEIGH_CELL)
		{
			testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.Num_of_NeigbourSet=count;
			testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[count].PSC = primaryScramblingCode;
			testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[count].Downlink_Freq =testContext->WcdmaRadioInfo.wcdmaCellInfo.URAFCN_Downlink ;
			testContext->WcdmaRadioInfo.wcdmaActiveAndNeigbourInfo.neigbSetInfo.neigbCellSetInfo[count].isDetected= 0;
		}
	
}

void updateNeigbourCell_EcNO_LCR(TestContext *testContext ,IntraFreqCellInfoSI_List_ECN0_LCR_r4 *intraFreqCellInfo_List )
{
	u_int32_t count = 0;

	NewIntraFreqCellSI_ECN0 *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_ECN0*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode ,count);
			}
		}
		head = head->next;
	}
}

void updateNeigbourCell_RSCP_LCR(TestContext *testContext ,IntraFreqCellInfoSI_List_RSCP_LCR_r4 *intraFreqCellInfo_List )
{
	u_int32_t count =0;

	NewIntraFreqCellSI_RSCP *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_RSCP*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode,count);
			}
		}
		head = head->next;
	}
}


void updateNeigbourCell_EcNO_HCS(TestContext *testContext ,IntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4 *intraFreqCellInfo_List )
{
	u_int32_t count = 0;

	NewIntraFreqCellSI_ECN0 *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_ECN0*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode ,count);
			}
		}
		head = head->next;
	}
}

void updateNeigbourCell_RSCP_HCS(TestContext *testContext ,IntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4 *intraFreqCellInfo_List )
{
	u_int32_t count =0;

	NewIntraFreqCellSI_RSCP *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_RSCP*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode,count);
			}
		}
		head = head->next;
	}
}

void updateNeigbourCell_HCS_RSCP(TestContext *testContext ,IntraFreqCellInfoSI_List_HCS_RSCP *intraFreqCellInfo_List )
{
	u_int32_t count =0;

	NewIntraFreqCellSI_RSCP *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_RSCP*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode,count);
			}
		}
		head = head->next;
	}
}

void updateNeigbourCell_HCS_ECN0(TestContext *testContext ,IntraFreqCellInfoSI_List_HCS_ECN0 *intraFreqCellInfo_List )
{
	u_int32_t count =0;

	NewIntraFreqCellSI_RSCP *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_RSCP*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode,count);
			}
		}
		head = head->next;
	}
}

void updateNeigbourCell_RSCP(TestContext *testContext ,IntraFreqCellInfoSI_List_RSCP *intraFreqCellInfo_List )
{
	u_int32_t count =0;

	NewIntraFreqCellSI_RSCP *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_RSCP*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode,count);
			}
		}
		head = head->next;
	}
}

void updateNeigbourCell_ECN0(TestContext *testContext ,IntraFreqCellInfoSI_List_ECN0 *intraFreqCellInfo_List )
{
	u_int32_t count =0;

	NewIntraFreqCellSI_RSCP *pNewIntraFreqCell ;
	//Traverse the list
	OSRTDListNode *head = intraFreqCellInfo_List->newIntraFreqCellList.head;

	for(count=0;count< intraFreqCellInfo_List->newIntraFreqCellList.count;count++)
	{
		pNewIntraFreqCell = (NewIntraFreqCellSI_RSCP*)head->data;
		if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.t == 1)
		{
			if(pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->m.primaryCPICH_InfoPresent){
				updateDetectedInfo (testContext ,pNewIntraFreqCell->cellInfo.modeSpecificInfo.u.fdd->primaryCPICH_Info.primaryScramblingCode,count);
			}
		}
		head = head->next;
	}
}



void updateDetectedSet(TestContext *testContext,SysInfoType11 *sysInfoType11)
{

	IntraFreqCellInfoSI_List_RSCP_LCR_r4  rrcIntraFreqCellInfoSI_List_RSCP_LCR_r4;
	IntraFreqCellInfoSI_List_ECN0_LCR_r4  rrcIntraFreqCellInfoSI_List_ECNO_LCR_r4;
	IntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4 rrcIntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4;
	IntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4 rrcIntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4;
	IntraFreqCellInfoSI_List_HCS_RSCP rccIntraFreqCellInfoSI_List_HCS_RSCP;
	IntraFreqCellInfoSI_List_HCS_ECN0 rrcIntraFreqCellInfoSI_List_HCS_ECN0 ;
	IntraFreqCellInfoSI_List_RSCP rrcIntraFreqCellInfoSI_List_RSCP;
	IntraFreqCellInfoSI_List_ECN0 rrcIntraFreqCellInfoSI_List_ECN0;


	if(sysInfoType11->v4b0NonCriticalExtensions.m.sysInfoType11_v4b0extPresent)
	{
		if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.t == 1)
		{
			if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t == 1) {
					if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->m.intraFreqMeasurementSysInfoPresent)
					if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
					{
						rrcIntraFreqCellInfoSI_List_RSCP_LCR_r4 = sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
						updateNeigbourCell_RSCP_LCR(testContext ,&rrcIntraFreqCellInfoSI_List_RSCP_LCR_r4);
					}
			}else if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t == 2)
			{
				if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.intraFreqMeasurementSysInfoPresent)
				if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
				{
					rrcIntraFreqCellInfoSI_List_ECNO_LCR_r4 = sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
					updateNeigbourCell_EcNO_LCR(testContext ,&rrcIntraFreqCellInfoSI_List_ECNO_LCR_r4);
				}
			}
		} else if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.t == 2)
		{
			if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t==1)
			{
					if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->m.intraFreqMeasurementSysInfoPresent)
						if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
						{
							rrcIntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4 =sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
							updateNeigbourCell_RSCP_HCS(testContext ,&rrcIntraFreqCellInfoSI_List_HCS_RSCP_LCR_r4);
						}
			}else if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t==2)
			{
				if (sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.intraFreqMeasurementSysInfoPresent)
					if(sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
					{
						rrcIntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4 =sysInfoType11->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.measurementControlSysInfo_LCR.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
						updateNeigbourCell_EcNO_HCS(testContext ,&rrcIntraFreqCellInfoSI_List_HCS_ECN0_LCR_r4);
					}
			}
		}
	 }else if(sysInfoType11->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent)
	 {
		if(sysInfoType11->v4b0NonCriticalExtensions.v590NonCriticalExtension.sysInfoType11_v590ext.m.newIntraFrequencyCellInfoList_v590extPresent)
		{
			//rrcSysInfoType11_v590ext_IEs_newIntraFrequencyCellInfoList_v590ext = sysInfoType11->v4b0NonCriticalExtensions.v590NonCriticalExtension.sysInfoType11_v590ext.newIntraFrequencyCellInfoList_v590ext;
			//updateNeigbourCell_EcNO_HCS(testContext ,& rrcSysInfoType11_v590ext_IEs_newIntraFrequencyCellInfoList_v590ext);
		}

	 }else if(sysInfoType11->measurementControlSysInfo.use_of_HCS.t== 1)
	 {
		if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t ==1)
		{
			if(sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->m.intraFreqMeasurementSysInfoPresent)
				if(sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
				{
					rrcIntraFreqCellInfoSI_List_RSCP =sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
					updateNeigbourCell_RSCP(testContext ,&rrcIntraFreqCellInfoSI_List_RSCP);
				}

		}else if(sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t ==2)
		{
			if(sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.intraFreqMeasurementSysInfoPresent)
			{
				if(sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.intraFreqMeasurementSysInfoPresent)
					if(sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
					{
						rrcIntraFreqCellInfoSI_List_ECN0 =sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
						updateNeigbourCell_ECN0(testContext ,&rrcIntraFreqCellInfoSI_List_ECN0);
					}
			}
		}
	}else if (sysInfoType11->measurementControlSysInfo.use_of_HCS.t == 2)
	{
		if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t == 1)
		{
			/*cpich_RSCP*/
			if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->m.intraFreqMeasurementSysInfoPresent)
				if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
				{
					rccIntraFreqCellInfoSI_List_HCS_RSCP = sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_RSCP->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
					updateNeigbourCell_HCS_RSCP(testContext ,&rccIntraFreqCellInfoSI_List_HCS_RSCP);
				}
		}else if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t==2)
		{
			/*cpich_ECIO*/
			if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->m.intraFreqMeasurementSysInfoPresent)
					if (sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.m.intraFreqCellInfoSI_ListPresent)
					{
						rrcIntraFreqCellInfoSI_List_HCS_ECN0 = sysInfoType11->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.u.cpich_Ec_N0->intraFreqMeasurementSysInfo.intraFreqCellInfoSI_List;
						updateNeigbourCell_HCS_ECN0(testContext ,&rrcIntraFreqCellInfoSI_List_HCS_ECN0);
					}
			}
	}

}
void updateCellMccAndMnc(TestContext *testContext ,MasterInformationBlock *mMasterInformationBlock )
{

	MCC wcdmaMcc;
	MNC wcdmaMnc;
	u_int32_t counter=0;
	char tempBuff[4]= {'\0'};
 	if(mMasterInformationBlock->plmn_Type.t==1)
 	{
 		wcdmaMcc = mMasterInformationBlock->plmn_Type.u.gsm_MAP->plmn_Identity.mcc;
 		/* WCDMA MCC*/
 		for(counter=0;counter< 3 ;counter++)
 		{
 			sprintf(tempBuff+counter,"%d",wcdmaMcc.elem[counter]);
 		}
 		strncpy(testContext->WcdmaRadioInfo.wcdmaCellInfo.MCC,tempBuff,4);
 		/* WCDMA MNC*/
 		wcdmaMnc = mMasterInformationBlock->plmn_Type.u.gsm_MAP->plmn_Identity.mnc;
 		strncpy(tempBuff,"\0",4);
 		for(counter=0;counter<(wcdmaMnc.n);counter++)
 		{
 			sprintf(tempBuff+counter,"%d",wcdmaMnc.elem[counter]);
 		}
 		strncpy(testContext->WcdmaRadioInfo.wcdmaCellInfo.MNC,tempBuff,4);
	}
}

void updateWcdmaCellLac(TestContext *testContext,SysInfoType1  *mSysInfoType1)
{
  u_int32_t counter=0;
  for(counter=0;counter<mSysInfoType1->cn_CommonGSM_MAP_NAS_SysInfo.numocts;counter++)
 	 {
	  if (!counter){
	 	testContext->WcdmaRadioInfo.wcdmaCellInfo.LAC |=  mSysInfoType1->cn_CommonGSM_MAP_NAS_SysInfo.data[counter];
	 	testContext->WcdmaRadioInfo.wcdmaCellInfo.LAC = testContext->WcdmaRadioInfo.wcdmaCellInfo.LAC << 8;
	  }else
		 testContext->WcdmaRadioInfo.wcdmaCellInfo.LAC |= mSysInfoType1->cn_CommonGSM_MAP_NAS_SysInfo.data[counter];
 	 }
}

void  updateWcdmaMaxAllowedUlTxPower(TestContext *testContext,SysInfoType3 *mSysInfoType3)
{
   /*Max_Allow_UL_Tx_Power*/  
   testContext->WcdmaRadioInfo.wcdmaCellInfo.Max_Allow_UL_Tx_Power= mSysInfoType3->cellSelectReselectInfo.maxAllowedUL_TX_Power;
}

void decodeSIBMessage(OSCTXT *p_lctxt,TestContext *testContext ,SIB_Type sibType , u_int8_t *data,u_int32_t length )
{
	//This is SIB Message now , we can directly decode based on sibType and print the same
	//
	u_int32_t datalength = length/8;
	if(length%8) { datalength++;}
	/*d_arraylog(data,length);*/

	pu_setBuffer(p_lctxt,data,datalength,0);/*To set buffer pointer for decoding*/
	
	int decode = 0;
	switch(sibType)
	{
		case masterInformationBlock :
			{
				MasterInformationBlock masterInfoBlock;	
				decode = asn1PD_MasterInformationBlock(p_lctxt, &masterInfoBlock);
				if(decode ==0) {
					strcpy(testContext->msgName,"MasterInformationBlock");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_MasterInformationBlock (p_lctxt,"MasterInforBlock", &masterInfoBlock);
					updateCellMccAndMnc(testContext,&masterInfoBlock);
				}
				else{
					d_warning("Unable to decoder MIB");
				}
			}
			break;
		case systemInformationBlockType1:
			{	
				SysInfoType1  mSysInfoType1;			
				decode = asn1PD_SysInfoType1(p_lctxt, &mSysInfoType1);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType1");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType1 (p_lctxt,"SysInfoType1", &mSysInfoType1);
					mSysInfoType1.cn_CommonGSM_MAP_NAS_SysInfo.numocts;
					updateWcdmaCellLac(testContext,&mSysInfoType1);

				}
				else{
					d_warning("Unable to decoder SysInfoType1");
				}
			}
			break;
		case systemInformationBlockType2:
			{
				SysInfoType2  mSysInfoType2;
				decode = asn1PD_SysInfoType2(p_lctxt, &mSysInfoType2);
				if(decode !=0) {
					strcpy(testContext->msgName,"SysInfoType2");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType2 (p_lctxt,"SysInfoType2", &mSysInfoType2);

				}
				else{
					d_warning("Unable to decoder SysInfoType2");
				}
			}
			break;
		case systemInformationBlockType3:
			{
				SysInfoType3  mSysInfoType3;
				decode = asn1PD_SysInfoType3(p_lctxt, &mSysInfoType3);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType3");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType3 (p_lctxt,"SysInfoType3", &mSysInfoType3);
                                        updateWcdmaMaxAllowedUlTxPower(testContext,&mSysInfoType3);
				}
				else{
					d_warning("Unable to decoder SysInfoType3");
				}
			}
			break;
			case systemInformationBlockType4:
			{
				SysInfoType4  mSysInfoType4;
				decode = asn1PD_SysInfoType4(p_lctxt, &mSysInfoType4);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType4");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType4 (p_lctxt,"SysInfoType4", &mSysInfoType4);
				}
				else{
					d_warning("Unable to decoder SysInfoType4");
				}
			}
			break;
				case systemInformationBlockType5:
			{
				SysInfoType5  mSysInfoType5;
				decode = asn1PD_SysInfoType5(p_lctxt, &mSysInfoType5);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType5");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType5 (p_lctxt,"SysInfoType5", &mSysInfoType5);
				}
				else{
					d_warning("Unable to decoder SysInfoType5");
				}
			}
			break;
		case systemInformationBlockType6:
			{
				SysInfoType6  mSysInfoType6;
				decode = asn1PD_SysInfoType6(p_lctxt, &mSysInfoType6);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType6");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType6 (p_lctxt,"SysInfoType6", &mSysInfoType6);
				}
				else{
					d_warning("Unable to decoder SysInfoType6");
				}

			}
			break;
		case systemInformationBlockType7:
			{
				SysInfoType7  mSysInfoType7;
				decode = asn1PD_SysInfoType7(p_lctxt, &mSysInfoType7);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType7");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType7 (p_lctxt,"SysInfoType7", &mSysInfoType7);
				}
				else{
					d_warning("Unable to decoder SysInfoType7");
				}
			}
			break;
		case systemInformationBlockType11:
			{
				SysInfoType11  mSysInfoType11;
				decode = asn1PD_SysInfoType11(p_lctxt, &mSysInfoType11);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType11");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType11 (p_lctxt,"SysInfoType11", &mSysInfoType11);
					/*updateDetectedSet(testContext,&mSysInfoType11);*/
				}
				else{
					d_warning("Unable to decoder SysInfoType11");
				}
			}
			break;
		case systemInformationBlockType12:
			{
				SysInfoType12  mSysInfoType12;
				decode = asn1PD_SysInfoType12(p_lctxt, &mSysInfoType12);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType12");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType12 (p_lctxt,"SysInfoType12", &mSysInfoType12);
				}
				else{
					d_warning("Unable to decoder SysInfoType12");
				}
			}
			break;
		case systemInformationBlockType13:
			{
				SysInfoType13  mSysInfoType13;
				decode = asn1PD_SysInfoType13(p_lctxt, &mSysInfoType13);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType13");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType13 (p_lctxt,"SysInfoType13", &mSysInfoType13);
				}
				else{
					d_warning("Unable to decoder SysInfoType13");
				}
			}
			break;
	case systemInformationBlockType13_1:
			{
				SysInfoType13_1  mSysInfoType13_1;
				decode = asn1PD_SysInfoType13_1(p_lctxt, &mSysInfoType13_1);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType13_1");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType13_1 (p_lctxt,"SysInfoType13_1", &mSysInfoType13_1);
				}
				else{
					d_warning("Unable to decoder SysInfoType13_1");
				}
			}
			break;
	case systemInformationBlockType13_2:
			{
				SysInfoType13_2  mSysInfoType13_2;
				decode = asn1PD_SysInfoType13_2(p_lctxt, &mSysInfoType13_2);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType13_2");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType13_2 (p_lctxt,"SysInfoType13_2", &mSysInfoType13_2);
				}
				else{
					d_warning("Unable to decoder SysInfoType13_2");
				}
			}
			break;
	case systemInformationBlockType13_3:
			{
				SysInfoType13_3  mSysInfoType13_3;
				decode = asn1PD_SysInfoType13_3(p_lctxt, &mSysInfoType13_3);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType13_3");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType13_3 (p_lctxt,"SysInfoType13_3", &mSysInfoType13_3);
				}else {
					d_warning("Unable to decoder SysInfoType13_3");
				}
			}
			break;
	case systemInformationBlockType13_4:
			{
				SysInfoType13_4  mSysInfoType13_4;
				decode = asn1PD_SysInfoType13_4(p_lctxt, &mSysInfoType13_4);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType13");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType13_4 (p_lctxt,"SysInfoType13_4", &mSysInfoType13_4);
				}
				else {
					d_warning("Unable to decoder SysInfoType13_4");
				}
			}
			break;
	case systemInformationBlockType14:
			{
				SysInfoType14  mSysInfoType14;
				decode = asn1PD_SysInfoType14(p_lctxt, &mSysInfoType14);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType14");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType14 (p_lctxt,"SysInfoType14", &mSysInfoType14);
				}
				else{
					d_warning("Unable to decoder SysInfoType14");
				}
			}
			break;
		case systemInformationBlockType15:
			{
				SysInfoType15  mSysInfoType15;
				decode = asn1PD_SysInfoType15(p_lctxt, &mSysInfoType15);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType15_1");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType15 (p_lctxt,"SysInfoType15", &mSysInfoType15);
				}
				else{
					d_warning("Unable to decoder SysInfoType15_1");
				}
			}
			break;
		case systemInformationBlockType15_1:
			{
				SysInfoType15_1  mSysInfoType15_1;
				decode = asn1PD_SysInfoType15_1(p_lctxt, &mSysInfoType15_1);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType15_1");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType15_1 (p_lctxt,"SysInfoType15_1", &mSysInfoType15_1);
				}
				else{
					d_warning("Unable to decoder SysInfoType15_1");
				}
			}
			break;
	
		case systemInformationBlockType15_2:
			{
				SysInfoType15_2  mSysInfoType15_2;
				decode = asn1PD_SysInfoType15_2(p_lctxt, &mSysInfoType15_2);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType15_2");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType15_2 (p_lctxt,"SysInfoType15_2", &mSysInfoType15_2);
				}
				else{
					d_warning("Unable to decoder SysInfoType15_1");
				}
			}
			break;
		case systemInformationBlockType15_3:
			{
				SysInfoType15_3  mSysInfoType15_3;
				decode = asn1PD_SysInfoType15_3(p_lctxt, &mSysInfoType15_3);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType15_3");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType15_3 (p_lctxt,"SysInfoType15_3", &mSysInfoType15_3);
				}
				else{
					d_warning("Unable to decoder SysInfoType15_1");
				}
			}
			break;
				
		case systemInformationBlockType15_4:
			{
				SysInfoType15_4  mSysInfoType15_4;
				decode = asn1PD_SysInfoType15_4(p_lctxt, &mSysInfoType15_4);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType15_4");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType15_4(p_lctxt,"SysInfoType15_4", &mSysInfoType15_4);
				}
				else{
					d_warning("Unable to decoder SysInfoType15_1");
				}
			}
			break;
		case systemInformationBlockType16:
			{
				SysInfoType16  mSysInfoType16;
				decode = asn1PD_SysInfoType16(p_lctxt, &mSysInfoType16);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType16");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType16 (p_lctxt,"SysInfoType16", &mSysInfoType16);
				}
				else{
					d_warning("Unable to decoder SysInfoType16");
				}
			}
			break;
		case systemInformationBlockType17:
			{
				SysInfoType17  mSysInfoType17;
				decode = asn1PD_SysInfoType17(p_lctxt, &mSysInfoType17);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType17");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType17 (p_lctxt,"SysInfoType17", &mSysInfoType17);
				}
				else{
					d_warning("Unable to decoder SysInfoType17");
				}
			}
			break;
		case systemInformationBlockType18:
			{
				SysInfoType18  mSysInfoType18;
				decode = asn1PD_SysInfoType18(p_lctxt, &mSysInfoType18);
				if(decode ==0) {
					strcpy(testContext->msgName,"SysInfoType18");
					updateGenericParameter(testContext);	
					asn1PrtToStrm_SysInfoType18 (p_lctxt,"SysInfoType18", &mSysInfoType18);
				}
				else{
					d_warning("Unable to decoder SysInfoType18");
				}
			}
			break;
	default:
			break;
	}
/*
   systemInformationBlockType15_5 = 29,
   systemInformationBlockType5bis = 30,
   extensionType = 31
	*/

}

#endif
void copybits( u_int8_t *srcData , u_int8_t *destData , u_int32_t len , u_int8_t dstBitPos)
{
	u_int8_t srcCounter=0,modulus=0,destCounter=0;
	if(!dstBitPos)
	{
		/* dstBitPos is zero*/
		for(srcCounter=0;srcCounter<(len/8);srcCounter++,destCounter++)
		{
			/* Copy all Bytes from src to dest*/
			destData[destCounter]=srcData[srcCounter];
		}
		if((modulus=len%8))
		{
			/* modulus is greater than zero, so copybits*/
			u_int8_t tempValue = ((1<<modulus)-1) << (8-modulus);
			destData[destCounter] = (tempValue) & srcData[srcCounter] ;
		}
	}
	else
	{
		u_int8_t byteLength = len/8;
		while(byteLength)
		{
			//Copy the source byte to different destination
			destData[destCounter] 		= destData[destCounter] | (srcData[srcCounter] >> dstBitPos);
			destData[destCounter+1] 	= srcData[srcCounter]<< (8-dstBitPos);		
			destCounter++;
			srcCounter++;
			byteLength--;
		}
		if((modulus = len%8))
	       	{	//Still last bits need to be copy
			u_int8_t tempValue = ((1<<modulus)-1) << (8-modulus);
			tempValue = (tempValue) & srcData[srcCounter];
			destData[destCounter]   = destData[destCounter] | (tempValue >> dstBitPos);
			destData[destCounter+1]	= tempValue << (8-dstBitPos);
		}

	}
}
#if 0
void copybits( u_int8_t *srcData , u_int8_t *destData , u_int32_t len , u_int8_t dstBitPos)
{
	u_int8_t srcCounter=0,modulus=0,destCounter=0;
	if(!dstBitPos)
	{
		/* dstBitPos is zero*/
		for(srcCounter=0;srcCounter<(len/8);srcCounter++,destCounter++)
		{
			/* Copy all Bytes from src to dest*/
			destData[destCounter]=srcData[srcCounter];
		}
		if((modulus=len%8))
		{
			/* modulus is greater than zero, so copybits*/
			u_int8_t tempValue = 1<<modulus;
			destData[destCounter] = (tempValue-1) & srcData[srcCounter] ;
		}
	}
	else
	{
		u_int8_t byteLength = len/8;

		while(byteLength)
		{
			//Copy the source byte to different destination
			destData[destCounter] 	= destData[destCounter] | (srcData[srcCounter] << dstBitPos);
			destData[destCounter+1] 	= srcData[srcCounter]>> (8-dstBitPos);		
			destCounter++;
			srcCounter++;
			byteLength--;
		}
		if((modulus = len%8))
	       	{	//Still last bits need to be copy
			u_int8_t tempValue = 1<<modulus;
			tempValue = (tempValue-1) & srcData[srcCounter];
			destData[destCounter]   = destData[destCounter] | (tempValue << dstBitPos);
			destData[destCounter+1]	= tempValue >> (8-dstBitPos);
		}

	}
}
#endif





