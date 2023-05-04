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
//! @file       gos_message.c
//! @author     Gabor Repasi
//! @date       2023-05-04
//! @version    1.6
//!
//! @brief      GOS message service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_message.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-27    Gabor Repasi    Initial version created
// 1.1        2022-11-05    Gabor Repasi    +    Lock introduced to protect internal arrays as
//                                               shared resources
//                                          +    Simple array handling replaced with FIFO
//                                               implementation
//                                          *    Buffer emptying is not FIFO yet in case there was
//                                               a buffer overflow!
// 1.2        2022-11-15    Gabor Repasi    +    License added
// 1.3        2022-12-13    Gabor Repasi    +    Privilege handling added
//                                          +    Initialization error logging added
// 1.4        2022-12-15    Gabor Repasi    *    Waiter array handling bugfix
// 1.5        2023-01-11    Gabor Repasi    -    Unnecessary includes removed
//                                               Initialization error logging removed
// 1.6        2023-05-04    Gabor Repasi    *    Lock calls replaced with mutex calls
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
#include <gos_message.h>
#include <gos_mutex.h>
#include <string.h>

/*
 * Macros
 */
/**
 * Message daemon poll time in [ms].
 */
#define GOS_MESSAGE_DAEMON_POLL_TIME_MS    ( 150u )

/*
 * Type definitions
 */
/**
 * Message waiter descriptor type.
 */
typedef struct
{
    gos_tid_t            waiterTaskId;                                //!< Waiter task ID.
    gos_messageTimeout_t waitTmo;                                     //!< Wait timeout value.
    gos_messageTimeout_t waitTmoCounter;                              //!< Wait timeout counter.
    gos_messageId_t      messageIdArray [CFG_MESSAGE_MAX_WAITER_IDS]; //!< Message ID array.
    gos_message_t*       target;                                      //!< Target buffer.
}gos_messageWaiterDesc_t;

/*
 * Static variables
 */
/**
 * Internal message array.
 */
GOS_STATIC gos_message_t            messageArray       [CFG_MESSAGE_MAX_NUMBER];

/**
 * Internal message waiter array.
 */
GOS_STATIC gos_messageWaiterDesc_t  messageWaiterArray [CFG_MESSAGE_MAX_WAITERS];

/**
 * Message daemon task ID.
 */
GOS_STATIC gos_tid_t                messageDaemonTaskId;

/**
 * Index of the next message slot (for circular buffer).
 */
GOS_STATIC gos_messageIndex_t       nextMessageIndex;

/**
 * Index of the next waiter slot (for circular buffer).
 */
GOS_STATIC gos_messageWaiterIndex_t nextWaiterIndex;

/**
 * Message mutex to protect the internal arrays as shared resources.
 */
GOS_STATIC gos_mutex_t              messageMutex;

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_messageDaemonTask (void_t);

/**
 * Message daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t messageDaemonTaskDesc =
{
    .taskFunction        = gos_messageDaemonTask,
    .taskName            = "gos_message_daemon",
    .taskPriority        = CFG_TASK_MESSAGE_DAEMON_PRIO,
    .taskStackSize       = CFG_TASK_MESSAGE_DAEMON_STACK,
    .taskPrivilegeLevel  = GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_messageInit
 */
gos_result_t gos_messageInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t             messageInitResult  = GOS_SUCCESS;
    gos_messageIndex_t       messageIndex       = 0u;
    gos_messageWaiterIndex_t messageWaiterIndex = 0u;

    /*
     * Function code.
     */
    nextMessageIndex = 0u;
    nextWaiterIndex  = 0u;

    for (messageIndex = 0u; messageIndex < CFG_MESSAGE_MAX_NUMBER; messageIndex++)
    {
        messageArray[messageIndex].messageId   = GOS_MESSAGE_INVALID_ID;
        messageArray[messageIndex].messageSize = 0u;
    }

    for (messageWaiterIndex = 0u; messageWaiterIndex < CFG_MESSAGE_MAX_WAITERS; messageWaiterIndex++)
    {
        messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
    }

    // Initialize message mutex.
    gos_mutexInit(&messageMutex);

    if (gos_kernelTaskRegister(&messageDaemonTaskDesc, &messageDaemonTaskId) != GOS_SUCCESS)
    {
        messageInitResult = GOS_ERROR;
    }

    return messageInitResult;
}

/*
 * Function: gos_messageRx
 */
GOS_INLINE gos_result_t gos_messageRx (gos_messageId_t* messageIdArray, gos_message_t* target, gos_messageTimeout_t tmo)
{
    /*
     * Local variables.
     */
    gos_result_t             messageRxResult    = GOS_ERROR;
    gos_tid_t                currentTaskId      = GOS_INVALID_TASK_ID;
    gos_messageWaiterIndex_t messageWaiterIndex = 0u;
    gos_messageWaiterIndex_t index              = 0u;
    gos_messageIdIndex_t     messageIdIndex     = 0u;

    /*
     * Function code.
     */
    if (target != NULL && messageIdArray != NULL &&
    	gos_mutexLock(&messageMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
    {
        if (messageWaiterArray[nextWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID &&
            gos_kernelTaskGetCurrentId(&currentTaskId) == GOS_SUCCESS)
        {
            // Add waiter to array.
            messageWaiterArray[nextWaiterIndex].waiterTaskId   = currentTaskId;
            messageWaiterArray[nextWaiterIndex].waitTmo        = tmo;
            messageWaiterArray[nextWaiterIndex].waitTmoCounter = 0u;
            messageWaiterArray[nextWaiterIndex].target         = target;

            memset(messageWaiterArray[nextWaiterIndex].messageIdArray, 0, CFG_MESSAGE_MAX_WAITER_IDS * sizeof(gos_messageId_t));

            for (messageIdIndex = 0u; messageIdIndex < CFG_MESSAGE_MAX_WAITER_IDS; messageIdIndex++)
            {
                if (messageIdArray[messageIdIndex] == 0u)
                {
                    break;
                }
                messageWaiterArray[nextWaiterIndex].messageIdArray[messageIdIndex] = messageIdArray[messageIdIndex];
            }

            messageWaiterIndex = nextWaiterIndex;

            for (index = 0u; index < CFG_MESSAGE_MAX_WAITERS; index++)
            {
                if (++nextWaiterIndex == CFG_MESSAGE_MAX_WAITERS)
                {
                    nextWaiterIndex = 0u;
                }

                if (messageWaiterArray[nextWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID)
                {
                    break;
                }
            }

            // Unlock message mutex.
            gos_mutexUnlock(&messageMutex);

            // Block task (to be unblocked by daemon).
            GOS_PRIVILEGED_ACCESS
            (void_t) gos_kernelTaskBlock(currentTaskId);

            // Task unblocked, check TMO.
            if ((messageWaiterArray[messageWaiterIndex].waitTmoCounter * GOS_MESSAGE_DAEMON_POLL_TIME_MS) < messageWaiterArray[messageWaiterIndex].waitTmo)
            {
                // Message received successfully.
                messageRxResult = GOS_SUCCESS;
            }
            else
            {
                // Remove waiter.
                messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
            }
        }
    }

    // Unlock message mutex.
    gos_mutexUnlock(&messageMutex);

    return messageRxResult;
}

/*
 * Function: gos_messageTx
 */
GOS_INLINE gos_result_t gos_messageTx (gos_message_t* message)
{
    /*
     * Local variables.
     */
    gos_result_t       messageTxResult = GOS_ERROR;
    gos_messageIndex_t msgIndex        = 0u;

    /**
     * Function code.
     */
    if (message != NULL &&
        message->messageId != GOS_MESSAGE_INVALID_ID &&
        message->messageSize < CFG_MESSAGE_MAX_LENGTH &&
		gos_mutexLock(&messageMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
    {
        if (messageArray[nextMessageIndex].messageId == GOS_MESSAGE_INVALID_ID)
        {
            memcpy((void_t*)&messageArray[nextMessageIndex], (void_t*)message, sizeof(*message));
            messageTxResult = GOS_SUCCESS;

            for (msgIndex = 0u; msgIndex < CFG_MESSAGE_MAX_NUMBER; msgIndex++)
            {
                if (++nextMessageIndex == CFG_MESSAGE_MAX_NUMBER)
                {
                    nextMessageIndex = 0u;
                }

                if (messageArray[nextMessageIndex].messageId == GOS_MESSAGE_INVALID_ID)
                {
                    break;
                }
            }
        }
    }

    // Unlock message mutex.
    gos_mutexUnlock(&messageMutex);

    return messageTxResult;
}

/**
 * @brief   Message daemon task.
 * @details This task is responsible for message transfer between tasks. It loops
 *          through the internal message array and the waiter task array and if a
 *          message and a waiter matches, it copies the message to the target
 *          buffer and unblocks the previously blocked task.
 *
 * @return    -
 */
GOS_STATIC void_t gos_messageDaemonTask (void_t)
{
    /*
     * Local variables.
     */
    gos_messageIndex_t       messageIndex       = 0u;
    gos_messageWaiterIndex_t messageWaiterIndex = 0u;
    gos_messageIdIndex_t     messageIdIndex     = 0u;
    bool_t                   waiterServed       = GOS_FALSE;

    /*
     * Function code.
     */
    for(;;)
    {
        if (gos_mutexLock(&messageMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
        {
            for (messageWaiterIndex = 0u; messageWaiterIndex < CFG_MESSAGE_MAX_WAITERS; messageWaiterIndex++)
            {
                if (messageWaiterArray[messageWaiterIndex].waiterTaskId != GOS_INVALID_TASK_ID)
                {
                    waiterServed = GOS_FALSE;
                    for (messageIdIndex = 0u; messageIdIndex < CFG_MESSAGE_MAX_WAITER_IDS; messageIdIndex++)
                    {
                    	for (messageIndex = 0u; messageIndex < CFG_MESSAGE_MAX_NUMBER; messageIndex++)
                    	{
                            if (messageWaiterArray[messageWaiterIndex].messageIdArray[messageIdIndex] ==
                                messageArray[messageIndex].messageId)
                            {
                                memcpy(messageWaiterArray[messageWaiterIndex].target->messageBytes,
                                      (void_t*)messageArray[messageIndex].messageBytes,
                                      messageArray[messageIndex].messageSize);
                                messageWaiterArray[messageWaiterIndex].target->messageSize    = messageArray[messageIndex].messageSize;
                                messageWaiterArray[messageWaiterIndex].target->messageId    = messageArray[messageIndex].messageId;

                                (void_t) gos_kernelTaskUnblock(messageWaiterArray[messageWaiterIndex].waiterTaskId);

                                messageArray[messageIndex].messageId = GOS_MESSAGE_INVALID_ID;
                                messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
                                waiterServed = GOS_TRUE;

                                break;
                            }
                    	}
                    }

                    if (waiterServed == GOS_FALSE && messageWaiterArray[messageWaiterIndex].waitTmo != GOS_MESSAGE_ENDLESS_TMO)
                    {
                        messageWaiterArray[messageWaiterIndex].waitTmoCounter++;

                        if ((messageWaiterArray[messageWaiterIndex].waitTmoCounter * GOS_MESSAGE_DAEMON_POLL_TIME_MS)  > messageWaiterArray[messageWaiterIndex].waitTmo)
                        {
                            // Timeout. Delete waiter, unblock task.
                            (void_t) gos_kernelTaskUnblock(messageWaiterArray[messageWaiterIndex].waiterTaskId);
                            messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
                        }
                    }
                }
            }

            // Unlock message mutex.
            gos_mutexUnlock(&messageMutex);
        }

        (void_t) gos_kernelTaskSleep(GOS_MESSAGE_DAEMON_POLL_TIME_MS);
    }
}
