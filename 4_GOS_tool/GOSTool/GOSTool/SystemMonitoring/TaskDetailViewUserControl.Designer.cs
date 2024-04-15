
namespace GOSTool
{
    partial class TaskDetailViewUserControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.cpuLoadGraph = new GOSTool.LoadGraphUserControl();
            this.stackLoadGraph = new GOSTool.LoadGraphUserControl();
            this.listView1 = new System.Windows.Forms.ListView();
            this.paramCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.valCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.cpuUtilLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.stackUtilLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.avgCpuUtil = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cpuLoadGraph
            // 
            this.cpuLoadGraph.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cpuLoadGraph.BackColor = System.Drawing.Color.Black;
            this.cpuLoadGraph.Location = new System.Drawing.Point(16, 37);
            this.cpuLoadGraph.Name = "cpuLoadGraph";
            this.cpuLoadGraph.NumberOfSamples = 200;
            this.cpuLoadGraph.Size = new System.Drawing.Size(614, 148);
            this.cpuLoadGraph.TabIndex = 0;
            // 
            // stackLoadGraph
            // 
            this.stackLoadGraph.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.stackLoadGraph.BackColor = System.Drawing.Color.Black;
            this.stackLoadGraph.Location = new System.Drawing.Point(16, 215);
            this.stackLoadGraph.Name = "stackLoadGraph";
            this.stackLoadGraph.NumberOfSamples = 200;
            this.stackLoadGraph.Size = new System.Drawing.Size(614, 148);
            this.stackLoadGraph.TabIndex = 1;
            // 
            // listView1
            // 
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.paramCol,
            this.valCol});
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(16, 388);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(614, 282);
            this.listView1.TabIndex = 7;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // paramCol
            // 
            this.paramCol.Text = "Parameter";
            this.paramCol.Width = 100;
            // 
            // valCol
            // 
            this.valCol.Text = "Value";
            this.valCol.Width = 200;
            // 
            // cpuUtilLabel
            // 
            this.cpuUtilLabel.AutoSize = true;
            this.cpuUtilLabel.Location = new System.Drawing.Point(119, 17);
            this.cpuUtilLabel.Name = "cpuUtilLabel";
            this.cpuUtilLabel.Size = new System.Drawing.Size(47, 17);
            this.cpuUtilLabel.TabIndex = 9;
            this.cpuUtilLabel.Text = "N/A %";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(99, 17);
            this.label1.TabIndex = 8;
            this.label1.Text = "CPU utilization";
            // 
            // stackUtilLabel
            // 
            this.stackUtilLabel.AutoSize = true;
            this.stackUtilLabel.Location = new System.Drawing.Point(122, 194);
            this.stackUtilLabel.Name = "stackUtilLabel";
            this.stackUtilLabel.Size = new System.Drawing.Size(47, 17);
            this.stackUtilLabel.TabIndex = 11;
            this.stackUtilLabel.Text = "N/A %";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 195);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(106, 17);
            this.label2.TabIndex = 10;
            this.label2.Text = "Stack utilization";
            // 
            // avgCpuUtil
            // 
            this.avgCpuUtil.AutoSize = true;
            this.avgCpuUtil.Location = new System.Drawing.Point(363, 17);
            this.avgCpuUtil.Name = "avgCpuUtil";
            this.avgCpuUtil.Size = new System.Drawing.Size(47, 17);
            this.avgCpuUtil.TabIndex = 13;
            this.avgCpuUtil.Text = "N/A %";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(197, 17);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(195, 21);
            this.label4.TabIndex = 12;
            this.label4.Text = "Average CPU utilization";
            // 
            // TaskDetailViewUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.avgCpuUtil);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.stackUtilLabel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cpuUtilLabel);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.stackLoadGraph);
            this.Controls.Add(this.cpuLoadGraph);
            this.Name = "TaskDetailViewUserControl";
            this.Size = new System.Drawing.Size(647, 684);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private LoadGraphUserControl cpuLoadGraph;
        private LoadGraphUserControl stackLoadGraph;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader paramCol;
        private System.Windows.Forms.ColumnHeader valCol;
        private System.Windows.Forms.Label cpuUtilLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label stackUtilLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label avgCpuUtil;
        private System.Windows.Forms.Label label4;
    }
}
