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
//! @file		gos.c
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS source.
//! @details	For a more detailed description of this service, please refer to @ref gos.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-31	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Init error flag setting modified.
// 1.2		2022-11-15	Gabor Repasi	+	License added
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
#include "gos.h"

/*
 * Static variables
 */
GOS_STATIC bool_t initError;

/*
 * Function: gos_Init
 */
gos_result_t gos_Init (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t initStatus = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	// Initialize drivers and services.
	if (
		gos_timerSysTimerInit() != GOS_SUCCESS	||
		gos_kernelInit()		!= GOS_SUCCESS	||
	    gos_lockInit()			!= GOS_SUCCESS	||
	    gos_queueInit() 		!= GOS_SUCCESS	||
	    gos_logInit() 			!= GOS_SUCCESS	||
		gos_signalInit() 		!= GOS_SUCCESS	||
#if CFG_PROC_USE_SERVICE == 1
		gos_procInit() 			!= GOS_SUCCESS	||
#endif
		gos_timeInit() 			!= GOS_SUCCESS	||
#if CFG_SHELL_USE_SERVICE == 1
		gos_shellInit()			!= GOS_SUCCESS	||
#endif
		gos_messageInit() 		!= GOS_SUCCESS
	)
	{
		initStatus = GOS_ERROR;
		initError = GOS_TRUE;
	}
	else
	{
		initError = GOS_FALSE;
	}

    return initStatus;
}

/*
 * Function: gos_Start
 */
gos_result_t gos_Start (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t startStatus = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (initError == GOS_FALSE)
	{
		startStatus = gos_kernelStart();
	}

	return startStatus;
}
