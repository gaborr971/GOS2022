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
//                                      (c) Ahmed Gazar, 2024
//
//*************************************************************************************************
//! @file       uart_cfg.c
//! @author     Ahmed Gazar
//! @date       2024-05-03
//! @version    1.0
//!
//! @brief      UART configuration source.
//! @details    This component contains the UART configuration parameters.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-05-03    Ahmed Gazar     Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2024 Ahmed Gazar
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
#include <drv_uart.h>
#include <drv_dma.h>
#include <cfg_def.h>
#include "stm32f4xx_hal.h"

/*
 * Extern variables
 */
GOS_EXTERN drv_dmaDescriptor_t dmaConfig [];

/**
 * UART configuration array.
 */
GOS_CONST drv_uartDescriptor_t uartConfig [] =
{
	// USART1 for shell.
	{
		.periphInstance = DRV_UART_INSTANCE_1,
		.baudRate       = 115200,
		.wordLength     = UART_WORDLENGTH_8B,
		.stopBits       = UART_STOPBITS_1,
		.parity         = UART_PARITY_NONE,
		.mode           = UART_MODE_TX_RX,
		.hwFlowControl  = UART_HWCONTROL_NONE,
		.overSampling   = UART_OVERSAMPLING_16,
		.dmaConfigTx    = &dmaConfig[DMA_CFG_USART1]
	},

	// USART2 for trace.
	{
		.periphInstance = DRV_UART_INSTANCE_2,
		.baudRate       = 115200,
		.wordLength     = UART_WORDLENGTH_8B,
		.stopBits       = UART_STOPBITS_1,
		.parity         = UART_PARITY_NONE,
		.mode           = UART_MODE_TX_RX,
		.hwFlowControl  = UART_HWCONTROL_NONE,
		.overSampling   = UART_OVERSAMPLING_16,
		.dmaConfigTx    = &dmaConfig[DMA_CFG_USART2_TX]
	},
};

/**
 * UART service timeout configuration.
 */
drv_uartServiceTimeoutConfig_t uartServiceTmoConfig =
{
	.traceMutexTmo          = 2000u,
	.traceTriggerTmo        = 2000u,
	.traceUnsafeTransmitTmo = 1000u,
	.shellRxMutexTmo        = GOS_MUTEX_ENDLESS_TMO,
	.shellRxTriggerTmo      = GOS_TRIGGER_ENDLESS_TMO,
	.shellTxMutexTmo        = 2000u,
	.shellTxTriggerTmo      = 2000u,
};

/**
 * UART service configuration array.
 */
GOS_CONST drv_uartPeriphInstance_t  uartServiceConfig [] =
{
	[DRV_UART_SHELL_INSTANCE]  = DRV_UART_INSTANCE_1,
	[DRV_UART_TRACE_INSTANCE]  = DRV_UART_INSTANCE_2
};

/**
 * UART service configuration size.
 */
u32_t uartConfigSize = sizeof(uartConfig);
