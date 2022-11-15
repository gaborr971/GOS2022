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
//! @date		2022-11-10
//! @version	1.1
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
