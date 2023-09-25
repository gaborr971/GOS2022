using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOS_MonitoringTool
{
    public partial class MainWindow : Form
    {
        private string _port;

        private System.Windows.Forms.Timer monitoringTimer;

        private DateTime monitoringTime = new DateTime();
        
        private bool monitoring = false;

        public List<int> monitoringIndeces = new List<int>();

        private bool refershAllTaskData = false;

        private List<TaskData> taskDatas = new List<TaskData>();

        ContextMenu taskCm = new ContextMenu();

        public event EventHandler TaskDataListUpdated;

        private object lockObject = new object();

        SemaphoreSlim semaphore = new SemaphoreSlim(1, 1);

        public static SemaphoreSlim gcpSemaphore = new SemaphoreSlim(1, 1);

        public MainWindow()
        {
            InitializeComponent();
        }

        private async void Form1_Load(object sender, EventArgs e)
        {
            checkBox1.BackColor = Color.Gray;
            checkBox1.Text = "Link inactive";
            currentCpuUtil.Text = "N/A %";

            taskCm.MenuItems.Add("Show details", new EventHandler(ShowTaskDetails));
            taskCm.MenuItems.Add("Suspend", new EventHandler(SuspendTask));
            taskCm.MenuItems.Add("Resume", new EventHandler(ResumeTask));
            taskCm.MenuItems.Add("Delete", new EventHandler(DeleteTask));


            if (await DetectDevice() == true)
            {
                await Task.Run(() =>
                {
                    BackgroundTask();
                });
            }
        }

        private void ShowTaskDetails(object sender, EventArgs e)
        {
            try
            {
                //var itemIndex = (((sender as MenuItem).Parent as ContextMenu).SourceControl as ListView).FocusedItem.Index;
                var itemIndex = focusedIdx;
                TaskDetailsWindow taskDetailsWindow = new TaskDetailsWindow(this, ref taskDatas, itemIndex);
                taskDetailsWindow.Show();
                lock (lockObject)
                {
                    monitoringIndeces.Add(itemIndex);
                }
                taskDetailsWindow.FormClosed += ((s, args) =>
                {
                    lock (lockObject)
                    {
                        monitoringIndeces.Remove(itemIndex);
                    }
                });
            }
            catch
            {

            }
        }

        private async void SuspendTask(object sender, EventArgs e)
        {
            try
            {
                var itemIndex = focusedIdx;

                await Task.Run(() =>
                {
                    semaphore.Wait();

                    ModifyTask(itemIndex, SysmonTaskModifyType.GOS_SYSMON_TASK_MOD_TYPE_SUSPEND);

                    semaphore.Release();
                });
            }
            catch
            {

            }
        }

        private async void ResumeTask(object sender, EventArgs e)
        {
            try
            {
                var itemIndex = focusedIdx;

                await Task.Run(() =>
                {
                    semaphore.Wait();

                    ModifyTask(itemIndex, SysmonTaskModifyType.GOS_SYSMON_TASK_MOD_TYPE_RESUME);

                    semaphore.Release();
                });
            }
            catch
            {

            }
        }

        private async void DeleteTask(object sender, EventArgs e)
        {
            try
            {
                var itemIndex = focusedIdx;

                await Task.Run(() =>
                {
                    semaphore.Wait();

                    ModifyTask(itemIndex, SysmonTaskModifyType.GOS_SYSMON_TASK_MOD_TYPE_DELETE);

                    semaphore.Release();
                });
            }
            catch
            {

            }
        }

        private async Task<bool> DetectDevice ()
        {
            string[] ports = Uart.GetPortNames();
            foreach (string port in ports)
            {
                bool result = false;

                progressLabel.Text = "Searching on " + port + "...";

                await Task.Run(() =>
                {
                    Uart.Init(port, 115200);
                    result = PingDevice();

                    if (result == false)
                    {
                        Uart.DeInit();
                    }
                });

                if (result == true)
                {
                    progressLabel.Text = "Device found on: " + port;
                    _port = port;
                    return true;
                }
            }

            progressLabel.Text = "Device not found.";
            return false;
        }

        private bool PingDevice ()
        {
            SysmonPingMessage sysmonPingMessage = new SysmonPingMessage();

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_PING_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_PING_PV;
            messageHeader.PayloadSize = 0;

            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) != true)
                return false;

            byte[] recvBuf;

            if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                return false;
            sysmonPingMessage.GetFromBytes(recvBuf);

            if (sysmonPingMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private string GetSystemRuntime ()
        {
            byte[] recvBuf;
            SysRuntimeMessage sysRuntimeMessage = new SysRuntimeMessage();

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_SYSRUNTIME_GET_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_SYSRUNTIME_GET_PV;
            messageHeader.PayloadSize = 0;

            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) != true)
                return "0000:00:00:00";
   
            if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                return "0000:00:00:00";

            sysRuntimeMessage.GetFromBytes(recvBuf);

            if (sysRuntimeMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                return sysRuntimeMessage.SysRunTime.Days.ToString("D4") + ":" +
                    sysRuntimeMessage.SysRunTime.Hours.ToString("D2") + ":" +
                    sysRuntimeMessage.SysRunTime.Minutes.ToString("D2") + ":" +
                    sysRuntimeMessage.SysRunTime.Seconds.ToString("D2");
            }
            else
            {
                return "0000:00:00:00";
            }
        }

        private void SetSysTime()
        {
            byte[] recvBuf;
            SysTimeMessage sysTimeMessage = new SysTimeMessage();
            sysTimeMessage.SystemTime.Years = (UInt16)DateTime.Now.Year;
            sysTimeMessage.SystemTime.Months = (byte)DateTime.Now.Month;
            sysTimeMessage.SystemTime.Days = (UInt16)DateTime.Now.Day;
            sysTimeMessage.SystemTime.Hours = (byte)DateTime.Now.Hour;
            sysTimeMessage.SystemTime.Minutes = (byte)DateTime.Now.Minute;
            sysTimeMessage.SystemTime.Seconds = (byte)DateTime.Now.Second;

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_SYSTIME_SET_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_SYSTIME_SET_PV;
            messageHeader.PayloadSize = SysTimeMessage.ExpectedSize;

            if (GCP.TransmitMessage(0, messageHeader, sysTimeMessage.GetBytes()) != true)
                return;

            if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                return;

            sysTimeMessage.GetFromBytes(recvBuf);

            if (sysTimeMessage.MessageResult != SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                MessageBox.Show("System time synchronization failed!");
            }
        }

        private float GetCpuUsage ()
        {
            byte[] recvBuf;
            CpuUsageMessage cpuUsageMessage = new CpuUsageMessage();

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_CPU_USAGE_GET_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_CPU_USAGE_GET_PV;
            messageHeader.PayloadSize = 0;

            if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) != true)
                return -1;

            if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                return -1;
            cpuUsageMessage.GetFromBytes(recvBuf);


            if (cpuUsageMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                return cpuUsageMessage.CpuUsagePercent;
            }
            else
            {
                return -1;
            }
        }

        private void ModifyTask(int taskIndex, SysmonTaskModifyType modifyType)
        {
            byte[] recvBuf;

            TaskModifyMessage taskModifyMessage = new TaskModifyMessage();
            taskModifyMessage.TaskIndex = (UInt16)taskIndex;
            taskModifyMessage.TaskModifyType = modifyType;
            TaskModifyResultMessage taskModifyResultMessage = new TaskModifyResultMessage();

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_MODIFY_STATE_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_MODIFY_STATE_PV;
            messageHeader.PayloadSize = TaskModifyMessage.ExpectedSize;

            GCP.TransmitMessage(0, messageHeader, taskModifyMessage.GetBytes());

            GCP.ReceiveMessage(0, out messageHeader, out recvBuf);
            taskModifyResultMessage.GetFromBytes(recvBuf);

            if (taskModifyResultMessage.MessageResult != SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                MessageBox.Show("Task modification failed! Error: " + taskModifyResultMessage.MessageResult.ToString());
            }
        }

        private void GetAllTaskVariableData ()
        {
            TaskVariableDataMessage taskDataMessage = new TaskVariableDataMessage();
            TaskDataGetMessage taskDataGetMessage = new TaskDataGetMessage();
            taskDataGetMessage.TaskIndex = 0xFFFF;
            int i = 0;

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_GET_VAR_DATA_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_GET_VAR_DATA_PV;
            messageHeader.PayloadSize = TaskDataGetMessage.ExpectedSize;

            GCP.TransmitMessage(0, messageHeader, taskDataGetMessage.GetBytes());

            byte[] recvBuf;

            while (true)
            {
                GCP.ReceiveMessage(0, out messageHeader, out recvBuf);

                taskDataMessage.GetFromBytes(recvBuf);

                if (taskDataMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
                {
                    if (i < taskDatas.Count)
                    {
                        taskDatas[i].TaskState = taskDataMessage.TaskVariableData.TaskState;
                        taskDatas[i].TaskStackMaxUsage = taskDataMessage.TaskVariableData.TaskStackMaxUsage;
                        taskDatas[i].TaskRuntime = taskDataMessage.TaskVariableData.TaskRuntime;
                        taskDatas[i].TaskPriority = taskDataMessage.TaskVariableData.TaskPriority;
                        taskDatas[i].TaskCsCounter = taskDataMessage.TaskVariableData.TaskCsCounter;
                        taskDatas[i].TaskCpuUsageMax = taskDataMessage.TaskVariableData.TaskCpuUsageMax;
                        taskDatas[i].TaskCpuUsage = taskDataMessage.TaskVariableData.TaskCpuUsage;
                    }

                    i++;
                }
                else
                {
                    break;
                }
            }
        }

        public void GetAllTaskData ()
        {
            TaskDataMessage taskDataMessage = new TaskDataMessage();
            taskDatas.Clear();

            TaskDataGetMessage taskDataGetMessage = new TaskDataGetMessage();
            taskDataGetMessage.TaskIndex = 0xFFFF;

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_GET_DATA_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_GET_DATA_PV;
            messageHeader.PayloadSize = TaskDataGetMessage.ExpectedSize;

            GCP.TransmitMessage(0, messageHeader, taskDataGetMessage.GetBytes());

            byte[] recvBuf;

            while (true)
            {
                GCP.ReceiveMessage(0, out messageHeader, out recvBuf);

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
        }

        /*private async Task<bool> GetTaskData ()
        {
            TaskDataMessage taskDataMessage = new TaskDataMessage();

            await Task.Run(() =>
            {
                TaskDataGetMessage taskDataGetMessage = new TaskDataGetMessage();
                taskDataGetMessage.TaskIndex = 0;
                
                GcpMessageHeader messageHeader = new GcpMessageHeader();
                messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_GET_DATA_ID;
                messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_GET_DATA_PV;
                messageHeader.PayloadSize = TaskDataGetMessage.ExpectedSize;

                GCP.TransmitMessage(0, messageHeader, taskDataGetMessage.GetBytes());

                byte[] recvBuf;

                GCP.ReceiveMessage(0, out messageHeader, out recvBuf);
                
                taskDataMessage.GetFromBytes(recvBuf);

            });

            if (taskDataMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                string[] row = {
                        string.Format("0x{0:X4}", taskDataMessage.TaskData.TaskId),
                        taskDataMessage.TaskData.TaskName,
                        string.Format("0x{0:X4}", taskDataMessage.TaskData.TaskStackSize),
                        taskDataMessage.TaskData.TaskPriority.ToString(),
                        taskDataMessage.TaskData.TaskCsCounter.ToString(),
                        string.Format("0x{0:X4}", taskDataMessage.TaskData.TaskStackMaxUsage),
                        ((float)taskDataMessage.TaskData.TaskCpuUsage / 100f).ToString(),
                        Convert.ToString(taskDataMessage.TaskData.TaskPrivileges, 2).PadLeft(16, '0')
                    };

                var listViewItem = new ListViewItem(row);
                

                if (taskListView.Items.Count == 0)
                {
                    taskListView.Items.Add(listViewItem);
                }
                else
                {
                    taskListView.Items[0] = listViewItem;
                }

                return true;
            }
            else
            {
                return false;
            }
        }*/

        private void MonitoringTimer_Tick(object sender, EventArgs e)
        {
            TimeSpan elapsedTime = (DateTime.Now - monitoringTime);
            monitoringTimeLabel.Text = elapsedTime.Days.ToString("D2") + ":" +
                elapsedTime.Hours.ToString("D2") + ":" + elapsedTime.Minutes.ToString("D2") + ":" +
                elapsedTime.Seconds.ToString("D2") + ":" + elapsedTime.Milliseconds.ToString("D3");
        }
        
        private void GetTaskVariableData(int taskIndex)
        {
            TaskVariableDataMessage taskDataMessage = new TaskVariableDataMessage();
            TaskDataGetMessage taskDataGetMessage = new TaskDataGetMessage();
            taskDataGetMessage.TaskIndex = (UInt16)taskIndex;

            GcpMessageHeader messageHeader = new GcpMessageHeader();
            messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_TASK_GET_VAR_DATA_ID;
            messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_TASK_GET_VAR_DATA_PV;
            messageHeader.PayloadSize = TaskDataGetMessage.ExpectedSize;

            GCP.TransmitMessage(0, messageHeader, taskDataGetMessage.GetBytes());

            byte[] recvBuf;

            GCP.ReceiveMessage(0, out messageHeader, out recvBuf);

            taskDataMessage.GetFromBytes(recvBuf);

            if (taskDataMessage.MessageResult == SysmonMessageResult.GOS_SYSMON_MSG_RES_OK)
            {
                if (taskIndex < taskDatas.Count)
                {
                    taskDatas[taskIndex].TaskState = taskDataMessage.TaskVariableData.TaskState;
                    taskDatas[taskIndex].TaskStackMaxUsage = taskDataMessage.TaskVariableData.TaskStackMaxUsage;
                    taskDatas[taskIndex].TaskRuntime = taskDataMessage.TaskVariableData.TaskRuntime;
                    taskDatas[taskIndex].TaskPriority = taskDataMessage.TaskVariableData.TaskPriority;
                    taskDatas[taskIndex].TaskCsCounter = taskDataMessage.TaskVariableData.TaskCsCounter;
                    taskDatas[taskIndex].TaskCpuUsageMax = taskDataMessage.TaskVariableData.TaskCpuUsageMax;
                    taskDatas[taskIndex].TaskCpuUsage = taskDataMessage.TaskVariableData.TaskCpuUsage;

                    TaskDataListUpdated?.Invoke(this, null);
                }

            }
        }

        private void BackgroundTask ()
        {
            // Preparation.
            // Get task list.
            Helpers.SetLabelText_ThreadSafe(this, progressLabel, "Getting task list...");
            GetAllTaskData();

            List<ListViewItem> listViewItems = new List<ListViewItem>();

            foreach (var taskData in taskDatas)
            {
                string[] row = {
                            string.Format("0x{0:X4}", taskData.TaskId),
                            taskData.TaskName,
                            string.Format("0x{0:X4}", taskData.TaskStackSize),
                            taskData.TaskPriority.ToString(),
                            taskData.TaskCsCounter.ToString(),
                            string.Format("0x{0:X4}", taskData.TaskStackMaxUsage),
                            ((float)taskData.TaskCpuUsage / 100f).ToString() + "%",
                            ((float)taskData.TaskCpuUsageLimit / 100f).ToString() + "%",
                            Convert.ToString(taskData.TaskPrivileges, 2).PadLeft(16, '0'),
                            ConvertTaskState(taskData.TaskState),
                            ConvertTaskRuntime(taskData.TaskRuntime)
                        };

                var listViewItem = new ListViewItem(row);
                listViewItems.Add(listViewItem);
            }

            Helpers.UpdateListViewWithItems_ThreadSafe(this, taskListView, listViewItems);
            Helpers.ResizeListView_ThreadSafe(this, taskListView);
            Helpers.SetLabelText_ThreadSafe(this, progressLabel, "Task list received.");

            Uart.Init(_port, 115200);

            object syncObject = new object();
            float percentage = 0;
            DateTime pingTime = DateTime.Now;
            DateTime cpuGetTime = DateTime.Now;
            DateTime sysRunTimeGetTime = DateTime.Now;
            DateTime allTaskRefreshTime = DateTime.Now;
            DateTime selectedTaskRefreshTime = DateTime.Now;
            
            // Synchronize system time.
            SetSysTime();

            // Ping task for link activity check.
            Task.Run(() =>
            {
                while(true)
                {
                    if (monitoring && (DateTime.Now - pingTime).TotalMilliseconds > 100)
                    {
                        if (semaphore.Wait(25) == true)
                        {
                            if (PingDevice())
                            {
                                Helpers.SetCheckBoxParameters_ThreadSafe(this, checkBox1, "Link active", Color.Green, true);
                            }
                            else
                            {
                                Helpers.SetCheckBoxParameters_ThreadSafe(this, checkBox1, "Link inactive", Color.Gray, false);
                            }
                            semaphore.Release();
                            pingTime = DateTime.Now;
                        }
                        else
                        {
                            Thread.Sleep(10);
                        }
                    }
                    else
                    {
                        if (!monitoring)
                        {
                            Helpers.SetCheckBoxParameters_ThreadSafe(this, checkBox1, "Link inactive", Color.Gray, false);
                        }
                        Thread.Sleep(20);
                    }
                }
            });

            // Overall system runtime monitoring task.
            Task.Run(() =>
            {
                while (true)
                {
                    if (monitoring && (DateTime.Now - sysRunTimeGetTime).TotalMilliseconds > 500)
                    {
                        if (semaphore.Wait(25) == true)
                        {
                            // Refresh system runtime.
                            Helpers.SetLabelText_ThreadSafe(this, sysRuntimeLabel, GetSystemRuntime());

                            semaphore.Release();
                            sysRunTimeGetTime = DateTime.Now;
                        }
                        else
                        {
                            Thread.Sleep(10);
                        }
                    }
                    else
                    {
                        Thread.Sleep(250);
                    }
                }
            });

            // Overall CPU load monitoring task.
            Task.Run(() =>
            {
                while (true)
                {
                    if (monitoring && (DateTime.Now - cpuGetTime).TotalMilliseconds > 200)
                    {
                        if (semaphore.Wait(25) == true)
                        {
                            // Refresh CPU load.
                            percentage = GetCpuUsage();

                            if (percentage != -1)
                            {
                                Helpers.SetLabelText_ThreadSafe(this, currentCpuUtil, String.Format("{0:0.##}", percentage) + "%");
                                cpuLoadGraph1.AddNewMeasurement(percentage);
                            }
                            else
                            {

                            }
                            semaphore.Release();
                            cpuGetTime = DateTime.Now;
                        }
                        else
                        {
                            Thread.Sleep(10);
                        }
                    }
                    else
                    {
                        Thread.Sleep(50);
                    }
                }
            });

            // All task data list update task.
            Task.Run(() =>
            {
                while (true)
                {
                    if (monitoring && refershAllTaskData && (DateTime.Now - allTaskRefreshTime).TotalMilliseconds > 1000)
                    {
                        if (semaphore.Wait(25) == true)
                        {
                            // Refresh all task data if required.
                            GetAllTaskVariableData();

                            listViewItems.Clear();
                            foreach (var taskData in taskDatas)
                            {
                                string[] row = {
                            string.Format("0x{0:X4}", taskData.TaskId),
                            taskData.TaskName,
                            string.Format("0x{0:X4}", taskData.TaskStackSize),
                            taskData.TaskPriority.ToString(),
                            taskData.TaskCsCounter.ToString(),
                            string.Format("0x{0:X4}", taskData.TaskStackMaxUsage),
                            ((float)taskData.TaskCpuUsage / 100f).ToString() + "%",
                            ((float)taskData.TaskCpuUsageLimit / 100f).ToString() + "%",
                            Convert.ToString(taskData.TaskPrivileges, 2).PadLeft(16, '0'),
                            ConvertTaskState(taskData.TaskState),
                            ConvertTaskRuntime(taskData.TaskRuntime)
                            };

                                var listViewItem = new ListViewItem(row);
                                listViewItems.Add(listViewItem);
                            }
                            Helpers.UpdateListViewWithItems_ThreadSafe(this, taskListView, listViewItems);
                            TaskDataListUpdated?.Invoke(this, null);
                            Thread.Sleep(250);
                            semaphore.Release();
                            allTaskRefreshTime = DateTime.Now;
                        }
                        else
                        {
                            Thread.Sleep(10);
                        }
                    }
                    else
                    {
                        Thread.Sleep(200);
                    }
                }
            });

            // Individual task data list update task.
            Task.Run(() =>
            {
                while (true)
                {
                    if ((DateTime.Now - selectedTaskRefreshTime).TotalMilliseconds > 200)
                    {
                        if (semaphore.Wait(5) == true)
                        {
                            List<int> monIndCopy;
                            lock (lockObject)
                            {
                                monIndCopy = new List<int>(monitoringIndeces);
                            }

                            foreach (int taskIdx in monIndCopy)
                            {
                                GetTaskVariableData(taskIdx);
                            }

                            semaphore.Release();
                            selectedTaskRefreshTime = DateTime.Now;
                        }
                        else
                        {
                            Thread.Sleep(10);
                        }
                    }
                    else
                    {
                        Thread.Sleep(50);
                    }
                }
            });
        }

        private void monitoringButton_Click(object sender, EventArgs e)
        {
            monitoring = !monitoring;

            if (monitoring)
            {
                // Start monitoring.
                monitoringTime = DateTime.Now;
                monitoringTimer = new System.Windows.Forms.Timer();
                monitoringTimer.Interval = 100;
                monitoringTimer.Tick += MonitoringTimer_Tick;
                monitoringTimer.Start();
            }
            else
            {
                monitoringTimer.Stop();
                Helpers.SetLabelText_ThreadSafe(this, progressLabel, "Monitoring stopped.");
            }

        }

        public static string ConvertTaskState (byte taskState)
        {
            switch(taskState)
            {
                case 10: return "ready";
                case 22: return "sleeping";
                case 25: return "blocked";
                case 5: return "suspended";
                case 13: return "zombie";
                default: return "invalid";
            }
        }

        public static string ConvertTaskRuntime (RunTime runtime)
        {
            if (runtime is null)
            {
                return "";
            }
            else
            {
                return runtime.Days.ToString("D3") + ":" +
                    runtime.Hours.ToString("D2") + ":" +
                    runtime.Minutes.ToString("D2") + ":" +
                    runtime.Seconds.ToString("D2") + "." +
                    runtime.Milliseconds.ToString("D3");
            }
        }

        int focusedIdx = 0;

        private void taskListView_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                try
                {
                    var focusedItem = taskListView.FocusedItem;
                    focusedIdx = taskListView.FocusedItem.Index;
                    if (focusedItem != null && focusedItem.Bounds.Contains(e.Location))
                    {
                        taskCm.Show(taskListView, focusedItem.Position);
                    }
                }
                catch (Exception ex)
                {

                }
            }
        }

        private void refreshTaskListCB_CheckedChanged(object sender, EventArgs e)
        {
            refershAllTaskData = refreshTaskListCB.Checked;
        }

        private async void resetButton_Click(object sender, EventArgs e)
        {
            try
            {
                await Task.Run(() =>
                {
                    semaphore.Wait();

                    GcpMessageHeader messageHeader = new GcpMessageHeader();
                    messageHeader.MessageId = (UInt16)SysmonMessageId.GOS_SYSMON_MSG_RESET_REQ_ID;
                    messageHeader.ProtocolVersion = (UInt16)SysmonMessagePv.GOS_SYSMON_MSG_RESET_REQ_PV;
                    messageHeader.PayloadSize = 0;

                    GCP.TransmitMessage(0, messageHeader, new byte[] { });

                    semaphore.Release();
                });
            }
            catch
            {

            }
        }

        private async void timeSyncButton_Click(object sender, EventArgs e)
        {
            try
            {
                await Task.Run(() =>
                {
                    semaphore.Wait();

                    SetSysTime();

                    semaphore.Release();
                });
            }
            catch
            {

            }
        }
    }
}
