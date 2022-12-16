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
//! @date		2022-12-13
//! @version	1.5
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
// 1.5		2022-12-13	Gabor Repasi	+	Privilege handling added
//										+	Initialization error logging added
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
#include "gos_error.h"
#include "gos_log.h"
#include "gos_lock.h"
#include "gos_queue.h"
#include "gos_log_driver.h"

/*
 * Macros
 */
/**
 * Log daemon poll time [ms].
 */
#define GOS_LOG_DAEMON_POLL_TIME_MS	( 20u )

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

/**
 * Log formatted buffer for message formatting.
 */
GOS_STATIC char_t formattedBuffer [CFG_LOG_MAX_LENGTH];

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_logDaemonTask (void_t);

/**
 * Message daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t logDaemonTaskDesc =
{
	.taskFunction		= gos_logDaemonTask,
	.taskName			= "gos_log_daemon",
	.taskPriority		= CFG_TASK_LOG_DAEMON_PRIO,
	.taskStackSize		= CFG_TASK_LOG_DAEMON_STACK,
	.taskPrivilegeLevel	= GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_logInit
 */
gos_result_t gos_logInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t logInitResult = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	if (gos_queueCreate(&logQueue) != GOS_SUCCESS)
	{
		(void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "Log queue creation error.");
		logInitResult = GOS_ERROR;
	}

	if (gos_kernelTaskRegister(&logDaemonTaskDesc, NULL) != GOS_SUCCESS)
	{
		(void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "Log daemon task registration error.");
		logInitResult = GOS_ERROR;
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
	gos_result_t			logLogResult	= GOS_ERROR;
	gos_tid_t				callerTaskId	= GOS_INVALID_TASK_ID;
	gos_taskDescriptor_t	callerTaskDesc;
	bool_t					isCallerIsr;
	bool_t					isCallerPrivileged;

	/*
	 * Function code.
	 */
	GOS_IS_CALLER_ISR(isCallerIsr);
	GOS_IS_ACCESS_PRIVILEGED(isCallerPrivileged);

	if (logMessage != NULL &&
		(isCallerIsr == GOS_TRUE || isCallerPrivileged == GOS_TRUE ||
		(gos_kernelTaskGetCurrentId(&callerTaskId) == GOS_SUCCESS &&
		gos_kernelTaskGetData(callerTaskId, &callerTaskDesc) == GOS_SUCCESS &&
		(callerTaskDesc.taskPrivilegeLevel & GOS_PRIV_LOG) == GOS_PRIV_LOG)) &&
		gos_queuePut(logQueue.queueId, (void_t*)logMessage, strlen(logMessage) + 1) == GOS_SUCCESS
		)
	{
		GOS_UNPRIVILEGED_ACCESS
		logLogResult = GOS_SUCCESS;
	}

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
	gos_result_t 			logLogResult 	= GOS_ERROR;
	va_list					args;
	gos_tid_t				callerTaskId	= GOS_INVALID_TASK_ID;
	gos_taskDescriptor_t	callerTaskDesc;
	bool_t					isCallerIsr;
	bool_t					isCallerPrivileged;

	/*
	 * Function code.
	 */
	GOS_IS_CALLER_ISR(isCallerIsr);
	GOS_IS_ACCESS_PRIVILEGED(isCallerPrivileged);

	if (logFormat != NULL)
	{
		GOS_DISABLE_SCHED
		va_start(args, logFormat);
		(void_t) vsprintf(formattedBuffer, logFormat, args);
		va_end(args);

		if ((isCallerIsr == GOS_TRUE || isCallerPrivileged == GOS_TRUE ||
			(gos_kernelTaskGetCurrentId(&callerTaskId) == GOS_SUCCESS &&
			gos_kernelTaskGetData(callerTaskId, &callerTaskDesc) == GOS_SUCCESS &&
			(callerTaskDesc.taskPrivilegeLevel & GOS_PRIV_LOG) == GOS_PRIV_LOG)) &&
			gos_queuePut(logQueue.queueId, (void_t*)formattedBuffer, strlen(formattedBuffer) + 1) == GOS_SUCCESS
			)
		{
			GOS_UNPRIVILEGED_ACCESS
			logLogResult = GOS_SUCCESS;
		}
		GOS_ENABLE_SCHED
	}


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
	va_list args;

	/*
	 * Function code.
	 */
	if (logFormat != NULL)
	{
		GOS_DISABLE_SCHED
		{
			va_start(args, logFormat);
			(void_t) vsprintf(formattedBuffer, logFormat, args);
			va_end(args);

			logLogResult = gos_logDriverTransmitString_Unsafe(formattedBuffer);
		}
		GOS_ENABLE_SCHED
	}

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
			(void_t) gos_logDriverTransmitString(logLine);
		}
		(void_t) gos_kernelTaskSleep(GOS_LOG_DAEMON_POLL_TIME_MS);
	}
}
