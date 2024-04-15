using System;
using System.Collections.Generic;
using System.Drawing;
using System.Threading;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class LoadGraphUserControl : UserControl
    {
        private int _numOfSamples;
        public int NumberOfSamples
        { 
            get
            {
                return _numOfSamples;
            }
            set
            {
                measurementSemaphore.Wait();
                _numOfSamples = value;

                // Adjust size.
                if (measurements.Count != NumberOfSamples)
                {
                    if (measurements.Count > NumberOfSamples)
                    {
                        measurements = measurements.GetRange(measurements.Count - NumberOfSamples, NumberOfSamples);
                    }
                    else
                    {
                        int size = measurements.Count;
                        for (int i = 0; i < NumberOfSamples - size; i++)
                        {
                            measurements.Insert(0, 0);
                        }
                    }
                }
                measurementSemaphore.Release();
            }
        }

        private List<float> measurements = new List<float>();
        private SemaphoreSlim measurementSemaphore = new SemaphoreSlim(1, 1);
        public LoadGraphUserControl()
        {
            InitializeComponent();

            NumberOfSamples = 200;

            for (int i = 0; i < NumberOfSamples; i++)
            {
                measurements.Add(0);
            }
        }

        public void AddNewMeasurement(float value)
        {
            if (value >= 0)
            {
                measurementSemaphore.Wait();

                List<float> shifterMeasurements = new List<float>(measurements.GetRange(1, measurements.Count - 1));
                shifterMeasurements.Add(value);
                measurements = shifterMeasurements;
                Invalidate();

                measurementSemaphore.Release();
            }
        }

        public float GetAvg()
        {
            float sum = 0;

            for (int i = 0; i < measurements.Count; i++)
            {
                sum += measurements[i];
            }

            return sum / measurements.Count;
        }

        private void LoadGraphUserControl_Paint(object sender, PaintEventArgs e)
        {
            measurementSemaphore.Wait();
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
            float step = (float)width / (float)(NumberOfSamples - 1);
            int j = 1;

            for (float i = step; i < width && j < NumberOfSamples; i += step)
            {
                y1 = 0.1f * height + 0.8f * height * (100f - measurements[j - 1]) / 100f;

                if (measurements[j - 1] == 0)
                {
                    y1 = 0.9f * height;
                }

                y2 = 0.1f * height + 0.8f * height * (100f - measurements[j]) / 100f;

                if (measurements[j] == 0)
                {
                    y2 = 0.9f * height;
                }

                x1 = i - step;
                x2 = i;

                g.DrawLine(linePen, x1, y1, x2, y2);
                j++;
            }
            measurementSemaphore.Release();
        }

        private void LoadGraphUserControl_Resize(object sender, EventArgs e)
        {
            Invalidate();
        }
    }
}
