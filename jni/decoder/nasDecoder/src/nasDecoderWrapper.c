#include <stdio.h>
#include <stdlib.h>
#include "TS24008Msgs.h"
#include "rtxsrc/rtxDiag.h"
#include "rtxsrc/rtxDiagBitTrace.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxFile.h"
#include "rtxsrc/rtxMemLeakCheck.h"
#include "rtxsrc/rtxPrintStream.h"
#include "nasDecoderWrapper.h"
#include "errors.h"
#include "lteNasDecoder.h"
#include "ccEvents.h"
#include "interfaceapi.h"


static unsigned char eventbuff[20];
static int causeValue = 0;
static char number[15];

extern int sendInformation ( int systype, int notifier ,int identifier , char *buf , int length);

/*UMTS_NAS_SIGNALING*/
struct nas_signaling{
 u_int8_t msg_dir;
 u_int32_t msg_lenth;
 u_int8_t data[0];
}__attribute__ ((packed));
typedef struct nas_signaling nas_signaling;

void decodeNASbuffer(	void *p_encoded_data,
		u_int32_t    encoded_buflen,
		u_int32_t    type,/*Not used*/
		u_int32_t Id)/*Id = 0 will be recieved here always*/
{
	TS24008Msg_PDU data;

	OSCTXT *p_lctxt = (OSCTXT *)MALLOC(sizeof(OSCTXT));
	OSOCTET*     msgbuf = (OSOCTET *)p_encoded_data;

	int          ret;
	OSSIZE       len = encoded_buflen;
	OSBOOL       trace = TRUE, verbose = FALSE;

	rtInitContext (p_lctxt);

	rtxSetDiag (p_lctxt, verbose);

	/* Set message buffer pointer */

	ret = rtxCtxtSetBufPtr (p_lctxt, msgbuf, len);

	memset(number, 0, sizeof(number));

	if (0 != ret) {
		////WARNING_LOG ("rtxCtxtSetBufPtr failed; status = %d\n", ret);
		rtxErrPrint (p_lctxt);
		rtxFreeContext (p_lctxt);
		if (NULL != p_lctxt)
		{
			FREE(p_lctxt);
		}
		return ;
	}

	/* Enable bit tracing */

	if (trace) {
		rtxDiagCtxtBitFieldListInit (p_lctxt);
	}

	/* Set protocol version number */
	rtxCtxtSetProtocolVersion (p_lctxt, 8);

	/* Call decode function */
	ret = NASDec_TS24008Msg_PDU (p_lctxt, &data);

	////////////////////for mo call/////////////////////////////////////////////////////////
	if(data.data.t == T_TS24008Msgs_TS24008_IE_OBJECTSET_obj_Setup) {
		if(data.data.u.obj_Setup->m.calledPartyNumberPresent){
			strcpy(number, data.data.u.obj_Setup->calledPartyNumber.number);
		} else if(data.data.u.obj_Setup->m.callingPartyNumberPresent) {
			strcpy(number, data.data.u.obj_Setup->callingPartyNumber.number);
		}
	}
	////////////////////for disconnect//////////////////////////////////////////////////////
	if(data.data.t == T_TS24008Msgs_TS24008_IE_OBJECTSET_obj_Disconnect) {
		causeValue = data.data.u.obj_Disconnect->cause.causeValue;
	}

	if (0 != ret) {
		rtxFreeContext (p_lctxt);
		if (NULL != p_lctxt)
		{
			FREE(p_lctxt);
		}
		return ;
	}

	if (verbose) {
		//WARNING_LOG ("Bit fields:  \n");
		rtxDiagBitTracePrint (p_lctxt->pBitFldList, "");
		//WARNING_LOG ("\n");

	}


	if (trace) {
		//memset(decodeMsgBuffer, 0, 60000);
		/* Register print-to-stream callback function */
		//rtxSetPrintStream(p_lctxt,writeToBuffer, decodeMsgBuffer);
		/* Invoke generated print-to-stream function */
		//asn1PrtToStrm_TS24008Msg_PDU (p_lctxt, "PDU", &data);
	}

	rtxFreeContext (p_lctxt);
	if (NULL != p_lctxt)
	{
		FREE(p_lctxt);
	}

}


void NASDecoderCCevents(unsigned char *buf, int length, u_int64_t cmdmillSeconds)
{
	int msgid = 0, protocol = 0, i;
	char *message, *msgName2 = NULL;

	//char *msgName = NULL;
	memset(eventbuff, 0, sizeof(eventbuff));

	nas_signaling *uns = (nas_signaling*)&buf[0];

	message = (char *)uns->data;

	if(((message[0]) & 0x0f) == 3 || ((message[0]) & 0x0f) == 5) {
	    msgid = (((message[1])& 0x3f));
	}
	else {
		msgid = (message[1]);
	}
	protocol = (message[0] & 0x0f);
	//printf("nas message name : %s\n\n", msgName[protocol][msgid]);

	if(protocol == 3) { //call control
		switch(msgid) {
		case 1: //alerting
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, ALERTING, eventbuff, sizeof(u_int64_t));
			break;
		case 2: //call proceeding
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, CALL_PROCEEDING, eventbuff, sizeof(u_int64_t));
			break;
		case 3: //progress
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, PROGRESS, eventbuff, sizeof(u_int64_t));
			break;
		case 5: //setup
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			decodeNASbuffer(uns->data, uns->msg_lenth, 0, 0);
			memcpy(eventbuff + sizeof(u_int64_t), number, strlen(number));
			sendInformation(WCDMA, EVENT_CC, SETUP, eventbuff, sizeof(u_int64_t) + strlen(number));
			break;
		case 7: //connect
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, CONNECT, eventbuff, sizeof(u_int64_t));
			break;
		case 15: //connect ack
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, CONNECT_ACK, eventbuff, sizeof(u_int64_t));
			break;
		case 37: //disconnect
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			decodeNASbuffer(uns->data, uns->msg_lenth, 0, 0);
			memcpy(eventbuff + sizeof(u_int64_t), (unsigned char *)&causeValue, sizeof(int));
			sendInformation(WCDMA, EVENT_CC, DISCONNECT, eventbuff, sizeof(u_int64_t)+sizeof(int));
			break;
		case 42: //release complete
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, RELEASE_COMPLETE, eventbuff, sizeof(u_int64_t));
			break;
		case 45: //release
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			sendInformation(WCDMA, EVENT_CC, RELEASE, eventbuff, sizeof(u_int64_t));
			break;
		}
	}
	if(protocol == 5) { //mobility management
		if(msgid == 36) { //cm service request
			int ismt = 0;
			memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
			memcpy(eventbuff + sizeof(u_int64_t), (unsigned char *)& ismt,sizeof(int32_t));
			sendInformation(WCDMA, EVENT_CC, CALL_START_INDICATION, eventbuff, sizeof(u_int64_t) + sizeof(int32_t));
		}
	}
}

