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
//! @file       drv.c
//! @author     Ahmed Gazar
//! @date       2024-02-23
//! @version    1.0
//!
//! @brief      GOS2022 Library / Driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-23    Ahmed Gazar     Initial version created.
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
#include <drv.h>
#include <string.h>

/*
 * Macros
 */
/**
 * Driver major version.
 */
#define DRIVER_VERSION_MAJOR      ( 0                 )

/**
 * Driver minor version.
 */
#define DRIVER_VERSION_MINOR      ( 1                 )

/**
 * Driver build number.
 */
#define DRIVER_VERSION_BUILD      ( 0                 )

/**
 * Driver version date / year.
 */
#define DRIVER_VERSION_DATE_YEAR  ( 2024              )

/**
 * Driver version date / month.
 */
#define DRIVER_VERSION_DATE_MONTH ( GOS_TIME_FEBRUARY )

/**
 * Driver version date / day.
 */
#define DRIVER_VERSION_DATE_DAY   ( 27                )

/**
 * Driver name.
 */
#define DRIVER_NAME               "GOS2022_drv_pckg"

/**
 * Driver description.
 */
#define DRIVER_DESCRIPTION        "GOS2022 device driver package."

/**
 * Driver author.
 */
#define DRIVER_AUTHOR             "Ahmed Ibrahim Gazar"

/*
 * Function: drv_getVersion
 */
gos_result_t drv_getVersion (lib_version_t* pDrvVersion)
{
    /*
     * Local variables.
     */
    gos_result_t getVersionResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pDrvVersion != NULL)
    {
        pDrvVersion->major       = DRIVER_VERSION_MAJOR;
        pDrvVersion->minor       = DRIVER_VERSION_MINOR;
        pDrvVersion->build       = DRIVER_VERSION_BUILD;
        pDrvVersion->date.years  = DRIVER_VERSION_DATE_YEAR;
        pDrvVersion->date.months = DRIVER_VERSION_DATE_MONTH;
        pDrvVersion->date.days   = DRIVER_VERSION_DATE_DAY;

        (void_t) strcpy((char_t*)pDrvVersion->name, DRIVER_NAME);
        (void_t) strcpy((char_t*)pDrvVersion->description, DRIVER_DESCRIPTION);
        (void_t) strcpy((char_t*)pDrvVersion->author, DRIVER_AUTHOR);

        getVersionResult = GOS_SUCCESS;
    }
    else
    {
        // Target is NULL.
    }

    return getVersionResult;
}
