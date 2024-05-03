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
//! @file       drv_adc.c
//! @author     Ahmed Gazar
//! @date       2024-04-04
//! @version    1.0
//!
//! @brief      GOS2022 Library / ADC driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_adc.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-04    Ahmed Gazar     Initial version created.
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
#include <drv_adc.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"

/*
 * Static variables
 */
/**
 * ADC instance lookup table.
 */
GOS_STATIC ADC_TypeDef* adcInstanceLut [] =
{
    [DRV_ADC_INSTANCE_1] = ADC1,
    [DRV_ADC_INSTANCE_2] = ADC2,
    [DRV_ADC_INSTANCE_3] = ADC3,
};

/**
 * ADC handles.
 */
GOS_STATIC ADC_HandleTypeDef hadcs              [DRV_ADC_NUM_OF_INSTANCES];

/**
 * ADC mutexes.
 */
GOS_STATIC gos_mutex_t       adcMutexes         [DRV_ADC_NUM_OF_INSTANCES];

/**
 * ADC ready triggers.
 */
GOS_STATIC gos_trigger_t     adcReadyTriggers   [DRV_ADC_NUM_OF_INSTANCES];

/*
 * External variables
 */
/**
 * ADC configuration array (shall be defined by user).
 */
GOS_EXTERN GOS_CONST drv_adcDescriptor_t     adcConfig [];

/**
 * ADC configuration array size (shall be defined by user).
 */
GOS_EXTERN u32_t                             adcConfigSize;

/*
 * Function: drv_adcInit
 */
gos_result_t drv_adcInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t adcDriverInitResult = GOS_SUCCESS;
    u8_t         adcIdx              = 0u;

    /*
     * Function code.
     */
    if (adcConfig != NULL)
    {
        for (adcIdx = 0u; adcIdx < adcConfigSize / sizeof(drv_adcDescriptor_t); adcIdx++)
        {
            GOS_CONCAT_RESULT(adcDriverInitResult, drv_adcInitInstance(adcIdx));
        }
    }
    else
    {
        // Configuration array is NULL pointer.
        adcDriverInitResult = GOS_ERROR;
    }

    return adcDriverInitResult;
}

/*
 * Function: drv_adcInitInstance
 */
gos_result_t drv_adcInitInstance (u8_t adcInstanceIndex)
{
    /*
     * Local variables.
     */
    gos_result_t            adcInitResult = GOS_ERROR;
    drv_adcPeriphInstance_t instance      = 0u;
    ADC_ChannelConfTypeDef  sConfig       = {0};

    /*
     * Function code.
     */
    if (adcConfig != NULL && adcInstanceIndex < (adcConfigSize / sizeof(drv_adcDescriptor_t)))
    {
        instance = adcConfig[adcInstanceIndex].periphInstance;

        hadcs[instance].Instance = adcInstanceLut[instance];
        hadcs[instance].Init.ClockPrescaler = adcConfig[adcInstanceIndex].clockPrescaler;
        hadcs[instance].Init.ContinuousConvMode = adcConfig[adcInstanceIndex].continuousConvMode;
        hadcs[instance].Init.DMAContinuousRequests = adcConfig[adcInstanceIndex].dmaContinuousRequests;
        hadcs[instance].Init.DataAlign = adcConfig[adcInstanceIndex].dataAlign;
        hadcs[instance].Init.DiscontinuousConvMode = adcConfig[adcInstanceIndex].discontinuousConvMode;
        hadcs[instance].Init.EOCSelection = adcConfig[adcInstanceIndex].eocSelection;
        hadcs[instance].Init.ExternalTrigConv = adcConfig[adcInstanceIndex].externalTrigConv;
        hadcs[instance].Init.ExternalTrigConvEdge = adcConfig[adcInstanceIndex].externalTrigConvEdge;
        hadcs[instance].Init.NbrOfConversion = adcConfig[adcInstanceIndex].nbrOfConversion;
        hadcs[instance].Init.NbrOfDiscConversion = adcConfig[adcInstanceIndex].nbrOfDiscConversion;
        hadcs[instance].Init.Resolution = adcConfig[adcInstanceIndex].resolution;
        hadcs[instance].Init.ScanConvMode = adcConfig[adcInstanceIndex].scanConvMode;

        /*
         *  Configure for the selected ADC regular channel its corresponding rank
         *  in the sequencer and its sample time.
         */
        sConfig.Channel      = adcConfig[adcInstanceIndex].channel;
        sConfig.Rank         = adcConfig[adcInstanceIndex].rank;
        sConfig.SamplingTime = adcConfig[adcInstanceIndex].samplingTime;

        if (HAL_ADC_Init         (&hadcs[instance])                 == HAL_OK      &&
            HAL_ADC_ConfigChannel(&hadcs[instance], &sConfig)       == HAL_OK      &&
            gos_mutexInit        (&adcMutexes[instance])            == GOS_SUCCESS &&
            gos_triggerInit      (&adcReadyTriggers[instance])    == GOS_SUCCESS   &&
            gos_triggerReset     (&adcReadyTriggers[instance])    == GOS_SUCCESS
            )
        {
            adcInitResult = GOS_SUCCESS;
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

    return adcInitResult;
}

/*
 * Function: drv_adcGetValueBlocking
 */
gos_result_t drv_adcGetValueBlocking (
        drv_adcPeriphInstance_t instance, u16_t* pValue,
        u32_t                   mutexTmo, u32_t  readTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t adcDriverGetValueResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pValue                                               != NULL &&
        gos_mutexLock(&adcMutexes[instance], mutexTmo)       == GOS_SUCCESS &&
        HAL_ADC_Start(&hadcs[instance])                      == HAL_OK &&
        HAL_ADC_PollForConversion(&hadcs[instance], readTmo) == HAL_OK)
    {
        *pValue = HAL_ADC_GetValue(&hadcs[instance]);

        if (HAL_ADC_Stop(&hadcs[instance]) == HAL_OK)
        {
            adcDriverGetValueResult = GOS_SUCCESS;
        }
        else
        {
            // ADC stop error.
        }
    }
    else
    {
        // Mutex or ADC error.
    }

    (void_t) gos_mutexUnlock(&adcMutexes[instance]);

    return adcDriverGetValueResult;
}

/*
 * Function: drv_adcGetValueIT
 */
gos_result_t drv_adcGetValueIT (
        drv_adcPeriphInstance_t instance, u16_t* pValue,
        u32_t                   mutexTmo, u32_t  triggerTmo
        )
{
    /*
     * Local variables.
     */
    gos_result_t adcDriverGetValueResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pValue != NULL && gos_mutexLock(&adcMutexes[instance], mutexTmo) == GOS_SUCCESS)
    {
        if (HAL_ADC_Start_IT(&hadcs[instance]) == HAL_OK)
        {
            if (gos_triggerWait (&adcReadyTriggers[instance], 1, triggerTmo) == GOS_SUCCESS &&
                gos_triggerReset(&adcReadyTriggers[instance])                == GOS_SUCCESS)
            {
                *pValue = HAL_ADC_GetValue(&hadcs[instance]);

                if (HAL_ADC_Stop_IT(&hadcs[instance]) == HAL_OK)
                {
                    adcDriverGetValueResult = GOS_SUCCESS;
                }
                else
                {
                    // ADC stop error.
                }
            }
            else
            {
                // Trigger error.
            }
        }
        else
        {
            // ADC start error.
        }
    }
    else
    {
        // Mutex error.
    }

    (void_t) gos_mutexUnlock(&adcMutexes[instance]);

    return adcDriverGetValueResult;
}

/*
 * Function: HAL_ADC_ConvCpltCallback
 */
void_t HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc)
{
    /*
     * Function code.
     */
    if (hadc->Instance == adcInstanceLut[DRV_ADC_INSTANCE_1])
    {
        (void_t) gos_triggerIncrement(&adcReadyTriggers[DRV_ADC_INSTANCE_1]);
    }
    else if (hadc->Instance == adcInstanceLut[DRV_ADC_INSTANCE_2])
    {
        (void_t) gos_triggerIncrement(&adcReadyTriggers[DRV_ADC_INSTANCE_2]);
    }
    else if (hadc->Instance == adcInstanceLut[DRV_ADC_INSTANCE_3])
    {
        (void_t) gos_triggerIncrement(&adcReadyTriggers[DRV_ADC_INSTANCE_3]);
    }
    else
    {
        // Wrong instance.
    }
}

/*
 * Function: ADC_IRQHandler
 */
void_t ADC_IRQHandler (void_t)
{
    /*
     * Local variables.
     */
    drv_adcPeriphInstance_t instance = DRV_ADC_INSTANCE_1;

    /*
     * Function code.
     */
    GOS_ISR_ENTER

    for (instance = DRV_ADC_INSTANCE_1; instance < DRV_ADC_NUM_OF_INSTANCES; instance++)
    {
        HAL_ADC_IRQHandler(&hadcs[instance]);
    }

    GOS_ISR_EXIT
}

/*
 * Function: HAL_ADC_MspInit
 */
void_t HAL_ADC_MspInit (ADC_HandleTypeDef* hadc)
{
    /*
     * Local variables.
     */
    drv_adcPeriphInstance_t instance = 0u;
    u8_t                    idx      = 0u;

    /*
     * Function code.
     */
    for (instance = 0u; instance < DRV_ADC_NUM_OF_INSTANCES; instance++)
    {
        if (adcInstanceLut[instance] == hadc->Instance)
        {
            for (idx = 0u; idx < adcConfigSize / sizeof(drv_adcDescriptor_t); idx++)
            {
                if (adcConfig[idx].periphInstance == instance)
                {
                    if (adcConfig[idx].dmaConfig != NULL)
                    {
                        __HAL_LINKDMA(hadc, DMA_Handle, adcConfig[idx].dmaConfig->hdma);
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
