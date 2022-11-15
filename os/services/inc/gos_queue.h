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
//! @file		gos_queue.h
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS queue service header.
//! @details	Queue service is one of the inter-task communication solutions offered by the OS.
//! 			A queue can hold a given number of elements with a given element size. The queue
//! 			is implemented as a FIFO. Queues are unsafe in a way that any task with the
//! 			queue ID in their knowledge can put and get elements to/from the queue. So queues
//! 			are most suitable for serializing data that are coming from multiple resources but
//! 			processes by a single task. It is also possible to peek the queue, meaning that
//! 			the next element of the queue can be requested without deleting it form the queue.
//! 			This way multiple tasks can process data coming from a queue in a cooperative way,
//! 			but they rely on each others data processing.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-23	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	Function descriptions updated
// 1.2		2022-11-15	Gabor Repasi	+	Queue peek function added
//										+	Service description added
//										+	License added
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
#ifndef GOS_QUEUE_H
#define GOS_QUEUE_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Macros
 */
/**
 * Default queue ID.
 */
#define GOS_DEFAULT_QUEUE_ID	( (gos_queueId_t)0x3000 )

/**
 * Invalid queue ID.
 */
#define GOS_INVALID_QUEUE_ID	( (gos_queueId_t)0x0300 )

/*
 * Type definitions
 */
/**
 * One byte in a queue element.
 */
typedef u8_t	gos_queueByte_t;

#if CFG_QUEUE_MAX_LENGTH < 255
typedef u8_t	gos_queueLength_t;								//!< Queue length type.
#else
typedef u16_t	gos_queueLength_t;								//!< Queue length type.
#endif
#if CFG_QUEUE_MAX_NUMBER < 255 && CFG_QUEUE_MAX_LENGTH < 255
typedef u8_t	gos_queueIndex_t;								//!< Queue index type.
#else
typedef u16_t	gos_queueIndex_t;								//!< Queue index type.
#endif
typedef u16_t	gos_queueId_t;									//!< Queue ID type.

#if CFG_QUEUE_USE_NAME == 1
typedef char_t	gos_queueName_t [CFG_QUEUE_MAX_NAME_LENGTH];	//!< Queue name type.
#endif

/**
 * Queue full hook type.
 */
typedef void_t (*gos_queueFullHook)(gos_queueId_t);

/**
 * Queue full hook type.
 */
typedef void_t (*gos_queueEmptyHook)(gos_queueId_t);

/**
 * Public queue descriptor type.
 */
typedef struct
{
	gos_queueId_t	queueId;	//!< Queue ID.
#if CFG_QUEUE_USE_NAME == 1
	gos_queueName_t	queueName;	//!< Queue name.
#endif
}gos_queueDescriptor_t;

/*
 * Global data
 */
/**
 * Queue dump task ID.
 */
gos_tid_t queueDumpTaskId;

/*
 * Function prototypes
 */
/**
 * @brief	This function initializes the queue service.
 * @details	TODO
 *
 * @return	Result of initialization.
 *
 * @retval	GOS_SUCCESS : Initialization successful.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queueInit (void_t);

/**
 * @brief	This function creates a new queue.
 * @details	TODO
 *
 * @param	pQueueDescriptor	: Pointer to queue descriptor variable with queue data.
 *
 * @return	Result of queue creation.
 *
 * @retval	GOS_SUCCESS : Queue creation successful.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queueCreate (gos_queueDescriptor_t* pQueueDescriptor);

/**
 * @brief	This function puts an element in the given queue.
 * @details	TODO
 *
 * @param	queueId		: Queue ID.
 * @param	element		: Pointer to element.
 * @param	elementSize	: Size of element.
 *
 * @return	Result of element putting.
 *
 * @retval	GOS_SUCCESS : Element successfully put in the queue.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queuePut (gos_queueId_t queueId, void_t* element, gos_queueLength_t elementSize);

/**
 * @brief	This function gets the next element from the given queue.
 * @details	TODO
 *
 * @param	queueId		: Queue ID.
 * @param	target		: Pointer to target variable.
 * @param 	targetSize	: Size of target.
 *
 * @return	Result of element getting.
 *
 * @retval	GOS_SUCCESS : Element successfully moved from queue to target.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queueGet (gos_queueId_t queueId, void_t* target, gos_queueLength_t targetSize);

/**
 * @brief	This function gets the next element from the given queue without removing it.
 * @details	TODO
 *
 * @param	queueId		: Queue ID.
 * @param	target		: Pointer to target variable.
 * @param 	targetSize	: Size of target.
 *
 * @return	Result of element getting.
 *
 * @retval	GOS_SUCCESS : Element successfully copied from queue to target.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queuePeek (gos_queueId_t queueId, void_t* target, gos_queueLength_t targetSize);

/**
 * @brief	This function registers a queue full hook function.
 * @details	TODO
 *
 * @param	fullHook	: Hook function.
 *
 * @return	Result of hook registration.
 *
 * @retval	GOS_SUCCESS : Hook registration successful.
 * @retval	GOS_ERROR   : Hook already exists or parameter is NULL pointer.
 */
gos_result_t gos_queueRegisterFullHook (gos_queueFullHook fullHook);

/**
 * @brief	This function registers a queue empty hook function.
 * @details	TODO
 *
 * @param	emptyHook	: Hook function.
 *
 * @return	Result of hook registration.
 *
 * @retval	GOS_SUCCESS : Hook registration successful.
 * @retval	GOS_ERROR   : Hook already exists or parameter is NULL pointer.
 */
gos_result_t gos_queueRegisterEmptyHook (gos_queueEmptyHook emptyHook);

/**
 * @brief	This function gets the name of the given queue.
 * @details	TODO
 *
 * @param 	queueId		: Queue ID.
 * @param 	queueName	: Queue name.
 *
 * @return	Result of queue name getting.
 *
 * @retval	GOS_SUCCESS : Name getting successful.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queueGetName (gos_queueId_t queueId, gos_queueName_t queueName);

/**
 * @brief	This function gets the number of elements in the given queue.
 * @details	TODO
 *
 * @param 	queueId			: Queue ID.
 * @param 	elementNumber	: Pointer to variable to store the element number in.
 *
 * @return	Result of queue element number getting.
 *
 * @retval	GOS_SUCCESS : Element number getting successful.
 * @retval	GOS_ERROR   : TODO
 */
gos_result_t gos_queueGetElementNumber (gos_queueId_t queueId, gos_queueIndex_t* elementNumber);

#endif
