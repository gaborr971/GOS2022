
namespace GOSTool
{
    partial class MonitoringWindow
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.sampleNUD = new System.Windows.Forms.NumericUpDown();
            this.monitoringButton = new System.Windows.Forms.Button();
            this.linkActiveCheckBox = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.monitoringTimeLabel = new System.Windows.Forms.Label();
            this.sysRuntimeLabel = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.avgCpuUtil = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.currentCpuUtil = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.resetRequestButton = new System.Windows.Forms.Button();
            this.connectButton = new System.Windows.Forms.Button();
            this.usbConfigUserControl1 = new GOSTool.UsbConfigUserControl();
            this.label1 = new System.Windows.Forms.Label();
            this.wirelessComRadioButton = new System.Windows.Forms.RadioButton();
            this.usbComRadioButton = new System.Windows.Forms.RadioButton();
            this.cpuLoadGraph = new GOSTool.LoadGraphUserControl();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.softwareInfoPage = new System.Windows.Forms.TabPage();
            this.swInfoListView = new System.Windows.Forms.ListView();
            this.paramCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.valCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskListView = new System.Windows.Forms.ListView();
            this.taskIdCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskNameCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskStackCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskPrioCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskCSCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskStackMaxCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskCpuCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskCpuLimitCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskPrivCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskStateCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.taskRuntimeCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sampleNUD)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.softwareInfoPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.sampleNUD);
            this.groupBox1.Controls.Add(this.monitoringButton);
            this.groupBox1.Controls.Add(this.linkActiveCheckBox);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.monitoringTimeLabel);
            this.groupBox1.Controls.Add(this.sysRuntimeLabel);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.avgCpuUtil);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.currentCpuUtil);
            this.groupBox1.Location = new System.Drawing.Point(12, 224);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(439, 176);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Monitoring parameters";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(291, 69);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(62, 17);
            this.label6.TabIndex = 29;
            this.label6.Text = "Samples";
            // 
            // sampleNUD
            // 
            this.sampleNUD.Location = new System.Drawing.Point(359, 67);
            this.sampleNUD.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.sampleNUD.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.sampleNUD.Name = "sampleNUD";
            this.sampleNUD.Size = new System.Drawing.Size(56, 22);
            this.sampleNUD.TabIndex = 28;
            this.sampleNUD.Value = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.sampleNUD.ValueChanged += new System.EventHandler(this.sampleNUD_ValueChanged);
            // 
            // monitoringButton
            // 
            this.monitoringButton.BackColor = System.Drawing.Color.Green;
            this.monitoringButton.ForeColor = System.Drawing.Color.White;
            this.monitoringButton.Location = new System.Drawing.Point(313, 114);
            this.monitoringButton.Name = "monitoringButton";
            this.monitoringButton.Size = new System.Drawing.Size(102, 32);
            this.monitoringButton.TabIndex = 6;
            this.monitoringButton.Text = "Monitoring";
            this.monitoringButton.UseVisualStyleBackColor = false;
            this.monitoringButton.Click += new System.EventHandler(this.monitoringButton_Click);
            // 
            // linkActiveCheckBox
            // 
            this.linkActiveCheckBox.AutoCheck = false;
            this.linkActiveCheckBox.AutoSize = true;
            this.linkActiveCheckBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.linkActiveCheckBox.Location = new System.Drawing.Point(307, 35);
            this.linkActiveCheckBox.Name = "linkActiveCheckBox";
            this.linkActiveCheckBox.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.linkActiveCheckBox.Size = new System.Drawing.Size(108, 21);
            this.linkActiveCheckBox.TabIndex = 27;
            this.linkActiveCheckBox.Text = "Link inactive";
            this.linkActiveCheckBox.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 129);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(108, 17);
            this.label5.TabIndex = 26;
            this.label5.Text = "Monitoring time:";
            // 
            // monitoringTimeLabel
            // 
            this.monitoringTimeLabel.AutoSize = true;
            this.monitoringTimeLabel.Location = new System.Drawing.Point(171, 129);
            this.monitoringTimeLabel.Name = "monitoringTimeLabel";
            this.monitoringTimeLabel.Size = new System.Drawing.Size(92, 17);
            this.monitoringTimeLabel.TabIndex = 25;
            this.monitoringTimeLabel.Text = "00:00:00.000";
            // 
            // sysRuntimeLabel
            // 
            this.sysRuntimeLabel.AutoSize = true;
            this.sysRuntimeLabel.Location = new System.Drawing.Point(171, 98);
            this.sysRuntimeLabel.Name = "sysRuntimeLabel";
            this.sysRuntimeLabel.Size = new System.Drawing.Size(92, 17);
            this.sysRuntimeLabel.TabIndex = 24;
            this.sysRuntimeLabel.Text = "00:00:00.000";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 98);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(109, 17);
            this.label4.TabIndex = 23;
            this.label4.Text = "System runtime:";
            // 
            // avgCpuUtil
            // 
            this.avgCpuUtil.AutoSize = true;
            this.avgCpuUtil.Location = new System.Drawing.Point(171, 67);
            this.avgCpuUtil.Name = "avgCpuUtil";
            this.avgCpuUtil.Size = new System.Drawing.Size(31, 17);
            this.avgCpuUtil.TabIndex = 22;
            this.avgCpuUtil.Text = "N/A";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(124, 17);
            this.label2.TabIndex = 8;
            this.label2.Text = "Average CPU load";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 37);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(118, 17);
            this.label3.TabIndex = 7;
            this.label3.Text = "Current CPU load";
            // 
            // currentCpuUtil
            // 
            this.currentCpuUtil.AutoSize = true;
            this.currentCpuUtil.Location = new System.Drawing.Point(171, 37);
            this.currentCpuUtil.Name = "currentCpuUtil";
            this.currentCpuUtil.Size = new System.Drawing.Size(31, 17);
            this.currentCpuUtil.TabIndex = 6;
            this.currentCpuUtil.Text = "N/A";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.resetRequestButton);
            this.groupBox2.Controls.Add(this.connectButton);
            this.groupBox2.Controls.Add(this.usbConfigUserControl1);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.wirelessComRadioButton);
            this.groupBox2.Controls.Add(this.usbComRadioButton);
            this.groupBox2.Location = new System.Drawing.Point(457, 224);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(790, 176);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Communication configuration";
            // 
            // resetRequestButton
            // 
            this.resetRequestButton.Location = new System.Drawing.Point(178, 129);
            this.resetRequestButton.Name = "resetRequestButton";
            this.resetRequestButton.Size = new System.Drawing.Size(124, 32);
            this.resetRequestButton.TabIndex = 5;
            this.resetRequestButton.Text = "Reset request";
            this.resetRequestButton.UseVisualStyleBackColor = true;
            this.resetRequestButton.Click += new System.EventHandler(this.resetRequestButton_Click);
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(25, 129);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(102, 32);
            this.connectButton.TabIndex = 4;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // usbConfigUserControl1
            // 
            this.usbConfigUserControl1.Baud = -2147483648;
            this.usbConfigUserControl1.Location = new System.Drawing.Point(6, 64);
            this.usbConfigUserControl1.Name = "usbConfigUserControl1";
            this.usbConfigUserControl1.Port = null;
            this.usbConfigUserControl1.Size = new System.Drawing.Size(470, 56);
            this.usbConfigUserControl1.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(135, 17);
            this.label1.TabIndex = 2;
            this.label1.Text = "Communication type";
            // 
            // wirelessComRadioButton
            // 
            this.wirelessComRadioButton.AutoSize = true;
            this.wirelessComRadioButton.Enabled = false;
            this.wirelessComRadioButton.Location = new System.Drawing.Point(252, 37);
            this.wirelessComRadioButton.Name = "wirelessComRadioButton";
            this.wirelessComRadioButton.Size = new System.Drawing.Size(83, 21);
            this.wirelessComRadioButton.TabIndex = 1;
            this.wirelessComRadioButton.Text = "Wireless";
            this.wirelessComRadioButton.UseVisualStyleBackColor = true;
            // 
            // usbComRadioButton
            // 
            this.usbComRadioButton.AutoSize = true;
            this.usbComRadioButton.Checked = true;
            this.usbComRadioButton.Location = new System.Drawing.Point(178, 37);
            this.usbComRadioButton.Name = "usbComRadioButton";
            this.usbComRadioButton.Size = new System.Drawing.Size(57, 21);
            this.usbComRadioButton.TabIndex = 0;
            this.usbComRadioButton.TabStop = true;
            this.usbComRadioButton.Text = "USB";
            this.usbComRadioButton.UseVisualStyleBackColor = true;
            // 
            // cpuLoadGraph
            // 
            this.cpuLoadGraph.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cpuLoadGraph.BackColor = System.Drawing.Color.Black;
            this.cpuLoadGraph.Location = new System.Drawing.Point(12, 12);
            this.cpuLoadGraph.Name = "cpuLoadGraph";
            this.cpuLoadGraph.NumberOfSamples = 200;
            this.cpuLoadGraph.Size = new System.Drawing.Size(1235, 206);
            this.cpuLoadGraph.TabIndex = 0;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(12, 406);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.taskListView);
            this.splitContainer1.Size = new System.Drawing.Size(1235, 380);
            this.splitContainer1.SplitterDistance = 410;
            this.splitContainer1.TabIndex = 6;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.softwareInfoPage);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(410, 380);
            this.tabControl1.TabIndex = 5;
            // 
            // softwareInfoPage
            // 
            this.softwareInfoPage.Controls.Add(this.swInfoListView);
            this.softwareInfoPage.Location = new System.Drawing.Point(4, 25);
            this.softwareInfoPage.Name = "softwareInfoPage";
            this.softwareInfoPage.Padding = new System.Windows.Forms.Padding(3);
            this.softwareInfoPage.Size = new System.Drawing.Size(402, 351);
            this.softwareInfoPage.TabIndex = 0;
            this.softwareInfoPage.Text = "Software Info";
            this.softwareInfoPage.UseVisualStyleBackColor = true;
            // 
            // swInfoListView
            // 
            this.swInfoListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.paramCol,
            this.valCol});
            this.swInfoListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.swInfoListView.HideSelection = false;
            this.swInfoListView.Location = new System.Drawing.Point(3, 3);
            this.swInfoListView.Name = "swInfoListView";
            this.swInfoListView.Size = new System.Drawing.Size(396, 345);
            this.swInfoListView.TabIndex = 0;
            this.swInfoListView.UseCompatibleStateImageBehavior = false;
            this.swInfoListView.View = System.Windows.Forms.View.Details;
            // 
            // paramCol
            // 
            this.paramCol.Text = "Parameter";
            // 
            // valCol
            // 
            this.valCol.Text = "Value";
            // 
            // taskListView
            // 
            this.taskListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.taskIdCol,
            this.taskNameCol,
            this.taskStackCol,
            this.taskPrioCol,
            this.taskCSCol,
            this.taskStackMaxCol,
            this.taskCpuCol,
            this.taskCpuLimitCol,
            this.taskPrivCol,
            this.taskStateCol,
            this.taskRuntimeCol});
            this.taskListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.taskListView.HideSelection = false;
            this.taskListView.Location = new System.Drawing.Point(0, 0);
            this.taskListView.Name = "taskListView";
            this.taskListView.Size = new System.Drawing.Size(821, 380);
            this.taskListView.TabIndex = 6;
            this.taskListView.UseCompatibleStateImageBehavior = false;
            this.taskListView.View = System.Windows.Forms.View.Details;
            this.taskListView.MouseClick += new System.Windows.Forms.MouseEventHandler(this.taskListView_MouseClick);
            // 
            // taskIdCol
            // 
            this.taskIdCol.Text = "ID";
            this.taskIdCol.Width = 67;
            // 
            // taskNameCol
            // 
            this.taskNameCol.Text = "Name";
            this.taskNameCol.Width = 55;
            // 
            // taskStackCol
            // 
            this.taskStackCol.Text = "Stack Size";
            this.taskStackCol.Width = 80;
            // 
            // taskPrioCol
            // 
            this.taskPrioCol.Text = "Priority";
            this.taskPrioCol.Width = 64;
            // 
            // taskCSCol
            // 
            this.taskCSCol.Text = "Context Switch Counter";
            this.taskCSCol.Width = 150;
            // 
            // taskStackMaxCol
            // 
            this.taskStackMaxCol.Text = "Max. Stack Usage";
            this.taskStackMaxCol.Width = 123;
            // 
            // taskCpuCol
            // 
            this.taskCpuCol.Text = "CPU Usage";
            this.taskCpuCol.Width = 87;
            // 
            // taskCpuLimitCol
            // 
            this.taskCpuLimitCol.Text = "CPU Usage Limit";
            this.taskCpuLimitCol.Width = 115;
            // 
            // taskPrivCol
            // 
            this.taskPrivCol.Text = "Privileges";
            this.taskPrivCol.Width = 73;
            // 
            // taskStateCol
            // 
            this.taskStateCol.Text = "State";
            // 
            // taskRuntimeCol
            // 
            this.taskRuntimeCol.Text = "Runtime";
            this.taskRuntimeCol.Width = 67;
            // 
            // MonitoringWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1259, 798);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cpuLoadGraph);
            this.Name = "MonitoringWindow";
            this.Text = "MonitoringWindow";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MonitoringWindow_FormClosing);
            this.Load += new System.EventHandler(this.MonitoringWindow_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sampleNUD)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.softwareInfoPage.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private LoadGraphUserControl cpuLoadGraph;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label avgCpuUtil;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label currentCpuUtil;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton wirelessComRadioButton;
        private System.Windows.Forms.RadioButton usbComRadioButton;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label monitoringTimeLabel;
        private System.Windows.Forms.Label sysRuntimeLabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox linkActiveCheckBox;
        private UsbConfigUserControl usbConfigUserControl1;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.Button resetRequestButton;
        private System.Windows.Forms.Button monitoringButton;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown sampleNUD;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage softwareInfoPage;
        private System.Windows.Forms.ListView swInfoListView;
        private System.Windows.Forms.ColumnHeader paramCol;
        private System.Windows.Forms.ColumnHeader valCol;
        private System.Windows.Forms.ListView taskListView;
        private System.Windows.Forms.ColumnHeader taskIdCol;
        private System.Windows.Forms.ColumnHeader taskNameCol;
        private System.Windows.Forms.ColumnHeader taskStackCol;
        private System.Windows.Forms.ColumnHeader taskPrioCol;
        private System.Windows.Forms.ColumnHeader taskCSCol;
        private System.Windows.Forms.ColumnHeader taskStackMaxCol;
        private System.Windows.Forms.ColumnHeader taskCpuCol;
        private System.Windows.Forms.ColumnHeader taskCpuLimitCol;
        private System.Windows.Forms.ColumnHeader taskPrivCol;
        private System.Windows.Forms.ColumnHeader taskStateCol;
        private System.Windows.Forms.ColumnHeader taskRuntimeCol;
    }
}