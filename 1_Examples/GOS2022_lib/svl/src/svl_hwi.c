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
//! @file       svl_hwi.c
//! @author     Ahmed Gazar
//! @date       2024-04-27
//! @version    1.0
//!
//! @brief      GOS2022 Library / Hardware Info source.
//! @details    For a more detailed description of this driver, please refer to @ref svl_hwi.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-27    Ahmed Gazar     Initial version created.
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
/*
 * Includes
 */
#include <svl_hwi.h>

/*
 * Static variables
 */
/**
 * Read function pointer.
 */
GOS_STATIC svl_hwiReadWriteFunc_t hwiReadFunction = NULL;

/**
 * Write function pointer.
 */
GOS_STATIC svl_hwiReadWriteFunc_t hwiWriteFunction = NULL;

/*
 * Function: svl_hwiInit
 */
gos_result_t svl_hwiInit (svl_hwiInitStruct_t* pInitStruct)
{
	/*
	 * Local variables.
	 */
	gos_result_t hwiInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pInitStruct != NULL && pInitStruct->readFunction != NULL && pInitStruct->writeFunction != NULL)
	{
		hwiReadFunction  = pInitStruct->readFunction;
		hwiWriteFunction = pInitStruct->writeFunction;
		hwiInitResult    = GOS_SUCCESS;
	}
	else
	{
		// NULL parameter.
	}

	return hwiInitResult;
}

/*
 * Function: svl_hwiRead
 */
gos_result_t svl_hwiRead (svl_hwiInfo_t* pHwInfo)
{
	/*
	 * Local variables.
	 */
	gos_result_t hwiReadResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pHwInfo != NULL && hwiReadFunction != NULL)
	{
		hwiReadResult = hwiReadFunction(
				SVL_HWI_ENTRY_START_ADDR,
				(u8_t*)pHwInfo,
				SVL_HWI_ENTRY_SIZE
				);
	}
	else
	{
		// Struct or read function is NULL.
	}

	return hwiReadResult;
}

/*
 * Function: svl_hwiWrite
 */
gos_result_t svl_hwiWrite (svl_hwiInfo_t* pHwInfo)
{
	/*
	 * Local variables.
	 */
	gos_result_t hwiWriteResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pHwInfo != NULL && hwiWriteFunction != NULL)
	{
		hwiWriteResult = hwiWriteFunction(
				SVL_HWI_ENTRY_START_ADDR,
				(u8_t*)pHwInfo,
				SVL_HWI_ENTRY_SIZE
				);
	}
	else
	{
		// Struct or read function is NULL.
	}

	return hwiWriteResult;
}
