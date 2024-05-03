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
//! @file       drv_mcp23017.c
//! @author     Ahmed Gazar
//! @date       2024-04-12
//! @version    1.0
//!
//! @brief      GOS2022 Library / MCP23017 driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_mcp23017.h
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
/*
 * Includes
 */
#include <drv_mcp23017.h>

/*
 * Macros
 */
/**
 * Pull up resistor.
 * Default state: MCP23017_GPPU_ALL_DISABLED
 */
#define MCP23017_GPPU_ALL_DISABLED  ( 0x00 )
#define MCP23017_GPPU_ALL_ENABLED   ( 0xFF )
#define MCP23017_GPPU_IO0_ENABLED   ( 0x01 )
#define MCP23017_GPPU_IO1_ENABLED   ( 0x02 )
#define MCP23017_GPPU_IO2_ENABLED   ( 0x04 )
#define MCP23017_GPPU_IO3_ENABLED   ( 0x08 )
#define MCP23017_GPPU_IO4_ENABLED   ( 0x10 )
#define MCP23017_GPPU_IO5_ENABLED   ( 0x20 )
#define MCP23017_GPPU_IO6_ENABLED   ( 0x40 )
#define MCP23017_GPPU_IO7_ENABLED   ( 0x80 )

/**
 * Registers-
 */
#define REGISTER_IODIRA             ( 0x00 )
#define REGISTER_IODIRB             ( 0x01 )
#define REGISTER_IPOLA              ( 0x02 )
#define REGISTER_IPOLB              ( 0x03 )
#define REGISTER_GPINTENA           ( 0x04 )
#define REGISTER_GPINTENB           ( 0x05 )
#define REGISTER_DEFVALA            ( 0x06 )
#define REGISTER_DEFVALB            ( 0x07 )
#define REGISTER_INTCONA            ( 0x08 )
#define REGISTER_INTCONB            ( 0x09 )

/**
 * IOCON 0x0A
 * IOCON 0x0B
 */
#define REGISTER_GPPUA              ( 0x0C )
#define REGISTER_GPPUB              ( 0x0D )
#define REGISTER_INTFA              ( 0x0E )
#define REGISTER_INTFB              ( 0x0F )
#define REGISTER_INTCAPA            ( 0x10 )
#define REGISTER_INTCAPB            ( 0x11 )
#define REGISTER_GPIOA              ( 0x12 )
#define REGISTER_GPIOB              ( 0x13 )
#define REGISTER_OLATA              ( 0x14 )
#define REGISTER_OLATB              ( 0x15 )

/*
 * Function prototypes
 */
GOS_STATIC gos_result_t drv_mcp23017Read     (drv_mcp23017Descriptor_t* pDevice, u16_t reg, u8_t* pData);
GOS_STATIC gos_result_t drv_mcp23017Write    (drv_mcp23017Descriptor_t* pDevice, u16_t reg, u8_t* pData);
GOS_STATIC gos_result_t drv_mcp23017SetIoDir (drv_mcp23017Descriptor_t* pDevice);
GOS_STATIC gos_result_t drv_mcp23017SetIoPol (drv_mcp23017Descriptor_t* pDevice);

/*
 * Function: drv_mcp23017Init
 */
gos_result_t drv_mcp23017Init (void_t* pDevice)
{
    /*
     * Local variables.
     */
    gos_result_t mcp23017InitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (drv_mcp23017SetIoDir((drv_mcp23017Descriptor_t*)pDevice) == GOS_SUCCESS &&
        drv_mcp23017SetIoPol((drv_mcp23017Descriptor_t*)pDevice) == GOS_SUCCESS)
    {
        mcp23017InitResult = GOS_SUCCESS;
    }
    else
    {
        // Init error.
    }

    return mcp23017InitResult;
}

/*
 * Function: drv_mcp23017IsDeviceAvailable
 */
bool_t drv_mcp23017IsDeviceAvailable (void_t* pDevice)
{
    /*
     * Local variables.
     */
    drv_gpioState_t dummyState        = GPIO_STATE_LOW;
    bool_t          isDeviceAvailable = GOS_FALSE;

    /*
     * Function code.
     */
    if (drv_mcp23017ReadPin((drv_mcp23017Descriptor_t*)pDevice, MCP23017_PORTA, 0u, &dummyState) == GOS_SUCCESS)
    {
        isDeviceAvailable = GOS_TRUE;
    }
    else
    {
        // Device is not available.
    }

    return isDeviceAvailable;
}

/*
 * Function: drv_mcp23017ReadPin
 */
gos_result_t drv_mcp23017ReadPin (
        drv_mcp23017Descriptor_t* pDevice, u8_t             port,
        drv_mcp23017Pin_t         pin,     drv_gpioState_t* pGpioState
        )
{
    /*
     * Local variables.
     */
    gos_result_t mcp23017ReadPinResult = GOS_ERROR;
    u8_t         data                  = 0u;

    /*
     * Function code.
     */
    if (pDevice != NULL && pGpioState != NULL &&
        drv_mcp23017Read(pDevice, REGISTER_GPIOA | port, &data) == GOS_SUCCESS)
    {
        *pGpioState = (data & pin) > 0 ? GPIO_STATE_HIGH : GPIO_STATE_LOW;
        mcp23017ReadPinResult = GOS_SUCCESS;
    }
    else
    {
        // Read error.
    }

    return mcp23017ReadPinResult;
}

/*
 * Function: drv_mcp23017WritePin
 */
gos_result_t drv_mcp23017WritePin (
        drv_mcp23017Descriptor_t* pDevice, u8_t            port,
        drv_mcp23017Pin_t         pin,     drv_gpioState_t gpioState
        )
{
    /*
     * Local variables.
     */
    gos_result_t    mcp23017WritePinResult = GOS_ERROR;
    u8_t            portState              = 0u;
    u8_t            data                   = 0u;

    /*
     * Function code.
     */
    if (pDevice != NULL &&
        drv_mcp23017Read(pDevice, REGISTER_GPIOA | port, &data) == GOS_SUCCESS)
    {
        portState = data;

        // Clear pin state.
        if (gpioState == GPIO_STATE_LOW)
        {
            portState &= ~(pin & 0b11111111);
        }
        else
        {
            portState |= (pin & 0b11111111);
        }

        data = portState;

        mcp23017WritePinResult = drv_mcp23017Write(pDevice, REGISTER_GPIOA | port, &data);
    }
    else
    {
        // Null pointer or read error.
    }

    return mcp23017WritePinResult;
}

/**
 * TODO
 * @param pDevice
 * @param reg
 * @param pData
 * @return
 */
GOS_STATIC gos_result_t drv_mcp23017Read (drv_mcp23017Descriptor_t* pDevice, u16_t reg, u8_t* pData)
{
    return drv_i2cMemRead(pDevice->i2cInstance, (pDevice->deviceAddress) << 1,
            reg, 1, pData, 1, pDevice->readMutexTmo, pDevice->readTriggerTmo);
}

/**
 * TODO
 * @param reg
 * @param pData
 * @return
 */
GOS_STATIC gos_result_t drv_mcp23017Write (drv_mcp23017Descriptor_t* pDevice, u16_t reg, u8_t* pData)
{
    return drv_i2cMemWrite(pDevice->i2cInstance, (pDevice->deviceAddress) << 1,
            reg, 1, pData, 1, pDevice->writeMutexTmo, pDevice->writeTriggerTmo);
}

/**
 * TODO
 * @param pDevice
 * @return
 */
GOS_STATIC gos_result_t drv_mcp23017SetIoDir (drv_mcp23017Descriptor_t* pDevice)
{
    /*
     * Local variables.
     */
    gos_result_t setIoDirResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (drv_mcp23017Write(pDevice, REGISTER_IODIRA, &(pDevice->portADir)) == GOS_SUCCESS &&
        drv_mcp23017Write(pDevice, REGISTER_IODIRB, &(pDevice->portBDir)) == GOS_SUCCESS)
    {
        setIoDirResult = GOS_SUCCESS;
    }
    else
    {
        // Write error.
    }

    return setIoDirResult;
}

/**
 * TODO
 * @param pDevice
 * @return
 */
GOS_STATIC gos_result_t drv_mcp23017SetIoPol (drv_mcp23017Descriptor_t* pDevice)
{
    /*
     * Local variables.
     */
    gos_result_t setIoPolResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (drv_mcp23017Write(pDevice, REGISTER_IPOLA, &(pDevice->portAPol)) == GOS_SUCCESS &&
        drv_mcp23017Write(pDevice, REGISTER_IPOLB, &(pDevice->portBPol)) == GOS_SUCCESS)
    {
        setIoPolResult = GOS_SUCCESS;
    }
    else
    {
        // Write error.
    }

    return setIoPolResult;
}
