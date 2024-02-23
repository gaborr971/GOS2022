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
//! @file       drv_uart.h
//! @author     Ahmed Gazar
//! @date       2024-02-21
//! @version    1.0
//!
//! @brief      GOS2022 Library / UART driver header.
//! @details    This component provides access to the UART peripheries.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-21    Ahmed Gazar     Initial version created.
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
#ifndef DRV_UART_H
#define DRV_UART_H

/*
 * Includes
 */
#include <gos.h>
#include <drv_dma.h>

/*
 * Type definitions
 */
/**
 * UART periphery instance enum.
 */
typedef enum
{
	DRV_UART_INSTANCE_1 = 0,                 //!< USART1
	DRV_UART_INSTANCE_2,                     //!< USART2
	DRV_UART_INSTANCE_3,                     //!< USART3
	DRV_UART_INSTANCE_4,                     //!< UART4
	DRV_UART_INSTANCE_5,                     //!< UART5
	DRV_UART_INSTANCE_6,                     //!< USART6
	DRV_UART_NUM_OF_INSTANCES                //!< Number of UART instances.
}drv_uartPeriphInstance_t;

/**
 * UART service instance enum.
 */
typedef enum
{
	DRV_UART_SYSMON_INSTANCE = 0,            //!< System monitoring instance.
	DRV_UART_SHELL_INSTANCE,                 //!< Shell instance.
	DRV_UART_TRACE_INSTANCE                  //!< Trace instance.
}drv_uartServiceInstance_t;

/**
 * UART descriptor type.
 */
typedef struct
{
	drv_uartPeriphInstance_t periphInstance; //!< Periphery instance.
	u32_t                    baudRate;       //!< Baud rate.
	u32_t                    wordLength;     //!< Word length.
	u32_t                    stopBits;       //!< Stop bits.
	u32_t                    parity;         //!< Parity.
	u32_t                    mode;           //!< Mode.
	u32_t                    hwFlowControl;  //!< Hardware flow control.
	u32_t                    overSampling;   //!< Over-sampling.
	drv_dmaDescriptor_t*     dmaConfigRx;    //!< TODO
	drv_dmaDescriptor_t*     dmaConfigTx;    //!< TODO
}drv_uartDescriptor_t;

/*
 * Function prototypes
 */

// TODO
gos_result_t drv_uartInit (void_t);

// TODO
gos_result_t drv_uartTraceTransmit (char_t* message);

// TODO
gos_result_t drv_uartTraceTransmitUnsafe (char_t* message);

// TODO
gos_result_t drv_uartShellReceiveChar (char_t* buffer);

// TODO
gos_result_t drv_uartShellTransmitString (char_t* pString);

// TODO
gos_result_t drv_uartSysmonTransmit (u8_t* data, u16_t size);

// TODO
gos_result_t drv_uartSysmonReceive (u8_t* data, u16_t size);

#endif
