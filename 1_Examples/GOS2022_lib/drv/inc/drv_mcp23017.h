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
//! @file       drv_mcp23017.h
//! @author     Ahmed Gazar
//! @date       2024-04-12
//! @version    1.0
//!
//! @brief      GOS2022 Library / MCP23017 driver header.
//! @details    This component provides the interface for the MCP23017 I/O extender chip.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-12    Ahmed Gazar     Initial version created.
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
#ifndef DRV_MCP23017_H
#define DRV_MCP23017_H
/*
 * Includes
 */
#include <gos.h>
#include <drv_gpio.h>
#include <drv_i2c.h>

/*
 * Macros
 */
#define MCP23017_PORTA                ( 0x00 ) //!< Port A.
#define MCP23017_PORTB                ( 0x01 ) //!< Port B.

#define MCP23017_IODIR_ALL_OUTPUT    ( 0x00 ) //!<
#define MCP23017_IODIR_ALL_INPUT    ( 0xFF ) //!<
#define MCP23017_IODIR_IO0_INPUT    ( 0x01 ) //!<
#define MCP23017_IODIR_IO1_INPUT    ( 0x02 ) //!<
#define MCP23017_IODIR_IO2_INPUT    ( 0x04 ) //!<
#define MCP23017_IODIR_IO3_INPUT    ( 0x08 ) //!<
#define MCP23017_IODIR_IO4_INPUT    ( 0x10 ) //!<
#define MCP23017_IODIR_IO5_INPUT    ( 0x20 ) //!<
#define MCP23017_IODIR_IO6_INPUT    ( 0x40 ) //!<
#define MCP23017_IODIR_IO7_INPUT    ( 0x80 ) //!<

#define MCP23017_IPOL_ALL_NORMAL    ( 0x00 ) //!<
#define MCP23017_IPOL_ALL_INVERTED    ( 0xFF ) //!<
#define MCP23017_IPOL_IO0_INVERTED    ( 0x01 ) //!<
#define MCP23017_IPOL_IO1_INVERTED    ( 0x02 ) //!<
#define MCP23017_IPOL_IO2_INVERTED    ( 0x04 ) //!<
#define MCP23017_IPOL_IO3_INVERTED    ( 0x08 ) //!<
#define MCP23017_IPOL_IO4_INVERTED    ( 0x10 ) //!<
#define MCP23017_IPOL_IO5_INVERTED    ( 0x20 ) //!<
#define MCP23017_IPOL_IO6_INVERTED    ( 0x40 ) //!<
#define MCP23017_IPOL_IO7_INVERTED    ( 0x80 ) //!<

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
    u8_t                    portADir;
    u8_t                    portBDir;
    u8_t                    portAPol;
    u8_t                    portBPol;
}drv_mcp23017Descriptor_t;

/**
 * TODO
 */
typedef enum
{
    DRV_MCP23017_PIN_0       = 0b00000001,
    DRV_MCP23017_PIN_1       = 0b00000010,
    DRV_MCP23017_PIN_2       = 0b00000100,
    DRV_MCP23017_PIN_3       = 0b00001000,
    DRV_MCP23017_PIN_4       = 0b00010000,
    DRV_MCP23017_PIN_5       = 0b00100000,
    DRV_MCP23017_PIN_6       = 0b01000000,
    DRV_MCP23017_PIN_7       = 0b10000000,
    DRV_MCP23017_NUM_OF_PINS = 8
}drv_mcp23017Pin_t;

// TODO
gos_result_t drv_mcp23017Init (
        void_t* pDevice
        );

// TODO
bool_t drv_mcp23017IsDeviceAvailable (
        void_t* pDevice
        );

// TODO
gos_result_t drv_mcp23017ReadPin (
        drv_mcp23017Descriptor_t* pDevice, u8_t             port,
        drv_mcp23017Pin_t         pin,     drv_gpioState_t* pGpioState
        );

// TODO
gos_result_t drv_mcp23017WritePin (
        drv_mcp23017Descriptor_t* pDevice, u8_t            port,
        drv_mcp23017Pin_t         pin,     drv_gpioState_t gpioState
        );

#endif
