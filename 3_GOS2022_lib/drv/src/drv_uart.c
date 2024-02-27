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
//! @date       2024-02-01
//! @version    1.0
//!
//! @brief      GOS2022 Library / UART driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_uart.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-01    Ahmed Gazar     Initial version created.
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
#include <string.h>
#include "stm32f4xx_hal.h"

/*
 * Static variables
 */
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

/*
 * Function: drv_uartInit
 */
gos_result_t drv_uartInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t             uartDriverInitResult = GOS_SUCCESS;
    drv_uartPeriphInstance_t instance             = 0u;
    u8_t                     uartIdx              = 0u;

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
    if (uartConfig != NULL && uartInstanceIndex < (uartConfigSize / sizeof(drv_uartDescriptor_t)))
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
        }
    }
    else
    {
        // Configuration missing or index is out of array boundary.
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
    if (uartConfig != NULL && uartInstanceIndex < (uartConfigSize / sizeof(drv_uartDescriptor_t)))
    {
        instance = uartConfig[uartInstanceIndex].periphInstance;

        if (HAL_UART_DeInit(&huarts[instance]) == HAL_OK)
        {
            uartDeInitResult = GOS_SUCCESS;
        }
        else
        {
            // De-init error.
        }
    }
    else
    {
        // Configuration missing or index is out of array boundary.
    }

    return uartDeInitResult;
}

/*
 * Function: drv_uartTraceTransmit
 */
GOS_INLINE gos_result_t drv_uartTraceTransmit (char_t* message)
{
    /*
     * Local variables.
     */
    gos_result_t             uartTransmitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_TRACE_INSTANCE];

        if (gos_mutexLock(&uartTxMutexes[instance], 2000u) == GOS_SUCCESS)
        {
            if (HAL_UART_Transmit_DMA(&huarts[instance], (u8_t*)message, strlen(message)) == HAL_OK &&
                gos_triggerWait(&uartTxReadyTriggers[instance], 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS &&
                gos_triggerReset(&uartTxReadyTriggers[instance]) == GOS_SUCCESS)
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
            }
        }
        else
        {
            // Mutex error.
        }

        (void_t) gos_mutexUnlock(&uartTxMutexes[instance]);
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartTransmitResult;
}

/*
 * Function: drv_uartTraceTransmitUnsafe
 */
GOS_INLINE gos_result_t drv_uartTraceTransmitUnsafe (char_t* message)
{
    /*
     * Local variables.
     */
    gos_result_t             uartTransmitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_TRACE_INSTANCE];
        GOS_DISABLE_SCHED

        if (HAL_UART_Abort(&huarts[instance]) == HAL_OK &&
            HAL_UART_Transmit(&huarts[instance], (u8_t*)message, strlen(message), 1000) == HAL_OK)
        {
            uartTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Error.
        }
        GOS_ENABLE_SCHED
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartTransmitResult;
}

/*
 * Function: drv_uartShellReceiveChar
 */
GOS_INLINE gos_result_t drv_uartShellReceiveChar (char_t* buffer)
{
    /*
     * Local variables.
     */
    gos_result_t             uartReceiveResult  = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_SHELL_INSTANCE];

        if (gos_mutexLock(&uartRxMutexes[instance], GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
        {
            if (HAL_UART_Receive_IT(&huarts[instance], (u8_t*)buffer, sizeof(char_t)) == HAL_OK &&
                gos_triggerWait(&uartRxReadyTriggers[instance], 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS &&
                gos_triggerReset(&uartRxReadyTriggers[instance]) == GOS_SUCCESS)
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
            }
        }
        else
        {
            // Mutex error.
        }

        (void_t) gos_mutexUnlock(&uartRxMutexes[instance]);
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartReceiveResult;
}

/*
 * Function: drv_uartShellTransmitString
 */
GOS_INLINE gos_result_t drv_uartShellTransmitString (char_t* pString)
{
    /*
     * Local variables.
     */
    gos_result_t             uartTransmitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_SHELL_INSTANCE];

        if (gos_mutexLock(&uartTxMutexes[instance], 2000u) == GOS_SUCCESS)
        {
            if (HAL_UART_Transmit_IT(&huarts[instance], (u8_t*)pString, strlen(pString)) == HAL_OK &&
                gos_triggerWait(&uartTxReadyTriggers[instance], 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS &&
                gos_triggerReset(&uartTxReadyTriggers[instance]) == GOS_SUCCESS)
            {
                /*
                 * At this point the caller task is unblocked,
                 * reception ready, we can return.
                 */
                uartTransmitResult = GOS_SUCCESS;
            }
            else
            {
                // Transmit or trigger error.
            }
        }
        else
        {
            // Mutex error.
        }

        (void_t) gos_mutexUnlock(&uartTxMutexes[instance]);
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartTransmitResult;
}

/*
 * Function: drv_uartSysmonTransmit
 */
GOS_INLINE gos_result_t drv_uartSysmonTransmit (u8_t* data, u16_t size)
{
    /*
     * Local variables.
     */
    gos_result_t             uartTransmitResult = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_SYSMON_INSTANCE];

        if (gos_mutexLock(&uartTxMutexes[instance], 5000u) == GOS_SUCCESS)
        {
            if (HAL_UART_Transmit_IT(&huarts[instance], data, size) == HAL_OK &&
                gos_triggerWait(&uartTxReadyTriggers[instance], 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS &&
                gos_triggerReset(&uartTxReadyTriggers[instance]) == GOS_SUCCESS)
            {
                /*
                 * At this point the caller task is unblocked,
                 * reception ready, we can return.
                 */
                uartTransmitResult = GOS_SUCCESS;
            }
            else
            {
                // Transmit or trigger error.
            }
        }
        else
        {
            // Mutex error.
        }

        (void_t) gos_mutexUnlock(&uartTxMutexes[instance]);
    }
    else
    {
        // Configuration array is NULL.
    }

    return uartTransmitResult;
}

/*
 * Function: drv_uartSysmonReceive
 */
GOS_INLINE gos_result_t drv_uartSysmonReceive (u8_t* data, u16_t size)
{
    /*
     * Local variables.
     */
    gos_result_t             uartReceiveResult  = GOS_ERROR;
    drv_uartPeriphInstance_t instance           = 0u;

    /*
     * Function code.
     */
    if (uartServiceConfig != NULL)
    {
        instance = uartServiceConfig[DRV_UART_SYSMON_INSTANCE];

        if (gos_mutexLock(&uartRxMutexes[instance], GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
        {
            if (HAL_UART_Receive_DMA(&huarts[instance], data, size) == HAL_OK &&
                gos_triggerWait(&uartRxReadyTriggers[instance], 1, 500) == GOS_SUCCESS &&
                gos_triggerReset(&uartRxReadyTriggers[instance]) == GOS_SUCCESS)
            {
                uartReceiveResult = GOS_SUCCESS;
            }
            else
            {
                HAL_UART_Abort_IT(&huarts[instance]);
            }
        }
        else
        {
            // Mutex error.
        }

        (void_t) gos_mutexUnlock(&uartRxMutexes[instance]);
    }
    else
    {
        // Configuration array is NULL.
    }

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


      if(pHuart->Instance==USART2)
      {
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);

        HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
      }
      else if (pHuart->Instance == UART5)
      {
            /* UART5 interrupt Init */
            HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(UART5_IRQn);

            /* DMA1_Stream0_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

            /* DMA1_Stream7_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
      }
      else if (pHuart->Instance == USART1)
      {
            /* USART1 interrupt Init */
            HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
      }
}
