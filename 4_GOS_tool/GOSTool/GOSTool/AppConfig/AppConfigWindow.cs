using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class AppConfigWindow : Form
    {
        private ProjectData _projectData;
        public AppConfigWindow()
        {
            InitializeComponent();
            _projectData = ProjectHandler.GetProjectData();
            
            useConfigPatternCb.Checked = _projectData.AppConfig.UseConfigPattern;
            appNameTB.Text = _projectData.AppConfig.Name;
            appMajorNUD.Value = _projectData.AppConfig.Major;
            appMinorNUD.Value = _projectData.AppConfig.Minor;

            BuildTree(_projectData.AppConfig.Modules, appTreeView.Nodes);

            taskConfigUserControl1.saveClicked += (sender, arg) =>
            {
                BuildTree(_projectData.AppConfig.Modules, appTreeView.Nodes);
            };

            moduleConfigUserControl1.saveClicked += (sender, arg) =>
            {
                BuildTree(_projectData.AppConfig.Modules, appTreeView.Nodes);
            };
        }

        private void newModuleButton_Click(object sender, EventArgs e)
        {
            _projectData.AppConfig.Modules.Add(new ModuleConfig());
            BuildTree(_projectData.AppConfig.Modules, appTreeView.Nodes);
        }

        private void BuildTree(List<ModuleConfig> moduleConfigs, TreeNodeCollection addInMe)
        {
            addInMe.Clear();

            foreach (ModuleConfig moduleConfig in moduleConfigs)
            {
                TreeNode curNode = addInMe.Add(moduleConfig.ModuleName);

                // Create the ContextMenuStrip.
                ContextMenuStrip moduleMenu = new ContextMenuStrip();

                // Add menu items.
                ToolStripMenuItem deleteLabel = new ToolStripMenuItem();
                deleteLabel.Text = "Delete";
                deleteLabel.Click += (s, args) =>
                {
                    addInMe.RemoveAt(addInMe.IndexOf(curNode));
                    moduleConfigs.RemoveAt(moduleConfigs.IndexOf(moduleConfig));
                };

                ToolStripMenuItem addTaskLabel = new ToolStripMenuItem();
                addTaskLabel.Text = "Add task";
                addTaskLabel.Click += (s, args) =>
                {
                    moduleConfig.Tasks.Add(new TaskConfig());
                    BuildTree(_projectData.AppConfig.Modules, appTreeView.Nodes);
                };

                // Add the menu items to the menu.
                moduleMenu.Items.AddRange(new ToolStripMenuItem[] { /*editLabel,*/ deleteLabel, addTaskLabel });

                // Set the ContextMenuStrip property to the ContextMenuStrip.
                curNode.ContextMenuStrip = moduleMenu;

                foreach (TaskConfig taskConfig in moduleConfig.Tasks)
                {
                    TreeNode taskNode = curNode.Nodes.Add(taskConfig.TaskName);

                    ToolStripMenuItem deleteTaskLabel = new ToolStripMenuItem();
                    deleteTaskLabel.Text = "Delete";
                    deleteTaskLabel.Click += (s, args) =>
                    {
                        curNode.Nodes.RemoveAt(curNode.Nodes.IndexOf(taskNode));
                        moduleConfig.Tasks.RemoveAt(moduleConfig.Tasks.IndexOf(taskConfig));
                    };

                    // Create the ContextMenuStrip.
                    ContextMenuStrip taskMenu = new ContextMenuStrip();

                    // Add the menu items to the menu.
                    taskMenu.Items.AddRange(new ToolStripMenuItem[] { deleteTaskLabel });
                    taskNode.ContextMenuStrip = taskMenu;
                }
            }

            appTreeView.ExpandAll();

            appTreeView.AfterSelect += (s, e) =>
            {
                if (e.Node.Parent != null)
                {
                    taskConfigUserControl1.SetConfiguration(moduleConfigs.First(x=>x.ModuleName == e.Node.Parent.Text).Tasks[e.Node.Parent.Nodes.IndexOf(e.Node)]);
                    taskConfigUserControl1.Show();

                    moduleConfigUserControl1.Hide();

                    groupBox1.Text = "Task configuration";
                }
                else
                {
                    moduleConfigUserControl1.SetConfiguration(moduleConfigs.First(x => x.ModuleName == e.Node.Text));
                    moduleConfigUserControl1.Show();

                    taskConfigUserControl1.Hide();

                    groupBox1.Text = "Module configuration";
                }
            };
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (_projectData.AppConfig.Name != appNameTB.Text)
            {
                string projectFolder = ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\" + _projectData.AppConfig.Name;
                Directory.Delete(projectFolder, true);
            }
            _projectData.AppConfig.UseConfigPattern = useConfigPatternCb.Checked;
            _projectData.AppConfig.Name = appNameTB.Text;
            _projectData.AppConfig.Major = (int)appMajorNUD.Value;
            _projectData.AppConfig.Minor = (int)appMinorNUD.Value;

            ProjectHandler.SaveProjectData(_projectData);
            GenerateApplicationFiles();
            Close();
        }

        private void GenerateApplicationFiles()
        {
            Directory.Delete(ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\" + _projectData.AppConfig.Name, true);
            Helper.CopyFilesRecursively(ProgramData.EmptyAppPath, ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\" + _projectData.AppConfig.Name);
            CodeGenerator.GenerateApplicationFiles();
        }

        private void UpdateTitle()
        {
            Text = "Application configuration - [" + appNameTB.Text + " v" + appMajorNUD.Value + "." + appMinorNUD.Value + "]";
        }

        private void appNameTB_TextChanged(object sender, EventArgs e)
        {
            UpdateTitle();
        }

        private void appMajorNUD_ValueChanged(object sender, EventArgs e)
        {
            UpdateTitle();
        }

        private void appMinorNUD_ValueChanged(object sender, EventArgs e)
        {
            UpdateTitle();
        }
    }
}
