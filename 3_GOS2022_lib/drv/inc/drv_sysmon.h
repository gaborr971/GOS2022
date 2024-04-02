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
//! @file       drv_sysmon.h
//! @author     Ahmed Gazar
//! @date       2024-03-15
//! @version    1.0
//!
//! @brief      GOS2022 Library / Sysmon driver header.
//! @details    This component provides the implementation of sysmon functions that are used
//!             by the OS.
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
#ifndef DRV_SYSMON_H
#define DRV_SYSMON_H
/*
 * Includes
 */
#include <gos.h>

/*
 * Function prototypes
 */
/**
 * @brief   Sysmon transmit function.
 * @details This function uses the configured sysmon UART instance to
 *          transmit the given data bytes ensuring safe access to the
 *          periphery (mutex and trigger are used).
 *
 * @param   pData : Data bytes to be transmitted.
 * @param   size  : Size of data.
 *
 * @return  Result of transmission.
 *
 * @retval  GOS_SUCCESS : Transmit successful.
 * @retval  GOS_ERROR   : Empty UART service configuration or low-level
 *                        driver error.
 */
gos_result_t drv_sysmonTransmit (u8_t* pData, u16_t size);

/**
 * @brief   Sysmon receive function.
 * @details This function uses the configured sysmon UART instance to
 *          receive data from ensuring safe access to the periphery
 *          (mutex and trigger are used).
 *
 * @param   pBuffer : Buffer where the received data is stored.
 * @param   size    : Size of the receive buffer.
 *
 * @return  Result of reception.
 *
 * @retval  GOS_SUCCESS : Reception successful.
 * @retval  GOS_ERROR   : Empty UART service configuration or low-level
 *                        driver error.
 */
gos_result_t drv_sysmonReceive (u8_t* pBuffer, u16_t size);

#endif
