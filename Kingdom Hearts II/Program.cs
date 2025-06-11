using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using KH2FML;

using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;


namespace ReFined.KH2
{
    public partial class Program
    {
        public static void Main(string[] args)
        {
            Boot.Initialization();
            Boot.Execute();
            Console.ReadLine();
        }
    }
}
