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
//! @file       gos_trigger.c
//! @author     Ahmed Gazar
//! @date       2023-09-07
//! @version    2.4
//!
//! @brief      GOS trigger service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_trigger.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-01-11    Ahmed Gazar     Initial version created
// 2.0        2023-05-04    Ahmed Gazar     *    Service completely reworked
// 2.1        2023-06-20    Ahmed Gazar     *    Timeout check moved to end of loop
// 2.2        2023-06-30    Ahmed Gazar     +    TRIGGER_WAIT_SLEEP_MS added
// 2.3        2023-07-12    Ahmed Gazar     *    TRIGGER_WAIT_SLEEP_MS renamed to
//                                               TRIGGER_WAIT_BLOCK_MS
//                                          *    Waiting changed from sleep to block
// 2.4        2023-09-07    Ahmed Gazar     *    Timeout handling rework
//                                          -    TRIGGER_WAIT_BLOCK_MS removed
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
#include <gos_trigger.h>

/*
 * Function: gos_mutexInit
 */
void_t gos_triggerInit (gos_trigger_t* pTrigger)
{
    /*
     * Function code.
     */

    // Null pointer check.
    if (pTrigger != NULL)
    {
        gos_mutexInit(&pTrigger->triggerMutex);
    }
    else
    {
        // Nothing to do.
    }
}

/*
 * Function: gos_triggerReset
 */
GOS_INLINE void_t gos_triggerReset (gos_trigger_t* pTrigger)
{
    /*
     * Function code.
     */

    // Null pointer check.
    if (pTrigger != NULL &&
        gos_mutexLock(&pTrigger->triggerMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
    {
        pTrigger->valueCounter = 0u;
        pTrigger->desiredValue = 0u;

        gos_mutexUnlock(&pTrigger->triggerMutex);
    }
    else
    {
        // Nothing to do.
    }
}

/*
 * Function: gos_triggerWait
 */
GOS_INLINE gos_result_t gos_triggerWait (gos_trigger_t* pTrigger, u32_t value, u32_t timeout)
{
    /*
     * Local variables.
     */
    gos_result_t triggerWaitResult = GOS_ERROR;
    gos_tid_t    currentId         = GOS_INVALID_TASK_ID;

    /*
     * Function code.
     */

    // Null pointer check.
    if (pTrigger != NULL)
    {
        // Set owner task ID.
        (void_t) gos_mutexLock(&pTrigger->triggerMutex, GOS_MUTEX_ENDLESS_TMO);
        (void_t) gos_kernelTaskGetCurrentId(&currentId);
        pTrigger->waiterTaskId = currentId;
        pTrigger->desiredValue = value;
        gos_mutexUnlock(&pTrigger->triggerMutex);

        // Block until timeout in case trigger value has not been reached yet.
        if (pTrigger->valueCounter < pTrigger->desiredValue)
        {
            (void_t) gos_kernelTaskBlock(currentId, timeout);
        }
        else
        {
            // Trigger value already reached.
        }

        // Check if the trigger value is reached after unblock.
        (void_t) gos_mutexLock(&pTrigger->triggerMutex, GOS_MUTEX_ENDLESS_TMO);
        if (pTrigger->valueCounter >= pTrigger->desiredValue)
        {
            triggerWaitResult = GOS_SUCCESS;
            pTrigger->waiterTaskId = GOS_INVALID_TASK_ID;
        }
        else
        {
            // Nothing to do.
        }
        gos_mutexUnlock(&pTrigger->triggerMutex);
    }
    else
    {
        // Nothing to do.
    }

    return triggerWaitResult;
}

/*
 * Function: gos_triggerIncrement
 */
GOS_INLINE void_t gos_triggerIncrement (gos_trigger_t* pTrigger)
{
    /*
     * Local variables.
     */
    gos_taskPrivilegeLevel_t originalPrivileges = GOS_TASK_PRIVILEGE_USER;
    gos_tid_t                currentTaskId      = GOS_INVALID_TASK_ID;

    /*
     * Function code.
     */

    // Null pointer check.
    if (pTrigger != NULL)
    {
        (void_t) gos_mutexLock(&pTrigger->triggerMutex, GOS_MUTEX_ENDLESS_TMO);

        // Increment trigger value.
        pTrigger->valueCounter++;

        // If desired value is reached, unblock waiter.
        if (pTrigger->valueCounter >= pTrigger->desiredValue)
        {
            // Get current task ID.
            (void_t) gos_kernelTaskGetCurrentId(&currentTaskId);

            // Get original privileges.
            (void_t) gos_kernelTaskGetPrivileges(currentTaskId, &originalPrivileges);

            // Add necessary privilege.
            (void_t) gos_kernelTaskAddPrivilege(currentTaskId, GOS_PRIV_TASK_MANIPULATE);

            // Unblock owner task.
            (void_t) gos_kernelTaskUnblock(pTrigger->waiterTaskId);

            // Restore privileges.
            (void_t) gos_kernelTaskSetPrivileges(currentTaskId, originalPrivileges);
        }
        else
        {
            // Trigger value not reached yet.
        }

        gos_mutexUnlock(&pTrigger->triggerMutex);
    }
    else
    {
        // Nothing to do.
    }
}
