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
//! @file       drv_gpio.h
//! @author     Ahmed Gazar
//! @date       2024-02-27
//! @version    1.0
//!
//! @brief      GOS2022 Library / GPIO driver header.
//! @details    This component provides access to the GPIO peripheries.
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
#ifndef DRV_GPIO_H
#define DRV_GPIO_H

/*
 * Includes
 */
#include <gos.h>
#include "stm32f4xx_hal.h"

/*
 * Type definitions
 */
/**
 * GPIO interrupt callback function type.
 */
typedef void_t (*drv_gpioItCallback_t)(void_t);

/**
 * GPIO interrupt callback descriptor.
 */
typedef struct
{
    drv_gpioItCallback_t callback; //!< Callback function.
    u16_t                pin;      //!< GPIO pin.
}drv_gpioItCallbackDescriptor_t;

/**
 * GPIO descriptor.
 */
typedef struct
{
    GPIO_TypeDef* port;      //!< Port.
    u16_t         pin;       //!< Pin.
    u32_t         mode;      //!< Mode.
    u32_t         speed;     //!< Speed.
    u32_t         pull;      //!< Pull.
    u32_t         alternate; //!< Alternate.
}drv_gpioDescriptor_t;

/**
 * GPIO state enumerator.
 */
typedef enum
{
    GPIO_STATE_LOW = 0u, //!< GPIO low.
    GPIO_STATE_HIGH      //!< GPIO high.
}drv_gpioState_t;

/*
 * Function prototypes
 */
// TODO
gos_result_t drv_gpioInit (void_t);

// TODO
gos_result_t drv_gpioInitPin (u8_t pinIndex);

// TODO
gos_result_t drv_gpioDeInitPin (u8_t pinIndex);

// TODO
gos_result_t drv_gpioInitItPin (u8_t pinIndex);

// TODO
gos_result_t drv_gpioWritePin (u8_t pinIndex, drv_gpioState_t state);

// TODO
gos_result_t drv_gpioTgglePin (u8_t pinIndex);

// TODO
gos_result_t drv_gpioReadPin (u8_t pinIndex, drv_gpioState_t* pState);

#endif
