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
//! @file       SysmonMessages.cs
//! @author     Ahmed Gazar
//! @date       2023-09-22
//! @version    1.0
//!
//! @brief      System monitoring messages.
//! @details    Implements the messages used by the system monitoring service.
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
using System.Text;

namespace GOSTool
{
    /// <summary>
    /// System monitoring message IDs.
    /// </summary>
    public enum SysmonMessageId
    {
        GOS_SYSMON_MSG_UNKNOWN_ID = 0,
        GOS_SYSMON_MSG_PING_ID = 0x1010,
        GOS_SYSMON_MSG_PING_RESP_ID = 0x50A0,
        GOS_SYSMON_MSG_CPU_USAGE_GET_ID = 0x1023,
        GOS_SYSMON_MSG_CPU_USAGE_GET_RESP_ID = 0x5C20,
        GOS_SYSMON_MSG_TASK_GET_DATA_ID = 0x1B67,
        GOS_SYSMON_MSG_TASK_GET_DATA_RESP_ID = 0x5F8A,
        GOS_SYSMON_MSG_TASK_GET_VAR_DATA_ID = 0x12D5,
        GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_ID = 0x596B,
        GOS_SYSMON_MSG_TASK_MODIFY_STATE_ID = 0xA917,
        GOS_SYSMON_MSG_TASK_MODIFY_STATE_RESP_ID = 0x4AB2,
        GOS_SYSMON_MSG_SYSRUNTIME_GET_ID = 0x33AF,
        GOS_SYSMON_MSG_SYSRUNTIME_GET_RESP_ID = 0xD91E,
        GOS_SYSMON_MSG_SYSTIME_SET_ID = 0xBB53,
        GOS_SYSMON_MSG_SYSTIME_SET_RESP_ID = 0x174C,
        GOS_SYSMON_MSG_RESET_REQ_ID = 0x0A78,
    }

    /// <summary>
    /// System monitoring message protocol versions.
    /// </summary>
    public enum SysmonMessagePv
    {
        GOS_SYSMON_MSG_UNKNOWN_PV = 1,
        GOS_SYSMON_MSG_PING_PV = 1,
        GOS_SYSMON_MSG_PING_ACK_PV = 1,
        GOS_SYSMON_MSG_CPU_USAGE_GET_PV = 1,
        GOS_SYSMON_MSG_CPU_USAGE_GET_RESP_PV = 1,
        GOS_SYSMON_MSG_TASK_GET_DATA_PV = 1,
        GOS_SYSMON_MSG_TASK_GET_DATA_RESP_PV = 1,
        GOS_SYSMON_MSG_TASK_GET_VAR_DATA_PV = 1,
        GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_PV = 1,
        GOS_SYSMON_MSG_TASK_MODIFY_STATE_PV = 1,
        GOS_SYSMON_MSG_TASK_MODIFY_STATE_RESP_PV = 1,
        GOS_SYSMON_MSG_SYSRUNTIME_GET_PV = 1,
        GOS_SYSMON_MSG_SYSRUNTIME_GET_RESP_PV = 1,
        GOS_SYSMON_MSG_SYSTIME_SET_PV = 1,
        GOS_SYSMON_MSG_SYSTIME_SET_RESP_PV = 1,
        GOS_SYSMON_MSG_RESET_REQ_PV = 1,
    }

    /// <summary>
    /// System monitoring message results.
    /// </summary>
    public enum SysmonMessageResult
    {
        GOS_SYSMON_MSG_RES_OK = 40,
        GOS_SYSMON_MSG_RES_ERROR = 99,
        GOS_SYSMON_MSG_INV_PV = 35,
        GOS_SYSMON_MSG_INV_PAYLOAD_CRC = 28
    }

    /// <summary>
    /// Task modification types.
    /// </summary>
    public enum SysmonTaskModifyType
    {
        GOS_SYSMON_TASK_MOD_TYPE_SUSPEND = 12,
        GOS_SYSMON_TASK_MOD_TYPE_RESUME = 34,
        GOS_SYSMON_TASK_MOD_TYPE_DELETE = 49,
        GOS_SYSMON_TASK_MOD_TYPE_BLOCK = 52,
        GOS_SYSMON_TASK_MOD_TYPE_UNBLOCK = 63,
        GOS_SYSMON_TASK_MOD_TYPE_WAKEUP = 74
    }

    /// <summary>
    /// Ping message class.
    /// </summary>
    public class SysmonPingMessage
    {
        public SysmonMessageResult MessageResult { get; set; }

        public const UInt16 ExpectedSize = 1;

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            MessageResult = (SysmonMessageResult)(bytes[0]);
        }

        /// <summary>
        /// Returns the message as a byte array.
        /// </summary>
        /// <returns></returns>
        public byte[] GetBytes()
        {
            return new byte[]
            {
                (byte)(MessageResult)
            };
        }
    }

    /// <summary>
    /// CPU usage message class.
    /// </summary>
    public class CpuUsageMessage
    {
        public SysmonMessageResult MessageResult { get; set; }

        public UInt16 CpuUsage { get; set; }

        public float CpuUsagePercent { get; set; }

        public const UInt16 ExpectedSize = 3;

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length >= ExpectedSize)
            {
                MessageResult = (SysmonMessageResult)(bytes[0]);
                CpuUsage = (UInt16)((bytes[2] << 8) + bytes[1]);
                CpuUsagePercent = (float)CpuUsage / 100f;
            }
        }

        /// <summary>
        /// Returns the message as a byte array.
        /// </summary>
        /// <returns></returns>
        /*public byte[] GetBytes()
        {
            return new byte[]
            {
                (byte)(MessageResult),
                (byte)((int)CpuUsage << 8),
                (byte)(CpuUsage),
            };
        }*/
    }

    /// <summary>
    /// Task data get message class.
    /// </summary>
    public class TaskDataGetMessage
    {
        public UInt16 TaskIndex { get; set; }

        public const UInt16 ExpectedSize = 2;

        /// <summary>
        /// Returns the message as a byte array.
        /// </summary>
        /// <returns></returns>
        public byte[] GetBytes()
        {
            return new byte[]
            {
                (byte)(TaskIndex),
                (byte)((int)TaskIndex >> 8),
            };
        }
    }

    /// <summary>
    /// Task data message class.
    /// </summary>
    public class TaskDataMessage
    {
        public SysmonMessageResult MessageResult { get; set; }
        public TaskData TaskData { get; set; }

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes != null)
            {
                MessageResult = (SysmonMessageResult)(bytes[0]);
                byte[] taskDataByes = new byte[bytes.Length - 1];
                Array.Copy(bytes, 1, taskDataByes, 0, bytes.Length - 1);
                TaskData = new TaskData();
                TaskData.GetFromBytes(taskDataByes);
            }
            else
            {
                MessageResult = SysmonMessageResult.GOS_SYSMON_MSG_RES_ERROR;
            }
        }
    }

    /// <summary>
    /// Task variable data message class.
    /// </summary>
    public class TaskVariableDataMessage
    {
        public SysmonMessageResult MessageResult { get; set; }
        public TaskVariableData TaskVariableData { get; set; }

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes != null)
            {
                MessageResult = (SysmonMessageResult)(bytes[0]);
                byte[] taskDataByes = new byte[bytes.Length - 1];
                Array.Copy(bytes, 1, taskDataByes, 0, bytes.Length - 1);
                TaskVariableData = new TaskVariableData();
                TaskVariableData.GetFromBytes(taskDataByes);
            }
        }
    }

    /// <summary>
    /// Runtime class.
    /// </summary>
    public class RunTime
    {
        public UInt16 Microseconds { get; set; }
        public UInt16 Milliseconds { get; set; }
        public byte Seconds { get; set; }
        public byte Minutes { get; set; }
        public byte Hours { get; set; }
        public UInt16 Days { get; set; }

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            Microseconds = (UInt16)((bytes[1] << 8) + bytes[0]);
            Milliseconds = (UInt16)((bytes[3] << 8) + bytes[2]);
            Seconds = bytes[4];
            Minutes = bytes[5];
            Hours = bytes[6];
            Days = (UInt16)((bytes[8] << 8) + bytes[7]);
        }
    }

    /// <summary>
    /// Time class.
    /// </summary>
    public class Time
    {
        public UInt16 Milliseconds { get; set; }
        public byte Seconds { get; set; }
        public byte Minutes { get; set; }
        public byte Hours { get; set; }
        public UInt16 Days { get; set; }
        public byte Months { get; set; }
        public UInt16 Years { get; set; }

        public const int ExpectedSize = 2 + 1 + 1 + 1 + 2 + 1 + 2;

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            Milliseconds = (UInt16)((bytes[1] << 8) + bytes[0]);
            Seconds = bytes[2];
            Minutes = bytes[3];
            Hours = bytes[4];
            Days = (UInt16)((bytes[6] << 8) + bytes[5]);
            Months = bytes[7];
            Years = (UInt16)((bytes[9] << 8) + bytes[8]);
        }

        public byte[] GetBytes()
        {
            return new byte[]
            {
                (byte)(Milliseconds),
                (byte)(Milliseconds >> 8),
                Seconds,
                Minutes,
                Hours,
                (byte)(Days),
                (byte)(Days >> 8),
                Months,
                (byte)(Years),
                (byte)(Years >> 8)
            };
        }
    }

    /// <summary>
    /// Task data class.
    /// </summary>
    public class TaskData
    {
        public byte TaskState { get; set; }
        public byte TaskPriority { get; set; }
        public byte TaskOriginalPriority { get; set; }
        public UInt16 TaskPrivileges { get; set; }
        public string TaskName { get; set; }
        public UInt16 TaskId { get; set; }
        public UInt32 TaskCsCounter { get; set; }
        public UInt16 TaskStackSize { get; set; }
        public RunTime TaskRuntime { get; set; }
        public UInt16 TaskCpuUsageLimit { get; set; }
        public UInt16 TaskCpuUsageMax { get; set; }
        public UInt16 TaskCpuUsage { get; set; }
        public UInt16 TaskStackMaxUsage { get; set; }

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            TaskState = (bytes[0]);
            TaskPriority = (bytes[1]);
            TaskOriginalPriority = (bytes[2]);
            TaskPrivileges = (UInt16)((bytes[4] << 8) + bytes[3]);

            byte[] taskNameBytes = new byte[32];
            Array.Copy(bytes, 5, taskNameBytes, 0, 32);

            TaskName = Encoding.ASCII.GetString(taskNameBytes).Substring(0, Encoding.ASCII.GetString(taskNameBytes).IndexOf("\0"));
            TaskId = (UInt16)((bytes[38] << 8) + bytes[37]);
            TaskCsCounter = (UInt32)((bytes[42] << 24) + (bytes[41] << 16) + (bytes[40] << 8) + bytes[39]);
            TaskStackSize = (UInt16)((bytes[44] << 8) + bytes[43]);

            byte[] taskRunTimeBytes = new byte[9];
            Array.Copy(bytes, 45, taskRunTimeBytes, 0, 9);

            TaskRuntime = new RunTime();
            TaskRuntime.GetFromBytes(taskRunTimeBytes);

            TaskCpuUsageLimit = (UInt16)((bytes[55] << 8) + bytes[54]);
            TaskCpuUsageMax = (UInt16)((bytes[57] << 8) + bytes[56]);
            TaskCpuUsage = (UInt16)((bytes[59] << 8) + bytes[58]);
            TaskStackMaxUsage = (UInt16)((bytes[61] << 8) + bytes[60]);
        }
    }

    /// <summary>
    /// Task variable data class.
    /// </summary>
    public class TaskVariableData
    {
        public byte TaskState { get; set; }
        public byte TaskPriority { get; set; }
        public UInt32 TaskCsCounter { get; set; }
        public RunTime TaskRuntime { get; set; }
        public UInt16 TaskCpuUsageMax { get; set; }
        public UInt16 TaskCpuUsage { get; set; }
        public UInt16 TaskStackMaxUsage { get; set; }

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length == 21)
            {
                TaskState = (bytes[0]);
                TaskPriority = (bytes[1]);
                TaskCsCounter = (UInt32)((bytes[5] << 24) + (bytes[4] << 16) + (bytes[3] << 8) + bytes[2]);

                byte[] taskRunTimeBytes = new byte[9];
                Array.Copy(bytes, 6, taskRunTimeBytes, 0, 9);

                TaskRuntime = new RunTime();
                TaskRuntime.GetFromBytes(taskRunTimeBytes);

                TaskCpuUsageMax = (UInt16)((bytes[16] << 8) + bytes[15]);
                TaskCpuUsage = (UInt16)((bytes[18] << 8) + bytes[17]);
                TaskStackMaxUsage = (UInt16)((bytes[20] << 8) + bytes[19]);
            }
        }
    }

    /// <summary>
    /// Task modify message class.
    /// </summary>
    public class TaskModifyMessage
    {
        public UInt16 TaskIndex { get; set; }
        public SysmonTaskModifyType TaskModifyType { get; set; }
        public UInt32 Param { get; set; }

        public const UInt16 ExpectedSize = 2 + 1 + 4;

        /// <summary>
        /// Returns the message as a byte array.
        /// </summary>
        /// <returns></returns>
        public byte[] GetBytes()
        {
            return new byte[]
            {
                (byte)(TaskIndex),
                (byte)((int)TaskIndex >> 8),
                (byte)(TaskModifyType),
                (byte)(Param >> 24),
                (byte)(Param >> 16),
                (byte)(Param >> 8),
                (byte)(Param)
            };
        }
    }

    /// <summary>
    /// Task modify message result class.
    /// </summary>
    public class TaskModifyResultMessage
    {
        public SysmonMessageResult MessageResult { get; set; }

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes != null)
            {
                MessageResult = (SysmonMessageResult)(bytes[0]);
            }
        }
    }

    public class SysRuntimeMessage
    {
        public SysmonMessageResult MessageResult { get; set; }

        public RunTime SysRunTime { get; set; }

        public const UInt16 ExpectedSize = 10;

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length >= ExpectedSize)
            {
                MessageResult = (SysmonMessageResult)(bytes[0]);
                byte[] sysruntimeBytes = new byte[9];
                Array.Copy(bytes, 1, sysruntimeBytes, 0, 9);

                SysRunTime = new RunTime();
                SysRunTime.GetFromBytes(sysruntimeBytes);
            }
        }
    }

    public class SysTimeMessage
    {
        public SysmonMessageResult MessageResult { get; set; }
        public Time SystemTime { get; set; } = new Time();

        public const UInt16 ExpectedSize = 10;

        /// <summary>
        /// Gets the class member values from the byte array.
        /// </summary>
        /// <param name="bytes"></param>
        public void GetFromBytes(byte[] bytes)
        {
            MessageResult = (SysmonMessageResult)(bytes[0]);
        }

        /// <summary>
        /// Returns the message as a byte array.
        /// </summary>
        /// <returns></returns>
        public byte[] GetBytes()
        {
            return SystemTime.GetBytes();
        }
    }
}
