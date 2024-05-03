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
//! @file       svl_dhs.h
//! @author     Ahmed Gazar
//! @date       2024-04-13
//! @version    1.0
//!
//! @brief      GOS2022 Library / Device Handler Service header.
//! @details    This component TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-13    Ahmed Gazar     Initial version created.
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
#ifndef SVL_DHS_H
#define SVL_DHS_H
/*
 * Includes
 */
#include <gos.h>

/*
 * Macros
 */
/**
 * TODO
 */
#define SVL_DHS_MAX_DEVICES ( 8u )

/*
 * Type definitions
 */
/**
 * TODO
 */
typedef bool_t       (*svl_dhsAvailable_t)(void_t*);

/**
 * TODO
 */
typedef gos_result_t (*svl_dhsInitFunc_t )(void_t*);

/**
 * TODO
 */
typedef struct
{
	svl_dhsInitFunc_t  initFunction;
    svl_dhsAvailable_t availableFunction;
    void_t*            pDevice;
    u32_t              reconnectTime;
    u32_t              inactiveTicks;
    gos_tid_t          deviceHandlerTaskId;
    bool_t             devicePrevState;
}svl_dhsDescriptor_t;

// TODO
gos_result_t svl_deviceHandlerInit (
        void_t
        );

// TODO
gos_result_t svl_deviceHandlerRegisterDevice (
		svl_dhsDescriptor_t* pDevice
        );

#endif
