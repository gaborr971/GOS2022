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

namespace GOSTool
{
    public partial class ProjectDataUserControl : UserControl
    {
        public ProjectDataUserControl()
        {
            InitializeComponent();      
        }

        private async void ProjectDataUserControl_Load(object sender, EventArgs e)
        {
            await Task.Run(() => {
                while (true)
                {
                    SetLabel_ThreadSafe(currTimeLabel, DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                    Thread.Sleep(1000);
                }
            });
        }

        private delegate void SetLabelDelegate(Label label, string text);

        private void SetLabel(Label label, string text)
        {
            label.Text = text;
        }

        private void SetLabel_ThreadSafe(Label label, string text)
        {
            if (label.InvokeRequired)
            {
                SetLabelDelegate d = new SetLabelDelegate(SetLabel);
                Invoke(d, label, text);
            }
            else
            {
                SetLabel(label, text);
            }
        }

        public void SetProjectData(ProjectData projectData)
        {
            projNameLabel.Text = projectData.ProjectName;
            createdOnLabel.Text = projectData.CreationTime.ToString("yyyy-MM-dd HH:mm:ss");
            lastAccessedLabel.Text = projectData.LastAccessed.ToString("yyyy-MM-dd HH:mm:ss");
            lastModifiedLabel.Text = projectData.LastModified.ToString("yyyy-MM-dd HH:mm:ss");
            workspaceLabel.Text = projectData.Workspace;
        }
    }
}
