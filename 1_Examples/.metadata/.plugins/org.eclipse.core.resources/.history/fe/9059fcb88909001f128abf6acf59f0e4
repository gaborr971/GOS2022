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
//! @file       iodef.c
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      IO definition source.
//! @details    For a more detailed description of this module, please refer to @ref iodef.h
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
#include "iodef.h"

/*
 * Static variables.
 */
/**
 * IO definition array.
 */
GOS_CONST GOS_STATIC gpio_desc_t io [] =
{
    [IO_USER_LED] =
    {
        .port      = GPIOA,
        .pin       = GPIO_PIN_5,
        .mode      = GPIO_MODE_OUTPUT_PP,
        .pull      = GPIO_NOPULL
    },
    [IO_USER_BUTTON] =
    {
        .port      = GPIOC,
        .pin       = GPIO_PIN_13,
        .mode      = GPIO_MODE_IT_FALLING,
        .pull      = GPIO_NOPULL
    },
    [IO_UART_2_RX] =
    {
        .port      = GPIOA,
        .pin       = GPIO_PIN_2,
        .mode      = GPIO_MODE_AF_PP,
        .pull      = GPIO_NOPULL,
        .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = GPIO_AF7_USART2
    },
    [IO_UART_2_TX] =
    {
        .port      = GPIOA,
        .pin       = GPIO_PIN_3,
        .mode      = GPIO_MODE_AF_PP,
        .pull      = GPIO_NOPULL,
        .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = GPIO_AF7_USART2
    },
    [IO_UART_5_RX] =
    {
        .port      = GPIOD,
        .pin       = GPIO_PIN_2,
        .mode      = GPIO_MODE_AF_PP,
        .pull      = GPIO_NOPULL,
        .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = GPIO_AF8_UART5
    },
    [IO_UART_5_TX] =
    {
        .port      = GPIOC,
        .pin       = GPIO_PIN_12,
        .mode      = GPIO_MODE_AF_PP,
        .pull      = GPIO_NOPULL,
        .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
        .alternate = GPIO_AF8_UART5
    }
};

/*
 * Function: iodef_get
 */
GOS_CONST gpio_desc_t* iodef_get (io_pin_t pin)
{
    /*
     * Function code.
     */
    return &io[pin];
}
