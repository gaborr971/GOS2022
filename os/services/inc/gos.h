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
//! @file       gos.h
//! @author     Gabor Repasi
//! @date       2023-06-17
//! @version    1.5
//!
//! @brief      GOS header.
//! @details    This header is a wrapper for the inclusion of all OS services and drivers for
//!             GOS2022 v0.4
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-28    Gabor Repasi    Initial version created
// 1.1        2022-11-14    Gabor Repasi    +    Header description added
//                                          +    Function descriptions added
// 1.2        2022-11-15    Gabor Repasi    +    License added
// 1.3        2022-12-11    Gabor Repasi    -    Function prototypes removed
// 1.4        2022-12-15    Gabor Repasi    *    OS version number changed to v0.3
// 1.5        2023-06-17    Ahmed Gazar     +    gos_Dump added
//                                               OS version number changed to v0.4
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
#ifndef GOS_H
#define GOS_H
/*
 * Includes
 */
#include <gos_driver.h>
#include <gos_error.h>
#include <gos_gcp.h>
#include <gos_message.h>
#include <gos_mutex.h>
#include <gos_process.h>
#include <gos_queue.h>
#include <gos_shell.h>
#include <gos_signal.h>
#include <gos_time.h>
#include <gos_trace.h>
#include <gos_trigger.h>

/*
 * Function prototypes
 */
/**
 * @brief   GOS dump.
 * @details Sets the dump required flag and wakes up the system task
 *          that calls the individual dump functions.
 *
 * @return  -
 */
void_t gos_Dump (void_t);
#endif
