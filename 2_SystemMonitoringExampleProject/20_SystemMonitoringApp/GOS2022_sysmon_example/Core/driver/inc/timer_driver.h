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
//! @file       timer_driver.h
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      Timer driver header.
//! @details    This component implements the microsecond resolution timer interface for the kernel.
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
#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

/*
 * Includes
 */
#include "gos.h"

/*
 * Function prototypes
 */
/**
 * @brief   Initializes the system timer.
 * @details Sets up a microsecond resolution timer for the kernel.
 *
 * @return  Result of initialization.
 *
 * @retval  GOS_SUCCESS : Initialization successful.
 * @retval  GOS_ERROR   : Periphery error.
 */
gos_result_t timer_driverSystimerInit (void_t);

/**
 * @brief   Gets the actual system timer value.
 * @details Gets the system timer counter value.
 *
 * @param   pValue      : Target variable.
 *
 * @return  Result of timer reading.
 *
 * @retval  GOS_SUCCESS : Timer value successfully returned.
 * @retval  GOS_ERROR   : Target variable is NULL pointer.
 */
gos_result_t timer_driverSystimerGet (u16_t* pValue);

#endif
