
namespace GOSTool
{
    partial class AppConfigWindow
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
            this.appTreeView = new System.Windows.Forms.TreeView();
            this.okButton = new System.Windows.Forms.Button();
            this.newModuleButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.appMinorNUD = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.appMajorNUD = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.appNameTB = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.useConfigPatternCb = new System.Windows.Forms.CheckBox();
            this.taskConfigUserControl1 = new GOSTool.AppConfig.TaskConfigUserControl();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.appMinorNUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.appMajorNUD)).BeginInit();
            this.SuspendLayout();
            // 
            // appTreeView
            // 
            this.appTreeView.Location = new System.Drawing.Point(12, 12);
            this.appTreeView.Name = "appTreeView";
            this.appTreeView.Size = new System.Drawing.Size(440, 664);
            this.appTreeView.TabIndex = 0;
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(866, 634);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(106, 42);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "Save configuration";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // newModuleButton
            // 
            this.newModuleButton.Location = new System.Drawing.Point(733, 634);
            this.newModuleButton.Name = "newModuleButton";
            this.newModuleButton.Size = new System.Drawing.Size(106, 42);
            this.newModuleButton.TabIndex = 2;
            this.newModuleButton.Text = "Add module";
            this.newModuleButton.UseVisualStyleBackColor = true;
            this.newModuleButton.Click += new System.EventHandler(this.newModuleButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.taskConfigUserControl1);
            this.groupBox1.Location = new System.Drawing.Point(480, 143);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(520, 485);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Task configuration";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.appMinorNUD);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.appMajorNUD);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.appNameTB);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.useConfigPatternCb);
            this.groupBox2.Location = new System.Drawing.Point(480, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(520, 125);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Application data / settings";
            // 
            // appMinorNUD
            // 
            this.appMinorNUD.Location = new System.Drawing.Point(377, 85);
            this.appMinorNUD.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.appMinorNUD.Name = "appMinorNUD";
            this.appMinorNUD.Size = new System.Drawing.Size(57, 22);
            this.appMinorNUD.TabIndex = 18;
            this.appMinorNUD.ValueChanged += new System.EventHandler(this.appMinorNUD_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(278, 87);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(93, 17);
            this.label3.TabIndex = 17;
            this.label3.Text = "Minor version";
            // 
            // appMajorNUD
            // 
            this.appMajorNUD.Location = new System.Drawing.Point(166, 85);
            this.appMajorNUD.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.appMajorNUD.Name = "appMajorNUD";
            this.appMajorNUD.Size = new System.Drawing.Size(57, 22);
            this.appMajorNUD.TabIndex = 16;
            this.appMajorNUD.ValueChanged += new System.EventHandler(this.appMajorNUD_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(28, 87);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(93, 17);
            this.label2.TabIndex = 15;
            this.label2.Text = "Major version";
            // 
            // appNameTB
            // 
            this.appNameTB.Location = new System.Drawing.Point(166, 53);
            this.appNameTB.Name = "appNameTB";
            this.appNameTB.Size = new System.Drawing.Size(270, 22);
            this.appNameTB.TabIndex = 14;
            this.appNameTB.TextChanged += new System.EventHandler(this.appNameTB_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(28, 56);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(116, 17);
            this.label1.TabIndex = 13;
            this.label1.Text = "Application name";
            // 
            // useConfigPatternCb
            // 
            this.useConfigPatternCb.AutoSize = true;
            this.useConfigPatternCb.Location = new System.Drawing.Point(31, 26);
            this.useConfigPatternCb.Name = "useConfigPatternCb";
            this.useConfigPatternCb.Size = new System.Drawing.Size(146, 21);
            this.useConfigPatternCb.TabIndex = 12;
            this.useConfigPatternCb.Text = "Use config pattern";
            this.useConfigPatternCb.UseVisualStyleBackColor = true;
            // 
            // taskConfigUserControl1
            // 
            this.taskConfigUserControl1.Location = new System.Drawing.Point(6, 24);
            this.taskConfigUserControl1.Name = "taskConfigUserControl1";
            this.taskConfigUserControl1.Size = new System.Drawing.Size(486, 463);
            this.taskConfigUserControl1.TabIndex = 4;
            // 
            // AppConfigWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1012, 688);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.newModuleButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.appTreeView);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "AppConfigWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Application configuration";
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.appMinorNUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.appMajorNUD)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView appTreeView;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button newModuleButton;
        private AppConfig.TaskConfigUserControl taskConfigUserControl1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown appMinorNUD;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown appMajorNUD;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox appNameTB;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox useConfigPatternCb;
    }
}