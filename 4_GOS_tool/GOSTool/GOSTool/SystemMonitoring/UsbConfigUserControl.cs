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
    public partial class UsbConfigUserControl : UserControl
    {
        public string Port { get; set; } = null;
        public int Baud { get; set; } = int.MinValue;
        public UsbConfigUserControl()
        {
            InitializeComponent();
        }

        private void portComboBox_DropDown(object sender, EventArgs e)
        {
            portComboBox.Items.Clear();
            portComboBox.Items.AddRange(Uart.GetPortNames());            
        }

        private void baudComboBox_DropDown(object sender, EventArgs e)
        {
            baudComboBox.Items.Clear();
            baudComboBox.Items.AddRange(Uart.GetBaudRates());            
        }

        private void portComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            Port = portComboBox.SelectedItem.ToString();
        }

        private void baudComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            Baud = int.Parse(baudComboBox.SelectedItem.ToString());
        }
    }
}
