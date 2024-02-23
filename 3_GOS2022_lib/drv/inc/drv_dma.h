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
//! @file       drv_dma.h
//! @author     Ahmed Gazar
//! @date       2024-02-23
//! @version    1.0
//!
//! @brief      GOS2022 Library / DMA driver header.
//! @details    This component provides access to the DMA peripheries.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-23    Ahmed Gazar     Initial version created.
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
#ifndef DRV_DMA_H
#define DRV_DMA_H

/*
 * Includes
 */
#include <gos.h>
#include "stm32f4xx_hal.h"

/*
 * Type definitions
 */
/**
 * DMA stream enum.
 */
typedef enum
{
	DRV_DMA_1_STREAM_0 = 0,/**< DRV_DMA_1_STREAM_0 */
	DRV_DMA_1_STREAM_1,    /**< DRV_DMA_1_STREAM_1 */
	DRV_DMA_1_STREAM_2,    /**< DRV_DMA_1_STREAM_2 */
	DRV_DMA_1_STREAM_3,    /**< DRV_DMA_1_STREAM_3 */
	DRV_DMA_1_STREAM_4,    /**< DRV_DMA_1_STREAM_4 */
	DRV_DMA_1_STREAM_5,    /**< DRV_DMA_1_STREAM_5 */
	DRV_DMA_1_STREAM_6,    /**< DRV_DMA_1_STREAM_6 */
	DRV_DMA_1_STREAM_7,    /**< DRV_DMA_1_STREAM_7 */
	DRV_DMA_2_STREAM_0,    /**< DRV_DMA_2_STREAM_0 */
	DRV_DMA_2_STREAM_1,    /**< DRV_DMA_2_STREAM_1 */
	DRV_DMA_2_STREAM_2,    /**< DRV_DMA_2_STREAM_2 */
	DRV_DMA_2_STREAM_3,    /**< DRV_DMA_2_STREAM_3 */
	DRV_DMA_2_STREAM_4,    /**< DRV_DMA_2_STREAM_4 */
	DRV_DMA_2_STREAM_5,    /**< DRV_DMA_2_STREAM_5 */
	DRV_DMA_2_STREAM_6,    /**< DRV_DMA_2_STREAM_6 */
	DRV_DMA_2_STREAM_7,    /**< DRV_DMA_2_STREAM_7 */
}drv_dmaStream_t;

/**
 * DMA descriptor type.
 */
typedef struct
{
	drv_dmaStream_t    dmaStream;
	DMA_HandleTypeDef  hdma;
	u32_t              channel;
	u32_t              direction;
	u32_t              periphInc;
	u32_t              memInc;
	u32_t              periphDataAlignment;
	u32_t              memDataAlignment;
	u32_t              mode;
	u32_t              priority;
	u32_t              fifoMode;
}drv_dmaDescriptor_t;

/*
 * Function prototypes
 */
// TODO
gos_result_t drv_dmaInit (void_t);

#endif
