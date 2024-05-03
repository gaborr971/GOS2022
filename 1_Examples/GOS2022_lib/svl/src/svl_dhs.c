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
//! @file       svl_dhs.c
//! @author     Ahmed Gazar
//! @date       2024-04-13
//! @version    1.0
//!
//! @brief      GOS2022 Library / Device Handler Service source.
//! @details    For a more detailed description of this driver, please refer to @ref svl_dhs.h
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
/*
 * Includes
 */
#include <string.h>
#include <svl_dhs.h>

/*
 * Static variables
 */
GOS_STATIC svl_dhsDescriptor_t devices [SVL_DHS_MAX_DEVICES];

/*
 * Function prototypes
 */
GOS_STATIC void_t svl_deviceHandlerDaemon (void_t);

/**
 * TODO
 */
GOS_STATIC gos_taskDescriptor_t deviceHandlerDaemonDesc =
{
    .taskFunction       = svl_deviceHandlerDaemon,
    .taskStackSize      = 0x300,
    .taskPriority       = 200,
    .taskName           = "svl_device_handler_daemon",
    .taskPrivilegeLevel = GOS_TASK_PRIVILEGE_SUPERVISOR
};

/*
 * Function: svl_deviceHandlerInit
 */
gos_result_t svl_deviceHandlerInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t initResult = GOS_ERROR;

    /*
     * Function code.
     */
    initResult = gos_taskRegister(&deviceHandlerDaemonDesc, NULL);

    return initResult;
}

/*
 * Function: svl_deviceHandlerRegisterDevice
 */
gos_result_t svl_deviceHandlerRegisterDevice (svl_dhsDescriptor_t* pDevice)
{
    /*
     * Local variables.
     */
    gos_result_t registerDeviceResult = GOS_ERROR;
    u8_t         deviceIdx            = 0u;

    /*
     * Function code.
     */
    if (pDevice != NULL && pDevice->availableFunction != NULL)
    {
        for (deviceIdx = 0u; deviceIdx < SVL_DHS_MAX_DEVICES; deviceIdx++)
        {
            if (devices[deviceIdx].initFunction == NULL &&
                devices[deviceIdx].availableFunction == NULL)
            {
                (void_t*) memcpy((void_t*)&(devices[deviceIdx]), (void_t*)pDevice, sizeof(*pDevice));
                devices[deviceIdx].devicePrevState = GOS_TRUE;
                registerDeviceResult = GOS_SUCCESS;
                break;
            }
            else
            {
                // Continue checking.
            }
        }
    }
    else
    {
        // Device descriptor error.
    }

    return registerDeviceResult;
}

/**
 * TODO
 * @param
 * @return
 */
GOS_STATIC void_t svl_deviceHandlerDaemon (void_t)
{
    /*
     * Local variables.
     */
    u8_t  deviceIdx   = 0u;
    u32_t kernelTicks = gos_kernelGetSysTicks();

    /*
     * Function code.
     */
    for (;;)
    {
        for (deviceIdx = 0u; deviceIdx < SVL_DHS_MAX_DEVICES; deviceIdx++)
        {
            kernelTicks = gos_kernelGetSysTicks();

            if (devices[deviceIdx].availableFunction == NULL &&
                devices[deviceIdx].initFunction == NULL)
            {
                // Last handler was found, no more to check.
                break;
            }
            else
            {
                // If device was previously not available...
                if (devices[deviceIdx].devicePrevState == GOS_FALSE)
                {
                    // If ticks elapsed.
                    if (kernelTicks - devices[deviceIdx].inactiveTicks > devices[deviceIdx].reconnectTime)
                    {
                        // Re-initialize.
                        (void_t) devices[deviceIdx].initFunction(devices[deviceIdx].pDevice);

                        // Check availability.
                        devices[deviceIdx].devicePrevState = devices[deviceIdx].availableFunction(devices[deviceIdx].pDevice);

                        // If it is available now, resume handler task.
                        if (devices[deviceIdx].devicePrevState == GOS_TRUE)
                        {
                            (void_t) gos_taskResume(devices[deviceIdx].deviceHandlerTaskId);
                        }
                        else
                        {
                            // Retry, when time elapsed again.
                            // Update ticks.
                            devices[deviceIdx].inactiveTicks = kernelTicks;
                        }
                    }
                    else
                    {
                        // Wait.
                    }
                }
                else
                {
                    // If device is not present, suspend handler.
                    if (devices[deviceIdx].availableFunction(devices[deviceIdx].pDevice) == GOS_FALSE)
                    {
                        // Update ticks.
                        devices[deviceIdx].inactiveTicks   = kernelTicks;
                        devices[deviceIdx].devicePrevState = GOS_FALSE;
                        (void_t) gos_taskSuspend(devices[deviceIdx].deviceHandlerTaskId);
                    }
                    else
                    {
                        // Device is OK.
                    }
                }
            }
        }

        (void_t) gos_taskSleep(250);
    }
}
