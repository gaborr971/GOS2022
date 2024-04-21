using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class MainWindow : Form
    {
        public ProjectData ProjectData { get; private set; }
        public MainWindow(ProjectData projectData)
        {
            InitializeComponent();
            ProjectData = projectData;
            Text = ProgramData.Name + " " + ProgramData.Version + " [" + ProgramData.Date + "]";

            treeView1.NodeMouseDoubleClick += (sender, e) =>
            {
                DirectoryInfo directoryInfo = new DirectoryInfo(ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build");
                string fileName = e.Node.Text;
                var fileInfos = directoryInfo.GetFiles(fileName, SearchOption.AllDirectories);
                if (fileInfos.Length == 1)
                {
                    FileViewerWindow fileViewer = new FileViewerWindow();
                    fileViewer.OpenFileForView(fileInfos[0].FullName);
                    fileViewer.ShowDialog();
                }
            };
        }

        private void MainWindow_Load(object sender, EventArgs e)
        {
            WindowState = FormWindowState.Maximized;
            projectDataUserControl1.SetProjectData(ProjectData);
            UpdateProjectTree();
            UpdateProjectConfiguration();
        }

        private void configureOSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GOSConfigWindow configWindow = new GOSConfigWindow();
            configWindow.ShowDialog();
            ProjectData = ProjectHandler.GetProjectData();
            projectDataUserControl1.SetProjectData(ProjectData);
            UpdateProjectTree();
            UpdateProjectConfiguration();
        }

        private void UpdateProjectConfiguration()
        {
            useOsCheckBox.Checked = true;
            osVersionLabel.Text = ProjectData.OsConfig.Version;
        }

        private void UpdateProjectTree()
        {
            treeView1.Nodes.Clear();
            DirectoryInfo directoryInfo = new DirectoryInfo(ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build");
            if (directoryInfo.Exists)
            {
                BuildTree(directoryInfo, treeView1.Nodes);
            }
            Helper.ExpandTreeView(treeView1.Nodes, 2);
        }

        private void BuildTree(DirectoryInfo directoryInfo, TreeNodeCollection addInMe)
        {
            TreeNode curNode = addInMe.Add(directoryInfo.Name);

            foreach (FileInfo file in directoryInfo.GetFiles())
            {
                curNode.Nodes.Add(file.FullName, file.Name);
            }
            foreach (DirectoryInfo subdir in directoryInfo.GetDirectories())
            {
                BuildTree(subdir, curNode.Nodes);
            }
        }

        private void configureApplicationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AppConfigWindow appConfigWindow = new AppConfigWindow();
            appConfigWindow.ShowDialog();
            UpdateProjectTree();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Environment.Exit(-1);
        }

        private void systemMonitoringToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MonitoringWindow monitoringWindow = new MonitoringWindow();
            monitoringWindow.Show();
        }

        private void softwareInstallToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SoftwareInstallWindow softwareInstallWindow = new SoftwareInstallWindow();
            softwareInstallWindow.Show();
        }
    }
}
