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
//! @file		gos_tmr.c
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.1
//!
//! @brief		GOS timer driver source.
//! @details	For a more detailed description of this service, please refer to @ref gos_tmr.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-11-10	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	License added
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
#include "gos_tmr.h"

/*
 * Static variables
 */
/**
 * Driver function configuration.
 */
GOS_STATIC gos_timerFunctions_t timerDriverFunctions =
{
	.sysTimerInitFunction	= NULL,
	.sysTimerGetFunction	= NULL
};

/*
 * Function: gos_timerDriverSetup
 */
gos_result_t gos_timerDriverSetup (gos_timerFunctions_t* timerFunctions)
{
	/*
	 * Local variables.
	 */
	gos_result_t timerDriverSetupResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (timerFunctions->sysTimerInitFunction != NULL &&
		timerFunctions->sysTimerGetFunction != NULL)
	{
		timerDriverFunctions.sysTimerInitFunction = timerFunctions->sysTimerInitFunction;
		timerDriverFunctions.sysTimerGetFunction = timerFunctions->sysTimerGetFunction;
		timerDriverSetupResult = GOS_SUCCESS;
	}

	return timerDriverSetupResult;
}

/*
 * Function: gos_timerSysTimerInit
 */
gos_result_t gos_timerSysTimerInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t timerSysTimerInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (timerDriverFunctions.sysTimerInitFunction != NULL)
	{
		timerSysTimerInitResult = timerDriverFunctions.sysTimerInitFunction();
	}

	return timerSysTimerInitResult;
}

/*
 * Function: gos_timerSysTimerGet
 */
u32_t gos_timerSysTimerGet (void_t)
{
	/*
	 * Local variables.
	 */
	u32_t timerSysTimerValue = 0u;

	/*
	 * Function code.
	 */
	if (timerDriverFunctions.sysTimerGetFunction != NULL)
	{
		timerSysTimerValue = timerDriverFunctions.sysTimerGetFunction();
	}

	return timerSysTimerValue;
}
