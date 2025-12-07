/*
 * csv_logger_mng.c
 *
 *  Created on: 30 nov. 2025
 *      Author: yoyos
 */

#include "csv_logger_mng.h"



ts_DataLog eDataList[CSVDataNb];
char cSeparator;
uint8_t u8DataNb = 0;
extern ts_ltm LocalTime;
char*  cLogPath= "/dataLog.csv";
int iLogFrequency=0;
uint8_t u8EnableCSV=0;

CSVLoggerMng_Result CSVLoggerMng_Init(char fcSeparator, int fiFrequency)
{
	CSVLoggerMng_Result res = CSVLoggerMng_OK;
	cSeparator = fcSeparator;
	iLogFrequency = fiFrequency;
	char headerBuffer[64];
	memset(headerBuffer,0,64);
//	if(fs_mng_fileExist(cLogPath)==FSMng_OK)
//	{
//
//	}
	strcat(headerBuffer,"Date");
	headerBuffer[strlen(headerBuffer)]=cSeparator;
	strcat(headerBuffer,"Time");
	headerBuffer[strlen(headerBuffer)]=cSeparator;
	for(uint8_t i=0;i<u8DataNb;i++)
	{
		strcat(headerBuffer,eDataList[i].pcDataName);
		headerBuffer[strlen(headerBuffer)]='(';
		strcat(headerBuffer,eDataList[i].pcUnit);
		headerBuffer[strlen(headerBuffer)]=')';
		headerBuffer[strlen(headerBuffer)]=cSeparator;
	}
	headerBuffer[strlen(headerBuffer)]='\n';
	fs_mng_writeFile(cLogPath, headerBuffer, strlen(headerBuffer), FSMng_NewFile);
	return res;
}
CSVLoggerMng_Result CSVLoggerMng_AddData(ts_DataLog* feData)
{
	CSVLoggerMng_Result res = CSVLoggerMng_OK;
	if(u8DataNb<CSVDataNb)
	{
		//eDataList[u8DataNb] = feData;
		memcpy(&eDataList[u8DataNb], feData, sizeof(ts_DataLog));
		u8DataNb++;
	}
	return res;
}
CSVLoggerMng_Result CSVLoggerMng_Record()
{
	CSVLoggerMng_Result res = CSVLoggerMng_OK;
	sRTCDateTime dt;
	char lineBuffer[128];
	memset(lineBuffer,0,128);
	ltm_UnixToDateTime(LocalTime.u32LocalTime, &dt);
	char cStrLine[17];
	dateFormat("dmy", dt, cStrLine);
	strcat(lineBuffer,cStrLine);
	//strcat(lineBuffer,cSeparator);
	lineBuffer[strlen(lineBuffer)]=cSeparator;
	memset(cStrLine, 0, 17);
	dateFormat("H:i:s", dt, cStrLine);
	strcat(lineBuffer,cStrLine);
	lineBuffer[strlen(lineBuffer)]=cSeparator;
	memset(cStrLine, 0, 17);
	for(uint8_t i=0;i<u8DataNb;i++)
	{
		char cformat[10]={0};
		sprintf(cformat,"%%.%uf",eDataList[i].u8Digitnb);
		sprintf(cStrLine,cformat,*(eDataList[i].pvDataVal));
		strcat(lineBuffer,cStrLine);
		lineBuffer[strlen(lineBuffer)]=cSeparator;
		memset(cStrLine, 0, 17);
	}
	lineBuffer[strlen(lineBuffer)]='\n';
	fs_mng_writeFile(cLogPath, lineBuffer, strlen(lineBuffer), FSMng_Append);
	return res;
}

CSVLoggerMng_Result CSVLoggerMng_PrintLog(char* pcBuffer, int iBufSize, UART_HandleTypeDef* huart)
{
	CSVLoggerMng_Result res = CSVLoggerMng_OK;
	char cReadBuf[100];
	fs_mng_PrintFile(cLogPath, pcBuffer, iBufSize, huart);
	return res;
}

CSVLoggerMng_Result CSVLoggerMng_test(UART_HandleTypeDef* huart)
{
	CSVLoggerMng_Result res = CSVLoggerMng_OK;
	float data1Val=0;
	float data2Val=0;
	ts_DataLog eData={"data1","unit1",&data1Val,2};
	CSVLoggerMng_AddData(&eData);
	eData.pcDataName="data2";
	eData.pcUnit    ="unit2";
	eData.pvDataVal=&data2Val;
	eData.u8Digitnb=0;
	CSVLoggerMng_AddData(&eData);
	CSVLoggerMng_Init(';', 1);
	data1Val=1;
	data2Val=2;
	CSVLoggerMng_Record();
	data1Val=10;
	data2Val=20;
	HAL_Delay(2000);
	CSVLoggerMng_Record();
	char cBuffer[100];
	CSVLoggerMng_PrintLog(cBuffer, 100, huart);
	return res;
}
