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
//! @date       2023-06-30
//! @version    1.2
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
#define MUTEX_LOCK_SLEEP_MS ( 10u )

/*
 * Function: gos_mutexInit
 */
void_t gos_mutexInit (gos_mutex_t* pMutex)
{
    /*
     * Function code.
     */
    pMutex->mutexState = GOS_MUTEX_UNLOCKED;
    pMutex->owner      = GOS_INVALID_TASK_ID;
}

/*
 * Function: gos_mutexLock
 */
GOS_INLINE gos_result_t gos_mutexLock (gos_mutex_t* pMutex, u32_t timeout)
{
    /*
     * Local variables.
     */
    bool_t                   isLocked        = GOS_TRUE;
    gos_result_t             mutexLockResult = GOS_ERROR;
    u32_t                    sysTickInitial  = 0u;
    gos_taskPrivilegeLevel_t privileges      = 0u;
#if CFG_USE_PRIO_INHERITANCE
    gos_taskPrio_t           ownerPrio       = CFG_PROC_MAX_PRIO_LEVELS;
    gos_taskPrio_t           currentPrio     = CFG_PROC_MAX_PRIO_LEVELS;
    gos_tid_t                currentId       = GOS_INVALID_TASK_ID;
#endif

    /*
     * Function code.
     */
    sysTickInitial = gos_kernelGetSysTicks();

#if CFG_USE_PRIO_INHERITANCE
    (void_t) gos_kernelTaskGetCurrentId(&currentId);
    (void_t) gos_kernelTaskGetPriority(currentId, &currentPrio);
    (void_t) gos_kernelTaskGetPriority(pMutex->owner, &ownerPrio);
#endif

    // Wait for the mutex to become available
    while (isLocked == GOS_TRUE)
    {
        // Check if the mutex is locked
        GOS_ATOMIC_ENTER
        if (pMutex->mutexState == GOS_MUTEX_UNLOCKED)
        {
            pMutex->mutexState = GOS_MUTEX_LOCKED;
            (void_t) gos_kernelTaskGetCurrentId((gos_tid_t*)&(pMutex->owner));
            isLocked = GOS_FALSE;
            mutexLockResult = GOS_SUCCESS;
        }
#if CFG_USE_PRIO_INHERITANCE
        else
        {
            if (ownerPrio > currentPrio)
            {
                (void_t) gos_kernelTaskGetPrivileges(currentId, &privileges);

                if ((privileges & GOS_PRIV_TASK_PRIO_CHANGE) != GOS_PRIV_TASK_PRIO_CHANGE)
                {
                    (void_t) gos_kernelTaskAddPrivilege(currentId, GOS_PRIV_TASK_PRIO_CHANGE);
                    if (gos_kernelTaskSetPriority(pMutex->owner, currentPrio) == GOS_SUCCESS)
                    {
                        ownerPrio = currentPrio;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                    (void_t) gos_kernelTaskRemovePrivilege(currentId, GOS_PRIV_TASK_PRIO_CHANGE);
                }
                else
                {
                    if (gos_kernelTaskSetPriority(pMutex->owner, currentPrio) == GOS_SUCCESS)
                    {
                        ownerPrio = currentPrio;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
            }
            else
            {
                // Nothing to do.
            }
        }
#endif
        GOS_ATOMIC_EXIT

        if (isLocked == GOS_TRUE)
        {
            gos_kernelTaskBlock(currentId, MUTEX_LOCK_SLEEP_MS);
        }
        else
        {
            // Nothing to do.
        }

        if ((timeout != GOS_MUTEX_ENDLESS_TMO) &&
            ((gos_kernelGetSysTicks() - sysTickInitial) >= timeout))
        {
            break;
        }
        else
        {
            // Nothing to do.
        }
    }

    return mutexLockResult;
}

/*
 * Function: gos_mutexUnlock
 */
GOS_INLINE void_t gos_mutexUnlock (gos_mutex_t* pMutex)
{
    /*
     * Local variables.
     */
    gos_tid_t                currentTaskId     = GOS_INVALID_TASK_ID;
    gos_taskPrivilegeLevel_t privileges        = 0u;
#if CFG_USE_PRIO_INHERITANCE
    gos_taskPrio_t           ownerOriginalPrio = GOS_TASK_MAX_PRIO_LEVELS;
    gos_taskPrio_t           ownerCurrentPrio  = GOS_TASK_MAX_PRIO_LEVELS;
#endif
    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER

#if CFG_USE_PRIO_INHERITANCE
    (void_t) gos_kernelTaskGetOriginalPriority(pMutex->owner, &ownerOriginalPrio);
    (void_t) gos_kernelTaskGetPriority(pMutex->owner, &ownerCurrentPrio);
    (void_t) gos_kernelTaskGetCurrentId(&currentTaskId);

    if (ownerCurrentPrio != ownerOriginalPrio)
    {
        (void_t) gos_kernelTaskGetPrivileges(currentTaskId, &privileges);

        if ((privileges & GOS_PRIV_TASK_PRIO_CHANGE) != GOS_PRIV_TASK_PRIO_CHANGE)
        {
            (void_t) gos_kernelTaskAddPrivilege(currentTaskId, GOS_PRIV_TASK_PRIO_CHANGE);
            (void_t) gos_kernelTaskSetPriority(pMutex->owner, ownerOriginalPrio);
            (void_t) gos_kernelTaskRemovePrivilege(currentTaskId, GOS_PRIV_TASK_PRIO_CHANGE);
        }
        else
        {
            (void_t) gos_kernelTaskSetPriority(pMutex->owner, ownerOriginalPrio);
        }
    }
    else
    {
        // Nothing to do.
    }
#endif

    // Only owner can release the mutex.
    if (currentTaskId == pMutex->owner)
    {
        pMutex->mutexState = GOS_MUTEX_UNLOCKED;
        pMutex->owner      = GOS_INVALID_TASK_ID;
    }
    else
    {
        // Nothing to do.
    }

    GOS_ATOMIC_EXIT
}
