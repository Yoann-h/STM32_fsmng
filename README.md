
# STM32 file system manager

Intermediate layer interface for operations using file system. The library is a work in progress and will lately be able to use multiple file systems. 

## Description

The project offers generic interface to operate on memory through file systems. It offers the ability to create/read/delete files and folders. For now, only one partition is managed and only fatfs is taken into account.
The project includes as well the necessary services to manage a csv-type (coma -separated txt) datalog file. 

## Getting Started

### Dependencies

The project is thought to be used in an STM32Cube IDE-type project, so it will need the access to many low-level services offered by "stm32zyxx_hal.h".

### Installing

Simply download the repository content and include the folder directly in your stm32Cube project.
The project will need an "SPI_HandleTypeDef" object, defined in main.h as SD_SPI_HANDLE:

    /* USER CODE BEGIN Private defines */
    #define  SD_SPI_HANDLE hspi3
    /* USER CODE END Private defines */

The SPI_HandleTypedDef can be configured from the STM32Cube IDE configuration interface (which generates the file spi.c) , it should be adapted to your memory device. Here is an example for an SD card:

    /* SPI3 init function */
    void  MX_SPI3_Init(void)
    {
    /* USER CODE BEGIN SPI3_Init 0 */
    /* USER CODE END SPI3_Init 0 */
    /* USER CODE BEGIN SPI3_Init 1 */
    /* USER CODE END SPI3_Init 1 */
    hspi3.Instance  = SPI3;
    hspi3.Init.Mode  = SPI_MODE_MASTER;
    hspi3.Init.Direction  = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize  = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity  = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase  = SPI_PHASE_1EDGE;
    hspi3.Init.NSS  = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_256;
    hspi3.Init.FirstBit  = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode  = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation  = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial  =  7;
    hspi3.Init.CRCLength  = SPI_CRC_LENGTH_DATASIZE;
    hspi3.Init.NSSPMode  = SPI_NSS_PULSE_ENABLE;
    if  (HAL_SPI_Init(&hspi3)  !=  HAL_OK)
    {
    Error_Handler();
    }
    /* USER CODE BEGIN SPI3_Init 2 */
    /* USER CODE END SPI3_Init 2 */
    }


### Using the services

#### Using the file system
For now every operation uses the Fatfs file system.
First, call **fs_mng_Init()** to mount the file system.
Then, create and write in a file with **fs_mng_writeFile(char*  fcppath,  void*  fcbuffer,  int  fiBufSize,  FSMng_Option  feOption)**, use FSMng_NewFile or FSMng_Append as option.
You don't have to manage the file opening or closing steps.
Read the file content with **fs_mng_readFile(char*  fcppath,  void*  fcbuffer,  int  fiBufSize)**.
Finaly, you can delete the file with **fs_mng_deleteFile(char*  fcppath)**.
You can also create a subfolder with **fs_mng_createDir(char*  fcpDirPath)**.
The interface offers some useful tools as well:

 - fs_mng_fileExist(char*  fcppath): check if a file exists
 - fs_mng_PartitionSize(int*  fpiSize): get the available memory size
 - fs_mng_PrintFile(char  *fcppath,  char*  pcBuffer,  int  iBufSize,  UART_HandleTypeDef*  huart): print the file content on an uart interface

#### Using the CSV data logger
`



## Help

### Testing Conditions



## Authors

Yoann Hervagault

