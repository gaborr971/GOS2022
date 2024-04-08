using GOSTool.DataHandler;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GOSTool
{
    public static class Converter
    {
        public static void IsCreateEnabled(DataVariable<string> input1, DataVariable<List<string>> input2, out DataVariable<bool> output)
        {
            output = new DataVariable<bool>(!input2.Value.Contains(input1.Value));
        }

        public static void IsOpenEnabled(DataVariable<string> input1, DataVariable<List<string>> input2, out DataVariable<bool> output)
        {
            output = new DataVariable<bool>(input2.Value.Contains(input1.Value));
        }
    }
}
