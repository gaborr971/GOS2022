using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool
{
    public static class Helper<T>
    {
        public static T GetVariable(byte[] buffer, ref int index)
        {
            UInt32 returnValue = 0;
            int numberOfBytes = 0;

            if (buffer != null)
            {
                if (typeof(T) == typeof(UInt32))
                {
                    numberOfBytes = 4;
                }
                else if (typeof(T) == typeof(UInt16))
                {
                    numberOfBytes = 2;
                }
                else if (typeof(T) == typeof(byte))
                {
                    numberOfBytes = 1;
                }

                for (int i = 0; i < numberOfBytes; i++)
                {
                    returnValue += (UInt32)(buffer[index++] << (i * 8));
                }
            }

            return (T)Convert.ChangeType(returnValue, typeof(T));
        }

        public static byte[] GetBytes(T variable)
        {
            if (typeof(T) == typeof(UInt32))
            {
                UInt32 var = (UInt32)Convert.ChangeType(variable, typeof(UInt32));
                return new byte[]
                {
                    (byte)(var),
                    (byte)((int)var >> 8),
                    (byte)((int)var >> 16),
                    (byte)((int)var >> 24)
                };
            }
            else if (typeof(T) == typeof(UInt16))
            {
                UInt16 var = (UInt16)Convert.ChangeType(variable, typeof(UInt16));
                return new byte[]
                {
                    (byte)(var),
                    (byte)((int)var >> 8)
                };
            }
            else if (typeof(T) == typeof(byte))
            {
                byte var = (byte)Convert.ChangeType(variable, typeof(byte));
                return new byte[]
                {
                    (byte)(var),
                };
            }

            return null;
        }
    }

    public static class Helper
    {
        public static void CopyFilesRecursively(string sourcePath, string targetPath)
        {
            //Now Create all of the directories
            foreach (string dirPath in Directory.GetDirectories(sourcePath, "*", SearchOption.AllDirectories))
            {
                Directory.CreateDirectory(dirPath.Replace(sourcePath, targetPath));
            }

            //Copy all the files & Replaces any files with the same name
            foreach (string newPath in Directory.GetFiles(sourcePath, "*.*", SearchOption.AllDirectories))
            {
                File.Copy(newPath, newPath.Replace(sourcePath, targetPath), true);
            }
        }

        public static void ExpandTreeView (TreeNodeCollection nodes, int expandDepth)
        {
            if (expandDepth == 0)
            {
                return;
            }
            else
            {
                for (int i = 0; i < nodes.Count; i++)
                {
                    nodes[i].Expand();
                    ExpandTreeView(nodes[i].Nodes, expandDepth - 1);
                }
            }
        }

        public static string GetPrivilegeText(int priv)
        {
            bool isTaskManipulate = false, isTaskPrioChange = false, isTracing = false, isSignalInvoking = false;
            bool isFirst = true;
            string privString = "";

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

        private delegate void SetLabelTextDelegate(Label label, string Text);

        public static void SetLabelText_ThreadSafe(Control control, Label label, string text)
        {
            try
            {
                if (label.InvokeRequired)
                {
                    SetLabelTextDelegate d = new SetLabelTextDelegate(SetLabelText);
                    control.Invoke(d, label, text);
                }
                else
                {
                    SetLabelText(label, text);
                }
            }
            catch
            {

            }
        }

        private static void SetLabelText(Label label, string text)
        {
            label.Text = text;
        }

        private delegate void SetCheckBoxParametersDelegate(CheckBox checkBox, string text, Color backgroundColor, bool check);

        public static void SetCheckBoxParameters_ThreadSafe(Control control, CheckBox checkBox, string text, Color backgroundColor, bool check)
        {
            try
            {
                if (checkBox.InvokeRequired)
                {
                    SetCheckBoxParametersDelegate d = new SetCheckBoxParametersDelegate(SetCheckBoxParameters);
                    control.Invoke(d, checkBox, text, backgroundColor, check);
                }
                else
                {
                    SetCheckBoxParameters(checkBox, text, backgroundColor, check);
                }
            }
            catch
            {

            }
        }
        private static void SetCheckBoxParameters(CheckBox checkBox, string text, Color backgroundColor, bool check)
        {
            checkBox.Text = text;
            checkBox.BackColor = backgroundColor;
            checkBox.Checked = check;
        }

        public static string ConvertTaskState(byte taskState)
        {
            switch (taskState)
            {
                case 10: return "ready";
                case 22: return "sleeping";
                case 25: return "blocked";
                case 5: return "suspended";
                case 13: return "zombie";
                default: return "invalid";
            }
        }

        public static string ConvertTaskRuntime(RunTime runtime)
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

        private delegate void UpdateListViewWithItemDelegate(ListView listview, List<ListViewItem> listViewItems);

        /// <summary>
        /// Thread-safe wrapper for listview updating.
        /// </summary>
        /// <param name="control"></param>
        /// <param name="listView"></param>
        /// <param name="listViewItems"></param>
        public static void UpdateListViewWithItems_ThreadSafe(Control control, ListView listView, List<ListViewItem> listViewItems)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    UpdateListViewWithItemDelegate d = new UpdateListViewWithItemDelegate(UpdateListViewWithItems);
                    control.Invoke(d, listView, listViewItems);
                }
                else
                {
                    UpdateListViewWithItems(listView, listViewItems);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Listview updating method.
        /// </summary>
        /// <param name="listView"></param>
        /// <param name="listViewItems"></param>
        private static void UpdateListViewWithItems(ListView listView, List<ListViewItem> listViewItems)
        {
            listView.Items.Clear();
            listView.Items.AddRange(listViewItems.ToArray());
        }

        private delegate void ResizeListViewDelegate(ListView listView);

        /// <summary>
        /// Thread-safe wrapper for listview resizing.
        /// </summary>
        /// <param name="control"></param>
        /// <param name="listView"></param>
        public static void ResizeListView_ThreadSafe(Control control, ListView listView)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    ResizeListViewDelegate d = new ResizeListViewDelegate(ResizeListView);
                    control.Invoke(d, listView);
                }
                else
                {
                    ResizeListView(listView);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Listview resizing method.
        /// </summary>
        /// <param name="listView"></param>
        private static void ResizeListView(ListView listView)
        {
            for (int i = 0; i < listView.Columns.Count; i++)
            {
                listView.AutoResizeColumn(i, ColumnHeaderAutoResizeStyle.ColumnContent);
            }
        }
    }
}
