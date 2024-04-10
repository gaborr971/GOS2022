using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool.AppConfig
{
    public partial class TaskConfigUserControl : UserControl
    {
        public TaskConfig TaskConfiguration = new TaskConfig();

        public event EventHandler saveClicked;

        public TaskConfigUserControl()
        {
            InitializeComponent();
        }

        public void SetConfiguration(TaskConfig taskConfig)
        {
            TaskConfiguration = taskConfig;
            taskTypeCB.Items.Clear();
            taskTypeCB.Items.AddRange(Enum.GetValues(typeof(TaskType)).OfType<object>().Select(o => o.ToString()).ToArray());
            taskTypeCB.SelectedItem = TaskConfiguration.TaskType.ToString();
            taskNameTb.Text = TaskConfiguration.TaskName;
            cpuLoadNUD.Value = TaskConfiguration.TaskMaxCpu / 100;
            stackSizeNUD.Value = TaskConfiguration.TaskStackSize;
            priorityNUD.Value = TaskConfiguration.TaskPriority;
            taskPrivilegeSelector1.SetPrivileges(TaskConfiguration.TaskPrivileges);
            Invalidate();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            TaskConfiguration.TaskMaxCpu = (int)cpuLoadNUD.Value * 100;
            TaskConfiguration.TaskName = taskNameTb.Text;
            TaskConfiguration.TaskPriority = (int)priorityNUD.Value;
            TaskConfiguration.TaskType = (TaskType)Enum.Parse(typeof(TaskType), taskTypeCB.SelectedItem.ToString());
            TaskConfiguration.TaskStackSize = (int)stackSizeNUD.Value;
            TaskConfiguration.TaskPrivileges = taskPrivilegeSelector1.GetPrivileges();

            saveClicked?.Invoke(this, null);
        }
    }
}
