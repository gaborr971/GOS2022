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
//! @file       drv_dma.c
//! @author     Ahmed Gazar
//! @date       2024-02-23
//! @version    1.0
//!
//! @brief      GOS2022 Library / DMA driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_dma.h
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
/*
 * Includes
 */
#include <drv_dma.h>

/*
 * Static variables
 */
/**
 * DMA stream lookup table.
 */
GOS_STATIC DMA_Stream_TypeDef* dmaStreamLut [] =
{
    [DRV_DMA_1_STREAM_0] = DMA1_Stream0,
    [DRV_DMA_1_STREAM_1] = DMA1_Stream1,
    [DRV_DMA_1_STREAM_2] = DMA1_Stream2,
    [DRV_DMA_1_STREAM_3] = DMA1_Stream3,
    [DRV_DMA_1_STREAM_4] = DMA1_Stream4,
    [DRV_DMA_1_STREAM_5] = DMA1_Stream5,
    [DRV_DMA_1_STREAM_6] = DMA1_Stream6,
    [DRV_DMA_1_STREAM_7] = DMA1_Stream7,

    [DRV_DMA_2_STREAM_0] = DMA2_Stream0,
    [DRV_DMA_2_STREAM_1] = DMA2_Stream1,
    [DRV_DMA_2_STREAM_2] = DMA2_Stream2,
    [DRV_DMA_2_STREAM_3] = DMA2_Stream3,
    [DRV_DMA_2_STREAM_4] = DMA2_Stream4,
    [DRV_DMA_2_STREAM_5] = DMA2_Stream5,
    [DRV_DMA_2_STREAM_6] = DMA2_Stream6,
    [DRV_DMA_2_STREAM_7] = DMA2_Stream7,
};

/*
 * External variables
 */
/**
 * DMA configuration array (shall be defined by user).
 */
GOS_EXTERN drv_dmaDescriptor_t dmaConfig [];

/**
 * DMA configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t               dmaConfigSize;

/*
 * Function: drv_dmaInit
 */
gos_result_t drv_dmaInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t dmaInitResult = GOS_SUCCESS;
    u8_t         idx           = 0u;

    /*
     * Function code.
     */
    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        dmaConfig[idx].hdma.Instance                 = dmaStreamLut[dmaConfig[idx].dmaStream];
        dmaConfig[idx].hdma.Init.Channel             = dmaConfig[idx].channel;
        dmaConfig[idx].hdma.Init.Direction           = dmaConfig[idx].direction;
        dmaConfig[idx].hdma.Init.PeriphInc           = dmaConfig[idx].periphInc;
        dmaConfig[idx].hdma.Init.MemInc              = dmaConfig[idx].memInc;
        dmaConfig[idx].hdma.Init.PeriphDataAlignment = dmaConfig[idx].periphDataAlignment;
        dmaConfig[idx].hdma.Init.MemDataAlignment    = dmaConfig[idx].memDataAlignment;
        dmaConfig[idx].hdma.Init.Mode                = dmaConfig[idx].mode;
        dmaConfig[idx].hdma.Init.Priority            = dmaConfig[idx].priority;
        dmaConfig[idx].hdma.Init.FIFOMode            = dmaConfig[idx].fifoMode;

        if (HAL_DMA_Init(&(dmaConfig[idx].hdma)) != HAL_OK)
        {
            dmaInitResult = GOS_ERROR;
        }
        else
        {
            // Init OK.
        }
    }

    return dmaInitResult;
}

/*
 * Function: DMA1_Stream0_IRQHandler
 */
void_t DMA1_Stream0_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_0)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA1_Stream1_IRQHandler
 */
void_t DMA1_Stream1_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_1)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA1_Stream2_IRQHandler
 */
void_t DMA1_Stream2_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_2)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA1_Stream3_IRQHandler
 */
void_t DMA1_Stream3_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_3)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA1_Stream4_IRQHandler
 */
void_t DMA1_Stream4_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_4)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

#if 0 // USED FOR DAC
/*
 * Function: DMA1_Stream5_IRQHandler
 */
void_t DMA1_Stream5_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_5)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}
#endif

/*
 * Function: DMA1_Stream6_IRQHandler
 */
void_t DMA1_Stream6_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_6)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

#if 0 // USED FOR SPI
/*
 * Function: DMA1_Stream7_IRQHandler
 */
void_t DMA1_Stream7_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_1_STREAM_7)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}
#endif

/*
 * Function: DMA2_Stream0_IRQHandler
 */
void_t DMA2_Stream0_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_0)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream1_IRQHandler
 */
void_t DMA2_Stream1_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_1)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream2_IRQHandler
 */
void_t DMA2_Stream2_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_2)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream3_IRQHandler
 */
void_t DMA2_Stream3_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_3)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream4_IRQHandler
 */
void_t DMA2_Stream4_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_4)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream5_IRQHandler
 */
void_t DMA2_Stream5_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_5)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream6_IRQHandler
 */
void_t DMA2_Stream6_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_6)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}

/*
 * Function: DMA2_Stream7_IRQHandler
 */
void_t DMA2_Stream7_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    u8_t idx = 0u;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (idx = 0u; idx < dmaConfigSize / sizeof(drv_dmaDescriptor_t); idx++)
    {
        if (dmaConfig[idx].dmaStream == DRV_DMA_2_STREAM_7)
        {
            HAL_DMA_IRQHandler(&dmaConfig[idx].hdma);
        }
        else
        {
            // Continue.
        }
    }

    GOS_ISR_EXIT
}
