using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
