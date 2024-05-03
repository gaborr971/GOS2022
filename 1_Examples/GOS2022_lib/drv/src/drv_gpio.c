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
//! @file       drv_gpio.c
//! @author     Ahmed Gazar
//! @date       2024-02-27
//! @version    1.0
//!
//! @brief      GOS2022 Library / GPIO driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_gpio.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-27    Ahmed Gazar     Initial version created.
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
#include <drv_gpio.h>

/*
 * External variables
 */
/**
 * GPIO configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_gpioDescriptor_t           gpioConfig [];

/**
 * GPIO configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                                    gpioConfigSize;

/**
 * GPIO interrupt configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_gpioItCallbackDescriptor_t gpioItConfig [];

/**
 * GPIO interrupt configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                                    gpioItConfigSize;

/*
 * Function: drv_gpioInit
 */
gos_result_t drv_gpioInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t gpioInitResult = GOS_SUCCESS;
    u8_t         gpioIdx        = 0u;

    /*
     * Function code.
     */
    if (gpioConfig != NULL)
    {
        for (gpioIdx = 0u; gpioIdx < gpioConfigSize / sizeof(drv_gpioDescriptor_t); gpioIdx++)
        {
            GOS_CONCAT_RESULT(gpioInitResult, drv_gpioInitPin(gpioIdx));
        }
    }
    else
    {
        // Configuration array is NULL pointer.
    }

    return gpioInitResult;
}

/*
 * Function: drv_gpioInitPin
 */
gos_result_t drv_gpioInitPin (u8_t pinIndex)
{
    /*
     * Local variables.
     */
    gos_result_t     gpioInitPinResult = GOS_ERROR;
    GPIO_InitTypeDef gpioInit          = {0};

    /*
     * Function code.
     */
    if (pinIndex < (gpioConfigSize / sizeof(drv_gpioDescriptor_t)))
    {
        gpioInit.Pin       = gpioConfig[pinIndex].pin;
        gpioInit.Mode      = gpioConfig[pinIndex].mode;
        gpioInit.Pull      = gpioConfig[pinIndex].pull;
        gpioInit.Speed     = gpioConfig[pinIndex].speed;
        gpioInit.Alternate = gpioConfig[pinIndex].alternate;

        HAL_GPIO_Init(gpioConfig[pinIndex].port, &gpioInit);

        gpioInitPinResult = GOS_SUCCESS;
    }
    else
    {
        // Index error.
    }

    return gpioInitPinResult;
}

/*
 * Function: drv_gpioDeInitPin
 */
gos_result_t drv_gpioDeInitPin (u8_t pinIndex)
{
    /*
     * Local variables.
     */
    gos_result_t gpioDeInitPinResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pinIndex < (gpioConfigSize / sizeof(drv_gpioDescriptor_t)))
    {
        HAL_GPIO_DeInit(gpioConfig[pinIndex].port, gpioConfig[pinIndex].pin);

        gpioDeInitPinResult = GOS_SUCCESS;
    }
    else
    {
        // Index error.
    }

    return gpioDeInitPinResult;
}

/*
 * Function: drv_gpioWritePin
 */
gos_result_t drv_gpioWritePin (u8_t pinIndex, drv_gpioState_t state)
{
    /*
     * Local variables.
     */
    gos_result_t gpioWritePinResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pinIndex < gpioConfigSize / sizeof(drv_gpioDescriptor_t) && state >= GPIO_STATE_LOW &&
        state <= GPIO_STATE_HIGH)
    {
        HAL_GPIO_WritePin(gpioConfig[pinIndex].port, gpioConfig[pinIndex].pin, state);
        gpioWritePinResult = GOS_SUCCESS;
    }
    else
    {
        // Wrong index or invalid state.
    }

    return gpioWritePinResult;
}

/*
 * Function: drv_gpioTgglePin
 */
gos_result_t drv_gpioTgglePin (u8_t pinIndex)
{
    /*
     * Local variables.
     */
    gos_result_t gpioTogglePinResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pinIndex < gpioConfigSize / sizeof(drv_gpioDescriptor_t))
    {
        HAL_GPIO_TogglePin(gpioConfig[pinIndex].port, gpioConfig[pinIndex].pin);
        gpioTogglePinResult = GOS_SUCCESS;
    }
    else
    {
        // Wrong index or invalid state.
    }

    return gpioTogglePinResult;
}

/*
 * Function: drv_gpioReadPin
 */
gos_result_t drv_gpioReadPin (u8_t pinIndex, drv_gpioState_t* pState)
{
    /*
     * Local variables.
     */
    gos_result_t gpioReadPinResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pinIndex < gpioConfigSize / sizeof(drv_gpioDescriptor_t) && pState != NULL)
    {
        *pState = HAL_GPIO_ReadPin(gpioConfig[pinIndex].port, gpioConfig[pinIndex].pin);
        gpioReadPinResult = GOS_SUCCESS;
    }
    else
    {
        // Wrong index or pState is NULL.
    }

    return gpioReadPinResult;
}

/*
 * Function: EXTI9_5_IRQHandler
 */
void_t EXTI9_5_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    GOS_ISR_EXIT
}

/*
 * Function: EXTI15_10_IRQHandler
 */
void_t EXTI15_10_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    GOS_ISR_EXIT
}

/*
 * Function: HAL_GPIO_EXTI_Falling_Callback
 */
void_t HAL_GPIO_EXTI_Falling_Callback (u16_t pin)
{
    /*
     * Function code.
     */
    HAL_GPIO_EXTI_Callback(pin);
}

/*
 * Function: HAL_GPIO_EXTI_Rising_Callback
 */
void_t HAL_GPIO_EXTI_Rising_Callback (u16_t pin)
{
    /*
     * Function code.
     */
    HAL_GPIO_EXTI_Callback(pin);
}

/*
 * Function: HAL_GPIO_EXTI_Callback
 */
void_t HAL_GPIO_EXTI_Callback (u16_t pin)
{
    GOS_ISR_ENTER
    /*
     * Local variables.
     */
    u8_t callbackIndex = 0u;

    /*
     * Function code.
     */
    if (gpioItConfig != NULL)
    {
        for (callbackIndex = 0u; callbackIndex < gpioItConfigSize / sizeof(drv_gpioItCallbackDescriptor_t); callbackIndex++)
        {
            if (gpioItConfig[callbackIndex].pin == pin && gpioItConfig[callbackIndex].callback != NULL)
            {
                gpioItConfig[callbackIndex].callback();
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
        // Configuration array is NULL.
    }

    GOS_ISR_EXIT
}
