using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DiscordClient = Discord.Discord;

namespace ReFined.COM
{
    public static class Variables
    {
        public static DiscordClient DiscordClient;

        public static ulong ADDR_Area = 0x87B862;
        public static ulong ADDR_Reset = 0xAC4474;
        public static ulong ADDR_BattleFlag = 0x87B858;
        public static ulong ADDR_LoadFlag = 0xAB590B;

        public static ulong PINT_Input = 0xAC5D40;
        public static ulong PINT_RoomInfo = 0x87CB38;
        public static ulong PINT_PlayerInfo = 0x87B380;

        public static ulong PINT_SaveTable1 = 0x87B0E0;
        public static ulong PINT_SaveTable2 = 0x87B0E8;

        public static string PLATFORM = "STEAM";

        public static bool QUICK_BOOT = false;
        public static BUTTON RESET_COMBO = BUTTON.L2 | BUTTON.R2;

        public static bool RPC_ACTIVE = true;
        public static bool IS_ZEROEXP = false;
        public static bool IS_ALTERNATIVE_BUTTONS = false;
        public static bool IS_TITLE => Hypervisor.Read<byte>(ADDR_Area) == 0xFF;
        public static bool IS_LOADED => Hypervisor.Read<byte>(ADDR_LoadFlag) == 0x01;

        public static string[] BATTLE_DICT = { "safe", "battle" };
        public static string[] WORLD_DICT = { "wm", "tr", "al", "he", "wl", "pn", "nm", "lm", "pp", "hb", "po", "tt", "di", "eh" };

        public enum BUTTON : ushort
        {
            NONE = 0x0000,
            SELECT = 0x0100,
            START = 0x0800,
            TRIANGLE = 0x0010,
            CIRCLE = 0x0020,
            CROSS = 0x0040,
            SQUARE = 0x0080,
            L1 = 0x0004,
            R1 = 0x0008,
            L2 = 0x0001,
            R2 = 0x0002,
            L3 = 0x0200,
            R3 = 0x0400,
            UP = 0x1000,
            RIGHT = 0x2000,
            DOWN = 0x4000,
            LEFT = 0x8000
        }
    }
}
