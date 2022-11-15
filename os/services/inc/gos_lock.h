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
//! @file		gos_lock.h
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.1
//!
//! @brief		GOS lock service header.
//! @details	Lock service is a way of protecting shared resources between different tasks and
//! 			thus synchronizing the operation of tasks in the operating system. Locks can be
//! 			created, locked and released. The locking of a lock instance can happen in two
//! 			ways: either the caller task tries to get the lock and if it fails the API simply
//! 			returns with an error or the locking can be forced: in this scenario if the lock is
//! 			currently used by another task, the caller task will go to blocked state and put in
//! 			a waiting array. When the lock is released, the next waiter in the array will be
//! 			unblocked and can access the lock. When using this approach, the programmer shall
//! 			be aware of the risk of causing deadlocks in the system!
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-22	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	License added
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
#ifndef GOS_LOCK_H
#define GOS_LOCK_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * OS lock type.
 */
typedef u8_t	gos_lock_t;

#if CFG_LOCK_MAX_NUMBER < 255
typedef u8_t	gos_lockIndex_t;		//!< Lock index type.
#else
typedef	u16_t	gos_lockIndex_t;		//!< Lock index type.
#endif

#if CFG_LOCK_MAX_WAITERS < 255
typedef u8_t	gos_lockWaiterIndex_t;	//!< Lock waiter index type.
#else
typedef u16_t	gos_lockWaiterIndex_t;	//!< Lock waiter index type.
#endif

/**
 * Lock status type.
 */
typedef enum
{
	GOS_LOCK_UNLOCKED	= 0b0101,	//!< Lock unclocked.
	GOS_LOCK_LOCKED		= 0b1010	//!< Lock locked.
}gos_lock_status_t;

/**
 * Lock descriptor type.
 */
typedef struct
{
	bool_t		inUse;				//!< Flag indicating that the given lock is in use.
	gos_lock_t	lock;				//!< The lock instance.
}gos_lock_descriptor_t;

/*
 * Function prototypes.
 */
/**
 * @brief	This function initializes the lock service.
 * @details	This function sets the locks to unused and unlocked, and fills the waiter task
 * 			array with invalid task IDs.
 *
 * @return	Result of initialization.
 *
 * @retval	GOS_SUCCESS : Initialization successful.
 */
gos_result_t gos_lockInit (void_t);

/**
 * @brief	This function creates a new lock instance.
 * @details	Goes through the lock array and allocates the next free lock.
 *
 * @return	Result of lock creation.
 *
 * @retval	GOS_SUCCESS : Lock created successfully.
 * @retval	GOS_ERROR   : Lock array is full.
 */
gos_result_t gos_lockCreate (gos_lock_t* pLock);

/**
 * @brief	This function waits for a lock instance to
 * 			be available and gets it.
 * @details	Checks the lock ID and registers the waiter in the next free
 * 			element of the waiter task array. The waiter task will be blocked
 * 			if the lock is not available.
 *
 * @return	Result of lock get.
 *
 * @retval	GOS_SUCCESS : Lock get successful.
 * @retval	GOS_ERROR   : Invalid or unused lock.
 */
gos_result_t gos_lockWaitGet (gos_lock_t lock);

/**
 * @brief	This function releases a lock instance.
 * @details	Sets the state of the given lock instance to unlocked and
 * 			loops through the waiter array and unblocks the first waiter
 * 			task of the given lock (if there are any).
 *
 * @return	Result of lock release.
 *
 * @retval	GOS_SUCCESS : Lock released successfully.
 * @retval	GOS_ERROR   : Invalid or unused lock.
 */
gos_result_t gos_lockRelease (gos_lock_t lock);
#endif
