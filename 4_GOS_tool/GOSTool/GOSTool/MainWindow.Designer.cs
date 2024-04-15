
namespace GOSTool
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
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.templateGeneratorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configuratorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureOSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureGOSLibToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureBootloaderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureApplicationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.monitoringToolToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.softwareInstallToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.systemMonitoringToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphicsDesignerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageToolToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createFLASHImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.useBldCheckBox = new System.Windows.Forms.CheckBox();
            this.bldVersionLabel = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.useLibCheckBox = new System.Windows.Forms.CheckBox();
            this.libVersionLabel = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.osVersionLabel = new System.Windows.Forms.Label();
            this.useOsCheckBox = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.projectDataUserControl1 = new GOSTool.ProjectDataUserControl();
            this.menuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.templateGeneratorToolStripMenuItem,
            this.configuratorToolStripMenuItem,
            this.monitoringToolToolStripMenuItem,
            this.graphicsDesignerToolStripMenuItem,
            this.imageToolToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1275, 38);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveProjectToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(46, 34);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveProjectToolStripMenuItem
            // 
            this.saveProjectToolStripMenuItem.Enabled = false;
            this.saveProjectToolStripMenuItem.Name = "saveProjectToolStripMenuItem";
            this.saveProjectToolStripMenuItem.Size = new System.Drawing.Size(174, 26);
            this.saveProjectToolStripMenuItem.Text = "Save project";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(174, 26);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // templateGeneratorToolStripMenuItem
            // 
            this.templateGeneratorToolStripMenuItem.Enabled = false;
            this.templateGeneratorToolStripMenuItem.Name = "templateGeneratorToolStripMenuItem";
            this.templateGeneratorToolStripMenuItem.Size = new System.Drawing.Size(154, 34);
            this.templateGeneratorToolStripMenuItem.Text = "Template generator";
            // 
            // configuratorToolStripMenuItem
            // 
            this.configuratorToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.configureOSToolStripMenuItem,
            this.configureGOSLibToolStripMenuItem,
            this.configureBootloaderToolStripMenuItem,
            this.configureApplicationToolStripMenuItem});
            this.configuratorToolStripMenuItem.Name = "configuratorToolStripMenuItem";
            this.configuratorToolStripMenuItem.Size = new System.Drawing.Size(156, 34);
            this.configuratorToolStripMenuItem.Text = "System configurator";
            // 
            // configureOSToolStripMenuItem
            // 
            this.configureOSToolStripMenuItem.Name = "configureOSToolStripMenuItem";
            this.configureOSToolStripMenuItem.Size = new System.Drawing.Size(236, 26);
            this.configureOSToolStripMenuItem.Text = "Configure GOS2022";
            this.configureOSToolStripMenuItem.Click += new System.EventHandler(this.configureOSToolStripMenuItem_Click);
            // 
            // configureGOSLibToolStripMenuItem
            // 
            this.configureGOSLibToolStripMenuItem.Enabled = false;
            this.configureGOSLibToolStripMenuItem.Name = "configureGOSLibToolStripMenuItem";
            this.configureGOSLibToolStripMenuItem.Size = new System.Drawing.Size(236, 26);
            this.configureGOSLibToolStripMenuItem.Text = "Configure GOSLib";
            // 
            // configureBootloaderToolStripMenuItem
            // 
            this.configureBootloaderToolStripMenuItem.Enabled = false;
            this.configureBootloaderToolStripMenuItem.Name = "configureBootloaderToolStripMenuItem";
            this.configureBootloaderToolStripMenuItem.Size = new System.Drawing.Size(236, 26);
            this.configureBootloaderToolStripMenuItem.Text = "Configure GOSLoader";
            // 
            // configureApplicationToolStripMenuItem
            // 
            this.configureApplicationToolStripMenuItem.Name = "configureApplicationToolStripMenuItem";
            this.configureApplicationToolStripMenuItem.Size = new System.Drawing.Size(236, 26);
            this.configureApplicationToolStripMenuItem.Text = "Configure application";
            this.configureApplicationToolStripMenuItem.Click += new System.EventHandler(this.configureApplicationToolStripMenuItem_Click);
            // 
            // monitoringToolToolStripMenuItem
            // 
            this.monitoringToolToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.softwareInstallToolStripMenuItem,
            this.systemMonitoringToolStripMenuItem});
            this.monitoringToolToolStripMenuItem.Name = "monitoringToolToolStripMenuItem";
            this.monitoringToolToolStripMenuItem.Size = new System.Drawing.Size(128, 34);
            this.monitoringToolToolStripMenuItem.Text = "Monitoring tool";
            // 
            // softwareInstallToolStripMenuItem
            // 
            this.softwareInstallToolStripMenuItem.Enabled = false;
            this.softwareInstallToolStripMenuItem.Name = "softwareInstallToolStripMenuItem";
            this.softwareInstallToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.softwareInstallToolStripMenuItem.Text = "Software install";
            // 
            // systemMonitoringToolStripMenuItem
            // 
            this.systemMonitoringToolStripMenuItem.Name = "systemMonitoringToolStripMenuItem";
            this.systemMonitoringToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.systemMonitoringToolStripMenuItem.Text = "System monitoring";
            this.systemMonitoringToolStripMenuItem.Click += new System.EventHandler(this.systemMonitoringToolStripMenuItem_Click);
            // 
            // graphicsDesignerToolStripMenuItem
            // 
            this.graphicsDesignerToolStripMenuItem.Enabled = false;
            this.graphicsDesignerToolStripMenuItem.Name = "graphicsDesignerToolStripMenuItem";
            this.graphicsDesignerToolStripMenuItem.Size = new System.Drawing.Size(141, 34);
            this.graphicsDesignerToolStripMenuItem.Text = "Graphics designer";
            // 
            // imageToolToolStripMenuItem
            // 
            this.imageToolToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createFLASHImageToolStripMenuItem});
            this.imageToolToolStripMenuItem.Name = "imageToolToolStripMenuItem";
            this.imageToolToolStripMenuItem.Size = new System.Drawing.Size(116, 34);
            this.imageToolToolStripMenuItem.Text = "Binary merger";
            // 
            // createFLASHImageToolStripMenuItem
            // 
            this.createFLASHImageToolStripMenuItem.Enabled = false;
            this.createFLASHImageToolStripMenuItem.Name = "createFLASHImageToolStripMenuItem";
            this.createFLASHImageToolStripMenuItem.Size = new System.Drawing.Size(228, 26);
            this.createFLASHImageToolStripMenuItem.Text = "Create FLASH image";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem1});
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(55, 34);
            this.aboutToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem1
            // 
            this.aboutToolStripMenuItem1.Enabled = false;
            this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
            this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(133, 26);
            this.aboutToolStripMenuItem1.Text = "About";
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.projectDataUserControl1);
            this.groupBox1.Location = new System.Drawing.Point(12, 551);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(996, 122);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Project Info";
            // 
            // treeView1
            // 
            this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView1.Location = new System.Drawing.Point(3, 18);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(418, 493);
            this.treeView1.TabIndex = 3;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.useBldCheckBox);
            this.groupBox2.Controls.Add(this.bldVersionLabel);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.useLibCheckBox);
            this.groupBox2.Controls.Add(this.libVersionLabel);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.osVersionLabel);
            this.groupBox2.Controls.Add(this.useOsCheckBox);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(442, 31);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(563, 514);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Project Configuration";
            // 
            // useBldCheckBox
            // 
            this.useBldCheckBox.AutoSize = true;
            this.useBldCheckBox.Enabled = false;
            this.useBldCheckBox.Location = new System.Drawing.Point(286, 31);
            this.useBldCheckBox.Name = "useBldCheckBox";
            this.useBldCheckBox.Size = new System.Drawing.Size(18, 17);
            this.useBldCheckBox.TabIndex = 11;
            this.useBldCheckBox.UseVisualStyleBackColor = true;
            // 
            // bldVersionLabel
            // 
            this.bldVersionLabel.AutoSize = true;
            this.bldVersionLabel.Location = new System.Drawing.Point(283, 67);
            this.bldVersionLabel.Name = "bldVersionLabel";
            this.bldVersionLabel.Size = new System.Drawing.Size(30, 17);
            this.bldVersionLabel.TabIndex = 10;
            this.bldVersionLabel.Text = "X.X";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(191, 67);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(85, 17);
            this.label6.TabIndex = 9;
            this.label6.Text = "BLD version";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(191, 31);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(64, 17);
            this.label7.TabIndex = 8;
            this.label7.Text = "Use BLD";
            // 
            // useLibCheckBox
            // 
            this.useLibCheckBox.AutoSize = true;
            this.useLibCheckBox.Enabled = false;
            this.useLibCheckBox.Location = new System.Drawing.Point(101, 95);
            this.useLibCheckBox.Name = "useLibCheckBox";
            this.useLibCheckBox.Size = new System.Drawing.Size(18, 17);
            this.useLibCheckBox.TabIndex = 7;
            this.useLibCheckBox.UseVisualStyleBackColor = true;
            // 
            // libVersionLabel
            // 
            this.libVersionLabel.AutoSize = true;
            this.libVersionLabel.Location = new System.Drawing.Point(98, 131);
            this.libVersionLabel.Name = "libVersionLabel";
            this.libVersionLabel.Size = new System.Drawing.Size(30, 17);
            this.libVersionLabel.TabIndex = 6;
            this.libVersionLabel.Text = "X.X";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 131);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(78, 17);
            this.label5.TabIndex = 5;
            this.label5.Text = "LIB version";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 95);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 17);
            this.label3.TabIndex = 4;
            this.label3.Text = "Use LIB";
            // 
            // osVersionLabel
            // 
            this.osVersionLabel.AutoSize = true;
            this.osVersionLabel.Location = new System.Drawing.Point(98, 61);
            this.osVersionLabel.Name = "osVersionLabel";
            this.osVersionLabel.Size = new System.Drawing.Size(30, 17);
            this.osVersionLabel.TabIndex = 3;
            this.osVersionLabel.Text = "X.X";
            // 
            // useOsCheckBox
            // 
            this.useOsCheckBox.AutoSize = true;
            this.useOsCheckBox.Enabled = false;
            this.useOsCheckBox.Location = new System.Drawing.Point(101, 28);
            this.useOsCheckBox.Name = "useOsCheckBox";
            this.useOsCheckBox.Size = new System.Drawing.Size(18, 17);
            this.useOsCheckBox.TabIndex = 2;
            this.useOsCheckBox.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "OS version";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "Use OS";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox3.Controls.Add(this.treeView1);
            this.groupBox3.Location = new System.Drawing.Point(12, 31);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(424, 514);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Project Tree";
            // 
            // projectDataUserControl1
            // 
            this.projectDataUserControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.projectDataUserControl1.Location = new System.Drawing.Point(3, 18);
            this.projectDataUserControl1.Name = "projectDataUserControl1";
            this.projectDataUserControl1.Size = new System.Drawing.Size(990, 101);
            this.projectDataUserControl1.TabIndex = 1;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1020, 685);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Load += new System.EventHandler(this.MainWindow_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem templateGeneratorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem configuratorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem monitoringToolToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem graphicsDesignerToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private ProjectDataUserControl projectDataUserControl1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ToolStripMenuItem configureOSToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem configureGOSLibToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem configureApplicationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem configureBootloaderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label osVersionLabel;
        private System.Windows.Forms.CheckBox useOsCheckBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ToolStripMenuItem softwareInstallToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem systemMonitoringToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem imageToolToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createFLASHImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;
        private System.Windows.Forms.CheckBox useBldCheckBox;
        private System.Windows.Forms.Label bldVersionLabel;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox useLibCheckBox;
        private System.Windows.Forms.Label libVersionLabel;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
    }
}