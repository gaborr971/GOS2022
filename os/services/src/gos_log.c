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
//! @file		gos_log.c
//! @author		Gabor Repasi
//! @date		2022-11-14
//! @version	1.1
//!
//! @brief		GOS log service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_log.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-24	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Formatted log function implemented
//*************************************************************************************************
/*
 * Includes
 */
#include <stdarg.h>
#include <stdio.h>
#include "gos_log.h"
#include "gos_lock.h"
#include "gos_uart.h"

/*
 * Static variables
 */
/**
 * Log lock.
 */
GOS_STATIC gos_lock_t logLock;

/*
 * Function: gos_logInit
 */
gos_result_t gos_logInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t logInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (gos_lockCreate(&logLock) == GOS_SUCCESS &&
		gos_uartInit(CFG_LOG_PORT) == GOS_SUCCESS)
	{
		logInitResult = GOS_SUCCESS;
	}

	return logInitResult;
}

/*
 * Function: gos_logLog
 */
GOS_INLINE gos_result_t gos_logLog (char_t* logMessage)
{
	/*
	 * Local variables.
	 */
	gos_result_t logLogResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (gos_lockWaitGet(logLock) 					== GOS_SUCCESS &&
		gos_uartTransmit(CFG_LOG_PORT, logMessage)	== GOS_SUCCESS)
	{
		logLogResult = GOS_SUCCESS;
	}
	(void_t)gos_lockRelease(logLock);

	return logLogResult;
}

/*
 * Function: gos_logLogFormatted
 */
gos_result_t gos_logLogFormatted (const char_t* logFormat, ...)
{
	/*
	 * Local variables.
	 */
	gos_result_t logLogResult = GOS_ERROR;
	static char_t formattedMessage [CFG_LOG_MAX_LENGTH];
	va_list args;

	/*
	 * Function code.
	 */
	va_start(args, logFormat);
	vsprintf(formattedMessage, logFormat, args);
	va_end(args);

	if (gos_lockWaitGet(logLock) 							== GOS_SUCCESS &&
		gos_uartTransmit(CFG_LOG_PORT, formattedMessage)	== GOS_SUCCESS)
	{
		logLogResult = GOS_SUCCESS;
	}
	(void_t)gos_lockRelease(logLock);


	return logLogResult;
}

