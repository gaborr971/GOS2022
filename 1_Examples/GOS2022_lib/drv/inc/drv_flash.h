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
//! @file       drv_flash.h
//! @author     Ahmed Gazar
//! @date       2023-09-26
//! @version    1.0
//!
//! @brief      GOS2022 Library / FLASH driver header.
//! @details    This component provides access to the internal FLASH memory.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-09-26    Ahmed Gazar     Initial version created.
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
#ifndef DRV_FLASH_H
#define DRV_FLASH_H

/*
 * Includes.
 */
#include <gos.h>

/*
 * Function prototypes
 */
// TODO
gos_result_t drv_flashErase (u32_t address, u32_t requiredSize);

// TODO
gos_result_t drv_flashWrite (u32_t address, void_t* pData, u32_t size);

// TODO
gos_result_t drv_flashWriteWithoutLock (u32_t address, void_t* pData, u32_t size);

// TODO
gos_result_t drv_flashUnlock (void_t);

// TODO
gos_result_t drv_flashLock (void_t);

// TODO
gos_result_t drv_flashRead (u32_t address, void_t* pData, u32_t size);

#endif
