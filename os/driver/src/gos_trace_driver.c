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
//! @file       gos_trace_driver.c
//! @author     Gabor Repasi
//! @date       2023-01-13
//! @version    1.2
//!
//! @brief      GOS trace driver source.
//! @details    For a more detailed description of this driver, please refer to
//!             @ref gos_trace_driver.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-12-11    Gabor Repasi    Initial version created.
// 1.1        2022-12-13    Gabor Repasi    +    Unsafe transmit string function added
// 1.2        2023-01-13    Gabor Repasi    *    Service renamed to trace
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
/*
 * Includes
 */
#include <gos_trace_driver.h>
#include <gos_driver.h>

/*
 * External variables
 */
GOS_EXTERN gos_driver_functions_t driverFunctions;

/*
 * Function: gos_traceDriverTransmitString
 */
gos_result_t gos_traceDriverTransmitString (char_t* pString)
{
    /*
     * Local variables.
     */
    gos_result_t traceDriverTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (driverFunctions.traceDriverTransmitString != NULL)
    {
        traceDriverTransmitResult = driverFunctions.traceDriverTransmitString(pString);
    }

    return traceDriverTransmitResult;
}

/*
 * Function: gos_traceDriverTransmitString_Unsafe
 */
gos_result_t gos_traceDriverTransmitString_Unsafe (char_t* pString)
{
    /*
     * Local variables.
     */
    gos_result_t traceDriverTransmitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (driverFunctions.traceDriverTransmitStringUnsafe != NULL)
    {
        traceDriverTransmitResult = driverFunctions.traceDriverTransmitStringUnsafe(pString);
    }

    return traceDriverTransmitResult;
}
