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
//! @date		2022-11-10
//! @version	1.0
//!
//! @brief		GOS timer driver source.
//! @details	For a more detailed description of this service, please refer to @ref gos_tmr.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-11-10	Gabor Repasi	Initial version created.
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
