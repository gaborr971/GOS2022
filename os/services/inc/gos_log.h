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
//! @date		2022-12-11
//! @version	1.3
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
// 1.3		2022-12-11	Gabor Repasi	+	Font formatting macros added
//										+	Unsafe formatted log function added
//										*	Function descriptions updated
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
 * Macros
 */
#define LOG_FG_WHITE_START		"\x1B[37m"	//!< White foreground start.
#define LOG_FG_CYAN_START		"\x1B[36m"	//!< Cyan foreground start.
#define LOG_FG_MAGENTA_START	"\x1B[35m"	//!< Magenta foreground start.
#define LOG_FG_BLUE_START		"\x1B[34m"	//!< Blue foreground start.
#define LOG_FG_YELLOW_START		"\x1B[33m"	//!< Yellow foreground start.
#define LOG_FG_GREEN_START		"\x1B[32m"	//!< Green foreground start.
#define LOG_FG_RED_START		"\x1B[31m"	//!< Red foreground start.
#define LOG_FORMAT_RESET 		"\x1B[0m"	//!< Reset formatting.

/*
 * Function prototypes
 */
/**
 * @brief	Initializes the log service.
 * @details	Creates a log queue and registers the log daemon in the kernel.
 *
 * @return	Result of initialization.
 *
 * @retval 	GOS_SUCCESS	: Initialization successful.
 * @retval	GOS_ERROR	: Queue creation or task registration error.
 */
gos_result_t gos_logInit (void_t);

/**
 * @brief	Logs a given message.
 * @details	Places the given message to the log queue (for the log daemon to print it).
 *
 * @param	logMessage	: String to log.
 *
 * @return	Result of logging.
 *
 * @retval 	GOS_SUCCESS	: Logging successful.
 * @retval	GOS_ERROR	: Queue put error.
 *
 * @remark	This function uses the lock service.
 */
gos_result_t gos_logLog (char_t* logMessage);

/**
 * @brief	Logs a given formatted message.
 * @details	Prints the formatted message into a local buffer and places it
 * 			to the log queue (for the log daemon to print it).
 *
 * @param 	logFormat	: Formatter string.
 * @param	...			: Optional parameters.
 *
 * @return	Result of formatted logging.
 *
 * @retval	GOS_SUCCESS	: Formatted logging successful.
 * @retval	GOS_ERROR	: Queue put error.
 */
gos_result_t gos_logLogFormatted (const char_t* logFormat, ...);

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
 * @retval	GOS_ERROR	: Transmit error.
 */
gos_result_t gos_logLogFormattedUnsafe (const char_t* logFormat, ...);

#endif
