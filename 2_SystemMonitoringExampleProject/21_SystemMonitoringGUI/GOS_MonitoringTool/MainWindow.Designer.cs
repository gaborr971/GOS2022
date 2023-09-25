
namespace GOS_MonitoringTool
{
    partial class MainWindow
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
            this.currentCpuUtil = new System.Windows.Forms.Label();
            this.monitoringButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fIleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.progressLabel = new System.Windows.Forms.Label();
            this.taskListView = new System.Windows.Forms.ListView();
            this.taskIdCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskNameCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskStackCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskPrioCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskCsCntr = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskStackMaxUsage = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskCpuUsage = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskCpuUsageLimit = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskPrivileges = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskState = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskRuntime = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.monitoringTimeLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.refreshTaskListCB = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.sysRuntimeLabel = new System.Windows.Forms.Label();
            this.resetButton = new System.Windows.Forms.Button();
            this.timeSyncButton = new System.Windows.Forms.Button();
            this.cpuLoadGraph1 = new GOS_MonitoringTool.CpuLoadGraph();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // currentCpuUtil
            // 
            this.currentCpuUtil.AutoSize = true;
            this.currentCpuUtil.Location = new System.Drawing.Point(147, 40);
            this.currentCpuUtil.Name = "currentCpuUtil";
            this.currentCpuUtil.Size = new System.Drawing.Size(31, 17);
            this.currentCpuUtil.TabIndex = 2;
            this.currentCpuUtil.Text = "N/A";
            // 
            // monitoringButton
            // 
            this.monitoringButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.monitoringButton.Location = new System.Drawing.Point(1041, 518);
            this.monitoringButton.Name = "monitoringButton";
            this.monitoringButton.Size = new System.Drawing.Size(86, 37);
            this.monitoringButton.TabIndex = 3;
            this.monitoringButton.Text = "Monitoring";
            this.monitoringButton.UseVisualStyleBackColor = true;
            this.monitoringButton.Click += new System.EventHandler(this.monitoringButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 40);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(129, 17);
            this.label3.TabIndex = 5;
            this.label3.Text = "System CPU usage";
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fIleToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1139, 30);
            this.menuStrip1.TabIndex = 6;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fIleToolStripMenuItem
            // 
            this.fIleToolStripMenuItem.Name = "fIleToolStripMenuItem";
            this.fIleToolStripMenuItem.Size = new System.Drawing.Size(46, 26);
            this.fIleToolStripMenuItem.Text = "File";
            // 
            // checkBox1
            // 
            this.checkBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBox1.AutoCheck = false;
            this.checkBox1.AutoSize = true;
            this.checkBox1.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.checkBox1.Location = new System.Drawing.Point(1030, 33);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBox1.Size = new System.Drawing.Size(97, 21);
            this.checkBox1.TabIndex = 7;
            this.checkBox1.Text = "Link active";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // progressLabel
            // 
            this.progressLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.progressLabel.AutoSize = true;
            this.progressLabel.Location = new System.Drawing.Point(15, 538);
            this.progressLabel.Name = "progressLabel";
            this.progressLabel.Size = new System.Drawing.Size(30, 17);
            this.progressLabel.TabIndex = 8;
            this.progressLabel.Text = "Idle";
            // 
            // taskListView
            // 
            this.taskListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.taskListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.taskIdCol,
            this.taskNameCol,
            this.taskStackCol,
            this.taskPrioCol,
            this.taskCsCntr,
            this.taskStackMaxUsage,
            this.taskCpuUsage,
            this.taskCpuUsageLimit,
            this.taskPrivileges,
            this.taskState,
            this.taskRuntime});
            this.taskListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.taskListView.HideSelection = false;
            this.taskListView.Location = new System.Drawing.Point(15, 279);
            this.taskListView.Name = "taskListView";
            this.taskListView.ShowItemToolTips = true;
            this.taskListView.Size = new System.Drawing.Size(1112, 233);
            this.taskListView.TabIndex = 9;
            this.taskListView.UseCompatibleStateImageBehavior = false;
            this.taskListView.View = System.Windows.Forms.View.Details;
            this.taskListView.MouseClick += new System.Windows.Forms.MouseEventHandler(this.taskListView_MouseClick);
            // 
            // taskIdCol
            // 
            this.taskIdCol.Text = "Task ID";
            this.taskIdCol.Width = 69;
            // 
            // taskNameCol
            // 
            this.taskNameCol.Text = "Task Name";
            this.taskNameCol.Width = 86;
            // 
            // taskStackCol
            // 
            this.taskStackCol.Text = "Task Stack Size";
            this.taskStackCol.Width = 116;
            // 
            // taskPrioCol
            // 
            this.taskPrioCol.Text = "Task Priority";
            this.taskPrioCol.Width = 109;
            // 
            // taskCsCntr
            // 
            this.taskCsCntr.Text = "Task CS Counter";
            this.taskCsCntr.Width = 128;
            // 
            // taskStackMaxUsage
            // 
            this.taskStackMaxUsage.Text = "Task Max. Stack Usage";
            this.taskStackMaxUsage.Width = 114;
            // 
            // taskCpuUsage
            // 
            this.taskCpuUsage.Text = "Task CPU usage";
            this.taskCpuUsage.Width = 118;
            // 
            // taskCpuUsageLimit
            // 
            this.taskCpuUsageLimit.Text = "Task CPU limit";
            this.taskCpuUsageLimit.Width = 113;
            // 
            // taskPrivileges
            // 
            this.taskPrivileges.Text = "Task Privileges";
            // 
            // taskState
            // 
            this.taskState.Text = "TaskState";
            // 
            // taskRuntime
            // 
            this.taskRuntime.Text = "Task Runtime";
            // 
            // monitoringTimeLabel
            // 
            this.monitoringTimeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.monitoringTimeLabel.AutoSize = true;
            this.monitoringTimeLabel.Location = new System.Drawing.Point(882, 40);
            this.monitoringTimeLabel.Name = "monitoringTimeLabel";
            this.monitoringTimeLabel.Size = new System.Drawing.Size(92, 17);
            this.monitoringTimeLabel.TabIndex = 10;
            this.monitoringTimeLabel.Text = "00:00:00.000";
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(768, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(108, 17);
            this.label1.TabIndex = 11;
            this.label1.Text = "Monitoring time:";
            // 
            // refreshTaskListCB
            // 
            this.refreshTaskListCB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.refreshTaskListCB.AutoSize = true;
            this.refreshTaskListCB.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.refreshTaskListCB.Location = new System.Drawing.Point(885, 534);
            this.refreshTaskListCB.Name = "refreshTaskListCB";
            this.refreshTaskListCB.Size = new System.Drawing.Size(131, 21);
            this.refreshTaskListCB.TabIndex = 12;
            this.refreshTaskListCB.Text = "Refresh task list";
            this.refreshTaskListCB.UseVisualStyleBackColor = true;
            this.refreshTaskListCB.CheckedChanged += new System.EventHandler(this.refreshTaskListCB_CheckedChanged);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(462, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(109, 17);
            this.label2.TabIndex = 13;
            this.label2.Text = "System runtime:";
            // 
            // sysRuntimeLabel
            // 
            this.sysRuntimeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.sysRuntimeLabel.AutoSize = true;
            this.sysRuntimeLabel.Location = new System.Drawing.Point(577, 40);
            this.sysRuntimeLabel.Name = "sysRuntimeLabel";
            this.sysRuntimeLabel.Size = new System.Drawing.Size(92, 17);
            this.sysRuntimeLabel.TabIndex = 14;
            this.sysRuntimeLabel.Text = "00:00:00.000";
            // 
            // resetButton
            // 
            this.resetButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.resetButton.Location = new System.Drawing.Point(771, 518);
            this.resetButton.Name = "resetButton";
            this.resetButton.Size = new System.Drawing.Size(89, 37);
            this.resetButton.TabIndex = 15;
            this.resetButton.Text = "Reset";
            this.resetButton.UseVisualStyleBackColor = true;
            this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
            // 
            // timeSyncButton
            // 
            this.timeSyncButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.timeSyncButton.Location = new System.Drawing.Point(634, 518);
            this.timeSyncButton.Name = "timeSyncButton";
            this.timeSyncButton.Size = new System.Drawing.Size(97, 37);
            this.timeSyncButton.TabIndex = 16;
            this.timeSyncButton.Text = "Time sync";
            this.timeSyncButton.UseVisualStyleBackColor = true;
            this.timeSyncButton.Click += new System.EventHandler(this.timeSyncButton_Click);
            // 
            // cpuLoadGraph1
            // 
            this.cpuLoadGraph1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cpuLoadGraph1.BackColor = System.Drawing.Color.Black;
            this.cpuLoadGraph1.Location = new System.Drawing.Point(15, 60);
            this.cpuLoadGraph1.Name = "cpuLoadGraph1";
            this.cpuLoadGraph1.Size = new System.Drawing.Size(1115, 184);
            this.cpuLoadGraph1.TabIndex = 4;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1139, 567);
            this.Controls.Add(this.timeSyncButton);
            this.Controls.Add(this.resetButton);
            this.Controls.Add(this.sysRuntimeLabel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.refreshTaskListCB);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.monitoringTimeLabel);
            this.Controls.Add(this.taskListView);
            this.Controls.Add(this.progressLabel);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cpuLoadGraph1);
            this.Controls.Add(this.monitoringButton);
            this.Controls.Add(this.currentCpuUtil);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Text = "GOS2022 Monitoring Tool v0.1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label currentCpuUtil;
        private System.Windows.Forms.Button monitoringButton;
        private CpuLoadGraph cpuLoadGraph1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fIleToolStripMenuItem;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Label progressLabel;
        private System.Windows.Forms.ListView taskListView;
        private System.Windows.Forms.Label monitoringTimeLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ColumnHeader taskIdCol;
        private System.Windows.Forms.ColumnHeader taskNameCol;
        private System.Windows.Forms.ColumnHeader taskStackCol;
        private System.Windows.Forms.ColumnHeader taskPrioCol;
        private System.Windows.Forms.ColumnHeader taskCsCntr;
        private System.Windows.Forms.ColumnHeader taskStackMaxUsage;
        private System.Windows.Forms.ColumnHeader taskCpuUsage;
        private System.Windows.Forms.ColumnHeader taskCpuUsageLimit;
        private System.Windows.Forms.ColumnHeader taskPrivileges;
        private System.Windows.Forms.ColumnHeader taskState;
        private System.Windows.Forms.CheckBox refreshTaskListCB;
        private System.Windows.Forms.ColumnHeader taskRuntime;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label sysRuntimeLabel;
        private System.Windows.Forms.Button resetButton;
        private System.Windows.Forms.Button timeSyncButton;
    }
}

