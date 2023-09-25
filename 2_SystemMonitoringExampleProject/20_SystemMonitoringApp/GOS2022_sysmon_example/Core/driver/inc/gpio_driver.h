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
//! @file       gpio_driver.h
//! @author     Ahmed Gazar
//! @date       2023-09-14
//! @version    1.0
//!
//! @brief      GPIO driver header.
//! @details    This component provides access to GPIOs.
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
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

/*
 * Includes.
 */
#include "gos.h"
#include "stm32f4xx.h"

/*
 * Type definitions.
 */
/**
 * GPIO interrupt callback function type.
 */
typedef void_t (*gpio_it_callback_t)(void_t);

/**
 * GPIO callback descriptor type.
 */
typedef struct
{
    gpio_it_callback_t    callback; //!< Callback function.
    u16_t               pin;      //!< GPIO pin.
}gpio_callback_desc_t;

/**
 * GPIO descriptor type.
 */
typedef struct
{
    GPIO_TypeDef* port;      //!< GPIO port.
    u16_t         pin;       //!< GPIO pin.
    u32_t         mode;      //!< GPIO mode.
    u32_t         speed;     //!< GPIO speed.
    u32_t         pull;      //!< GPIO pull.
    u32_t         alternate; //!< GPIO alternate.
}gpio_desc_t;

/**
 * GPIO state enumerator.
 */
typedef enum
{
    GPIO_STATE_LOW = 0u, //!< GPIO low.
    GPIO_STATE_HIGH      //!< GPIO high.
}gpio_state_t;

/*
 * Function prototypes
 */
/**
 * @brief   Initializes the GPIO module.
 * @details Enabled the peripheral clocks and external interrupts.
 *
 * @return  Result of initialization.
 *
 * @retval  GOS_SUCCESS : Initialization successful.
 */
gos_result_t gpio_driverInit (void_t);

/**
 * @brief   Initializes the GPIO described by the descriptor parameter.
 * @details Performs the low-level GPIO initialization.
 *
 * @param   pDesc       : Pointer to the GPIO descriptor.
 *
 * @return  Result of pin initialization.
 *
 * @retval  GOS_SUCCESS : Initialization successful.
 * @retval  GOS_ERROR   : Descriptor is NULL pointer.
 */
gos_result_t gpio_driverInitPin (GOS_CONST gpio_desc_t* pDesc);

/**
 * @brief   Registers an interrupt callback.
 * @details Registers the interrupt callback in the internal callback array.
 *
 * @param   pDesc       : Pointer to the GPIO callback descriptor.
 *
 * @return  Result of callback registration.
 *
 * @retval  GOS_SUCCESS : Callback registration successful.
 * @retval  GOS_ERROR   : Descriptor or callback function pointer is NULL.
 */
gos_result_t gpio_driverRegisterItCallback (gpio_callback_desc_t* pDesc);

/**
 * @brief   Sets the given pin to the given state.
 * @details Sets the state of the given GPIO described by the descriptor structure
 *          to the desired value.
 *
 * @param   pDesc       : Pointer to the GPIO descriptor.
 * @param   state       : Desired GPIO state.
 *
 * @return  Result of pin setting.
 *
 * @retval  GOS_SUCCESS : Pin setting successful.
 * @retval  GOS_ERROR   : Descriptor is NULL pointer or state is invalid.
 */
gos_result_t gpio_driverWritePin (GOS_CONST gpio_desc_t* pDesc, gpio_state_t state);

/**
 * @brief   Toggles the given pin state.
 * @details Toggles the given pin state.
 *
 * @param   pDesc       : Pointer to the GPIO descriptor.
 *
 * @return  Result of pin toggling.
 *
 * @retval  GOS_SUCCESS : Pin state toggling successful.
 * @retval  GOS_ERROR   : Descriptor is NULL pointer.
 */
gos_result_t gpio_driverTogglePin (GOS_CONST gpio_desc_t* pDesc);

#endif
