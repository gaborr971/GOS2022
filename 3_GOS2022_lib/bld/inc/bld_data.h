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
/**
 * TODO
 */
typedef struct
{
	u32_t                 appStartAddress;
	u32_t                 appSize;
	u32_t                 appCrc;
	lib_version_t         appVersion;
	u32_t                 appDataCrc;
}bld_app_data_struct_t;

/**
 * TODO
 */
typedef struct
{
	u32_t                 initPattern;
	u32_t                 bldStartAddress;
	u32_t                 bldSize;
	u32_t                 bldCrc;
	lib_version_t         bldVersion;
	bld_app_data_struct_t appData;
	bool_t                bootUpdateMode;
	u32_t                 bldDataCrc;
}bld_data_struct_t;

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
gos_result_t bld_dataGet (bld_data_struct_t* pData);

/**
 * TODO
 * @param
 * @return
 */
gos_result_t bld_dataSet (bld_data_struct_t* pData);

/**
 * TODO
 * @param
 * @return
 */
void_t bld_dataPrint (void_t);

#endif
