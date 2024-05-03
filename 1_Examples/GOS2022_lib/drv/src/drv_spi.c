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
//! @file       drv_spi.c
//! @author     Ahmed Gazar
//! @date       2024-04-02
//! @version    1.1
//!
//! @brief      GOS2022 Library / SPI driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_spi.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-03-19    Ahmed Gazar     Initial version created.
// 1.1        2024-04-02    Ahmed Gazar     *    Complete callbacks made faster by removing loops
//                                          *    Trigger usage made optional by 0 timeout
//                                          *    Inline macros removed from functions
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
#include <drv_spi.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

/*
 * Static variables
 */
/**
 * SPI instance lookup table.
 */
GOS_STATIC SPI_TypeDef* spiInstanceLut [] =
{
    [DRV_SPI_INSTANCE_1] = SPI1,
    [DRV_SPI_INSTANCE_2] = SPI2,
    [DRV_SPI_INSTANCE_3] = SPI3,
    [DRV_SPI_INSTANCE_4] = SPI4
};

/**
 * SPI handles.
 */
GOS_STATIC SPI_HandleTypeDef hspis                [DRV_SPI_NUM_OF_INSTANCES];

/**
 * SPI mutexes.
 */
GOS_STATIC gos_mutex_t       spiMutexes           [DRV_SPI_NUM_OF_INSTANCES];

/**
 * SPI RX ready triggers.
 */
GOS_STATIC gos_trigger_t     spiRxReadyTriggers   [DRV_SPI_NUM_OF_INSTANCES];

/**
 * SPI TX ready triggers.
 */
GOS_STATIC gos_trigger_t     spiTxReadyTriggers   [DRV_SPI_NUM_OF_INSTANCES];

/**
 * SPI TX-RX ready triggers.
 */
GOS_STATIC gos_trigger_t     spiTxRxReadyTriggers [DRV_SPI_NUM_OF_INSTANCES];

/*
 * External variables
 */
/**
 * SPI configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_spiDescriptor_t     spiConfig [];

/**
 * SPI configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                             spiConfigSize;

/*
 * Function: drv_spiInit
 */
gos_result_t drv_spiInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverInitResult = GOS_SUCCESS;
    u8_t         spiIdx              = 0u;

    /*
     * Function code.
     */
    if (spiConfig != NULL)
    {
        for (spiIdx = 0u; spiIdx < spiConfigSize / sizeof(drv_spiDescriptor_t); spiIdx++)
        {
            GOS_CONCAT_RESULT(spiDriverInitResult, drv_spiInitInstance(spiIdx));
        }
    }
    else
    {
        // Configuration array is NULL pointer.
        spiDriverInitResult = GOS_ERROR;
    }

    return spiDriverInitResult;
}

/*
 * Function: drv_spiInitInstance
 */
gos_result_t drv_spiInitInstance (u8_t spiInstanceIndex)
{
    /*
     * Local variables.
     */
    gos_result_t            spiInitResult = GOS_ERROR;
    drv_spiPeriphInstance_t instance      = 0u;

    /*
     * Function code.
     */
    if (spiConfig != NULL && spiInstanceIndex < (spiConfigSize / sizeof(drv_spiDescriptor_t)))
    {
        instance = spiConfig[spiInstanceIndex].periphInstance;

        hspis[instance].Instance               = spiInstanceLut[instance];
        hspis[instance].Init.Mode              = spiConfig[spiInstanceIndex].mode;
        hspis[instance].Init.Direction         = spiConfig[spiInstanceIndex].direction;
        hspis[instance].Init.DataSize          = spiConfig[spiInstanceIndex].dataSize;
        hspis[instance].Init.CLKPolarity       = spiConfig[spiInstanceIndex].clkPolarity;
        hspis[instance].Init.CLKPhase          = spiConfig[spiInstanceIndex].clkPhase;
        hspis[instance].Init.NSS               = spiConfig[spiInstanceIndex].nss;
        hspis[instance].Init.BaudRatePrescaler = spiConfig[spiInstanceIndex].baudRatePrescaler;
        hspis[instance].Init.FirstBit          = spiConfig[spiInstanceIndex].firstBit;
        hspis[instance].Init.TIMode            = spiConfig[spiInstanceIndex].tiMode;
        hspis[instance].Init.CRCCalculation    = spiConfig[spiInstanceIndex].crcCalculation;
        hspis[instance].Init.CRCPolynomial     = spiConfig[spiInstanceIndex].crcPolynomial;

        if (HAL_SPI_Init    (&hspis[instance])                 == HAL_OK      &&
            gos_mutexInit   (&spiMutexes[instance])            == GOS_SUCCESS &&
            gos_triggerInit (&spiRxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerInit (&spiTxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerInit (&spiTxRxReadyTriggers[instance])  == GOS_SUCCESS &&
            gos_triggerReset(&spiRxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerReset(&spiTxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerReset(&spiTxRxReadyTriggers[instance])  == GOS_SUCCESS
            )
        {
            spiInitResult = GOS_SUCCESS;
        }
        else
        {
            // Init error.
        }
    }
    else
    {
        // Configuration missing or index is out of array boundary.
    }

    return spiInitResult;
}

/*
 * Function: drv_spiTransmitBlocking
 */
gos_result_t drv_spiTransmitBlocking (
        drv_spiPeriphInstance_t instance, u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t transmitTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverTransmitResult = GOS_ERROR;


    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_Transmit(&hspis[instance], pData, size, transmitTmo) == HAL_OK)
        {
            spiDriverTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverTransmitResult;
}

/*
 * Function: drv_spiReceiveBlocking
 */
gos_result_t drv_spiReceiveBlocking (
        drv_spiPeriphInstance_t instance, u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t receiveTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_Receive(&hspis[instance], pBuffer, size, receiveTmo) == HAL_OK)
        {
            spiDriverReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverReceiveResult;
}

/*
 * Function: drv_spiTransmitIT
 */
gos_result_t drv_spiTransmitIT (
        drv_spiPeriphInstance_t instance, u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_Transmit_IT(&hspis[instance], pData, size) == HAL_OK)
        {
            if (triggerTmo > 0u)
            {
                if (gos_triggerWait (&spiTxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                    gos_triggerReset(&spiTxReadyTriggers[instance])                == GOS_SUCCESS)
                {
                    spiDriverTransmitResult = GOS_SUCCESS;
                }
                else
                {
                    // Trigger error.
                }
            }
            else
            {
                spiDriverTransmitResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Transmit error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverTransmitResult;
}

/*
 * Function: drv_spiReceiveIT
 */
gos_result_t drv_spiReceiveIT (
        drv_spiPeriphInstance_t instance, u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_Receive_IT(&hspis[instance], pBuffer, size) == HAL_OK)
        {
            if (triggerTmo > 0u)
            {
                if (gos_triggerWait (&spiRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                    gos_triggerReset(&spiRxReadyTriggers[instance])                == GOS_SUCCESS)
                {
                    spiDriverReceiveResult = GOS_SUCCESS;
                }
                else
                {
                    // Trigger error.
                }
            }
            else
            {
                spiDriverReceiveResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Receive error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverReceiveResult;
}

/*
 * Function: drv_spiTransmitDMA
 */
gos_result_t drv_spiTransmitDMA (
        drv_spiPeriphInstance_t instance, u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_Transmit_DMA(&hspis[instance], pData, size) == HAL_OK)
        {
            if (triggerTmo > 0u)
            {
                if (gos_triggerWait (&spiTxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                    gos_triggerReset(&spiTxReadyTriggers[instance])                == GOS_SUCCESS)
                {
                    spiDriverTransmitResult = GOS_SUCCESS;
                }
                else
                {
                    // Trigger error.
                }
            }
            else
            {
                // Trigger is not used.
                spiDriverTransmitResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Transmit error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverTransmitResult;
}

/*
 * Function: drv_spiReceiveDMA
 */
gos_result_t drv_spiReceiveDMA (
        drv_spiPeriphInstance_t instance, u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_Receive_DMA(&hspis[instance], pBuffer, size) == HAL_OK)
        {
            if (triggerTmo > 0u)
            {
                if (gos_triggerWait (&spiRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                    gos_triggerReset(&spiRxReadyTriggers[instance])                == GOS_SUCCESS)
                {
                    spiDriverReceiveResult = GOS_SUCCESS;
                }
                else
                {
                    // Trigger error.
                }
            }
            else
            {
                spiDriverReceiveResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Receive error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverReceiveResult;
}

/*
 * Function: drv_spiTransmitReceiveBlocking
 */
gos_result_t drv_spiTransmitReceiveBlocking (
        drv_spiPeriphInstance_t instance, u8_t* pTxData,  u8_t* pRxData,
        u16_t                   size,     u32_t mutexTmo, u32_t transmitReceiveTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverTransmitReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_TransmitReceive(&hspis[instance], pTxData, pRxData, size, transmitReceiveTmo) == HAL_OK)
        {
            spiDriverTransmitReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverTransmitReceiveResult;
}

/*
 * Function: drv_spiTransmitReceiveIT
 */
gos_result_t drv_spiTransmitReceiveIT (
        drv_spiPeriphInstance_t instance, u8_t* pTxData,  u8_t* pRxData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverTransmitReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_TransmitReceive_IT(&hspis[instance], pTxData, pRxData, size) == HAL_OK)
        {
            if (triggerTmo > 0u)
            {
                if (gos_triggerWait (&spiTxRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                    gos_triggerReset(&spiTxRxReadyTriggers[instance])                == GOS_SUCCESS)
                {
                    spiDriverTransmitReceiveResult = GOS_SUCCESS;
                }
                else
                {
                    // Trigger error.
                }
            }
            else
            {
                spiDriverTransmitReceiveResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Transmit-receive error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverTransmitReceiveResult;
}

/*
 * Function: drv_spiTransmitReceiveDMA
 */
gos_result_t drv_spiTransmitReceiveDMA (
        drv_spiPeriphInstance_t instance, u8_t* pTxData,  u8_t* pRxData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t spiDriverTransmitReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&spiMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_SPI_TransmitReceive_DMA(&hspis[instance], pTxData, pRxData, size) == HAL_OK)
        {
            if (triggerTmo > 0u)
            {
                if (gos_triggerWait (&spiTxRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                    gos_triggerReset(&spiTxRxReadyTriggers[instance])                == GOS_SUCCESS)
                {
                    spiDriverTransmitReceiveResult = GOS_SUCCESS;
                }
                else
                {
                    // Trigger error.
                }
            }
            else
            {
                spiDriverTransmitReceiveResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Transmit-receive error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&spiMutexes[instance]);

    return spiDriverTransmitReceiveResult;
}

/*
 * Function: HAL_SPI_TxCpltCallback
 */
void_t HAL_SPI_TxCpltCallback (SPI_HandleTypeDef* hspi)
{
    /*
     * Function code.
     */
    if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&spiTxReadyTriggers[DRV_SPI_INSTANCE_1]);
    }
    else if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&spiTxReadyTriggers[DRV_SPI_INSTANCE_2]);
    }
    else if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_3])
    {
        (void_t) gos_triggerIncrement(&spiTxReadyTriggers[DRV_SPI_INSTANCE_3]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: HAL_SPI_RxCpltCallback
 */
void_t HAL_SPI_RxCpltCallback (SPI_HandleTypeDef* hspi)
{
    /*
     * Function code.
     */
    if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&spiRxReadyTriggers[DRV_SPI_INSTANCE_1]);
    }
    else if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&spiRxReadyTriggers[DRV_SPI_INSTANCE_2]);
    }
    else if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_3])
    {
        (void_t) gos_triggerIncrement(&spiRxReadyTriggers[DRV_SPI_INSTANCE_3]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: HAL_SPI_TxRxCpltCallback
 */
void_t HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef *hspi)
{
    /*
     * Function code.
     */
    if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&spiTxRxReadyTriggers[DRV_SPI_INSTANCE_1]);
    }
    else if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&spiTxRxReadyTriggers[DRV_SPI_INSTANCE_2]);
    }
    else if (hspi->Instance == spiInstanceLut[DRV_SPI_INSTANCE_3])
    {
        (void_t) gos_triggerIncrement(&spiTxRxReadyTriggers[DRV_SPI_INSTANCE_3]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: SPI1_IRQHandler
 */
void_t SPI1_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_SPI_IRQHandler(&hspis[DRV_SPI_INSTANCE_1]);

    GOS_ISR_EXIT
}

/*
 * Function: SPI2_IRQHandler
 */
void_t SPI2_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_SPI_IRQHandler(&hspis[DRV_SPI_INSTANCE_2]);

    GOS_ISR_EXIT
}

void_t SPI3_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_SPI_IRQHandler(&hspis[DRV_SPI_INSTANCE_3]);

    GOS_ISR_EXIT
}

/*
 * Function: SPI4_IRQHandler
 */
void_t SPI4_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_SPI_IRQHandler(&hspis[DRV_SPI_INSTANCE_4]);

    GOS_ISR_EXIT
}

/*
 * Function: HAL_SPI_MspInit
 */
void_t HAL_SPI_MspInit (SPI_HandleTypeDef* hspi)
{
    /*
     * Local variables.
     */
    drv_spiPeriphInstance_t instance = 0u;
    u8_t                    idx      = 0u;

    /*
     * Function code.
     */
    for (instance = 0u; instance < DRV_SPI_NUM_OF_INSTANCES; instance++)
    {
        if (spiInstanceLut[instance] == hspi->Instance)
        {
            for (idx = 0u; idx < spiConfigSize / sizeof(drv_spiDescriptor_t); idx++)
            {
                if (spiConfig[idx].periphInstance == instance)
                {
                    if (spiConfig[idx].dmaConfigRx != NULL)
                    {
                        __HAL_LINKDMA(hspi, hdmarx, spiConfig[idx].dmaConfigRx->hdma);
                    }
                    else
                    {
                        // There is no DMA assigned.
                    }

                    if (spiConfig[idx].dmaConfigTx != NULL)
                    {
                        __HAL_LINKDMA(hspi, hdmatx, spiConfig[idx].dmaConfigTx->hdma);
                    }
                    else
                    {
                        // There is no DMA assigned.
                    }
                }
                else
                {
                    // Continue.
                }
            }
        }
        else
        {
            // Continue.
        }
    }
}
