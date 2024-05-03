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
//! @file       drv_crc.h
//! @author     Ahmed Gazar
//! @date       2024-02-01
//! @version    1.0
//!
//! @brief      GOS2022 Library / Cyclic Redundancy Check driver header.
//! @details    This driver provides CRC8/CRC16/CRC32/CRC64 and CHK8/CHK16/CHK32/CHK64 algorithms.
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
#ifndef DRV_CRC_H
#define DRV_CRC_H
/*
 * Includes
 */
#include <gos.h>

/*
 * Type definitions
 */
/**
 * CRC check result values.
 */
typedef enum
{
    DRV_CRC_CHECK_OK       = 0b11001010, //!< CRC match.
    DRV_CRC_CHECK_ERROR    = 0b10011100  //!< CRC mismatch.
}drv_crcCheckResult_t;

/**
 * Checksum check result value.
 */
typedef enum
{
    DRV_CHKSUM_CHECK_OK    = 0b10100111, //!< CHK match.
    DRV_CHKSUM_CHECK_ERROR = 0b11101001  //!< CHK mismatch.
}drv_crcChkSumCheckResult_t;

/*
 * Function prototypes
 */
/**
 * @brief   8-bit CRC calculator.
 * @details Calculates the 8-bit CRC value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pCrc     : Variable to store the result in.
 *
 * @return  Result of CRC calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pCrc is NULL).
 */
gos_result_t drv_crcGetCrc8  (u8_t* pData, u32_t dataSize, u8_t*  pCrc);

/**
 * @brief   16-bit CRC calculator.
 * @details Calculates the 16-bit CRC value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pCrc     : Variable to store the result in.
 *
 * @return  Result of CRC calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pCrc is NULL).
 */
gos_result_t drv_crcGetCrc16 (u8_t* pData, u32_t dataSize, u16_t* pCrc);

/**
 * @brief   32-bit CRC calculator.
 * @details Calculates the 32-bit CRC value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pCrc     : Variable to store the result in.
 *
 * @return  Result of CRC calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pCrc is NULL).
 */
gos_result_t drv_crcGetCrc32 (u8_t* pData, u32_t dataSize, u32_t* pCrc);

/**
 * @brief   64-bit CRC calculator.
 * @details Calculates the 64-bit CRC value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pCrc     : Variable to store the result in.
 *
 * @return  Result of CRC calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pCrc is NULL).
 */
gos_result_t drv_crcGetCrc64 (u8_t* pData, u32_t dataSize, u64_t* pCrc);

/**
 * @brief   8-bit CRC checker.
 * @details Calculates the 8-bit CRC value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   expCrc   : Expected value.
 * @param   calcCrc  : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CRC checking.
 *
 * @retval  DRV_CRC_CHECK_OK    : CRC match.
 * @retval  DRV_CRC_CHECK_ERROR : CRC mismatch or pData is NULL.
 */
drv_crcCheckResult_t drv_crcCheckCrc8  (u8_t* pData, u32_t dataSize, u8_t  expCrc, u8_t*  calcCrc);

/**
 * @brief   16-bit CRC checker.
 * @details Calculates the 16-bit CRC value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   expCrc   : Expected value.
 * @param   calcCrc  : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CRC checking.
 *
 * @retval  DRV_CRC_CHECK_OK    : CRC match.
 * @retval  DRV_CRC_CHECK_ERROR : CRC mismatch or pData is NULL.
 */
drv_crcCheckResult_t drv_crcCheckCrc16 (u8_t* pData, u32_t dataSize, u16_t expCrc, u16_t* calcCrc);

/**
 * @brief   32-bit CRC checker.
 * @details Calculates the 32-bit CRC value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   expCrc   : Expected value.
 * @param   calcCrc  : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CRC checking.
 *
 * @retval  DRV_CRC_CHECK_OK    : CRC match.
 * @retval  DRV_CRC_CHECK_ERROR : CRC mismatch or pData is NULL.
 */
drv_crcCheckResult_t drv_crcCheckCrc32 (u8_t* pData, u32_t dataSize, u32_t expCrc, u32_t* calcCrc);

/**
 * @brief   64-bit CRC checker.
 * @details Calculates the 64-bit CRC value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   expCrc   : Expected value.
 * @param   calcCrc  : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CRC checking.
 *
 * @retval  DRV_CRC_CHECK_OK    : CRC match.
 * @retval  DRV_CRC_CHECK_ERROR : CRC mismatch or pData is NULL.
 */
drv_crcCheckResult_t drv_crcCheckCrc64 (u8_t* pData, u32_t dataSize, u64_t expCrc, u64_t* calcCrc);

/**
 * @brief   8-bit CHK calculator.
 * @details Calculates the 8-bit CHK value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pChkSum  : Variable to store the result in.
 *
 * @return  Result of CHK calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pChkSum is NULL).
 */
gos_result_t drv_crcGetChkSum8 (u8_t* pData, u32_t dataSize, u8_t*  pChkSum);

/**
 * @brief   16-bit CHK calculator.
 * @details Calculates the 16-bit CHK value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pChkSum  : Variable to store the result in.
 *
 * @return  Result of CHK calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pChkSum is NULL).
 */
gos_result_t drv_crcGetChkSum16 (u8_t* pData, u32_t dataSize, u16_t* pChkSum);

/**
 * @brief   32-bit CHK calculator.
 * @details Calculates the 32-bit CHK value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pChkSum  : Variable to store the result in.
 *
 * @return  Result of CHK calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pChkSum is NULL).
 */
gos_result_t drv_crcGetChkSum32 (u8_t* pData, u32_t dataSize, u32_t* pChkSum);

/**
 * @brief   64-bit CHK calculator.
 * @details Calculates the 64-bit CHK value on the given buffer.
 *
 * @param   pData    : Pointer to the buffer.
 * @param   dataSize : Size of the buffer.
 * @param   pChkSum  : Variable to store the result in.
 *
 * @return  Result of CHK calculation.
 *
 * @retval  GOS_SUCCESS : Calculation successful.
 * @retval  GOS_ERROR   : Calculation error (pData or pChkSum is NULL).
 */
gos_result_t drv_crcGetChkSum64 (u8_t* pData, u32_t dataSize, u64_t* pChkSum);

/**
 * @brief   8-bit CHK checker.
 * @details Calculates the 8-bit CHK value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData      : Pointer to the buffer.
 * @param   dataSize   : Size of the buffer.
 * @param   expChkSum  : Expected value.
 * @param   calcChkSum : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CHK checking.
 *
 * @retval  DRV_CHKSUM_CHECK_OK    : CHK match.
 * @retval  DRV_CHKSUM_CHECK_ERROR : CHK mismatch or pData is NULL.
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum8 (u8_t* pData, u32_t dataSize, u8_t  expChkSum, u8_t*  calcChkSum);

/**
 * @brief   16-bit CHK checker.
 * @details Calculates the 16-bit CHK value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData      : Pointer to the buffer.
 * @param   dataSize   : Size of the buffer.
 * @param   expChkSum  : Expected value.
 * @param   calcChkSum : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CHK checking.
 *
 * @retval  DRV_CHKSUM_CHECK_OK    : CHK match.
 * @retval  DRV_CHKSUM_CHECK_ERROR : CHK mismatch or pData is NULL.
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum16 (u8_t* pData, u32_t dataSize, u16_t expChkSum, u16_t* calcChkSum);

/**
 * @brief   32-bit CHK checker.
 * @details Calculates the 32-bit CHK value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData      : Pointer to the buffer.
 * @param   dataSize   : Size of the buffer.
 * @param   expChkSum  : Expected value.
 * @param   calcChkSum : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CHK checking.
 *
 * @retval  DRV_CHKSUM_CHECK_OK    : CHK match.
 * @retval  DRV_CHKSUM_CHECK_ERROR : CHK mismatch or pData is NULL.
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum32 (u8_t* pData, u32_t dataSize, u32_t expChkSum, u32_t* calcChkSum);

/**
 * @brief   64-bit CHK checker.
 * @details Calculates the 64-bit CHK value on the given buffer and compares it with the
 *          expected value.
 *
 * @param   pData      : Pointer to the buffer.
 * @param   dataSize   : Size of the buffer.
 * @param   expChkSum  : Expected value.
 * @param   calcChkSum : Calculated value (can be NULL if not needed).
 *
 * @return  Result of CHK checking.
 *
 * @retval  DRV_CHKSUM_CHECK_OK    : CHK match.
 * @retval  DRV_CHKSUM_CHECK_ERROR : CHK mismatch or pData is NULL.
 */
drv_crcChkSumCheckResult_t drv_crcCheckChkSum64 (u8_t* pData, u32_t dataSize, u64_t expChkSum, u64_t* calcChkSum);

#endif
