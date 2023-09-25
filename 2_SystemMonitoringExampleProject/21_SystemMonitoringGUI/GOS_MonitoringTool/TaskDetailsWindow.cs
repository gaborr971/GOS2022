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

namespace GOS_MonitoringTool
{
    public partial class TaskDetailsWindow : Form
    {
        private List<TaskData> taskDatas;
        private int taskIndex;

        public TaskDetailsWindow(object sender, ref List<TaskData> taskData, int index)
        {
            InitializeComponent();
            taskDatas = taskData;
            taskIndex = index;
            Text = taskData[index].TaskName + " details";
            (sender as MainWindow).TaskDataListUpdated += TaskDetailsWindow_TaskDataListUpdated;
        }

        private void TaskDetailsWindow_TaskDataListUpdated(object sender, EventArgs e)
        {
            float cpuUtil = (float)taskDatas[taskIndex].TaskCpuUsage / 100f;
            float stackUtil = 100f * (float)taskDatas[taskIndex].TaskStackMaxUsage / (float)taskDatas[taskIndex].TaskStackSize;
            cpuLoadGraph1.AddNewMeasurement(cpuUtil);
            cpuLoadGraph2.AddNewMeasurement(stackUtil);

            List<ListViewItem> listViewItems = new List<ListViewItem>();

            listViewItems.Add(new ListViewItem(new string[] { "Task ID", string.Format("0x{0:X4}", taskDatas[taskIndex].TaskId) }));
            listViewItems.Add(new ListViewItem(new string[] { "Task name", taskDatas[taskIndex].TaskName }));
            listViewItems.Add(new ListViewItem(new string[] { "Task priority", taskDatas[taskIndex].TaskPriority.ToString() }));
            listViewItems.Add(new ListViewItem(new string[] { "Task original priority", taskDatas[taskIndex].TaskOriginalPriority.ToString() }));
            listViewItems.Add(new ListViewItem(new string[] { "Task state", MainWindow.ConvertTaskState(taskDatas[taskIndex].TaskState) }));
            listViewItems.Add(new ListViewItem(new string[] { "Task runtime", MainWindow.ConvertTaskRuntime(taskDatas[taskIndex].TaskRuntime) }));
            listViewItems.Add(new ListViewItem(new string[] { "Task CS counter", taskDatas[taskIndex].TaskCsCounter.ToString() }));
            listViewItems.Add(new ListViewItem(new string[] { "Task CPU usage", (taskDatas[taskIndex].TaskCpuUsage / 100f).ToString() + "%" }));
            listViewItems.Add(new ListViewItem(new string[] { "Task CPU limit", (taskDatas[taskIndex].TaskCpuUsageLimit / 100f).ToString() + "%" }));
            listViewItems.Add(new ListViewItem(new string[] { "Task max stack usage", string.Format("0x{0:X4}", taskDatas[taskIndex].TaskStackMaxUsage) }));

            Helpers.UpdateListViewWithItems_ThreadSafe(this, listView1, listViewItems);

            Helpers.SetLabelText_ThreadSafe(this, cpuUtilLabel, cpuUtil.ToString() + "%");
            Helpers.SetLabelText_ThreadSafe(this, stackUtilLabel, stackUtil.ToString() + "%");
        }

        private void TaskDetailsWindow_Load(object sender, EventArgs e)
        {

        }
    }
}
