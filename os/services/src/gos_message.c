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
//! @file		gos_message.c
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS message service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_message.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-27	Gabor Repasi	Initial version created.
// 1.1		2022-11-05	Gabor Repasi	+	Lock introduced to protect internal arrays as shared
//											resources
//										+	Simple array handling replaced with FIFO implementation
//										*	Buffer emptying is not FIFO yet in case there was a
//											buffer overflow!
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
#include <string.h>
#include "gos_lock.h"
#include "gos_message.h"
#include "gos_queue.h"

/*
 * Macros
 */
/**
 * Message daemon poll time in [ms].
 */
#define GOS_MESSAGE_DAEMON_POLL_TIME_MS	( 10u )

/*
 * Type definitions
 */
/**
 * Message waiter descriptor type.
 */
typedef struct
{
	gos_tid_t				waiterTaskId;									//!< Waiter task ID.
	gos_messageTimeout_t	waitTmo;										//!< Wait timeout value.
	gos_messageTimeout_t	waitTmoCounter;									//!< Wait timeout counter.
	gos_messageId_t 		messageIdArray [CFG_MESSAGE_MAX_WAITER_IDS];	//!< Message ID array.
	gos_message_t*			target;											//!< Target buffer.
}gos_messageWaiterDesc_t;

/*
 * Static variables
 */
/**
 * Internal message array.
 */
GOS_STATIC gos_message_t 			messageArray 		[CFG_MESSAGE_MAX_NUMBER];

/**
 * Internal message waiter array.
 */
GOS_STATIC gos_messageWaiterDesc_t	messageWaiterArray 	[CFG_MESSAGE_MAX_WAITERS];

/**
 * Message daemon task ID.
 */
GOS_STATIC gos_tid_t				messageDaemonTaskId;

/**
 * Index of the next message slot (for circular buffer).
 */
GOS_STATIC gos_messageIndex_t		nextMessageIndex;

/**
 * Index of the next waiter slot (for circular buffer).
 */
GOS_STATIC gos_messageWaiterIndex_t	nextWaiterIndex;

/**
 * Message lock to protect the internal arrays as shared resources.
 */
GOS_STATIC gos_lock_t				messageLock;

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_messageDaemonTask (void_t);

/**
 * Message daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t messageDaemonTaskDesc =
{
	.taskFunction	= gos_messageDaemonTask,
	.taskName		= "gos_message_daemon",
	.taskPriority	= CFG_TASK_MESSAGE_DAEMON_PRIO,
	.taskStackSize	= CFG_TASK_MESSAGE_DAEMON_STACK
};

/*
 * Function: gos_messageInit
 */
gos_result_t gos_messageInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t 				messageInitResult 	= GOS_ERROR;
	gos_messageIndex_t			messageIndex		= 0u;
	gos_messageWaiterIndex_t	messageWaiterIndex 	= 0u;

	/*
	 * Function code.
	 */
	nextMessageIndex 	= 0u;
	nextWaiterIndex		= 0u;

	for (messageIndex = 0u; messageIndex < CFG_MESSAGE_MAX_NUMBER; messageIndex++)
	{
		messageArray[messageIndex].messageId 	= GOS_MESSAGE_INVALID_ID;
		messageArray[messageIndex].messageSize	= 0u;
	}

	for (messageWaiterIndex = 0u; messageWaiterIndex < CFG_MESSAGE_MAX_WAITERS; messageWaiterIndex++)
	{
		messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
	}

	if (gos_kernelTaskRegister(&messageDaemonTaskDesc, &messageDaemonTaskId) == GOS_SUCCESS &&
		gos_lockCreate(&messageLock) == GOS_SUCCESS)
	{
		messageInitResult = GOS_SUCCESS;
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
	gos_result_t				messageRxResult		= GOS_ERROR;
	gos_tid_t					currentTaskId		= GOS_INVALID_TASK_ID;
	gos_messageWaiterIndex_t	messageWaiterIndex 	= 0u;
	gos_messageIdIndex_t		messageIdIndex 		= 0u;

	/*
	 * Function code.
	 */
	if (target != NULL && messageIdArray != NULL)
	{
		if (messageWaiterArray[nextWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID)
		{
			// Get message lock.
			gos_lockWaitGet(messageLock);

			// Add waiter to array.
			gos_kernelTaskGetCurrentId(&currentTaskId);
			messageWaiterArray[nextWaiterIndex].waiterTaskId 	= currentTaskId;
			messageWaiterArray[nextWaiterIndex].waitTmo			= tmo;
			messageWaiterArray[nextWaiterIndex].waitTmoCounter	= 0u;
			messageWaiterArray[nextWaiterIndex].target			= target;

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

			if (++nextWaiterIndex == CFG_MESSAGE_MAX_WAITERS)
			{
				nextWaiterIndex = 0u;
			}

			// Release message lock.
			gos_lockRelease(messageLock);

			// Block task (to be unblocked by daemon).
			gos_kernelTaskBlock(currentTaskId);

			// Task unblocked, check TMO.
			if (messageWaiterArray[messageWaiterIndex].waitTmoCounter < messageWaiterArray[messageWaiterIndex].waitTmo)
			{
				// Message received successfully.
				messageRxResult = GOS_SUCCESS;
			}
		}
	}

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
	gos_result_t		messageTxResult = GOS_ERROR;

	/**
	 * Function code.
	 */
	if (message != NULL &&
		message->messageSize > 0 &&
		message->messageId != GOS_MESSAGE_INVALID_ID &&
		message->messageSize < CFG_MESSAGE_MAX_LENGTH)
	{
		if (messageArray[nextMessageIndex].messageId == GOS_MESSAGE_INVALID_ID)
		{
			// Get message lock.
			gos_lockWaitGet(messageLock);

			memcpy((void_t*)&messageArray[nextMessageIndex], (void_t*)message, sizeof(*message));
			messageTxResult = GOS_SUCCESS;
			if (++nextMessageIndex == CFG_MESSAGE_MAX_NUMBER)
			{
				nextMessageIndex = 0u;
			}

			// Release message lock.
			gos_lockRelease(messageLock);
		}
	}

	return messageTxResult;
}

/**
 * @brief	Message daemon task.
 * @details	This task is responsible for message transfer between tasks. It loops
 * 			through the internal message array and the waiter task array and if a
 * 			message and a waiter matches, it copies the message to the target
 * 			buffer and unblocks the previously blocked task.
 *
 * @return	-
 */
GOS_STATIC void_t gos_messageDaemonTask (void_t)
{
	/*
	 * Local variables.
	 */
	gos_messageIndex_t			messageIndex		= 0u;
	gos_messageWaiterIndex_t	messageWaiterIndex 	= 0u;
	gos_messageIdIndex_t		messageIdIndex 		= 0u;
	bool_t						waiterServed		= GOS_FALSE;

	/*
	 * Function code.
	 */
	for(;;)
	{
		for (messageIndex = 0u; messageIndex < CFG_MESSAGE_MAX_NUMBER; messageIndex++)
		{
			for (messageWaiterIndex = 0u; messageWaiterIndex < CFG_MESSAGE_MAX_WAITERS; messageWaiterIndex++)
			{
				if (messageWaiterArray[messageWaiterIndex].waiterTaskId != GOS_INVALID_TASK_ID)
				{
					waiterServed = GOS_FALSE;
					for (messageIdIndex = 0u; messageIdIndex < CFG_MESSAGE_MAX_WAITER_IDS; messageIdIndex++)
					{
						if (messageWaiterArray[messageWaiterIndex].messageIdArray[messageIdIndex] ==
							messageArray[messageIndex].messageId)
						{
							// Get message lock.
							gos_lockWaitGet(messageLock);

							// ID match found. Copy message to target and delete waiter and message.
							memcpy(messageWaiterArray[messageWaiterIndex].target->messageBytes,
								  (void_t*)messageArray[messageIndex].messageBytes,
								  messageArray[messageIndex].messageSize);
							messageWaiterArray[messageWaiterIndex].target->messageSize	= messageArray[messageIndex].messageSize;
							messageWaiterArray[messageWaiterIndex].target->messageId	= messageArray[messageIndex].messageId;

							gos_kernelTaskUnblock(messageWaiterArray[messageWaiterIndex].waiterTaskId);

							messageArray[messageIndex].messageId = GOS_MESSAGE_INVALID_ID;
							messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
							waiterServed = GOS_TRUE;

							// Release message lock.
							gos_lockRelease(messageLock);

							break;
						}
					}

					if (waiterServed == GOS_FALSE)
					{
						messageWaiterArray[messageWaiterIndex].waitTmoCounter++;

						if (messageWaiterArray[messageWaiterIndex].waitTmoCounter > (messageWaiterArray[messageWaiterIndex].waitTmo * GOS_MESSAGE_DAEMON_POLL_TIME_MS))
						{
							// Timeout. Delete waiter, unblock task.
							gos_kernelTaskUnblock(messageWaiterArray[messageWaiterIndex].waiterTaskId);
							messageWaiterArray[messageWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
						}
					}
				}
			}
		}
		gos_kernelTaskSleep(GOS_MESSAGE_DAEMON_POLL_TIME_MS);
	}
}
