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
//! @file       drv_trace.c
//! @author     Ahmed Gazar
//! @date       2024-03-15
//! @version    1.0
//!
//! @brief      GOS2022 Library / Trace driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_trace.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-03-15    Ahmed Gazar     Initial version created.
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
#include <drv_trace.h>
#include <drv_uart.h>
#include <string.h>

/*
 * External variables
 */
/**
 * UART service configuration (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_uartPeriphInstance_t uartServiceConfig [];

/**
 * UART service timeout configuration.
 */
GOS_EXTERN drv_uartServiceTimeoutConfig_t     uartServiceTmoConfig;

/*
 * Function: drv_traceTransmitUnsafe
 */
GOS_INLINE gos_result_t drv_traceTransmitUnsafe (char_t* pMessage)
{
    /*
     * Local variables.
     */
    gos_result_t             uartTransmitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_TRACE_INSTANCE];

        uartTransmitResult = drv_uartTransmitBlocking(instance, (u8_t*)pMessage, strlen(pMessage),
                uartServiceTmoConfig.traceUnsafeTransmitTmo);
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartTransmitResult;
}

/*
 * Function: drv_traceTransmit
 */
GOS_INLINE gos_result_t drv_traceTransmit (char_t* pMessage)
{
    /*
     * Local variables.
     */
    gos_result_t             uartTransmitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_TRACE_INSTANCE];

        uartTransmitResult = drv_uartTransmitDMA(instance, (u8_t*)pMessage, strlen(pMessage),
                uartServiceTmoConfig.traceMutexTmo, uartServiceTmoConfig.traceTriggerTmo);
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartTransmitResult;
}
