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
//! @file       gos_signal.c
//! @author     Gabor Repasi
//! @date       2023-06-17
//! @version    1.5
//!
//! @brief      GOS signal service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_signal.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-23    Gabor Repasi    Initial version created
// 1.1        2022-11-15    Gabor Repasi    +    License added
// 1.2        2022-12-04    Gabor Repasi    +    Kernel dump ready signal handler added
// 1.3        2022-12-15    Gabor Repasi    +    Initialization error logging added
//                                          +    Privilege handling added
// 1.4        2023-05-04    Gabor Repasi    -    Lock dump signal removed
// 1.5        2023-06-17    Ahmed Gazar     *    Service rework, optimizations, queue use removed,
//                                               unnecessary system signals removed
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
#include <gos_signal.h>
#include <gos_error.h>
#include <gos_queue.h>
#include <string.h>

/*
 * Macros
 */
/**
 * Signal daemon poll time in [ms].
 */
#define GOS_SIGNAL_DAEMON_POLL_TIME_MS    ( 50u )

/*
 * Type definitions
 */
/**
 * Signal descriptor type.
 */
typedef struct
{
    bool_t               inUse;                                 //!< Flag to indicate whether the signal is in use.
    gos_signalHandler_t  handlers [CFG_SIGNAL_MAX_SUBSCRIBERS]; //!< Signal handler array.
    bool_t               invokeRequired;                        //!< Invoke required flag.
    gos_signalSenderId_t senderId;                              //!< Sender ID.
}gos_signalDescriptor_t;

/**
 * Signal invoke descriptor type.
 */
typedef struct
{
    gos_signalId_t       signalId; //!< Signal ID.
    gos_signalSenderId_t senderId; //!< Sender ID.
}gos_signalInvokeDescriptor;

/*
 * Static variables
 */
/**
 * Internal signal descriptor array.
 */
GOS_STATIC gos_signalDescriptor_t signalArray [CFG_SIGNAL_MAX_NUMBER];

/**
 * Signal daemon task ID.
 */
GOS_STATIC gos_tid_t      signalDaemonTaskId;

/*
 * External variables
 */
GOS_EXTERN gos_signalId_t kernelTaskDeleteSignal;

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_signalDaemonTask (void_t);

/**
 * Signal daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t signalDaemonTaskDescriptor =
{
    .taskFunction        = gos_signalDaemonTask,
    .taskName            = "gos_signal_daemon",
    .taskStackSize       = CFG_TASK_SIGNAL_DAEMON_STACK,
    .taskPriority        = CFG_TASK_SIGNAL_DAEMON_PRIO,
    .taskPrivilegeLevel  = GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_signalInit
 */
gos_result_t gos_signalInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t      signalInitResult = GOS_SUCCESS;
    gos_signalIndex_t signalIndex      = 0u;

    /*
     * Function code.
     */
    // Reset the in-use flag for every signal instance.
    for (signalIndex = 0u; signalIndex < CFG_SIGNAL_MAX_NUMBER; signalIndex++)
    {
        signalArray[signalIndex].inUse = GOS_FALSE;
    }

    // Register signal daemon and create kernel task delete signal.
    if (gos_kernelTaskRegister(&signalDaemonTaskDescriptor, &signalDaemonTaskId) != GOS_SUCCESS ||
        gos_signalCreate(&kernelTaskDeleteSignal) != GOS_SUCCESS
    )
    {
        signalInitResult = GOS_ERROR;
    }

    return signalInitResult;
}

/*
 * Function: gos_signalCreate
 */
gos_result_t gos_signalCreate (gos_signalId_t* pSignal)
{
    /*
     * Local variables.
     */
    gos_result_t      signalCreateResult = GOS_ERROR;
    gos_signalIndex_t signalIndex        = 0u;

    /*
     * Function code.
     */
    // Find the next unused signal and reserve it.
    for (signalIndex = 0u; signalIndex < CFG_SIGNAL_MAX_NUMBER; signalIndex++)
    {
        if (signalArray[signalIndex].inUse == GOS_FALSE)
        {
            if (pSignal != NULL)
            {
                *pSignal = signalIndex;
                signalArray[signalIndex].inUse = GOS_TRUE;
                signalArray[signalIndex].invokeRequired = GOS_FALSE;
                signalCreateResult = GOS_SUCCESS;
                break;
            }
        }
    }

    return signalCreateResult;
}

/*
 * Function: gos_signalSubscribe
 */
gos_result_t gos_signalSubscribe (gos_signalId_t signalId, gos_signalHandler_t signalHandler)
{
    /*
     * Local variables.
     */
    gos_result_t             signalSubscribeResult = GOS_ERROR;
    gos_signalHandlerIndex_t signalHandlerIndex    = 0u;

    /*
     * Function code.
     */
    if (signalId < CFG_SIGNAL_MAX_NUMBER && signalArray[signalId].inUse == GOS_TRUE && signalHandler != NULL)
    {
        for (signalHandlerIndex = 0u; signalHandlerIndex < CFG_SIGNAL_MAX_SUBSCRIBERS; signalHandlerIndex++)
        {
            if (signalArray[signalId].handlers[signalHandlerIndex] == NULL)
            {
                signalArray[signalId].handlers[signalHandlerIndex] = signalHandler;
                signalSubscribeResult = GOS_SUCCESS;
                break;
            }
        }
    }

    return signalSubscribeResult;
}

/*
 * Function: gos_signalInvoke
 */
GOS_INLINE gos_result_t gos_signalInvoke (gos_signalId_t signalId, gos_signalSenderId_t senderId)
{
    /*
     * Local variables.
     */
    gos_result_t               signalInvokeResult     = GOS_ERROR;
    gos_tid_t                  callerTaskId           = GOS_INVALID_TASK_ID;
    gos_taskDescriptor_t       callerTaskDesc         = {0};
    bool_t                     isCallerIsr            = GOS_FALSE;
    bool_t                     isCallerPrivileged     = GOS_FALSE;

    /*
     * Function code.
     */
    GOS_IS_CALLER_ISR(isCallerIsr);
    GOS_IS_ACCESS_PRIVILEGED(isCallerPrivileged);

    if (signalId < CFG_SIGNAL_MAX_NUMBER && signalArray[signalId].inUse == GOS_TRUE)
    {
        if ((isCallerIsr == GOS_TRUE || isCallerPrivileged == GOS_TRUE ||
            (gos_kernelTaskGetCurrentId(&callerTaskId) == GOS_SUCCESS &&
            gos_kernelTaskGetData(callerTaskId, &callerTaskDesc) == GOS_SUCCESS &&
            (callerTaskDesc.taskPrivilegeLevel & GOS_PRIV_SIGNALING) == GOS_PRIV_SIGNALING)))
        {
            GOS_UNPRIVILEGED_ACCESS
            signalArray[signalId].senderId       = senderId;
            signalArray[signalId].invokeRequired = GOS_TRUE;
            signalInvokeResult = GOS_SUCCESS;
        }
        else
        {
            gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "<%s> has no privilege to invoke signals!",
                    callerTaskDesc.taskName
            );
        }
    }
    GOS_UNPRIVILEGED_ACCESS

    return signalInvokeResult;
}

/**
 * @brief   Signal daemon task.
 * @details Polls the signal invoke queue, and completes the necessary signal invokings.
 *
 * @return -
 */
GOS_STATIC void_t gos_signalDaemonTask (void_t)
{
    /*
     * Local variables.
     */
    gos_signalHandlerIndex_t signalHandlerIndex = 0u;
    gos_signalIndex_t        signalIndex        = 0u;

    /*
     * Function code.
     */
    for (;;)
    {
        for (signalIndex = 0u; signalIndex < CFG_SIGNAL_MAX_NUMBER; signalIndex++)
        {
            if (signalArray[signalIndex].invokeRequired == GOS_TRUE)
            {
                for (signalHandlerIndex = 0u; signalHandlerIndex < CFG_SIGNAL_MAX_SUBSCRIBERS; signalHandlerIndex++)
                {
                    if (signalArray[signalIndex].handlers[signalHandlerIndex] == NULL)
                    {
                        // Last handler called, stop calling.
                        break;
                    }
                    else
                    {
                        // Call signal handler.
                        signalArray[signalIndex].handlers[signalHandlerIndex](signalArray[signalIndex].senderId);
                    }
                }
                signalArray[signalIndex].invokeRequired = GOS_FALSE;
            }
        }

        (void_t) gos_kernelTaskSleep(GOS_SIGNAL_DAEMON_POLL_TIME_MS);
    }
}
