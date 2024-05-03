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
//! @file       timer_driver.c
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      Timer driver source.
//! @details    For a more detailed description of this module, please refer to @ref timer_driver.h
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
#include "timer_driver.h"
#include "stm32f4xx.h"

/*
 * Static variables
 */
/**
 * Timer handle structure.
 */
GOS_STATIC TIM_HandleTypeDef htim12 = {0};

/*
 * Function: timer_driverSystimerInit
 */
gos_result_t timer_driverSystimerInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t systimerInitResult = GOS_ERROR;

    /*
     * Function code.
     */
    __HAL_RCC_TIM12_CLK_ENABLE();

    htim12.Instance               = TIM12;
    htim12.Init.Prescaler         = 84-1;
    htim12.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim12.Init.Period            = 0xffffffff-1;
    htim12.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim12)  == HAL_OK &&
        HAL_TIM_Base_Start(&htim12) == HAL_OK)
    {
        systimerInitResult = GOS_SUCCESS;
    }

    return systimerInitResult;
}

/*
 * Function: timer_driverSystimerGet
 */
gos_result_t timer_driverSystimerGet (u16_t* pValue)
{
    /*
     * Local variables.
     */
    gos_result_t timerDriverGetResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pValue != NULL)
    {
        *pValue              = __HAL_TIM_GET_COUNTER(&htim12);
        timerDriverGetResult = GOS_SUCCESS;
    }
    else
    {
        // Target is NULL pointer.
    }

    return timerDriverGetResult;
}
