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
//! @file       TODO
//! @author     Ahmed Gazar
//! @date       2024-04-24
//! @version    1.0
//!
//! @brief      GOS2022 Library / TODO header.
//! @details    TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-24    Ahmed Gazar     Initial version created.
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
#ifndef DRV_ERROR_H
#define DRV_ERROR_H

/*
 * Macros
 */
#define DRV_ERROR_SET(flags, error)       (flags |= error)
#define DRV_ERROR_CLEAR(flags, error)     (flags &= ~error)

#define DRV_ERROR_UART_CFG_ARRAY_NULL     ( 1 << 0  )
#define DRV_ERROR_UART_INDEX_OUT_OF_BOUND ( 1 << 1  )
#define DRV_ERROR_UART_INSTANCE_INIT      ( 1 << 2  )
#define DRV_ERROR_UART_INSTANCE_DEINIT    ( 1 << 3  )
#define DRV_ERROR_UART_TX_BLOCKING        ( 1 << 4  )
#define DRV_ERROR_UART_TX_DMA_MUTEX       ( 1 << 5  )
#define DRV_ERROR_UART_TX_DMA_TRIG_HAL    ( 1 << 6  )
#define DRV_ERROR_UART_RX_DMA_MUTEX       ( 1 << 7  )
#define DRV_ERROR_UART_RX_DMA_TRIG_HAL    ( 1 << 8  )
#define DRV_ERROR_UART_TX_IT_MUTEX        ( 1 << 9  )
#define DRV_ERROR_UART_TX_IT_TRIG_HAL     ( 1 << 10 )
#define DRV_ERROR_UART_RX_IT_MUTEX        ( 1 << 11 )
#define DRV_ERROR_UART_RX_IT_TRIG_HAL     ( 1 << 12 )

#endif
