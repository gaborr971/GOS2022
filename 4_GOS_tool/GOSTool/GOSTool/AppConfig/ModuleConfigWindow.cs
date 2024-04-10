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
    public partial class ModuleConfigWindow : Form
    {
        public string ModuleName { get; private set; }
        public ModuleConfigWindow()
        {
            InitializeComponent();
        }

        public ModuleConfigWindow(string moduleName)
        {
            InitializeComponent();
            ModuleName = moduleName;
            moduleNameTb.Text = ModuleName;
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            ModuleName = moduleNameTb.Text;
            Close();
        }
    }
}
