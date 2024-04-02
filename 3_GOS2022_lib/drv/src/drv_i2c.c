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
//! @file       drv_i2c.c
//! @author     Ahmed Gazar
//! @date       2024-04-02
//! @version    1.1
//!
//! @brief      GOS2022 Library / I2C driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_i2c.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-03-15    Ahmed Gazar     Initial version created.
// 1.1        2024-04-02    Ahmed Gazar     *    Complete callbacks made faster by removing loops
//                                          *    Inline macros removed from functions
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
#include <drv_i2c.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

/*
 * Static variables
 */
/**
 * I2C instance lookup table.
 */
GOS_STATIC I2C_TypeDef* i2cInstanceLut [] =
{
    [DRV_I2C_INSTANCE_1] = I2C1,
    [DRV_I2C_INSTANCE_2] = I2C2,
    [DRV_I2C_INSTANCE_3] = I2C3,
};

/**
 * I2C handles.
 */
GOS_STATIC I2C_HandleTypeDef hi2cs                [DRV_I2C_NUM_OF_INSTANCES];

/**
 * I2C mutexes.
 */
GOS_STATIC gos_mutex_t       i2cMutexes           [DRV_I2C_NUM_OF_INSTANCES];

/**
 * I2C RX ready triggers.
 */
GOS_STATIC gos_trigger_t     i2cRxReadyTriggers   [DRV_I2C_NUM_OF_INSTANCES];

/**
 * I2C TX ready triggers.
 */
GOS_STATIC gos_trigger_t     i2cTxReadyTriggers   [DRV_I2C_NUM_OF_INSTANCES];

/**
 * I2C RX mem ready triggers.
 */
GOS_STATIC gos_trigger_t     i2cRxMemReadyTriggers [DRV_I2C_NUM_OF_INSTANCES];

/**
 * I2C TX mem ready triggers.
 */
GOS_STATIC gos_trigger_t     i2cTxMemReadyTriggers [DRV_I2C_NUM_OF_INSTANCES];

/*
 * External variables
 */
/**
 * I2C configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_i2cDescriptor_t     i2cConfig [];

/**
 * I2C configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                             i2cConfigSize;

/*
 * Function: drv_i2cInit
 */
gos_result_t drv_i2cInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverInitResult = GOS_SUCCESS;
    u8_t         i2cIdx              = 0u;

    /*
     * Function code.
     */
    if (i2cConfig != NULL)
    {
        for (i2cIdx = 0u; i2cIdx < i2cConfigSize / sizeof(drv_i2cDescriptor_t); i2cIdx++)
        {
            GOS_CONCAT_RESULT(i2cDriverInitResult, drv_i2cInitInstance(i2cIdx));
        }
    }
    else
    {
        // Configuration array is NULL pointer.
        i2cDriverInitResult = GOS_ERROR;
    }

    return i2cDriverInitResult;
}

/*
 * Function: drv_i2cInitInstance
 */
gos_result_t drv_i2cInitInstance (u8_t i2cInstanceIndex)
{
    /*
     * Local variables.
     */
    gos_result_t            i2cInitResult = GOS_ERROR;
    drv_i2cPeriphInstance_t instance      = 0u;

    /*
     * Function code.
     */
    if (i2cConfig != NULL && i2cInstanceIndex < (i2cConfigSize / sizeof(drv_i2cDescriptor_t)))
    {
        instance = i2cConfig[i2cInstanceIndex].periphInstance;

        hi2cs[instance].Instance             = i2cInstanceLut[instance];
        hi2cs[instance].Init.ClockSpeed      = i2cConfig[i2cInstanceIndex].clockSpeed;
        hi2cs[instance].Init.DutyCycle       = i2cConfig[i2cInstanceIndex].dutyCycle;
        hi2cs[instance].Init.AddressingMode  = i2cConfig[i2cInstanceIndex].addressingMode;
        hi2cs[instance].Init.OwnAddress1     = i2cConfig[i2cInstanceIndex].ownAddress1;
        hi2cs[instance].Init.OwnAddress2     = i2cConfig[i2cInstanceIndex].ownAddress2;
        hi2cs[instance].Init.DualAddressMode = i2cConfig[i2cInstanceIndex].dualAddressMode;
        hi2cs[instance].Init.GeneralCallMode = i2cConfig[i2cInstanceIndex].generalCallMode;
        hi2cs[instance].Init.NoStretchMode   = i2cConfig[i2cInstanceIndex].noStretchMode;

        if (HAL_I2C_Init    (&hi2cs[instance])                 == HAL_OK      &&
            gos_mutexInit   (&i2cMutexes[instance])            == GOS_SUCCESS &&
            gos_triggerInit (&i2cRxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerInit (&i2cTxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerInit (&i2cRxMemReadyTriggers[instance]) == GOS_SUCCESS &&
            gos_triggerInit (&i2cTxMemReadyTriggers[instance]) == GOS_SUCCESS &&
            gos_triggerReset(&i2cRxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerReset(&i2cTxReadyTriggers[instance])    == GOS_SUCCESS &&
            gos_triggerReset(&i2cRxMemReadyTriggers[instance]) == GOS_SUCCESS &&
            gos_triggerReset(&i2cTxMemReadyTriggers[instance]) == GOS_SUCCESS
            )
        {
            i2cInitResult = GOS_SUCCESS;
        }
        else
        {
            // Init error.
        }
    }
    else
    {
        // Configuration missing or index is out of array boundary.
    }

    return i2cInitResult;
}

/*
 * Function: drv_i2cMemWrite
 */
gos_result_t drv_i2cMemWrite (
        drv_i2cPeriphInstance_t instance, u16_t address, u16_t memAddress, u16_t memAddressSize,
        u8_t*                   data,     u16_t size,    u32_t mutexTmo,   u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cMemWriteResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Mem_Write_IT(&hi2cs[instance], address, memAddress, memAddressSize, data, size) == HAL_OK      &&
            gos_triggerWait     (&i2cTxMemReadyTriggers[instance], 1, triggerTmo)                   == GOS_SUCCESS &&
            gos_triggerReset    (&i2cTxMemReadyTriggers[instance])                                  == GOS_SUCCESS)
        {
            i2cMemWriteResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cMemWriteResult;
}

/*
 * Function: drv_i2cMemRead
 */
gos_result_t drv_i2cMemRead (
        drv_i2cPeriphInstance_t instance, u16_t address, u16_t memAddress, u16_t memAddressSize,
        u8_t*                   data,     u16_t size,    u32_t mutexTmo,   u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cMemReadResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Mem_Read_IT(&hi2cs[instance], address, memAddress, memAddressSize, data, size) == HAL_OK      &&
            gos_triggerWait    (&i2cRxMemReadyTriggers[instance], 1, triggerTmo)                   == GOS_SUCCESS &&
            gos_triggerReset   (&i2cRxMemReadyTriggers[instance])                                  == GOS_SUCCESS)
        {
            i2cMemReadResult = GOS_SUCCESS;
        }
        else
        {
            // Receive or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cMemReadResult;
}

/*
 * Function: drv_i2cTransmitBlocking
 */
gos_result_t drv_i2cTransmitBlocking (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t transmitTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverTransmitResult = GOS_ERROR;


    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Master_Transmit(&hi2cs[instance], address, pData, size, transmitTmo) == HAL_OK)
        {
            i2cDriverTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cDriverTransmitResult;
}

/*
 * Function: drv_i2cReceiveBlocking
 */
gos_result_t drv_i2cReceiveBlocking (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t receiveTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverReceiveResult = GOS_ERROR;


    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Master_Receive(&hi2cs[instance], address, pBuffer, size, receiveTmo) == HAL_OK)
        {
            i2cDriverReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cDriverReceiveResult;
}

/*
 * Function: drv_i2cTransmitIT
 */
gos_result_t drv_i2cTransmitIT (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverTransmitResult = GOS_ERROR;


    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Master_Transmit_IT(&hi2cs[instance], address, pData, size)       == HAL_OK      &&
            gos_triggerWait           (&i2cTxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset          (&i2cTxReadyTriggers[instance])                == GOS_SUCCESS)
        {
            i2cDriverTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cDriverTransmitResult;
}

/*
 * Function: drv_i2cReceiveIT
 */
gos_result_t drv_i2cReceiveIT (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Master_Receive_IT(&hi2cs[instance], address, pBuffer, size)     == HAL_OK      &&
            gos_triggerWait          (&i2cRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset         (&i2cRxReadyTriggers[instance])                == GOS_SUCCESS)
        {
            i2cDriverReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cDriverReceiveResult;
}

/*
 * Function: drv_i2cTransmitDMA
 */
gos_result_t drv_i2cTransmitDMA (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pData,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverTransmitResult = GOS_ERROR;


    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Master_Transmit_DMA(&hi2cs[instance], address, pData, size)       == HAL_OK      &&
            gos_triggerWait            (&i2cTxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset           (&i2cTxReadyTriggers[instance])                == GOS_SUCCESS)
        {
            i2cDriverTransmitResult = GOS_SUCCESS;
        }
        else
        {
            // Transmit or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cDriverTransmitResult;
}

/*
 * Function: drv_i2cReceiveDMA
 */
gos_result_t drv_i2cReceiveDMA (
        drv_i2cPeriphInstance_t instance, u16_t address,  u8_t* pBuffer,
        u16_t                   size,     u32_t mutexTmo, u32_t triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t i2cDriverReceiveResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_mutexLock(&i2cMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_I2C_Master_Receive_DMA(&hi2cs[instance], address, pBuffer, size)     == HAL_OK      &&
            gos_triggerWait           (&i2cRxReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
            gos_triggerReset          (&i2cRxReadyTriggers[instance])                == GOS_SUCCESS)
        {
            i2cDriverReceiveResult = GOS_SUCCESS;
        }
        else
        {
            // Receive or trigger error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&i2cMutexes[instance]);

    return i2cDriverReceiveResult;
}

/*
 * Function: HAL_I2C_MasterTxCpltCallback
 */
void_t HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef* hi2c)
{
    /*
     * Function code.
     */
    if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&i2cTxReadyTriggers[DRV_I2C_INSTANCE_1]);
    }
    else if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&i2cTxReadyTriggers[DRV_I2C_INSTANCE_2]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: HAL_I2C_MemTxCpltCallback
 */
void_t HAL_I2C_MemTxCpltCallback (I2C_HandleTypeDef* hi2c)
{
    /*
     * Function code.
     */
    if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&i2cTxMemReadyTriggers[DRV_I2C_INSTANCE_1]);
    }
    else if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&i2cTxMemReadyTriggers[DRV_I2C_INSTANCE_2]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: HAL_I2C_MasterRxCpltCallback
 */
void_t HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef* hi2c)
{
    /*
     * Function code.
     */
    if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&i2cRxReadyTriggers[DRV_I2C_INSTANCE_1]);
    }
    else if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&i2cRxReadyTriggers[DRV_I2C_INSTANCE_2]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: HAL_I2C_MemRxCpltCallback
 */
void_t HAL_I2C_MemRxCpltCallback (I2C_HandleTypeDef* hi2c)
{
    /*
     * Function code.
     */
    if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&i2cRxMemReadyTriggers[DRV_I2C_INSTANCE_1]);
    }
    else if (hi2c->Instance == i2cInstanceLut[DRV_I2C_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&i2cRxMemReadyTriggers[DRV_I2C_INSTANCE_2]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: I2C1_EV_IRQHandler
 */
void_t I2C1_EV_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_I2C_EV_IRQHandler(&hi2cs[DRV_I2C_INSTANCE_1]);

    GOS_ISR_EXIT
}

/*
 * Function: I2C2_EV_IRQHandler
 */
void_t I2C2_EV_IRQHandler (void_t)
{
    /*
     * Function code.
     */
    GOS_ISR_ENTER

    HAL_I2C_EV_IRQHandler(&hi2cs[DRV_I2C_INSTANCE_2]);

    GOS_ISR_EXIT
}

/*
 * Function: HAL_I2C_MspInit
 */
void_t HAL_I2C_MspInit (I2C_HandleTypeDef* hi2c)
{
    /*
     * Local variables.
     */
    drv_i2cPeriphInstance_t instance = 0u;
    u8_t                    idx      = 0u;

    /*
     * Function code.
     */
    for (instance = 0u; instance < DRV_I2C_NUM_OF_INSTANCES; instance++)
    {
        if (i2cInstanceLut[instance] == hi2c->Instance)
        {
            for (idx = 0u; idx < i2cConfigSize / sizeof(drv_i2cDescriptor_t); idx++)
            {
                if (i2cConfig[idx].periphInstance == instance)
                {
                    if (i2cConfig[idx].dmaConfigRx != NULL)
                    {
                        __HAL_LINKDMA(hi2c, hdmarx, i2cConfig[idx].dmaConfigRx->hdma);
                    }
                    else
                    {
                        // There is no DMA assigned.
                    }

                    if (i2cConfig[idx].dmaConfigTx != NULL)
                    {
                        __HAL_LINKDMA(hi2c, hdmatx, i2cConfig[idx].dmaConfigTx->hdma);
                    }
                    else
                    {
                        // There is no DMA assigned.
                    }
                }
                else
                {
                    // Continue.
                }
            }
        }
        else
        {
            // Continue.
        }
    }
}
