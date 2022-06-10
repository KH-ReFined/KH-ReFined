/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
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

        public const bool DualAudio = false;

        //
        // CONFIG VARIABLES
        //
        // Variables that will be read from a config file to tell Re:Fixed what to do.
        //

        public static bool saveToggle = true;
        public static bool sfxToggle = true;

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

        public static DiscordRpcClient DiscordClient = new DiscordRpcClient("833511404274974740");

        public static Stream SaveSFX = ExeAssembly.GetManifestResourceStream("sfxSave.wav");
        public static Stream SwitchSFX = ExeAssembly.GetManifestResourceStream("sfxSwitch.wav");

        public static string[] FRIENDObjentry = { "P_EX020{0}", "P_EX030{0}" };
        public static string[] SORAObjentry = { "P_EX100{0}", "P_EX100{0}_BTLF", "P_EX100{0}_MAGF", "P_EX100{0}_TRIF", "P_EX100{0}_ULTF", "P_EX100{0}_HTLF" };

        public static string SaveSFXPath = Path.GetTempPath() + "ReFixed/saveSFX.wav";
        public static string SwitchSFXPath = Path.GetTempPath() + "ReFixed/switchSFX.wav";

        //
        // RPC ASSET LIBRARY
        //
        // Everything DiscordRPC uses (except for the RPC itself) resides here.
        //

        public static string[] BTLDictionary = { "safe", "mob", "boss" };
        public static string[] WRLDictionary = { "", "", "tt", "", "hb", "bb", "he", "al", "mu", "po", "lk", "lm", "dc", "wi", "nm", "wm", "ca", "tr", "eh" };
        public static string[] FRMDictionary = { "None", "Valor", "Wisdom", "Limit", "Master", "Final", "Anti" };
        public static string[] MDEDictionary = { "Beginner Mode", "Standard Mode", "Proud Mode", "Critical Mode" };

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
        // ADDRESSES
        //
        // All of the necessary address values.
        //

        public static ulong BASE_ADDRESS = 0x56454E;

        public static ulong ADDR_MagicLV1 = 0x4460F6;
        public static ulong ADDR_MagicLV2 = 0x446131;

        public static ulong ADDR_DeadSora = 0x553FC6;

        public static ulong ADDR_SoraHP = 0x024BC74A;
        public static ulong ADDR_SoraForm = 0x0446086;

        public static ulong ADDR_ContText = 0x2562CA2;
        public static ulong ADDR_ItemStart = 0x0445086;
        public static ulong ADDR_DriveStart = 0x24BC8FA;

        public static ulong[] ADDR_Objentry = { 0x24BFA72, 0x24BFD72, 0x24E7852 };
        public static ulong[] ADDR_MagicMenu = { 0x24AA2CA, 0x24AA33A, 0x24A98EE };
        public static ulong[] ADDR_MPSEQD = { 0x4A03C6, 0x4A0376, 0x4A035A, 0x4A038A };

        public static ulong ADDR_Reset = 0x553F0C;
        public static ulong ADDR_Config = 0x446D06;
        public static ulong ADDR_LoadFlag = 0x453B82;
        public static ulong ADDR_PauseFlag = 0x554092;
        public static ulong ADDR_StoryFlag = 0x444832;
        public static ulong ADDR_Difficulty = 0x444FFA;
        public static ulong ADDR_BattleFlag = 0x24AA5B6;
        public static ulong ADDR_InventoryFlag = 0x444F00;

        public static ulong ADDR_MenuSelect = 0x39C242;
        public static ulong ADDR_MenuCount = 0x39C258;

        public static ulong ADDR_Limiter = 0x553EBA;
        public static ulong ADDR_FinishFlag = 0x572672;

        public static ulong ADDR_SaveData = 0x442B62;
        public static ulong ADDR_InvMemory = 0x2496C22;

        public static ulong ADDR_Input = 0x1ACF3B;
        public static ulong ADDR_Confirm = 0x365520;
        public static ulong ADDR_Framerate = 0x36550C;

        public static ulong ADDR_PAXFormatter = 0x61F92;
        public static ulong ADDR_BTLFormatter = 0x5F83A;
        public static ulong ADDR_ANBFormatter = 0x529C2;
        public static ulong ADDR_EVTFormatter = 0x52A32;

        public static ulong ADDR_Title = 0x1B0216;
        public static ulong ADDR_World = 0x1B086A;

        public static ulong ADDR_CutsceneFlag = 0x1C1C52;
        public static ulong ADDR_LimitShortcut = 0x06306A;

        public static ulong PINT_SystemBAR = 0x24AA82A;
        public static ulong PINT_DeadMenu = 0x68863A;
        public static ulong PINT_SaveInformation = 0x25A5972;

        //
        // INSTRUCTION ADDRESSES
        //
        // Addresses for instructions are here.
        //

        public static ulong ADDR_LimiterINST = 0x152220;
        public static ulong ADDR_WarpINST = 0x150782;
        public static ulong ADDR_RevertINST = 0x39D8D6;
        public static ulong ADDR_InventoryINST = 0x39D8EF;
        public static ulong[] ADDR_CMDSelectINST = { 0x3AEC01, 0x3AECE5, 0x3AED8C, 0x3AED0D, 0x3AED5C };

        //
        // INSTRUCTIONS
        // 
        // We store the actual instructions here.
        //

        public static byte[] INST_FrameLimiter = { 0x89, 0x1D, 0xE2, 0x61, 0x96, 0x00 };
        public static byte[] INST_RoomWarp = { 0xE8, 0x59, 0x00, 0x00, 0x00 };
        public static byte[] INST_FlagRevert = { 0xE8, 0x05, 0x01, 0x00, 0x00 };
        public static byte[] INST_InvRevert = { 0xE8, 0xA4, 0x1A, 0x0D, 0x00};

        public static byte[][] INST_CMDSelect = 
        {
            new byte[] { 0x89, 0x4B, 0x74 },
            new byte[] { 0x89, 0x53, 0x74 },
            new byte[] { 0x89, 0x7B, 0x74 },
            new byte[] { 0x89, 0x4B, 0x74 },
            new byte[] { 0x89, 0x43, 0x74 }
        };

        //
        // VALUE DUMP
        //
        // The values themselves, which will be written to shit, are stored here.
        //

        public static byte[] VALUE_MPSEQD = { 0x7A, 0x78, 0x18, 0x79 };
        public static byte[] VALUE_StoryFlag = { 0x01, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x05, 0x08, 0x01, 0x00, 0x00, 0x81 };
    }
}
