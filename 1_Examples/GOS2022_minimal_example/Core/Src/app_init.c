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
//! @file       app_init.c
//! @author     Ahmed Gazar
//! @date       2024-05-03
//! @version    1.0
//!
//! @brief      Application initialization source.
//! @details    This component contains the platform and application level initializer function
//!             implementations (OS weak functions).
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
#include <drv_shell.h>
#include <drv_trace.h>
#include "driver.h"
#include "timer_driver.h"
#include "stm32f4xx.h"

/*
 * Extern functions
 */
GOS_EXTERN gos_result_t APP_LedInit    (void_t);
GOS_EXTERN gos_result_t APP_ButtonInit (void_t);
GOS_EXTERN gos_result_t APP_CpuInit    (void_t);

/*
 * Function: gos_platformDriverInit
 *
 * This function is defined as weak in the OS and shall be redefined by the user
 * as seen here.
 *
 * This is to initialize the low-level drivers required by the OS, and it is called
 * during the OS startup.
 */
gos_result_t gos_platformDriverInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t platformDriverInitResult = GOS_SUCCESS;

    /*
     * These are the implementations of the necessary driver functions that
     * match the skeletons defined by the kernel driver functions.
     */
    GOS_STATIC gos_driver_functions_t driverFunctions =
    {
		.traceDriverTransmitString       = drv_traceTransmit,
		.traceDriverTransmitStringUnsafe = drv_traceTransmitUnsafe,
		.shellDriverTransmitString       = drv_shellTransmitString,
		.shellDriverReceiveChar          = drv_shellReceiveChar,
		.timerDriverSysTimerGetValue     = timer_driverSystimerGet,
    };

    /*
     * Function code.
     */
    // Low-level initialization.
    if (HAL_Init() != HAL_OK)
    {
        platformDriverInitResult = GOS_ERROR;
    }

    // Driver initialization.
    platformDriverInitResult = driver_init();

    // Register kernel drivers.
    platformDriverInitResult &= gos_driverInit(&driverFunctions);

    if (platformDriverInitResult != GOS_SUCCESS)
    {
        platformDriverInitResult = GOS_ERROR;
    }

    // Systick initialization.
    SysTick->VAL = 0;
    SysTick->CTRL = 0b111;

    __enable_irq();

    return platformDriverInitResult;
}

/*
 * Function: gos_userApplicationInit
 *
 * This function is defined as weak in the OS and shall be redefined by the user
 * as seen here.
 *
 * This is to initialize the user application before the kernel starts the scheduling.
 * This can be used to register user tasks and initialize application modules.
 */
gos_result_t gos_userApplicationInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t appInitResult = GOS_SUCCESS;

    /*
     * Function code.
     */
    // Register tasks.
    appInitResult &= gos_errorTraceInit("LED task initialization",    APP_LedInit());
    appInitResult &= gos_errorTraceInit("Button task initialization", APP_ButtonInit());
    appInitResult &= gos_errorTraceInit("CPU task initialization",    APP_CpuInit());

    if (appInitResult != GOS_SUCCESS)
    {
        appInitResult = GOS_ERROR;
    }
    else
    {
    	// Error.
    }

    return appInitResult;
}
