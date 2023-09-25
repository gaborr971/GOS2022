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
//! @file       uart_driver.h
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      UART driver header.
//! @details    This component implements the necessary OS driver functions via UART.
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
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

/*
 * Includes
 */
#include "gos.h"

/*
 * Function prototypes
 */

/**
 * @brief   This function initializes the used UART peripheries.
 * @details Enables the peripheral clocks, initializes the UART peripheries, and
 *          the related triggers and mutexes.
 *
 * @return  Result of initialization.
 *
 * @retval  GOS_SUCCESS : UART initialization successful.
 * @retval  GOS_ERROR   : Periphery initialization error.
 */
gos_result_t uart_driverInit (void_t);

/**
 * @brief   Wrapper for kernel unsafe trace function.
 * @details Transmits the given string via the pre-defined UART instance in blocking mode while
 *          disabling rescheduling.
 *
 * @param   message : String to be traced.
 *
 * @return  Result of tracing.
 *
 * @retval  GOS_SUCCESS : Transmission successful.
 * @retval  GOS_ERROR   : Transmission error on periphery.
 */
gos_result_t uart_driverTraceTransmitStringUnsafe (char_t* message);

/**
 * @brief   Wrapper for kernel trace function.
 * @details Transmits the given string via the pre-defined UART instance.
 *
 * @param   message : String to be transmitted.
 *
 * @return  Result of tracing.
 *
 * @retval  GOS_SUCCESS : Transmission successful.
 * @retval  GOS_ERROR   : Mutex lock, transmit or trigger error.
 */
gos_result_t uart_driverTraceTransmitString (char_t* message);

/**
 * @brief   Wrapper for shell receive character function.
 * @details Receives a single character from the pre-defined UART instance.
 *
 * @param   buffer      : Variable where the received character shall be stored.
 *
 * @return  Result of reception.
 *
 * @retval  GOS_SUCCESS : Reception successful.
 * @retval  GOS_ERROR   : Mutex lock, reception or trigger error.
 */
gos_result_t uart_driverShellReceiveChar (char_t* buffer);

/**
 * @brief   Wrapper for shell transmit string function.
 * @details Transmits the given string via the pre-defined UART instance.
 *
 * @param   pString     : String to be transmitted.
 *
 * @return  Result of transmission.
 *
 * @retval  GOS_SUCCESS : Transmission successful.
 * @retval  GOS_ERROR   : Mutex lock, transmit or trigger error.
 */
gos_result_t uart_driverShellTransmitString (char_t* pString);

/**
 * @brief   Wrapper for sysmon transmit function.
 * @details Transmits the given buffer via the pre-defined UART instance.
 *
 * @param   pData       : Buffer to be transmitted.
 * @param   size        : Buffer size.
 *
 * @return  Result of transmission.
 *
 * @retval  GOS_SUCCESS : Transmission successful.
 * @retval  GOS_ERROR   : Mutex lock, transmit or trigger error.
 */
gos_result_t uart_driverSysmonTransmit (u8_t* pData, u16_t size);

/**
 * @brief   Wrapper for sysmon receive function.
 * @details Receives data to a buffer from the pre-defined UART instance.
 *
 * @param   pData       : Variable where the received data shall be stored.
 * @param   size        : Buffer size.
 *
 * @return  Result of reception.
 *
 * @retval  GOS_SUCCESS : Reception successful.
 * @retval  GOS_ERROR   : Mutex lock, reception or trigger error.
 */
gos_result_t uart_driverSysmonReceive (u8_t* pData, u16_t size);

#endif
