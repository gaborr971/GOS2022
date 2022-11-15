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
//! @file		gos_uart.h
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS UART driver header.
//! @details	This is a driver skeleton with the required interface functions that the OS needs
//! 			to use for its services. The implementation of the drivers is in the hands of the
//! 			user, this is only a required interface collection. When a driver is used, the OS
//! 			calls the driver skeleton functions. These skeleton functions then check whether
//! 			the necessary driver functions are registered, and if so, they call them.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-31	Gabor Repasi	Initial version created.
// 1.1		2022-11-10	Gabor Repasi	+	Interface changed, driver is now just a skeleton.
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
#ifndef GOS_UART_H
#define GOS_UART_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * Enumerator for UART instances.
 */
typedef enum
{
	DRV_UART1 = 0U,				//!< UART 1st instance.
	DRV_UART2,					//!< UART 2nd instance.
	DRV_UART3,					//!< UART 3rd instance.
	DRV_UART4,					//!< UART 4th instance.
	NUMBER_OF_UART_PERIPHERIES	//!< Total number of UART peripheries.
}gos_uart_t;

/**
 * Initializer function type.
 */
typedef gos_result_t (*gos_uartInit_t			)(gos_uart_t			);

/**
 * Transmit function type.
 */
typedef gos_result_t (*gos_uartTransmit_t		)(gos_uart_t, char_t*	);

/**
 * Character receive function type.
 */
typedef gos_result_t (*gos_uartReceiveChar_t	)(gos_uart_t, char_t*	);

/**
 * Driver function list type.
 */
typedef struct
{
	gos_uartInit_t			initFunction;			//!< Initializer function.
	gos_uartTransmit_t		transmitFunction;		//!< Transmit function.
	gos_uartReceiveChar_t	receiveCharFunction;	//!< Character receive function.
}gos_uartFunctions_t;

/*
 * Function prototypes
 */
/**
 * @brief	Sets up the UART driver.
 * @details	This function is used to register the user UART driver that is
 * 			required by the OS for logging.
 *
 * @param	uartFunctions	: Set of UART driver function pointers.
 *
 * @return	Result of driver setup.
 *
 * @retval	GOS_SUCCESS	: Driver setup successful.
 * @retval	GOS_ERROR	: At least one of the driver functions is NULL pointer.
 */
gos_result_t gos_uartDriverSetup (gos_uartFunctions_t* uartFunctions);

/**
 * @brief	UART initializer skeleton.
 * @details	If registered, it calls the custom UART initializer function.
 *
 * @param	instance	: The UART instance (there can be multiple ones).
 *
 * @return	Result of UART initialization.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
gos_result_t gos_uartInit (gos_uart_t instance);

/**
 * @brief	UART transmit function skeleton.
 * @details	If registered, it calls the custom UART transmit function.
 *
 * @param	instance	: The UART instance.
 * @param	message		: Message string to be sent.
 *
 * @return	Result of UART transmission.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
gos_result_t gos_uartTransmit (gos_uart_t instance, char_t* message);

/**
 * @brief	UART character receiving function skeleton.
 * @details	If registered, it calls the custom UART character receiving function.
 *
 * @param	instance	: The UART instance.
 * @param	buffer		: Pointer to the variable to store the character in.
 *
 * @return	Result of UART reception.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
gos_result_t gos_uartReceiveChar (gos_uart_t instance, char_t* buffer);
#endif
