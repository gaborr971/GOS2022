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
//                                      (c) Gabor Repasi, 2022
//
//*************************************************************************************************
//! @file		gos_log_driver.c
//! @author		Gabor Repasi
//! @date		2022-12-11
//! @version	1.0
//!
//! @brief		GOS LOG driver source.
//! @details	For a more detailed description of this driver, please refer to
//! 			@ref gos_log_driver.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-12-11	Gabor Repasi	Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2022 Gabor Repasi
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
#include "gos_log_driver.h"
#include "gos_driver.h"

/*
 * External variables
 */
GOS_EXTERN gos_driver_functions_t driverFunctions;

/*
 * Function: gos_logDriverTransmitString
 */
gos_result_t gos_logDriverTransmitString (char_t* pString)
{
	/*
	 * Local variables.
	 */
	gos_result_t logDriverTransmitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (driverFunctions.logDriverTransmitString != NULL)
	{
		logDriverTransmitResult = driverFunctions.logDriverTransmitString(pString);
	}

	return logDriverTransmitResult;
}
