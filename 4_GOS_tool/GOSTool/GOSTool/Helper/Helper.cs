using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool
{
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
    }
}
