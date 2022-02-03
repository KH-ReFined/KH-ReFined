/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace ReFixed
{
	public partial class Variables
	{
        public static Process GameProcess;
        public static IntPtr GameHandle;
        public static ulong GameAddress;

		public static bool SkipRoxas = false;
		public static bool SkipComplete = false;

		public static bool Debounce = false;

		public static ulong BaseAddress = 0x56454E;
		public static ulong InputAddress = 0x01ACF3B;
		public static ulong VibrationAddress = 0x55A15E;

		public static ulong[] ConfigTextAddresses = new ulong[] { 0x2565A19, 0x2565C54 };
		public static ulong[] TitleTextAddresses = new ulong[] { 0x256E0CA, 0x256E0E5, 0x256E0EC, 0x256E112, 0x256E255 };

		public static ulong TitleBackAddress = 0x553ECC;

		public static ulong TitleFlagAddress = 0x1B0216;
		public static ulong TitleButtonAddress = 0x255BE8E;

		public static ulong RoomAddress = 0x1B086A;
		public static ulong StoryFlagAddress = 0x4447F2;
		public static ulong DifficultyAddress = 0x444FBA;
		public static ulong InventoryFlagAddress = 0x444EC0;

		public static byte[] StoryFlagArray = new byte[]
		{
			0x01, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 
			0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x05, 0x08, 0x01, 0x00, 0x00, 0x81
		};

		public static List<string> TitleStrings = new List<string>()
		{
			"Play Roxas' Story?",
			"YES{0x00}NO",
			"Play through Roxas' Story normally.",
			"                Skip Roxas' Story entirely.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(You will miss important story elements if you do!)",
			"Roxas' Story"
		};

		public static ulong[] MagicAddresses = new ulong[] { 0x24AA28A, 0x24AA2FA, 0x24A98AE };
	}
}
