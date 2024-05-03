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
//                                      (c) Ahmed Gazar, 2023
//
//*************************************************************************************************
//! @file       uart_driver.c
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      UART driver source.
//! @details    For a more detailed description of this module, please refer to @ref uart_driver.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-09-14    Ahmed Gazar     Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2023 Ahmed Gazar
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
 * Includes.
 */
#include <string.h>
#include "uart_driver.h"
#include "stm32f4xx.h"

/*
 * Static variables.
 */
/**
 * UART2 handle structure.
 */
GOS_STATIC UART_HandleTypeDef huart2;
/**
 * UART5 handle structure.
 */
GOS_STATIC UART_HandleTypeDef huart5;

/**
 * UART2 DMA TX handle structure.
 */
GOS_STATIC DMA_HandleTypeDef  hdma_usart2_tx;

/**
 * UART5 DMA RX handle structure.
 */
GOS_STATIC DMA_HandleTypeDef  hdma_uart5_rx;

/**
 * UART2 TX mutex.
 */
GOS_STATIC gos_mutex_t        uart2TxMutex;
/**
 * UART5 RX mutex.
 */
GOS_STATIC gos_mutex_t        uart5RxMutex;
/**
 * UART5 TX mutex.
 */
GOS_STATIC gos_mutex_t        uart5TxMutex;

/**
 * UART2 TX ready trigger.
 */
GOS_STATIC gos_trigger_t      uart2TxReadyTrigger;
/**
 * UART5 RX ready trigger.
 */
GOS_STATIC gos_trigger_t      uart5RxReadyTrigger;
/**
 * UART5 TX ready trigger.
 */
GOS_STATIC gos_trigger_t      uart5TxReadyTrigger;

/*
 * Function: uart_driverInit
 */
gos_result_t uart_driverInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t uartDriverInitResult = GOS_SUCCESS;

    /*
     * Function code.
     */
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_UART5_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 115200;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        uartDriverInitResult = GOS_ERROR;
    }

    // Initialize mutex.
    uartDriverInitResult &= gos_mutexInit(&uart2TxMutex);

    // Initialize trigger.
    uartDriverInitResult &= gos_triggerInit(&uart2TxReadyTrigger);

    huart5.Instance          = UART5;
    huart5.Init.BaudRate     = 115200;
    huart5.Init.WordLength   = UART_WORDLENGTH_8B;
    huart5.Init.StopBits     = UART_STOPBITS_1;
    huart5.Init.Parity       = UART_PARITY_NONE;
    huart5.Init.Mode         = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
        uartDriverInitResult = GOS_ERROR;
    }

    // Initialize mutexes.
    uartDriverInitResult &= gos_mutexInit(&uart5RxMutex);
    uartDriverInitResult &= gos_mutexInit(&uart5TxMutex);

    // Initialize triggers.
    uartDriverInitResult &= gos_triggerInit(&uart5RxReadyTrigger);
    uartDriverInitResult &= gos_triggerInit(&uart5TxReadyTrigger);

    if (uartDriverInitResult != GOS_SUCCESS)
    {
        uartDriverInitResult = GOS_ERROR;
    }

    return uartDriverInitResult;
}

/*
 * Function: uart_driverTraceTransmitStringUnsafe
 */
gos_result_t uart_driverTraceTransmitStringUnsafe (char_t* message)
{
    /*
     * Local variables.
     */
    gos_result_t uartTransmitResult    = GOS_ERROR;

    /*
     * Function code.
     */
    GOS_DISABLE_SCHED

    if (HAL_UART_Abort(&huart2) == HAL_OK &&
       HAL_UART_Transmit(&huart2, (u8_t*)message, strlen(message), 1000) == HAL_OK)
    {
        uartTransmitResult = GOS_SUCCESS;
    }
    GOS_ENABLE_SCHED

    return uartTransmitResult;
}

/*
 * Function: uart_driverTraceTransmitString
 */
GOS_INLINE gos_result_t uart_driverTraceTransmitString (char_t* message)
{
    /*
     * Local variables.
     */
    gos_result_t uartTransmitResult    = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uart2TxMutex, 2000u) == GOS_SUCCESS)
    {
        if (HAL_UART_Transmit_DMA(&huart2, (u8_t*)message, strlen(message)) == HAL_OK &&
            gos_triggerWait(&uart2TxReadyTrigger, 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS)
        {
            gos_triggerReset(&uart2TxReadyTrigger);
            /*
             * At this point the caller task is unblocked,
             * transmission ready, we can return.
             */
            uartTransmitResult = GOS_SUCCESS;
        }
    }

    gos_mutexUnlock(&uart2TxMutex);

    return uartTransmitResult;
}

/*
 * Function: uart_driverShellReceiveChar
 */
GOS_INLINE gos_result_t uart_driverShellReceiveChar (char_t* buffer)
{
    /*
     * Local variables.
     */
    gos_result_t uartReceiveResult    = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uart5RxMutex, GOS_MUTEX_ENDLESS_TMO) == GOS_SUCCESS)
    {
        if (HAL_UART_Receive_IT(&huart5, (u8_t*)buffer, sizeof(char_t)) == HAL_OK &&
            gos_triggerWait(&uart5RxReadyTrigger, 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS)
        {
            gos_triggerReset(&uart5RxReadyTrigger);
            /*
             * At this point the caller task is unblocked,
             * reception ready, we can return.
             */
            uartReceiveResult = GOS_SUCCESS;
        }
        gos_mutexUnlock(&uart5RxMutex);
    }

    return uartReceiveResult;
}

/*
 * Function: uart_driverShellTransmitString
 */
GOS_INLINE gos_result_t uart_driverShellTransmitString (char_t* pString)
{
    /*
     * Local variables.
     */
    gos_result_t uartTransmitResult    = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&uart5TxMutex, 2000u) == GOS_SUCCESS)
    {
        if (HAL_UART_Transmit_IT(&huart5, (u8_t*)pString, strlen(pString)) == HAL_OK &&
            gos_triggerWait(&uart5TxReadyTrigger, 1, GOS_TRIGGER_ENDLESS_TMO) == GOS_SUCCESS)
        {
            gos_triggerReset(&uart5TxReadyTrigger);
            /*
             * At this point the caller task is unblocked,
             * reception ready, we can return.
             */
            uartTransmitResult = GOS_SUCCESS;
        }
        gos_mutexUnlock(&uart5TxMutex);
    }

    return uartTransmitResult;
}

/*
 * Function: HAL_UART_MspInit
 */
void_t HAL_UART_MspInit (UART_HandleTypeDef* pHuart)
{
    /*
     * Function code.
     */
    if (pHuart->Instance == USART2)
    {
        /* USART2 DMA Init */
        /* USART2_TX Init */
        hdma_usart2_tx.Instance                 = DMA1_Stream6;
        hdma_usart2_tx.Init.Channel             = DMA_CHANNEL_4;
        hdma_usart2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_usart2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_usart2_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_usart2_tx.Init.Mode                = DMA_NORMAL;
        hdma_usart2_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_usart2_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_usart2_tx);

        __HAL_LINKDMA(pHuart,hdmatx,hdma_usart2_tx);

        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);

        HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    }
    else if (pHuart->Instance == UART5)
    {
        /* UART5 DMA Init */
        /* UART5_RX Init */
        hdma_uart5_rx.Instance                 = DMA1_Stream0;
        hdma_uart5_rx.Init.Channel             = DMA_CHANNEL_4;
        hdma_uart5_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_uart5_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart5_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart5_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart5_rx.Init.Mode                = DMA_NORMAL;
        hdma_uart5_rx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart5_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_uart5_rx);

        __HAL_LINKDMA(pHuart,hdmarx,hdma_uart5_rx);

        /* UART5 interrupt Init */
        HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(UART5_IRQn);

        /* DMA1_Stream0_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    }
    else
    {
        // Nothing to do.
    }
}


/*
 * Function: USART2_IRQHandler
 */
void_t USART2_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huart2);

    GOS_ISR_EXIT
}

/*
 * Function: UART5_IRQHandler
 */
void_t UART5_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_UART_IRQHandler(&huart5);

    GOS_ISR_EXIT
}

/*
 * Function: HAL_UART_TxCpltCallback
 */
void_t HAL_UART_TxCpltCallback (UART_HandleTypeDef *pHuart)
{
    /*
     * Function code.
     */
    if (pHuart->Instance == USART2)
    {
        gos_triggerIncrement(&uart2TxReadyTrigger);
    }
    else if (pHuart->Instance == UART5)
    {
        gos_triggerIncrement(&uart5TxReadyTrigger);
    }
    else
    {
        // Nothing to do.
    }
}

/*
 * Function: HAL_UART_RxCpltCallback
 */
void_t HAL_UART_RxCpltCallback (UART_HandleTypeDef *pHuart)
{
    /*
     * Function code.
     */
    if (pHuart->Instance == UART5)
    {
        gos_triggerIncrement(&uart5RxReadyTrigger);
    }
    else
    {
        // Nothing to do.
    }
}

/*
 * Function: DMA1_Stream0_IRQHandler
 */
void_t DMA1_Stream0_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    HAL_DMA_IRQHandler(&hdma_uart5_rx);
}

/*
 * Function: DMA1_Stream6_IRQHandler
 */
void_t DMA1_Stream6_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
}
