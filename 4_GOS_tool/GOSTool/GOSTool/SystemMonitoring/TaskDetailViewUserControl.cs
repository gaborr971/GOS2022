using System;
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

            await Task.Run(() =>
            {
                while (isMonitoringOn)
                {
                    var taskData = SysmonFunctions.GetTaskVariableData(TaskIndex);

                    if (!(taskData is null))
                    {
                        var stackUtil = 100f * (float)taskData.TaskStackMaxUsage / _taskData.TaskStackSize;
                        var percentage = taskData.TaskCpuUsage / 100f;
                        stackLoadGraph.AddNewMeasurement(stackUtil);
                        cpuLoadGraph.AddNewMeasurement(percentage);

                        Helper.SetLabelText_ThreadSafe(this, stackUtilLabel, String.Format("{0:0.##}", stackUtil) + "%");
                        Helper.SetLabelText_ThreadSafe(this, cpuUtilLabel, String.Format("{0:0.##}", percentage) + "%");
                        Helper.SetLabelText_ThreadSafe(this, avgCpuUtil, String.Format("{0:0.##}", cpuLoadGraph.GetAvg()) + "%");
                    }

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
