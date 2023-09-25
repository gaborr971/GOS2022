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
//! @file       Uart.cs
//! @author     Ahmed Gazar
//! @date       2023-09-22
//! @version    1.0
//!
//! @brief      UART driver.
//! @details    UART/Serial driver implementation for low-level communication.
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
using System.IO.Ports;
using System.Diagnostics;

namespace GOS_MonitoringTool
{
    /// <summary>
    /// Static class for UART communication.
    /// </summary>
    public static class Uart
    {
        private static SerialPort serialPort;
        private static List<byte> rxBuffer = new List<byte>();
        private static Stopwatch sw = new Stopwatch();

        /// <summary>
        /// Port initialized property.
        /// </summary>
        public static bool IsInitialized { get; private set; } = false;

        /// <summary>
        /// Initializes the given serial port with the given baud rate.
        /// </summary>
        /// <param name="port"></param>
        /// <param name="baud"></param>
        /// <returns></returns>
        public static bool Init (string port, int baud)
        {
            try
            {
                if (serialPort != null && serialPort.IsOpen)
                {
                    serialPort.Close();
                }
                else
                {
                    // Port is null or closed.
                }

                serialPort = new SerialPort(port, baud);
                serialPort.DataReceived += SerialPort_DataReceived;
                serialPort.WriteTimeout = 1000;
                serialPort.ReadTimeout = 1000;
                serialPort.Open();

                serialPort.DiscardInBuffer();
                serialPort.DiscardOutBuffer();
                rxBuffer.Clear();

                IsInitialized = true;

                return true;
            }
            catch
            {
                if (serialPort != null)
                {
                    serialPort.Close();
                }
                else
                {
                    // Serial port is null.
                }
                IsInitialized = false;
                return false;
            }
        }

        /// <summary>
        /// Data received event callback.
        /// Places the received bytes in the RX buffer.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (serialPort.IsOpen)
            {
                try
                {
                    int receivedBytes = 0;
                    byte[] rxTempBuffer;
                    do
                    {
                        receivedBytes = serialPort.BytesToRead;
                        rxTempBuffer = new byte[receivedBytes];
                        serialPort.Read(rxTempBuffer, 0, receivedBytes);
                        rxBuffer.AddRange(rxTempBuffer);
                    }
                    while (serialPort.BytesToRead > 0);
                }
                catch
                {

                }
            }
        }

        /// <summary>
        /// De-initializes the serial port.
        /// </summary>
        public static void DeInit ()
        {
            if (serialPort != null)
            {
                serialPort.Close();
            }
           
            rxBuffer.Clear();

            IsInitialized = false;
        }

        /// <summary>
        /// Returns the names of the available serial ports.
        /// </summary>
        /// <returns></returns>
        public static string[] GetPortNames ()
        {
            return SerialPort.GetPortNames();
        }

        /// <summary>
        /// Sends out the given number of bytes from the given buffer via
        /// serial port.
        /// </summary>
        /// <param name="data"></param>
        /// <param name="size"></param>
        /// <returns></returns>
        public static bool Send (byte[] data, UInt16 size)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                for (int i = 0; i< size; i++)
                {
                    try
                    {
                        byte[] sendByte = new byte[] { data[i] };
                        serialPort.Write(sendByte, 0, 1);
                    }
                    catch
                    {
                        return false;
                    }
                }

                return true;
            }

            return false;
        }

        /// <summary>
        /// Receives at least the given number of bytes from the serial port
        /// with a 500 ms timeout.
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="size"></param>
        /// <returns></returns>
        public static bool Receive (out byte[] buffer, UInt16 size)
        {
            buffer = new byte[size];
            sw.Restart();

            if (serialPort != null && serialPort.IsOpen)
            {
                while (rxBuffer.Count < size && sw.ElapsedMilliseconds < 2000) ;

                try
                {
                    if (sw.ElapsedMilliseconds < 2000)
                    {
                        Array.Copy(rxBuffer.ToArray(), buffer, size);
                        rxBuffer.RemoveRange(0, size);
                        sw.Stop();
                        return true;
                    }
                    else
                    {
                        sw.Stop();
                    }
                }
                catch
                {
                    sw.Stop();
                }

            }

            return false;
        }

        /// <summary>
        /// Clears the RX buffer.
        /// </summary>
        public static void ClearRxBuffer()
        {
            rxBuffer.Clear();
        }
    }
}
