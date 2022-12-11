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
//! @file		gos_driver.h
//! @author		Gabor Repasi
//! @date		2022-12-09
//! @version	1.0
//!
//! @brief		GOS driver header.
//! @details	This header is used for the inclusion of all driver skeletons.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-12-09	Gabor Repasi	Initial version created.
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
#ifndef GOS_DRIVER_H
#define GOS_DRIVER_H
/*
 * Includes
 */
#include "gos_crc_driver.h"
#include "gos_log_driver.h"
#include "gos_shell_driver.h"
#include "gos_timer_driver.h"

/*
 * Type definitions
 */
/**
 * Driver functions type.
 */
typedef struct
{
	gos_shellDriverReceiveChar_t	shellDriverReceiveChar;			//!< Shell character receive function.
	gos_shellDriverTransmitString_t	shellDriverTransmitString;		//!< Shell string transmit function.
	gos_logDriverTransmitString_t	logDriverTransmitString;		//!< Log string transmit function.
	gos_timerDriverSysTimerGetVal_t	timerDriverSysTimerGetValue;	//!< System timer get function.
}gos_driver_functions_t;

/*
 * Function prototypes
 */
/**
 * @brief	Initializes the kernel drivers.
 * @details	Copies the function pointers into the internal structure.
 *
 * @param	pDriverFunctions	: Pointer to the function pointer structure.
 *
 * @return	Result of initialization.
 *
 * @retval	GOS_SUCCESS	:	Initialization successful.
 * @retval	GOS_ERROR	:	NULL pointer parameter.
 */
gos_result_t gos_driverInit (gos_driver_functions_t* pDriverFunctions);

#endif
