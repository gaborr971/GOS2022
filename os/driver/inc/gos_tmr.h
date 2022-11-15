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
//! @file		gos_tmr.h
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS timer driver header.
//! @details	This is the timer driver skeleton. It contains the required interface functions for
//! 			the OS. These functions shall be implemented by the user in order to be able to use
//! 			microsecond delay and task run-time monitoring.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-11-10	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	API documentation added for functions
//										+	Header description added
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
#ifndef GOS_TMR_H
#define GOS_TMR_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * System timer initializer function type.
 */
typedef gos_result_t	(*gos_timerSysTimerInit_t	)(void_t);

/**
 * System timer value get function type.
 */
typedef u32_t			(*gos_timerSysTimerGet_t	)(void_t);

/**
 * Driver function list type.
 */
typedef struct
{
	gos_timerSysTimerInit_t	sysTimerInitFunction;	//!< Initializer function.
	gos_timerSysTimerGet_t	sysTimerGetFunction;	//!< Transmit function.
}gos_timerFunctions_t;

/*
 * Function prototypes
 */
/**
 * @brief	Sets up the timer driver.
 * @details	This function is used to register the user timer driver that is
 * 			required by the OS for microsecond delay and task run-time monitoring.
 *
 * @param	timerFunctions	: Set of timer driver function pointers.
 *
 * @return	Result of driver setup.
 *
 * @retval	GOS_SUCCESS	: Driver setup successful.
 * @retval	GOS_ERROR	: At least one of the driver functions is NULL pointer.
 */
gos_result_t gos_timerDriverSetup (gos_timerFunctions_t* timerFunctions);

/**
 * @brief	System timer initializer skeleton.
 * @details	If registered, it calls the custom system timer initializer function.
 *
 * @return	Result of system timer initialization.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
gos_result_t gos_timerSysTimerInit (void_t);

/**
 * @brief	System timer value getter skeleton.
 * @details	If registered, it calls the custom system timer getter function.
 *
 * @return	Result of system timer value getting.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
u32_t gos_timerSysTimerGet (void_t);
#endif
