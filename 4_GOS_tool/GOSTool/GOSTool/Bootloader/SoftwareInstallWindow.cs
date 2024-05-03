using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GOSTool
{
    public partial class SoftwareInstallWindow : Form
    {
        public SoftwareInstallWindow()
        {
            InitializeComponent();
        }

        private async void connectButton_Click(object sender, EventArgs e)
        {
            if (usbComRadioButton.Checked)
            {
                // Check for invalid configuration.
                if (usbConfigUserControl1.Port == null || usbConfigUserControl1.Baud < 0)
                {
                    MessageBox.Show("Please select a port and a baud rate first!", "USB not configured", MessageBoxButtons.OK);
                }
                else
                {
                    await Task.Run(() =>
                    {
                        // Try to connect on the given port configuration.
                        Uart.Init(usbConfigUserControl1.Port, usbConfigUserControl1.Baud);

                        if (SysmonFunctions.PingDevice() == SysmonFunctions.PingResult.OK)
                        {
                            // Get software info.
                            BootloaderData softwareInfo = SysmonFunctions.GetSoftwareInfo();
                            List<ListViewItem> swInfoItems = new List<ListViewItem>();

                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader driver lib name", softwareInfo.BootloaderDriverInfo.Name }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader driver lib version", softwareInfo.BootloaderDriverInfo.Major.ToString("D2") + "." + softwareInfo.BootloaderDriverInfo.Minor.ToString("D2") + "." + softwareInfo.BootloaderDriverInfo.Build.ToString("D2") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader driver lib author", softwareInfo.BootloaderDriverInfo.Author }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader driver lib description", softwareInfo.BootloaderDriverInfo.Description }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader driver lib date", softwareInfo.BootloaderDriverInfo.Date.ToString("yyyy-MM-dd") }));

                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader name", softwareInfo.BootloaderInfo.Name }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader version", softwareInfo.BootloaderInfo.Major.ToString("D2") + "." + softwareInfo.BootloaderInfo.Minor.ToString("D2") + "." + softwareInfo.BootloaderInfo.Build.ToString("D2") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader author", softwareInfo.BootloaderInfo.Author }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader description", softwareInfo.BootloaderInfo.Description }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader date", softwareInfo.BootloaderInfo.Date.ToString("yyyy-MM-dd") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader address", "0x" + softwareInfo.StartAddress.ToString("X8") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader size", softwareInfo.Size.ToString() }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader CRC", "0x" + softwareInfo.Crc.ToString("X8") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Bootloader update mode", softwareInfo.BootUpdateMode.ToString() }));

                            swInfoItems.Add(new ListViewItem(new string[] { "Application driver lib name", softwareInfo.ApplicationData.DriverVersion.Name }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application driver lib version", softwareInfo.ApplicationData.DriverVersion.Major.ToString("D2") + "." + softwareInfo.ApplicationData.DriverVersion.Minor.ToString("D2") + "." + softwareInfo.ApplicationData.DriverVersion.Build.ToString("D2") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application driver lib author", softwareInfo.ApplicationData.DriverVersion.Author }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application driver lib description", softwareInfo.ApplicationData.DriverVersion.Description }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application driver lib date", softwareInfo.ApplicationData.DriverVersion.Date.ToString("yyyy-MM-dd") }));

                            swInfoItems.Add(new ListViewItem(new string[] { "Application name", softwareInfo.ApplicationData.AppVersion.Name }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application version", softwareInfo.ApplicationData.AppVersion.Major.ToString("D2") + "." + softwareInfo.ApplicationData.AppVersion.Minor.ToString("D2") + "." + softwareInfo.ApplicationData.AppVersion.Build.ToString("D2") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application author", softwareInfo.ApplicationData.AppVersion.Author }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application description", softwareInfo.ApplicationData.AppVersion.Description }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application date", softwareInfo.ApplicationData.AppVersion.Date.ToString("yyyy-MM-dd") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application address", "0x" + softwareInfo.ApplicationData.StartAddress.ToString("X8") }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application size", softwareInfo.ApplicationData.Size.ToString() }));
                            swInfoItems.Add(new ListViewItem(new string[] { "Application CRC", "0x" + softwareInfo.ApplicationData.Crc.ToString("X8") }));

                            Helper.UpdateListViewWithItems_ThreadSafe(this, swInfoListView, swInfoItems);
                            Helper.ResizeListView_ThreadSafe(this, swInfoListView);
                            TraceProgressNew_ThreadSafe("Device info received on " + usbConfigUserControl1.Port + ".");

                            EnableButton_ThreadSafe(bootModeRequestButton, true);
                            EnableButton_ThreadSafe(resetRequestButton, true);
                            EnableButton_ThreadSafe(disconnectButton, true);
                            EnableButton_ThreadSafe(installButton, true);
                        }
                        else
                        {
                            Uart.DeInit();
                            TraceProgressNew_ThreadSafe("Device is not present on " + usbConfigUserControl1.Port + ".");
                        }
                    });
                }
            }
            else
            {
                // To be implemented later.
            }
        }

        private async void bootModeRequestButton_Click(object sender, EventArgs e)
        {
            await Task.Run(() =>
            {
                byte[] recvBuf;

                GcpMessageHeader messageHeader = new GcpMessageHeader();
                messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_SWITCH_TO_BOOT_MODE;
                messageHeader.ProtocolVersion = 1;
                messageHeader.PayloadSize = 0;

                if (GCP.TransmitMessage(0, messageHeader, new byte[] { }) != true)
                {
                    TraceProgressNew_ThreadSafe("Communication error.");
                }
                else
                {
                    TraceProgressNew_ThreadSafe("Boot mode request sent.");

                    Thread.Sleep(1000);

                    bld_com_conn_req_msg_t reqMsg = new bld_com_conn_req_msg_t();
                    bld_com_conn_resp_msg_t respMsg = new bld_com_conn_resp_msg_t();

                    reqMsg.client.name = "gos_monitoring_tool_pc";

                    messageHeader = new GcpMessageHeader();
                    messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_CONN_REQ_ID;
                    messageHeader.ProtocolVersion = 0;
                    messageHeader.PayloadSize = (ushort)reqMsg.GetBytes().Length;

                    if (GCP.TransmitMessage(0, messageHeader, reqMsg.GetBytes()) != true)
                    {
                        TraceProgressNew_ThreadSafe("Communication error.");
                    }
                    else
                    {
                        if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                        {
                            TraceProgressNew_ThreadSafe("Communication error.");
                        }
                        else
                        {
                            // Connected.
                            TraceProgressNew_ThreadSafe("Connection established.");
                        }
                    }
                }
            });
        }

        private async void disconnectButton_Click(object sender, EventArgs e)
        {
            await Task.Run(() =>
            {
                byte[] recvBuf;

                bld_com_disconn_req_msg_t reqMsg = new bld_com_disconn_req_msg_t();
                bld_com_disconn_resp_msg_t respMsg = new bld_com_disconn_resp_msg_t();

                reqMsg.reason = 100;

                GcpMessageHeader messageHeader = new GcpMessageHeader();
                messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_DISCONN_REQ_ID;
                messageHeader.ProtocolVersion = 0;
                messageHeader.PayloadSize = (ushort)reqMsg.GetBytes().Length;

                if (GCP.TransmitMessage(0, messageHeader, reqMsg.GetBytes()) != true)
                {
                    TraceProgressNew_ThreadSafe("Communication error.");
                }
                else
                {
                    if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                    {
                        TraceProgressNew_ThreadSafe("Communication error.");
                    }
                    else
                    {
                        respMsg.GetFromBytes(recvBuf);
                        TraceProgressNew_ThreadSafe("Disconnected from device.");
                    }
                }
            });
        }

        private delegate void SetProgressBarDelegate(int percentage);

        private void SetProgressBar(int percentage)
        {
            progressBar1.Value = percentage;
            progressLabel.Text = percentage + "%";
        }

        private void SetProgressBar_ThreadSafe(int percentage)
        {
            if (progressBar1.InvokeRequired)
            {
                SetProgressBarDelegate d = new SetProgressBarDelegate(SetProgressBar);
                progressBar1.Invoke(d, percentage);
            }
            else
            {
                SetProgressBar(percentage);
            }
        }

        private delegate void TraceProgressNewDelegate(string message);

        private void TraceProgressNew(string message)
        {
            richTextBox1.Text += "[" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "]\t" + message + "\r\n";
        }

        private void TraceProgressNew_ThreadSafe(string message)
        {
            if (richTextBox1.InvokeRequired)
            {
                TraceProgressNewDelegate d = new TraceProgressNewDelegate(TraceProgressNew);
                richTextBox1.Invoke(d, message);
            }
            else
            {
                TraceProgressNew(message);
            }
        }

        private delegate void TraceProgressChangingDelegate(string message);

        private void TraceProgressChanging(string message)
        {
            richTextBox1.Lines = richTextBox1.Lines.Take(richTextBox1.Lines.Length - 2).ToArray();
            richTextBox1.Text += "\r\n[" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "]\t" + message + "\r\n";
        }

        private void TraceProgressChanging_ThreadSafe(string message)
        {
            if (richTextBox1.InvokeRequired)
            {
                TraceProgressChangingDelegate d = new TraceProgressChangingDelegate(TraceProgressChanging);
                richTextBox1.Invoke(d, message);
            }
            else
            {
                TraceProgressChanging(message);
            }
        }

        private delegate void EnableButtonDelegate(Button button, bool enabled);

        private void EnableButton(Button button, bool enabled)
        {
            button.Enabled = enabled;
        }

        private void EnableButton_ThreadSafe(Button button, bool enabled)
        {
            if (button.InvokeRequired)
            {
                EnableButtonDelegate d = new EnableButtonDelegate(EnableButton);
                button.Invoke(d, button, enabled);
            }
            else
            {
                EnableButton(button, enabled);
            }
        }

        private delegate void SetLabelTextDelegate(Label label, string text);

        private void SetLabelText(Label label, string text)
        {
            label.Text = text;
        }

        private void SetLabelText_ThreadSafe(Label label, string text)
        {
            if (label.InvokeRequired)
            {
                SetLabelTextDelegate d = new SetLabelTextDelegate(SetLabelText);
                label.Invoke(d, label, text);
            }
            else
            {
                SetLabelText(label, text);
            }
        }

        private async void resetRequestButton_Click(object sender, EventArgs e)
        {
            await Task.Run(() =>
            {
                SysmonFunctions.SendResetRequest();
            });
        }

        private async void installButton_Click(object sender, EventArgs e)
        {
            if (appBinaryPathTb.Text == "" || nameTb.Text == "" || authorTb.Text == "" || descTb.Text == "")
            {
                MessageBox.Show("Application binary path, application name, author, and description fields are mandatory.");
            }
            else
            {
                TraceProgressNew_ThreadSafe("Reading binary file...");

                List<byte> memoryContent = new List<byte>();
                memoryContent.AddRange(File.ReadAllBytes(appBinaryPathTb.Text));
                SetProgressBar_ThreadSafe(0);

                await Task.Run(() =>
                {
                    byte[] recvBuf;
                    bool installSuccess = true;
                    bld_com_install_data_req_msg_t installDataReq = new bld_com_install_data_req_msg_t();
                    bld_com_install_data_resp_msg_t installDataResp = new bld_com_install_data_resp_msg_t();

                    installDataReq.updateType = bld_com_update_t.BLD_COM_UPDATE_TYPE_INSTALL;
                    installDataReq.appData.appStartAddress = 0x08020000;
                    installDataReq.appData.appCrc = Crc.GetCrc32_new(memoryContent.ToArray());
                    installDataReq.appData.appSize = (uint)memoryContent.Count;
                    installDataReq.appData.appVersion.name = nameTb.Text;
                    installDataReq.appData.appVersion.author = authorTb.Text;
                    installDataReq.appData.appVersion.description = descTb.Text;
                    installDataReq.appData.appVersion.date.Years = (ushort)dateDTP.Value.Date.Year;
                    installDataReq.appData.appVersion.date.Months = (byte)dateDTP.Value.Date.Month;
                    installDataReq.appData.appVersion.date.Days = (ushort)dateDTP.Value.Date.Day;
                    installDataReq.appData.appVersion.major = (ushort)majorNUD.Value;
                    installDataReq.appData.appVersion.minor = (ushort)minorNUD.Value;
                    installDataReq.appData.appVersion.build = (ushort)buildNUD.Value;
                    installDataReq.appData.appDataCrc = Crc.GetCrc32_new(installDataReq.appData.GetBytes().Take(installDataReq.appData.GetBytes().Length - 4).ToArray());

                    GcpMessageHeader messageHeader = new GcpMessageHeader();
                    messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_APP_DATA_REQ_ID;
                    messageHeader.ProtocolVersion = 0;
                    messageHeader.PayloadSize = (ushort)installDataReq.GetBytes().Length;

                    if (GCP.TransmitMessage(0, messageHeader, installDataReq.GetBytes()) != true)
                    {
                        TraceProgressNew_ThreadSafe("Communication error.");
                    }
                    else
                    {
                        TraceProgressNew_ThreadSafe("Install request sent.");

                        if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 10000) != true)
                        {
                            TraceProgressNew_ThreadSafe("Communication error.");
                        }
                        else
                        {
                            TraceProgressNew_ThreadSafe("Application erased.\r\n");
                            installDataResp.GetFromBytes(recvBuf);
                            Thread.Sleep(100);

                            bld_com_install_packet_req_msg_t installPacketRequest = new bld_com_install_packet_req_msg_t();
                            bld_com_install_packet_resp_msg_t installPacketResponse = new bld_com_install_packet_resp_msg_t();
                            UInt32 seqCntr = 1;
                            UInt16 packetSize = 512;
                            UInt32 packetCntr = 0;
                            UInt32 appBytesCntr = 0;

                            while (appBytesCntr < memoryContent.Count)
                            {
                                installPacketRequest.sequenceCounter = seqCntr;

                                if (appBytesCntr + packetSize > memoryContent.Count)
                                {
                                    UInt16 originalPacketSize = packetSize;
                                    packetSize = (UInt16)(memoryContent.Count - appBytesCntr);

                                    installPacketRequest.packetBuffer = memoryContent.Skip((int)(packetCntr * originalPacketSize)).Take(packetSize).ToList();
                                }
                                else
                                {
                                    installPacketRequest.packetBuffer = memoryContent.Skip((int)(packetCntr * packetSize)).Take(packetSize).ToList();
                                }

                                installPacketRequest.packetSize = packetSize;
                                installPacketRequest.packetChk = Crc.GetCrc32_new(installPacketRequest.packetBuffer.Take(installPacketRequest.packetSize).ToArray());

                                appBytesCntr += packetSize;

                                if (messageHeader == null)
                                    messageHeader = new GcpMessageHeader();

                                messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_PACKET_REQ_ID;
                                messageHeader.ProtocolVersion = 0;
                                messageHeader.PayloadSize = (ushort)installPacketRequest.GetBytes().Length;

                                if (GCP.TransmitMessage(0, messageHeader, installPacketRequest.GetBytes()) != true ||
                                    GCP.ReceiveMessage(0, out messageHeader, out recvBuf, 10000) != true)
                                {
                                    TraceProgressNew_ThreadSafe("Communication error.");
                                    installSuccess = false;
                                    break;
                                }
                                else
                                {
                                    installPacketResponse.GetFromBytes(recvBuf);

                                    seqCntr = installPacketResponse.sequenceCounter + 1;

                                    int perc = (int)((double)100 * appBytesCntr / memoryContent.Count);
                                    SetProgressBar_ThreadSafe(perc);
                                    TraceProgressChanging_ThreadSafe("Installing progress: [" + appBytesCntr + " / " + memoryContent.Count + "] bytes");

                                    packetCntr++;
                                    //Thread.Sleep(100);
                                }
                            }

                            if (installSuccess)
                            {
                                TraceProgressNew_ThreadSafe("Application install successful.");
                            }
                        }
                    }
                });
            }
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                appBinaryPathTb.Text = openFileDialog.FileName;
            }
        }

        private void SoftwareInstallWindow_Load(object sender, EventArgs e)
        {
            Text = ProgramData.Name + " " + ProgramData.Version + " - Software Install";
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            // set the current caret position to the end
            richTextBox1.SelectionStart = richTextBox1.Text.Length;
            // scroll it automatically
            richTextBox1.ScrollToCaret();
        }

        private async void bldConnectBtn_Click(object sender, EventArgs e)
        {
            await Task.Run(() =>
            {
                byte[] recvBuf;

                GcpMessageHeader messageHeader = new GcpMessageHeader();
                bld_com_conn_req_msg_t reqMsg = new bld_com_conn_req_msg_t();
                bld_com_conn_resp_msg_t respMsg = new bld_com_conn_resp_msg_t();

                reqMsg.client.name = "gos_monitoring_tool_pc";

                messageHeader = new GcpMessageHeader();
                messageHeader.MessageId = (UInt16)BootloaderMessageId.BLD_MSG_CONN_REQ_ID;
                messageHeader.ProtocolVersion = 0;
                messageHeader.PayloadSize = (ushort)reqMsg.GetBytes().Length;

                Uart.Init(usbConfigUserControl1.Port, usbConfigUserControl1.Baud);

                if (GCP.TransmitMessage(0, messageHeader, reqMsg.GetBytes()) != true)
                {
                    TraceProgressNew_ThreadSafe("Communication error.");
                }
                else
                {
                    if (GCP.ReceiveMessage(0, out messageHeader, out recvBuf) != true)
                    {
                        TraceProgressNew_ThreadSafe("Communication error.");
                    }
                    else
                    {
                        // Connected.
                        TraceProgressNew_ThreadSafe("Connection established.");

                        EnableButton_ThreadSafe(bootModeRequestButton, true);
                        EnableButton_ThreadSafe(resetRequestButton, true);
                        EnableButton_ThreadSafe(disconnectButton, true);
                        EnableButton_ThreadSafe(installButton, true);
                    }
                }
            });
        }
    }
}
