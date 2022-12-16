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
//! @file		gos_lock.c
//! @author		Gabor Repasi
//! @date		2022-12-13
//! @version	1.3
//!
//! @brief		GOS lock service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_lock.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-22	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	License added
// 1.2		2022-12-11	Gabor Repasi	*	Lock variable changed to structure
//										+	Lock owner, ID, and status added to handling
// 1.3		2022-12-13	Gabor Repasi	+	Privilege handling added to ensure that lock service
//											have privileged access to kernel functions no matter
//											what privilege the caller task has
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
#include "gos_lock.h"
#include "gos_error.h"

/*
 * Macros
 */
/**
 * Invalid lock ID.
 */
#define GOS_INVALID_LOCK_ID	( 0x100 )

/**
 * Default lock ID.
 */
#define GOS_DEFAULT_LOCK_ID	( 0x7000 )

/*
 * Type definitions
 */
/**
 * Lock waiter descriptor type.
 */
typedef struct
{
	gos_lockId_t	lockId;			//!< Lock ID.
	gos_tid_t		waiterTaskId;	//!< Waiter task ID.
}gos_lockWaiterDescriptor_t;

/**
 * Lock status type.
 */
typedef enum
{
	GOS_LOCK_UNLOCKED	= 0b0101,	//!< Lock unclocked.
	GOS_LOCK_LOCKED		= 0b1010	//!< Lock locked.
}gos_lock_status_t;

/**
 * Lock type.
 */
typedef struct
{
	gos_lockId_t		lockId;			//!< Lock ID.
	gos_lock_status_t	lockStatus;		//!< Lock status.
	gos_tid_t			lockOwnerTask;	//!< Lock owner task ID.
}gos_lock_t;

/*
 * Static variables
 */
/**
 * Lock array.
 */
GOS_STATIC gos_lock_t					lockArray		[CFG_LOCK_MAX_NUMBER];

/**
 * Wait array (FIFO).
 */
GOS_STATIC gos_lockWaiterDescriptor_t	waitArray 		[CFG_LOCK_MAX_WAITERS];

/*
 * Function: gos_lockInit
 */
gos_result_t gos_lockInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t			lockInitResult	= GOS_SUCCESS;
	gos_lockIndex_t			index			= 0u;
	gos_lockWaiterIndex_t	lockWaiterIndex	= 0u;

	/*
	 * Function code.
	 */
	// Initialize lock array.
	for (index = 0u; index < CFG_LOCK_MAX_NUMBER; index++)
	{
		lockArray[index].lockId			= GOS_INVALID_LOCK_ID;
		lockArray[index].lockOwnerTask	= GOS_INVALID_TASK_ID;
		lockArray[index].lockStatus		= GOS_LOCK_UNLOCKED;
	}

	// Initialize wait array.
	for (lockWaiterIndex = 0u; lockWaiterIndex < CFG_LOCK_MAX_WAITERS; lockWaiterIndex++)
	{
		waitArray[lockWaiterIndex].lockId		= GOS_INVALID_LOCK_ID;
		waitArray[lockWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
	}

	return lockInitResult;
}

/*
 * Function: gos_lockCreate
 */
gos_result_t gos_lockCreate (gos_lockId_t* pLockId)
{
	/*
	 * Local variables.
	 */
	gos_result_t	lockCreateResult	= GOS_ERROR;
	gos_lockIndex_t	index				= 0u;

	/*
	 * Function code.
	 */
	// Find the next unused lock and reserve it.
	for (index = 0u; index < CFG_LOCK_MAX_NUMBER; index++)
	{
		if (lockArray[index].lockId == GOS_INVALID_LOCK_ID)
		{
			if (pLockId != NULL)
			{
				lockArray[index].lockId 	= GOS_DEFAULT_LOCK_ID + index;
				*pLockId 					= lockArray[index].lockId;
				lockArray[index].lockStatus = GOS_LOCK_UNLOCKED;
				lockCreateResult 			= GOS_SUCCESS;
				break;
			}
		}
	}
	return lockCreateResult;
}

/*
 * Function: gos_lockWaitGet
 */
gos_result_t gos_lockWaitGet (gos_lockId_t lockId)
{
	GOS_DISABLE_SCHED
	/*
	 * Local variables.
	 */
	gos_result_t			lockGetResult		= GOS_ERROR;
	gos_tid_t				currentTaskId		= GOS_INVALID_TASK_ID;
	gos_lockWaiterIndex_t	lockWaiterIndex		= 0u;
	gos_lockIndex_t			lockIndex			= 0u;
	bool_t					isCallerIsr;
#if CFG_USE_PRIO_INHERITANCE == 1
	gos_taskPrio_t			currentTaskPrio		= CFG_TASK_MAX_PRIO_LEVELS;
	gos_taskPrio_t			lockOwnerPrio		= CFG_TASK_MAX_PRIO_LEVELS;
#endif

	/*
	 * Function code.
	 */
	GOS_IS_CALLER_ISR(isCallerIsr);
	lockIndex = lockId - GOS_DEFAULT_LOCK_ID;

	if (lockIndex < CFG_LOCK_MAX_NUMBER && lockArray[lockIndex].lockId != GOS_INVALID_LOCK_ID &&
		gos_kernelTaskGetCurrentId(&currentTaskId) == GOS_SUCCESS)
	{
		if (lockArray[lockIndex].lockStatus != GOS_LOCK_UNLOCKED)
		{
			for (lockWaiterIndex = 0u; lockWaiterIndex < CFG_LOCK_MAX_WAITERS; lockWaiterIndex++)
			{
				if (isCallerIsr == GOS_TRUE)
				{
					break;
				}

				if (waitArray[lockWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID)
				{
					waitArray[lockWaiterIndex].waiterTaskId	= currentTaskId;
					waitArray[lockWaiterIndex].lockId	= lockId;

#if CFG_USE_PRIO_INHERITANCE == 1
					// If priority of waiter is higher than priority of lock owner,
					// change lock owner priority.
					(void_t) gos_kernelTaskGetPriority(currentTaskId, &currentTaskPrio);
					(void_t) gos_kernelTaskGetPriority(lockArray[lockIndex].lockOwnerTask, &lockOwnerPrio);

					if (currentTaskPrio < lockOwnerPrio)
					{
						GOS_PRIVILEGED_ACCESS
						(void_t) gos_kernelTaskSetPriority(lockArray[lockIndex].lockOwnerTask, currentTaskPrio);
					}
#endif
					GOS_ENABLE_SCHED
					GOS_PRIVILEGED_ACCESS
					if (gos_kernelTaskBlock(currentTaskId) != GOS_SUCCESS)
					{
						(void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "Error blocking task: %d.", currentTaskId);
					}
					break;
				}
			}
		}

		// At this point task is either waken-up or waiter array is full.
		if (lockWaiterIndex == CFG_LOCK_MAX_WAITERS)
		{
			GOS_ENABLE_SCHED
			(void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "Lock waiter array full. Current task: %d.", currentTaskId);
		}
		else if (lockArray[lockIndex].lockStatus == GOS_LOCK_UNLOCKED)
		{
			// Get lock.
			lockArray[lockIndex].lockStatus		= GOS_LOCK_LOCKED;
			lockArray[lockIndex].lockOwnerTask	= currentTaskId;
			lockGetResult = GOS_SUCCESS;
		}
	}
	GOS_ENABLE_SCHED

	return lockGetResult;
}

/*
 * Function: gos_lockRelease
 */
gos_result_t gos_lockRelease (gos_lockId_t lockId)
{
	GOS_DISABLE_SCHED
	/*
	 * Local variables.
	 */
	gos_result_t 			releaseResult 			= GOS_ERROR;
	gos_lockWaiterIndex_t	lockWaiterIndex			= 0u;
	gos_lockWaiterIndex_t	nextEmptyIndex			= 0u;
	bool_t					orderingNeeded			= GOS_FALSE;
	gos_lockIndex_t			lockIndex				= 0u;
	gos_tid_t				currentTask				= GOS_INVALID_TASK_ID;
#if CFG_USE_PRIO_INHERITANCE == 1
	gos_taskPrio_t			lockOwnerPrio			= 0u;
	gos_taskPrio_t			lockOwnerOriginalPrio	= 0u;
#endif

	/*
	 * Function code.
	 */
	lockIndex = lockId - GOS_DEFAULT_LOCK_ID;

	if (lockIndex < CFG_LOCK_MAX_NUMBER &&
		lockArray[lockIndex].lockId != GOS_INVALID_LOCK_ID &&
		lockArray[lockIndex].lockOwnerTask != GOS_INVALID_TASK_ID &&
		gos_kernelTaskGetCurrentId(&currentTask) == GOS_SUCCESS &&
		currentTask == lockArray[lockIndex].lockOwnerTask)
	{
		// Release lock.
		lockArray[lockIndex].lockStatus = GOS_LOCK_UNLOCKED;
		lockArray[lockIndex].lockOwnerTask = GOS_INVALID_TASK_ID;
#if CFG_USE_PRIO_INHERITANCE == 1
		// Restore priority if needed.
		(void_t) gos_kernelTaskGetPriority(lockArray[lockIndex].lockOwnerTask, &lockOwnerPrio);
		(void_t) gos_kernelTaskGetOriginalPriority(lockArray[lockIndex].lockOwnerTask, &lockOwnerOriginalPrio);

		if (lockOwnerPrio != lockOwnerOriginalPrio)
		{
			GOS_PRIVILEGED_ACCESS
			(void_t) gos_kernelTaskSetPriority(lockArray[lockIndex].lockOwnerTask, lockOwnerOriginalPrio);
		}
#endif

		releaseResult = GOS_SUCCESS;

		// Check waiter array.
		for (lockWaiterIndex = 0u; lockWaiterIndex < CFG_LOCK_MAX_WAITERS; lockWaiterIndex++)
		{
			if (waitArray[lockWaiterIndex].lockId == lockId && orderingNeeded == GOS_FALSE)
			{
				GOS_PRIVILEGED_ACCESS
				(void_t) gos_kernelTaskUnblock(waitArray[lockWaiterIndex].waiterTaskId);
				waitArray[lockWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
				nextEmptyIndex = lockWaiterIndex;
				orderingNeeded = GOS_TRUE;
			}
			else if (orderingNeeded == GOS_TRUE)
			{
				if (waitArray[lockWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID)
				{
					break;
				}
				waitArray[nextEmptyIndex].waiterTaskId = waitArray[lockWaiterIndex].waiterTaskId;
				waitArray[nextEmptyIndex].lockId = waitArray[lockWaiterIndex].lockId;

				nextEmptyIndex = lockWaiterIndex;
			}
		}
	}
	GOS_ENABLE_SCHED

	return releaseResult;
}
