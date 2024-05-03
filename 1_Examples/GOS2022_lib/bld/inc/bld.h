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
//! @file       bld.h
//! @author     Ahmed Gazar
//! @date       2024-02-07
//! @version    1.0
//!
//! @brief      GOS2022 Library / Bootloader header.
//! @details    TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-07    Ahmed Gazar     Initial version created.
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
#ifndef BLD_H
#define BLD_H

/*
 * Includes
 */
#include <gos.h>
#include <libdef.h>
#include <bld_data.h>

/*
 * Type definitions
 */
/**
 * Bootloader state enumerator.
 */
typedef enum
{
    BLD_STATE_CONNECTION_ESTABLISHMENT,    //!< Conncetion establishment phase.
    BLD_STATE_REQUEST_WAIT,                //!< Request wait phase.
    BLD_STATE_DATA_REQUEST,                //!< Data request phase.
    BLD_STATE_CONNECTION_CLOSE,            //!< Connection close phase.
    BLD_STATE_APP_INSTALL,                 //!< Application install phase.
    BLD_STATE_APP_CHECK                    //!< Application check phase.
}bld_state_t;

/*
 * Function prototypes
 */
/**
 * @brief   TODO
 * @details TODO
 *
 * @param   pBldVersion : TODO
 *
 * @return  TODO
 *
 * @retval  TODO
 * @retval  TODO
 */
gos_result_t bld_getVersion (lib_version_t* pBldVersion);

/**
 * @brief   TODO
 * @details TODO
 *
 * @param   pAppData : TODO
 *
 * @return  TODO
 *
 * @retval  TODO
 * @retval  TODO
 */
gos_result_t bld_checkApplication (bld_appData_t* pAppData, bool_t bootMode);

/**
 * @brief   TODO
 * @details TODO
 * *
 * @return  TODO
 *
 * @retval  TODO
 */
gos_result_t bld_jumpToApplication (void_t);

/**
 * @brief   TODO
 * @details TODO
 *
 * @return  TODO
 *
 * @retval  TODO
 */
gos_result_t bld_stateMachineInitialize (void_t);

/**
 * @brief   TODO
 * @details TODO
 *
 * @param pState
 *
 * @return  TODO
 *
 * @retval  TODO
 */
gos_result_t bld_getState (bld_state_t* pState);

/**
 * @brief   TODO
 * @details TODO
 *
 * @return  TODO
 *
 * @retval  TODO
 */
gos_result_t bld_stateMachineRun (void_t);

#endif
