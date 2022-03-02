/*
===================================================
      KINGDOM HEARTS, RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK, 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
===================================================
*/

using System;
using System.Threading;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
    public class Variables
    {
        public static bool Initialized = false;

        public static Task DiscordTask;
        public static CancellationToken DiscordToken;
        public static CancellationTokenSource CancelSource;

        public static DiscordRpcClient DiscordClient = new DiscordRpcClient("837171155076513834");

        public static string[] ModeText = { "Beginner", "Standard", "Proud" };
        public static string[] WorldImages =
        {
            "",
            "di",
            "",
            "tt",
            "wl",
            "tz",
            "po",
            "",
            "al",
            "lm",
            "nm",
            "he",
            "pn",
            "nv",
            "",
            "hb",
            "eh"
        };

        public static Process GameProcess;
        public static IntPtr GameHandle;

        public static ulong GameAddress;
        public static ulong ExeAddress;

        public static ulong BaseAddress = 0x3A0606;
        public static ulong VibrationAddress = 0x2A5B7DA;

        public static ulong InputAddress = 0x1F9CA2E;

        public static ulong SoraMPAddress = 0x246E512;
        public static ulong SoraMagicAddress = 0x2A4543E;

        public static ulong SaveMenuSelect = 0x2A7C552;
        public static ulong[] ResetAddresses = new ulong[] { 0x1F480D6, 0x1F480DA, 0x1F9BC3A };

        public static ulong[] FovTextAddresses = new ulong[] { 0x2A76F66, 0x2A78F86 };
        public static ulong[] CamTextAddresses = new ulong[] { 0x2A76F6E, 0x2A79D4D };

        public static ushort[] FovTextOffsets = new ushort[] { 0x1EBA, 0x1EC8, 0x1ED0, 0x1ED9 };
        public static ushort[] CamTextOffsets = new ushort[] { 0x2C88, 0x2C8F };

        public static float[] FovClassic = new float[] { 400, -50, -100, 350, 300, 300, 300, 300 };
        public static float[] FovEnhanced = new float[] { 600, 0, -150, 600, 600, 600, 600, 600 };

        public static ulong[] FovAddresses = new ulong[]
        {
            0x2194066,
            0x2194072,
            0x219406E,
            0x0163352,
            0x0163372,
            0x0163392,
            0x01633B2,
            0x2A4518E
        };

        public static bool AbilityBool = false;
        public static ulong PartyStart = 0x2A45859;
        public static ulong SharedStart = 0x2A45963;
        public static ulong AbilityStart = 0x2A4540E;
        public static ulong AbilityMenuStart = 0x2AF8BD2;
        public static ulong AbilityPointAddress = 0x2A453D3;

        // Look, I know this is dumb but what can I do.
        public static Dictionary<byte, byte> DictionaryAP = new Dictionary<byte, byte>()
        {
            { 0x01, 0x00 },
            { 0x02, 0x00 },
            { 0x03, 0x00 },
            { 0x04, 0x00 },
            { 0x05, 0x02 },
            { 0x06, 0x01 },
            { 0x07, 0x01 },
            { 0x08, 0x02 },
            { 0x09, 0x03 },
            { 0x0A, 0x01 },
            { 0x0B, 0x03 },
            { 0x0C, 0x04 },
            { 0x0D, 0x03 },
            { 0x0E, 0x04 },
            { 0x0F, 0x01 },
            { 0x10, 0x01 },
            { 0x11, 0x01 },
            { 0x12, 0x02 },
            { 0x13, 0x02 },
            { 0x14, 0x02 },
            { 0x15, 0x02 },
            { 0x16, 0x01 },
            { 0x17, 0x03 },
            { 0x18, 0x03 },
            { 0x19, 0x02 },
            { 0x1A, 0x01 },
            { 0x1B, 0x02 },
            { 0x1C, 0x02 },
            { 0x1D, 0x01 },
            { 0x1E, 0x02 },
            { 0x1F, 0x02 },
            { 0x20, 0x03 },
            { 0x21, 0x02 },
            { 0x22, 0x02 },
            { 0x23, 0x03 },
            { 0x24, 0x03 },
            { 0x25, 0x02 },
            { 0x26, 0x02 },
            { 0x27, 0x01 },
            { 0x28, 0x02 },
            { 0x29, 0x02 },
            { 0x2A, 0x03 },
            { 0x2B, 0x03 },
            { 0x2C, 0x02 },
            { 0x2D, 0x04 },
            { 0x2E, 0x03 },
            { 0x2F, 0x02 },
            { 0x30, 0x03 },
            { 0x31, 0x01 },
            { 0x32, 0x03 },
            { 0x33, 0x03 },
            { 0x34, 0x03 },
            { 0x35, 0x01 },
            { 0x36, 0x01 },
            { 0x37, 0x02 },
            { 0x38, 0x03 },
            { 0x39, 0x02 },
            { 0x3A, 0x02 },
            { 0x3B, 0x02 },
            { 0x3C, 0x02 },
            { 0x3D, 0x01 },
            { 0x3E, 0x05 },
            { 0x3F, 0x03 },
            { 0x40, 0x00 },
            { 0x41, 0x03 }
        };

        public static ulong InformationPointer = 0x2B379CA;
        public static ulong SaveAddress = 0x2A5BFCA;

        public static ulong LevelAddress = 0x02A453CE;
        public static ulong DifficultyAddress = 0x02A5B7F6;
        public static ulong WorldAddress = 0x01F9C4D6;
        public static ulong TimeAddress = 0x01F9BC4E;
        public static ulong MunnyAddress = 0x2A5B7E6;

        public static byte SaveIterator;
        public static byte SaveWorld;
        public static byte SaveRoom;

        public static string FovTextString = "Field of View{0x00}Classic{0x00}Enhanced{0x00}Toggle between Field of View modes.";
        public static string CamTextString = "X-Axis{0x00}Y-Axis";
    }
}
