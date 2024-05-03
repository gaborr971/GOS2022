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
//! @file       app_led.c
//! @author     Ahmed Gazar
//! @date       2024-05-03
//! @version    1.0
//!
//! @brief      LED handler component source.
//! @details    This component contains the initializer and task function for the user LED handling.
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
#include <drv_gpio.h>
#include <cfg_def.h>

/*
 * Macros
 */
/**
 * LED task sleep time in [ms].
 */
#define LED_TASK_SLEEP_TIME_MS ( 1000u )

/*
 * Static function prototypes
 */
GOS_STATIC void_t APP_LedTask (void_t);

/**
 * LED task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t ledTaskDesc =
{
	.taskFunction       = APP_LedTask,
	.taskStackSize      = 0x200,
	.taskPriority       = 0,
	.taskName           = "app_led_task",
	.taskPrivilegeLevel = GOS_TASK_PRIVILEGE_USER
};

/*
 * LED initializer
 *
 * Registers the LED task in the scheduler.
 */
gos_result_t APP_LedInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t ledInitResult = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	ledInitResult = gos_taskRegister(&ledTaskDesc, NULL);

	return ledInitResult;
}

/*
 * LED task
 *
 * Every 1 second, it toggles the user LED.
 */
GOS_STATIC void_t APP_LedTask (void_t)
{
	/*
	 * Function code.
	 */
	for (;;)
	{
		(void_t) drv_gpioTgglePin(IO_USER_LED);
		(void_t) gos_taskSleep(LED_TASK_SLEEP_TIME_MS);
	}
}
