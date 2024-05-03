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
//! @file       drv_uart.c
//! @author     Ahmed Gazar
//! @date       2024-04-24
//! @version    1.1
//!
//! @brief      GOS2022 Library / UART driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_uart.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-01    Ahmed Gazar     Initial version created.
// 1.1        2024-04-24    Ahmed Gazar     +    Error reporting added
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
#include <drv_error.h>
#include <drv_uart.h>
#include <string.h>
#include "stm32f4xx_hal.h"

/*
 * Static variables
 */
/**
 * UART error flags.
 */
GOS_STATIC u32_t uartErrorFlags = 0u;

/**
 * UART instance lookup table.
 */
GOS_STATIC USART_TypeDef* uartInstanceLut [] =
{
    [DRV_UART_INSTANCE_1] = USART1,
    [DRV_UART_INSTANCE_2] = USART2,
    [DRV_UART_INSTANCE_3] = USART3,
    [DRV_UART_INSTANCE_4] = UART4,
    [DRV_UART_INSTANCE_5] = UART5,
    [DRV_UART_INSTANCE_6] = USART6,
};

/**
 * UART handles.
 */
GOS_STATIC UART_HandleTypeDef huarts              [DRV_UART_NUM_OF_INSTANCES];

/**
 * UART RX mutexes.
 */
GOS_STATIC gos_mutex_t        uartRxMutexes       [DRV_UART_NUM_OF_INSTANCES];

/**
 * UART TX mutexes.
 */
GOS_STATIC gos_mutex_t        uartTxMutexes       [DRV_UART_NUM_OF_INSTANCES];

/**
 * UART RX ready triggers.
 */
GOS_STATIC gos_trigger_t      uartRxReadyTriggers [DRV_UART_NUM_OF_INSTANCES];

/**
 * UART TX ready triggers.
 */
GOS_STATIC gos_trigger_t      uartTxReadyTriggers [DRV_UART_NUM_OF_INSTANCES];

/*
 * External variables
 */
/**
 * UART configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_uartDescriptor_t     uartConfig [];

/**
 * UART configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                              uartConfigSize;

/**
 * UART service configuration (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_uartPeriphInstance_t uartServiceConfig [];

/**
 * UART service timeout configuration.
 */
GOS_EXTERN drv_uartServiceTimeoutConfig_t     uartServiceTmoConfig;

/*
 * Function: drv_uartInit
 */
gos_result_t drv_uartInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t uartDriverInitResult = GOS_SUCCESS;
    u8_t         uartIdx              = 0u;

    /*
     * Function code.
     */
    if (uartConfig != NULL)
    {
        for (uartIdx = 0u; uartIdx < uartConfigSize / sizeof(drv_uartDescriptor_t); uartIdx++)
        {
            GOS_CONCAT_RESULT(uartDriverInitResult, drv_uartInitInstance(uartIdx));
        }
    }
    else
    {
        // Configuration array is NULL pointer.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_CFG_ARRAY_NULL);
        uartDriverInitResult = GOS_ERROR;
    }

    return uartDriverInitResult;
}

/*
 * Function: drv_uartInitInstance
 */
gos_result_t drv_uartInitInstance (u8_t uartInstanceIndex)
{
    /*
     * Local variables.
     */
    gos_result_t             uartInitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance       = 0u;

    /*
     * Function code.
     */
    if (uartConfig != NULL)
    {
        if (uartInstanceIndex < (uartConfigSize / sizeof(drv_uartDescriptor_t)))
        {
            instance = uartConfig[uartInstanceIndex].periphInstance;

            huarts[instance].Instance          = uartInstanceLut[instance];
            huarts[instance].Init.BaudRate     = uartConfig[uartInstanceIndex].baudRate;
            huarts[instance].Init.WordLength   = uartConfig[uartInstanceIndex].wordLength;
            huarts[instance].Init.StopBits     = uartConfig[uartInstanceIndex].stopBits;
            huarts[instance].Init.Parity       = uartConfig[uartInstanceIndex].parity;
            huarts[instance].Init.Mode         = uartConfig[uartInstanceIndex].mode;
            huarts[instance].Init.HwFlowCtl    = uartConfig[uartInstanceIndex].hwFlowControl;
            huarts[instance].Init.OverSampling = uartConfig[uartInstanceIndex].overSampling;

            if (HAL_UART_Init   (&huarts[instance])              == HAL_OK      &&
                gos_mutexInit   (&uartRxMutexes[instance])       == GOS_SUCCESS &&
                gos_mutexInit   (&uartTxMutexes[instance])       == GOS_SUCCESS &&
                gos_triggerInit (&uartRxReadyTriggers[instance]) == GOS_SUCCESS &&
                gos_triggerInit (&uartTxReadyTriggers[instance]) == GOS_SUCCESS &&
                gos_triggerReset(&uartRxReadyTriggers[instance]) == GOS_SUCCESS &&
                gos_triggerReset(&uartTxReadyTriggers[instance]) == GOS_SUCCESS
                )
            {
                uartInitResult = GOS_SUCCESS;
            }
            else
            {
                // Init error.
                DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_INSTANCE_INIT);
            }
        }
        else
        {
            // Index is out of array boundary.
            DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_INDEX_OUT_OF_BOUND);
        }
    }
    else
    {
        // Configuration is NULL.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_CFG_ARRAY_NULL);
    }

    return uartInitResult;
}

/*
 * Function: drv_uartDeInitInstance
 */
gos_result_t drv_uartDeInitInstance (u8_t uartInstanceIndex)
{
    /*
     * Local variables.
     */
    gos_result_t             uartDeInitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance         = 0u;

    /*
     * Function code.
     */
    if (uartConfig != NULL)
    {
        if (uartInstanceIndex < (uartConfigSize / sizeof(drv_uartDescriptor_t)))
        {
            instance = uartConfig[uartInstanceIndex].periphInstance;

            if (HAL_UART_DeInit(&huarts[instance]) == HAL_OK)
            {
                uartDeInitResult = GOS_SUCCESS;
            }
            else
            {
                // De-init error.
                DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_INSTANCE_DEINIT);
            }
        }
        else
        {
            // Index is out of array boundary.
            DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_INDEX_OUT_OF_BOUND);
        }
    }
    else
    {
        // Configuration is NULL.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_CFG_ARRAY_NULL);
    }

    return uartDeInitResult;
}

/*
 * Function: drv_uartGetErrorFlags
 */
gos_result_t drv_uartGetErrorFlags (u32_t* pErrorFlags)
{
    /*
     * Local variables.
     */
    gos_result_t uartGetErrorFlagsResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pErrorFlags != NULL)
    {
        *pErrorFlags = uartErrorFlags;
        uartGetErrorFlagsResult = GOS_SUCCESS;
    }
    else
    {
        // NULL pointer.
    }

    return uartGetErrorFlagsResult;
}

/*
 * Function: drv_uartClearErrorFlags
 */
gos_result_t drv_uartClearErrorFlags (u32_t errorFlags)
{
    /*
     * Local variables.
     */
    gos_result_t uartClearErrorFlagsResult = GOS_SUCCESS;

    /*
     * Function code.
     */
    DRV_ERROR_CLEAR(uartErrorFlags, errorFlags);

    return uartClearErrorFlagsResult;
}

/*
 * Function: drv_uartTransmitBlocking
 */
GOS_INLINE gos_result_t drv_uartTransmitBlocking (
        drv_uartPeriphInstance_t instance, u8_t* message,
        u16_t                    size,     u32_t timeout
        )
{
    /*
     * Local variables.
     */
    gos_result_t uartTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    GOS_DISABLE_SCHED

    if (HAL_UART_Abort   (&huarts[instance])                         == HAL_OK &&
        HAL_UART_Transmit(&huarts[instance], message, size, timeout) == HAL_OK)
    {
        uartTransmitResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_TX_BLOCKING);
    }
    GOS_ENABLE_SCHED

    return uartTransmitResult;
}

/*
 * Function: drv_uartTransmitDMA
 */
GOS_INLINE gos_result_t drv_uartTransmitDMA (
        drv_uartPeriphInstance_t instance, u8_t* message,
        u16_t                    size,     u32_t mutexTmo,
        u32_t                    triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t uartTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uartTxMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_UART_Transmit_DMA(&huarts[instance], message, size)        == HAL_OK      &&
            gos_triggerWait(&uartTxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset(&uartTxReadyTriggers[instance])               == GOS_SUCCESS)
        {
            /*
             * At this point the caller task is unblocked,
             * transmission ready, we can return.
             */
            uartTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit or trigger error.
            (void_t) HAL_UART_Abort_IT(&huarts[instance]);
            DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_TX_DMA_TRIG_HAL);
        }
    }
    else
    {
        // Mutex error.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_TX_DMA_MUTEX);
    }

    (void_t) gos_mutexUnlock(&uartTxMutexes[instance]);

    return uartTransmitResult;
}

/*
 * Function: drv_uartReceiveDMA
 */
GOS_INLINE gos_result_t drv_uartReceiveDMA (
        drv_uartPeriphInstance_t instance, u8_t* message,
        u16_t                    size,     u32_t mutexTmo,
        u32_t                    triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t uartReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uartRxMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_UART_Receive_DMA(&huarts[instance], message, size)         == HAL_OK      &&
            gos_triggerWait(&uartRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset(&uartRxReadyTriggers[instance])               == GOS_SUCCESS)
        {
            uartReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive or trigger error.
            (void_t) HAL_UART_Abort_IT(&huarts[instance]);
            DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_RX_DMA_TRIG_HAL);
        }
    }
    else
    {
        // Mutex error.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_RX_DMA_MUTEX);
    }

    (void_t) gos_mutexUnlock(&uartRxMutexes[instance]);

    return uartReceiveResult;
}

/*
 * Function: drv_uartTransmitIT
 */
GOS_INLINE gos_result_t drv_uartTransmitIT (
        drv_uartPeriphInstance_t instance, u8_t* message,
        u16_t                    size,     u32_t mutexTmo,
        u32_t                    triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t uartTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uartTxMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_UART_Transmit_IT(&huarts[instance], message, size)         == HAL_OK      &&
            gos_triggerWait(&uartTxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset(&uartTxReadyTriggers[instance])               == GOS_SUCCESS)
        {
            /*
             * At this point the caller task is unblocked,
             * transmission ready, we can return.
             */
            uartTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit or trigger error.
            (void_t) HAL_UART_Abort_IT(&huarts[instance]);
            DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_TX_IT_TRIG_HAL);
        }
    }
    else
    {
        // Mutex error.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_TX_IT_MUTEX);
    }

    (void_t) gos_mutexUnlock(&uartTxMutexes[instance]);

    return uartTransmitResult;
}

/*
 * Function: drv_uartReceiveIT
 */
GOS_INLINE gos_result_t drv_uartReceiveIT (drv_uartPeriphInstance_t instance, u8_t* message, u16_t size, u32_t mutexTmo, u32_t triggerTmo)
{
    /*
     * Local variables.
     */
    gos_result_t uartReceiveResult  = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uartRxMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_UART_Receive_IT(&huarts[instance], message, size)          == HAL_OK      &&
            gos_triggerWait(&uartRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset(&uartRxReadyTriggers[instance])               == GOS_SUCCESS)
        {
            /*
             * At this point the caller task is unblocked,
             * reception ready, we can return.
             */
            uartReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive or trigger error.
            (void_t) HAL_UART_Abort_IT(&huarts[instance]);
            DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_RX_IT_TRIG_HAL);
        }
    }
    else
    {
        // Mutex error.
        DRV_ERROR_SET(uartErrorFlags, DRV_ERROR_UART_RX_IT_MUTEX);
    }

    (void_t) gos_mutexUnlock(&uartRxMutexes[instance]);

    return uartReceiveResult;
}

/*
 * Function: USART1_IRQHandler
 */
void_t USART1_IRQHandler (void_t)
{
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huarts[DRV_UART_INSTANCE_1]);

    GOS_ISR_EXIT
}

/*
 * Function: USART2_IRQHandler
 */
void_t USART2_IRQHandler (void_t)
{
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huarts[DRV_UART_INSTANCE_2]);

    GOS_ISR_EXIT
}

/*
 * Function: USART3_IRQHandler
 */
void_t USART3_IRQHandler (void_t)
{
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huarts[DRV_UART_INSTANCE_3]);

    GOS_ISR_EXIT
}

/*
 * Function: UART4_IRQHandler
 */
void_t UART4_IRQHandler (void_t)
{
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huarts[DRV_UART_INSTANCE_4]);

    GOS_ISR_EXIT
}

/*
 * Function: UART5_IRQHandler
 */
void_t UART5_IRQHandler (void_t)
{
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huarts[DRV_UART_INSTANCE_5]);

    GOS_ISR_EXIT
}

/*
 * Function: USART6_IRQHandler
 */
void_t USART6_IRQHandler (void_t)
{
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huarts[DRV_UART_INSTANCE_6]);

    GOS_ISR_EXIT
}

/*
 * Function: HAL_UART_TxCpltCallback
 */
void_t HAL_UART_TxCpltCallback (UART_HandleTypeDef *pHuart)
{
    /*
     * Local variables.
     */
    drv_uartPeriphInstance_t instance = DRV_UART_INSTANCE_1;

    /*
     * Function code.
     */
    for (instance = DRV_UART_INSTANCE_1; instance < DRV_UART_NUM_OF_INSTANCES; instance++)
    {
        if (uartInstanceLut[instance] == pHuart->Instance)
        {
            (void_t) gos_triggerIncrement(&uartTxReadyTriggers[instance]);
            break;
        }
        else
        {
            // Continue.
        }
    }
}

/*
 * Function: HAL_UART_RxCpltCallback
 */
void_t HAL_UART_RxCpltCallback (UART_HandleTypeDef *pHuart)
{
    /*
     * Local variables.
     */
    drv_uartPeriphInstance_t instance = DRV_UART_INSTANCE_1;

    /*
     * Function code.
     */
    for (instance = DRV_UART_INSTANCE_1; instance < DRV_UART_NUM_OF_INSTANCES; instance++)
    {
        if (uartInstanceLut[instance] == pHuart->Instance)
        {
            (void_t) gos_triggerIncrement(&uartRxReadyTriggers[instance]);
            break;
        }
        else
        {
            // Continue.
        }
    }
}

/*
 * Function: HAL_UART_MspInit
 */
void_t HAL_UART_MspInit (UART_HandleTypeDef* pHuart)
{
    /*
     * Local variables.
     */
    drv_uartPeriphInstance_t instance = 0u;
    u8_t                     idx      = 0u;

    /*
     * Function code.
     */
    for (instance = 0u; instance < DRV_UART_NUM_OF_INSTANCES; instance++)
    {
        if (uartInstanceLut[instance] == pHuart->Instance)
        {
            for (idx = 0u; idx < uartConfigSize / sizeof(drv_uartDescriptor_t); idx++)
            {
                if (uartConfig[idx].periphInstance == instance)
                {
                    if (uartConfig[idx].dmaConfigRx != NULL)
                    {
                        __HAL_LINKDMA(pHuart, hdmarx, uartConfig[idx].dmaConfigRx->hdma);
                    }
                    else
                    {
                        // There is no DMA assigned.
                    }

                    if (uartConfig[idx].dmaConfigTx != NULL)
                    {
                        __HAL_LINKDMA(pHuart, hdmatx, uartConfig[idx].dmaConfigTx->hdma);
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
