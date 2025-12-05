/*
 * fs_mng.c
 *
 *  Created on: Nov 2, 2025
 *      Author: yoyos
 */


#include "fs_mng.h"

FATFS FatFs;
FIL fileHandler;
uint8_t u8fsMngInitiated = 0;

FRESULT fs_mng_Init()
{
	FRESULT fres;
	if(u8fsMngInitiated==0)
	{
		fres = f_mount(&FatFs, "", 1); //1=mount now
		u8fsMngInitiated=1;
	}
	return fres;
}

FRESULT fs_mng_Deinit()
{
	FRESULT fres;
	if(u8fsMngInitiated==1)
	{
		fres = f_mount(&FatFs, "", 0);
		u8fsMngInitiated=0;
	}
	return fres;
}

FSMng_Result fs_mng_createDir(char* fcpDirPath)
{
	FSMng_Result res=FSMng_OK;
	if(u8fsMngInitiated==0)
	{
		res=FSMng_NOTINITIATED;
	}
	else
	{
		if(fs_mng_fileExist(fcpDirPath)==FSMng_INTERNAL_ERROR)//folder already exists
		{
			if(f_mkdir(fcpDirPath)!=FR_OK)
			{
				res=FSMng_INTERNAL_ERROR;
			}
		}
	}
	return res;
}

FSMng_Result fs_mng_readFile(char* fcppath, void* fcbuffer, int fiBufSize)
{
	FSMng_Result res=FSMng_OK;
	if(u8fsMngInitiated==0)
	{
		res=FSMng_NOTINITIATED;
	}
	else
	{
		if(f_open(&fileHandler, fcppath, FA_READ)!=FR_OK)
		{
			res=FSMng_INTERNAL_ERROR;
		}
		else
		{
			//f_gets((TCHAR*)fcbuffer, fiBufSize, &fileHandler);
			int readenBytes=0;
			f_read (&fileHandler, fcbuffer, fiBufSize, &readenBytes);
			if(f_close(&fileHandler)!=FR_OK)
			{
				res=FSMng_INTERNAL_ERROR;
			}
		}
	}
	return res;
}

FSMng_Result fs_mng_writeFile(char* fcppath, void* fcbuffer, int fiBufSize, FSMng_Option feOption)
{
	FSMng_Result res=FSMng_OK;
	int writtenBytes=0;
	if(feOption>= FSMng_OptionNB || fcppath==0 || fcbuffer==0)
	{
		res = FSMng_PARAMETER_ERROR;
		return res;
	}
	if(u8fsMngInitiated==0)
	{
		res=FSMng_NOTINITIATED;
	}
	else
	{
		BYTE mode;
		if(feOption==FSMng_NewFile)
		{
			mode = FA_WRITE|FA_CREATE_ALWAYS;
		}
		else if (feOption==FSMng_Append)
		{
			mode = FA_OPEN_APPEND | FA_WRITE;
		}
		if(f_open(&fileHandler, fcppath, mode)!=FR_OK)
		{
			res=FSMng_INTERNAL_ERROR;
		}
		else
		{
			//f_puts((TCHAR*)fcbuffer, fiBufSize, &fileHandler);
			f_write (&fileHandler, fcbuffer, fiBufSize, &writtenBytes);
			if(f_close(&fileHandler)!=FR_OK)
			{
				res=FSMng_INTERNAL_ERROR;
			}
			else if (writtenBytes!=fiBufSize)
			{
				res=FSMng_PARAMETER_ERROR;
			}
		}
	}
	return res;
}

FSMng_Result fs_mng_PartitionSize(int* fpiSize)
{
	FSMng_Result res=FSMng_OK;
	if(fpiSize==NULL)
	{
		res=FSMng_PARAMETER_ERROR;
	}
	else
	{
		DWORD free_clusters, free_sectors, total_sectors;
		FRESULT testResult = f_getfree("", &free_clusters, &FatFs);
		if(testResult== FR_OK || testResult== FR_NO_FILE)
		{
			if(testResult== FR_OK )
			{
				total_sectors = (FatFs.n_fatent - 2) * FatFs.csize;
				*fpiSize = free_clusters * FatFs.csize;
			}
			else
			{
				res = FSMng_NOFILE;
			}
		}
		else
		{
			res=FSMng_INTERNAL_ERROR;
		}
	}
	return res;
}

FSMng_Result fs_mng_deleteFile(char* fcppath)
{
	FSMng_Result res=FSMng_OK;
	if(fcppath==NULL)
	{
		res=FSMng_PARAMETER_ERROR;
	}
	else
	{
		if(u8fsMngInitiated==1)
		{
			if(f_unlink(fcppath)!=FR_OK)
			{
				res=FSMng_INTERNAL_ERROR;
			}
		}
		else
		{
			res=FSMng_NOTINITIATED;
		}
	}
	return res;
}

FSMng_Result fs_mng_fileExist(char* fcppath)
{
	FILINFO fInfo;
	FSMng_Result res=FSMng_OK;
	if(fcppath==NULL)
	{
		res=FSMng_PARAMETER_ERROR;
	}
	if(u8fsMngInitiated==1)
	{
		if(f_stat(fcppath, &fInfo)!=FR_OK)
		{
			res=FSMng_INTERNAL_ERROR;
		}
	}
	else
	{
		res=FSMng_NOTINITIATED;
	}
	return res;
}

FSMng_Result fs_mng_dispDirectory(char *path, char *outString, int bufsize)
{
	FSMng_Result res=FSMng_OK;
	FRESULT fres;
    DIR dir;
    char clocalBuf[100];
    int totallength=0;
    memset(outString,0,bufsize);
    fres = f_opendir(&dir, path);

    if (fres == FR_OK)
    {
      while(1)
      {
        FILINFO fno;

        fres = f_readdir(&dir, &fno);

        if ((fres != FR_OK) || (fno.fname[0] == 0))
          break;

        sprintf(clocalBuf, "%c%c%c%c %10d %s/%s\r\n",
          ((fno.fattrib & AM_DIR) ? 'D' : '-'),
          ((fno.fattrib & AM_RDO) ? 'R' : '-'),
          ((fno.fattrib & AM_SYS) ? 'S' : '-'),
          ((fno.fattrib & AM_HID) ? 'H' : '-'),
          (int)fno.fsize, path, fno.fname);
        if(totallength + strlen(clocalBuf)<bufsize)
        {
        	totallength+=strlen(clocalBuf);
        	strcat(outString,clocalBuf);
        }
        else
        {
        	res = FSMng_PARAMETER_ERROR;
        	break;
        }
      }
    }
    return res;
}

FSMng_Result fs_mng_PrintFile(char *fcppath, char* pcBuffer, int iBufSize, UART_HandleTypeDef* huart)
{
	FSMng_Result res=FSMng_OK;
	if(f_open(&fileHandler, fcppath, FA_READ)!=FR_OK)
	{
		res=FSMng_INTERNAL_ERROR;
	}
	else
	{
		uint32_t fileSize = f_size(&fileHandler);
		uint32_t u32BytesCOunt=0;
		while(u32BytesCOunt<fileSize)
		{
			memset(pcBuffer,0,iBufSize);
			f_gets(pcBuffer,iBufSize,&fileHandler);
			u32BytesCOunt+=strlen(pcBuffer);
			HAL_UART_Transmit(huart, (uint8_t *)pcBuffer, strlen(pcBuffer), HAL_MAX_DELAY);
		}
		if(f_close(&fileHandler)!=FR_OK)
		{
			res=FSMng_INTERNAL_ERROR;
		}
	}
}
