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
//                                      (c) Ahmed Gazar, 2023
//
//*************************************************************************************************
//! @file       drv_flash.c
//! @author     Ahmed Gazar
//! @date       2023-09-26
//! @version    1.0
//!
//! @brief      FLASH driver source.
//! @details    For a more detailed description of this module, please refer to @ref drv_flash.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-09-26    Ahmed Gazar     Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2023 Ahmed Gazar
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
#include <drv_flash.h>
#include <stm32f4xx_hal_flash.h>
#include <stm32f4xx_hal_flash_ex.h>

/*
 * Function prototypes.
 */
GOS_STATIC u32_t drv_flashGetSector     (u32_t address);
GOS_STATIC u32_t drv_flashGetSectorSize (u32_t address);

/*
 * Function: drv_flashErase
 */
gos_result_t drv_flashErase (u32_t address, u32_t requiredSize)
{
    /*
     * Local variables.
     */
    gos_result_t           flashEraseResult = GOS_ERROR;
    FLASH_EraseInitTypeDef eraseInitStruct  = {0};
    u32_t                  startSector      = 0u;
    u32_t                  endSector        = 0u;
    u32_t                  endSectorAddress = 0u;
    u32_t                  sectorError      = 0u;

    /*
     * Function code.
     */
    (void_t) HAL_FLASH_Unlock();

    startSector = drv_flashGetSector(address);
    endSectorAddress = address + requiredSize - 1;
    endSector = drv_flashGetSector(endSectorAddress);

    eraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    eraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    eraseInitStruct.Sector        = startSector;
    eraseInitStruct.NbSectors     = (endSector - startSector) + 1;

    if (HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError) == HAL_OK)
    {
        flashEraseResult = GOS_SUCCESS;
    }
    else
    {
        // Flash erase error.
    }

    (void_t) HAL_FLASH_Lock();

    return flashEraseResult;
}

/*
 * Function: drv_flashWrite
 */
gos_result_t drv_flashWrite (u32_t address, void_t* pData, u32_t size)
{
    /*
     * Local variables.
     */
    gos_result_t flashWriteResult = GOS_ERROR;
    u32_t        addressCounter   = 0u;
    u32_t        idx              = 0u;

    /*
     * Function code.
     */
    (void_t) HAL_FLASH_Unlock();

    // Preset flag.
    flashWriteResult = GOS_SUCCESS;

    for (addressCounter = 0u; addressCounter < size; addressCounter+=4)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (address + addressCounter),
            ((u32_t*)pData)[idx++]) == HAL_OK)
        {
            // Programming OK, continue.
        }
        else
        {
            flashWriteResult = GOS_ERROR;
            break;
        }
    }

    (void_t) HAL_FLASH_Lock();

    return flashWriteResult;
}

/*
 * Function: drv_flashWriteWithoutLock
 */
gos_result_t drv_flashWriteWithoutLock (u32_t address, void_t* pData, u32_t size)
{
    /*
     * Local variables.
     */
    gos_result_t flashWriteResult = GOS_ERROR;
    u32_t        addressCounter   = 0u;
    u32_t        idx              = 0u;

    /*
     * Function code.
     */
    // Preset flag.
    flashWriteResult = GOS_SUCCESS;

    for (addressCounter = 0u; addressCounter < size; addressCounter+=4)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (address + addressCounter),
            ((u32_t*)pData)[idx++]) == HAL_OK)
        {
            // Programming OK, continue.
            GOS_NOP;
        }
        else
        {
            flashWriteResult = GOS_ERROR;
            break;
        }
    }

    return flashWriteResult;
}

/*
 * Function: drv_flashUnlock
 */
gos_result_t drv_flashUnlock (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t unlockResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (HAL_FLASH_Unlock() == HAL_OK)
    {
        unlockResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return unlockResult;
}

/*
 * Function: drv_flashLock
 */
gos_result_t drv_flashLock (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t lockResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (HAL_FLASH_Lock() == HAL_OK)
    {
        lockResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return lockResult;
}

/*
 * Function: drv_flashRead
 */
gos_result_t drv_flashRead (u32_t address, void_t* pData, u32_t size)
{
    /*
     * Local variables.
     */
    gos_result_t flashReadResult = GOS_SUCCESS;
    u32_t        addressCounter  = 0u;

    /*
     * Function code.
     */
    (void_t) HAL_FLASH_Unlock();

    for (addressCounter = 0u; addressCounter < size; addressCounter++)
    {
        ((u8_t*)pData)[addressCounter] = *(__IO u8_t*)(address + addressCounter);
    }

    (void_t) HAL_FLASH_Lock();

    return flashReadResult;
}

/**
 * @brief   Returns the corresponding sector number for the given address.
 * @details Returns the corresponding sector number for the given address.
 *
 * @param   address : Address to get the related sector number for.
 *
 * @return  Sector number.
 */
GOS_STATIC u32_t drv_flashGetSector (u32_t address)
{
    /*
     * Local variables.
     */
    u32_t sector = 0u;

    /*
     * Function code.
     */
    if ((address <= 0x08003FFF) && (address >= 0x08000000))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((address <= 0x08007FFF) && (address >= 0x08004000))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((address <= 0x0800BFFF) && (address >= 0x08008000))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((address <= 0x0800FFFF) && (address >= 0x0800C000))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((address <= 0x0801FFFF) && (address >= 0x08010000))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((address <= 0x0803FFFF) && (address >= 0x08020000))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((address <= 0x0805FFFF) && (address >= 0x08040000))
    {
        sector = FLASH_SECTOR_6;
    }
    else if ((address <= 0x0807FFFF) && (address >= 0x08060000))
    {
        sector = FLASH_SECTOR_7;
    }
    else
    {
        // Wrong address.
    }

    return sector;
}

/**
 * @brief   Returns the corresponding sector size for the given address.
 * @details Returns the corresponding sector size for the given address.
 *
 * @param   address : Address to get the related sector size for.
 *
 * @return  Sector size.
 */
GOS_UNUSED GOS_STATIC u32_t drv_flashGetSectorSize (u32_t address)
{
    /*
     * Local variables.
     */
    u32_t sector     = 0u;
    u32_t sectorSize = 0u;

    /*
     * Function code.
     */
    sector = drv_flashGetSector(address);

    switch (sector)
    {
        case FLASH_SECTOR_0:
        {
            sectorSize = 0x4000u;
            break;
        }
        case FLASH_SECTOR_1:
        {
            sectorSize = 0x4000u;
            break;
        }
        case FLASH_SECTOR_2:
        {
            sectorSize = 0x4000u;
            break;
        }
        case FLASH_SECTOR_3:
        {
            sectorSize = 0x10000u;
            break;
        }
        case FLASH_SECTOR_4:
        {
            sectorSize = 0x20000u;
            break;
        }
        case FLASH_SECTOR_5:
        {
            sectorSize = 0x20000u;
            break;
        }
        case FLASH_SECTOR_6:
        {
            sectorSize = 0x20000u;
            break;
        }
        case FLASH_SECTOR_7:
        {
            sectorSize = 0x20000u;
            break;
        }
        default: break;
    }

    return sectorSize;
}
