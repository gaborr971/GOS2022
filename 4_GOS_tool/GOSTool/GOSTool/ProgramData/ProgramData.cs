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
        public static string Version { get; } = "0.2";
        public static string Date { get; } = "2024-04-09";

        public static string BaseDirectory = AppDomain.CurrentDomain.BaseDirectory;

        public static string OSPath = BaseDirectory + "Lib\\GOS2022";

        public static string EmptyAppPath = BaseDirectory + "Lib\\EmptyApp";

        public static string SourceTemplatePath = BaseDirectory + "Lib\\Templates\\source.c";
        
        public static string HeaderTemplatePath = BaseDirectory + "Lib\\Templates\\header.h";
    }
}
