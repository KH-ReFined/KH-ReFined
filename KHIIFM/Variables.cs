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
		public const int Version = 0;

        public static Process GameProcess;
        public static IntPtr GameHandle;
        public static ulong GameAddress;

		public static bool SkipRoxas = false;
		public static bool SkipComplete = false;

		public static bool Debounce = false;

		public static ulong BaseAddress = Version == 0x00 ? 0x56450E : 0x56454E;

		public static ulong InputAddress = 0x1ACF7B;
		public static ulong VibrationAddress = 0x55A19E;

		public static ulong FramerateAddress = 0x36553C;
		public static ulong LimiterAddress = 0x553EBA;
		
		public static ulong InstructionAddress = 0x152160;

		public static ulong[] ConfigTextAddresses = new ulong[] { 0x2565A59, 0x2565C94 };
		public static ulong[] TitleTextAddresses = new ulong[] { 0x256E10A, 0x256E125, 0x256E12C, 0x256E152, 0x256E295 };

		public static ulong TitleBackAddress = 0x553F0C;

		public static ulong TitleFlagAddress = 0x1B0256;
		public static ulong TitleButtonAddress = 0x255BECE;

		public static byte[] MagicStoreMemory = null;

		public static bool RoomLoad = false;
		public static ulong LoadAddress = 0x453B82;

		public static ulong RoomAddress = 0x1B086A;
		public static ulong StoryFlagAddress = 0x444832;
		public static ulong DifficultyAddress = 0x444FFA;
		public static ulong InventoryFlagAddress = 0x444F00;

		public static byte[] LimiterInstruction = new byte[] { 0x89, 0x1D, 0x62, 0x62, 0x96, 0x00 };
		public static byte[] LimiterRemoved = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

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

		public static ulong[] MagicAddresses = new ulong[] { 0x24AA2CA, 0x24AA33A, 0x24A98EE };
	}
}
