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
//! @file       GCP.cs
//! @author     Ahmed Gazar
//! @date       2023-09-22
//! @version    1.0
//!
//! @brief      GOS General Communication Protocol handler.
//! @details    Implements the GCP frame and message layers.
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
using System.Collections.Generic;
using System.Threading;

namespace GOSTool
{
    /// <summary>
    /// GCP message header class.
    /// </summary>
    public class GcpMessageHeader
    {
        public UInt16 ProtocolVersion { get; set; }
        public UInt16 MessageId { get; set; }
        public UInt16 PayloadSize { get; set; }
        public UInt32 PayloadCrc { get; set; }

        public const UInt16 ExpectedSize = 10;

        /// <summary>
        /// Fills out the class members from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length == 10)
            {
                ProtocolVersion = (UInt16)((bytes[1] << 8) + bytes[0]);
                MessageId = (UInt16)((bytes[3] << 8) + bytes[2]);
                PayloadSize = (UInt16)((bytes[5] << 8) + bytes[4]);
                PayloadCrc = (UInt16)((bytes[9] << 24) + (bytes[8] << 16) + (bytes[7] << 8) + bytes[6]);
            }
        }

        /// <summary>
        /// Returns the header bytes as an array based on the
        /// class member values.
        /// </summary>
        /// <returns></returns>
        public byte[] GetHeaderBytes()
        {
            return new byte[]
            {
                (byte)(ProtocolVersion), (byte)(ProtocolVersion >> 8),
                (byte)(MessageId), (byte)(MessageId >> 8),
                (byte)(PayloadSize), (byte)(PayloadSize >> 8),
                (byte)(PayloadCrc), (byte)(PayloadCrc >> 8), (byte)(PayloadCrc >> 16), (byte)(PayloadCrc >> 24),
            };
        }
    }

    /// <summary>
    /// GCP frame header class.
    /// </summary>
    class GcpFrameHeader
    {
        public UInt16 ProtocolVersion { get; set; }
        public UInt16 SessionId { get; set; }
        public UInt16 DataSize { get; set; }
        public UInt16 FrameSize { get; set; }
        public UInt32 DataCrc { get; set; }
        public UInt32 HeaderCrc { get; set; }

        public const UInt16 ExpectedFrameSize = 48;

        public const UInt16 ExpectedProtocolVersion = ((1 << 8) + 0);

        public const UInt16 ExpectedSize = 16;

        public static UInt32 SyncPattern = 0xAFBC8C55;

        /// <summary>
        /// Fills out the class members from the given byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            ProtocolVersion = (UInt16)((bytes[1] << 8) + bytes[0]);
            SessionId = (UInt16)((bytes[3] << 8) + bytes[2]);
            DataSize = (UInt16)((bytes[5] << 8) + bytes[4]);
            FrameSize = (UInt16)((bytes[7] << 8) + bytes[6]);
            DataCrc = (UInt32)((bytes[11] << 24) + (bytes[10] << 16) + (bytes[9] << 8) + bytes[8]);
            HeaderCrc = (UInt32)((bytes[15] << 24) + (bytes[14] << 16) + (bytes[13] << 8) + bytes[12]);
        }

        /// <summary>
        /// Returns the header bytes as an array based on the
        /// class member values.
        /// </summary>
        /// <returns></returns>
        public byte[] GetHeaderBytes()
        {
            List<byte> headerBytes = new List<byte>();
            byte[] headerBytesWithoutHeaderCrc = new byte[]
            {
                (byte)(ProtocolVersion), (byte)(ProtocolVersion >> 8),
                (byte)(SessionId), (byte)(SessionId >> 8),
                (byte)(DataSize), (byte)(DataSize >> 8),
                (byte)(FrameSize), (byte)(FrameSize >> 8),
                (byte)(DataCrc), (byte)(DataCrc >> 8), (byte)(DataCrc >> 16), (byte)(DataCrc >> 24),
            };
            HeaderCrc = Crc.GetCrc32(headerBytesWithoutHeaderCrc);
            headerBytes.AddRange(headerBytesWithoutHeaderCrc);
            headerBytes.AddRange(new byte[] { (byte)(HeaderCrc), (byte)(HeaderCrc >> 8), (byte)(HeaderCrc >> 16), (byte)(HeaderCrc >> 24) });
            return headerBytes.ToArray();
        }
    }

    /// <summary>
    /// GCP static class.
    /// </summary>
    public static class GCP
    {
        /// <summary>
        /// Virtually unused.
        /// </summary>
        private static UInt16 sessionId;

        /// <summary>
        /// Transmits the given message defined by the header and payload
        /// on the given channel.
        /// </summary>
        /// <param name="channelNumber"></param>
        /// <param name="messageHeader"></param>
        /// <param name="payload"></param>
        /// <returns></returns>
        public static bool TransmitMessage(int channelNumber, GcpMessageHeader messageHeader, byte[] payload)
        {
            // TODO
            Uart.ClearRxBuffer();

            if (messageHeader != null && ((payload != null) || (payload == null && messageHeader.PayloadSize == 0u)))
            {
                messageHeader.PayloadCrc = Crc.GetCrc32(payload);

                if (TransmitFrames(channelNumber, messageHeader.GetHeaderBytes(), (UInt16)messageHeader.GetHeaderBytes().Length) == true &&
                    TransmitFrames(channelNumber, payload, messageHeader.PayloadSize) == true)
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Receives a message (header + payload) on the given channel.
        /// </summary>
        /// <param name="channelNumber"></param>
        /// <param name="messageHeader"></param>
        /// <param name="payload"></param>
        /// <returns></returns>
        public static bool ReceiveMessage(int channelNumber, out GcpMessageHeader messageHeader, out byte[] payload, int timeout = 250)
        {
            byte[] messageHeaderBytes;

            if (ReceiveFrames(channelNumber, out messageHeaderBytes, GcpMessageHeader.ExpectedSize, timeout) == true)
            {
                messageHeader = new GcpMessageHeader();
                messageHeader.GetFromBytes(messageHeaderBytes);

                if (ReceiveFrames(channelNumber, out payload, messageHeader.PayloadSize, timeout) == true &&
                   messageHeader.PayloadCrc == Crc.GetCrc32(payload))
                {
                    return true;
                }
                else
                {
                    _ = 2;
                }
            }
            else
            {
                _ = 2;
            }

            messageHeader = null;
            payload = null;
            return false;
        }

        /// <summary>
        /// Transmits the frames of the given message on the given channel.
        /// </summary>
        /// <param name="channelNumber"></param>
        /// <param name="messageBytes"></param>
        /// <param name="messageSize"></param>
        /// <returns></returns>
        private static bool TransmitFrames(int channelNumber, byte[] messageBytes, UInt16 messageSize)
        {
            GcpFrameHeader frameHeader = new GcpFrameHeader();
            UInt16 frameNumber = 0;
            UInt16 frameCounter = 0;

            frameHeader.ProtocolVersion = GcpFrameHeader.ExpectedProtocolVersion;
            frameHeader.FrameSize = GcpFrameHeader.ExpectedFrameSize;
            frameHeader.DataSize = messageSize;
            frameHeader.SessionId = sessionId++;
            frameHeader.DataCrc = Crc.GetCrc32(messageBytes);

            frameNumber = (UInt16)(messageSize / GcpFrameHeader.ExpectedFrameSize + ((messageSize % GcpFrameHeader.ExpectedFrameSize != 0) ? 1 : 0));

            if (Uart.Send(frameHeader.GetHeaderBytes(), GcpFrameHeader.ExpectedSize) == true)
            {
                Thread.Sleep(10);

                // Transmit frames.
                for (frameCounter = 0; frameCounter < frameNumber; frameCounter++)
                {
                    byte[] frame = new byte[GcpFrameHeader.ExpectedFrameSize];

                    if (frameCounter * GcpFrameHeader.ExpectedFrameSize + GcpFrameHeader.ExpectedFrameSize > messageBytes.Length)
                    {
                        if (messageBytes.Length > GcpFrameHeader.ExpectedFrameSize)
                        {
                            Array.Copy(messageBytes, frameCounter * GcpFrameHeader.ExpectedFrameSize, frame, 0, messageBytes.Length - frameCounter * GcpFrameHeader.ExpectedFrameSize);
                        }
                        else
                        {
                            Array.Copy(messageBytes, frameCounter * GcpFrameHeader.ExpectedFrameSize, frame, 0, messageBytes.Length);
                        }
                    }
                    else
                    {
                        Array.Copy(messageBytes, frameCounter * GcpFrameHeader.ExpectedFrameSize, frame, 0, GcpFrameHeader.ExpectedFrameSize);
                    }

                    // Transmit frame.
                    if (Uart.Send(frame, GcpFrameHeader.ExpectedFrameSize) != true)
                    {
                        break;
                    }
                    Thread.Sleep(1);
                }

                if (frameCounter == frameNumber)
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Receives frames on the given channel.
        /// </summary>
        /// <param name="channelNumber"></param>
        /// <param name="target"></param>
        /// <param name="targetSize"></param>
        /// <returns></returns>
        private static bool ReceiveFrames(int channelNumber, out byte[] target, UInt16 targetSize, int timeout = 10000)
        {
            GcpFrameHeader frameHeader = new GcpFrameHeader();
            UInt16 frameNumber;
            UInt16 frameCounter;
            byte[] frameHeaderBytes;

            if (Uart.Receive(out frameHeaderBytes, GcpFrameHeader.ExpectedSize, timeout) == true)
            {
                frameHeader.GetFromBytes(frameHeaderBytes);
                byte[] frameHeaderForCrcCheck = new byte[GcpFrameHeader.ExpectedSize - 4];
                Array.Copy(frameHeaderBytes, frameHeaderForCrcCheck, GcpFrameHeader.ExpectedSize - 4);

                if (frameHeader.HeaderCrc == Crc.GetCrc32(frameHeaderForCrcCheck) &&
                    frameHeader.ProtocolVersion == GcpFrameHeader.ExpectedProtocolVersion)
                {
                    frameNumber = (UInt16)(frameHeader.DataSize / frameHeader.FrameSize + ((frameHeader.DataSize % frameHeader.FrameSize != 0) ? 1 : 0));
                    target = new byte[frameHeader.DataSize];

                    // Receive frames.
                    for (frameCounter = 0; frameCounter < frameNumber; frameCounter++)
                    {
                        byte[] frameBuffer;
                        if (Uart.Receive(out frameBuffer, frameHeader.FrameSize, timeout) == true)
                        {
                            if ((frameCounter * frameHeader.FrameSize + frameHeader.FrameSize) > target.Length)
                            {
                                if (target.Length > frameHeader.FrameSize)
                                {
                                    Array.Copy(frameBuffer, 0, target, frameCounter * frameHeader.FrameSize, target.Length - frameCounter * frameHeader.FrameSize);
                                }
                                else
                                {
                                    Array.Copy(frameBuffer, 0, target, frameCounter * frameHeader.FrameSize, target.Length);
                                }
                            }
                            else
                            {
                                Array.Copy(frameBuffer, 0, target, frameCounter * frameHeader.FrameSize, frameHeader.FrameSize);
                            }

                        }
                        else
                        {
                            break;
                        }
                    }

                    // Check data integrity.
                    if (frameCounter == frameNumber &&
                        frameHeader.DataCrc == Crc.GetCrc32(target))
                    {
                        return true;
                    }
                }
                else
                {
                    _ = 2;
                }
            }
            else
            {
                _ = 2;
            }

            target = null;
            return false;
        }
    }
}
