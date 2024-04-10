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
    public partial class ModuleConfigUserControl : UserControl
    {
        public ModuleConfig ModuleConfiguration = new ModuleConfig();

        public event EventHandler saveClicked;
        public ModuleConfigUserControl()
        {
            InitializeComponent();
        }

        public void SetConfiguration(ModuleConfig moduleConfig)
        {
            ModuleConfiguration = moduleConfig;
            moduleNameTb.Text = moduleConfig.ModuleName;
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            ModuleConfiguration.ModuleName = moduleNameTb.Text;
            saveClicked?.Invoke(this, null);
        }
    }
}
