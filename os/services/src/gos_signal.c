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
//! @file		gos_signal.c
//! @author		Gabor Repasi
//! @date		2022-10-23
//! @version	1.0
//!
//! @brief		GOS signal service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_signal.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-23	Gabor Repasi	Initial version created.
//*************************************************************************************************
/*
 * Includes
 */
#include <string.h>
#include "gos_signal.h"
#include "gos_queue.h"

/*
 * Type definitions
 */
/**
 * Signal descriptor type.
 */
typedef struct
{
	bool_t				inUse;									//!< Flag to indicate whether the signal is in use.
	gos_signalHandler_t	handlers [CFG_SIGNAL_MAX_SUBSCRIBERS];	//!< Signal handler array.
}gos_signalDescriptor_t;

/**
 * Signal invoke descriptor type.
 */
typedef struct
{
	gos_signalId_t 			signalId;	//!< Signal ID.
	gos_signalSenderId_t 	senderId;	//!< Sender ID.
}gos_signalInvokeDescriptor;

/*
 * Static variables
 */
/**
 * Internal signal descriptor array.
 */
GOS_STATIC gos_signalDescriptor_t	signalArray 		[CFG_SIGNAL_MAX_NUMBER];

/**
 * Signal invoke queue (FIFO).
 */
GOS_STATIC gos_queueDescriptor_t	signalInvokeQueueDesc = {
#if CFG_QUEUE_USE_NAME == 1
		.queueName = "gos_signal_invoke_queue"
#endif
};

/**
 * Signal daemon task ID.
 */
GOS_STATIC gos_tid_t				signalDaemonTaskId;

/*
 * External data
 */
GOS_EXTERN gos_signalId_t			kernelDumpSignal;

/*
 * External functions
 */
GOS_EXTERN void_t gos_kernelDumpSignalHandler	(gos_signalSenderId_t senderId);
GOS_EXTERN void_t gos_procDumpSignalHandler	 	(gos_signalSenderId_t senderId);
GOS_EXTERN void_t gos_queueDumpSignalHandler	(gos_signalSenderId_t senderId);

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_signalDaemonTask (void_t);

/**
 * Signal daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t signalDaemonTaskDescriptor =
{
	.taskFunction 	= gos_signalDaemonTask,
	.taskName		= "gos_signal_daemon",
	.taskStackSize	= CFG_TASK_SIGNAL_DAEMON_STACK,
	.taskPriority	= CFG_TASK_SIGNAL_DAEMON_PRIO
};

/*
 * Function: gos_signalInit
 */
gos_result_t gos_signalInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t 		signalInitResult	= GOS_ERROR;
	gos_signalIndex_t	signalIndex			= 0u;

	/*
	 * Function code.
	 */
	// Reset the in-use flag for every signal instance.
	for (signalIndex = 0u; signalIndex < CFG_SIGNAL_MAX_NUMBER; signalIndex++)
	{
		signalArray[signalIndex].inUse = GOS_FALSE;
	}

	// Create queue for signal invoking and register signal daemon task.
	if (gos_queueCreate(&signalInvokeQueueDesc) == GOS_SUCCESS &&
		gos_kernelTaskRegister(&signalDaemonTaskDescriptor, &signalDaemonTaskId) == GOS_SUCCESS &&
		gos_signalCreate(&kernelDumpSignal) == GOS_SUCCESS &&
		gos_signalSubscribe(kernelDumpSignal, gos_kernelDumpSignalHandler) == GOS_SUCCESS &&
		gos_signalSubscribe(kernelDumpSignal, gos_procDumpSignalHandler) == GOS_SUCCESS &&
		gos_signalSubscribe(kernelDumpSignal, gos_queueDumpSignalHandler) == GOS_SUCCESS)
	{
		signalInitResult = GOS_SUCCESS;
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
	gos_result_t		signalCreateResult 	= GOS_ERROR;
	gos_signalIndex_t	signalIndex			= 0u;

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
	gos_result_t				signalSubscribeResult 	= GOS_ERROR;
	gos_signalHandlerIndex_t	signalHandlerIndex		= 0u;

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
gos_result_t gos_signalInvoke (gos_signalId_t signalId, gos_signalSenderId_t senderId)
{
	/*
	 * Local variables.
	 */
	gos_result_t				signalInvokeResult 		= GOS_ERROR;
	gos_signalInvokeDescriptor	signalInvokeDescriptor	= {0};

	/*
	 * Function code.
	 */
	if (signalId < CFG_SIGNAL_MAX_NUMBER && signalArray[signalId].inUse == GOS_TRUE)
	{
		signalInvokeDescriptor.signalId = signalId;
		signalInvokeDescriptor.senderId = senderId;

		gos_queuePut(signalInvokeQueueDesc.queueId, (void_t*)&signalInvokeDescriptor, sizeof(signalInvokeDescriptor));

		signalInvokeResult = GOS_SUCCESS;
	}

	return signalInvokeResult;
}

/**
 * @brief	Signal daemon task.
 * @details	Polls the signal invoke queue, and completes the necessary signal invokings.
 *
 * @return -
 */
GOS_STATIC void_t gos_signalDaemonTask (void_t)
{
	/*
	 * Local variables.
	 */
	gos_signalInvokeDescriptor	signalInvokeDescriptor	= {0};
	gos_signalHandlerIndex_t	signalHandlerIndex		= 0u;

	/*
	 * Function code.
	 */
	for (;;)
	{
		if (gos_queueGet(signalInvokeQueueDesc.queueId, (void_t*)&signalInvokeDescriptor, sizeof(signalInvokeDescriptor)) == GOS_SUCCESS)
		{
			for (signalHandlerIndex = 0u; signalHandlerIndex < CFG_SIGNAL_MAX_SUBSCRIBERS; signalHandlerIndex++)
			{
				if (signalArray[signalInvokeDescriptor.signalId].handlers[signalHandlerIndex] == NULL)
				{
					// Last handler called, stop calling.
					break;
				}
				else
				{
					// Call signal handler.
					signalArray[signalInvokeDescriptor.signalId].handlers[signalHandlerIndex](signalInvokeDescriptor.senderId);
				}
			}
		}
		gos_kernelTaskSleep(100);
	}
}
