using System.Reflection;

namespace ReFined.COM
{
    public static class Terminal
    {
        public static void Log(string Input, byte Type)
        {
            var _dateStr = DateTime.Now.ToString("dd-MM-yyyy");
            var _timeStr = "[" + DateTime.Now.ToString("hh:mm:ss") + "] ";

            var _typeStr = "";

            switch (Type)
            {
                case 0:
                    _typeStr = "MESSAGE";
                    break;

                case 1:
                    _typeStr = "WARNING";
                    break;

                case 2:
                    _typeStr = "ERROR";
                    break;
            }

            Console.Write(_timeStr);
            Console.ForegroundColor = Type == 0x00 ? ConsoleColor.Green : (Type == 0x01 ? ConsoleColor.Yellow : ConsoleColor.Red);
            Console.Write(_typeStr + ": ");
            Console.ForegroundColor = ConsoleColor.Gray;
            Console.WriteLine(Input);
        }

        public static void Log(Exception Input)
        {
            var _dateStr = DateTime.Now.ToString("dd-MM-yyyy");
            var _timeStr = DateTime.Now.ToString("hh:mm:ss");

            var _exString = Input.ToString().Replace("   ", "").Replace(System.Environment.NewLine, " ");

            Console.Write("[" + _timeStr + "] ");
            Console.ForegroundColor = ConsoleColor.Red;
            Console.Write("EXCEPTION: ");
            Console.ForegroundColor = ConsoleColor.Gray;
            Console.WriteLine(_exString);
        }
    }
}
