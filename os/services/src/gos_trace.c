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
//! @file       gos_trace.c
//! @author     Gabor Repasi
//! @date       2023-01-13
//! @version    1.6
//!
//! @brief      GOS trace service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_trace.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-24    Gabor Repasi    Initial version created
// 1.1        2022-11-14    Gabor Repasi    +    Formatted log function implemented
// 1.2        2022-11-15    Gabor Repasi    +    License added
// 1.3        2022-12-04    Gabor Repasi    +    Log daemon task added
// 1.4        2022-12-11    Gabor Repasi    -    Log lock removed
// 1.5        2022-12-13    Gabor Repasi    +    Privilege handling added
//                                          +    Initialization error logging added
// 1.6        2023-01-13    Gabor Repasi    *    Service renamed to trace
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
#include <gos_trace.h>
#include <gos_error.h>
#include <gos_queue.h>
#include <gos_trace_driver.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/*
 * Macros
 */
/**
 * Trace daemon poll time [ms].
 */
#define GOS_TRACE_DAEMON_POLL_TIME_MS    ( 20u )

/*
 * Static variables
 */
/**
 * Trace queue.
 */
GOS_STATIC gos_queueDescriptor_t traceQueue =
{
    .queueName = "gos_trace_queue"
};

/**
 * Trace line buffer.
 */
GOS_STATIC char_t traceLine       [CFG_TRACE_MAX_LENGTH];

/**
 * Trace formatted buffer for message formatting.
 */
GOS_STATIC char_t formattedBuffer [CFG_TRACE_MAX_LENGTH];

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_traceDaemonTask (void_t);

/**
 * Trace daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t traceDaemonTaskDesc =
{
    .taskFunction        = gos_traceDaemonTask,
    .taskName            = "gos_trace_daemon",
    .taskPriority        = CFG_TASK_TRACE_DAEMON_PRIO,
    .taskStackSize       = CFG_TASK_TRACE_DAEMON_STACK,
    .taskPrivilegeLevel  = GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_traceInit
 */
gos_result_t gos_traceInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t traceInitResult = GOS_SUCCESS;

    /*
     * Function code.
     */
    if (gos_queueCreate(&traceQueue) != GOS_SUCCESS ||
    	gos_kernelTaskRegister(&traceDaemonTaskDesc, NULL) != GOS_SUCCESS)
    {
    	traceInitResult = GOS_ERROR;
    }

    return traceInitResult;
}

/*
 * Function: gos_traceTrace
 */
GOS_INLINE gos_result_t gos_traceTrace (char_t* traceMessage)
{
    /*
     * Local variables.
     */
    gos_result_t         traceResult         = GOS_ERROR;
    gos_tid_t            callerTaskId        = GOS_INVALID_TASK_ID;
    gos_taskDescriptor_t callerTaskDesc      = {0};
    bool_t               isCallerIsr         = GOS_FALSE;
    bool_t               isCallerPrivileged  = GOS_FALSE;

    /*
     * Function code.
     */
    GOS_DISABLE_SCHED
    GOS_IS_CALLER_ISR(isCallerIsr);
    GOS_IS_ACCESS_PRIVILEGED(isCallerPrivileged);

    if (traceMessage != NULL &&
        (isCallerIsr == GOS_TRUE || isCallerPrivileged == GOS_TRUE ||
        (gos_kernelTaskGetCurrentId(&callerTaskId) == GOS_SUCCESS &&
        gos_kernelTaskGetData(callerTaskId, &callerTaskDesc) == GOS_SUCCESS &&
        (callerTaskDesc.taskPrivilegeLevel & GOS_PRIV_TRACE) == GOS_PRIV_TRACE)) &&
        gos_queuePut(traceQueue.queueId, (void_t*)traceMessage, strlen(traceMessage) + 1) == GOS_SUCCESS
        )
    {
        GOS_UNPRIVILEGED_ACCESS
        traceResult = GOS_SUCCESS;

        GOS_PRIVILEGED_ACCESS
		(void_t) gos_kernelTaskResume(traceDaemonTaskDesc.taskId);
    }
    GOS_ENABLE_SCHED

    return traceResult;
}

/*
 * Function: gos_traceTraceFormatted
 */
gos_result_t gos_traceTraceFormatted (const char_t* traceFormat, ...)
{
	GOS_DISABLE_SCHED
    /*
     * Local variables.
     */
    gos_result_t         traceResult        = GOS_ERROR;
    va_list              args;
    gos_tid_t            callerTaskId       = GOS_INVALID_TASK_ID;
    gos_taskDescriptor_t callerTaskDesc     = {0};
    bool_t               isCallerIsr        = GOS_FALSE;
    bool_t               isCallerPrivileged = GOS_FALSE;

    /*
     * Function code.
     */
    GOS_IS_CALLER_ISR(isCallerIsr);
    GOS_IS_ACCESS_PRIVILEGED(isCallerPrivileged);

    if (traceFormat != NULL)
    {
        va_start(args, traceFormat);
        (void_t) vsprintf(formattedBuffer, traceFormat, args);
        va_end(args);

        if ((isCallerIsr == GOS_TRUE || isCallerPrivileged == GOS_TRUE ||
            (gos_kernelTaskGetCurrentId(&callerTaskId) == GOS_SUCCESS &&
            gos_kernelTaskGetData(callerTaskId, &callerTaskDesc) == GOS_SUCCESS &&
            (callerTaskDesc.taskPrivilegeLevel & GOS_PRIV_TRACE) == GOS_PRIV_TRACE)) &&
            gos_queuePut(traceQueue.queueId, (void_t*)formattedBuffer, strlen(formattedBuffer) + 1) == GOS_SUCCESS
            )
        {
            GOS_UNPRIVILEGED_ACCESS
            traceResult = GOS_SUCCESS;

            GOS_PRIVILEGED_ACCESS
			(void_t) gos_kernelTaskResume(traceDaemonTaskDesc.taskId);
        }
    }
    GOS_ENABLE_SCHED

    return traceResult;
}

/*
 * Function: gos_traceTraceFormattedUnsafe
 */
gos_result_t gos_traceTraceFormattedUnsafe (const char_t* traceFormat, ...)
{
    /*
     * Local variables.
     */
    gos_result_t traceResult = GOS_ERROR;
    va_list      args;

    /*
     * Function code.
     */
    if (traceFormat != NULL)
    {
        GOS_DISABLE_SCHED
        {
            va_start(args, traceFormat);
            (void_t) vsprintf(formattedBuffer, traceFormat, args);
            va_end(args);

            traceResult = gos_traceDriverTransmitString_Unsafe(formattedBuffer);
        }
        GOS_ENABLE_SCHED
    }

    return traceResult;
}

/**
 * @brief   Trace daemon task.
 * @details Polls the trace queue and transmits the elements in the trace queue
 *          via the registered trace driver.
 *
 * @return  -
 */
GOS_STATIC void_t gos_traceDaemonTask (void_t)
{
    /*
     * Function code.
     */
    for(;;)
    {
        while (gos_queueGet(traceQueue.queueId, (void_t*)traceLine, sizeof(traceLine)) == GOS_SUCCESS)
        {
            (void_t) gos_traceDriverTransmitString(traceLine);
        }
        (void_t) gos_kernelTaskSuspend(traceDaemonTaskDesc.taskId);
    }
}
