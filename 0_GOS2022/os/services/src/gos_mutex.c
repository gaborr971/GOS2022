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
//                                      (c) Ahmed Gazar, 2023
//
//*************************************************************************************************
//! @file       gos_mutex.c
//! @author     Ahmed Gazar
//! @date       2023-10-31
//! @version    1.4
//!
//! @brief      GOS mutex service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_mutex.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-05-04    Ahmed Gazar     Initial version created
// 1.1        2023-05-15    Ahmed Gazar     *    Mutex lock timeout check moved to end of loop
// 1.2        2023-06-30    Ahmed Gazar     +    MUTEX_LOCK_SLEEP_MS added
//                                          +    Mutex unlock current task check added so that
//                                               only the owner can unlock the mutex
// 1.3        2023-09-14    Ahmed Gazar     *    gos_mutexInit return value modified
// 1.4        2023-10-31    Ahmed Gazar     *    Lock and unlock functions reworked
//                                          -    Priority inheritance temporarily removed
//*************************************************************************************************
//
// Copyright (c) 2023 Ahmed Gazar
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
#include <gos_mutex.h>

/*
 * Macros
 */
/**
 * Sleep time in [ms] when trigger timeout is set to endless.
 */
#define MUTEX_LOCK_SLEEP_MS ( 5u )

/*
 * Function: gos_mutexInit
 */
gos_result_t gos_mutexInit (gos_mutex_t* pMutex)
{
    /*
     * Local variables.
     */
    gos_result_t mutexInitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pMutex != NULL)
    {
        pMutex->mutexState = GOS_MUTEX_UNLOCKED;
        pMutex->owner      = GOS_INVALID_TASK_ID;
        mutexInitResult    = GOS_SUCCESS;
    }
    else
    {
        // Nothing to do.
    }

    return mutexInitResult;
}

/*
 * Function: gos_mutexLock
 */
GOS_INLINE gos_result_t gos_mutexLock (gos_mutex_t* pMutex, u32_t timeout)
{
    /*
     * Local variables.
     */
    gos_result_t lockResult   = GOS_ERROR;
    u32_t        sysTickStart = gos_kernelGetSysTicks();

    /*
     * Function code.
     */
    while ((pMutex != NULL) && (((gos_kernelGetSysTicks() - sysTickStart) <= timeout) || (timeout == GOS_MUTEX_ENDLESS_TMO)))
    {
        GOS_ATOMIC_ENTER

        if (pMutex->mutexState == GOS_MUTEX_UNLOCKED)
        {
            pMutex->mutexState = GOS_MUTEX_LOCKED;
            gos_taskGetCurrentId(&(pMutex->owner));
            lockResult = GOS_SUCCESS;
        }
        else
        {
            // Mutex is locked.
        }

        GOS_ATOMIC_EXIT

        if (lockResult == GOS_SUCCESS)
        {
            break;
        }
        else
        {
            gos_taskSleep(MUTEX_LOCK_SLEEP_MS);
        }
    }

    return lockResult;
}

/*
 * Function: gos_mutexUnlock
 */
GOS_INLINE gos_result_t gos_mutexUnlock (gos_mutex_t* pMutex)
{
    /*
     * Local variables.
     */
    gos_result_t unlockResult = GOS_ERROR;
    gos_tid_t    currentTask  = GOS_INVALID_TASK_ID;

    /*
     * Function code.
     */
    if (pMutex != NULL)
    {
        GOS_ATOMIC_ENTER

        gos_taskGetCurrentId(&currentTask);

        if (pMutex->owner == currentTask)
        {
            pMutex->mutexState = GOS_MUTEX_UNLOCKED;
            pMutex->owner      = GOS_INVALID_TASK_ID;
            unlockResult       = GOS_SUCCESS;
        }
        else
        {
            // Only the owner can unlock it.
        }

        GOS_ATOMIC_EXIT
    }
    else
    {
        // Null pointer.
    }

    return unlockResult;
}
