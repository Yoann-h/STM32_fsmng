/*
 * fs_mng.h
 *
 *  Created on: Nov 2, 2025
 *      Author: yoyos
 */

#ifndef YHLIB_FS_FS_MNG_H_
#define YHLIB_FS_FS_MNG_H_

#include "fatfs.h"
#include "stm32l4xx_hal.h"

typedef enum {
	FSMng_OK,
	FSMng_PARAMETER_ERROR,
	FSMng_INTERNAL_ERROR,
	FSMng_NOTINITIATED,
	FSMng_NOFILE,
	FSMng_RESULTNB
} FSMng_Result;

typedef enum {
	FSMng_NewFile,
	FSMng_Append,
	FSMng_OptionNB
} FSMng_Option;

FRESULT fs_mng_Init();
FRESULT fs_mng_Deinit();
FSMng_Result fs_mng_readFile(char* fcppath, void* fcbuffer, int fiBufSize);
FSMng_Result fs_mng_writeFile(char* fcppath, void* fcbuffer, int fiBufSize, FSMng_Option feOption);
FSMng_Result fs_mng_PartitionSize(int* fpiSize);
FSMng_Result fs_mng_fileExist(char* fcppath, uint32_t* fpFileSize);
FSMng_Result fs_mng_dispDirectory(char *path, char *outString, int bufsize);
FSMng_Result fs_mng_deleteFile(char* fcppath);
FSMng_Result fs_mng_createDir(char* fcpDirPath);
FSMng_Result fs_mng_PrintFile(char *fcppath, char* pcBuffer, int iBufSize, UART_HandleTypeDef* huart);

#endif /* YHLIB_FS_FS_MNG_H_ */
