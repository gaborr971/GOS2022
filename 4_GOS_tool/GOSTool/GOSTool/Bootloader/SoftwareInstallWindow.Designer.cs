
namespace GOSTool
{
    partial class SoftwareInstallWindow
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
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.usbConfigUserControl1 = new GOSTool.UsbConfigUserControl();
            this.label1 = new System.Windows.Forms.Label();
            this.wirelessComRadioButton = new System.Windows.Forms.RadioButton();
            this.usbComRadioButton = new System.Windows.Forms.RadioButton();
            this.bootModeRequestButton = new System.Windows.Forms.Button();
            this.resetRequestButton = new System.Windows.Forms.Button();
            this.connectButton = new System.Windows.Forms.Button();
            this.swInfoListView = new System.Windows.Forms.ListView();
            this.paramCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.valCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.disconnectButton = new System.Windows.Forms.Button();
            this.installButton = new System.Windows.Forms.Button();
            this.browseButton = new System.Windows.Forms.Button();
            this.appBinaryPathTb = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.dateDTP = new System.Windows.Forms.DateTimePicker();
            this.label7 = new System.Windows.Forms.Label();
            this.descTb = new System.Windows.Forms.TextBox();
            this.authorTb = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.buildNUD = new System.Windows.Forms.NumericUpDown();
            this.minorNUD = new System.Windows.Forms.NumericUpDown();
            this.majorNUD = new System.Windows.Forms.NumericUpDown();
            this.nameTb = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.progressLabel = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.bldConnectBtn = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.buildNUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.minorNUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.majorNUD)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.usbConfigUserControl1);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.wirelessComRadioButton);
            this.groupBox2.Controls.Add(this.usbComRadioButton);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(595, 176);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Communication configuration";
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
            // bootModeRequestButton
            // 
            this.bootModeRequestButton.Enabled = false;
            this.bootModeRequestButton.Location = new System.Drawing.Point(372, 31);
            this.bootModeRequestButton.Name = "bootModeRequestButton";
            this.bootModeRequestButton.Size = new System.Drawing.Size(146, 32);
            this.bootModeRequestButton.TabIndex = 6;
            this.bootModeRequestButton.Text = "Boot mode request";
            this.bootModeRequestButton.UseVisualStyleBackColor = true;
            this.bootModeRequestButton.Click += new System.EventHandler(this.bootModeRequestButton_Click);
            // 
            // resetRequestButton
            // 
            this.resetRequestButton.Enabled = false;
            this.resetRequestButton.Location = new System.Drawing.Point(195, 31);
            this.resetRequestButton.Name = "resetRequestButton";
            this.resetRequestButton.Size = new System.Drawing.Size(146, 32);
            this.resetRequestButton.TabIndex = 5;
            this.resetRequestButton.Text = "Reset request";
            this.resetRequestButton.UseVisualStyleBackColor = true;
            this.resetRequestButton.Click += new System.EventHandler(this.resetRequestButton_Click);
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(18, 31);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(146, 32);
            this.connectButton.TabIndex = 4;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // swInfoListView
            // 
            this.swInfoListView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.swInfoListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.paramCol,
            this.valCol});
            this.swInfoListView.HideSelection = false;
            this.swInfoListView.Location = new System.Drawing.Point(12, 194);
            this.swInfoListView.Name = "swInfoListView";
            this.swInfoListView.Size = new System.Drawing.Size(595, 726);
            this.swInfoListView.TabIndex = 4;
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
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.bldConnectBtn);
            this.groupBox1.Controls.Add(this.disconnectButton);
            this.groupBox1.Controls.Add(this.installButton);
            this.groupBox1.Controls.Add(this.browseButton);
            this.groupBox1.Controls.Add(this.bootModeRequestButton);
            this.groupBox1.Controls.Add(this.connectButton);
            this.groupBox1.Controls.Add(this.appBinaryPathTb);
            this.groupBox1.Controls.Add(this.resetRequestButton);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(613, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(580, 176);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Control";
            // 
            // disconnectButton
            // 
            this.disconnectButton.Enabled = false;
            this.disconnectButton.Location = new System.Drawing.Point(372, 78);
            this.disconnectButton.Name = "disconnectButton";
            this.disconnectButton.Size = new System.Drawing.Size(146, 32);
            this.disconnectButton.TabIndex = 12;
            this.disconnectButton.Text = "Disconnect";
            this.disconnectButton.UseVisualStyleBackColor = true;
            this.disconnectButton.Click += new System.EventHandler(this.disconnectButton_Click);
            // 
            // installButton
            // 
            this.installButton.Enabled = false;
            this.installButton.Location = new System.Drawing.Point(195, 78);
            this.installButton.Name = "installButton";
            this.installButton.Size = new System.Drawing.Size(146, 32);
            this.installButton.TabIndex = 11;
            this.installButton.Text = "Install";
            this.installButton.UseVisualStyleBackColor = true;
            this.installButton.Click += new System.EventHandler(this.installButton_Click);
            // 
            // browseButton
            // 
            this.browseButton.Location = new System.Drawing.Point(477, 131);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(42, 26);
            this.browseButton.TabIndex = 10;
            this.browseButton.Text = "...";
            this.browseButton.UseVisualStyleBackColor = true;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // appBinaryPathTb
            // 
            this.appBinaryPathTb.Location = new System.Drawing.Point(147, 133);
            this.appBinaryPathTb.Name = "appBinaryPathTb";
            this.appBinaryPathTb.Size = new System.Drawing.Size(324, 22);
            this.appBinaryPathTb.TabIndex = 9;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 136);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(124, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "Application binary:";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.dateDTP);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.descTb);
            this.groupBox3.Controls.Add(this.authorTb);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.buildNUD);
            this.groupBox3.Controls.Add(this.minorNUD);
            this.groupBox3.Controls.Add(this.majorNUD);
            this.groupBox3.Controls.Add(this.nameTb);
            this.groupBox3.Location = new System.Drawing.Point(613, 194);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(580, 225);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Application data";
            // 
            // dateDTP
            // 
            this.dateDTP.Location = new System.Drawing.Point(310, 180);
            this.dateDTP.Name = "dateDTP";
            this.dateDTP.Size = new System.Drawing.Size(200, 22);
            this.dateDTP.TabIndex = 27;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(221, 185);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 17);
            this.label7.TabIndex = 26;
            this.label7.Text = "Date";
            // 
            // descTb
            // 
            this.descTb.Location = new System.Drawing.Point(310, 127);
            this.descTb.Name = "descTb";
            this.descTb.Size = new System.Drawing.Size(200, 22);
            this.descTb.TabIndex = 25;
            // 
            // authorTb
            // 
            this.authorTb.Location = new System.Drawing.Point(310, 76);
            this.authorTb.Name = "authorTb";
            this.authorTb.Size = new System.Drawing.Size(200, 22);
            this.authorTb.TabIndex = 24;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(221, 132);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(79, 17);
            this.label6.TabIndex = 23;
            this.label6.Text = "Description";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(221, 81);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(50, 17);
            this.label5.TabIndex = 22;
            this.label5.Text = "Author";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(221, 35);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(45, 17);
            this.label4.TabIndex = 21;
            this.label4.Text = "Name";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 132);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(39, 17);
            this.label3.TabIndex = 20;
            this.label3.Text = "Build";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(15, 81);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(43, 17);
            this.label8.TabIndex = 19;
            this.label8.Text = "Minor";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(15, 35);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(43, 17);
            this.label9.TabIndex = 18;
            this.label9.Text = "Major";
            // 
            // buildNUD
            // 
            this.buildNUD.Location = new System.Drawing.Point(92, 127);
            this.buildNUD.Name = "buildNUD";
            this.buildNUD.Size = new System.Drawing.Size(58, 22);
            this.buildNUD.TabIndex = 17;
            // 
            // minorNUD
            // 
            this.minorNUD.Location = new System.Drawing.Point(92, 76);
            this.minorNUD.Name = "minorNUD";
            this.minorNUD.Size = new System.Drawing.Size(58, 22);
            this.minorNUD.TabIndex = 16;
            // 
            // majorNUD
            // 
            this.majorNUD.Location = new System.Drawing.Point(92, 30);
            this.majorNUD.Name = "majorNUD";
            this.majorNUD.Size = new System.Drawing.Size(58, 22);
            this.majorNUD.TabIndex = 15;
            // 
            // nameTb
            // 
            this.nameTb.Location = new System.Drawing.Point(310, 30);
            this.nameTb.Name = "nameTb";
            this.nameTb.Size = new System.Drawing.Size(200, 22);
            this.nameTb.TabIndex = 14;
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.richTextBox1);
            this.groupBox4.Controls.Add(this.progressLabel);
            this.groupBox4.Controls.Add(this.progressBar1);
            this.groupBox4.Location = new System.Drawing.Point(613, 423);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(580, 495);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Status";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.richTextBox1.BackColor = System.Drawing.Color.Black;
            this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextBox1.ForeColor = System.Drawing.Color.White;
            this.richTextBox1.Location = new System.Drawing.Point(18, 21);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(556, 416);
            this.richTextBox1.TabIndex = 10;
            this.richTextBox1.Text = "";
            this.richTextBox1.TextChanged += new System.EventHandler(this.richTextBox1_TextChanged);
            // 
            // progressLabel
            // 
            this.progressLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.progressLabel.AutoSize = true;
            this.progressLabel.Location = new System.Drawing.Point(542, 460);
            this.progressLabel.Name = "progressLabel";
            this.progressLabel.Size = new System.Drawing.Size(28, 17);
            this.progressLabel.TabIndex = 9;
            this.progressLabel.Text = "0%";
            // 
            // progressBar1
            // 
            this.progressBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar1.Location = new System.Drawing.Point(18, 454);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(513, 23);
            this.progressBar1.TabIndex = 8;
            // 
            // bldConnectBtn
            // 
            this.bldConnectBtn.Location = new System.Drawing.Point(18, 78);
            this.bldConnectBtn.Name = "bldConnectBtn";
            this.bldConnectBtn.Size = new System.Drawing.Size(146, 32);
            this.bldConnectBtn.TabIndex = 13;
            this.bldConnectBtn.Text = "Bootloader connect";
            this.bldConnectBtn.UseVisualStyleBackColor = true;
            this.bldConnectBtn.Click += new System.EventHandler(this.bldConnectBtn_Click);
            // 
            // SoftwareInstallWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1205, 930);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.swInfoListView);
            this.Controls.Add(this.groupBox2);
            this.Name = "SoftwareInstallWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Software Install";
            this.Load += new System.EventHandler(this.SoftwareInstallWindow_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.buildNUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.minorNUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.majorNUD)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button bootModeRequestButton;
        private System.Windows.Forms.Button resetRequestButton;
        private System.Windows.Forms.Button connectButton;
        private UsbConfigUserControl usbConfigUserControl1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton wirelessComRadioButton;
        private System.Windows.Forms.RadioButton usbComRadioButton;
        private System.Windows.Forms.ListView swInfoListView;
        private System.Windows.Forms.ColumnHeader paramCol;
        private System.Windows.Forms.ColumnHeader valCol;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button disconnectButton;
        private System.Windows.Forms.Button installButton;
        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.TextBox appBinaryPathTb;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.DateTimePicker dateDTP;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox descTb;
        private System.Windows.Forms.TextBox authorTb;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown buildNUD;
        private System.Windows.Forms.NumericUpDown minorNUD;
        private System.Windows.Forms.NumericUpDown majorNUD;
        private System.Windows.Forms.TextBox nameTb;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label progressLabel;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button bldConnectBtn;
    }
}