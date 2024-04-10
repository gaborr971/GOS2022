using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class TaskPrivilegeSelector : UserControl
    {
        private List<TaskPrivilege> taskPrivileges = new List<TaskPrivilege>();

        public TaskPrivilegeSelector()
        {
            InitializeComponent();

            taskPrivileges.Add(new TaskPrivilege() { Name = "Task state manipulation" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Task priority change" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Trace" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Signal invoking" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Reserved 0" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Reserved 1" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Reserved 2" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "Reserved 3" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 0" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 1" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 2" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 3" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 4" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 5" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 6" });
            taskPrivileges.Add(new TaskPrivilege() { Name = "User-defined 7" });
        }

        private void TaskPrivilegeSelector_Load(object sender, EventArgs e)
        {
            int width = this.Width;
            int height = this.Height;

            for (int i = 0; i < taskPrivileges.Count; i++)
            {
                taskPrivileges[i].Rectangle.X = (int)(i * (float)width / (float)taskPrivileges.Count);
                taskPrivileges[i].Rectangle.Y = (int)(0);
                taskPrivileges[i].Rectangle.Width = (int)((float)width / (float)taskPrivileges.Count);
                taskPrivileges[i].Rectangle.Height = taskPrivileges[i].Rectangle.Width;
            }

            Invalidate();
        }

        private void TaskPrivilegeSelector_MouseClick(object sender, MouseEventArgs e)
        {
            for(int i=0;i<taskPrivileges.Count;i++)
            {
                if (taskPrivileges[i].Rectangle.Contains(e.Location))
                {
                    taskPrivileges[i].Selected = !taskPrivileges[i].Selected;
                }
            }

            Invalidate();
        }

        private void TaskPrivilegeSelector_Paint(object sender, PaintEventArgs e)
        {
            Pen blackPen = new Pen(Color.Black);
            SolidBrush greenBrush = new SolidBrush(Color.Green);
            SolidBrush redBrush = new SolidBrush(Color.Red);

            Font drawFont = new Font("Consolas", 8);
            SolidBrush drawBrush = new SolidBrush(Color.Black);
            StringFormat drawFormat = new StringFormat();
            drawFormat.FormatFlags = StringFormatFlags.DirectionVertical;

            foreach (TaskPrivilege taskPrivilege in taskPrivileges)
            {
                if (taskPrivilege.Selected)
                {
                    e.Graphics.FillRectangle(greenBrush, taskPrivilege.Rectangle);
                }
                else
                {
                    e.Graphics.FillRectangle(redBrush, taskPrivilege.Rectangle);
                }

                e.Graphics.DrawRectangle(blackPen, taskPrivilege.Rectangle);

                e.Graphics.DrawString(taskPrivilege.Name, drawFont, drawBrush, taskPrivilege.Rectangle.X /*+ (int)(0.1f * taskPrivilege.Rectangle.X)*/,
                    taskPrivilege.Rectangle.Y + taskPrivilege.Rectangle.Height + (int)(0.1f * taskPrivilege.Rectangle.Y), drawFormat);
            }
        }

        public int GetPrivileges()
        {
            int priv = 0b0000000000000000;

            for (int i = 0; i < taskPrivileges.Count; i++)
            {
                if (taskPrivileges[i].Selected)
                    priv |= 1 << (taskPrivileges.Count - i - 1);
            }

            return priv;
        }

        public void SetPrivileges(int priv)
        {
            for (int i = 0; i < taskPrivileges.Count; i++)
            {
                if ((priv >> (taskPrivileges.Count - i - 1) & 0b1) == 0b1)
                {
                    taskPrivileges[i].Selected = true;
                }
                else
                {
                    taskPrivileges[i].Selected = false;
                }
            }
            Invalidate();
        }

        public string GetPrivilegeText ()
        {
            int priv = 0b0000000000000000;
            bool isTaskManipulate = false, isTaskPrioChange = false, isTracing = false, isSignalInvoking = false;
            bool isFirst = true;
            string privString = "";

            for (int i = 0; i < taskPrivileges.Count; i++)
            {
                if (taskPrivileges[i].Selected)
                    priv |= 1 << (taskPrivileges.Count - i - 1);
            }

            if (priv == 0xFFFF)
                return "GOS_TASK_PRIVILEGE_SUPERVISOR";
            if (priv == 0xFF00)
                return "GOS_TASK_PRIVILEGE_KERNEL";
            if (priv == 0x00FF)
                return "GOS_TASK_PRIVILEGE_USER";
            if (priv == 0x20FF)
                return "GOS_TASK_PRIVILEGED_USER";

            if ((priv & (1 << 15)) == (1 << 15))
            {
                isTaskManipulate = true;
            }

            if ((priv & (1 << 14)) == (1 << 14))
            {
                isTaskPrioChange = true;
            }

            if ((priv & (1 << 13)) == (1 << 13))
            {
                isTracing = true;
            }

            if ((priv & (1 << 12)) == (1 << 12))
            {
                isSignalInvoking = true;
            }

            if (isTaskManipulate)
            {
                privString += "GOS_PRIV_TASK_MANIPULATE";
                priv &= ~(1 << 15);
                isFirst = false;
            }

            if (isTaskPrioChange)
            {
                if (!isFirst)
                    privString += " | ";
                privString += "GOS_PRIV_TASK_PRIO_CHANGE";
                priv &= ~(1 << 14);
                isFirst = false;
            }

            if (isTracing)
            {
                if (!isFirst)
                    privString += " | ";
                privString += "GOS_PRIV_TRACE";
                priv &= ~(1 << 13);
                isFirst = false;
            }

            if (isSignalInvoking)
            {
                if (!isFirst)
                    privString += " | ";
                privString += "GOS_PRIV_SIGNALING";
                priv &= ~(1 << 12);
                isFirst = false;
            }

            if (!isFirst)
                privString += " | ";
            privString += "0x" + priv.ToString("X");

            return privString;
        }
    }

    class TaskPrivilege
    {
        public bool Selected { get; set; } = false;
        public string Name { get; set; } = "";
        public Rectangle Rectangle = new Rectangle();
    }
}
