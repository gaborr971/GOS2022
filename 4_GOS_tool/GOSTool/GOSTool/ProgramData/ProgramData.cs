using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GOSTool
{
    public class ProgramData
    {
        public static string Name { get; } = "GOS Tool";
        public static string Version { get; } = "0.1";
        public static string Date { get; } = "2024-04-08";

        public static string BaseDirectory = AppDomain.CurrentDomain.BaseDirectory;

        public static string OSPath = BaseDirectory + "Lib\\GOS2022";
    }
}
