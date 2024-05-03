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
//! @file       svl_hwi.h
//! @author     Ahmed Gazar
//! @date       2024-04-28
//! @version    1.0
//!
//! @brief      GOS2022 Library / Hardware Info header.
//! @details    This component TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-28    Ahmed Gazar     Initial version created.
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
#ifndef SVL_HWI_H
#define SVL_HWI_H
/*
 * Includes
 */
#include <gos.h>

/*
 * Macros
 */
/**
 * TODO
 */
//#define SVL_FRS_FAULT_DATA_SIZE  ( 64u )

#define SVL_HWI_CRC_ADDR             ( 0u  )

#define SVL_HWI_ENTRY_START_ADDR     ( SVL_HWI_CRC_ADDR + sizeof(u32_t) )

#define SVL_HWI_ENTRY_SIZE           ( sizeof(svl_hwiInfo_t) )

#define SVL_HWI_BOARD_NAME_LENGTH    ( 32u )

#define SVL_HWI_REVISION_LENGTH      ( 8u  )

#define SVL_HWI_AUTHOR_LENGTH        ( 32u )

#define SVL_HWI_DESCRIPTION_LENGTH   ( 64u )

#define SVL_HWI_SERIAL_NUMBER_LENGTH ( 64u )

/*
 * Type definitions
 */
/**
 * TODO
 */
typedef gos_result_t (*svl_hwiReadWriteFunc_t)(u32_t address, u8_t* pData, u16_t size);

/**
 * TODO
 */
typedef struct
{
	char_t boardName [SVL_HWI_BOARD_NAME_LENGTH];
	char_t revision [SVL_HWI_REVISION_LENGTH];
	char_t author [SVL_HWI_AUTHOR_LENGTH];
	char_t description [SVL_HWI_DESCRIPTION_LENGTH];
	gos_time_t date;
	char_t serialNumber [SVL_HWI_SERIAL_NUMBER_LENGTH];
}svl_hwiInfo_t;

/**
 * TODO
*/
typedef struct
{
	svl_hwiReadWriteFunc_t readFunction;
	svl_hwiReadWriteFunc_t writeFunction;
}svl_hwiInitStruct_t;

// TODO
gos_result_t svl_hwiInit (svl_hwiInitStruct_t* pInitStruct);

// TODO
gos_result_t svl_hwiRead (svl_hwiInfo_t* pHwInfo);

// TODO
gos_result_t svl_hwiWrite (svl_hwiInfo_t* pHwInfo);

#endif
