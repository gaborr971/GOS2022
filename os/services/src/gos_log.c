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
//! @date		2022-11-15
//! @version	1.2
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

