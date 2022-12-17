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
//! @file       gos_timer_driver.h
//! @author     Gabor Repasi
//! @date       2022-12-09
//! @version    1.0
//!
//! @brief      GOS timer driver header.
//! @details    This is the timer driver skeleton. It contains the required interface functions for
//!             the OS. These functions shall be implemented by the user in order to be able to use
//!             microsecond delay and task run-time monitoring.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-12-09    Gabor Repasi    Initial version created.
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
#ifndef GOS_TIMER_DRIVER_H
#define GOS_TIMER_DRIVER_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * System timer value get function type.
 */
typedef u32_t (*gos_timerDriverSysTimerGetVal_t)(void_t);

/*
 * Function prototypes
 */
/**
 * @brief   System timer value getter skeleton.
 * @details If registered, it calls the custom system timer getter function.
 *
 * @return  Result of system timer value getting.
 *
 * @retval  GOS_SUCCESS : According to user implementation.
 * @retval  GOS_ERROR   : According to user implementation / function not registered.
 */
u32_t gos_timerDriverSysTimerGet (void_t);
#endif
