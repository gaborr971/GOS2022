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
//! @file		gos_log.h
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS log service header.
//! @details	Log service is a simple interface to send out strings via a configured UART
//! 			periphery.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-24	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Formatted log function added
//										+	Descriptions added
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
#ifndef GOS_LOG_H
#define GOS_LOG_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Function prototypes
 */
/**
 * @brief	Initializes the log service.
 * @details	Creates a lock for logging and initializes the UART periphery
 * 			based on the configuration settings.
 *
 * @return	Result of initialization.
 *
 * @retval 	GOS_SUCCESS	: Initialization successful.
 * @retval	GOS_ERROR	: Lock creation error or UART initialization error.
 */
gos_result_t gos_logInit (void_t);

/**
 * @brief	Logs a given message.
 * @details	Sends out the given log message via UART.
 *
 * @param	logMessage	: String to log.
 *
 * @return	Result of logging.
 *
 * @retval 	GOS_SUCCESS	: Logging successful.
 * @retval	GOS_ERROR	: Lock get/release error or UART transmit error.
 *
 * @remark	This function uses the lock service.
 */
gos_result_t gos_logLog (char_t* logMessage);

/**
 * @brief	Logs a given formatted message.
 * @details	Prints the formatted message into a local buffer and transmits it
 * 			via the log port.
 *
 * @param 	logFormat	: Formatter string.
 * @param	...			: Optional parameters.
 *
 * @return	Result of formatted logging.
 *
 * @retval	GOS_SUCCESS	: Message logged successfully.
 * @retval	GOS_ERROR	: Lock get or UART transmit error.
 */
gos_result_t gos_logLogFormatted (const char_t* logFormat, ...);

#endif
