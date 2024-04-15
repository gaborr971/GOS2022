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
//! @file       drv_24lc256.h
//! @author     Ahmed Gazar
//! @date       2024-04-15
//! @version    1.0
//!
//! @brief      GOS2022 Library / 24LC256 driver header.
//! @details    This component provides the interface for the 24LC256 EEPROM chip.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-15    Ahmed Gazar     Initial version created.
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
#ifndef DRV_24LC256_H
#define DRV_24LC256_H
/*
 * Includes
 */
#include <gos.h>
#include <drv_i2c.h>

/*
 * Type definitions
 */
/**
 * TODO
 */
typedef struct
{
    drv_i2cPeriphInstance_t i2cInstance;
    u16_t                   deviceAddress;
    u32_t                   readMutexTmo;
    u32_t                   readTriggerTmo;
    u32_t                   writeMutexTmo;
    u32_t                   writeTriggerTmo;
}drv_24lc256Descriptor_t;

// TODO
gos_result_t drv_24lc256Init (
		void_t* pDevice
		);

// TODO
bool_t drv_24lc256IsDeviceAvailable (
		void_t* pDevice
		);

// TODO
gos_result_t drv_24lc256Read (
		void_t* pDevice, u16_t address,
		u8_t*   pTarget, u16_t size
		);

// TODO
gos_result_t drv_24lc256Write (
		void_t* pDevice, u16_t address,
		u8_t*   pData,   u16_t size
		);
#endif
