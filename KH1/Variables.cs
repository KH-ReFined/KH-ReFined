/*
===================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
===================================================
*/

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
	public partial class Variables
	{
		public static readonly DiscordRpcClient RichClient = new DiscordRpcClient("837171155076513834");

	    public static readonly string[] ModeText = new string[] { "Beginner", "Standard", "Proud" };
        public static readonly string[] WorldImages = new string[] { "", "di", "", "tt", "wl", "tz", "po", "", "al", "lm", "nm", "he", "pn", "nv", "", "hb", "eh" };
       
        public static Process GameProcess;
        public static IntPtr GameHandle;

        public static ulong GameAddress;
		public static ulong ExeAddress;

		public static readonly ulong BaseAddress = 0x3A0606;
		public static readonly ulong VibrationAddress = 0x2A5B7DA;

		public static readonly ulong InputAddress = 0x1F9CA2E;

		public static readonly ulong SoraMPAddress = 0x246E512;
		public static readonly ulong SoraMagicAddress = 0x2A4543E;
		
		public static readonly ulong SaveMenuSelect = 0x2A7C552; 
		public static readonly ulong[] ResetAddresses = new ulong[] { 0x1F480D6, 0x1F480DA, 0x1F9BC3A };

		public static readonly ulong SaveTextAddress = 0x2A78F86;

		public static readonly ulong InformationPointer = 0x2B379CA;
		public static readonly ulong SaveAddress = 0x2A5BFCA;

		public static readonly ulong LevelAddress = 0x02A453CE;
		public static readonly ulong DifficultyAddress = 0x02A5B7F6;
		public static readonly ulong WorldAddress = 0x01F9C4D6;
		public static readonly ulong TimeAddress = 0x01F9BC4E;
		public static readonly ulong MunnyAddress = 0x2A5B7E6;

		public static byte SaveIterator;
		public static byte SaveWorld;
		public static byte SaveRoom;

		public static readonly string SaveTextString = "Auto-Save{0x00}On{0x00}Off{0x00}Turn Auto-Save on or off.";
	}
}
