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
//! @file		gos.h
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS header.
//! @details	This header is a wrapper for the inclusion of all OS services and drivers for
//! 			GOS2022 v0.1
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-28	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Header description added
//										+	Function descriptions added
// 1.2		2022-11-15	Gabor Repasi	+	License added
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
#include "gos_drv.h"
#include "gos_kernel.h"
#include "gos_lock.h"
#include "gos_log.h"
#include "gos_message.h"
#include "gos_process.h"
#include "gos_queue.h"
#include "gos_signal.h"
#include "gos_shell.h"
#include "gos_time.h"

/*
 * Function prototypes
 */
/**
 * @brief	OS initializer function.
 * @details	Calls the necessary driver and service initializer functions.
 *
 * @return	Result of OS initialization.
 *
 * @retval	GOS_SUCCESS	: OS initialization successful.
 * @retval	GOS_ERROR	: At least one driver or service initialization failed.
 */
gos_result_t gos_Init (void_t);

/**
 * @brief	Starts the OS.
 * @details	Checks whether the initializer function has set the error flag to GOS_FALSE,
 * 			and if so, it starts the kernel (and thus the scheduling of tasks).
 *
 * @return	Result of OS starting.
 *
 * @retval	GOS_ERROR	: OS not started due to initialization error or kernel start error.
 *
 * @remark	This function should only return with error. If the initialization is successful,
 * 			the function is not expected to return.
 */
gos_result_t gos_Start (void_t);
#endif
