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
//! @file       dma_cfg.c
//! @author     Ahmed Gazar
//! @date       2024-05-03
//! @version    1.0
//!
//! @brief      DMA configuration source.
//! @details    This component contains the DMA configuration parameters.
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
#include <drv_dma.h>
#include <cfg_def.h>
#include "stm32f4xx_hal.h"

/**
 * DMA configuration array.
 */
drv_dmaDescriptor_t dmaConfig [] =
{
	// DMA1 stream 6 -> USART2 TX
	[DMA_CFG_USART2_TX] =
	{
		.dmaStream           = DRV_DMA_1_STREAM_6,
	    .channel             = DMA_CHANNEL_4,
	    .direction           = DMA_MEMORY_TO_PERIPH,
	    .periphInc           = DMA_PINC_DISABLE,
	    .memInc              = DMA_MINC_ENABLE,
	    .periphDataAlignment = DMA_PDATAALIGN_BYTE,
	    .memDataAlignment    = DMA_MDATAALIGN_BYTE,
	    .mode                = DMA_NORMAL,
	    .priority            = DMA_PRIORITY_LOW,
	    .fifoMode            = DMA_FIFOMODE_DISABLE,
	},
	// DMA2 stream 7 -> USART1 TX
	[DMA_CFG_USART1] =
	{
		.dmaStream           = DRV_DMA_2_STREAM_7,
		.channel             = DMA_CHANNEL_4,
		.direction           = DMA_MEMORY_TO_PERIPH,
		.periphInc           = DMA_PINC_DISABLE,
		.memInc              = DMA_MINC_ENABLE,
		.periphDataAlignment = DMA_PDATAALIGN_BYTE,
		.memDataAlignment    = DMA_MDATAALIGN_BYTE,
		.mode                = DMA_NORMAL,
		.priority            = DMA_PRIORITY_LOW,
		.fifoMode            = DMA_FIFOMODE_DISABLE,
	},
};

/**
 * DMA configuration size.
 */
u32_t dmaConfigSize = sizeof(dmaConfig);

/**
 * DMA stream assignment LUT.
 */
drv_dmaDescriptor_t* pDMADescriptorLut [DRV_DMA_X_STREAM_NUM] =
{
	[DRV_DMA_1_STREAM_0] = NULL,
	[DRV_DMA_1_STREAM_1] = NULL,
	[DRV_DMA_1_STREAM_2] = NULL,
	[DRV_DMA_1_STREAM_3] = NULL,
	[DRV_DMA_1_STREAM_4] = NULL,
	[DRV_DMA_1_STREAM_5] = NULL,
	[DRV_DMA_1_STREAM_6] = &dmaConfig[DMA_CFG_USART2_TX],
	[DRV_DMA_1_STREAM_7] = NULL,

	[DRV_DMA_2_STREAM_0] = NULL,
	[DRV_DMA_2_STREAM_1] = NULL,
	[DRV_DMA_2_STREAM_2] = NULL,
	[DRV_DMA_2_STREAM_3] = NULL,
	[DRV_DMA_2_STREAM_4] = NULL,
	[DRV_DMA_2_STREAM_5] = NULL,
	[DRV_DMA_2_STREAM_6] = NULL,
	[DRV_DMA_2_STREAM_7] = &dmaConfig[DMA_CFG_USART1],
};
