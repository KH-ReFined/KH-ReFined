/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
	public partial class Variables
	{
		// Set to 0x01 if playing on v0.1
		public const int Version = 0x00;

		public static readonly DiscordRpcClient RichClient = new DiscordRpcClient("833511404274974740");

		public static readonly string[] BattleImages = new string[] { "safe", "mob", "boss" };
        public static readonly string[] WorldImages = new string[] { "", "", "tt", "", "hb", "bb", "he", "al", "mu", "po", "lk", "lm", "dc", "wi", "nm", "wm", "ca", "tr", "eh" };

		public static readonly string[] FormText = new string[] { "None", "Valor", "Wisdom", "Limit", "Master", "Final", "Anti" };
        public static readonly string[] ModeText = new string[] { "Beginner Mode", "Standard Mode", "Proud Mode", "Critical Mode" };

        public static Process GameProcess;
        public static IntPtr GameHandle;

        public static ulong GameAddress;
		public static ulong ExeAddress;

		public static bool SkipRoxas = false;
		public static bool SkipComplete = false;

		public static bool Debounce = false;

		public static readonly ulong BaseAddress = Version == 0x00 ? 0x56450E : 0x56454E;

		public static readonly ulong InputAddress = Version == 0x00 ? 0x1ACF7B : 0x1ACF3B;
		public static readonly ulong VibrationAddress = 0x55A19E;

		public static readonly ulong ConfirmAddress = Version == 0x00 ? 0x365550 : 0x365520;
		public static readonly ulong FramerateAddress = Version == 0x00 ? 0x36553C : 0x36550C;

		public static readonly ulong LimiterAddress = 0x553EBA;
		
		public static readonly ulong InstructionAddress = Version == 0x00 ? 0x152160 : 0x152220;

		public static readonly ulong[] ConfigTextAddresses = new ulong[] { 0x2565A59, 0x2565C94 };
		public static readonly ulong[] TitleTextAddresses = new ulong[] { 0x256E10A, 0x256E125, 0x256E12C, 0x256E152, 0x256E295 };

		public static readonly ulong TitleBackAddress = 0x553F0C;

		public static readonly ulong TitleFlagAddress = Version == 0x00 ? 0x1B0256 : 0x1B0246;
		public static readonly ulong TitleButtonAddress = 0x255BECE;

		public static byte[] MagicStoreMemory = null;

		public static bool RoomLoad = false;
		public static readonly ulong LoadAddress = 0x453B82;

		public static readonly ulong RoomAddress = Version == 0x00 ? 0x1B08AA : 0x1B086A;
		public static readonly ulong StoryFlagAddress = 0x444832;
		public static readonly ulong DifficultyAddress = 0x444FFA;
		public static readonly ulong InventoryFlagAddress = 0x444F00;

		public static readonly ulong ShortcutStartAddress = Version == 0x00 ? 0x630AA : 0x6306A;

		public static readonly byte[] LimiterInstruction = Version == 0x00 ? new byte[] { 0x89, 0x1D, 0x62, 0x62, 0x96, 0x00 } : new byte[] { 0x89, 0x1D, 0xE2, 0x61, 0x96, 0x00 };
		public static readonly byte[] LimiterRemoved = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

		public static readonly ulong InformationPointer = 0x25A5972;
		public static readonly ulong SaveAddress = 0x442B62;

		public static byte SaveIterator;
		public static byte SaveWorld;
		public static byte SaveRoom;

		public static readonly ulong[] LimitAddresses = new ulong[]
		{
			0x257257C, 0x2571C0D,
			0x2572565, 0x2571B80,
			0x2572534, 0x2571D03
		};

		public static readonly ulong[] SelectAddresses = Version == 0x00 ? 
		
		new ulong[]
		{
			0x3AEAC1,
			0x3AEBA5,
			0x3AEC4C,
			0x3AEBCD,
			0x3AEC1C
		}

		: 
		
		new ulong[]
		{
			0x3AEC01,
			0x3AECE5,
			0x3AED8C,
			0x3AED0D,
			0x3AED5C
		};

		public static readonly byte[][] SelectInstructions = new byte[][]
		{
			new byte[] { 0x89, 0x4B, 0x74 },
			new byte[] { 0x89, 0x53, 0x74 },
			new byte[] { 0x89, 0x7B, 0x74 },
			new byte[] { 0x89, 0x4B, 0x74 },
			new byte[] { 0x89, 0x43, 0x74 }
		};

		public static readonly byte[] StoryFlagArray = new byte[]
		{
			0x01, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 
			0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x05, 0x08, 0x01, 0x00, 0x00, 0x81
		};

		public static readonly string[] TitleStrings = new string[]
		{
			"Play Roxas' Story?",
			"YES{0x00}NO",
			"Play through Roxas' Story normally.",
			"                Skip Roxas' Story entirely.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(You will miss important story elements if you do!)",
			"Roxas' Story"
		};

		public static readonly string[] LimitStrings = new string[]
		{
			"Ars Arcanum",
			"Sonic Blade",
			"Ragnarok"
		};

		public static readonly ulong[] MagicAddresses = new ulong[] { 0x24AA2CA, 0x24AA33A, 0x24A98EE };
	}
}
