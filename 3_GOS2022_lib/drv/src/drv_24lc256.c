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
//! @file       drv_24lc256.c
//! @author     Ahmed Gazar
//! @date       2024-04-15
//! @version    1.0
//!
//! @brief      GOS2022 Library / 24LC256 driver source.
//! @details    For a more detailed description of this driver, please refer to @ref drv_24lc256.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-15    Ahmed Gazar     Initial version created.
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
#include <drv_24lc256.h>

/*
 * Function: drv_24lc256Init
 */
gos_result_t drv_24lc256Init (void_t* pDevice)
{
	// TODO: to be implemented.
	return GOS_SUCCESS;
}

/*
 * Function: drv_24lc256IsDeviceAvailable
 */
bool_t drv_24lc256IsDeviceAvailable (void_t* pDevice)
{
	// TODO: to be implemented.
	return GOS_SUCCESS;
}

/*
 * Function: drv_24lc256Read
 */
gos_result_t drv_24lc256Read (void_t* pDevice, u16_t address, u8_t* pTarget, u16_t size)
{
	/*
	 * Local variables.
	 */
	gos_result_t readResult  = GOS_SUCCESS;
    u16_t        readCounter = 0u;

	/*
	 * Function code.
	 */
	while (readCounter < size && readResult != GOS_ERROR)
	{
		readResult = drv_i2cMemRead(
				((drv_24lc256Descriptor_t*)pDevice)->i2cInstance, ((drv_24lc256Descriptor_t*)pDevice)->deviceAddress,
				address + readCounter, 2u, (u8_t*)(pTarget + readCounter), 1u,
				((drv_24lc256Descriptor_t*)pDevice)->readMutexTmo, ((drv_24lc256Descriptor_t*)pDevice)->readTriggerTmo);
		readCounter += 1u;

		gos_taskSleep(5);
	}

	return readResult;
}

/*
 * Function: drv_24lc256Write
 */
gos_result_t drv_24lc256Write (void_t* pDevice, u16_t address, u8_t* pData, u16_t size)
{
	/*
	 * Local variables.
	 */
	gos_result_t writeResult  = GOS_SUCCESS;
    u16_t        writeCounter = 0u;
    u16_t        diff         = 0u;

	/*
	 * Function code.
	 */
	while (writeCounter < size && writeResult != GOS_ERROR)
	{
		writeResult = drv_i2cMemWrite(
				((drv_24lc256Descriptor_t*)pDevice)->i2cInstance, ((drv_24lc256Descriptor_t*)pDevice)->deviceAddress,
				address + writeCounter, 2u, (u8_t*)(pData + writeCounter), 1u,
				((drv_24lc256Descriptor_t*)pDevice)->writeMutexTmo, ((drv_24lc256Descriptor_t*)pDevice)->writeTriggerTmo);
		writeCounter += 1u;

		gos_taskSleep(10);
	}

	return writeResult;
}
