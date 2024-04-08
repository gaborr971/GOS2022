
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
            this.templateGeneratorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configuratorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.monitoringToolToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphicsDesignerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.projectDataUserControl1 = new GOSTool.ProjectDataUserControl();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.configureOSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureGOSLibToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureApplicationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configureBootloaderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.useOsCheckBox = new System.Windows.Forms.CheckBox();
            this.osVersionLabel = new System.Windows.Forms.Label();
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
            this.graphicsDesignerToolStripMenuItem});
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
            // monitoringToolToolStripMenuItem
            // 
            this.monitoringToolToolStripMenuItem.Enabled = false;
            this.monitoringToolToolStripMenuItem.Name = "monitoringToolToolStripMenuItem";
            this.monitoringToolToolStripMenuItem.Size = new System.Drawing.Size(128, 34);
            this.monitoringToolToolStripMenuItem.Text = "Monitoring tool";
            // 
            // graphicsDesignerToolStripMenuItem
            // 
            this.graphicsDesignerToolStripMenuItem.Enabled = false;
            this.graphicsDesignerToolStripMenuItem.Name = "graphicsDesignerToolStripMenuItem";
            this.graphicsDesignerToolStripMenuItem.Size = new System.Drawing.Size(141, 34);
            this.graphicsDesignerToolStripMenuItem.Text = "Graphics designer";
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // projectDataUserControl1
            // 
            this.projectDataUserControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.projectDataUserControl1.Location = new System.Drawing.Point(3, 18);
            this.projectDataUserControl1.Name = "projectDataUserControl1";
            this.projectDataUserControl1.Size = new System.Drawing.Size(990, 101);
            this.projectDataUserControl1.TabIndex = 1;
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
            // configureApplicationToolStripMenuItem
            // 
            this.configureApplicationToolStripMenuItem.Enabled = false;
            this.configureApplicationToolStripMenuItem.Name = "configureApplicationToolStripMenuItem";
            this.configureApplicationToolStripMenuItem.Size = new System.Drawing.Size(236, 26);
            this.configureApplicationToolStripMenuItem.Text = "Configure application";
            // 
            // configureBootloaderToolStripMenuItem
            // 
            this.configureBootloaderToolStripMenuItem.Enabled = false;
            this.configureBootloaderToolStripMenuItem.Name = "configureBootloaderToolStripMenuItem";
            this.configureBootloaderToolStripMenuItem.Size = new System.Drawing.Size(236, 26);
            this.configureBootloaderToolStripMenuItem.Text = "Configure GOSLoader";
            // 
            // saveProjectToolStripMenuItem
            // 
            this.saveProjectToolStripMenuItem.Name = "saveProjectToolStripMenuItem";
            this.saveProjectToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.saveProjectToolStripMenuItem.Text = "Save project";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.exitToolStripMenuItem.Text = "Exit";
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
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 21);
            this.label1.TabIndex = 0;
            this.label1.Text = "Use OS";
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
            // useOsCheckBox
            // 
            this.useOsCheckBox.AutoSize = true;
            this.useOsCheckBox.Checked = true;
            this.useOsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.useOsCheckBox.Enabled = false;
            this.useOsCheckBox.Location = new System.Drawing.Point(101, 28);
            this.useOsCheckBox.Name = "useOsCheckBox";
            this.useOsCheckBox.Size = new System.Drawing.Size(18, 17);
            this.useOsCheckBox.TabIndex = 2;
            this.useOsCheckBox.UseVisualStyleBackColor = true;
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
    }
}