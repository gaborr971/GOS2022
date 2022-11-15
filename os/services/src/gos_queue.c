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
//! @file		gos_queue.c
//! @author		Gabor Repasi
//! @date		2022-10-23
//! @version	1.0
//!
//! @brief		GOS queue service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_queue.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-23	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Dump separator changed
//										+	Signal sender enumerators added
//										+	Formatted log used in dump
//*************************************************************************************************
/*
 * Includes
 */
#include <string.h>
#include <stdio.h>
#include "gos_lock.h"
#include "gos_log.h"
#include "gos_process.h"
#include "gos_signal.h"
#include "gos_queue.h"

/*
 * Macros
 */
#if CFG_QUEUE_USE_NAME == 1
#define DUMP_SEPARATOR	"+--------+---------------------------+------------+\r\n"
#else
#define DUMP_SEPARATOR	"+--------+------------+\r\n"
#endif

/*
 * Type definitions
 */
/**
 * Queue element type.
 */
typedef struct
{
	gos_queueByte_t		queueElementBytes [CFG_QUEUE_MAX_LENGTH];	//!< Queue element bytes.
	gos_queueLength_t	elementLength;								//!< Queue element length.
}gos_queueElement_t;

/**
 * Queue type.
 */
typedef struct
{
	gos_queueId_t		queueId;								//!< Queue ID.
#if CFG_QUEUE_USE_NAME == 1
	gos_queueName_t		queueName;								//!< Queue name.
#endif
	gos_queueElement_t	queueElements [CFG_QUEUE_MAX_ELEMENTS];	//!< Queue element array.
	gos_queueIndex_t	actualElementNumber;					//!< Actual number of queue elements.
}gos_queue_t;

/*
 * Static variables
 */
/**
 * Internal queue array.
 */
GOS_STATIC 	gos_queue_t			queues			[CFG_QUEUE_MAX_NUMBER];

/**
 * Read counter array (next queue element to read).
 */
GOS_STATIC	gos_queueLength_t	readCounters	[CFG_QUEUE_MAX_NUMBER];

/**
 * Write counter array (next queue element to write).
 */
GOS_STATIC	gos_queueLength_t	writeCounters	[CFG_QUEUE_MAX_NUMBER];

/**
 * Queue lock.
 */
GOS_STATIC	gos_lock_t 			queueLock;

/**
 * Queue full hook.
 */
GOS_STATIC	gos_queueFullHook	queueFullHook	= NULL;

/**
 * Queue empty hook.
 */
GOS_STATIC	gos_queueEmptyHook	queueEmptyHook	= NULL;

/*
 * Function prototypes
 */
void_t 				gos_queueDumpSignalHandler 	(gos_signalSenderId_t senderId);
GOS_STATIC void_t 	gos_queueDumpTask			(void_t);

/**
 * Queue dump task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t queueDumpTaskDesc =
{
	.taskFunction 	= gos_queueDumpTask,
	.taskName		= "gos_queue_dump_task",
	.taskPriority	= CFG_TASK_QUEUE_DUMP_PRIO,
	.taskStackSize	= CFG_TASK_QUEUE_DUMP_STACK
};

/*
 * Function: gos_queueInit
 */
gos_result_t gos_queueInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t		queueInitResult		= GOS_ERROR;
	gos_queueIndex_t	queueIndex			= 0u;
	gos_queueIndex_t	queueElementIndex	= 0u;

	/*
	 * Function code.
	 */
	for (queueIndex = 0u; queueIndex < CFG_QUEUE_MAX_NUMBER; queueIndex++)
	{
		readCounters[queueIndex] 				= 0u;
		writeCounters[queueIndex] 				= 0u;
		queues[queueIndex].queueId 				= GOS_INVALID_QUEUE_ID;
		queues[queueIndex].actualElementNumber	= 0u;

		for (queueElementIndex = 0u; queueElementIndex < CFG_QUEUE_MAX_ELEMENTS; queueElementIndex++)
		{
			queues[queueIndex].queueElements[queueElementIndex].elementLength = 0u;
		}
	}

	// Create lock, register and suspend queue dump task.
	if (gos_lockCreate(&queueLock) == GOS_SUCCESS &&
		gos_kernelTaskRegister(&queueDumpTaskDesc, &queueDumpTaskId) == GOS_SUCCESS &&
		gos_kernelTaskSuspend(queueDumpTaskId) == GOS_SUCCESS)
	{
		queueInitResult = GOS_SUCCESS;
	}
	return queueInitResult;
}

/*
 * Function: gos_queueCreate
 */
gos_result_t gos_queueCreate (gos_queueDescriptor_t* pQueueDescriptor)
{
	/*
	 * Local variables.
	 */
	gos_result_t		queueCreateResult	= GOS_ERROR;
	gos_queueIndex_t	queueIndex			= 0u;

	/*
	 * Function code.
	 */
	if (pQueueDescriptor != NULL)
	{
		for (queueIndex = 0u; queueIndex < CFG_QUEUE_MAX_NUMBER; queueIndex++)
		{
			if (queues[queueIndex].queueId == GOS_INVALID_QUEUE_ID)
			{
				queues[queueIndex].queueId = (GOS_DEFAULT_QUEUE_ID + queueIndex);
				pQueueDescriptor->queueId = queues[queueIndex].queueId;
#if CFG_QUEUE_USE_NAME == 1
				if (pQueueDescriptor->queueName != NULL &&
					strlen(pQueueDescriptor->queueName) <= CFG_QUEUE_MAX_NAME_LENGTH)
				{
					strcpy(queues[queueIndex].queueName, pQueueDescriptor->queueName);
				}
#endif
				queueCreateResult = GOS_SUCCESS;
				break;
			}
		}
	}

	return queueCreateResult;
}

/*
 * Function: gos_queuePut
 */
gos_result_t gos_queuePut (gos_queueId_t queueId, void_t* element, gos_queueLength_t elementSize)
{
	/*
	 * Local variables.
	 */
	gos_result_t		queuePutResult	= GOS_ERROR;
	gos_queueIndex_t	queueIndex		= 0u;

	/*
	 * Function code.
	 */
	gos_lockWaitGet(queueLock);

	if (queueId >= GOS_DEFAULT_QUEUE_ID &&
		(queueId - GOS_DEFAULT_QUEUE_ID) < CFG_QUEUE_MAX_NUMBER &&
		element != NULL &&
		elementSize <= CFG_QUEUE_MAX_LENGTH &&
		queues[(queueId - GOS_DEFAULT_QUEUE_ID)].queueId != GOS_INVALID_QUEUE_ID)
	{
		queueIndex = (gos_queueIndex_t)(queueId - GOS_DEFAULT_QUEUE_ID);

		// Check if queue is full.
		if ((readCounters[queueIndex] > 0 && writeCounters[queueIndex] != (readCounters[queueIndex] - 1)) ||
			((readCounters[queueIndex] == 0) && writeCounters[queueIndex] != CFG_QUEUE_MAX_ELEMENTS - 1))
		{
			memcpy(queues[queueIndex].queueElements[writeCounters[queueIndex]].queueElementBytes, element, elementSize);
			queues[queueIndex].queueElements[writeCounters[queueIndex]].elementLength = elementSize;
			writeCounters[queueIndex]++;
			queues[queueIndex].actualElementNumber++;

			if (writeCounters[queueIndex] >= CFG_QUEUE_MAX_ELEMENTS)
			{
				writeCounters[queueIndex] = 0u;
			}

			// If queue gets full and there is a registered hook function, call it.
			if (queues[queueIndex].actualElementNumber == CFG_QUEUE_MAX_ELEMENTS - 1 &&
				queueFullHook != NULL)
			{
				queueFullHook(queueId);
			}

			queuePutResult = GOS_SUCCESS;
		}
	}

	gos_lockRelease(queueLock);
	return queuePutResult;
}

/*
 * Function: gos_queueGet
 */
gos_result_t gos_queueGet (gos_queueId_t queueId, void_t* target, gos_queueLength_t targetSize)
{
	/*
	 * Local variables.
	 */
	gos_result_t		queueGetResult	= GOS_ERROR;
	gos_queueIndex_t	queueIndex		= 0u;

	/*
	 * Function code.
	 */
	gos_lockWaitGet(queueLock);

	if (queueId >= GOS_DEFAULT_QUEUE_ID &&
		(queueId - GOS_DEFAULT_QUEUE_ID) < CFG_QUEUE_MAX_NUMBER &&
		target != NULL &&
		queues[(queueId - GOS_DEFAULT_QUEUE_ID)].queueId != GOS_INVALID_QUEUE_ID &&
		targetSize >= queues[(queueId - GOS_DEFAULT_QUEUE_ID)].queueElements[readCounters[(queueId - GOS_DEFAULT_QUEUE_ID)]].elementLength)
	{
		queueIndex = (gos_queueIndex_t)(queueId - GOS_DEFAULT_QUEUE_ID);

		// Check if queue is empty.
		if (readCounters[queueIndex] != writeCounters[queueIndex])
		{
			memcpy(target, queues[queueIndex].queueElements[readCounters[queueIndex]].queueElementBytes, queues[queueIndex].queueElements[readCounters[queueIndex]].elementLength);
			queues[queueIndex].queueElements[readCounters[queueIndex]].elementLength = 0u;
			readCounters[queueIndex]++;
			queues[queueIndex].actualElementNumber--;

			if (readCounters[queueIndex] >= CFG_QUEUE_MAX_ELEMENTS)
			{
				readCounters[queueIndex] = 0u;
			}

			// If queue is empty now and empty hook function is registered, call it.
			if (queues[queueIndex].actualElementNumber == 0u &&
				queueEmptyHook != NULL)
			{
				queueEmptyHook(queueId);
			}

			queueGetResult = GOS_SUCCESS;
		}
	}

	gos_lockRelease(queueLock);
	return queueGetResult;
}

/*
 * Function: gos_queueRegisterFullHook
 */
gos_result_t gos_queueRegisterFullHook (gos_queueFullHook fullHook)
{
	/*
	 * Local variables.
	 */
	gos_result_t queueRegisterFullHookResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (fullHook != NULL && queueFullHook == NULL)
	{
		queueFullHook = fullHook;
		queueRegisterFullHookResult = GOS_SUCCESS;
	}

	return queueRegisterFullHookResult;
}

/*
 * Function: gos_queueRegisterEmptyHook
 */
gos_result_t gos_queueRegisterEmptyHook (gos_queueEmptyHook emptyHook)
{
	/*
	 * Local variables.
	 */
	gos_result_t 		queueRegisterEmptyHookResult	= GOS_ERROR;

	/*
	 * Function code.
	 */
	if (emptyHook != NULL && queueEmptyHook == NULL)
	{
		queueEmptyHook = emptyHook;
		queueRegisterEmptyHookResult = GOS_SUCCESS;
	}

	return queueRegisterEmptyHookResult;
}

/*
 * Function: gos_queueGetName
 */
gos_result_t gos_queueGetName (gos_queueId_t queueId, gos_queueName_t queueName)
{
	/*
	 * Local variables.
	 */
	gos_result_t 		queueGetNameResult	= GOS_ERROR;
	gos_queueIndex_t	queueIndex			= 0u;

	/*
	 * Function code.
	 */
	if (queueId >= GOS_DEFAULT_QUEUE_ID &&
		(queueId - GOS_DEFAULT_QUEUE_ID) < CFG_QUEUE_MAX_NUMBER &&
		queues[(queueId - GOS_DEFAULT_QUEUE_ID)].queueId != GOS_INVALID_QUEUE_ID)
	{
		queueIndex = (gos_queueIndex_t)(queueId - GOS_DEFAULT_QUEUE_ID);

		if (queueName != NULL)
		{
			strcpy(queueName, queues[queueIndex].queueName);
			queueGetNameResult = GOS_SUCCESS;
		}
	}

	return queueGetNameResult;
}

/*
 * Function: gos_queueGetElementNumber
 */
gos_result_t gos_queueGetElementNumber (gos_queueId_t queueId, gos_queueIndex_t* elementNumber)
{

	/*
	 * Local variables.
	 */
	gos_result_t 		queueGetElementNumberResult	= GOS_ERROR;
	gos_queueIndex_t	queueIndex					= 0u;

	/*
	 * Function code.
	 */
	if (queueId >= GOS_DEFAULT_QUEUE_ID &&
		(queueId - GOS_DEFAULT_QUEUE_ID) < CFG_QUEUE_MAX_NUMBER &&
		queues[(queueId - GOS_DEFAULT_QUEUE_ID)].queueId != GOS_INVALID_QUEUE_ID)
	{
		queueIndex = (gos_queueIndex_t)(queueId - GOS_DEFAULT_QUEUE_ID);

		*elementNumber = queues[queueIndex].actualElementNumber;
		queueGetElementNumberResult = GOS_SUCCESS;
	}

	return queueGetElementNumberResult;
}

/**
 * @brief	Handles the kernel dump signal.
 * @details	Resumes the queue dump task based on the sender ID.
 *
 * @param	senderId	:	Sender ID.
 *
 * @return	-
 */
void_t gos_queueDumpSignalHandler (gos_signalSenderId_t senderId)
{
	/*
	 * Function code.
	 */
	if (senderId == GOS_DUMP_SENDER_PROC)
	{
		gos_kernelTaskResume(queueDumpTaskId);
	}
}

/**
 * @brief	Queue dump task.
 * @details	Prints the queue data of all queues to the log output and suspends itself.
 * 			This task is resumed by the kernel dump signal (through its handler function).
 *
 * @return	-
 */
GOS_STATIC void_t gos_queueDumpTask (void_t)
{
	/*
	 * Local variables.
	 */
	GOS_EXTERN gos_signalId_t	kernelDumpSignal;
	gos_queueIndex_t			queueIndex	= 0u;

	/*
	 * Function code.
	 */
	for(;;)
	{
		gos_logLogFormatted("Queue dump:\r\n");
		gos_logLogFormatted(DUMP_SEPARATOR);

#if CFG_QUEUE_USE_NAME == 1
		gos_logLogFormatted(
				"| %6s | %25s | %10s |\r\n",
				"qid",
				"queue name",
				"elements"
				);
#else
		gos_logLogFormatted(
				"| %6s | %10s |\r\n",
				"qid",
				"elements"
				);
#endif
		gos_logLogFormatted(DUMP_SEPARATOR);

		for (queueIndex = 0u; queueIndex < CFG_QUEUE_MAX_NUMBER; queueIndex++)
		{
			if (queues[queueIndex].queueId == GOS_INVALID_QUEUE_ID)
			{
				break;
			}
#if CFG_QUEUE_USE_NAME == 1
			gos_logLogFormatted(
					"| 0x%04X | %25s | %10d |\r\n",
					queues[queueIndex].queueId,
					queues[queueIndex].queueName,
					queues[queueIndex].actualElementNumber
					);
#else
			gos_logLogFormatted(
					"| 0x%04X | %10d |\r\n",
					queues[queueIndex].queueId,
					queues[queueIndex].actualElementNumber
					);
#endif
		}
		gos_logLogFormatted(DUMP_SEPARATOR"\n");
		gos_signalInvoke(kernelDumpSignal, GOS_DUMP_SENDER_QUEUE);
 		gos_kernelTaskSuspend(queueDumpTaskId);
	}
}
