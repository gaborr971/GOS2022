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
//! @date       2023-07-12
//! @version    2.3
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
 * Macros
 */
/**
 * Block time in [ms] when trigger timeout is set to endless.
 */
#define TRIGGER_WAIT_BLOCK_MS ( 10u )

/*
 * Function: gos_triggerReset
 */
GOS_INLINE void_t gos_triggerReset (gos_trigger_t* pTrigger)
{
    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER

    pTrigger->triggerValueCounter = 0u;

    GOS_ATOMIC_EXIT
}

/*
 * Function: gos_triggerWait
 */
GOS_INLINE gos_result_t gos_triggerWait (gos_trigger_t* pTrigger, u32_t value, u32_t timeout)
{
    /*
     * Local variables.
     */
    bool_t       isTriggerReached  = GOS_FALSE;
    gos_result_t triggerWaitResult = GOS_ERROR;
    u32_t        sysTickInitial    = 0u;
    gos_tid_t    currentId         = GOS_INVALID_TASK_ID;

    /*
     * Function code.
     */
    sysTickInitial = gos_kernelGetSysTicks();

    GOS_ATOMIC_ENTER
    pTrigger->numOfWaiters++;
    (void_t) gos_kernelTaskGetCurrentId(&currentId);
    GOS_ATOMIC_EXIT

    while (isTriggerReached == GOS_FALSE)
    {
        // Check if the trigger value is reached
        GOS_ATOMIC_ENTER
        if (pTrigger->triggerValueCounter >= value)
        {
            isTriggerReached = GOS_TRUE;
            triggerWaitResult = GOS_SUCCESS;
            pTrigger->numOfWaiters--;
        }
        else
        {
        	// Nothing to do.
        }
        GOS_ATOMIC_EXIT

        if (isTriggerReached == GOS_FALSE)
        {
            (void_t) gos_kernelTaskBlock(currentId, TRIGGER_WAIT_BLOCK_MS);
        }
        else
        {
        	// Nothing to do.
        }

        if ((timeout != GOS_TRIGGER_ENDLESS_TMO) &&
        	((gos_kernelGetSysTicks() - sysTickInitial) >= timeout))
        {
            break;
        }
        else
        {
        	// Nothing to do.
        }
    }

    return triggerWaitResult;
}

/*
 * Function: gos_triggerIncrement
 */
GOS_INLINE void_t gos_triggerIncrement (gos_trigger_t* pTrigger)
{
    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER

    pTrigger->triggerValueCounter++;

    GOS_ATOMIC_EXIT
}
