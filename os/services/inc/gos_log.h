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
//! @date		2022-11-14
//! @version	1.1
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
