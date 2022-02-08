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
		public static ulong ExeAddress;

		public static bool Debounce = false;

		public static ulong InputAddress = 0x20EA64;
		public static ulong NameAddress = 0x10994128;
		public static ulong FinisherAddress = 0x109A3334;
		public static ulong FramerateAddress = 0x10ADCEA6;

		public static ulong InstructionAddress = 0xE2009;

		public static ulong BaseAddress = 0x60E334;

		public static byte[] LimiterInstruction = new byte[] { 0x44, 0x89, 0x15, 0x00, 0x12, 0x78, 0x00 };
		public static byte[] LimiterRemoved = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	}
}
