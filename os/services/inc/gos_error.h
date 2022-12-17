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
//                                      (c) Gabor Repasi, 2022
//
//*************************************************************************************************
//! @file       gos_error.h
//! @author     Gabor Repasi
//! @date       2022-12-11
//! @version    1.0
//!
//! @brief      GOS error handler header.
//! @details    This service is used for tracing error and initialization messages on the log
//!             output.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-12-11    Gabor Repasi    Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2022 Gabor Repasi
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
#ifndef GOS_ERROR_H
#define GOS_ERROR_H
/*
 * Includes
 */
#include "gos_kernel.h"
#include "gos_log.h"
#include <stdarg.h>

/*
 * Type definitions
 */
/**
 * Error level enumerator.
 */
typedef enum
{
    GOS_ERROR_LEVEL_OS_FATAL     = 0b110100,    //!< OS-level fatal error causing the system to stop.
    GOS_ERROR_LEVEL_OS_WARNING   = 0b101110,    //!< OS-level warning message will be logged, but system will not be stopped.
    GOS_ERROR_LEVEL_USER_FATAL   = 0b011010,    //!< User-level fatal error causing system stop.
    GOS_ERROR_LEVEL_USER_WARNING = 0b111010     //!< User-level warning.
}gos_errorLevel_t;

/*
 * Function prototypes
 */

/**
 * @brief   Prints the startup logo on the log output.
 * @details Prints the GOS logo (similar to the ones used in the file headers) on the log output.
 *
 * @return    -
 */
void_t gos_printStartupLogo (void_t);

/**
 * @brief   Handles the given error.
 * @details Prints the formatted error message on the log output, and based on the error
 *          level, it returns or stays in an infinite loop and disables scheduling.
 *
 * @param   errorLevel   : Level of error (OS/user, warning/fatal).
 * @param   function     : Function name.
 * @param   line         : Line number.
 * @param   errorMessage : Error message.
 *
 * @return  -
 */
void_t gos_errorHandler (
        gos_errorLevel_t errorLevel,
        const char_t* function,
        u32_t line,
        const char_t* errorMessage,
        ...);

/**
 * @brief   Traces an initialization message.
 * @details Writes the formatted initialization message on the log output.
 *
 * @param   initDescription : Message to describe the initialization step.
 * @param   initResult      : Result of initialization.
 *
 * @return  -
 */
gos_result_t gos_traceInit (
        const char_t* initDescription,
        gos_result_t initResult
        );

#endif
