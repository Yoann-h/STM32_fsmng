/*
 * csv_logger_cfg.h
 *
 *  Created on: Nov 30, 2025
 *      Author: yoyos
 */

#ifndef FILESYSTEM_CSV_LOGGER_CFG_H_
#define FILESYSTEM_CSV_LOGGER_CFG_H_

#include "stm32l4xx_hal.h"

#define CSVDataNb 10
extern char* cLogPath;
extern int iLogFrequency;
extern uint8_t u8EnableCSV;

#endif /* FILESYSTEM_CSV_LOGGER_CFG_H_ */
