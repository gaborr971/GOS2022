using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GOSTool
{
    public static class SysmonFunctions
    {
        private static SemaphoreSlim sysmonSemaphore = new SemaphoreSlim(1, 1);
        public enum PingResult
        {
            OK,
            NOK
        }

        public static PingResult PingDevice()
        {           
            SysmonPingMessage sysmonPingMessage = new SysmonPingMessage();
            GcpMessageHeader messageHeader = new GcpMessageHeader();
            PingResult result = PingResult.OK;
            byte[] recvBuf;

            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_PING_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_PING_PV;
            messageHeader.PayloadSize = 0;

            sysmonSemaphore.Wait();
            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) != true)
            {
                result = PingResult.NOK;
            }
            else
            {
                if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 250) != true)
                {
                    result = PingResult.NOK;
                }
                else
                {
                    sysmonPingMessage.GetFromBytes(recvBuf);

                    if (sysmonPingMessage.MessageResult != SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
                    {
                        result = PingResult.NOK;
                    }
                }
            }

            Thread.Sleep(10);
            sysmonSemaphore.Release();

            return result;
        }

        public static List<TaskData> GetAllTaskData()
        {
            TaskDataMessage taskDataMessage = new TaskDataMessage();
            List<TaskData> taskDatas = new List<TaskData>();
            TaskDataGetMessage taskDataGetMessage = new TaskDataGetMessage();            
            GcpMessageHeader messageHeader = new GcpMessageHeader();
            byte[] recvBuf;

            taskDataGetMessage.TaskIndex = 0xFFFF;
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_GET_DATA_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_GET_DATA_PV;
            messageHeader.PayloadSize = TaskDataGetMessage.ExpectedSize;

            sysmonSemaphore.Wait();
            GCP.TransmitMessage(0, messageHeader, taskDataGetMessage.GetBytes());           

            while (true)
            {
                if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 500) == true)
                {
                    taskDataMessage.GetFromBytes(recvBuf);

                    if (taskDataMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
                    {
                        taskDatas.Add(taskDataMessage.TaskData);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }

            Thread.Sleep(10);
            sysmonSemaphore.Release();
            return taskDatas;
        }

        public static BootloaderData GetSoftwareInfo()
        {
            BootloaderData bootloaderData = new BootloaderData();
            bld_com_data_resp_msg_t respMsg = new bld_com_data_resp_msg_t();
            byte[] recvBuf;
            GcpMessageHeader messageHeader = new GcpMessageHeader();

            messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_DATA_REQ_ID;
            messageHeader.ProtocolVersion = 1;
            messageHeader.PayloadSize = 0;

            sysmonSemaphore.Wait();
            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) == true)
            {
                if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) == true)
                {
                    respMsg.GetFromBytes(recvBuf);

                    // Driver info.
                    bootloaderData.DriverInfo = new VersionInfo();
                    bootloaderData.DriverInfo.Author = respMsg.drvData.author;
                    bootloaderData.DriverInfo.Major = respMsg.drvData.major;
                    bootloaderData.DriverInfo.Minor = respMsg.drvData.minor;
                    bootloaderData.DriverInfo.Build = respMsg.drvData.build;
                    bootloaderData.DriverInfo.Date = DateTime.Parse(respMsg.drvData.date.Years.ToString("D4") + "-" + respMsg.drvData.date.Months.ToString("D2") + "-" + respMsg.drvData.date.Days.ToString("D2"));
                    bootloaderData.DriverInfo.Description = respMsg.drvData.description;
                    bootloaderData.DriverInfo.Name = respMsg.drvData.name;

                    // Bootloader info.
                    bootloaderData.BootloaderInfo = new VersionInfo();
                    bootloaderData.BootloaderInfo.Author = respMsg.bldData.bldVersion.author;
                    bootloaderData.BootloaderInfo.Major = respMsg.bldData.bldVersion.major;
                    bootloaderData.BootloaderInfo.Minor = respMsg.bldData.bldVersion.minor;
                    bootloaderData.BootloaderInfo.Build = respMsg.bldData.bldVersion.build;
                    try
                    {
                        bootloaderData.BootloaderInfo.Date = DateTime.Parse(respMsg.bldData.bldVersion.date.Years.ToString("D4") + "-" + respMsg.bldData.bldVersion.date.Months.ToString("D2") + "-" + respMsg.bldData.bldVersion.date.Days.ToString("D2"));
                    }
                    catch
                    {
                        bootloaderData.BootloaderInfo.Date = new DateTime();
                    }
                    bootloaderData.BootloaderInfo.Description = respMsg.bldData.bldVersion.description;
                    bootloaderData.BootloaderInfo.Name = respMsg.bldData.bldVersion.name;
                    bootloaderData.Crc = (int)respMsg.bldData.bldCrc;
                    bootloaderData.Size = (int)respMsg.bldData.bldSize;
                    bootloaderData.StartAddress = (int)respMsg.bldData.bldStartAddress;
                    bootloaderData.BootUpdateMode = respMsg.bldData.bootUpdateMode;

                    // Application info.
                    bootloaderData.ApplicationData = new ApplicationData();
                    bootloaderData.ApplicationData.Version = new VersionInfo();
                    bootloaderData.ApplicationData.Version.Author = respMsg.bldData.appData.appVersion.author;
                    bootloaderData.ApplicationData.Version.Major = respMsg.bldData.appData.appVersion.major;
                    bootloaderData.ApplicationData.Version.Minor = respMsg.bldData.appData.appVersion.minor;
                    bootloaderData.ApplicationData.Version.Build = respMsg.bldData.appData.appVersion.build;
                    try
                    {
                        bootloaderData.ApplicationData.Version.Date = DateTime.Parse(respMsg.bldData.appData.appVersion.date.Years.ToString("D4") + "-" + respMsg.bldData.appData.appVersion.date.Months.ToString("D2") + "-" + respMsg.bldData.appData.appVersion.date.Days.ToString("D2"));
                    }
                    catch
                    {
                        bootloaderData.ApplicationData.Version.Date = new DateTime();
                    }
                    bootloaderData.ApplicationData.Version.Description = respMsg.bldData.appData.appVersion.description;
                    bootloaderData.ApplicationData.Version.Name = respMsg.bldData.appData.appVersion.name;
                    bootloaderData.ApplicationData.Crc = (int)respMsg.bldData.appData.appCrc;
                    bootloaderData.ApplicationData.Size = (int)respMsg.bldData.appData.appSize;
                    bootloaderData.ApplicationData.StartAddress = (int)respMsg.bldData.appData.appStartAddress;
                }
                else
                {
                    // Error.
                }
            }
            else
            {
                // Error.
            }
            Thread.Sleep(10);
            sysmonSemaphore.Release();

            return bootloaderData;
        }

        public static void SendResetRequest()
        {
            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_RESET_REQ_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_RESET_REQ_PV;
            messageHeader.PayloadSize = 0;

            sysmonSemaphore.Wait();
            GCP.TransmitMessage(0, messageHeader, new byte[] { });
            Thread.Sleep(10);
            sysmonSemaphore.Release();
        }

        public static float GetCpuLoad()
        {
            byte[] recvBuf;
            float cpuLoad = -1;
            CpuUsageMessage cpuUsageMessage = new CpuUsageMessage();
            GcpMessageHeader messageHeader = new GcpMessageHeader();

            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_CPU_USAGE_GET_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_CPU_USAGE_GET_PV;
            messageHeader.PayloadSize = 0;

            sysmonSemaphore.Wait();
            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) == true)
            {
                if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 250) == true)
                {
                    cpuUsageMessage.GetFromBytes(recvBuf);

                    if (cpuUsageMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
                    {
                        cpuLoad = cpuUsageMessage.CpuUsagePercent;
                    }
                }
            }
            Thread.Sleep(10);
            sysmonSemaphore.Release();

            return cpuLoad;            
        }

        public static string GetSystemRuntime()
        {
            byte[] recvBuf;
            string runtime = "0000:00:00:00";
            SysRuntimeMessage sysRuntimeMessage = new SysRuntimeMessage();
            GcpMessageHeader messageHeader = new GcpMessageHeader();

            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_SYSRUNTIME_GET_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_SYSRUNTIME_GET_PV;
            messageHeader.PayloadSize = 0;

            sysmonSemaphore.Wait();
            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) == true)
            {
                if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 250) == true)
                {
                    sysRuntimeMessage.GetFromBytes(recvBuf);

                    if (sysRuntimeMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
                    {
                        runtime = sysRuntimeMessage.SysRunTime.Days.ToString("D2") + ":" +
                            sysRuntimeMessage.SysRunTime.Hours.ToString("D2") + ":" +
                            sysRuntimeMessage.SysRunTime.Minutes.ToString("D2") + ":" +
                            sysRuntimeMessage.SysRunTime.Seconds.ToString("D2") + "." +
                            sysRuntimeMessage.SysRunTime.Milliseconds.ToString("D3");
                    }
                }
            }
            Thread.Sleep(10);
            sysmonSemaphore.Release();

            return runtime;
        }

        public static TaskVariableData GetTaskVariableData(int taskIndex)
        {
            TaskVariableDataMessage taskDataMessage = new TaskVariableDataMessage();
            TaskDataGetMessage taskDataGetMessage = new TaskDataGetMessage();
            TaskVariableData taskVariableData = new TaskVariableData();
            GcpMessageHeader messageHeader = new GcpMessageHeader();
            byte[] recvBuf;

            taskDataGetMessage.TaskIndex = (UInt16)taskIndex;
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_GET_VAR_DATA_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_GET_VAR_DATA_PV;
            messageHeader.PayloadSize = TaskDataGetMessage.ExpectedSize;

            sysmonSemaphore.Wait();
            
            if (GCP.TransmitMessage(0, messageHeader, taskDataGetMessage.GetBytes()) == true)
            {
                if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 500) == true)
                {
                    taskDataMessage.GetFromBytes(recvBuf);

                    if (taskDataMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
                    {
                        taskVariableData = taskDataMessage.TaskVariableData;
                    }
                }
            }

            Thread.Sleep(10);
            sysmonSemaphore.Release();

            return taskVariableData;
        }
    }
}
