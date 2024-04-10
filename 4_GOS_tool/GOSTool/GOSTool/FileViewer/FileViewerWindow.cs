using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace GOSTool
{
    public partial class FileViewerWindow : Form
    {
        public FileViewerWindow()
        {
            InitializeComponent();
        }

        private void FileViewerWindow_Load(object sender, EventArgs e)
        {

        }

        public void OpenFileForView (string path)
        {
            string readFile = "";

            using (StreamReader sr = File.OpenText(path))
            {
                readFile = sr.ReadToEnd();
            }

            richTextBox1.Text = readFile;
            Text = "File viewer - [" + path + "]";
        }
    }
}
