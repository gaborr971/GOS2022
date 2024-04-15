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
//! @file       Crc.cs
//! @author     Ahmed Gazar
//! @date       2023-09-22
//! @version    1.0
//!
//! @brief      CRC-32 class.
//! @details    Implements a 32-bit CRC algorithm.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-09-22    Ahmed Gazar     Initial version created
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
using System;

namespace GOSTool
{
    /// <summary>
    /// CRC class.
    /// </summary>
    public class Crc
    {
        /// <summary>
        /// Returns the CRC32 value calculated on the given buffer.
        /// </summary>
        /// <param name="buffer"></param>
        /// <returns></returns>
        public static UInt32 GetCrc32(byte[] buffer)
        {
            UInt32 crcValue = 0xFFFFFFFF;
            UInt32 mask;

            for (int i = 0; i < buffer.Length; i++)
            {
                crcValue ^= buffer[i];

                for (int j = 8; j > 0; j--)
                {
                    mask = (UInt32)(-(crcValue & 1));
                    crcValue = (crcValue) ^ (0xEDB88320 & mask);
                }
            }

            return ~crcValue;
        }

        public static UInt32 GetCrc32_new(byte[] buffer)
        {
            UInt32 crcValue = 0xFFFFFFFF;

            for (int i = 0; i < buffer.Length; i++)
            {
                crcValue ^= buffer[i];

                for (int j = 0; j < 32; j++)
                {
                    if (0 != (0x80000000 & crcValue))
                    {
                        crcValue <<= 1;
                        crcValue ^= 0x04C11DB7u;
                    }
                    else
                    {
                        crcValue <<= 1;
                    }
                }
            }

            return crcValue;
        }
    }
}
