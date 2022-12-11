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
//! @date		2022-12-11
//! @version	1.4
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
// 1.3		2022-12-04	Gabor Repasi	+	Log daemon task added
// 1.4		2022-12-11	Gabor Repasi	-	Log lock removed
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
#include <string.h>
#include "gos_log.h"
#include "gos_lock.h"
#include "gos_queue.h"
#include "gos_log_driver.h"

/*
 * Static variables
 */
/**
 * Log queue.
 */
GOS_STATIC gos_queueDescriptor_t logQueue =
{
	.queueName = "gos_log_queue"
};

/**
 * Log line buffer.
 */
GOS_STATIC char_t logLine [CFG_LOG_MAX_LENGTH];

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_logDaemonTask (void_t);

/**
 * Message daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t logDaemonTaskDesc =
{
	.taskFunction	= gos_logDaemonTask,
	.taskName		= "gos_log_daemon",
	.taskPriority	= CFG_TASK_LOG_DAEMON_PRIO,
	.taskStackSize	= CFG_TASK_LOG_DAEMON_STACK
};

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
	if (gos_queueCreate(&logQueue) == GOS_SUCCESS &&
		gos_kernelTaskRegister(&logDaemonTaskDesc, NULL) == GOS_SUCCESS)
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
	logLogResult = gos_queuePut(logQueue.queueId, (void_t*)logMessage, strlen(logMessage) + 1);

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
	GOS_STATIC char_t formattedMessage [CFG_LOG_MAX_LENGTH];
	va_list args;

	/*
	 * Function code.
	 */
	va_start(args, logFormat);
	vsprintf(formattedMessage, logFormat, args);
	va_end(args);

	logLogResult = gos_queuePut(logQueue.queueId, (void_t*)formattedMessage, strlen(formattedMessage) + 1);

	return logLogResult;
}

/*
 * Function: gos_logLogFormattedUnsafe
 */
gos_result_t gos_logLogFormattedUnsafe (const char_t* logFormat, ...)
{
	/*
	 * Local variables.
	 */
	gos_result_t logLogResult = GOS_ERROR;
	GOS_STATIC char_t formattedMessage [CFG_LOG_MAX_LENGTH];
	va_list args;

	/*
	 * Function code.
	 */
	GOS_ATOMIC_ENTER
	va_start(args, logFormat);
	vsprintf(formattedMessage, logFormat, args);
	va_end(args);

	logLogResult = gos_logDriverTransmitString(formattedMessage);

	gos_kernelDelayMs(10);
	GOS_ATOMIC_EXIT

	return logLogResult;
}

GOS_STATIC void_t gos_logDaemonTask (void_t)
{
	/*
	 * Function code.
	 */
	for(;;)
	{
		while (gos_queueGet(logQueue.queueId, (void_t*)logLine, sizeof(logLine)) == GOS_SUCCESS)
		{
			gos_logDriverTransmitString(logLine);
		}
		gos_kernelTaskSleep(20);
	}
}
