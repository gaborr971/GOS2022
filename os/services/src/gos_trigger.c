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
//                                      (c) Gabor Repasi, 2023
//
//*************************************************************************************************
//! @file       gos_trigger.c
//! @author     Gabor Repasi
//! @date       2023-05-04
//! @version    2.0
//!
//! @brief      GOS trigger service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_trigger.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-01-11    Gabor Repasi    Initial version created
// 2.0        2023-05-04    Gabor Repasi    Service completely reworked
//*************************************************************************************************
//
// Copyright (c) 2023 Gabor Repasi
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

    /*
     * Function code.
     */
    sysTickInitial = gos_kernelGetSysTicks();

    GOS_ATOMIC_ENTER
    pTrigger->numOfWaiters++;
    GOS_ATOMIC_EXIT

    while (isTriggerReached == GOS_FALSE)
    {
        if ((timeout != GOS_TRIGGER_ENDLESS_TMO)
            && ((sysTickInitial - gos_kernelGetSysTicks()) >= timeout))
        {
            break;
        }

        // Check if the trigger value is reached
        GOS_ATOMIC_ENTER
        if (pTrigger->triggerValueCounter >= value)
        {
            isTriggerReached = GOS_TRUE;
            triggerWaitResult = GOS_SUCCESS;
            pTrigger->numOfWaiters--;
        }
        GOS_ATOMIC_EXIT

        if (isTriggerReached == GOS_FALSE)
        {
            if (timeout == GOS_TRIGGER_ENDLESS_TMO)
            {
                gos_kernelTaskSleep(5);
            }
            else
            {
                gos_kernelTaskYield();
            }
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
