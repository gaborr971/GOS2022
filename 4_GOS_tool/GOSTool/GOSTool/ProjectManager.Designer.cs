
namespace GOSTool
{
    partial class ProjectManager
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
            this.label3 = new System.Windows.Forms.Label();
            this.wsPathTb = new System.Windows.Forms.TextBox();
            this.projNameTb = new System.Windows.Forms.TextBox();
            this.availableProjCb = new System.Windows.Forms.ComboBox();
            this.openButton = new System.Windows.Forms.Button();
            this.createButton = new System.Windows.Forms.Button();
            this.browseButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(111, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "Workspace path";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 68);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(91, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "Project name";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 115);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(119, 17);
            this.label3.TabIndex = 2;
            this.label3.Text = "Available projects";
            // 
            // wsPathTb
            // 
            this.wsPathTb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.wsPathTb.Location = new System.Drawing.Point(143, 18);
            this.wsPathTb.Name = "wsPathTb";
            this.wsPathTb.Size = new System.Drawing.Size(289, 22);
            this.wsPathTb.TabIndex = 3;
            this.wsPathTb.TextChanged += new System.EventHandler(this.wsPathTb_TextChanged);
            // 
            // projNameTb
            // 
            this.projNameTb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.projNameTb.Location = new System.Drawing.Point(143, 65);
            this.projNameTb.Name = "projNameTb";
            this.projNameTb.Size = new System.Drawing.Size(352, 22);
            this.projNameTb.TabIndex = 4;
            this.projNameTb.TextChanged += new System.EventHandler(this.projNameTb_TextChanged);
            // 
            // availableProjCb
            // 
            this.availableProjCb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.availableProjCb.FormattingEnabled = true;
            this.availableProjCb.Location = new System.Drawing.Point(143, 112);
            this.availableProjCb.Name = "availableProjCb";
            this.availableProjCb.Size = new System.Drawing.Size(352, 24);
            this.availableProjCb.TabIndex = 5;
            this.availableProjCb.DropDown += new System.EventHandler(this.availableProjCb_DropDown);
            this.availableProjCb.SelectedValueChanged += new System.EventHandler(this.availableProjCb_SelectedValueChanged);
            // 
            // openButton
            // 
            this.openButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.openButton.AutoSize = true;
            this.openButton.Location = new System.Drawing.Point(112, 173);
            this.openButton.Name = "openButton";
            this.openButton.Size = new System.Drawing.Size(112, 33);
            this.openButton.TabIndex = 6;
            this.openButton.Text = "Open";
            this.openButton.UseVisualStyleBackColor = true;
            this.openButton.Click += new System.EventHandler(this.openButton_Click);
            // 
            // createButton
            // 
            this.createButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.createButton.Location = new System.Drawing.Point(296, 173);
            this.createButton.Name = "createButton";
            this.createButton.Size = new System.Drawing.Size(112, 33);
            this.createButton.TabIndex = 7;
            this.createButton.Text = "Create";
            this.createButton.UseVisualStyleBackColor = true;
            this.createButton.Click += new System.EventHandler(this.createButton_Click);
            // 
            // browseButton
            // 
            this.browseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.browseButton.Location = new System.Drawing.Point(453, 16);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(42, 26);
            this.browseButton.TabIndex = 8;
            this.browseButton.Text = "...";
            this.browseButton.UseVisualStyleBackColor = true;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // ProjectManager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(506, 231);
            this.Controls.Add(this.browseButton);
            this.Controls.Add(this.createButton);
            this.Controls.Add(this.openButton);
            this.Controls.Add(this.availableProjCb);
            this.Controls.Add(this.projNameTb);
            this.Controls.Add(this.wsPathTb);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.MinimumSize = new System.Drawing.Size(524, 278);
            this.Name = "ProjectManager";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Load += new System.EventHandler(this.ProjectManager_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox wsPathTb;
        private System.Windows.Forms.TextBox projNameTb;
        private System.Windows.Forms.ComboBox availableProjCb;
        private System.Windows.Forms.Button openButton;
        private System.Windows.Forms.Button createButton;
        private System.Windows.Forms.Button browseButton;
    }
}

