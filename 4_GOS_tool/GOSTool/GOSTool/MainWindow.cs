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
            GOSConfigWindow configWindow = new GOSConfigWindow(ProjectData);
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
            DirectoryInfo directoryInfo = new DirectoryInfo(ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\GOS2022");
            if (directoryInfo.Exists)
            {
                //treeView1.AfterSelect += treeView1_AfterSelect;
                BuildTree(directoryInfo, treeView1.Nodes);
            }
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
    }
}
