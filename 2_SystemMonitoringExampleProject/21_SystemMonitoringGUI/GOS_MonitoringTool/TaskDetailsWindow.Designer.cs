
namespace GOS_MonitoringTool
{
    partial class TaskDetailsWindow
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cpuUtilLabel = new System.Windows.Forms.Label();
            this.stackUtilLabel = new System.Windows.Forms.Label();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.cpuLoadGraph2 = new GOS_MonitoringTool.CpuLoadGraph();
            this.cpuLoadGraph1 = new GOS_MonitoringTool.CpuLoadGraph();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(99, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "CPU utilization";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 193);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(106, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Stack utilization";
            // 
            // cpuUtilLabel
            // 
            this.cpuUtilLabel.AutoSize = true;
            this.cpuUtilLabel.Location = new System.Drawing.Point(118, 15);
            this.cpuUtilLabel.Name = "cpuUtilLabel";
            this.cpuUtilLabel.Size = new System.Drawing.Size(47, 17);
            this.cpuUtilLabel.TabIndex = 4;
            this.cpuUtilLabel.Text = "N/A %";
            // 
            // stackUtilLabel
            // 
            this.stackUtilLabel.AutoSize = true;
            this.stackUtilLabel.Location = new System.Drawing.Point(121, 192);
            this.stackUtilLabel.Name = "stackUtilLabel";
            this.stackUtilLabel.Size = new System.Drawing.Size(47, 17);
            this.stackUtilLabel.TabIndex = 5;
            this.stackUtilLabel.Text = "N/A %";
            // 
            // listView1
            // 
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(12, 387);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(775, 149);
            this.listView1.TabIndex = 6;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Width = 100;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Width = 200;
            // 
            // cpuLoadGraph2
            // 
            this.cpuLoadGraph2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cpuLoadGraph2.BackColor = System.Drawing.Color.Black;
            this.cpuLoadGraph2.Location = new System.Drawing.Point(12, 216);
            this.cpuLoadGraph2.Name = "cpuLoadGraph2";
            this.cpuLoadGraph2.Size = new System.Drawing.Size(775, 140);
            this.cpuLoadGraph2.TabIndex = 2;
            // 
            // cpuLoadGraph1
            // 
            this.cpuLoadGraph1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cpuLoadGraph1.BackColor = System.Drawing.Color.Black;
            this.cpuLoadGraph1.Location = new System.Drawing.Point(12, 38);
            this.cpuLoadGraph1.Name = "cpuLoadGraph1";
            this.cpuLoadGraph1.Size = new System.Drawing.Size(775, 140);
            this.cpuLoadGraph1.TabIndex = 0;
            // 
            // TaskDetailsWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 548);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.stackUtilLabel);
            this.Controls.Add(this.cpuUtilLabel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cpuLoadGraph2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cpuLoadGraph1);
            this.Name = "TaskDetailsWindow";
            this.Text = "TaskDetailsWindow";
            this.Load += new System.EventHandler(this.TaskDetailsWindow_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CpuLoadGraph cpuLoadGraph1;
        private System.Windows.Forms.Label label1;
        private CpuLoadGraph cpuLoadGraph2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label cpuUtilLabel;
        private System.Windows.Forms.Label stackUtilLabel;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
    }
}