using KH2FML;
using DiscordClient = Discord.Discord;

namespace ReFined
{
    public class Locals : Variables
    {
        public static string PLATFORM = "STEAM";

        public static bool IS_LITE;
        public static int SAVE_MODE;
        public static int SAVE_SLOT;
        public static bool AUTOATTACK;
        public static bool MIRAGE_ARENA;
        public static bool RANDOMIZER;
        public static bool RPC_ENABLED;
        public static int AUDIO_MODE;
        public static bool COMBAT_MODE;
        public static bool MUSIC_MODE;
        public static bool ENEMY_MODE;
        public static bool ENEMY_LOADING;
        public static bool CONTROLLER_MODE;
        public static bool IS_FLASHBANG = false;
        public static bool FORM_SHORTCUT = true;
        public static bool RETRY_FIRST = true;
        public static bool RESET_PROMPT = true;
        public static bool CONFIG_RELOADING;
        public static bool DETECTED_RETRIBUTION;
        public static bool DETECTED_ABSOLUTION;
        public static bool INITIALIZED;
        public static BUTTON RESET_COMBO = BUTTON.L2 | BUTTON.R2;

        public static Dictionary<string, int> LOADED_LANGS = new Dictionary<string, int>();

        public static Intro MAIN_INTRO;
        public static Config MAIN_CONFIG;
        public static Continue MAIN_CONTINUE;

        public static Intro.Entry? AUDIO_SUB_INTRO;
        public static Config.Entry? AUDIO_SUB_CONFIG;

        public static bool AUDIO_SUB_ONLY;

        public static string[] SORA_FORMS = ["P_EX100", "P_EX100_BTLF", "P_EX100_MAGF", "P_EX100_KH1F", "P_EX100_TRIF", "P_EX100_ULTF", "P_EX100_HTLF"];

        public static short[] OBJENTRY_SUMMON =
        {
            0x045B,
            0x06C8,
            0x07F5,
            0x030A
        };

        public static short[] OBJENTRY_BOSS =
        {
            0x0161,
            0x088A,
            0x0190,
            0x0166,
            0x0607,
            0x08DB,
            0x0459,
            0x08C6,
            0x0165,
            0x08DD,
            0x05D0,
            0x08DC,
        };

        public static short[] OBJENTRY_ENEMY =
        {
            0x012D,
            0x072E,
            0x012F,
            0x0001,
            0x004A,
            0x0077,
            0x004A,
            0x0002,
            0x0888,
            0x0723,
            0x0003,
            0x0004,
            0x075F,
            0x0005,
            0x0006,
            0x0007,
            0x0008,
            0x0009,
            0x004B,
            0x000B,
            0x06E0,
            0x000C,
            0x000D,
            0x000E,
            0x000F,
            0x0011,
            0x0012,
            0x0729,
            0x00A7,
            0x0013,
            0x007B,
            0x007C,
            0x007D,
            0x072D,
            0x0014,
            0x081C,
            0x06F0,
            0x06F1,
            0x0889,
            0x06EE,
            0x06EF
        };

        public static Dictionary<ushort, short> KEY_DICTIONARY = new Dictionary<ushort, short>()
        {
            { 0x0021, 0x0029 },
            { 0x0022, 0x002A },
            { 0x0023, 0x002B },
            { 0x00FB, 0x01E0 },
            { 0x00FC, 0x01E1 },
            { 0x00FF, 0x01E4 },
            { 0x0100, 0x01E5 },
            { 0x0101, 0x01E6 },
            { 0x0102, 0x01E7 },
            { 0x0103, 0x01E8 },
            { 0x0104, 0x01E9 },
            { 0x0105, 0x01EA },
            { 0x0106, 0x01EB },
            { 0x0107, 0x01EC },
            { 0x0108, 0x01ED },
            { 0x0109, 0x01EE },
            { 0x010A, 0x01EF },
            { 0x010B, 0x01F0 },
            { 0x010C, 0x01F1 },
            { 0x010D, 0x01F2 },
            { 0x010E, 0x01F3 },
            { 0x010F, 0x01F4 },
            { 0x0118, 0x021F },
            { 0x0119, 0x0220 },
        };

        public static DiscordClient DiscordClient;

        public static List<string> RPC_TEXTS;
        public static List<string> MODE_TEXTS;
        public static List<string> FORM_TEXTS;

        public static string[] BATTLE_DICT = { "safe", "mob", "boss" };
        public static string[] WORLD_DICT = { "", "", "tt", "", "hb", "bb", "he", "al", "mu", "po", "lk", "lm", "dc", "wi", "nm", "wm", "ca", "tr", "eh" };
        public static string[] CUP_DICT = { "cup_pp", "cup_cerb", "cup_titan", "cup_god", "cup_hades" };

        public static Dictionary<string, short> DICTIONARY_LMT = new Dictionary<string, short>()
        {
            { "ragnarok", 0x02AB },
            { "arcanum", 0x02BD },
            { "raid", 0x02C0 },
            { "sonic", 0x02BA }
        };

        public new enum CONFIG : ushort
        {
            OFF = 0x0000,
            VIBRATION = 0x0001,
            AUTOSAVE_SILENT = 0x0002,
            AUTOSAVE_INDICATOR = 0x0004,
            NAVI_MAP = 0x0008,
            FIELD_CAM = 0x0010,
            RIGHT_STICK = 0x0020,
            COMMAND_KH1 = 0x0040,
            CAMERA_H = 0x0080,
            CAMERA_V = 0x0100,
            SUMMON_PARTIAL = 0x0200,
            SUMMON_FULL = 0x0400,
            AUDIO_PRIMARY = 0x0800,
            AUDIO_SECONDARY = 0x1000,
            PROMPT_CONTROLLER = 0x2000,
            BACKGROUND_MUSIC = 0x4000,
            HEARTLESS_PALETTE = 0x8000
        }
    }
}
