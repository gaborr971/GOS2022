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
//                                      (c) Gabor Repasi, 2023
//
//*************************************************************************************************
//! @file       gos_trigger.h
//! @author     Gabor Repasi
//! @date       2023-01-11
//! @version    1.0
//!
//! @brief      GOS trigger service header.
//! @details    Trigger service is a way of synchronizing tasks. A trigger instance works as a
//!             counter. A trigger can be incremented, and it can be waited on. When waiting for
//!             a trigger, the caller must specify a desired trigger value to be reached. Until
//!             the value is reached, the caller will be sent to blocked state. Once the value is
//!             reached, the caller is unblocked, and trigger value will be set to 0.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-01-11    Gabor Repasi    Initial version created
//*************************************************************************************************
//
// Copyright (c) 2023 Gabor Repasi
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
#ifndef GOS_TRIGGER_H
#define GOS_TRIGGER_H
/*
 * Includes
 */
#include <gos_kernel.h>

/*
 * Macros
 */
/**
 * Invalid trigger ID.
 */
#define GOS_TRIGGER_INVALID_ID      ( 0x0 )

/*
 * Type definitions
 */
/**
 * Trigger identifier type.
 */
typedef u16_t gos_triggerId_t;

#if CFG_TRIGGER_MAX_NUMBER < 255
typedef u8_t  gos_triggerIndex_t;          //!< Trigger index type.
#else
typedef u16_t gos_triggerIndex_t;          //!< Trigger index type.
#endif

#if CFG_TRIGGER_MAX_WAITERS < 255
typedef u8_t  gos_triggerWaiterIndex_t;    //!< Trigger waiter index type.
#else
typedef u16_t gos_triggerWaiterIndex_t;    //!< Trigger waiter index type.
#endif

/*
 * Function prototypes
 */
/**
 * @brief   Initializes the trigger service.
 * @details Initializes the internal waiter array.
 *
 * @return  Result of initialization.
 *
 * @retval  GOS_SUCCESS : Initialization successful.
 */
gos_result_t gos_triggerInit (void_t);

/**
 * @brief   Creates a trigger instance.
 * @details Registers the trigger instance to the next empty slot in the
 *          internal trigger array.
 *
 * @param   pTriggerId : Pointer to a trigger ID variable.
 *
 * @return  Result of trigger creation.
 *
 * @retval  GOS_SUCCESS : Trigger instance created successfully.
 * @retval  GOS_ERROR   : Trigger array is full.
 */
gos_result_t gos_triggerCreate (gos_triggerId_t* pTriggerId);

/**
 * @brief   Increments the trigger value of the given trigger.
 * @details Increments the trigger value and checks whether a waiter
 *          has the same desired value as the current trigger value.
 *          If so, it unblocks the waiter task.
 *
 * @param   triggerId : Trigger ID.
 *
 * @return  Result of trigger increasing.
 *
 * @retval  GOS_SUCCESS : Trigger increased successfully.
 * @retval  GOS_ERROR   : Invalid trigger ID.
 */
gos_result_t gos_triggerIncrement (gos_triggerId_t triggerId);

/**
 * @brief   Waits for the trigger instance to reach the given trigger value.
 * @details Checks whether the trigger already has the given value, and if not,
 *          it sends the caller task to blocked state.
 *
 * @param   triggerId : Trigger ID.
 * @param   value     : The desired trigger value.
 *
 * @return  Result of trigger waiting.
 *
 * @retval  GOS_SUCCESS : Trigger value reached.
 * @retval  GOS_ERROR   : Invalid trigger ID or current task ID could not be retrieved.
 */
gos_result_t gos_triggerWait (gos_triggerId_t triggerId, u32_t value);

#endif
