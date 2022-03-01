/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Threading;
using System.Reflection;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
	public class Variables
	{
		private static Assembly ExeAssembly = Assembly.GetExecutingAssembly();
		private static FileVersionInfo FileInfo = FileVersionInfo.GetVersionInfo(ExeAssembly.Location);
		private static String FileVersion = FileInfo.FileVersion;

		// Set to true if using Dual Audio
		public const bool DualAudio = true;

		public static bool Initialized = false;
		
		public static Task DiscordTask;
		public static CancellationToken DiscordToken;
		public static CancellationTokenSource CancelSource;

		public static readonly DiscordRpcClient DiscordClient = new DiscordRpcClient("939407076747272203");

	    public static readonly string[] ModeText = { "Beginner", "Standard", "Proud", "Critical" };
        public static readonly string[] WorldImages = { "", "ex", "sw", "ci", "sb", "tt", "hb", "dw", "he", "st", "di", "nl", "dc", "eh", "", "ar", "ar", "wm", "po" };

        public static readonly string[] CharText = 
		{
			"None", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra", 
			"Ventus", "Aqua", "Terra"
		};

		public static readonly string[] AudioText = 
		{
			"Dual-Audio\x00", 
			"English\x00", 
			"Japanese\x00", 
			"Switch to using English vocals.\x0A\u2219\x59(Work in Progress! Trigger a significant load\x000Afor the changes to take effect.)\x00",
			"Switch to using Japanese vocals.\x0A\u2219\x59(Work in Progress! Trigger a significant load\x000Afor the changes to take effect.)\x00"
		};

		public static readonly ulong[] AudioOffsets = 
		{
			0x1C0E,
			0x1C92,
			0x1C88,
			0x2330,
			0x23A1
		};

		
		public static readonly string[] SaveText = 
		{
			"Auto-Save", 
			"On\u0000", 
			"Off\u0000", 
			"Auto-Save", 
			"Auto-Save"
		};

		public static readonly ulong[] SaveOffsets = 
		{
			0x00,
			0x84,
			0x7A,
			0x1140,
			0x115C
		};
		
		public static readonly ulong SettingsPointer = 0x896C7BC;

        public static Process GameProcess;
        public static IntPtr GameHandle;

        public static ulong GameAddress;
		public static ulong ExeAddress;

		public static bool Debounce;

		public static readonly ulong BaseAddress = 0x60E334;

		public static readonly ulong InputAddress = 0x20EA64;
		public static readonly ulong NameAddress = 0x10994128;
		public static readonly ulong StatusAddress = 0x109A3334;
		public static readonly ulong VibrationAddress = 0x1FDB54;
		public static readonly ulong CommandAddress = 0x109A333C;

		public static readonly ulong LimiterAddress = 0x254EDC;
		public static readonly ulong InstructionAddress = 0xE2009;
		public static readonly ulong FramerateAddress = 0x10ADCEA6;

		public static readonly ulong InformationPointer = 0x109A387C;
		public static readonly ulong SaveAddress = 0x1098E9BC;
		public static readonly ulong WorldAddress = 0x1098E9D0;
		public static readonly ulong BattleAddress = 0x1098CF94;

		public static byte SaveIterator;
		public static byte SaveWorld;
		public static byte SaveRoom;

		public static readonly byte[] LimiterInstruction = new byte[] { 0x44, 0x89, 0x15, 0x00, 0x12, 0x78, 0x00 };
		public static readonly byte[] LimiterRemoved = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	}
}