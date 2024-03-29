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
//! @file       app.c
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      GOS example application (1).
//! @details    This test application demonstrates some basic features of the OS.
//!             The application consists of the following tasks:
//!             -    Periodic LED blinking
//!             -    Lifesign management with trigger service
//!             -    Mutex use with prevented priority inversion
//!             -    Interrupt handling with trigger service with the user button
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-09-14    Ahmed Gazar     Initial version created.
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
#include <stdio.h>
#include "gpio_driver.h"
#include "iodef.h"
#include "timer_driver.h"
#include "uart_driver.h"
#include "driver.h"

/*
 * Macros
 */
/**
 * Life sign timeout in [ms].
 */
#define APP_LIFE_SIGN_TMO_MS ( 3000u )

/*
 * Function prototypes
 */
/**
 * Task function and interrupt callback declarations.
 */
GOS_STATIC void_t APP_LedTask                        (void_t);
GOS_STATIC void_t APP_LifeSignTask                   (void_t);
GOS_STATIC void_t APP_ButtonTask                     (void_t);
GOS_STATIC void_t APP_ButtonPressedInterruptCallback (void_t);
GOS_STATIC void_t APP_HighPriorityTask               (void_t);
GOS_STATIC void_t APP_MediumPriorityTask             (void_t);
GOS_STATIC void_t APP_LowPriorityTask                (void_t);

/*
 * Static variables
 */
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

/**
 * Life sign task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t lifeSignTaskDesc =
{
    .taskFunction       = APP_LifeSignTask,
    .taskStackSize      = 0x400,
    .taskPriority       = 1,
    .taskName           = "app_lifesign_task",
    .taskPrivilegeLevel = GOS_TASK_PRIVILEGED_USER
};

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

/**
 * High priority task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t highPrioTaskDesc =
{
    .taskFunction        = APP_HighPriorityTask,
    .taskStackSize         = 0x400,
    .taskPriority         = 201,
    .taskName            = "app_high_prio_task",
    .taskPrivilegeLevel    = GOS_TASK_PRIVILEGED_USER
};

/**
 * Medium priority task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t mediumPrioTaskDesc =
{
    .taskFunction        = APP_MediumPriorityTask,
    .taskStackSize         = 0x400,
    .taskPriority         = 230,
    .taskName            = "app_medium_prio_task",
    .taskPrivilegeLevel    = GOS_TASK_PRIVILEGED_USER,
};

/**
 * Low priority task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t lowPrioTaskDesc =
{
    .taskFunction        = APP_LowPriorityTask,
    .taskStackSize         = 0x400,
    .taskPriority         = 254,
    .taskName            = "app_low_prio_task",
    .taskPrivilegeLevel    = GOS_TASK_PRIVILEGED_USER,
};

/**
 * Life sign trigger for synchronizing life sign task.
 */
GOS_STATIC gos_trigger_t lifeSignTrigger;

/**
 * Button trigger for synchronizing button task.
 */
GOS_STATIC gos_trigger_t buttonTrigger;

/**
 * Button blocked flag for software button-debouncing.
 */
GOS_STATIC bool_t        buttonBlocked;

/**
 * Priority mutex.
 */
GOS_STATIC gos_mutex_t   prioMutex;

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
     * These are the implementation of the necessary driver functions that
     * match the skeletons defined by the kernel driver functions.
     */
    GOS_STATIC gos_driver_functions_t driverFunctions =
    {
        .traceDriverTransmitString       = uart_driverTraceTransmitString,
        .traceDriverTransmitStringUnsafe = uart_driverTraceTransmitStringUnsafe,
        .shellDriverTransmitString       = uart_driverShellTransmitString,
        .shellDriverReceiveChar          = uart_driverShellReceiveChar,
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
    appInitResult &= gos_kernelTaskRegister(&ledTaskDesc, NULL);
    appInitResult &= gos_kernelTaskRegister(&lifeSignTaskDesc, NULL);
    appInitResult &= gos_kernelTaskRegister(&buttonTaskDesc, NULL);
    appInitResult &= gos_kernelTaskRegister(&highPrioTaskDesc, NULL);
    appInitResult &= gos_kernelTaskRegister(&mediumPrioTaskDesc, NULL);
    appInitResult &= gos_kernelTaskRegister(&lowPrioTaskDesc, NULL);

    // Initialize triggers.
    gos_triggerInit(&lifeSignTrigger);
    gos_triggerInit(&buttonTrigger);

    // Test user-level warning.
    gos_errorHandler(GOS_ERROR_LEVEL_USER_WARNING, __func__, __LINE__, "Test user-level warning. Starting OS...");

    if (appInitResult != GOS_SUCCESS)
    {
        appInitResult = GOS_ERROR;
    }

    (void_t) gos_errorTraceInit("Application initializetion", appInitResult);

    return appInitResult;
}

/*
 * LED task
 *
 * Every 1 second, it toggles the user LED and increments the life sign trigger
 * to wake up the life sign task before its timeout.
 */
GOS_STATIC void_t APP_LedTask (void_t)
{
    /*
     * Function code.
     */
    for (;;)
    {
        (void_t) gpio_driverTogglePin(iodef_get(IO_USER_LED));
        gos_triggerIncrement(&lifeSignTrigger);
        (void_t) gos_kernelTaskSleep(1000);
    }
}

/*
 * Life sign task
 *
 * Waits for a life sign trigger with timeout. If the trigger is received within
 * timeout, it resets the trigger and prints a message on the trace output.
 * Otherwise, it goes to error state, and resets the CPU.
 */
GOS_STATIC void_t APP_LifeSignTask (void_t)
{
    /*
     * Local variables.
     */
    u32_t lifeSignCounter = 0u;

    /*
     * Function code.
     */
    for (;;)
    {
        // Wait for trigger with timeout.
        if (gos_triggerWait(&lifeSignTrigger, 1, APP_LIFE_SIGN_TMO_MS) == GOS_SUCCESS)
        {
            // If trigger is received within timeout, reset it, and print the
            // life sign counter value on the trace output.
            gos_triggerReset(&lifeSignTrigger);
            (void_t) gos_traceTraceFormatted(GOS_TRUE, "Life sign counter: %u\r\n", lifeSignCounter++);
        }
        else
        {
            // Otherwise go to error and reset.
            gos_errorHandler(GOS_ERROR_LEVEL_USER_FATAL, __func__, __LINE__, "Life sign timeout.");
        }
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
     * Local variables.
     */
    gpio_callback_desc_t buttonDesc =
    {
        .callback = APP_ButtonPressedInterruptCallback,
        .pin = iodef_get(IO_USER_BUTTON)->pin
    };

    /*
     * Function code.
     */
    buttonBlocked = GOS_FALSE;
    (void_t) gpio_driverRegisterItCallback(&buttonDesc);

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
            (void_t) gos_kernelTaskSleep(500);
        }
    }
}

/*
 * Button pressed interrupt callback
 *
 * Called by HW interrupt.
 * Only effective when the debounce-flag is not set.
 * It sets the debounce-flag to active, and increments the trigger.
 */
GOS_STATIC void_t APP_ButtonPressedInterruptCallback (void_t)
{
    /*
     * Function code.
     */
    if (buttonBlocked == GOS_FALSE)
    {
        buttonBlocked = GOS_TRUE;
        gos_triggerIncrement(&buttonTrigger);
    }
}

/*
 * High priority task
 *
 * Locks the mutex, performs a short activity, and unlocks the mutex.
 */
GOS_STATIC void_t APP_HighPriorityTask (void_t)
{
    /*
     * Local variables.
     */
    u32_t time = 0u;

    /*
     * Function code.
     */
    // Initialize mutex.
    (void_t) gos_mutexInit(&prioMutex);

    // Add random delay.
    (void_t) gos_kernelTaskSleep(56);

    for (;;)
    {
        // Take mutex.
        (void_t) gos_traceTrace(GOS_TRUE, "Task H trying to lock the mutex...\r\n");
        time = gos_kernelGetSysTicks();

        if (gos_mutexLock(&prioMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
        {
            // Mutex is taken, print message on the trace output.
            time = gos_kernelGetSysTicks() - time;
            (void_t) gos_traceTraceFormatted(GOS_TRUE, "Task H locked the mutex. Time: %d ms. Do some work now...\r\n", time);

            // Do some work.
            time = gos_kernelGetSysTicks();
            // Work is expected to finish in 10 ms.
            gos_kernelDelayMs(10);
            time = (gos_kernelGetSysTicks() - time);

            // Unlock mutex.
            (void_t) gos_traceTraceFormatted(GOS_TRUE, "Task H unlocking mutex. H had the mutex for: %d ms\r\n", time);
            gos_mutexUnlock(&prioMutex);
        }

        // Task sleep.
        (void_t) gos_kernelTaskSleep(1020);
    }
}

/*
 * Medium priority task
 *
 * Performs a CPU intensive, long activity (active waiting for 1000 ms).
 */
GOS_STATIC void_t APP_MediumPriorityTask (void_t)
{
    /*
     * Local variables.
     */
    u32_t time = 0u;

    /*
     * Function code.
     */
    // Add random delay.
    (void_t) gos_kernelTaskSleep(61);

    for (;;)
    {
        // This task runs when low-priority task has the mutex.
        (void_t) gos_traceTrace(GOS_TRUE, "Medium priority task doing some work...\r\n");

        // Do some time consuming work.
        time = gos_kernelGetSysTicks();
        gos_kernelDelayMs(1000);
        time = gos_kernelGetSysTicks() - time;

        // End of work. Feedback and sleep.
        (void_t) gos_traceTraceFormatted(GOS_TRUE, "Medium prio task finished work. Time taken: %d ms\r\n", time);
        (void_t) gos_kernelTaskSleep(507);
    }
}

/*
 * Low priority task
 *
 * Locks the mutex, performs a long activity, and unlocks the mutex.
 */
GOS_STATIC void_t APP_LowPriorityTask (void_t)
{
    /*
     * Local variables.
     */
    u32_t time = 0u;

    /*
     * Function code.
     */
    for (;;)
    {
        // Take mutex.
        (void_t) gos_traceTrace(GOS_TRUE, "Task L trying to lock the mutex...\r\n");
        time =  gos_kernelGetSysTicks();

        if (gos_mutexLock(&prioMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
        {
            // Mutex is taken, print message on the trace output.
            time = gos_kernelGetSysTicks() - time;
            (void_t) gos_traceTraceFormatted(GOS_TRUE, "Task L got the mutex. Time: %d ms. Do some work now...\r\n", time);

            // Do some work.
            time = gos_kernelGetSysTicks();
            // Work is expected to finish in 1000 ms.
            (void_t) gos_kernelDelayMs(1000);
            time = gos_kernelGetSysTicks() - time;

            // Unlock mutex.
            (void_t) gos_traceTraceFormatted(GOS_TRUE, "Task L unlocking mutex. L had lock for: %d ms\r\n", time);
            gos_mutexUnlock(&prioMutex);
        }

        // Task sleep.
        (void_t) gos_kernelTaskSleep(504);
    }
}
