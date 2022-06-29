/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Media;
using System.Threading;
using System.Reflection;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
    public class Variables
    {
        //
        // COMPILER OPTIONS!
        // 
        // - DualAudio => If set to "true", will cause the Dual Audio feature to activate.
        //

        public const bool DualAudio = true;

        //
        // CONFIG VARIABLES
        //
        // Variables that will be read from a config file to tell Re:Fixed what to do.
        //

        public static bool sfxToggle = true;
        public static bool rpcToggle = true;
        public static bool saveToggle = true;
        
        public static bool devMode = false;

        public static bool chestToggle = true;

        //
        // INFORMATION GRAB
        //
        // Grab informaiton from the EXE direct.
        //

        static Assembly ExeAssembly = Assembly.GetExecutingAssembly();

        //
        // RESOURCE LIBRARY
        //
        // Reserved for static resources, or initialization of APIs
        //

        public static DiscordRpcClient DiscordClient = new DiscordRpcClient("837171155076513834");

        public static Stream SaveSFX = ExeAssembly.GetManifestResourceStream("sfxSave.wav");
        public static Stream DenySFX = ExeAssembly.GetManifestResourceStream("sfxDeny.wav");
        public static Stream ToggleSFX = ExeAssembly.GetManifestResourceStream("sfxToggle.wav");

        public static string SaveSFXPath = Path.GetTempPath() + "ReFixed/saveSFX.wav";
        public static string DenySFXPath = Path.GetTempPath() + "ReFixed/denySFX.wav";
        public static string ToggleSFXPath = Path.GetTempPath() + "ReFixed/toggleSFX.wav";

        public static Dictionary<byte, byte> APDictionary = new Dictionary<byte, byte>()
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
            { 0x19, 0x05 },
            { 0x1A, 0x01 },
            { 0x1B, 0x02 },
            { 0x1C, 0x02 },
            { 0x1D, 0x01 },
            { 0x1E, 0x01 },
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

        //
        // RPC ASSET LIBRARY
        //
        // Everything DiscordRPC uses (except for the RPC itself) resides here.
        //

        public static string[] BTLDictionary = { "safe", "battle" };
        public static string[] WRLDictionary = { "", "di", "", "tt", "wl", "tz", "po", "", "al", "lm", "nm", "he", "pn", "nv", "", "hb", "eh" };
        public static string[] MDEDictionary = { "Beginner", "Standard", "Proud" };

        //
        // ALTERED VARIABLES
        //
        // Variables that can be altered reside here.
        //

        public static bool Initialized = false;

        public static Task DCTask;
        public static Task ASTask;
        public static CancellationToken Token;
        public static CancellationTokenSource Source;

        //
        // STATIC ADDRESSES
        //
        // Addresses that do not need changes regardless of the version.
        //

        public static ulong BASE_ADDRESS = 0x3A0606;

        public static ulong ADDR_Config = 0x2A5B7DA;
        public static ulong ADDR_AudioPath = 0x184EBA;

        public static ulong ADDR_Input = 0x1F9CA2E;
        public static ulong ADDR_Confirm = 0x1F36678;
        public static ulong ADDR_GameState = 0x1372A7;

        public static ulong ADDR_VolumeTable = 0x364B2;
        public static ulong ADDR_MasterVolume = 0x1F3666E;

        public static ulong ADDR_Magic = 0x2A4543E;
        public static ulong ADDR_SoraMP = 0x246E512;

        public static ulong ADDR_SaveData = 0x2A5BFCA;
        public static ulong ADDR_SaveSelect = 0x2A7C552;

        public static ulong ADDR_Time = 0x01F9BC4E;
        public static ulong ADDR_Munny = 0x2A5B7E6;
        public static ulong ADDR_Level = 0x02A453CE;
        public static ulong ADDR_SoraStats = 0x029B8CC6;

        public static ulong ADDR_TitleSelect = 0x2B44FDE;

        public static ulong ADDR_World = 0x01F9C4D6;
        public static ulong ADDR_Difficulty = 0x02A5B7F6;

        public static ulong ADDR_TitleFlag = 0x45E38A;
        public static ulong ADDR_LoadFlag = 0x1F89D62;
        public static ulong ADDR_GummiFlag = 0x00163C17;
        public static ulong ADDR_BattleFlag = 0x024C3352;
        public static ulong ADDR_CutsceneFlag = 0x1F9A86E;

        public static ulong ADDR_Blackness = 0x0138DB2;
        public static ulong ADDR_Whiteness = 0x1F9BE96;

        public static ulong ADDR_Viewport = 0x10F2E;

        public static ulong ADDR_ALLAbility = 0x2A45963;
        public static ulong ADDR_SORAAbility = 0x2A4540E;
        public static ulong ADDR_FRIENDAbility = 0x2A45859;

        public static ulong ADDR_AbilityMenu = 0x2AF8BD2;
        public static ulong ADDR_AbilityPoint = 0x2AF031E;

        public static ulong ADDR_ChestCheck = 0x2B12C4;

        public static ulong[] ADDR_Reset = new ulong[]
        {
            0x1F480D6,
            0x1F480DA,
            0x1F9BC3A
        };

        public static ulong[] ADDR_FieldOfView = new ulong[]
        {
            0x2194066,
            0x2194072,
            0x219406E,
            0x0163352,
            0x0163392,
            0x01633B2,
            0x2A4518E
        };

        public static ulong PINT_MenuState = 0x2AF021A;
        public static ulong PINT_SystemBAR = 0x2A76CCA;
        public static ulong PINT_SaveInformation = 0x2B379CA;

        //
        // VALUE DUMP
        //
        // The values themselves, which will be written to shit, are stored here.
        //

        public static float[] VALUE_DefaultFOV =  { 400, -050, -100, 350, 300, 300, 300, 300 };
        public static float[] VALUE_EnhancedFOV = { 600, -000, -150, 600, 600, 600, 600, 600 };
    }
}
