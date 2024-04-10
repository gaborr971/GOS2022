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
        }

        private void newModuleButton_Click(object sender, EventArgs e)
        {
            ModuleConfigWindow w = new ModuleConfigWindow();
            w.ShowDialog();
            if (w.ModuleName != "")
            {
                _projectData.AppConfig.Modules.Add(new ModuleConfig() { ModuleName = w.ModuleName });
                BuildTree(_projectData.AppConfig.Modules, appTreeView.Nodes);
            }
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
                ToolStripMenuItem editLabel = new ToolStripMenuItem();
                editLabel.Text = "Edit";
                editLabel.Click += (s, args) =>
                {
                    ModuleConfigWindow mc = new ModuleConfigWindow(moduleConfig.ModuleName);
                    mc.ShowDialog();
                    int nodeIndex = addInMe.IndexOf(curNode);
                    addInMe[nodeIndex].Text = mc.ModuleName;
                    moduleConfig.ModuleName = mc.ModuleName;
                };

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

                //Add the menu items to the menu.
                moduleMenu.Items.AddRange(new ToolStripMenuItem[] { editLabel, deleteLabel, addTaskLabel });

                // Set the ContextMenuStrip property to the ContextMenuStrip.
                curNode.ContextMenuStrip = moduleMenu;

                foreach (TaskConfig taskConfig in moduleConfig.Tasks)
                {
                    TreeNode taskNode = curNode.Nodes.Add(taskConfig.TaskName);
                }
            }

            appTreeView.ExpandAll();

            appTreeView.AfterSelect += (s, e) =>
            {
                if (e.Node.Parent != null)
                {
                    taskConfigUserControl1.SetConfiguration(moduleConfigs.First(x=>x.ModuleName == e.Node.Parent.Text).Tasks[e.Node.Parent.Nodes.IndexOf(e.Node)]);
                    taskConfigUserControl1.Enabled = true;
                }
                else
                {
                    taskConfigUserControl1.Enabled = false;
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
            Helper.CopyFilesRecursively(ProgramData.EmptyAppPath, ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\" + _projectData.AppConfig.Name);

            CreateModules();
        }

        private void CreateModules()
        {
            string projectFolder = ProjectHandler.WorkspacePath.Value + "\\" + ProjectHandler.ProjectName.Value + "\\Build\\" + _projectData.AppConfig.Name;

            foreach (var moduleConfig in _projectData.AppConfig.Modules)
            {
                // Create module folders.
                if (Directory.Exists(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName))
                {
                    Directory.Delete(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName, true);
                }
                Directory.CreateDirectory(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName);
                Directory.CreateDirectory(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\src");
                Directory.CreateDirectory(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\inc");

                // Read template.
                string moduleSourceFile = "";

                using (StreamReader sr = File.OpenText(ProgramData.SourceTemplatePath))
                {
                    moduleSourceFile = sr.ReadToEnd();
                }

                // Modify fields.
                moduleSourceFile = moduleSourceFile.Replace("#source_name", "app_" + moduleConfig.ModuleName.ToLower() + ".c");
                moduleSourceFile = moduleSourceFile.Replace("#author", ProgramData.Name + " " + ProgramData.Version);
                moduleSourceFile = moduleSourceFile.Replace("#date", DateTime.Now.ToString("yyyy-MM-dd"));
                moduleSourceFile = moduleSourceFile.Replace("#year", DateTime.Now.ToString("yyyy"));
                moduleSourceFile = moduleSourceFile.Replace("#header_name", "app_" + moduleConfig.ModuleName.ToLower() + ".h");
                moduleSourceFile = moduleSourceFile.Replace("#includes", "#include \"app_" + moduleConfig.ModuleName.ToLower() + ".h\"");
                moduleSourceFile = moduleSourceFile.Replace("#macros", "");

                string staticVariables = "";
                string functionPrototypes = "";
                string taskDescriptors = "";

                foreach (var taskConfig in moduleConfig.Tasks)
                {
                    staticVariables += "\r\n/**\r\n * " + ConvertTaskNameToComment(taskConfig.TaskName) + "task ID.\r\n */\r\n";
                    staticVariables += "GOS_STATIC gos_tid_t " + taskConfig.TaskName.ToLower() + "_id;\r\n";

                    functionPrototypes += "GOS_STATIC void_t " + ConvertTaskNameToFunctionName(taskConfig.TaskName) + " (void_t);\r\n";

                    taskDescriptors += "\r\n/**\r\n * " + ConvertTaskNameToComment(taskConfig.TaskName) + "task descriptor.\r\n */\r\n";
                    taskDescriptors += "GOS_STATIC gos_taskDescriptor_t " + ConvertTaskNameToFunctionName(taskConfig.TaskName) + "Desc =\r\n";
                    taskDescriptors += "{\r\n";
                    taskDescriptors += "    .taskFunction       = " + ConvertTaskNameToFunctionName(taskConfig.TaskName) + ",\r\n";
                    taskDescriptors += "    .taskName           = \"" + taskConfig.TaskName + "\",\r\n";
                    taskDescriptors += "    .taskStackSize      = " + "0x" + taskConfig.TaskStackSize.ToString("X") + ",\r\n";
                    taskDescriptors += "    .taskPriority       = " + taskConfig.TaskPriority + ",\r\n";
                    taskDescriptors += "    .taskPrivilegeLevel = " + Helper.GetPrivilegeText(taskConfig.TaskPrivileges) + ",\r\n";
                    taskDescriptors += "    .taskCpuUsageLimit  = " + taskConfig.TaskMaxCpu + "\r\n";
                    taskDescriptors += "};\r\n";
                }
                moduleSourceFile = moduleSourceFile.Replace("#static_variables", staticVariables);
                moduleSourceFile = moduleSourceFile.Replace("#function_prototypes", functionPrototypes);
                moduleSourceFile = moduleSourceFile.Replace("#taskdescriptors", taskDescriptors);


                // Write file.
                using (StreamWriter sw = new StreamWriter(projectFolder + "\\Core\\Modules\\" + moduleConfig.ModuleName + "\\src\\" + "app_" + moduleConfig.ModuleName.ToLower() + ".c"))
                {
                    sw.Write(moduleSourceFile);
                }
            }
        }

        private string ConvertTaskNameToComment(string taskName)
        {
            string replaced = taskName.Replace("_", " ");
            string[] splitted = replaced.Split(' ');
            for (int i = 0; i < splitted.Length; i++)
            {
                splitted[i] = splitted[i].ToCharArray()[0].ToString().ToUpper() + splitted[i].Substring(1);
            }
            string built = "";
            foreach(var word in splitted)
            {
                built += word + " ";
            }
            return built;
        }

        private string ConvertTaskNameToFunctionName(string taskName)
        {
            string replaced = taskName.Replace("_", " ");
            string[] splitted = replaced.Split(' ');
            for (int i = 0; i < splitted.Length; i++)
            {
                splitted[i] = splitted[i].ToCharArray()[0].ToString().ToUpper() + splitted[i].Substring(1);
            }
            string built = "";
            foreach (var word in splitted)
            {
                built += word;
            }
            return built;
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
