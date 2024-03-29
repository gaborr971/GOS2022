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
//! @file       driver.c
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      Driver source.
//! @details    For a more detailed description of this module, please refer to @ref driver.h
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
 * Includes.
 */
#include "driver.h"
#include "gpio_driver.h"
#include "iodef.h"
#include "timer_driver.h"
#include "rcc_driver.h"
#include "uart_driver.h"

/*
 * Function prototypes.
 */
GOS_STATIC void_t sysTickHook (void_t);

/*
 * Function: driver_init
 */
gos_result_t driver_init (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t driverInitResult = GOS_SUCCESS;
    u8_t         ioIdx            = 0u;

    /*
     * Function code.
     */
    driverInitResult &= rcc_driverInit();
    driverInitResult &= timer_driverSystimerInit();
    driverInitResult &= gpio_driverInit();

    for (ioIdx = 0u; ioIdx < IO_NUMBER; ioIdx++)
    {
        gpio_driverInitPin(iodef_get(ioIdx));
    }

    driverInitResult &= uart_driverInit();
    driverInitResult &= gos_kernelRegisterSysTickHook(sysTickHook);

    if (driverInitResult != GOS_SUCCESS)
    {
        driverInitResult = GOS_ERROR;
    }

    return driverInitResult;
}

/**
 * @brief   System tick hook function.
 * @details Increments the HAL tick.
 *
 * @return  -
 */
GOS_STATIC void_t sysTickHook (void_t)
{
    HAL_IncTick();
}
