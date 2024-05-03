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
//! @file       drv_i2c.h
//! @author     Ahmed Gazar
//! @date       2024-03-15
//! @version    1.0
//!
//! @brief      GOS2022 Library / I2C driver header.
//! @details    This component provides access to the I2C peripheries.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-03-15    Ahmed Gazar     Initial version created.
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
#ifndef DRV_I2C_H
#define DRV_I2C_H

/*
 * Includes
 */
#include <gos.h>
#include <drv_dma.h>

/*
 * Type definitions
 */
/**
 * I2C periphery instance enum.
 */
typedef enum
{
    DRV_I2C_INSTANCE_1 = 0,                 //!< I2C1
    DRV_I2C_INSTANCE_2,                     //!< I2C2
    DRV_I2C_INSTANCE_3,                     //!< I2C3
    DRV_I2C_NUM_OF_INSTANCES                //!< Number of I2C instances.
}drv_i2cPeriphInstance_t;

/**
 * I2C descriptor type.
 */
typedef struct
{
    drv_i2cPeriphInstance_t periphInstance;  //!< Periphery instance.
    u32_t                   clockSpeed;      //!< Clock speed.
    u32_t                   dutyCycle;       //!< Duty cycle.
    u32_t                   ownAddress1;     //!< Own address.
    u32_t                   addressingMode;  //!< Addressing mode.
    u32_t                   dualAddressMode; //!< Dual address mode.
    u32_t                   ownAddress2;     //!< Own address 2.
    u32_t                   generalCallMode; //!< General call mode.
    u32_t                   noStretchMode;   //!< No stretch mode.
    drv_dmaDescriptor_t*    dmaConfigRx;     //!< RX DMA configuration.
    drv_dmaDescriptor_t*    dmaConfigTx;     //!< TX DMA configuration.
}drv_i2cDescriptor_t;

// TODO
gos_result_t drv_i2cInit (
        void_t
        );

// TODO
gos_result_t drv_i2cInitInstance (
        GOS_CONST drv_i2cDescriptor_t* pInstance
        );

// TODO
gos_result_t drv_i2cMemWrite (
        drv_i2cPeriphInstance_t instance, u16_t address, u16_t memAddress, u16_t memAddressSize,
        u8_t*                   data,     u16_t size,    u32_t mutexTmo,   u32_t triggerTmo
        );

// TODO
gos_result_t drv_i2cMemRead (
        drv_i2cPeriphInstance_t instance, u16_t address, u16_t memAddress, u16_t memAddressSize,
        u8_t*                   data,     u16_t size,    u32_t mutexTmo,   u32_t triggerTmo
        );

// TODO
gos_result_t drv_i2cTransmitBlocking (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t transmitTmo
        );

// TODO
gos_result_t drv_i2cReceiveBlocking (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t receiveTmo
        );

// TODO
gos_result_t drv_i2cTransmitIT (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        );

// TODO
gos_result_t drv_i2cReceiveIT (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        );

// TODO
gos_result_t drv_i2cTransmitDMA (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        );

// TODO
gos_result_t drv_i2cReceiveDMA (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        );
#endif
