#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxDiag.h"
#include "rtxsrc/rtxDiagBitTrace.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxFile.h"
#include "rtxsrc/rtxMemLeakCheck.h"
#include "rtxsrc/rtxPrintStream.h"
#include "CSNRR.h"
#include "errors.h"
#include "interfaceapi.h"
#include "ccEvents.h"

static u_int32_t tmsi = 0;
static int ismt = 0;
static unsigned char eventbuff[20];

extern int sendInformation (int systype, int notifier ,int identifier , char *buf , int length);

//function to set current tmsi of ue
void setTMSI(u_int32_t network_tmsi) {
	tmsi = network_tmsi;
}

int decodegsmRRBuffer(void *p_encoded_data,
		u_int32_t    encoded_buflen,
		u_int32_t    type,/*Not used*/
		u_int32_t Id)/*Id = 0 will be recieved here always*/
{
	GSMRRPagingReqType1 data_pagingreqtype1 = {0};
	GSMRRPagingReqType2 data_pagingreqtype2 = {0};
	GSMRRPagingReqType3 data_pagingreqtype3 = {0};
	GSMRRPagingResp data_pagingresp = {0};
	GSMRRMeasurementReport data_measurementreport = {0};
	GSMRRClassmarkChange data_classmarkchange = {0};
	GSMRRUtranClassmarkChange data_utranclassmarkchange = {0};
	GSMRRCipheringModeCommand data_cipheringmodecommand = {0};
	GSMRRGPRSSuspensionRequest data_gprssuspensionreq = {0};
	GSMRRChannelRelease data_channelrelease = {0};
	GSMRRAssignmentComp data_assignmentcomp = {0};
	ImmediateAssign data_immediateassign = {0};
	GSMRRAssignmentCommand data_assignmentcommand = {0};

	OSCTXT *p_lctxt = (OSCTXT *)MALLOC(sizeof(OSCTXT));
	OSOCTET*     msgbuf = (OSOCTET *)p_encoded_data;
	OSOCTET*     finalmsgbuf;

	int          ret = 0, msg_id = (int)msgbuf[1];
	OSSIZE       len = encoded_buflen;
	OSBOOL       trace = TRUE, verbose = FALSE;

	char *msgName;

	ismt = 0;

	rtInitContext (p_lctxt);

	rtxSetDiag (p_lctxt, verbose);

	if((msgbuf[0] == 0x81) || (msgbuf[0] == 0x83))
	{
		finalmsgbuf = (OSOCTET *)(msgbuf + 4);
	}
	else if((msgbuf[0] == 0x00) || (msgbuf[0] == 0x80) || (msgbuf[0] == 0x04) || (msgbuf[0] == 0x84))
	{
		finalmsgbuf = (OSOCTET *)(msgbuf + 3);
	}

	/* Set message buffer pointer */
	ret = rtxCtxtSetBufPtr (p_lctxt, finalmsgbuf, len);
	if (0 != ret) {
		printf ("rtxCtxtSetBufPtr failed; status = %d\n", ret);
		rtxErrPrint (p_lctxt);
		rtxFreeContext (p_lctxt);
		if (NULL != p_lctxt)
		{
			FREE(p_lctxt);
		}
		return ismt;
	}

	/* Enable bit tracing */

	if (trace) {
		rtxDiagCtxtBitFieldListInit (p_lctxt);
	}

	/* Set protocol version number */

	rtxCtxtSetProtocolVersion (p_lctxt, 8);

	/* Call decode function */
	switch(msg_id)
	{
		case 0x21:
			{
				ret = NASDec_GSMRRPagingReqType1 (p_lctxt, &data_pagingreqtype1);
				if((data_pagingreqtype1.mobileIdentity1.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype1.mobileIdentity1.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				} else if((data_pagingreqtype1.mobileIdentity2.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype1.mobileIdentity2.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				}
				break;
			}
		case 0x22:
			{
				ret = NASDec_GSMRRPagingReqType2 (p_lctxt, &data_pagingreqtype2);
				if((data_pagingreqtype2.mobileIdentity1.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype2.mobileIdentity1.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				} else if((data_pagingreqtype2.mobileIdentity2.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype2.mobileIdentity2.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				} else if((data_pagingreqtype2.mobileIdentity3.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype2.mobileIdentity3.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				}
				break;
			}
		case 0x24:
			{
				ret = NASDec_GSMRRPagingReqType3 (p_lctxt, &data_pagingreqtype3);
				if((data_pagingreqtype3.mobileIdentity1.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype3.mobileIdentity1.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				} else if((data_pagingreqtype3.mobileIdentity2.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype3.mobileIdentity2.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				} else if((data_pagingreqtype3.mobileIdentity3.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype3.mobileIdentity3.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				} else if((data_pagingreqtype3.mobileIdentity4.mobid.t == 4) && (*(u_int32_t*)(data_pagingreqtype3.mobileIdentity4.mobid.u.tmsi) == tmsi)) {
					ismt = 1;
				}
				break;
			}
		case 0x27:
			{
				ret = NASDec_GSMRRPagingResp (p_lctxt, &data_pagingresp);
				break;
			}
		case 0x15:
			{
				ret = NASDec_GSMRRMeasurementReport (p_lctxt, &data_measurementreport);
				break;
			}
		case 0x16:
			{
				ret = NASDec_GSMRRClassmarkChange (p_lctxt, &data_classmarkchange);
				break;
			}
		case 0x0d:
			{
				ret = NASDec_GSMRRChannelRelease (p_lctxt, &data_channelrelease);
				break;
			}
		case 0x34:
			{
				ret = NASDec_GSMRRGPRSSuspensionRequest (p_lctxt, &data_gprssuspensionreq);
				break;
			}

		case 0x60:
			{ 
				ret = NASDec_GSMRRUtranClassmarkChange (p_lctxt, &data_utranclassmarkchange);
				break;
			}

		case 0x35:
			{
				ret = NASDec_GSMRRCipheringModeCommand (p_lctxt, &data_cipheringmodecommand);
				break;
			}
		case 0x29:
			{
				ret = NASDec_GSMRRAssignmentComp (p_lctxt, &data_assignmentcomp);
				break;
			}
		case 0x3f:
			{
				ret = NASDec_ImmediateAssign (p_lctxt, &data_immediateassign);
				break;
			}
		case 0x2e: 
			{
				ret = NASDec_GSMRRAssignmentCommand (p_lctxt, &data_assignmentcommand);
				break;	
			}
		default:
			{
				//WARNING_LOG("Decoding of the message received is not supported");
				rtxFreeContext (p_lctxt);
				if (NULL != p_lctxt)
				{
					FREE(p_lctxt);
				}
				return ismt;
				break;

			}
	}

	rtxFreeContext (p_lctxt);
	if (NULL != p_lctxt)
	{
		FREE(p_lctxt);
	}
	return ismt;
}

void CSNDecoderCCevents(void *encoded_data, int length, u_int64_t cmdmillSeconds)
{
	OSOCTET*     msgbuf = (OSOCTET *)encoded_data;
	int msg_id = (int)msgbuf[1];
	int ret = 0;
	memset(eventbuff, 0, sizeof(eventbuff));
	switch(msg_id)
	{
		case 0x21:  //GSMRRPagingReqType1
		{
			ret = decodegsmRRBuffer(encoded_data, length, 0, 0);
			break;
		}
		case 0x22:  //GSMRRPagingReqType2
		{
			ret = decodegsmRRBuffer(encoded_data, length, 0, 0);
			break;
		}
		case 0x24:  //GSMRRPagingReqType3
		{
			ret = decodegsmRRBuffer(encoded_data, length, 0, 0);
			break;
		}
	}
	if(ret == 1) {
		memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
		memcpy(eventbuff + sizeof(u_int64_t), (unsigned char *)& ret,sizeof(int));
		sendInformation(WCDMA, EVENT_CC, CALL_START_INDICATION, eventbuff, sizeof(u_int64_t) + sizeof(int));
	}
}
