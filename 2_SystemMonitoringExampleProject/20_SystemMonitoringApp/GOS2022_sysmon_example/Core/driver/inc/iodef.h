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
//! @file       iodef.h
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      IO definitions header.
//! @details    This component contains the IO definitions.
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
#ifndef IODEF_H
#define IODEF_H

/*
 * Includes.
 */
#include "gos.h"
#include "gpio_driver.h"

/*
 * Type definitions.
 */
/**
 * IO pin enumerator.
 */
typedef enum
{
    IO_USER_LED     = 0u, //!< User LED.
    IO_USER_BUTTON,       //!< User button.
    IO_UART_2_RX,         //!< UART2 RX pin.
    IO_UART_2_TX,         //!< UART2 TX pin.
    IO_UART_5_RX,         //!< UART5 RX pin.
    IO_UART_5_TX,         //!< UART5 TX pin.
    IO_NUMBER             //!< Number of defined IOs.
}io_pin_t;

/*
 * Function prototypes
 */
/**
 * @brief   Returns the GPIO descriptor of the given pin.
 * @details Returns the GPIO descriptor of the given pin.
 *
 * @param   pin : IO pin.
 *
 * @return  Pointer to the corresponding IO structure.
 */
GOS_CONST gpio_desc_t* iodef_get (io_pin_t pin);

#endif
