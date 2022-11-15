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
//! @date		2022-10-22
//! @version	1.0
//!
//! @brief		GOS lock service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_lock.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-22	Gabor Repasi	Initial version created.
//*************************************************************************************************
/*
 * Includes
 */
#include "gos_lock.h"

/**
 * Lock waiter descriptor type.
 */
typedef struct
{
	gos_lock_t	lockInstance;		//!< Lock instance.
	gos_tid_t	waiterTaskId;		//!< Waiter task ID.
}gos_lockWaiterDescriptor_t;

/*
 * Static variables
 */
/**
 * Lock array.
 */
GOS_STATIC gos_lock_descriptor_t		lockArray		[CFG_LOCK_MAX_NUMBER];
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
	// Reset the in-use-flag for every lock instance.
	for (index = 0u; index < CFG_LOCK_MAX_NUMBER; index++)
	{
		lockArray[index].inUse 	= GOS_FALSE;
		lockArray[index].lock	= GOS_LOCK_UNLOCKED;
	}

	// Initialize wait array.
	for (lockWaiterIndex = 0u; lockWaiterIndex < CFG_LOCK_MAX_WAITERS; lockWaiterIndex++)
	{
		waitArray[lockWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
	}

	return lockInitResult;
}

/*
 * Function: gos_lockCreate
 */
gos_result_t gos_lockCreate (gos_lock_t* pLock)
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
		if (lockArray[index].inUse == GOS_FALSE)
		{
			if (pLock != NULL)
			{
				*pLock = index;
				lockArray[index].inUse	= GOS_TRUE;
				lockArray[index].lock 	= GOS_LOCK_UNLOCKED;
				lockCreateResult 		= GOS_SUCCESS;
				break;
			}
		}
	}
	return lockCreateResult;
}

/*
 * Function: gos_lockWaitGet
 */
GOS_INLINE gos_result_t gos_lockWaitGet (gos_lock_t lock)
{
	/*
	 * Local variables.
	 */
	gos_result_t				lockGetResult		= GOS_ERROR;
	gos_tid_t					currentTaskId		= GOS_INVALID_TASK_ID;
	gos_lockWaiterIndex_t		lockWaiterIndex		= 0u;

	/*
	 * Function code.
	 */
	if (lock < CFG_LOCK_MAX_NUMBER &&
		lockArray[lock].inUse == GOS_TRUE)
	{
		while (lockArray[lock].lock != GOS_LOCK_UNLOCKED)
		{
			for (lockWaiterIndex = 0u; lockWaiterIndex < CFG_LOCK_MAX_WAITERS; lockWaiterIndex++)
			{
				if (waitArray[lockWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID)
				{
					gos_kernelTaskGetCurrentId(&currentTaskId);
					waitArray[lockWaiterIndex].waiterTaskId	= currentTaskId;
					waitArray[lockWaiterIndex].lockInstance	= lock;

					gos_kernelTaskBlock(currentTaskId);
					break;
				}
			}
		}

		// Get lock.
		lockArray[lock].lock = GOS_LOCK_LOCKED;
		lockGetResult = GOS_SUCCESS;
	}

	return lockGetResult;
}

/*
 * Function: gos_lockRelease
 */
GOS_INLINE gos_result_t gos_lockRelease (gos_lock_t lock)
{
	/*
	 * Local variables.
	 */
	gos_result_t 			releaseResult 	= GOS_ERROR;
	gos_lockWaiterIndex_t	lockWaiterIndex	= 0u;
	gos_lockWaiterIndex_t	nextEmptyIndex	= 0u;
	bool_t					orderingNeeded	= GOS_FALSE;

	/*
	 * Function code.
	 */
	if (lock < CFG_LOCK_MAX_NUMBER &&
		lockArray[lock].inUse == GOS_TRUE)
	{
		// Release lock.
		lockArray[lock].lock = GOS_LOCK_UNLOCKED;
		releaseResult = GOS_SUCCESS;

		// Check waiter array.
		for (lockWaiterIndex = 0u; lockWaiterIndex < CFG_LOCK_MAX_WAITERS; lockWaiterIndex++)
		{
			if (waitArray[lockWaiterIndex].lockInstance == lock && orderingNeeded == GOS_FALSE)
			{
				gos_kernelTaskUnblock(waitArray[lockWaiterIndex].waiterTaskId);
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
				waitArray[nextEmptyIndex].lockInstance = waitArray[lockWaiterIndex].lockInstance;

				nextEmptyIndex = lockWaiterIndex;
			}
		}
	}

	return releaseResult;
}
