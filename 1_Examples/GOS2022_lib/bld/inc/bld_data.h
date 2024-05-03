//*************************************************************************************************
//
//                            #####             #####             #####
//                          #########         #########         #########
//                         ##                ##       ##       ##
//                        ##                ##         ##        #####
//                        ##     #####      ##         ##           #####
//                         ##       ##       ##       ##                ##
//                          #########         #########         #########
//                            #####             #####             #####
//
//                                      (c) Ahmed Gazar, 2024
//
//*************************************************************************************************
//! @file       bld_data.h
//! @author     Ahmed Gazar
//! @date       2024-02-07
//! @version    1.0
//!
//! @brief      GOS2022 Library / Bootloader data header.
//! @details    TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-07    Ahmed Gazar     Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2024 Ahmed Gazar
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//*************************************************************************************************
#ifndef BLD_DATA_H
#define BLD_DATA_H

/*
 * Includes
 */
#include <bld_cfg.h>
#include <libdef.h>

/*
 * Type definitions
 */
typedef struct
{
	u32_t         initPattern;
	u32_t         startAddress;
	u32_t         size;
	u32_t         crc;
	lib_version_t driverVersion;
	lib_version_t version;
	bool_t        updateMode;
	u32_t         dataCrc;
}bld_bootloaderData_t;

typedef struct
{
	u32_t         initPattern;
	u32_t         startAddress;
	u32_t         size;
	u32_t         crc;
	lib_version_t driverVersion;
	lib_version_t version;
	u32_t         dataCrc;
}bld_appData_t;

typedef struct
{
	u32_t         initPattern;
	bool_t        waitForConnectionOnStartup;
	u8_t          startupCounter;
	u32_t         connectionTimeout;
	u32_t         requestTimeout;
	u32_t         installTimeout;
	u32_t         dataCrc;
}bld_bootloaderConfig_t;

typedef struct
{
	bld_bootloaderData_t   bldData;
	bld_appData_t          appData;
	bld_bootloaderConfig_t bldConfig;
}bld_packedData;

/*
 * Function prototypes
 */
/**
 * TODO
 * @param
 * @return
 */
gos_result_t bld_dataInitialize (void_t);

/**
 * TODO
 * @param
 * @return
 */
gos_result_t bld_dataReset (void_t);

/**
 * TODO
 * @param
 * @return
 */
gos_result_t bld_dataGet (bld_bootloaderData_t* pData);

/**
 * TODO
 * @param pData
 * @return
 */
gos_result_t bld_appDataSet (bld_appData_t* pData);

/**
 * TODO
 * @param pData
 * @return
 */
gos_result_t bld_configSet (bld_bootloaderConfig_t* pData);

/**
 * TODO
 * @param
 * @return
 */
gos_result_t bld_dataSet (bld_bootloaderData_t* pData);

/**
 * TODO
 * @param pData
 * @return
 */
gos_result_t bld_appDataGet (bld_appData_t* pData);

/**
 * TODO
 * @param pData
 * @return
 */
gos_result_t bld_configGet (bld_bootloaderConfig_t* pData);

/**
 * TODO
 * @param
 * @return
 */
void_t bld_dataPrint (void_t);

#endif
