using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class ProjectManager : Form
    {
        private delegate void SetterDelegate<T>(T value);

        Dictionary<string, Delegate> delegateLookup = new Dictionary<string, Delegate>()
        {
            ["wsPathTb"] = (SetterDelegate<string>)ProjectHandler.WorkspacePath.SetValue,
            ["projNameTb"] = (SetterDelegate<string>)ProjectHandler.ProjectName.SetValue,
            ["availableProjCb"] = (SetterDelegate<string>)ProjectHandler.SelectedProject.SetValue
        };

        public ProjectManager()
        {
            InitializeComponent();
        }

        private void ProjectManager_Load(object sender, EventArgs e)
        {
            InitializeBindings();
            ProjectHandler.WorkspacePath.SetValue(Directory.GetCurrentDirectory());
            ProjectHandler.ProjectName.SetValue("New project");
            ProjectHandler.GetProjectNames();
            Text = ProgramData.Name + " " + ProgramData.Version + " - Project Manager";
        }

        private void InitializeBindings()
        {
            ValueHandler.SetBinding<string, string>(wsPathTb, "Text", ProjectHandler.WorkspacePath);
            ValueHandler.SetBinding<string, string>(projNameTb, "Text", ProjectHandler.ProjectName);
            ValueHandler.SetBinding<string, string>(availableProjCb, "SelectedItem", ProjectHandler.SelectedProject);

            ValueHandler.SetBinding<string, List<string>, bool>(createButton, "Enabled", ProjectHandler.ProjectName, ProjectHandler.ProjectNames, Converter.IsCreateEnabled);
            ValueHandler.SetBinding<string, List<string>, bool>(openButton, "Enabled", ProjectHandler.ProjectName, ProjectHandler.ProjectNames, Converter.IsOpenEnabled);
        }

        private void wsPathTb_TextChanged(object sender, EventArgs e)
        {
            var value = (sender as TextBox).Text;
            delegateLookup[(sender as TextBox).Name].DynamicInvoke(value);
        }

        private void projNameTb_TextChanged(object sender, EventArgs e)
        {
            var value = (sender as TextBox).Text;
            delegateLookup[(sender as TextBox).Name].DynamicInvoke(value);
        }

        private void availableProjCb_DropDown(object sender, EventArgs e)
        {
            ProjectHandler.GetProjectNames();
            availableProjCb.Items.Clear();
            availableProjCb.Items.AddRange(ProjectHandler.ProjectNames.Value.ToArray());
        }

        private void availableProjCb_SelectedValueChanged(object sender, EventArgs e)
        {
            var value = (sender as ComboBox).SelectedItem;
            delegateLookup[(sender as ComboBox).Name].DynamicInvoke(value);
            if (value != null)
            {
                projNameTb.Text = value.ToString();
            }
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                availableProjCb.SelectedItem = null;
                wsPathTb.Text = folderBrowserDialog.SelectedPath;
                ProjectHandler.GetProjectNames();
            }
        }

        private void createButton_Click(object sender, EventArgs e)
        {
            ProjectHandler.CreateProject();
            MainWindow mainWindow = new MainWindow(ProjectHandler.GetProjectData());
            mainWindow.Show();
            mainWindow.FormClosed += (s, args) =>
            {
                Close();
            };
            Hide();
        }

        private void openButton_Click(object sender, EventArgs e)
        {
            ProjectHandler.OpenProject();
            MainWindow mainWindow = new MainWindow(ProjectHandler.GetProjectData());
            mainWindow.Show();
            mainWindow.FormClosed += (s, args) =>
            {
                Close();
            };
            Hide();
        }
    }
}
