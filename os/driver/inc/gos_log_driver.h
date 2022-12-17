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
//! @file       gos_log_driver.h
//! @author     Gabor Repasi
//! @date       2022-12-13
//! @version    1.1
//!
//! @brief      GOS LOG driver header.
//! @details    This driver provides a skeleton for the driver for the log service.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-12-11    Gabor Repasi    Initial version created.
// 1.1        2022-12-13    Gabor Repasi    +    Unsafe transmit string function added
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
#ifndef GOS_LOG_DRIVER_H
#define GOS_LOG_DRIVER_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * Log driver transmit string function type.
 */
typedef gos_result_t (*gos_logDriverTransmitString_t)(char_t*);

/**
 * Log driver unsafe transmit string function type.
 */
typedef gos_result_t (*gos_logDriverTransmitString_Unsafe_t)(char_t*);

/*
 * Function prototypes
 */
/**
 * @brief   Log driver transmit string function skeleton.
 * @details If registered, it calls the custom transmit function.
 *
 * @param   pString     : String to log.
 *
 * @return  Result of string transmission.
 *
 * @retval  GOS_SUCCESS : According to user implementation.
 * @retval  GOS_ERROR   : According to user implementation / function not registered.
 */
gos_result_t gos_logDriverTransmitString (char_t* pString);

/**
 * @brief   Log driver unsafe transmit string function skeleton.
 * @details If registered, it calls the custom unsafe transmit function.
 *
 * @param   pString     : String to log.
 *
 * @return  Result of string transmission.
 *
 * @retval  GOS_SUCCESS : According to user implementation.
 * @retval  GOS_ERROR   : According to user implementation / function not registered.
 */
gos_result_t gos_logDriverTransmitString_Unsafe (char_t* pString);

#endif
