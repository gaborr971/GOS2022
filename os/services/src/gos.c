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
//                                      (c) Gabor Repasi, 2022
//
//*************************************************************************************************
//! @file       gos.c
//! @author     Gabor Repasi
//! @date       2022-12-11
//! @version    1.3
//!
//! @brief      GOS source.
//! @details    For a more detailed description of this service, please refer to @ref gos.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-31    Gabor Repasi    Initial version created.
// 1.1        2022-11-14    Gabor Repasi    +    Init error flag setting modified.
// 1.2        2022-11-15    Gabor Repasi    +    License added
// 1.3        2022-12-11    Gabor Repasi    +    main function added here
//                                          *    gos_Init and gos_Start made local static functions
//                                          +    platform and user initializers added
//*************************************************************************************************
//
// Copyright (c) 2022 Gabor Repasi
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
 * Initialization error flag.
 */
GOS_STATIC bool_t initError;

/*
 * Function prototypes
 */
GOS_STATIC gos_result_t gos_Init  (void_t);
GOS_STATIC gos_result_t gos_Start (void_t);

/*
 * Main
 */
int main (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t platformDriverInitResult;

    /*
     * Function code.
     */
    GOS_DISABLE_SCHED
    // Initialize platform drivers.
    platformDriverInitResult = gos_platformDriverInit();

    // Print startup logo.
    gos_printStartupLogo();

    // Pre-initialize.
    (void_t) gos_traceInit("Platform driver initialization", platformDriverInitResult);

    // Initialize OS.
    (void_t) gos_traceInit("OS initialization", gos_Init());


    // Call user initializer.
    (void_t) gos_traceInit("User application initialization", gos_userApplicationInit());

    // Start OS.
    (void_t) gos_Start();

    (void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "Kernel could not be started.");

    for(;;);
}

/*
 * Function: gos_platformDriverInit
 */
__attribute__((weak)) gos_result_t gos_platformDriverInit (void_t)
{
    /*
     * Function code.
     */
    (void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "Platform driver initializer missing!");
    return GOS_ERROR;
}

/*
 * Function: gos_userApplicationInit
 */
__attribute__((weak)) gos_result_t gos_userApplicationInit (void_t)
{
    /*
     * Function code.
     */
    (void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "User application initializer missing!");
    return GOS_ERROR;
}

/**
 * @brief   OS initializer function.
 * @details Calls the necessary driver and service initializer functions.
 *
 * @return  Result of OS initialization.
 *
 * @retval  GOS_SUCCESS : OS initialization successful.
 * @retval  GOS_ERROR   : At least one driver or service initialization failed.
 */
GOS_STATIC gos_result_t gos_Init (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t initStatus = GOS_SUCCESS;

    /*
     * Function code.
     */
    initStatus &= gos_traceInit("Kernel initialization", gos_kernelInit());
    initStatus &= gos_traceInit("Lock service initialization", gos_lockInit());
    initStatus &= gos_traceInit("Queue service initialization", gos_queueInit());
    initStatus &= gos_traceInit("Log service initialization", gos_logInit());
    initStatus &= gos_traceInit("Signal service initialization", gos_signalInit());
#if CFG_PROC_USE_SERVICE == 1
    initStatus &= gos_traceInit("Process service initialization", gos_procInit());
#endif
    initStatus &= gos_traceInit("Time service initialization", gos_timeInit());
#if CFG_SHELL_USE_SERVICE == 1
    initStatus &= gos_traceInit("Shell service initialization", gos_shellInit());
#endif
    initStatus &= gos_traceInit("Message service initialization", gos_messageInit());
    initStatus &= gos_gcpInit();

    if (initStatus != GOS_SUCCESS)
    {
        initStatus = GOS_ERROR;
        initError = GOS_TRUE;
    }
    else
    {
        initError = GOS_FALSE;
    }

    return initStatus;
}

/**
 * @brief   Starts the OS.
 * @details Checks whether the initializer function has set the error flag to GOS_FALSE,
 *          and if so, it starts the kernel (and thus the scheduling of tasks).
 *
 * @return  Result of OS starting.
 *
 * @retval  GOS_ERROR : OS not started due to initialization error or kernel start error.
 *
 * @remark  This function should only return with error. If the initialization is successful,
 *          the function is not expected to return.
 */
GOS_STATIC gos_result_t gos_Start (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t startStatus = GOS_ERROR;

    /*
     * Function code.
     */
    if (initError == GOS_FALSE)
    {
        startStatus = gos_kernelStart();
    }

    return startStatus;
}
