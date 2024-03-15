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
//! @file       drv_it.c
//! @author     Ahmed Gazar
//! @date       2024-03-08
//! @version    1.0
//!
//! @brief      GOS2022 Library / IT driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_it.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-03-08    Ahmed Gazar     Initial version created.
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
#include <drv_it.h>
#include "stm32f4xx_hal.h"

/*
 * External variables
 */
/**
 * IT configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_itDescriptor_t itConfig [];

/**
 * IT configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                        itConfigSize;

/*
 * Function: drv_itInit
 */
gos_result_t drv_itInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t itDriverInitResult = GOS_ERROR;
    u8_t         itIdx              = 0u;

    /*
     * Function code.
     */
    if (itConfig != NULL)
    {
        for (itIdx = 0u; itIdx < itConfigSize / sizeof(drv_itDescriptor_t); itIdx++)
        {
            HAL_NVIC_SetPriority(itConfig[itIdx].irqType, itConfig[itIdx].preemtPriority, itConfig[itIdx].subPriority);
            HAL_NVIC_EnableIRQ(itConfig[itIdx].irqType);
        }

        itDriverInitResult = GOS_SUCCESS;
    }
    else
    {
        // Configuration array is NULL pointer.
    }

    return itDriverInitResult;
}
