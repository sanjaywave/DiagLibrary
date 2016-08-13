/*
 *  File Name : - 	CdmaContext.h
 *  Created on: -	23-Aug-2013
 *  Description : - This files contains the CDMA Keep performance indicator (KPI)
 */
#ifndef CDMACONTEXT_H_
#define CDMACONTEXT_H_
#define MAX_PILOT 5

typedef struct pilotInfo {
	unsigned int pilotPN;
	float EcIO;
	float C_I;
}PilotInfo;

typedef struct CdmaKpi{
	int  ActivePilotPN;
	int  RxPwr_0;
	int  RxPwr_1;
	float  SNR;
	int  TxADJ;
	int  TxTotalPwr;
	int  maxPilot;
	int bad_frames;
	int total_frames;
	float Frame_err_rate;
	unsigned int bandClass;

	PilotInfo pilotInfo[MAX_PILOT];
}CdmaKpi;



typedef struct Kpi_CDMA{
	unsigned int  ActivePilotPN;
	int  RxPwr_0;
	int  RxPwr_1;
	float  SNR;
	int  TxADJ;
	int  TxTotalPwr;
	unsigned int  maxPilot;
	float Frame_err_rate;

	PilotInfo pilotInfo[MAX_PILOT];
}Kpi_CDMA;

int get_CDMA_Info(Kpi_CDMA *kpi_cdma); //external kpi

int get_CDMACellInfo(CdmaKpi *cdmaKpi );//internal kpi

#endif /* CDMACONTEXT_H_ */
