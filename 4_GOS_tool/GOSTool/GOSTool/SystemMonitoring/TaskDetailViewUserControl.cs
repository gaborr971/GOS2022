using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class TaskDetailViewUserControl : UserControl
    {
        private TaskData _taskData = new TaskData();

        private bool isMonitoringOn = false;

        public int TaskIndex { get; set; }

        public TaskDetailViewUserControl()
        {
            InitializeComponent();
        }

        public void SetTaskData(TaskData taskData)
        {
            _taskData = taskData;
        }

        public async void Activate()
        {
            isMonitoringOn = true;

            /*if (isMonitoringOn)
            {
                monitoringButton.Text = "Stop";
                monitoringButton.BackColor = Color.Red;

                // Start monitoring.
                monitoringTime = DateTime.Now;
                monitoringTimer = new System.Windows.Forms.Timer();
                monitoringTimer.Interval = 100;
                monitoringTimer.Tick += MonitoringTimer_Tick;
                monitoringTimer.Start();
            }
            else
            {
                monitoringButton.Text = "Monitoring";
                monitoringButton.BackColor = Color.Green;
                monitoringTimer.Stop();
            }*/

            await Task.Run(() =>
            {
                while (isMonitoringOn)
                {
                    //var percentage = SysmonFunctions.GetCpuLoad();
                    //cpuLoadGraph.AddNewMeasurement(percentage);

                    var taskData = SysmonFunctions.GetTaskVariableData(TaskIndex);
                    var stackUtil = 100f * (float)taskData.TaskStackMaxUsage / _taskData.TaskStackSize;
                    var percentage = taskData.TaskCpuUsage / 100f;
                    stackLoadGraph.AddNewMeasurement(stackUtil);
                    cpuLoadGraph.AddNewMeasurement(percentage);

                    Helper.SetLabelText_ThreadSafe(this, stackUtilLabel, String.Format("{0:0.##}", stackUtil) + "%");
                    Helper.SetLabelText_ThreadSafe(this, cpuUtilLabel, String.Format("{0:0.##}", percentage) + "%");
                    Helper.SetLabelText_ThreadSafe(this, avgCpuUtil, String.Format("{0:0.##}", cpuLoadGraph.GetAvg()) + "%");

                    Thread.Sleep(500);
                }
            });
        }

        public void Deactivate()
        {
            isMonitoringOn = false;
        }
    }
}
