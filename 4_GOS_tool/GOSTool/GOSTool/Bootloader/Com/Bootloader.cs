/*using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GOSTool
{
    public enum BootloaderMessageId
    {
        BLD_MSG_UNKNOWN_ID = 0,        //!< Unknown message ID.
        BLD_MSG_CONN_REQ_ID = 0x1011,   //!< todo message ID.
        BLD_MSG_CONN_RESP_ID = 0xA011,   //!< todo response message ID.
        BLD_MSG_DATA_REQ_ID = 0x1020,
        BLD_MSG_DATA_RESP_ID = 0xA020,
        BLD_MSG_APP_DATA_REQ_ID = 0x1030,
        BLD_MSG_APP_DATA_RESP_ID = 0xA030,
        BLD_MSG_PACKET_REQ_ID = 0x1040,
        BLD_MSG_PACKET_RESP_ID = 0xA040,
        BLD_MSG_DISCONN_REQ_ID = 0x1050,
        BLD_MSG_DISCONN_RESP_ID = 0xA050,

        // Custom messages.
        BLD_MSG_SWITCH_TO_BOOT_MODE = 0x9999
    }

    public enum bld_com_conn_result_t
    {
        BLD_COM_CONNECTION_ACCEPTED = 100,
        BLD_COM_CONNECTION_REFUSED = 1,
        BLD_COM_DISCONNECT_ACCEPTED = 2,
        BLD_COM_DISCONNECT_REFUSED = 3,
        BLD_COM_DISCONNECT_TMO = 4
    }

    public enum bld_com_update_t
    {
        BLD_COM_UPDATE_TYPE_INSTALL = 0,
        BLD_COM_UPDATE_TYPE_ERASE = 1
    }

    public enum bld_com_install_req_result_t
    {
        BLD_COM_INSTALL_REQUEST_ACCEPTED = 100,
        BLD_COM_INSTALL_REQUEST_SIZE_ERROR = 1,
        BLD_COM_INSTALL_REQUEST_START_ADDRESS_ERROR = 2,
        BLD_COM_INSTALL_REQUEST_DATA_CRC_ERROR = 3,
        BLD_COM_ERASE_SUCCESSFUL = 101,
        BLD_COM_ERASE_FAILED = 4
    }

    public enum bld_com_install_packet_req_result_t
    {
        BLD_COM_INSTALL_PACKET_ACCEPTED = 100,
        BLD_COM_INSTALL_PACKET_REPEAT = 0
    }
    public class bld_com_client_data_t
    {
        public string name { get; set; }
        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Encoding.ASCII.GetBytes(name));
            for (int i = 0; i < (32 - name.Length); i++)
                bytes.Add(0);
            return bytes.ToArray();
        }
    }

    public class lib_version_t
    {
        public const int ExpectedSize = 6 + Time.ExpectedSize + 48 + 64 + 32;
        public UInt16 major { get; set; } = 0;
        public UInt16 minor { get; set; } = 0;
        public UInt16 build { get; set; } = 0;
        public Time date { get; set; } = new Time();
        public string name { get; set; } = "N/A";
        public string description { get; set; } = "N/A";
        public string author { get; set; } = "N/A";

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(major));
            bytes.AddRange(Helper<UInt16>.GetBytes(minor));
            bytes.AddRange(Helper<UInt16>.GetBytes(build));
            bytes.AddRange(date.GetBytes());
            bytes.AddRange(Encoding.ASCII.GetBytes(name));
            for (int i = 0; i < (48 - name.Length); i++)
                bytes.Add(0);
            bytes.AddRange(Encoding.ASCII.GetBytes(description));
            for (int i = 0; i < (64 - description.Length); i++)
                bytes.Add(0);
            bytes.AddRange(Encoding.ASCII.GetBytes(author));
            for (int i = 0; i < (32 - author.Length); i++)
                bytes.Add(0);
            return bytes.ToArray();
        }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            major = Helper<UInt16>.GetVariable(bytes, ref idx);
            minor = Helper<UInt16>.GetVariable(bytes, ref idx);
            build = Helper<UInt16>.GetVariable(bytes, ref idx);
            date.GetFromBytes(bytes.Skip(idx).Take(Time.ExpectedSize).ToArray());
            idx += Time.ExpectedSize;
            name = Encoding.ASCII.GetString(bytes.Skip(idx).Take(48).ToArray());
            idx += 48;
            description = Encoding.ASCII.GetString(bytes.Skip(idx).Take(64).ToArray());
            idx += 64;
            author = Encoding.ASCII.GetString(bytes.Skip(idx).Take(32).ToArray());
            idx += 32;
        }
    }

    public class bld_app_data_struct_t
    {
        public const int ExpectedSize = 12 + lib_version_t.ExpectedSize + 4;
        public UInt32 appStartAddress { get; set; }
        public UInt32 appSize { get; set; }
        public UInt32 appCrc { get; set; }
        public lib_version_t appVersion { get; set; } = new lib_version_t();
        public UInt32 appDataCrc { get; set; }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt32>.GetBytes(appStartAddress));
            bytes.AddRange(Helper<UInt32>.GetBytes(appSize));
            bytes.AddRange(Helper<UInt32>.GetBytes(appCrc));
            bytes.AddRange(appVersion.GetBytes());
            bytes.AddRange(Helper<UInt32>.GetBytes(appDataCrc));
            return bytes.ToArray();
        }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            appStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
            appSize = Helper<UInt32>.GetVariable(bytes, ref idx);
            appCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
            appVersion.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            appDataCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_data_struct_t
    {
        public const int ExpectedSize = 16 + lib_version_t.ExpectedSize + bld_app_data_struct_t.ExpectedSize + 1 + 4;
        public UInt32 initPattern { get; set; }
        public UInt32 bldStartAddress { get; set; }
        public UInt32 bldSize { get; set; }
        public UInt32 bldCrc { get; set; }
        public lib_version_t bldVersion { get; set; } = new lib_version_t();
        public bld_app_data_struct_t appData { get; set; } = new bld_app_data_struct_t();
        public bool bootUpdateMode { get; set; }
        public UInt32 bldDataCrc { get; set; }

        public byte[] GetBytes()
        {
            return null;
        }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            initPattern = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldSize = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldVersion.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            appData.GetFromBytes(bytes.Skip(idx).Take(bld_app_data_struct_t.ExpectedSize).ToArray());
            idx += bld_app_data_struct_t.ExpectedSize;
            bootUpdateMode = bytes[idx] == 54 ? true : false;
            idx++;
            bldDataCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_data_resp_msg_t
    {
        public lib_version_t drvData { get; set; } = new lib_version_t();
        public bld_data_struct_t bldData { get; set; } = new bld_data_struct_t();

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            drvData.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            bldData.GetFromBytes(bytes.Skip(idx).Take(bld_data_struct_t.ExpectedSize).ToArray());
            idx += bld_data_struct_t.ExpectedSize;
        }
    }

    public class bld_com_conn_req_msg_t
    {
        public bld_com_client_data_t client { get; set; } = new bld_com_client_data_t();

        public byte[] GetBytes()
        {
            return client.GetBytes();
        }
    }

    public class bld_com_conn_resp_msg_t
    {
        public bld_com_conn_result_t result { get; set; }

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            result = (bld_com_conn_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_disconn_req_msg_t
    {
        public UInt16 reason { get; set; }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(reason));
            return bytes.ToArray();
        }
    }

    public class bld_com_disconn_resp_msg_t
    {
        public bld_com_conn_result_t result { get; set; }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            result = (bld_com_conn_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_install_data_req_msg_t
    {
        public bld_com_update_t updateType { get; set; } = new bld_com_update_t();
        public bld_app_data_struct_t appData { get; set; } = new bld_app_data_struct_t();

        public void GetFromBytes(byte[] bytes)
        {

        }
        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.Add((byte)updateType);
            bytes.AddRange(appData.GetBytes());
            return bytes.ToArray();
        }
    }

    public class bld_com_install_data_resp_msg_t
    {
        public bld_com_install_req_result_t result { get; set; }

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            result = (bld_com_install_req_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_install_packet_req_msg_t
    {
        public UInt16 packetSize { get; set; }
        public UInt32 sequenceCounter { get; set; }
        public UInt32 packetChk { get; set; }
        public List<byte> packetBuffer { get; set; } = new List<byte>();

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(packetSize));
            bytes.AddRange(Helper<UInt32>.GetBytes(sequenceCounter));
            bytes.AddRange(Helper<UInt32>.GetBytes(packetChk));
            bytes.AddRange(packetBuffer);
            return bytes.ToArray();
        }
    }

    public class bld_com_install_packet_resp_msg_t
    {
        public UInt32 sequenceCounter { get; set; }
        public bld_com_install_packet_req_result_t result { get; set; }

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            sequenceCounter = Helper<UInt32>.GetVariable(bytes, ref idx);
            result = (bld_com_install_packet_req_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public enum BootloaderMessagePv
    {
        BLD_MSG_UNKNOWN_PV = 1,
        BLD_MSG_PING_REQ_PV = 1,
        BLD_MSG_PING_RESP_PV = 1,
        BLD_MSG_DATA_REQ_PV = 1,
        BLD_MSG_DATA_RESP_PV = 1,
        BLD_MSG_UPDATE_REQ_PV = 1,
        BLD_MSG_UPDATE_RESP_PV = 1,
        BLD_MSG_BINARY_PACKET_SEND_PV = 1,
        BLD_MSG_BINARY_PACKET_ACK_PV = 1,
        BLD_MSG_UPDATE_ABORT_REQ_PV = 1,
        BLD_MSG_UPDATE_ABORT_RESP_PV = 1,
        BLD_MSG_DATA_SET_REQ_PV = 1,
        BLD_MSG_DATA_SET_RESP_PV = 1,
        BLD_MSG_QUIT_REQ_PV = 1,
        BLD_MSG_QUIT_RESP_PV = 1,
    }

    public class VersionInfo
    {
        public int Major { get; set; }
        public int Minor { get; set; }
        public int Build { get; set; }
        public DateTime Date { get; set; } = new DateTime();
        public string Name { get; set; }
        public string Description { get; set; }
        public string Author { get; set; }   
    }

    public class ApplicationData
    {
        public int StartAddress { get; set; }
        public int Size { get; set; }
        public int Crc { get; set; }
        public VersionInfo Version { get; set; } = new VersionInfo();
    }

    public class BootloaderData
    {
        public VersionInfo DriverInfo { get; set; } = new VersionInfo();
        public int StartAddress { get; set; }
        public int Size { get; set; }
        public int Crc { get; set; }
        public VersionInfo BootloaderInfo { get; set; } = new VersionInfo();
        public ApplicationData ApplicationData { get; set; } = new ApplicationData();
        public bool BootUpdateMode { get; set; }
    }

#if false
    public enum BootloaderUpdateType
    {
        BLD_COM_UPDATE_TYPE_INSTALL = 0,
        BLD_COM_UPDATE_TYPE_ERASE = 1
    }

    public class ApplicationData
    {
        public UInt32 AppStartAddress { get; set; }
        public UInt32 AppSize { get; set; }
        public UInt32 AppCrc { get; set; }
        public byte AppMajorVersion { get; set; }
        public byte AppMinorVersion { get; set; }
        public Time AppInstallDate { get; set; }
        public string AppName { get; set; }
        public UInt32 AppDataCrc { get; set; }

        public const int ExpectedSize = 12 + 2 + Time.ExpectedSize + 32 + 4;

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            AppStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
            AppSize = Helper<UInt32>.GetVariable(bytes, ref idx);
            AppCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
            AppMajorVersion = Helper<byte>.GetVariable(bytes, ref idx);
            AppMinorVersion = Helper<byte>.GetVariable(bytes, ref idx);
            AppInstallDate = new Time();
            AppInstallDate.GetFromBytes(bytes.Skip(idx).Take(Time.ExpectedSize).ToArray());
            idx += Time.ExpectedSize;
            AppName = Encoding.ASCII.GetString(bytes.Skip(idx).Take(32).ToArray());
            idx += 32;
            AppDataCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
        }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt32>.GetBytes(AppStartAddress));
            bytes.AddRange(Helper<UInt32>.GetBytes(AppSize));
            bytes.AddRange(Helper<UInt32>.GetBytes(AppCrc));
            bytes.AddRange(Helper<byte>.GetBytes(AppMajorVersion));
            bytes.AddRange(Helper<byte>.GetBytes(AppMinorVersion));
            bytes.AddRange(AppInstallDate.GetBytes());
            bytes.AddRange(Encoding.ASCII.GetBytes(AppName));
            for (int i = 0; i < (32 - AppName.Length); i++)
                bytes.Add(0);
            bytes.AddRange(Helper<UInt32>.GetBytes(AppDataCrc));

            return bytes.ToArray();
        }
    }

    public class BinaryPacketMessage
    {
        public UInt16 PacketId { get; set; }
        public byte[] Bytes { get; set; } = new byte[BinaryPacketSize];

        public const int BinaryPacketSize = 8192;

        public const int BinaryPacketDownSize = 1024;
        
        public const int ExpectedSize = 2 + BinaryPacketSize;

        public const int ExpectedDownSize = 2 + BinaryPacketDownSize;

        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length == ExpectedSize)
            {
                int idx = 0;
                PacketId = Helper<UInt16>.GetVariable(bytes, ref idx);
                Array.Copy(bytes, idx, Bytes, 0, BinaryPacketSize);
            }
        }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(PacketId));
            bytes.AddRange(Bytes.Take(BinaryPacketDownSize));
            return bytes.ToArray();
        }
    }

    public class BinaryPacketDescriptorMessage
    {
        public UInt32 NumberOfPackets { get; set; }

        public const int ExpetedSize = 4;

        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length == ExpetedSize)
            {
                int idx = 0;
                NumberOfPackets = Helper<UInt32>.GetVariable(bytes, ref idx);
            }
        }

        public byte[] GetBytes()
        {
            return Helper<UInt32>.GetBytes(NumberOfPackets);
        }
    }

    public class BootloaderUpdateDataMessage
    {
        public BootloaderUpdateType UpdateType { get; set; }
        public ApplicationData ApplicationData { get; set; }

        public const int ExpectedSize = 1 + ApplicationData.ExpectedSize;

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<byte>.GetBytes((byte)UpdateType));
            bytes.AddRange(ApplicationData.GetBytes());

            return bytes.ToArray();
        }
    }

    public class BootloaderDataMessage
    {
        public UInt32 InitPattern { get; set; }
        public UInt32 BldStartAddress { get; set; }
        public UInt32 BldSize { get; set; }
        public UInt32 BldCrc { get; set; }
        public byte BldMajorVersion { get; set; }
        public byte BldMinorVersion { get; set; }
        public Time BldDate { get; set; }
        public string BldName { get; set; }
        public List<ApplicationData> AppData { get; set; }
        public byte SelectedApp { get; set; }
        public UInt32 BldDataCrc { get; set; }

        public const int ExpectedSize = 16 + 2 + Time.ExpectedSize + 32 + 3 * ApplicationData.ExpectedSize + 1 + 4;

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            if (bytes.Length == ExpectedSize)
            {
                InitPattern = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldSize = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldMajorVersion = Helper<byte>.GetVariable(bytes, ref idx);
                BldMinorVersion = Helper<byte>.GetVariable(bytes, ref idx);
                BldDate = new Time();
                BldDate.GetFromBytes(bytes.Skip(idx).Take(Time.ExpectedSize).ToArray());
                idx += Time.ExpectedSize;
                BldName = Encoding.ASCII.GetString(bytes.Skip(idx).Take(32).ToArray());
                idx += 32;
                AppData = new List<ApplicationData>();

                for (int i = 0; i < 3; i++)
                {
                    AppData.Add(new ApplicationData());
                    AppData[i].GetFromBytes(bytes.Skip(idx).Take(ApplicationData.ExpectedSize).ToArray());
                    idx += ApplicationData.ExpectedSize;
                }

                SelectedApp = Helper<byte>.GetVariable(bytes, ref idx);
                BldDataCrc = Helper<byte>.GetVariable(bytes, ref idx);
            }
        }
    }
#endif
}
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GOSTool
{
    public enum BootloaderMessageId
    {
        BLD_MSG_UNKNOWN_ID = 0,        //!< Unknown message ID.
        BLD_MSG_CONN_REQ_ID = 0x1011,   //!< todo message ID.
        BLD_MSG_CONN_RESP_ID = 0xA011,   //!< todo response message ID.
        BLD_MSG_DATA_REQ_ID = 0x1020,
        BLD_MSG_DATA_RESP_ID = 0xA020,
        BLD_MSG_APP_DATA_REQ_ID = 0x1030,
        BLD_MSG_APP_DATA_RESP_ID = 0xA030,
        BLD_MSG_PACKET_REQ_ID = 0x1040,
        BLD_MSG_PACKET_RESP_ID = 0xA040,
        BLD_MSG_DISCONN_REQ_ID = 0x1050,
        BLD_MSG_DISCONN_RESP_ID = 0xA050,

        // Custom messages.
        BLD_MSG_SWITCH_TO_BOOT_MODE = 0x9999
    }

    public enum bld_com_conn_result_t
    {
        BLD_COM_CONNECTION_ACCEPTED = 100,
        BLD_COM_CONNECTION_REFUSED = 1,
        BLD_COM_DISCONNECT_ACCEPTED = 2,
        BLD_COM_DISCONNECT_REFUSED = 3,
        BLD_COM_DISCONNECT_TMO = 4
    }

    public enum bld_com_update_t
    {
        BLD_COM_UPDATE_TYPE_INSTALL = 0,
        BLD_COM_UPDATE_TYPE_ERASE = 1
    }

    public enum bld_com_install_req_result_t
    {
        BLD_COM_INSTALL_REQUEST_ACCEPTED = 100,
        BLD_COM_INSTALL_REQUEST_SIZE_ERROR = 1,
        BLD_COM_INSTALL_REQUEST_START_ADDRESS_ERROR = 2,
        BLD_COM_INSTALL_REQUEST_DATA_CRC_ERROR = 3,
        BLD_COM_ERASE_SUCCESSFUL = 101,
        BLD_COM_ERASE_FAILED = 4
    }

    public enum bld_com_install_packet_req_result_t
    {
        BLD_COM_INSTALL_PACKET_ACCEPTED = 100,
        BLD_COM_INSTALL_PACKET_REPEAT = 0
    }
    public class bld_com_client_data_t
    {
        public string name { get; set; }
        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Encoding.ASCII.GetBytes(name));
            for (int i = 0; i < (32 - name.Length); i++)
                bytes.Add(0);
            return bytes.ToArray();
        }
    }

    public class lib_version_t
    {
        public const int ExpectedSize = 6 + Time.ExpectedSize + 48 + 64 + 32;
        public UInt16 major { get; set; } = 0;
        public UInt16 minor { get; set; } = 0;
        public UInt16 build { get; set; } = 0;
        public Time date { get; set; } = new Time();
        public string name { get; set; } = "N/A";
        public string description { get; set; } = "N/A";
        public string author { get; set; } = "N/A";

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(major));
            bytes.AddRange(Helper<UInt16>.GetBytes(minor));
            bytes.AddRange(Helper<UInt16>.GetBytes(build));
            bytes.AddRange(date.GetBytes());
            bytes.AddRange(Encoding.ASCII.GetBytes(name));
            for (int i = 0; i < (48 - name.Length); i++)
                bytes.Add(0);
            bytes.AddRange(Encoding.ASCII.GetBytes(description));
            for (int i = 0; i < (64 - description.Length); i++)
                bytes.Add(0);
            bytes.AddRange(Encoding.ASCII.GetBytes(author));
            for (int i = 0; i < (32 - author.Length); i++)
                bytes.Add(0);
            return bytes.ToArray();
        }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            major = Helper<UInt16>.GetVariable(bytes, ref idx);
            minor = Helper<UInt16>.GetVariable(bytes, ref idx);
            build = Helper<UInt16>.GetVariable(bytes, ref idx);
            date.GetFromBytes(bytes.Skip(idx).Take(Time.ExpectedSize).ToArray());
            idx += Time.ExpectedSize;
            name = Encoding.ASCII.GetString(bytes.Skip(idx).Take(48).ToArray());
            idx += 48;
            description = Encoding.ASCII.GetString(bytes.Skip(idx).Take(64).ToArray());
            idx += 64;
            author = Encoding.ASCII.GetString(bytes.Skip(idx).Take(32).ToArray());
            idx += 32;
        }
    }

    public class bld_app_data_struct_t
    {
        public const int ExpectedSize = 16 + 2 * lib_version_t.ExpectedSize + 4;
        public UInt32 initPattern { get; set; }
        public UInt32 appStartAddress { get; set; }
        public UInt32 appSize { get; set; }
        public UInt32 appCrc { get; set; }
        public lib_version_t appDriverVersion { get; set; } = new lib_version_t();
        public lib_version_t appVersion { get; set; } = new lib_version_t();
        public UInt32 appDataCrc { get; set; }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt32>.GetBytes(initPattern));
            bytes.AddRange(Helper<UInt32>.GetBytes(appStartAddress));
            bytes.AddRange(Helper<UInt32>.GetBytes(appSize));
            bytes.AddRange(Helper<UInt32>.GetBytes(appCrc));
            bytes.AddRange(appDriverVersion.GetBytes());
            bytes.AddRange(appVersion.GetBytes());
            bytes.AddRange(Helper<UInt32>.GetBytes(appDataCrc));
            return bytes.ToArray();
        }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            initPattern = Helper<UInt32>.GetVariable(bytes, ref idx);
            appStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
            appSize = Helper<UInt32>.GetVariable(bytes, ref idx);
            appCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
            appDriverVersion.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            appVersion.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            appDataCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_data_struct_t
    {
        public const int ExpectedSize = 16 + 2 * lib_version_t.ExpectedSize + /*bld_app_data_struct_t.ExpectedSize +*/ 1 + 4 + 3;
        public UInt32 initPattern { get; set; }
        public UInt32 bldStartAddress { get; set; }
        public UInt32 bldSize { get; set; }
        public UInt32 bldCrc { get; set; }
        public lib_version_t bldDriverVersion { get; set; } = new lib_version_t();
        public lib_version_t bldVersion { get; set; } = new lib_version_t();
        //public bld_app_data_struct_t appData { get; set; } = new bld_app_data_struct_t();
        public bool bootUpdateMode { get; set; }
        public UInt32 bldDataCrc { get; set; }

        public byte[] GetBytes()
        {
            return null;
        }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            initPattern = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldSize = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
            bldDriverVersion.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            bldVersion.GetFromBytes(bytes.Skip(idx).Take(lib_version_t.ExpectedSize).ToArray());
            idx += lib_version_t.ExpectedSize;
            //appData.GetFromBytes(bytes.Skip(idx).Take(bld_app_data_struct_t.ExpectedSize).ToArray());
            //idx += bld_app_data_struct_t.ExpectedSize;
            bootUpdateMode = bytes[idx] == 54 ? true : false;
            idx++;
            bldDataCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_data_resp_msg_t
    {
        //public lib_version_t drvData { get; set; } = new lib_version_t();        
        public bld_data_struct_t bldData { get; set; } = new bld_data_struct_t();
        public bld_app_data_struct_t appData { get; set; } = new bld_app_data_struct_t();

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            bldData.GetFromBytes(bytes.Skip(idx).Take(bld_data_struct_t.ExpectedSize).ToArray());
            idx += bld_data_struct_t.ExpectedSize;
            appData.GetFromBytes(bytes.Skip(idx).Take(bld_app_data_struct_t.ExpectedSize).ToArray());
            idx += bld_app_data_struct_t.ExpectedSize;
        }
    }

    public class bld_com_conn_req_msg_t
    {
        public bld_com_client_data_t client { get; set; } = new bld_com_client_data_t();

        public byte[] GetBytes()
        {
            return client.GetBytes();
        }
    }

    public class bld_com_conn_resp_msg_t
    {
        public bld_com_conn_result_t result { get; set; }

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            result = (bld_com_conn_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_disconn_req_msg_t
    {
        public UInt16 reason { get; set; }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(reason));
            return bytes.ToArray();
        }
    }

    public class bld_com_disconn_resp_msg_t
    {
        public bld_com_conn_result_t result { get; set; }
        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            result = (bld_com_conn_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_install_data_req_msg_t
    {
        public bld_com_update_t updateType { get; set; } = new bld_com_update_t();
        public bld_app_data_struct_t appData { get; set; } = new bld_app_data_struct_t();

        public void GetFromBytes(byte[] bytes)
        {

        }
        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.Add((byte)updateType);
            bytes.AddRange(appData.GetBytes());
            return bytes.ToArray();
        }
    }

    public class bld_com_install_data_resp_msg_t
    {
        public bld_com_install_req_result_t result { get; set; }

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            result = (bld_com_install_req_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public class bld_com_install_packet_req_msg_t
    {
        public UInt16 packetSize { get; set; }
        public UInt32 sequenceCounter { get; set; }
        public UInt32 packetChk { get; set; }
        public List<byte> packetBuffer { get; set; } = new List<byte>();

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(packetSize));
            bytes.AddRange(Helper<UInt32>.GetBytes(sequenceCounter));
            bytes.AddRange(Helper<UInt32>.GetBytes(packetChk));
            bytes.AddRange(packetBuffer);
            return bytes.ToArray();
        }
    }

    public class bld_com_install_packet_resp_msg_t
    {
        public UInt32 sequenceCounter { get; set; }
        public bld_com_install_packet_req_result_t result { get; set; }

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            sequenceCounter = Helper<UInt32>.GetVariable(bytes, ref idx);
            result = (bld_com_install_packet_req_result_t)Helper<byte>.GetVariable(bytes, ref idx);
        }
    }

    public enum BootloaderMessagePv
    {
        BLD_MSG_UNKNOWN_PV = 1,
        BLD_MSG_PING_REQ_PV = 1,
        BLD_MSG_PING_RESP_PV = 1,
        BLD_MSG_DATA_REQ_PV = 1,
        BLD_MSG_DATA_RESP_PV = 1,
        BLD_MSG_UPDATE_REQ_PV = 1,
        BLD_MSG_UPDATE_RESP_PV = 1,
        BLD_MSG_BINARY_PACKET_SEND_PV = 1,
        BLD_MSG_BINARY_PACKET_ACK_PV = 1,
        BLD_MSG_UPDATE_ABORT_REQ_PV = 1,
        BLD_MSG_UPDATE_ABORT_RESP_PV = 1,
        BLD_MSG_DATA_SET_REQ_PV = 1,
        BLD_MSG_DATA_SET_RESP_PV = 1,
        BLD_MSG_QUIT_REQ_PV = 1,
        BLD_MSG_QUIT_RESP_PV = 1,
    }

    public class VersionInfo
    {
        public int Major { get; set; }
        public int Minor { get; set; }
        public int Build { get; set; }
        public DateTime Date { get; set; } = new DateTime();
        public string Name { get; set; }
        public string Description { get; set; }
        public string Author { get; set; }
    }

    public class ApplicationData
    {
        public int StartAddress { get; set; }
        public int Size { get; set; }
        public int Crc { get; set; }
        public VersionInfo DriverVersion { get; set; } = new VersionInfo();
        public VersionInfo AppVersion { get; set; } = new VersionInfo();
    }

    public class BootloaderData
    {
        public VersionInfo BootloaderDriverInfo { get; set; } = new VersionInfo();
        public int StartAddress { get; set; }
        public int Size { get; set; }
        public int Crc { get; set; }
        public VersionInfo BootloaderInfo { get; set; } = new VersionInfo();
        public ApplicationData ApplicationData { get; set; } = new ApplicationData();
        public bool BootUpdateMode { get; set; }
    }

#if false
    public enum BootloaderUpdateType
    {
        BLD_COM_UPDATE_TYPE_INSTALL = 0,
        BLD_COM_UPDATE_TYPE_ERASE = 1
    }

    public class ApplicationData
    {
        public UInt32 AppStartAddress { get; set; }
        public UInt32 AppSize { get; set; }
        public UInt32 AppCrc { get; set; }
        public byte AppMajorVersion { get; set; }
        public byte AppMinorVersion { get; set; }
        public Time AppInstallDate { get; set; }
        public string AppName { get; set; }
        public UInt32 AppDataCrc { get; set; }

        public const int ExpectedSize = 12 + 2 + Time.ExpectedSize + 32 + 4;

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            AppStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
            AppSize = Helper<UInt32>.GetVariable(bytes, ref idx);
            AppCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
            AppMajorVersion = Helper<byte>.GetVariable(bytes, ref idx);
            AppMinorVersion = Helper<byte>.GetVariable(bytes, ref idx);
            AppInstallDate = new Time();
            AppInstallDate.GetFromBytes(bytes.Skip(idx).Take(Time.ExpectedSize).ToArray());
            idx += Time.ExpectedSize;
            AppName = Encoding.ASCII.GetString(bytes.Skip(idx).Take(32).ToArray());
            idx += 32;
            AppDataCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
        }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt32>.GetBytes(AppStartAddress));
            bytes.AddRange(Helper<UInt32>.GetBytes(AppSize));
            bytes.AddRange(Helper<UInt32>.GetBytes(AppCrc));
            bytes.AddRange(Helper<byte>.GetBytes(AppMajorVersion));
            bytes.AddRange(Helper<byte>.GetBytes(AppMinorVersion));
            bytes.AddRange(AppInstallDate.GetBytes());
            bytes.AddRange(Encoding.ASCII.GetBytes(AppName));
            for (int i = 0; i < (32 - AppName.Length); i++)
                bytes.Add(0);
            bytes.AddRange(Helper<UInt32>.GetBytes(AppDataCrc));

            return bytes.ToArray();
        }
    }

    public class BinaryPacketMessage
    {
        public UInt16 PacketId { get; set; }
        public byte[] Bytes { get; set; } = new byte[BinaryPacketSize];

        public const int BinaryPacketSize = 8192;

        public const int BinaryPacketDownSize = 1024;
        
        public const int ExpectedSize = 2 + BinaryPacketSize;

        public const int ExpectedDownSize = 2 + BinaryPacketDownSize;

        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length == ExpectedSize)
            {
                int idx = 0;
                PacketId = Helper<UInt16>.GetVariable(bytes, ref idx);
                Array.Copy(bytes, idx, Bytes, 0, BinaryPacketSize);
            }
        }

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<UInt16>.GetBytes(PacketId));
            bytes.AddRange(Bytes.Take(BinaryPacketDownSize));
            return bytes.ToArray();
        }
    }

    public class BinaryPacketDescriptorMessage
    {
        public UInt32 NumberOfPackets { get; set; }

        public const int ExpetedSize = 4;

        public void GetFromBytes(byte[] bytes)
        {
            if (bytes.Length == ExpetedSize)
            {
                int idx = 0;
                NumberOfPackets = Helper<UInt32>.GetVariable(bytes, ref idx);
            }
        }

        public byte[] GetBytes()
        {
            return Helper<UInt32>.GetBytes(NumberOfPackets);
        }
    }

    public class BootloaderUpdateDataMessage
    {
        public BootloaderUpdateType UpdateType { get; set; }
        public ApplicationData ApplicationData { get; set; }

        public const int ExpectedSize = 1 + ApplicationData.ExpectedSize;

        public byte[] GetBytes()
        {
            List<byte> bytes = new List<byte>();
            bytes.AddRange(Helper<byte>.GetBytes((byte)UpdateType));
            bytes.AddRange(ApplicationData.GetBytes());

            return bytes.ToArray();
        }
    }

    public class BootloaderDataMessage
    {
        public UInt32 InitPattern { get; set; }
        public UInt32 BldStartAddress { get; set; }
        public UInt32 BldSize { get; set; }
        public UInt32 BldCrc { get; set; }
        public byte BldMajorVersion { get; set; }
        public byte BldMinorVersion { get; set; }
        public Time BldDate { get; set; }
        public string BldName { get; set; }
        public List<ApplicationData> AppData { get; set; }
        public byte SelectedApp { get; set; }
        public UInt32 BldDataCrc { get; set; }

        public const int ExpectedSize = 16 + 2 + Time.ExpectedSize + 32 + 3 * ApplicationData.ExpectedSize + 1 + 4;

        public void GetFromBytes(byte[] bytes)
        {
            int idx = 0;
            if (bytes.Length == ExpectedSize)
            {
                InitPattern = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldStartAddress = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldSize = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldCrc = Helper<UInt32>.GetVariable(bytes, ref idx);
                BldMajorVersion = Helper<byte>.GetVariable(bytes, ref idx);
                BldMinorVersion = Helper<byte>.GetVariable(bytes, ref idx);
                BldDate = new Time();
                BldDate.GetFromBytes(bytes.Skip(idx).Take(Time.ExpectedSize).ToArray());
                idx += Time.ExpectedSize;
                BldName = Encoding.ASCII.GetString(bytes.Skip(idx).Take(32).ToArray());
                idx += 32;
                AppData = new List<ApplicationData>();

                for (int i = 0; i < 3; i++)
                {
                    AppData.Add(new ApplicationData());
                    AppData[i].GetFromBytes(bytes.Skip(idx).Take(ApplicationData.ExpectedSize).ToArray());
                    idx += ApplicationData.ExpectedSize;
                }

                SelectedApp = Helper<byte>.GetVariable(bytes, ref idx);
                BldDataCrc = Helper<byte>.GetVariable(bytes, ref idx);
            }
        }
    }
#endif
}
