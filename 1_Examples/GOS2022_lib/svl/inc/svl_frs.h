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
//! @file       svl_frs.h
//! @author     Ahmed Gazar
//! @date       2024-04-24
//! @version    1.0
//!
//! @brief      GOS2022 Library / Fault Reporting Service header.
//! @details    This component TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-24    Ahmed Gazar     Initial version created.
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
#ifndef SVL_FRS_H
#define SVL_FRS_H
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
#define SVL_FRS_FAULT_DATA_SIZE  ( 64u )

#define SVL_FRS_ENTRY_NUM_ADDR   ( 0u  )

#define SVL_FRS_ENTRY_START_ADDR ( SVL_FRS_ENTRY_NUM_ADDR + sizeof(u32_t) )

#define SVL_FRS_ENTRY_SIZE       ( sizeof(svl_frsFault_t) )

/*
 * Type definitions
 */
/**
 * TODO
 */
typedef gos_result_t (*svl_frsReadWriteFunc_t)(u32_t address, u8_t* pData, u16_t size);

/**
 * TODO
 */
typedef struct
{
	u32_t      trigger;
	u32_t      source;
	gos_time_t timeStamp;
	u8_t       data [SVL_FRS_FAULT_DATA_SIZE];
}svl_frsFault_t;

/**
 * TODO
*/
typedef struct
{
	svl_frsReadWriteFunc_t readFunction;
	svl_frsReadWriteFunc_t writeFunction;
}svl_frsInitStruct_t;

// TODO
gos_result_t svl_frsInit (svl_frsInitStruct_t* pInitStruct);

gos_result_t svl_frsCreate (svl_frsFault_t* pFault);

gos_result_t svl_frsGetNumOfEntries (u32_t* pNumOfEntries);

gos_result_t svl_frsGetFaultByIndex (u16_t index, svl_frsFault_t *pFault);

gos_result_t svl_frsGetAllFaults (svl_frsFault_t* pFaultArray, u16_t arraySize);

gos_result_t svl_frsClearFaults (void_t);

#endif
