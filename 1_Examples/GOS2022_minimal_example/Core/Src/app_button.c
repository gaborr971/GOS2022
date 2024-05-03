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
//                                      (c) Ahmed Gazar, 2024
//
//*************************************************************************************************
//! @file       app_button.c
//! @author     Ahmed Gazar
//! @date       2024-05-03
//! @version    1.0
//!
//! @brief      Button handler component source.
//! @details    This component contains the initializer, interrupt handler, and task function
//!             for the user button handling.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-05-03    Ahmed Gazar     Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2024 Ahmed Gazar
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
#include "gos.h"

/*
 * Static variables
 */
/**
 * Button trigger for synchronizing button task.
 */
GOS_STATIC gos_trigger_t buttonTrigger;

/**
 * Button blocked flag for software button-debouncing.
 */
GOS_STATIC bool_t        buttonBlocked;

/*
 * Static function prototypes
 */
GOS_STATIC void_t APP_ButtonTask (void_t);

/**
 * Button task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t buttonTaskDesc =
{
    .taskFunction       = APP_ButtonTask,
    .taskStackSize      = 0x400,
    .taskPriority       = 2,
    .taskName           = "app_button_task",
    .taskPrivilegeLevel = GOS_TASK_PRIVILEGED_USER
};

/*
 * Button initializer
 *
 * Registers the button task in the scheduler and initializes
 * the button trigger.
 */
gos_result_t APP_ButtonInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t buttonInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (gos_taskRegister(&buttonTaskDesc, NULL) == GOS_SUCCESS &&
		gos_triggerInit(&buttonTrigger) == GOS_SUCCESS)
	{
		buttonInitResult = GOS_SUCCESS;
	}
	else
	{
		// Error.
	}

	return buttonInitResult;
}

/*
 * Button pressed interrupt callback
 *
 * Called by HW interrupt.
 * Only effective when the debounce-flag is not set.
 * It sets the debounce-flag and increments the button trigger.
 */
void_t APP_ButtonPressedInterruptCallback (void_t)
{
    /*
     * Function code.
     */
    if (buttonBlocked == GOS_FALSE)
    {
        buttonBlocked = GOS_TRUE;
        (void_t) gos_triggerIncrement(&buttonTrigger);
    }
    else
    {
    	// Button is blocked (software debouncing).
    }
}

/*
 * Button task
 *
 * Waits for a button pressed trigger with endless timeout (asynchronous external event,
 * with no other activities in the meantime).
 * When the trigger is received, it resets it, traces a message on the trace output,
 * invokes a kernel dump, and resets the debounce-flag.
 */
GOS_STATIC void_t APP_ButtonTask (void_t)
{
    /*
     * Function code.
     */
    buttonBlocked = GOS_FALSE;

    for (;;)
    {
        if (gos_triggerWait(&buttonTrigger, 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS)
        {
            gos_triggerReset(&buttonTrigger);
            (void_t) gos_traceTrace(GOS_TRUE, "User button pressed.\r\n");
            gos_kernelDump();
            buttonBlocked = GOS_FALSE;
        }
        else
        {
            gos_errorHandler(GOS_ERROR_LEVEL_USER_FATAL, NULL, 0, "Button task trigger error.");
        }
    }
}
