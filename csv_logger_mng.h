/*
 * csv_logger_mng.h
 *
 *  Created on: 30 nov. 2025
 *      Author: yoyos
 */

#ifndef FILESYSTEM_CSV_LOGGER_MNG_H_
#define FILESYSTEM_CSV_LOGGER_MNG_H_

#include "csv_logger_cfg.h"
#include "fs_mng.h"
#include "local_time_manager.h"

typedef enum {
	CSVLoggerMng_OK,
	CSVLoggerMng_PARAMETER_ERROR,
	CSVLoggerMng_INTERNAL_ERROR,
	CSVLoggerMng_NOTINITIATED,
	CSVLoggerMng_RESULTNB
} CSVLoggerMng_Result;

typedef struct {
	char* pcDataName;
	char* pcUnit;
	float* pvDataVal;
}ts_DataLog;

CSVLoggerMng_Result CSVLoggerMng_Init(char fcSeparator, int fiFrequency);
CSVLoggerMng_Result CSVLoggerMng_AddData(ts_DataLog* feData);
CSVLoggerMng_Result CSVLoggerMng_Record();
CSVLoggerMng_Result CSVLoggerMng_Task();

#endif /* FILESYSTEM_CSV_LOGGER_MNG_H_ */
