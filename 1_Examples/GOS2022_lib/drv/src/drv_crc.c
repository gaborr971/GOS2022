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
//! @file       drv_crc.c
//! @author     Ahmed Gazar
//! @date       2024-02-01
//! @version    1.0
//!
//! @brief      GOS2022 Library / Cyclic Redundancy Check driver source.
//! @details    For a more detailed description of this driver, please refer to
//!             @ref drv_crc.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-01    Ahmed Gazar     Initial version created.
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
#include <drv_crc.h>

/*
 * Macros
 */
/**
 * 8-bit CRC initial value.
 */
#define DRV_CRC_INIT_VALUE_CRC8     ( 0xFFu               )
/**
 * 16-bit CRC initial value.
 */
#define DRV_CRC_INIT_VALUE_CRC16    ( 0xFFFFu             )
/**
 * 32-bit CRC initial value.
 */
#define DRV_CRC_INIT_VALUE_CRC32    ( 0xFFFFFFFFu         )
/**
 * 64-bit CRC initial value.
 */
#define DRV_CRC_INIT_VALUE_CRC64    ( 0xFFFFFFFFFFFFFFFFu )

/**
 * 8-bit CRC mask value.
 */
#define DRV_CRC_MASK_CRC8           ( 0x80u               )
/**
 * 16-bit CRC mask value.
 */
#define DRV_CRC_MASK_CRC16          ( 0x8000u             )
/**
 * 32-bit CRC mask value.
 */
#define DRV_CRC_MASK_CRC32          ( 0x80000000u         )
/**
 * 64-bit CRC mask value.
 */
#define DRV_CRC_MASK_CRC64          ( 0x8000000000000000u )

/**
 * 8-bit CRC polynomial value.
 */
#define DRV_CRC_POLYNOMIAL_CRC8     ( 0x07u               )
/**
 * 16-bit CRC polynomial value.
 */
#define DRV_CRC_POLYNOMIAL_CRC16    ( 0x8005u             )
/**
 * 32-bit CRC polynomial value.
 */
#define DRV_CRC_POLYNOMIAL_CRC32    ( 0x04C11DB7u         )
/**
 * 64-bit CRC polynomial value.
 */
#define DRV_CRC_POLYNOMIAL_CRC64    ( 0x3A7FF201D298EE14u )

/*
 * Function: drv_crcGetCrc8
 */
gos_result_t drv_crcGetCrc8 (u8_t* pData, u32_t dataSize, u8_t* pCrc)
{
    /*
     * Local variables.
     */
    gos_result_t crcGetResult = GOS_ERROR;
    u32_t        index        = 0u;
    u8_t         bitCounter   = 0u;
    u8_t         crcValue     = DRV_CRC_INIT_VALUE_CRC8;

    /*
     * Function code.
     */
    if (pData != NULL && pCrc != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            crcValue ^= pData[index];

            for (bitCounter = 0; bitCounter < 8u; bitCounter++)
            {
                if (0 != (DRV_CRC_MASK_CRC8 & crcValue))
                {
                    crcValue <<= 1u;
                    crcValue ^= DRV_CRC_POLYNOMIAL_CRC8;
                }
                else
                {
                    crcValue <<= 1u;
                }
            }
        }

        *pCrc = crcValue;
        crcGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return crcGetResult;
}

/*
 * Function: drv_crcGetCrc16
 */
gos_result_t drv_crcGetCrc16 (u8_t* pData, u32_t dataSize, u16_t* pCrc)
{
    /*
     * Local variables.
     */
    gos_result_t crcGetResult = GOS_ERROR;
    u32_t        index        = 0u;
    u8_t         bitCounter   = 0u;
    u16_t        crcValue     = DRV_CRC_INIT_VALUE_CRC16;

    /*
     * Function code.
     */
    if (pData != NULL && pCrc != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            crcValue ^= pData[index];

            for (bitCounter = 0; bitCounter < 16u; bitCounter++)
            {
                if (0 != (DRV_CRC_MASK_CRC16 & crcValue))
                {
                    crcValue <<= 1u;
                    crcValue ^= DRV_CRC_POLYNOMIAL_CRC16;
                }
                else
                {
                    crcValue <<= 1u;
                }
            }
        }

        *pCrc = crcValue;
        crcGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return crcGetResult;
}

/*
 * Function: drv_crcGetCrc32
 */
gos_result_t drv_crcGetCrc32 (u8_t* pData, u32_t dataSize, u32_t* pCrc)
{
    /*
     * Local variables.
     */
    gos_result_t crcGetResult = GOS_ERROR;
    u32_t        index        = 0u;
    u8_t         bitCounter   = 0u;
    u32_t        crcValue     = DRV_CRC_INIT_VALUE_CRC32;

    /*
     * Function code.
     */
    if (pData != NULL && pCrc != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            crcValue ^= pData[index];

            for (bitCounter = 0; bitCounter < 32u; bitCounter++)
            {
                if (0 != (DRV_CRC_MASK_CRC32 & crcValue))
                {
                    crcValue <<= 1u;
                    crcValue ^= DRV_CRC_POLYNOMIAL_CRC32;
                }
                else
                {
                    crcValue <<= 1u;
                }
            }
        }

        *pCrc = crcValue;
        crcGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return crcGetResult;
}

/*
 * Function: drv_crcGetCrc64
 */
gos_result_t drv_crcGetCrc64 (u8_t* pData, u32_t dataSize, u64_t* pCrc)
{
    /*
     * Local variables.
     */
    gos_result_t crcGetResult = GOS_ERROR;
    u32_t        index        = 0u;
    u8_t         bitCounter   = 0u;
    u64_t        crcValue     = DRV_CRC_INIT_VALUE_CRC64;

    /*
     * Function code.
     */
    if (pData != NULL && pCrc != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            crcValue ^= pData[index];

            for (bitCounter = 0; bitCounter < 64u; bitCounter++)
            {
                if (0 != (DRV_CRC_MASK_CRC64 & crcValue))
                {
                    crcValue <<= 1u;
                    crcValue ^= DRV_CRC_POLYNOMIAL_CRC64;
                }
                else
                {
                    crcValue <<= 1u;
                }
            }
        }

        *pCrc = crcValue;
        crcGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return crcGetResult;
}

/*
 * Function: drv_crcCheckCrc8
 */
drv_crcCheckResult_t drv_crcCheckCrc8  (u8_t* pData, u32_t dataSize, u8_t expCrc, u8_t* calcCrc)
{
    /*
     * Local variables.
     */
    drv_crcCheckResult_t crcCheckResult = DRV_CRC_CHECK_ERROR;
    u8_t                 crcTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetCrc8(pData, dataSize, &crcTempValue) == GOS_SUCCESS)
    {
        // Check if CRCs match.
        if (crcTempValue == expCrc)
        {
            crcCheckResult = DRV_CRC_CHECK_OK;
        }
        else
        {
            // CRC mismatch.
        }

        // Return calculated CRC value.
        if (calcCrc != NULL)
        {
            *calcCrc = crcTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return crcCheckResult;
}

/*
 * Function: drv_crcCheckCrc16
 */
drv_crcCheckResult_t drv_crcCheckCrc16 (u8_t* pData, u32_t dataSize, u16_t expCrc, u16_t* calcCrc)
{
    /*
     * Local variables.
     */
    drv_crcCheckResult_t crcCheckResult = DRV_CRC_CHECK_ERROR;
    u16_t                crcTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetCrc16(pData, dataSize, &crcTempValue) == GOS_SUCCESS)
    {
        // Check if CRCs match.
        if (crcTempValue == expCrc)
        {
            crcCheckResult = DRV_CRC_CHECK_OK;
        }
        else
        {
            // CRC mismatch.
        }

        // Return calculated CRC value.
        if (calcCrc != NULL)
        {
            *calcCrc = crcTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return crcCheckResult;
}

/*
 * Function: drv_crcCheckCrc32
 */
drv_crcCheckResult_t drv_crcCheckCrc32 (u8_t* pData, u32_t dataSize, u32_t expCrc, u32_t* calcCrc)
{
    /*
     * Local variables.
     */
    drv_crcCheckResult_t crcCheckResult = DRV_CRC_CHECK_ERROR;
    u32_t                crcTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetCrc32(pData, dataSize, &crcTempValue) == GOS_SUCCESS)
    {
        // Check if CRCs match.
        if (crcTempValue == expCrc)
        {
            crcCheckResult = DRV_CRC_CHECK_OK;
        }
        else
        {
            // CRC mismatch.
        }

        // Return calculated CRC value.
        if (calcCrc != NULL)
        {
            *calcCrc = crcTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return crcCheckResult;
}

/*
 * Function: drv_crcCheckCrc64
 */
drv_crcCheckResult_t drv_crcCheckCrc64 (u8_t* pData, u32_t dataSize, u64_t expCrc, u64_t* calcCrc)
{
    /*
     * Local variables.
     */
    drv_crcCheckResult_t crcCheckResult = DRV_CRC_CHECK_ERROR;
    u64_t                crcTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetCrc64(pData, dataSize, &crcTempValue) == GOS_SUCCESS)
    {
        // Check if CRCs match.
        if (crcTempValue == expCrc)
        {
            crcCheckResult = DRV_CRC_CHECK_OK;
        }
        else
        {
            // CRC mismatch.
        }

        // Return calculated CRC value.
        if (calcCrc != NULL)
        {
            *calcCrc = crcTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return crcCheckResult;
}

/*
 * Function: drv_crcGetChkSum8
 */
gos_result_t drv_crcGetChkSum8 (u8_t* pData, u32_t dataSize, u8_t* pChkSum)
{
    /*
     * Local variables.
     */
    gos_result_t chkSumGetResult = GOS_ERROR;
    u32_t        index           = 0u;
    u8_t         chkSumValue     = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && pChkSum != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            chkSumValue += pData[index];
        }

        *pChkSum = chkSumValue;
        chkSumGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return chkSumGetResult;
}

/*
 * Function: drv_crcGetChkSum16
 */
gos_result_t drv_crcGetChkSum16 (u8_t* pData, u32_t dataSize, u16_t* pChkSum)
{
    /*
     * Local variables.
     */
    gos_result_t chkSumGetResult = GOS_ERROR;
    u32_t        index           = 0u;
    u16_t        chkSumValue     = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && pChkSum != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            chkSumValue += pData[index];
        }

        *pChkSum = chkSumValue;
        chkSumGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return chkSumGetResult;
}

/*
 * Function: drv_crcGetChkSum32
 */
gos_result_t drv_crcGetChkSum32 (u8_t* pData, u32_t dataSize, u32_t* pChkSum)
{
    /*
     * Local variables.
     */
    gos_result_t chkSumGetResult = GOS_ERROR;
    u32_t        index           = 0u;
    u32_t        chkSumValue     = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && pChkSum != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            chkSumValue += pData[index];
        }

        *pChkSum = chkSumValue;
        chkSumGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return chkSumGetResult;
}

/*
 * Function: drv_crcGetChkSum64
 */
gos_result_t drv_crcGetChkSum64 (u8_t* pData, u32_t dataSize, u64_t* pChkSum)
{
    /*
     * Local variables.
     */
    gos_result_t chkSumGetResult = GOS_ERROR;
    u32_t        index           = 0u;
    u64_t        chkSumValue     = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && pChkSum != NULL)
    {
        for (index = 0u; index < dataSize; index++)
        {
            chkSumValue += pData[index];
        }

        *pChkSum = chkSumValue;
        chkSumGetResult = GOS_SUCCESS;
    }
    else
    {
        // Error.
    }

    return chkSumGetResult;
}

/*
 * Function: drv_crcCheckChkSum8
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum8 (u8_t* pData, u32_t dataSize, u8_t expChkSum, u8_t* calcChkSum)
{
    /*
     * Local variables.
     */
    drv_crcChkSumCheckResult_t chkSumCheckResult = DRV_CHKSUM_CHECK_ERROR;
    u8_t                       chkSumTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetChkSum8(pData, dataSize, &chkSumTempValue) == GOS_SUCCESS)
    {
        // Check if CHKs match.
        if (chkSumTempValue == expChkSum)
        {
            chkSumCheckResult = DRV_CHKSUM_CHECK_OK;
        }
        else
        {
            // CHK mismatch.
        }

        // Return calculated CHK value.
        if (calcChkSum != NULL)
        {
            *calcChkSum = chkSumTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return chkSumCheckResult;
}

/*
 * Function: drv_crcCheckChkSum16
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum16 (u8_t* pData, u32_t dataSize, u16_t expChkSum, u16_t* calcChkSum)
{
    /*
     * Local variables.
     */
    drv_crcChkSumCheckResult_t chkSumCheckResult = DRV_CHKSUM_CHECK_ERROR;
    u16_t                      chkSumTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetChkSum16(pData, dataSize, &chkSumTempValue) == GOS_SUCCESS)
    {
        // Check if CHKs match.
        if (chkSumTempValue == expChkSum)
        {
            chkSumCheckResult = DRV_CHKSUM_CHECK_OK;
        }
        else
        {
            // CHK mismatch.
        }

        // Return calculated CHK value.
        if (calcChkSum != NULL)
        {
            *calcChkSum = chkSumTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return chkSumCheckResult;
}

/*
 * Function: drv_crcCheckChkSum32
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum32 (u8_t* pData, u32_t dataSize, u32_t expChkSum, u32_t* calcChkSum)
{
    /*
     * Local variables.
     */
    drv_crcChkSumCheckResult_t chkSumCheckResult = DRV_CHKSUM_CHECK_ERROR;
    u32_t                      chkSumTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetChkSum32(pData, dataSize, &chkSumTempValue) == GOS_SUCCESS)
    {
        // Check if CHKs match.
        if (chkSumTempValue == expChkSum)
        {
            chkSumCheckResult = DRV_CHKSUM_CHECK_OK;
        }
        else
        {
            // CHK mismatch.
        }

        // Return calculated CHK value.
        if (calcChkSum != NULL)
        {
            *calcChkSum = chkSumTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return chkSumCheckResult;
}

/*
 * Function: drv_crcCheckChkSum64
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum64 (u8_t* pData, u32_t dataSize, u64_t expChkSum, u64_t* calcChkSum)
{
    /*
     * Local variables.
     */
    drv_crcChkSumCheckResult_t chkSumCheckResult = DRV_CHKSUM_CHECK_ERROR;
    u64_t                      chkSumTempValue   = 0u;

    /*
     * Function code.
     */
    if (pData != NULL && drv_crcGetChkSum64(pData, dataSize, &chkSumTempValue) == GOS_SUCCESS)
    {
        // Check if CHKs match.
        if (chkSumTempValue == expChkSum)
        {
            chkSumCheckResult = DRV_CHKSUM_CHECK_OK;
        }
        else
        {
            // CHK mismatch.
        }

        // Return calculated CHK value.
        if (calcChkSum != NULL)
        {
            *calcChkSum = chkSumTempValue;
        }
        else
        {
            // Calculated value is not needed.
        }
    }
    else
    {
        // Error.
    }

    return chkSumCheckResult;
}
