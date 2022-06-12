/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
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

        public static bool sfxToggle = true;
        public static bool saveToggle = true;
        public static bool discordToggle = true;

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

        public static DiscordRpcClient DiscordClient = new DiscordRpcClient("939407076747272203");

        public static Stream SaveSFX = ExeAssembly.GetManifestResourceStream("sfxSave.wav");
        public static Stream DenySFX = ExeAssembly.GetManifestResourceStream("sfxDeny.wav");
        public static Stream ToggleSFX = ExeAssembly.GetManifestResourceStream("sfxToggle.wav");

        public static string SaveSFXPath = Path.GetTempPath() + "ReFixed/saveSFX.wav";
        public static string DenySFXPath = Path.GetTempPath() + "ReFixed/denySFX.wav";
        public static string ToggleSFXPath = Path.GetTempPath() + "ReFixed/toggleSFX.wav";

        //
        // RPC ASSET LIBRARY
        //
        // Everything DiscordRPC uses (except for the RPC itself) resides here.
        //

        public static string[] MDEDictionary = { "Beginner", "Standard", "Proud", "Critical" };
        public static string[] WRLDictionary = { "", "ex", "sw", "ci", "sb", "tt", "hb", "dw", "he", "st", "di", "nl", "dc", "eh", "", "ar", "ar", "wm", "po" };
        public static string[] CHRDictionary = { "None", "Ventus", "Aqua", "Terra", "Ventus", "Aqua", "Terra", "Ventus", "Aqua", "Terra", "Ventus", "Aqua", "Terra", "Ventus", "Aqua", "Terra" };

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

        public static ulong BASE_ADDRESS = 0x60E334;

        public static ulong ADDR_Input = 0x20EA64;
        public static ulong ADDR_Config = 0x1FDB54;
        public static ulong ADDR_FinisherName = 0x10994128;

        public static ulong ADDR_Limiter = 0x254EDC;
        public static ulong ADDR_Framerate = 0x10ADCEA6;
        public static ulong ADDR_LimiterINST = 0xE2009;

        public static ulong ADDR_VolumeTable = 0x7B5D4;
        public static ulong ADDR_MasterVolume = 0x10ADCEB0;

        public static ulong ADDR_World = 0x1098E9D0;
        public static ulong ADDR_SaveData = 0x1098E9BC;
        public static ulong ADDR_LoadWorld = 0x0022C5CC;
        public static ulong ADDR_LoadFlag = 0x254EE0;
        public static ulong ADDR_BattleFlag = 0x1098CF94;
        public static ulong ADDR_CutsceneFlag = 0x00254ECC;
        public static ulong ADDR_HUDFlag = 0x1098CF95;

        public static ulong ADDR_VoicePath = 0x023394;

        public static ulong PINT_StatusMenu = 0x109A3334;
        public static ulong PINT_CommandMenu = 0x109A333C;
        public static ulong PINT_SettingsText = 0x109794EC;
        public static ulong PINT_SaveInformation = 0x109A387C;

        //
        // INSTRUCTIONS
        // 
        // We store the actual instructions here.
        //

        public static byte[] INST_FrameLimiter = { 0x44, 0x89, 0x15, 0x00, 0x12, 0x78, 0x00 };
    }
}