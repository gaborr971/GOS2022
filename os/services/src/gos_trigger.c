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
//! @date       2023-01-11
//! @version    1.0
//!
//! @brief      GOS trigger service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_trigger.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-01-11    Gabor Repasi    Initial version created
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
#include <gos_lock.h>
#include <gos_trigger.h>

/*
 * Macros
 */
/**
 * Trigger daemon poll time in [ms].
 */
#define GOS_TRIGGER_DAEMON_POLL_TIME_MS    ( 5u )

/**
 * Default trigger ID.
 */
#define GOS_DEFAULT_TRIGGER_ID             ( 0x6000 )

/*
 * Type definitions
 */
/**
 * Trigger waiter descriptor type.
 */
typedef struct
{
    gos_tid_t            waiterTaskId;        //!< Waiter task ID.
    gos_triggerId_t      triggerId;           //!< Trigger ID.
    u32_t                desiredTriggerValue; //!< Desired trigger value.
}gos_triggerWaiterDesc_t;

/**
 * Trigger descriptor type.
 */
typedef struct
{
	gos_triggerId_t      triggerId;           //!< Trigger ID.
    u32_t                triggerValueCounter; //!< Trigger value counter.
}gos_trigger_t;

/*
 * Static variables
 */
/**
 * Internal trigger array.
 */
GOS_STATIC gos_trigger_t            triggerArray       [CFG_TRIGGER_MAX_NUMBER] = {0};

/**
 * Internal trigger waiter array.
 */
GOS_STATIC gos_triggerWaiterDesc_t  triggerWaiterArray [CFG_TRIGGER_MAX_WAITERS] = {0};

/*
 * Function: gos_triggerInit
 */
gos_result_t gos_triggerInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t             triggerInitResult  = GOS_SUCCESS;
    gos_triggerWaiterIndex_t triggerWaiterIndex = 0u;

    /*
     * Function code.
     */
    for (triggerWaiterIndex = 0u; triggerWaiterIndex < CFG_TRIGGER_MAX_WAITERS; triggerWaiterIndex++)
    {
        triggerWaiterArray[triggerWaiterIndex].waiterTaskId        = GOS_INVALID_TASK_ID;
        triggerWaiterArray[triggerWaiterIndex].triggerId           = GOS_TRIGGER_INVALID_ID;
        triggerWaiterArray[triggerWaiterIndex].desiredTriggerValue = 0u;
    }

    return triggerInitResult;
}

/*
 * Function: gos_triggerCreate
 */
gos_result_t gos_triggerCreate (gos_triggerId_t* pTriggerId)
{
    /*
     * Local variables.
     */
    gos_result_t       triggerCreateResult = GOS_ERROR;
    gos_triggerIndex_t triggerIndex        = 0u;

    /*
     * Function code.
     */
    if (pTriggerId != NULL)
    {
        // Find the next unused lock and reserve it.
        for (triggerIndex = 0u; triggerIndex < CFG_TRIGGER_MAX_NUMBER; triggerIndex++)
        {
            if (triggerArray[triggerIndex].triggerId == GOS_TRIGGER_INVALID_ID)
            {
                triggerArray[triggerIndex].triggerId = GOS_DEFAULT_TRIGGER_ID + triggerIndex;
                *pTriggerId                          = triggerArray[triggerIndex].triggerId;
                triggerCreateResult                  = GOS_SUCCESS;
                break;
            }
        }
    }
    return triggerCreateResult;
}

/*
 * Function: gos_triggerIncrement
 */
GOS_INLINE gos_result_t gos_triggerIncrement (gos_triggerId_t triggerId)
{
    /*
     * Local variables.
     */
	gos_result_t             triggerIncreaseResult = GOS_ERROR;
	gos_triggerIndex_t       triggerIndex = 0u;
	gos_triggerWaiterIndex_t triggerWaiterIndex = 0u;

    /*
     * Function code.
     */
	triggerIndex = triggerId - GOS_DEFAULT_TRIGGER_ID;

	if (triggerIndex < CFG_TRIGGER_MAX_NUMBER &&
		triggerArray[triggerIndex].triggerId != GOS_TRIGGER_INVALID_ID)
	{
		GOS_DISABLE_SCHED

		triggerArray[triggerIndex].triggerValueCounter++;
		triggerIncreaseResult = GOS_SUCCESS;

        for (triggerWaiterIndex = 0u; triggerWaiterIndex < CFG_TRIGGER_MAX_WAITERS; triggerWaiterIndex++)
        {
        	if (triggerWaiterArray[triggerWaiterIndex].triggerId != GOS_TRIGGER_INVALID_ID &&
        		triggerArray[triggerIndex].triggerValueCounter == triggerWaiterArray[triggerWaiterIndex].desiredTriggerValue)
        	{
        		triggerWaiterArray[triggerWaiterIndex].triggerId           = GOS_TRIGGER_INVALID_ID;
        		triggerArray[triggerIndex].triggerValueCounter             = 0u;
        		triggerWaiterArray[triggerWaiterIndex].desiredTriggerValue = 0u;

        		GOS_PRIVILEGED_ACCESS
        		(void_t) gos_kernelTaskUnblock(triggerWaiterArray[triggerWaiterIndex].waiterTaskId);

        		triggerWaiterArray[triggerWaiterIndex].waiterTaskId = GOS_INVALID_TASK_ID;
        	}
        }
        GOS_ENABLE_SCHED
	}

	return triggerIncreaseResult;
}

/*
 * Function: gos_triggerWait
 */
GOS_INLINE gos_result_t gos_triggerWait (gos_triggerId_t triggerId, u32_t value)
{
    /*
     * Local variables.
     */
    gos_result_t             triggerWaitResult  = GOS_ERROR;
    gos_tid_t                currentTaskId      = GOS_INVALID_TASK_ID;
    gos_triggerIndex_t       triggerIndex       = 0u;
    gos_triggerWaiterIndex_t triggerWaiterIndex = 0u;
    bool_t                   schedEnabled       = GOS_FALSE;

    /*
     * Function code.
     */
    GOS_DISABLE_SCHED
	triggerIndex = triggerId - GOS_DEFAULT_TRIGGER_ID;

	if (gos_kernelTaskGetCurrentId(&currentTaskId) == GOS_SUCCESS &&
		triggerIndex < CFG_TRIGGER_MAX_NUMBER &&
		triggerArray[triggerIndex].triggerId != GOS_TRIGGER_INVALID_ID)
	{
		if (triggerArray[triggerIndex].triggerValueCounter == value)
		{
			triggerArray[triggerIndex].triggerValueCounter = 0u;
			triggerWaitResult = GOS_SUCCESS;
		}
		else
		{
			for (triggerWaiterIndex = 0u; triggerWaiterIndex < CFG_TRIGGER_MAX_WAITERS; triggerWaiterIndex++)
			{
				if (triggerWaiterArray[triggerWaiterIndex].triggerId    == GOS_TRIGGER_INVALID_ID &&
					triggerWaiterArray[triggerWaiterIndex].waiterTaskId == GOS_INVALID_TASK_ID)
				{
					triggerWaiterArray[triggerWaiterIndex].triggerId           = triggerId;
					triggerWaiterArray[triggerWaiterIndex].desiredTriggerValue = value;
					triggerWaiterArray[triggerWaiterIndex].waiterTaskId        = currentTaskId;

		            // Block task.
					GOS_ENABLE_SCHED
		            GOS_PRIVILEGED_ACCESS
		            (void_t) gos_kernelTaskBlock(currentTaskId);

		            // Task unblocked.
		            triggerWaitResult = GOS_SUCCESS;

		            schedEnabled = GOS_TRUE;

		            break;
				}
			}
		}
	}

    // Enable scheduling.
	if (schedEnabled == GOS_FALSE)
	{
		GOS_ENABLE_SCHED
	}

    return triggerWaitResult;
}
