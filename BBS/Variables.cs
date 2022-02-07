/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
	public partial class Variables
	{
		public static DiscordRpcClient RichClient = new DiscordRpcClient("939407076747272203");

	    public static string[] ModeText = new string[] { "Beginner", "Standard", "Proud", "Critical" };
        public static string[] WorldImages = new string[] { "", "ex", "sw", "ci", "sb", "tt", "hb", "dw", "he", "st", "di", "nl", "dc", "eh", "", "ar", "ar", "wm", "po" };

        public static string[] CharText = 
		{
			"None", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra"
		};

        public static Process GameProcess;
        public static IntPtr GameHandle;
        public static ulong GameAddress;

		public static bool Debounce = false;

		public static ulong InputAddress = 0x20EA64;
		public static ulong NameAddress = 0x10994128;
		public static ulong FinisherAddress = 0x109A3334;

		public static ulong BaseAddress = 0x60E334;
	}
}
