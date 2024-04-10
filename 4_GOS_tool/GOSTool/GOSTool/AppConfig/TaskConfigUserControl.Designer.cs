
namespace GOSTool.AppConfig
{
    partial class TaskConfigUserControl
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
            this.okButton = new System.Windows.Forms.Button();
            this.priorityNUD = new System.Windows.Forms.NumericUpDown();
            this.stackSizeNUD = new System.Windows.Forms.NumericUpDown();
            this.taskNameTb = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.cpuLoadNUD = new System.Windows.Forms.NumericUpDown();
            this.taskTypeCB = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.taskPrivilegeSelector1 = new GOSTool.TaskPrivilegeSelector();
            ((System.ComponentModel.ISupportInitialize)(this.priorityNUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.stackSizeNUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cpuLoadNUD)).BeginInit();
            this.SuspendLayout();
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(25, 415);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(104, 36);
            this.okButton.TabIndex = 32;
            this.okButton.Text = "Save";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // priorityNUD
            // 
            this.priorityNUD.Location = new System.Drawing.Point(135, 86);
            this.priorityNUD.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.priorityNUD.Name = "priorityNUD";
            this.priorityNUD.Size = new System.Drawing.Size(263, 22);
            this.priorityNUD.TabIndex = 31;
            // 
            // stackSizeNUD
            // 
            this.stackSizeNUD.Hexadecimal = true;
            this.stackSizeNUD.Increment = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.stackSizeNUD.Location = new System.Drawing.Point(135, 52);
            this.stackSizeNUD.Maximum = new decimal(new int[] {
            65536,
            0,
            0,
            0});
            this.stackSizeNUD.Name = "stackSizeNUD";
            this.stackSizeNUD.Size = new System.Drawing.Size(263, 22);
            this.stackSizeNUD.TabIndex = 30;
            // 
            // taskNameTb
            // 
            this.taskNameTb.Location = new System.Drawing.Point(135, 19);
            this.taskNameTb.Name = "taskNameTb";
            this.taskNameTb.Size = new System.Drawing.Size(263, 22);
            this.taskNameTb.TabIndex = 29;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(372, 123);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(20, 17);
            this.label7.TabIndex = 27;
            this.label7.Text = "%";
            // 
            // cpuLoadNUD
            // 
            this.cpuLoadNUD.DecimalPlaces = 2;
            this.cpuLoadNUD.Location = new System.Drawing.Point(135, 120);
            this.cpuLoadNUD.Name = "cpuLoadNUD";
            this.cpuLoadNUD.Size = new System.Drawing.Size(231, 22);
            this.cpuLoadNUD.TabIndex = 26;
            this.cpuLoadNUD.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // taskTypeCB
            // 
            this.taskTypeCB.FormattingEnabled = true;
            this.taskTypeCB.Location = new System.Drawing.Point(135, 153);
            this.taskTypeCB.Name = "taskTypeCB";
            this.taskTypeCB.Size = new System.Drawing.Size(263, 24);
            this.taskTypeCB.TabIndex = 25;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(22, 156);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(70, 17);
            this.label6.TabIndex = 24;
            this.label6.Text = "Task type";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(22, 123);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(100, 17);
            this.label5.TabIndex = 23;
            this.label5.Text = "Max. CPU load";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 197);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 17);
            this.label3.TabIndex = 22;
            this.label3.Text = "Privileges";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(22, 88);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 17);
            this.label4.TabIndex = 21;
            this.label4.Text = "Priority";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 17);
            this.label2.TabIndex = 20;
            this.label2.Text = "Stack size";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 17);
            this.label1.TabIndex = 19;
            this.label1.Text = "Name";
            // 
            // taskPrivilegeSelector1
            // 
            this.taskPrivilegeSelector1.Location = new System.Drawing.Point(135, 197);
            this.taskPrivilegeSelector1.Name = "taskPrivilegeSelector1";
            this.taskPrivilegeSelector1.Size = new System.Drawing.Size(347, 201);
            this.taskPrivilegeSelector1.TabIndex = 28;
            // 
            // TaskConfigUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.priorityNUD);
            this.Controls.Add(this.stackSizeNUD);
            this.Controls.Add(this.taskNameTb);
            this.Controls.Add(this.taskPrivilegeSelector1);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.cpuLoadNUD);
            this.Controls.Add(this.taskTypeCB);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "TaskConfigUserControl";
            this.Size = new System.Drawing.Size(519, 495);
            ((System.ComponentModel.ISupportInitialize)(this.priorityNUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.stackSizeNUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cpuLoadNUD)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.NumericUpDown priorityNUD;
        private System.Windows.Forms.NumericUpDown stackSizeNUD;
        private System.Windows.Forms.TextBox taskNameTb;
        private TaskPrivilegeSelector taskPrivilegeSelector1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown cpuLoadNUD;
        private System.Windows.Forms.ComboBox taskTypeCB;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
    }
}
