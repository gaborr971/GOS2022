using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOS_MonitoringTool
{
    public partial class CpuLoadGraph : UserControl
    {
        private float[] Measurements = new float[100];

        public CpuLoadGraph()
        {
            InitializeComponent();
        }

        public void AddNewMeasurement (float value)
        {
            var shiftedArray = new float[100];

            Array.Copy(Measurements, 1, shiftedArray, 0, 99);

            shiftedArray[99] = value;

            Array.Copy(shiftedArray, 0, Measurements, 0, 100);

            Invalidate();
        }

        private void CpuLoadGraph_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(this.BackColor);
            Graphics g = e.Graphics;
            Pen pen = new Pen(Color.Green);
            Pen linePen = new Pen(Color.Yellow, 2);

            int height = this.ClientSize.Height;
            int width = this.ClientSize.Width;

            float x1, x2, y1, y2;

            for (int i = 10; i < 100; i += 10)
            {
                x1 = 0;
                x2 = width;
                y1 = (float)i / 100f * (float)height;
                y2 = (float)i / 100f * (float)height;

                g.DrawLine(pen, x1, y1, x2, y2);
            }
            
            for (int i = 5; i < 100; i += 5)
            {
                x1 = (float)i / 100f * (float)width;
                x2 = (float)i / 100f * (float)width;
                y1 = 0;
                y2 = height;

                g.DrawLine(pen, x1, y1, x2, y2);
            }

            // Paint measurements
            float step = (float)width / (float)(100 - 1);
            int j = 1;

            for (float i = step; i < width && j < 100; i += step)
            {
                y1 = 0.1f * height + 0.8f * height * (100f - Measurements[j - 1]) / 100f;

                if (Measurements[j - 1] == 0)
                {
                    y1 = 0.9f * height;
                }

                y2 = 0.1f * height + 0.8f * height * (100f - Measurements[j]) / 100f;

                if (Measurements[j] == 0)
                {
                    y2 = 0.9f * height;
                }

                x1 = i - step;
                x2 = i;

                g.DrawLine(linePen, x1, y1, x2, y2);
                j++;
            }
        }

        private void CpuLoadGraph_Resize(object sender, EventArgs e)
        {
            Invalidate();
        }
    }
}
