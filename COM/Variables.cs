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
        // CONFIG VARIABLES
        //
        // Variables that will be read from a config file to tell Re:Fixed what to do.
        //

        public static bool sfxToggle = true;
        public static bool rpcToggle = true;
        public static bool saveToggle = true;

        public static bool devMode = false;
        public static bool festiveToggle = true;

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

        public static DiscordRpcClient DiscordClient = new DiscordRpcClient("837938492976660481");

        public static Stream SaveSFX = ExeAssembly.GetManifestResourceStream("sfxSave.wav");        
        public static string SaveSFXPath = Path.GetTempPath() + "ReFixed/saveSFX.wav";

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

        public static ulong BASE_ADDRESS = 0x4E4660;

        public static ulong ADDR_VolumeTable = 0x44D42;
        public static ulong ADDR_MasterVolume = 0x365516;

        public static ulong ADDR_TitleSelect = 0x5B6896;
        public static ulong ADDR_TitleCount = 0x5B68A2;

        public static ulong ADDR_Reset = 0x5DC634;

        public static ulong ADDR_Input = 0x735E30;
        public static ulong ADDR_Confirm = 0x365520;
    }
}
