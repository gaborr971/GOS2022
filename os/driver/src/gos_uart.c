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
//! @file		gos_uart.c
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.2
//!
//! @brief		GOS UART driver source.
//! @details	For a more detailed description of this service, please refer to @ref gos_uart.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-31	Gabor Repasi	Initial version created.
// 1.1		2022-11-10	Gabor Repasi	+	Driver function structure added
//										-	Driver implementations removed
//										+	Driver function registration added, driver is now
//											a skeleton only with required interfaces
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
/*
 * Includes
 */
#include "gos_uart.h"

/*
 * Static variables
 */
/**
 * Driver function configuration.
 */
GOS_STATIC gos_uartFunctions_t uartDriverFunctions =
{
	.initFunction 			= NULL,
	.transmitFunction 		= NULL,
	.receiveCharFunction 	= NULL
};

/*
 * Function: gos_uartDriverSetup
 */
gos_result_t gos_uartDriverSetup (gos_uartFunctions_t* uartFunctions)
{
	/*
	 * Local variables.
	 */
	gos_result_t uartDriverSetupResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (uartFunctions->initFunction != NULL &&
		uartFunctions->transmitFunction != NULL &&
		uartFunctions->receiveCharFunction != NULL)
	{
		uartDriverFunctions.initFunction = uartFunctions->initFunction;
		uartDriverFunctions.transmitFunction = uartFunctions->transmitFunction;
		uartDriverFunctions.receiveCharFunction = uartFunctions->receiveCharFunction;
		uartDriverSetupResult = GOS_SUCCESS;
	}

	return uartDriverSetupResult;
}

/*
 * Function: gos_uartInit
 */
gos_result_t gos_uartInit (gos_uart_t instance)
{
	/*
	 * Local variables.
	 */
	gos_result_t uartInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (uartDriverFunctions.initFunction != NULL)
	{
		uartInitResult = uartDriverFunctions.initFunction(instance);
	}

	return uartInitResult;
}

/*
 * Function: gos_uartTransmit
 */
gos_result_t gos_uartTransmit (gos_uart_t instance, char_t* message)
{
	/*
	 * Local variables.
	 */
	gos_result_t uartTransmitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (uartDriverFunctions.transmitFunction != NULL)
	{
		uartTransmitResult = uartDriverFunctions.transmitFunction(instance, message);
	}

	return uartTransmitResult;
}

/*
 * Function: gos_uartReceiveChar
 */
gos_result_t gos_uartReceiveChar (gos_uart_t instance, char_t* buffer)
{
	/*
	 * Local variables.
	 */
	gos_result_t uartReceiveResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (uartDriverFunctions.receiveCharFunction != NULL)
	{
		uartReceiveResult = uartDriverFunctions.receiveCharFunction(instance, buffer);
	}

	return uartReceiveResult;
}
