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
//! @file       gpio_driver.c
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      GPIO driver source.
//! @details    For a more detailed description of this module, please refer to @ref gpio_driver.h
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
 * Includes
 */
#include "gpio_driver.h"
#include "iodef.h"

/*
 * Macros
 */
/**
 * Number of possible GPIO callbacks.
 */
#define GPIO_CALLBACK_NUM ( 8u )

/*
 * Static variables
 */
/**
 * GPIO callback descriptor array.
 */
GOS_STATIC gpio_callback_desc_t gpioCallbacks [GPIO_CALLBACK_NUM];

/*
 * Function: gpio_driverInit
 */
gos_result_t gpio_driverInit (void_t)
{
    /*
     * Local variables.
     */
    u8_t callbackIndex = 0u;

    /*
     * Function code.
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    for (callbackIndex = 0u; callbackIndex < GPIO_CALLBACK_NUM; callbackIndex++)
    {
        gpioCallbacks[callbackIndex].callback = NULL;
        gpioCallbacks[callbackIndex].pin      = 0xFFFF;
    }

    return GOS_SUCCESS;
}

/*
 * Function: gpio_driverInitPin
 */
gos_result_t gpio_driverInitPin (GOS_CONST gpio_desc_t* pDesc)
{
    /*
     * Local variables.
     */
    gos_result_t     gpioInitPinResult = GOS_ERROR;
    GPIO_InitTypeDef gpioInit          = {0};

    /*
     * Function code.
     */
    if (pDesc != NULL)
    {
        gpioInit.Pin       = pDesc->pin;
        gpioInit.Mode      = pDesc->mode;
        gpioInit.Pull      = pDesc->pull;
        gpioInit.Speed     = pDesc->speed;
        gpioInit.Alternate = pDesc->alternate;
        HAL_GPIO_Init(pDesc->port, &gpioInit);
        gpioInitPinResult  = GOS_SUCCESS;
    }
    else
    {
        // Descriptor is NULL pointer.
    }

    return gpioInitPinResult;
}

/*
 * Function: gpio_driverRegisterItCallback
 */
gos_result_t gpio_driverRegisterItCallback (gpio_callback_desc_t* pDesc)
{
    /*
     * Local variables.
     */
    u8_t         callbackIndex  = 0u;
    gos_result_t registerResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pDesc != NULL && pDesc->callback != NULL)
    {
        for (callbackIndex = 0u; callbackIndex < GPIO_CALLBACK_NUM; callbackIndex++)
        {
            if (gpioCallbacks[callbackIndex].callback == NULL)
            {
                gpioCallbacks[callbackIndex].callback = pDesc->callback;
                gpioCallbacks[callbackIndex].pin      = pDesc->pin;
                registerResult = GOS_SUCCESS;
                break;
            }
            else
            {
                // Continue.
            }
        }
    }
    else
    {
        // Callback or descriptor is NULL pointer.
    }

    return registerResult;
}

/*
 * Function: gpio_driverWritePin
 */
gos_result_t gpio_driverWritePin (GOS_CONST gpio_desc_t* pDesc, gpio_state_t state)
{
    /*
     * Local variables.
     */
    gos_result_t writePinResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pDesc != NULL && state <= GPIO_STATE_HIGH)
    {
        HAL_GPIO_WritePin(pDesc->port, pDesc->pin, state);
        writePinResult = GOS_SUCCESS;
    }
    else
    {
        // Descriptor is NULL pointer.
    }

    return writePinResult;
}

/*
 * Function: gpio_driverTogglePin
 */
gos_result_t gpio_driverTogglePin (GOS_CONST gpio_desc_t* pDesc)
{
    /*
     * Local variables.
     */
    gos_result_t togglePinResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pDesc != NULL)
    {
        HAL_GPIO_TogglePin(pDesc->port, pDesc->pin);
        togglePinResult = GOS_SUCCESS;
    }
    else
    {
        // Descriptor is NULL pointer.
    }

    return togglePinResult;
}

/*
 * Function: EXTI9_5_IRQHandler
 */
void_t EXTI15_10_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER
    HAL_GPIO_EXTI_IRQHandler(iodef_get(IO_USER_BUTTON)->pin);
    GOS_ISR_EXIT
}

/*
 * Function: HAL_GPIO_EXTI_Callback
 */
void_t HAL_GPIO_EXTI_Callback (u16_t GPIO_Pin)
{
    GOS_ISR_ENTER
    /*
     * Local variables.
     */
    u8_t callbackIndex = 0u;

    /*
     * Function code.
     */
    for (callbackIndex = 0u; callbackIndex < GPIO_CALLBACK_NUM; callbackIndex++)
    {
        if (gpioCallbacks[callbackIndex].pin == GPIO_Pin)
        {
            gpioCallbacks[callbackIndex].callback();
            break;
        }
        else
        {
            // Continue.
        }
    }
    GOS_ISR_EXIT
}
