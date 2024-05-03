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
//! @file       drv_adc.h
//! @author     Ahmed Gazar
//! @date       2024-04-04
//! @version    1.0
//!
//! @brief      GOS2022 Library / ADC driver header.
//! @details    This component provides access to the ADC peripheries.
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
#ifndef DRV_ADC_H
#define DRV_ADC_H

/*
 * Includes
 */
#include <gos.h>
#include <drv_dma.h>

/*
 * Type definitions
 */
/**
 * ADC periphery instance enum.
 */
typedef enum
{
    DRV_ADC_INSTANCE_1 = 0,                        //!< ADC1
    DRV_ADC_INSTANCE_2,                            //!< ADC2
    DRV_ADC_INSTANCE_3,                            //!< ADC3
    DRV_ADC_NUM_OF_INSTANCES                       //!< Number of ADC instances.
}drv_adcPeriphInstance_t;

/**
 * ADC descriptor type.
 */
typedef struct
{
    drv_adcPeriphInstance_t periphInstance;        //!< Periphery instance.
    u32_t                   clockPrescaler;        //!< Clock pre-scaler.
    u32_t                   resolution;            //!< Resolution.
    u32_t                   scanConvMode;          //!< Scan conversion mode.
    u32_t                   continuousConvMode;    //!< Continuous conversion mode.
    u32_t                   discontinuousConvMode; //!< Discontinuous conversion mode.
    u32_t                   externalTrigConvEdge;  //!< External trigger conversion edge.
    u32_t                   externalTrigConv;      //!< External trigger conversion.
    u32_t                   dataAlign;             //!< Data align.
    u32_t                   nbrOfConversion;       //!< Number of conversion.
    u32_t                   nbrOfDiscConversion;   //!< Number of discontinuous conversion.
    u32_t                   dmaContinuousRequests; //!< DMA continuous requests.
    u32_t                   eocSelection;          //!< EOC selection.
    u32_t                   channel;               //!< Channel.
    u32_t                   rank;                  //!< Rank.
    u32_t                   samplingTime;          //!< Sampling time.
    drv_dmaDescriptor_t*    dmaConfig;             //!< DMA configuration.
}drv_adcDescriptor_t;

// TODO
gos_result_t drv_adcInit (
        void_t
        );

// TODO
gos_result_t drv_adcInitInstance (
        u8_t adcInstanceIndex
        );

// TODO
gos_result_t drv_adcGetValueBlocking (
        drv_adcPeriphInstance_t instance, u16_t* pValue,
        u32_t                   mutexTmo, u32_t  readTmo
        );

// TODO
gos_result_t drv_adcGetValueIT (
        drv_adcPeriphInstance_t instance, u16_t* pValue,
        u32_t                   mutexTmo, u32_t  triggerTmo
        );
#endif
