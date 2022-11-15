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
//! @date		2022-11-14
//! @version	1.1
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
