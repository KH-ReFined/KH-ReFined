using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Security.Cryptography;

using Octokit;
using DiscordRPC;

using Binarysharp.MSharp;
using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;
using System.Diagnostics;

namespace ReFined
{
    public class Functions
    {
        #region Variable Space

        static bool[] DEBOUNCE = new bool[0x20];
        static ulong ULONG_MINIMAL = 0xFFFFFFFFFFFFFFFF;

        static List<ulong?> MAGIC_MEMORY = new List<ulong?>();
        static uint MAGIC_OLD = 0x00;

        static short[] LIMIT_SHORT;

        static uint MAGIC_LV1;
        static ushort MAGIC_LV2;
        static byte[] MAGIC_STORE;

        static bool ROOM_LOADED;

        static ushort SAVE_ROOM;
        static ushort SAVE_WORLD;
        static byte SAVE_ITERATOR;

        static byte CONFIG_COUNT;
        static bool ENTER_CONFIG;
        static bool LOADED_SETTINGS;
        static bool DETECTED_EXTRA_DUB;
        static byte[] AREA_READ;
        static uint[] CONFIG_REMOVE = new uint[3];

        static bool SKIP_ROXAS;
        static byte SKIP_STAGE;

        static byte HADES_COUNT;

        static bool IS_RANDO;
        static bool CONFIG_WRITTEN;
        static Variables.CONFIG_BITWISE CONFIG_BIT;

        static bool LOCK_AUTOSAVE;

        static List<byte> SETTINGS_FIRST;
        static byte[] SETTINGS_SECOND;

        static ulong[] LOAD_LIST;
        static int PAST_FORM = -1;

        static byte RETRY_MODE;
        static bool RETRY_BLOCK;
        static bool STATE_COPIED;
        static byte PREPARE_MODE;
        static ushort ROXAS_KEYBLADE;

        static int RATIO_DETECT;
        static float PAST_WIDTH;
        static float PAST_HEIGHT;

        static bool SAVE_RESET;

        static bool PAST_MUSIC;
        static bool PAST_ENEMY;

        static byte[] OBJENTRY_READ;

        static string WL_SUFF;
        static string US_SUFF;
        static string FM_SUFF;

        static bool IS_PIRATE;
        static string EPIC_ID = "1638";

        static bool DISABLE_SKIP;

        #endregion

        /// <summary>
        /// Re:Fined's Initialization Function. Must be executed before everything else.
        /// </summary>
        public static void Initialization()
        {
            try
            {
                Helpers.InitConfig();

                Helpers.Log("Initializing Re:Fined...", 0);

                ControllerIO.Initialize();

                // Open the config file for game-specific configs.
                var _configIni = new TinyIni("reFined.ini");

                Helpers.Log("Initializing SharpHook...", 0);

                Variables.SharpHook = new MemorySharp(Hypervisor.Process);

                Helpers.Log("Initializing Configuration...", 0);

                // Parse the Festive Toggle, and the chosen Limit Form shortcuts, default Retry Setting, and the Drive Shortcut setting.

                Variables.FORM_SHORTCUT = Convert.ToBoolean(_configIni.Read("driveShortcuts", "Kingdom Hearts II"));
                Variables.RESET_PROMPT = Convert.ToBoolean(_configIni.Read("resetPrompt", "Kingdom Hearts II"));
                Variables.RATIO_ADJUST = Convert.ToBoolean(_configIni.Read("adjustRatio", "Kingdom Hearts II"));

                Variables.LIMIT_SHORTS = _configIni.Read("limitShortcuts", "Kingdom Hearts II");
                Variables.RETRY_DEFAULT = _configIni.Read("deathPrompt", "Kingdom Hearts II") == "retry" ? true : false;

                Helpers.Log("Initializing Limit Shortcuts...", 0);

                // Should the shortcuts be parsed; Place them accordingly.
                if (Variables.LIMIT_SHORTS != "")
                {
                    LIMIT_SHORT = new short[4];

                    var _splitArr = Variables.LIMIT_SHORTS.Replace("[", "").Replace("]", "").Replace(", ", ",").Split(',');

                    // This code always presumes O is confirm.
                    LIMIT_SHORT[0] = Variables.LMTDictionary[_splitArr[0]];
                    LIMIT_SHORT[1] = Variables.LMTDictionary[_splitArr[1]];
                    LIMIT_SHORT[2] = Variables.LMTDictionary[_splitArr[2]];
                    LIMIT_SHORT[3] = Variables.LMTDictionary[_splitArr[3]];
                }

                Helpers.Log("Unlocking Memory Regions...", 0);

                // Unlock all the EXE-Related addresses.
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_FramelimiterINST, true);

                Hypervisor.UnlockBlock(Variables.ADDR_PAXFormatter);
                Hypervisor.UnlockBlock(Variables.ADDR_ANBFormatter);
                Hypervisor.UnlockBlock(Variables.ADDR_BTLFormatter);
                Hypervisor.UnlockBlock(Variables.ADDR_EVTFormatter);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_FramelimiterINST, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x18BA09, true);

                Hypervisor.UnlockBlock(Variables.ADDR_LimitShortcut);
                Hypervisor.UnlockBlock(Variables.ADDR_MusicPath);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortListFilterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortEquipFilterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortCategoryFilterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x39EF31, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x39EF36, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x38CD1A, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x38CD23, true);

                // Check if the patch is actually installed before proceeding.

                if (Operations.GetFileSize("itempic/item-271.imd") == 0x00)
                {
                    Helpers.Log("Re:Fined Main Patch was not detected! ERR404", 2);

                    var _messageResult = MessageBox.Show(
                        "Re:Fined could not detect the main patch in this game. Please reinstall! If you think this is a mistake, contact the Re:Fined Discord Server for assistance!\n\n" +
                        "Re:Fined will now be terminated.",
                        "Error #404: Patch Not Found!", MessageBoxButtons.OK,
                        MessageBoxIcon.Error);

                    if (_messageResult == DialogResult.Cancel || _messageResult == DialogResult.OK)
                        Environment.Exit(-2);
                }

                var _pathPrefix = Path.Combine(Path.GetDirectoryName(Hypervisor.Process.MainModule.FileName), "Image", "en");

                Helpers.Log("Scanning for Additional Features...", 0);

                // Remove the config options if the optionals are not patched in.

                var _audioItem = Variables.ARRY_ConfigMenu.First(x => x[1] == 0x01BD);
                var _musicItem = Variables.ARRY_ConfigMenu.First(x => x[1] == 0x01BE);
                var _heartItem = Variables.ARRY_ConfigMenu.First(x => x[1] == 0x01BF);

                var _audioWindow = Variables.ARRY_NewGameMenu.First(x => x[1] == 0x0000E006);
                var _musicWindow = Variables.ARRY_NewGameMenu.First(x => x[1] == 0x0000E007);
                var _heartWindow = Variables.ARRY_NewGameMenu.First(x => x[1] == 0x0000E008);

                // I had to add this after a total of 5 hours of troubleshooting people.

                if (Operations.GetFileSize("03system.bin") == 0x00)
                {
                    Helpers.Log("03system is too small! It is most likely corrupted! ERR430", 2);
                    var _messageResult = MessageBox.Show(
                        "03SYSTEM.BIN is corrupt! Please repatch the game correctly.\n" +
                        "Re:Fined will now be terminated.",
                        "Error #430: 03SYSTEM.BIN is corrupt.", MessageBoxButtons.OKCancel,
                        MessageBoxIcon.Error);

                    if (_messageResult == DialogResult.Cancel || _messageResult == DialogResult.OK)
                        Environment.Exit(-3);
                }

                // Check the Re:Fined add-ons.

                if (Operations.GetFileSize("obj/P_EX110.a.jp") == 0x00)
                {
                    Helpers.Log("The Audio Add-On was not found! Removing the option from all menus...", 1);
                    Variables.ARRY_ConfigMenu.Remove(_audioItem);
                    Variables.ARRY_NewGameMenu.Remove(_audioWindow);
                    CONFIG_REMOVE[0] = 0x01;
                }

                else if (Operations.GetFileSize("obj/P_EX110.a.ks") != 0x00)
                {
                    Helpers.Log("An additional Dub Patch was detected! Accommodating the config menu...", 1);

                    var _indexAudioItem = Variables.ARRY_ConfigMenu.IndexOf(_audioItem);
                    var _indexAudioWindow = Variables.ARRY_NewGameMenu.IndexOf(_audioWindow);

                    Variables.ARRY_ConfigMenu[_indexAudioItem] = Variables.VALUE_ConfigAudio;
                    Variables.ARRY_NewGameMenu[_indexAudioWindow] = Variables.VALUE_NewGameAudio;

                    DETECTED_EXTRA_DUB = true;
                }

                if (Operations.GetFileSize("bgm/ps2md050.win32.scd") == 0x00)
                {
                    Helpers.Log("The BGM Add-On was not found! Removing the option from all menus...", 1);
                    Variables.ARRY_ConfigMenu.Remove(_musicItem);
                    Variables.ARRY_NewGameMenu.Remove(_musicWindow);
                    CONFIG_REMOVE[1] = 0x01;
                }

                if (Operations.GetFileSize("obj/V_BB100.mdlx") == 0x00)
                {
                    Helpers.Log("The Enemy Palette Add-On was not found! Removing the option from all menus...", 1);
                    Variables.ARRY_ConfigMenu.Remove(_heartItem);
                    Variables.ARRY_NewGameMenu.Remove(_heartWindow);
                    CONFIG_REMOVE[2] = 0x01;
                }

                // Redirect libretto selection.

                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x38CD1A, new byte[] { 0x48, 0x8D, 0x0D, 0xDF, 0x31, 0x49, 0x00 }, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x38CD23, new byte[] { 0x4C, 0x8D, 0x05, 0xD6, 0x31, 0x49, 0x00 }, true);

                // Preapre the Game Over Menu for Retry.

                Helpers.Log("Writing the Game Over Menu...", 0);

                var _optionsPoint = Hypervisor.Read<ulong>(Variables.PINT_GameOverOptions);
                Hypervisor.WriteArray(_optionsPoint + 0x848, new byte[] { 0x20, 0x00, 0x04, 0x00, 0x02, 0x00, 0xB0, 0x8A, 0x03, 0x00, 0xB1, 0x8A, 0x0C, 0x00, 0x3A, 0xAF, 0x04, 0x00, 0xB2, 0x8A, 0x00, 0x00 }, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x350000, true);

                // Construct and write the Config Menu.

                Helpers.Log("Constructing the Configuration Menu...", 0);

                Hypervisor.Write(Hypervisor.PureAddress + 0x3624C3, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x362FFB, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x3630DC, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x363559, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x36362D, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x362653, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x362A21, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x36235A, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x362CB0, (byte)(Variables.ARRY_ConfigMenu.Count - 1), true);

                Hypervisor.Write(Hypervisor.PureAddress + 0x363050, (byte)Variables.ARRY_ConfigMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x36306F, (byte)Variables.ARRY_ConfigMenu.Count, true);

                CONFIG_COUNT = (byte)Variables.ARRY_ConfigMenu.Count;

                if (Variables.ARRY_ConfigMenu.Count != 14)
                {
                    for (int i = Variables.ARRY_ConfigMenu.Count; i < 14; i++)
                        Variables.ARRY_ConfigMenu.Add(new ushort[10]);
                }

                Helpers.Log("Writing the Configuration Menu...", 0);

                for (int i = 0; i < Variables.ARRY_ConfigMenu.Count; i++)
                {
                    for (int z = 0; z < Variables.ARRY_ConfigMenu[i].Length; z++)
                    {
                        var _address = Variables.ADDR_ConfigMenu + (ulong)(i * 0x14) + (ulong)(z * 2);
                        Hypervisor.WriteArray(_address, BitConverter.GetBytes(Variables.ARRY_ConfigMenu[i][z]));
                    }
                }

                // Construct and write the New Game Menu.

                Helpers.Log("Writing the New Game Menu...", 0);

                if (Operations.GetFileSize("obj/W_EX010_X0.mdlx") != 0)
                {
                    Helpers.Log("Garden of Assemblage Randomizer detected! Disabling Roxas Skip!", 1);
                    Variables.ARRY_NewGameMenu.Remove(Variables.ARRY_NewGameMenu.Last());
                    IS_RANDO = true;
                    DISABLE_SKIP = true;
                }

                Hypervisor.Write(Hypervisor.PureAddress + 0x2B5595, (byte)Variables.ARRY_NewGameMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B58D1, (byte)Variables.ARRY_NewGameMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B5A8F, (byte)Variables.ARRY_NewGameMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B5437, (byte)Variables.ARRY_NewGameMenu.Count, true);

                Hypervisor.Write(Hypervisor.PureAddress + 0x2B542E, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B6B8B, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B6767, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B6C11, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);

                for (int i = 0; i < Variables.ARRY_NewGameMenu.Count; i++)
                {
                    for (int z = 0; z < Variables.ARRY_NewGameMenu[i].Length; z++)
                    {
                        var _address = Variables.ADDR_NewGameMenu + (ulong)(i * 0x2C) + (ulong)(z * 4);
                        Hypervisor.WriteArray(_address, BitConverter.GetBytes(Variables.ARRY_NewGameMenu[i][z]));
                    }
                }

                if (Hypervisor.Read<byte>(0x5B6AD2) > 0x02)
                    Hypervisor.Write(0x5B6AD2, 0x00);

                // Initialize the source and the token for secondary tasks.
                Variables.Source = new CancellationTokenSource();
                Variables.Token = Variables.Source.Token;

                // Mark the initialization as complete.
                Variables.Initialized = true;

                Helpers.Log("Re:Fined initialized with no errors!", 0);
            }

            catch (Exception ERROR)
            {
                Helpers.LogException(ERROR);
                Helpers.Log("Re:Fined terminated with an exception!", 1);
                Environment.Exit(-1);
            }
        }

        /// <summary>
        /// Initializes the Save Sata stuff, along with Cloud Saving functionality. 
        /// Must be done after the game has initialized, as such, it is separate from the main function.
        /// </summary>
        public static void EGSInit()
        {
            Helpers.Log("Initializing Epic Games Store Variables...", 0);

            foreach (ProcessModule _module in Hypervisor.Process.Modules)
            {
                if (_module.ModuleName == "CODEX64.dll")
                {
                    Helpers.Log("Pirated version detected! Functions that rely on Epic Games Store has been disabled!", 1);
                    IS_PIRATE = true;
                }
            }

            Helpers.Log("Initializing Save/Config/Log Directories...", 0);

            var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

            var _logsDir = Path.Combine(_documentsPath, "Kingdom Hearts/Logs");
            var _saveDir = Path.Combine(_documentsPath, "Kingdom Hearts/Save Data");
            var _configDir = Path.Combine(_documentsPath, "Kingdom Hearts/Configuration");

            if (!Directory.Exists(_logsDir))
            {
                Directory.CreateDirectory(_logsDir);
                Directory.CreateDirectory(_saveDir);
                Directory.CreateDirectory(_configDir);
            }

            if (!IS_PIRATE)
            {
                var _epicPointer = Hypervisor.Read<ulong>(0x36548A);
                var _epicDenominator = Hypervisor.Read<ulong>(_epicPointer, true);
                var _epicIdentifier = Hypervisor.ReadArray(_epicDenominator, 0x40, true);

                if (_epicIdentifier[0] == 0x00)
                {
                    Helpers.Log("Waiting for the Epic ID. This may take a hot minute!", 1);

                    while (_epicIdentifier[0] == 0x00)
                    {
                        _epicPointer = Hypervisor.Read<ulong>(0x36548A);
                        _epicDenominator = Hypervisor.Read<ulong>(_epicPointer, true);
                        _epicIdentifier = Hypervisor.ReadArray(_epicDenominator, 0x40, true);
                    }
                }

                EPIC_ID = Encoding.Unicode.GetString(_epicIdentifier);
            }

            else
            {
                var _pirateLoc = Path.Combine(Path.GetDirectoryName(Hypervisor.Process.MainModule.FileName), "epic_emu.ini");
                var _pirateIni = new TinyIni(_pirateLoc);

                var _pirateID = _pirateIni.Read("AccountId", "Settings");

                if (_pirateID == "0")
                {
                    Helpers.Log("Randomized Epic ID Detected! ERR457", 2);

                    var _messageResult = MessageBox.Show(
                        "Re:Fined has detected a Randomized Epic ID being used with a pirated copy!\n\n" +
                        "While Re:Fined does not care about the game's origin, it does very much care about the Epic ID being used.\n\n" +
                        "Please set the \"AccountId\" variable in \"epic_emu.ini\" to anything else. preferably \"1638\" and launch Re:Fined again.\n\n" +
                        "Re:Fined will now be terminated.",
                        "Error #457: Randomized Epic ID Detected!", MessageBoxButtons.OK,
                        MessageBoxIcon.Error);

                    if (_messageResult == DialogResult.Cancel || _messageResult == DialogResult.OK)
                        Environment.Exit(-2);
                }

                else if (_pirateID != "")
                    EPIC_ID = _pirateID;
            }

            Directory.CreateDirectory(Path.Combine(_saveDir, EPIC_ID));
            Directory.CreateDirectory(Path.Combine(_configDir, EPIC_ID));

            Helpers.Log("Detected and created directories for ID: " + EPIC_ID, 0);
        }

        #region Base Functionality

        /// <summary>
        /// Allow a warp to the Garden of Assemblage from the World Map.
        /// </summary>
        public static void AllowGOA()
        {
            var _flagRead = Hypervisor.Read<byte>(0x444A58);
            var _goaVisit = Hypervisor.Read<byte>(0x444E7D);
            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);
            
            if (_worldRead == 0x0F && (_goaVisit & 0x04) == 0x04 && (_flagRead & 0x40) == 0x00)
                Hypervisor.Write(0x444A58, (byte)(_flagRead + 0x40));

            else if (_worldRead != 0x0F && (_flagRead & 0x40) == 0x40)
                Hypervisor.Write(0x444A58, (byte)(_flagRead - 0x40));
        }

        /// <summary>
        /// When the proper input is given, returns to the title screen.
        /// When the option for it is toggled, prompts the user for a cancellation.
        /// </summary>
        public static void ResetGame()
        {
            var _currentTime = DateTime.Now;

            var _buttonRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _confirmRead = Hypervisor.Read<ushort>(Variables.ADDR_Confirm);

            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _menuRead = Hypervisor.Read<ushort>(Variables.ADDR_MenuActive);

            var _canReset = !Operations.CheckTitle() && _loadRead == 0x01 && _menuRead == 0x00;

            // If the button combo was exactly as requested, and a menu isn't present:
            if (_buttonRead == Variables.RESET_COMBO && _canReset && !DEBOUNCE[0])
            {
                Helpers.Log("Reset Sequence Initiated! Checking for the prompt...", 0);
                DEBOUNCE[0] = true;

                // If the prompt has been requested:
                if (Variables.RESET_PROMPT)
                {
                    Helpers.Log("Reset prompt requested! Showing prompt...", 0);

                    // Show the prompt.
                    Additions.ShowSmallObtained(0x01BA);
                    var _cancelRequest = false;

                    // Start the prompt task.
                    Task.Factory.StartNew(() =>
                    {
                        Helpers.Log("Prompt shown! Waiting 2 seconds...", 0);

                        // For the next 2 seconds:
                        while ((DateTime.Now - _currentTime) < TimeSpan.FromSeconds(2))
                        {
                            // Monitor the buttons, and if pressed:
                            var _buttonSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
                            var _buttonSecond = Hypervisor.Read<ushort>(Variables.ADDR_Input);

                            // Cancel the reset.
                            if ((_buttonSecond & _buttonSeek) == _buttonSeek)
                            {
                                Helpers.Log("Reset cancelled!", 0);
                                Additions.ShowSmallObtained(0x01BB);
                                _cancelRequest = true;
                                DEBOUNCE[0] = false;
                                break;
                            };
                        }

                        // If not cancelled: Initiate the reset.
                        if (!_cancelRequest)
                        {
                            Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
                            Helpers.Log("Soft Reset was successful!", 0);
                            DEBOUNCE[0] = false;
                        }
                    });
                }

                // If the prompt isn't requested: Reset instantly.
                else
                {
                    Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
                    Helpers.Log("Soft Reset was successful!", 0);
                    DEBOUNCE[0] = false;
                }
            }
        }

        /// <summary>
        /// Adjusts and edits the current framelimiter, and the function which controls it,
        /// according to the current chosen framerate.
        /// </summary>
        public static void OverrideLimiter()
        {
            // Calculate the instruction address.
            var _instructionAddress = Hypervisor.PureAddress + Variables.ADDR_FramelimiterINST;

            // Fetch the framerate, and the first byte of the instruction.
            var _framerateRead = Hypervisor.Read<byte>(Variables.ADDR_Framerate);
            var _instructionRead = Hypervisor.Read<byte>(_instructionAddress, true);

            // If the framerate is set to 30FPS, and the limiter is NOP'd out: Rewrite the instruction.
            if (_framerateRead == 0x00 && _instructionRead == 0x90)
            {
                Helpers.Log("Switched to 30FPS! Restoring the framelimiter.", 0);
                Hypervisor.WriteArray(_instructionAddress, Variables.INST_FrameLimiter, true);
            }

            // Otherwise, if the framerate is not set to 30FPS, and the limiter is present:
            else if (_framerateRead != 0x00 && _instructionRead != 0x90)
            {
                Helpers.Log("Switched to 60FPS! Destroying the Framelimiter.", 0);

                // NOP the instruction.
                Hypervisor.WriteArray(_instructionAddress, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);

                // Set the current limiter to be off.
                Hypervisor.Write<byte>(Variables.ADDR_Framelimiter, 0x00);
            }
        }

        /// <summary>
        /// Overwrites the Limit Shortcuts with the ones given in the config file.
        /// </summary>
        public static void OverrideLimits()
        {
            // Read the necessary information.
            var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);
            var _modeRead = Hypervisor.Read<ushort>(Variables.ADDR_ControllerMode);
            var _shortRead = Hypervisor.Read<ushort>(Variables.ADDR_LimitShortcut);

            // If "O" is confirm, and the shortcuts are not set:
            if (_confirmRead == 0x00 && _shortRead != LIMIT_SHORT[0])
            {
                Helpers.Log("Overriding Limits for the Japanese scheme.", 0);

                Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[0]);
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[3]);
            }

            // If "X" is confirm, and the prompts are for a controller, and the shortcuts are not set:
            else if (_confirmRead == 0x01 && _shortRead != LIMIT_SHORT[3] && _modeRead == 0)
            {
                Helpers.Log("Overriding Limits for the English Scheme", 0);

                Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[3]);
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[0]);
            }

            // Write the SQU and TRI shortcuts.
            Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x02, LIMIT_SHORT[1]);
            Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x04, LIMIT_SHORT[2]);
        }

        /// <summary>
        /// The function responsible for automatically saving the game.
        /// </summary>
        public static void AutosaveEngine()
        {
            // Read all the values
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);

            var _pauseCheck = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);

            // Please refrain from saving in these rooms kthxbye!
            var _blacklistCheck =
                (_worldCheck == 0x08 && _roomCheck == 0x03) ||
                (_worldCheck == 0x0C && _roomCheck == 0x02) ||
                (_worldCheck == 0x12 && _roomCheck >= 0x13 && _roomCheck <= 0x1D) ||
                (_worldCheck == 0x02 && _roomCheck <= 0x01);

            // If the game is loaded, and Sora is in the map, and the Blacklist isn't in effect:
            if (!Operations.CheckTitle() && _loadRead == 0x01 && !_blacklistCheck)
            {
                // Wait for a bit.
                Thread.Sleep(250);

                // Read the BTL and CTS state.
                var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
                var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                // Read if the game is loaded again.
                _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

                // Check if the game is saveable:
                // 
                // - Autosave is enabled.
                // - Player isn't in battle.
                // - The map is loaded.
                // - Not on a cutscene.
                // - The world is valid.
                // - The game isn't paused.

                var _saveableBool = (Variables.SAVE_MODE != 0x02) && _battleRead == 0x00 && _loadRead == 0x01 && _cutsceneRead == 0x00 && _worldCheck >= 0x02 && _pauseCheck == 0x00;

                // If the game is indeed saveable:
                if (_saveableBool)
                {
                    // If the world has changed and it isn't the World Map:
                    if (SAVE_WORLD != _worldCheck && _worldCheck != 0x07)
                    {
                        Helpers.Log("The world has changed! Autosaving...", 0);

                        Operations.GenerateSave();
                        SAVE_ITERATOR = 0;
                    }

                    // If the room has changed, and the world is valid:
                    else if (SAVE_ROOM != _roomCheck && _worldCheck >= 2)
                    {
                        // Increase the iterator.
                        SAVE_ITERATOR++;

                        // If it has been 3 roomchanges:
                        if (SAVE_ITERATOR == 3)
                        {
                            Helpers.Log("Room condition met! Writing Autosave...", 0);

                            Operations.GenerateSave();
                            SAVE_ITERATOR = 0;
                        }
                    }
                    
                    // Record the last room and world visited.
                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;
                }
            }
        }

        /// <summary>
        /// Fixes the exit functions on the Title Screen.
        /// </summary>
        public static void ExitFix()
        {
            // If the game is in the Title Screen:
            if (Operations.CheckTitle())
            {
                // Record the count and the current button.
                var _countButton = Hypervisor.Read<byte>(Variables.ADDR_TitleOptionCount);
                var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleOptionSelect);

                // Read the input state.
                var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
                var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

                var _buttonSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
                var _inputValue = _inputRead & _buttonSeek;

                // If the chosen option is the last one, and CONFIRM is pressed: Exit the game.
                if (_inputValue == _buttonSeek && _selectButton == _countButton - 0x01)
                {
                    Helpers.Log("Title to Exit detected! 2.5 second limit set! Initating exit...", 0);
                    Thread.Sleep(2500);

                    Helpers.Log("Re:Fined terminated with no errors.", 0);
                    Environment.Exit(0);
                }
            }
        }

        #endregion

        #region Advanced Functionality

        /// <summary>
        /// The engine responsible for Discord Rich Presence.
        /// </summary>
        public static void DiscordEngine()
        {
            // Read all the values.

            var _worldID = Hypervisor.Read<byte>(Variables.ADDR_Area);

            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
            var _roundRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x02);
            var _eventRead = Hypervisor.Read<ushort>(Variables.ADDR_Area + 0x04);

            var _formValue = Hypervisor.Read<byte>(Variables.ADDR_PlayerForm);
            var _healthValue = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP);
            var _magicValue = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x180);

            var _battleFlag = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
            var _diffValue = Hypervisor.Read<byte>(Variables.ADDR_GameDifficulty);

            var _timeValue = Math.Floor(Hypervisor.Read<int>(Variables.ADDR_GameTime) / 60F);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            // Construct the necessary strings.

            var _stringState = string.Format
            (
                "Level {0} | Form: {1}",
                Hypervisor.Read<byte>(Variables.ADDR_PlayerLevel),
                Variables.DICTIONARY_FRM.ElementAtOrDefault(_formValue)
            );

            var _stringDetail = string.Format("HP: {0} | MP: {1}", _healthValue, _magicValue);
            var _timeText = string.Format("In-Game Time: {0}", string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00")));

            // If the game is NOT in the Title Screen, apply the detailed presence.
            if (!Operations.CheckTitle())
            {
                if (_worldID == 0x06 && _roomRead == 0x09 && (_eventRead >= 0xBD && _eventRead <= 0xC4))
                {
                    Variables.DiscordClient.SetPresence(
                        new RichPresence
                        {
                            Details = _stringDetail,
                            State = _stringState + " | Round: " + _roundRead,
                            Assets = new Assets
                            {
                                LargeImageText = _timeText,

                                LargeImageKey = Variables.DICTIONARY_CPS.ElementAtOrDefault(_eventRead < 0xC1 ? _eventRead - 0xBD : _eventRead - 0xC1),
                                SmallImageText = Variables.DICTIONARY_MDE.ElementAtOrDefault(_diffValue),
                                SmallImageKey = Variables.DICTIONARY_BTL.ElementAtOrDefault(_battleFlag)
                            },
                        }
                    );
                }

                else
                {
                    Variables.DiscordClient.SetPresence(
                        new RichPresence
                        {
                            Details = _stringDetail,
                            State = _stringState,
                            Assets = new Assets
                            {
                                LargeImageText = _timeText,

                                LargeImageKey = Variables.DICTIONARY_WRL.ElementAtOrDefault(_worldID),
                                SmallImageText = Variables.DICTIONARY_MDE.ElementAtOrDefault(_diffValue),
                                SmallImageKey = Variables.DICTIONARY_BTL.ElementAtOrDefault(_battleFlag)
                            },
                        }
                    );
                }
            }

            // If the game is in the Title Screen, apply the simple presence.
            else
            {
                Variables.DiscordClient.SetPresence(
                    new RichPresence
                    {
                        Details = "On the Title Screen",
                        State = null,
                        Assets = new Assets
                        {
                            LargeImageKey = "title",
                            SmallImageKey = null,
                            SmallImageText = null
                        },
                    }
                );
            }
        }

        /// <summary>
        /// Registers the magic spells.
        /// Prone to instability?
        /// </summary>
        public static void RegisterMagic()
        {
            var _canLoad = !Operations.CheckTitle() && Hypervisor.Read<byte>(Variables.ADDR_LoadFlag) == 0x01;
            var _magicRead = Hypervisor.Read<uint>(Variables.ADDR_MagicLV1) + Hypervisor.Read<ushort>(Variables.ADDR_MagicLV2); 

            if (!_canLoad && MAGIC_MEMORY.Count() > 0x00)
            {
                Helpers.Log("Flushing the current Magic Memory!", 0);
                MAGIC_MEMORY.Clear();
            }
            
            if (MAGIC_OLD != _magicRead && _canLoad)
            {
                var _tableMagic = new byte[] { 0x31, 0x33, 0x32, 0x34, 0xAE, 0xB1 };

                for (ulong i = 0; i < 0x06; i++)
                {
                    var _magicPointer = Variables.SharpHook[(IntPtr)0x3C3240].Execute<uint>((long)i);

                    if (_magicPointer != 0x00)
                    {
                        var _magNameAddr = Hypervisor.MemoryOffset + _magicPointer + 0x04;
                        var _magFileName = Hypervisor.ReadArray(_magNameAddr, 0x20, true);

                        var _currentDetails = Hypervisor.Read<ulong>(0x24BCC7A + (0x50 * i));

                        if (_currentDetails == Hypervisor.MemoryOffset + _magicPointer)
                            continue;

                        else if (_currentDetails != 0x00)
                        {
                            var _currentName = Encoding.Default.GetString(Hypervisor.ReadArray(_currentDetails + 0x04, 0x20, true));
                            var _newName = Encoding.Default.GetString(_magFileName);

                            var _fromString = _newName.IndexOf('/') + 1;
                            var _toString = _newName.IndexOf('_');

                            var _magicType = _newName.Substring(_fromString, _toString - _fromString);

                            // This prevents Magic Upgrade Registration, which crashes for some reason.
                            if (_currentName.Contains(_magicType))
                                continue;
                        }

                        Helpers.Log("Found Magic File: " + Encoding.Default.GetString(_magFileName), 0);

                        var _magFileSize = Variables.SharpHook[(IntPtr)0x39E2F0].Execute<int>((long)_magNameAddr);
                        var _magAllocMemory = Variables.SharpHook[(IntPtr)0x150030].Execute<int>(_magFileSize + 0x800) + 0x100000000;

                        Helpers.Log("Allocated Region for Magic at 0x" + (Hypervisor.MemoryOffset + (uint)_magAllocMemory).ToString("X12"), 1);

                        Variables.SharpHook[(IntPtr)0x39E4E0].ExecuteJMP(BSharpConvention.MicrosoftX64, (long)_magNameAddr, (long)(Hypervisor.MemoryOffset + (uint)_magAllocMemory));

                        Helpers.Log("Loaded Magic File to 0x" + (Hypervisor.MemoryOffset + (uint)_magAllocMemory).ToString("X12"), 0);

                        var _offsetBAR = Hypervisor.Read<uint>(Hypervisor.MemoryOffset + (uint)_magAllocMemory + 0x08, true);
                        var _offsetPAX = Hypervisor.Read<uint>(Hypervisor.MemoryOffset + (uint)_magAllocMemory + 0x18, true) - _offsetBAR;
                        var _offsetMAG = Hypervisor.Read<uint>(Hypervisor.MemoryOffset + (uint)_magAllocMemory + 0x28, true) - _offsetBAR;

                        Hypervisor.Write(0x24BCC7A + (0x50 * i), Hypervisor.MemoryOffset + _magicPointer);
                        Hypervisor.Write(0x24BCC32 + (0x50 * i), Hypervisor.MemoryOffset + (uint)_magAllocMemory);
                        Hypervisor.Write(0x24BCC3A + (0x50 * i), Hypervisor.MemoryOffset + (uint)_magAllocMemory + _offsetMAG);
                        Hypervisor.Write(0x24BCC42 + (0x50 * i), Hypervisor.MemoryOffset + (uint)_magAllocMemory + _offsetMAG);
                        Hypervisor.Write(0x24BCC52 + (0x50 * i), Hypervisor.MemoryOffset + (uint)_magAllocMemory + _offsetPAX + 0x10);

                        Helpers.Log("Magic Details loaded to 0x" + (Hypervisor.BaseAddress + 0x24BCC32 + (0x50 * i)).ToString("X12"), 1);

                        var _execOffset = Hypervisor.PureAddress + 0x2A21198 + (0x50 * i);

                        for (int z = 0; z < 6; z++)
                        {
                            if (MAGIC_MEMORY.ElementAtOrDefault(z) == _execOffset)
                                break;

                            if (MAGIC_MEMORY.ElementAtOrDefault(z) == null)
                            {
                                MAGIC_MEMORY.Add(_execOffset);
                                break;
                            }
                        }
                    }
                }

                Hypervisor.Write<ulong>(0x24AA33A, 0x00);
                ulong _magicOffset = 0x00;

                for (ulong i = 0; i < 0x06; i++)
                {
                    var _magicPointer = Variables.SharpHook[(IntPtr)0x3C3240].Execute<uint>((long)i);

                    if (_magicPointer != 0x00)
                    {
                        Hypervisor.Write(0x24AA33A + (0x02 * _magicOffset), _tableMagic[i]);
                        Helpers.Log("Command Written to 0x" + (Hypervisor.BaseAddress + 0x24AA33A + (0x02 * i)).ToString("X12"), 1);
                        _magicOffset++;
                    }
                }

                for (int i = 0; i < 0x06; i++)
                {
                    if (MAGIC_MEMORY.ElementAtOrDefault(i) != null)
                    {
                        var _barOffset = Hypervisor.Read<ulong>(MAGIC_MEMORY.ElementAtOrDefault(i).Value - 0x18, true);

                        Variables.SharpHook[(IntPtr)0x2C1AB0].Execute((long)MAGIC_MEMORY.ElementAtOrDefault(i).Value);
                        Variables.SharpHook[(IntPtr)0x2C3D80].Execute(BSharpConvention.MicrosoftX64, (long)MAGIC_MEMORY.ElementAtOrDefault(i).Value, (long)(_barOffset + 0x40));

                        Helpers.Log("Loaded PAX at 0x" + (_barOffset + 0x40).ToString("X12") + " for 0x" + MAGIC_MEMORY.ElementAtOrDefault(i).Value.ToString("X12"), 0);
                    }
                }

                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x3C314A, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                MAGIC_OLD = _magicRead;
            }
        }

        /// <summary>
        /// Shifts the chosen spell in the Magic menu up or down,
        /// depending on the input. Executed with L2 + DOWN or L2 + UP.
        /// </summary>
        public static void MagicSorting()
        {
            // Fetch the status of the room. This will be necessary.
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            // Fetch the input and read the pointer to the second CM.
            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _menuPointer = Hypervisor.Read<ulong>(Variables.PINT_MagicMenu);

            // Fetch the current Magic Level arrays.
            var _magicOne = Hypervisor.Read<uint>(Variables.ADDR_MagicLV1);
            var _magicTwo = Hypervisor.Read<ushort>(Variables.ADDR_MagicLV2);

            var _readMagic = Hypervisor.ReadArray(Variables.ADDR_SaveData + 0xE500, 0x0C);
            var _firstMagic = BitConverter.ToUInt16(_readMagic, 0x00);

            // If we have saved magic sorting before, reload it!
            if (_loadRead == 0x01 && MAGIC_STORE == null && _firstMagic != 0x00)
            {
                Helpers.Log("Detected a saved Magic sort! Reloading...", 0);
                MAGIC_STORE = _readMagic;
            }

            // If a new spell is learned: Forget the sorting.
            if (_magicOne != MAGIC_LV1 || _magicTwo != MAGIC_LV2)
            {
                Helpers.Log("Spell change detected! Resetting sort memory.", 1);

                MAGIC_STORE = null;

                MAGIC_LV1 = _magicOne;
                MAGIC_LV2 = _magicTwo;

                _readMagic = new byte[0x0C];
                Hypervisor.WriteArray(Variables.ADDR_SaveData + 0xE500, _readMagic);

                _firstMagic = 0x00;
            }

            // If a Soft Reset ID detected: Forget the sorting.
            if (Operations.CheckTitle())
                MAGIC_STORE = null;

            /*
                So the magic menu resets on each room transition.
                To prevent this, I check if the room is reloaded, and
                if so, just override it again.

                This is why we store the menu in memory when we shift it.
                This is why I hate my life.

                But eh, whatever.
            */

            // If the room is reloaded, and menu memory is not empty:
            if (_loadRead == 0x01 && ROOM_LOADED && _menuPointer != 0x00)
            {
                // Write back the memorized magic menu.
                if (MAGIC_STORE != null)
                {
                    Helpers.Log("Roomchange detected! Restoring the Magic Menu.", 1);
                    Hypervisor.WriteArray(Variables.ADDR_MagicCommands, MAGIC_STORE);
                }

                ROOM_LOADED = false;
            }

            // Otherwise, if the room ISN'T loaded: Declare as such.
            else if (_loadRead == 0x00 && !ROOM_LOADED)
                ROOM_LOADED = true;

            // If there is a secondary CM, and the room is loaded:
            if (_menuPointer != 0x00 && _loadRead == 0x01)
            {
                // Read the secondary CM's type.
                var _menuRead = Hypervisor.Read<byte>(_menuPointer, true);

                // If the Magic Menu is the second CM to be shown (ID: 0x01):
                if (_menuRead == 0x01)
                {
                    // Fetch the current magic index and the max index.
                    var _magicIndex = Hypervisor.Read<byte>(Variables.ADDR_MagicIndex);
                    var _magicMax = Hypervisor.Read<byte>(_menuPointer + 0x10, true);

                    // Set to 0x01 if it's going up, set to 0x02 if it's going down.
                    var _inputCheck = (_inputRead & 0x01001) == 0x1001 ? 0x01 : (_inputRead & 0x4001) == 0x4001 ? 0x02 : 0x00;
                    var _triggerCheck = (_inputRead & 0x01) == 0x01;

                    // Read the instruction.
                    var _insCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[0], true);

                    // If L2 is being held down: Destroy the command controller.
                    if (_triggerCheck && _insCheck != 0x90)
                    {
                        Helpers.Log("L2 Detected within Magic Menu! Disabling input registry.", 1);

                        // NOP out command selection, so it does not interfere with our input.
                        for (int _ins = 0; _ins < Variables.ADDR_CMDSelectINST.Length; _ins++)
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[_ins], new byte[] { 0x90, 0x90, 0x90 }, true);
                    }

                    // If L2 has been let go: Restore the command controller.
                    else if (!_triggerCheck && _insCheck == 0x90)
                    {
                        Helpers.Log("L2 has been let go! Enabling input registry.", 1);

                        // Revert the NOP'd instructions.
                        for (int _ins = 0; _ins < Variables.ADDR_CMDSelectINST.Length; _ins++)
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[_ins], Variables.INST_CMDSelect[_ins], true);
                    }

                    // If debounce is not active, and input is proper:
                    if (!DEBOUNCE[1] && _inputCheck != 0x00)
                    {
                        // Activate debounce.
                        DEBOUNCE[1] = true;

                        // Calculate the pointer and the bounds of the menu.
                        var _magicPointer = (0x02 * _magicIndex);
                        var _magicBounds = _magicPointer + (_inputCheck == 0x01 ? -0x02 : 0x02);

                        // Fetch the subject magic and the switchee.
                        var _subjectMagic = Hypervisor.Read<ushort>(Variables.ADDR_MagicCommands + (ulong)_magicPointer);
                        var _targetMagic = _magicBounds >= 0 ? Hypervisor.Read<ushort>(Variables.ADDR_MagicCommands + (ulong)_magicBounds) : (ushort)0x0000;

                        // If the move would be in bounds, and if the switchee is not "nothing":
                        if (_targetMagic != 0x0000)
                        {
                            // Make the switch
                            Hypervisor.Write(Variables.ADDR_MagicCommands + (ulong)_magicPointer, _targetMagic);
                            Hypervisor.Write(Variables.ADDR_MagicCommands + (ulong)_magicBounds, _subjectMagic);

                            // Move the magic index.
                            Hypervisor.Write(Variables.ADDR_MagicIndex, _magicIndex + (_inputCheck == 0x01 ? -0x01 : 0x01));
                            Hypervisor.Write(Variables.ADDR_MagicIndex + 0x04, _subjectMagic);

                            Helpers.Log(String.Format("Moving Magic ID \"{0}\" {1} within the menu!", "0x" + _subjectMagic.ToString("X4"), _inputCheck == 0x01 ? "up" : "down"), 0);

                            // Read the entirety of the magic menu, and save it to memory.
                            // And also save it to the save data.
                            MAGIC_STORE = Hypervisor.ReadArray(Variables.ADDR_MagicCommands, _magicMax * 0x02);
                            Hypervisor.WriteArray(Variables.ADDR_SaveData + 0xE500, MAGIC_STORE);

                        }

                        else
                            Helpers.Log("Could not move the spell out of bounds!", 1);
                    }

                    // Otherwise: If debounce is active and input is improper; deactivate debounce.
                    else if (DEBOUNCE[1] && _inputCheck == 0x00)
                        DEBOUNCE[1] = false;
                }
            }

            else
            {
                // Revert the NOP'd instructions.
                for (int _ins = 0; _ins < Variables.ADDR_CMDSelectINST.Length; _ins++)
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[_ins], Variables.INST_CMDSelect[_ins], true);
            }
        }

        /// <summary>
        /// Allows Drive Forms to be added to the Shortcut Menu.
        /// This function is a mess, please do not touch.
        /// </summary>
        public static void ShortcutForms()
        {
            var _instCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_ShortCategoryFilterINST, true);

            if (Variables.FORM_SHORTCUT && _instCheck != 0x90)
            {
                Helpers.Log("Allowing Drive Forms to be shortcutted...", 0);

                // This reads part of the actual instruction which handles drive icons on shortcuts to move it.
                var _instRead = Hypervisor.ReadArray(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST + 0x03, 0x19, true);

                // This writes a JMP statement to trigger an alternative condition.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST, Variables.INST_ShortIconAssign[0], true);

                // Move the instruction.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST + 0x02, _instRead, true);

                // Write the check for the improper icon, and correct it.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST + 0x1B, Variables.INST_ShortIconAssign[1], true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST + 0x21, Variables.INST_ShortIconAssign[2], true);

                // Write what icon it should be corrected to.
                Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST + 0x20, 0xCE, true);

                // Adjustments to the shortcut filter mechanism to show the drives in the list:
                // This one jumps out to a interrupt block so that we can inject code.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortListFilterINST, Variables.INST_ShortListFilter[0], true);

                // This one performs an OR operation and inserts the bit value 0x240000 so that
                // both Drive Forms (0x200000) and Magic (0x40000) can show up.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortListFilterINST + 0x50, Variables.INST_ShortListFilter[1], true);

                // This jumps out of the interrupt block and continues execution.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortListFilterINST + 0x56, Variables.INST_ShortListFilter[2], true);

                // Adjustments to the equip filter mechanism to actually equip the shortcuts.
                // This one wipes out the false condition and jumps out to a interrupt block so that we can inject code.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortEquipFilterINST, Variables.INST_ShortEquipFilter[0], true);

                // This one performs a check for Drive Forms, if true, it jumps to the code that 
                // handles non-magic shortcuts and their memorizaiton. 
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortEquipFilterINST + 0x1D, Variables.INST_ShortEquipFilter[1], true);

                // This re-implements the false condition.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortEquipFilterINST + 0x22, Variables.INST_ShortEquipFilter[2], true);

                // This NOPs a jump statement which causes the equipped drive to be treated as a spell.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortCategoryFilterINST, new byte[] { 0x90, 0x90 }, true);

                Helpers.Log("Success! Drive Forms can now be shortcutted!", 0);
            }
        }

        /// <summary>
        /// Reconstructs and handles the New Game menu.
        /// </summary>
        public static void NewGameHandler()
        {
            if (Operations.CheckTitle())
            {
                if (Hypervisor.Read<byte>(0x5B6AC2) == 0x03)
                {
                    for (int i = 0; i < Variables.ARRY_NewGameMenu.Count; i++)
                    {
                        for (int z = 0; z < Variables.ARRY_NewGameMenu[i].Length; z++)
                        {
                            var _address = Variables.ADDR_NewGameMenu + (ulong)(i * 0x2C) + (ulong)(z * 4);
                            Hypervisor.WriteArray(_address, BitConverter.GetBytes(Variables.ARRY_NewGameMenu[i][z]));
                        }
                    }
                }

                if (Hypervisor.Read<byte>(0x5B6AC2) != 0x03)
                {
                    for (int i = 0; i < Variables.ARRY_NewGameMenu.Count; i++)
                    {
                        for (int z = 0; z < Variables.ARRY_NewGameMenu[i].Length; z++)
                        {
                            var _address = Variables.ADDR_NewGameMenu + (ulong)(i * 0x2C) + (ulong)(z * 4);
                            Hypervisor.WriteArray(_address, BitConverter.GetBytes(Variables.ARRY_NewGameMenu[i][z]));
                        }
                    }
                }

                Hypervisor.Write(Hypervisor.PureAddress + 0x2B5595, (byte)Variables.ARRY_NewGameMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B58D1, (byte)Variables.ARRY_NewGameMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B5A8F, (byte)Variables.ARRY_NewGameMenu.Count, true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B5437, (byte)Variables.ARRY_NewGameMenu.Count, true);

                Hypervisor.Write(Hypervisor.PureAddress + 0x2B542E, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B6B8B, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B6767, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);
                Hypervisor.Write(Hypervisor.PureAddress + 0x2B6C11, (byte)(Variables.ARRY_NewGameMenu.Count - 1), true);

                CONFIG_WRITTEN = false;

                var _configOffset = (CONFIG_REMOVE[0] + CONFIG_REMOVE[1] + CONFIG_REMOVE[2]) * 0x04;

                var _vibration = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x04) == 0x00 ? Variables.CONFIG_BITWISE.VIBRATION : Variables.CONFIG_BITWISE.OFF;

                var _autoSave = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x08) == 0x00 ? Variables.CONFIG_BITWISE.AUTOSAVE_INDICATOR :
                               (Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x08) == 0x01 ? Variables.CONFIG_BITWISE.AUTOSAVE_SILENT : Variables.CONFIG_BITWISE.OFF);
                
                var _controlPrompt = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x18 - _configOffset) == 0x00 ? Variables.CONFIG_BITWISE.PROMPT_CONTROLLER :
                                    (Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x18 - _configOffset) == 0x01 ? Variables.CONFIG_BITWISE.PROMPT_KEYBOARD : Variables.CONFIG_BITWISE.OFF);

                SKIP_ROXAS = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x1C - _configOffset) == 0x00 ? false : true;

                CONFIG_BIT =
                    Variables.CONFIG_BITWISE.SUMMON_FULL | Variables.CONFIG_BITWISE.NAVI_MAP |
                    _vibration | _autoSave | _controlPrompt;

                if (CONFIG_REMOVE[0] == 0x00)
                {
                    var _audioLanguage = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x0C) == (DETECTED_EXTRA_DUB ? 0x02 : 0x01) ? Variables.CONFIG_BITWISE.AUDIO_JAPANESE :
                                        (Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x0C) == 0x00 ? Variables.CONFIG_BITWISE.OFF : Variables.CONFIG_BITWISE.AUDIO_OTHER);

                    CONFIG_BIT = CONFIG_BIT | _audioLanguage;
                }

                if (CONFIG_REMOVE[1] == 0x00)
                {
                    var _offsetMusic = CONFIG_REMOVE[0] * 0x04;
                    var _musicSelect = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x10 - _offsetMusic) == 0x01 ? Variables.CONFIG_BITWISE.MUSIC_VANILLA : Variables.CONFIG_BITWISE.OFF;

                    CONFIG_BIT = CONFIG_BIT | _musicSelect;
                }

                if (CONFIG_REMOVE[2] == 0x00)
                {
                    var _offsetHeartless = (CONFIG_REMOVE[0] + CONFIG_REMOVE[1]) * 0x04;
                    var _heartlessSelect = Hypervisor.Read<byte>(Variables.ADDR_NewGame + 0x14 - _offsetHeartless) == 0x01 ? Variables.CONFIG_BITWISE.HEARTLESS_VANILLA : Variables.CONFIG_BITWISE.OFF;
                    CONFIG_BIT = CONFIG_BIT | _heartlessSelect;
                }
            }

            if (!Operations.CheckTitle() && !CONFIG_WRITTEN)
            {
                var _areaRead = Hypervisor.Read<uint>(Variables.ADDR_Area);
                var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                if (_areaRead == 0x0102 || (IS_RANDO && _areaRead == 0x2002))
                {
                    Helpers.Log("Start of a new game detected! Writing configuration...", 0);

                    Hypervisor.Write(Variables.ADDR_Config, (ushort)CONFIG_BIT);

                    Variables.AUDIO_MODE = (byte)(((ushort)CONFIG_BIT & 0x0800) == 0x0800 ? 0x01 : (((ushort)CONFIG_BIT & 0x1000) == 0x1000 ? 0x02 : 0x0000));
                    Variables.MUSIC_VANILLA = ((ushort)CONFIG_BIT & 0x0020) == 0x0020;
                    Variables.ENEMY_VANILLA = ((ushort)CONFIG_BIT & 0x8000) == 0x8000;
                    Variables.SAVE_MODE = (byte)(((ushort)CONFIG_BIT & 0x0002) == 0x0002 ? 0x01 : (((ushort)CONFIG_BIT & 0x0004) == 0x0004 ? 0x00 : 0x02));
                    Variables.CONTROLLER_MODE = (byte)(((ushort)CONFIG_BIT & 0x2000) == 0x2000 ? 0x00 : (((ushort)CONFIG_BIT & 0x4000) == 0x4000 ? 0x01 : 0x02));

                    CONFIG_WRITTEN = true;
                }

                else
                {
                    Helpers.Log("Loaded game detected! Not writing configuration...", 0);
                    CONFIG_WRITTEN = true;
                }
            }
        }

        /// <summary>
        /// Handles the configuration of the game.
        /// Prone to crashing.
        /// </summary>
        public static void ConfigHandler()
        {
            // Read a LOT of shit.
            var _configRead = Hypervisor.Read<ushort>(Variables.ADDR_Config);
            var _selectPoint = Hypervisor.Read<ulong>(Variables.PINT_SubMenuOptionSelect);

            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _pauseRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);
            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);

            var _settingsPoint = Hypervisor.Read<ulong>(Variables.PINT_ConfigMenu);
            var _difficultyRead = Hypervisor.Read<byte>(Variables.ADDR_GameDifficulty);

            var _offsetTitle = Operations.FindInfoMSG(Variables.PINT_SystemMSG, 0x3738) + 0x04;
            var _offsetDesc = Operations.FindInfoMSG(Variables.PINT_SystemMSG, 0x373B) + 0x04;

            var _findTitle = Operations.FetchOffsetMSG(Variables.PINT_SystemMSG, Variables.VALUE_ConfigTitle[_difficultyRead]);
            var _findDesc = Operations.FetchOffsetMSG(Variables.PINT_SystemMSG, Variables.VALUE_ConfigDescription[_difficultyRead]);

            var _hardTitle = Operations.FetchOffsetMSG(Variables.PINT_SystemMSG, 0x600C);
            var _hardDesc = Operations.FetchOffsetMSG(Variables.PINT_SystemMSG, 0x600E);

            var _layoutPointer = Hypervisor.Read<ulong>(0x3A0952);

            // If we are not in the title screen and the settings are not yet loaded:
            if (!Operations.CheckTitle() && !LOADED_SETTINGS)
            {
                Helpers.Log("Fetching the current configuration from Save Data...", 0);

                Variables.AUDIO_MODE = (byte)((_configRead & 0x0800) == 0x0800 ? 0x01 : ((_configRead & 0x1000) == 0x1000 ? 0x02 : 0x0000));
                Variables.MUSIC_VANILLA = (_configRead & 0x0020) == 0x0020;
                Variables.ENEMY_VANILLA = (_configRead & 0x8000) == 0x8000;
                Variables.SAVE_MODE = (byte)((_configRead & 0x0002) == 0x0002 ? 0x01 : ((_configRead & 0x0004) == 0x0004 ? 0x00 : 0x02));
                Variables.CONTROLLER_MODE = (byte)((_configRead & 0x2000) == 0x2000 ? 0x00 : ((_configRead & 0x4000) == 0x4000 ? 0x01 : 0x02));

                Helpers.Log("Configuration fetched!", 0);

                LOADED_SETTINGS = true;
            }

            // If we are in the title: Flush them.
            else if (Operations.CheckTitle() && LOADED_SETTINGS)
                LOADED_SETTINGS = false;

            // If we are seemingly in the configuration menu, and the game is paused, and we are highlighting something:
            if (_menuRead == 0x24 && _pauseRead == 0x01 && _selectPoint != 0x00)
            {
                // If we are indeed in the configuration menu:
                if (_settingsPoint != 0x00 && !DEBOUNCE[6])
                {
                    Helpers.Log("Config Menu Detected! Setting the necessary values...", 0);

                    // Read the config:
                    var _naviMap = Variables.SharpHook[(IntPtr)0x2E2E00].Execute<byte>();
                    var _cameraAuto = Variables.SharpHook[(IntPtr)0x2E2D60].Execute<byte>();
                    var _cameraHRev = Variables.SharpHook[(IntPtr)0x2E2D80].Execute<byte>();
                    var _cameraVRev = Variables.SharpHook[(IntPtr)0x2E2DA0].Execute<byte>();
                    var _commandKH2 = Variables.SharpHook[(IntPtr)0x2E2DD0].Execute<byte>();
                    var _vibrationOn = Variables.SharpHook[(IntPtr)0x2E2E40].Execute<byte>();
                    var _summonEffect = Variables.SharpHook[(IntPtr)0x2E29F0].Execute<byte>();

                    var _musicClassic = (_configRead & 0x0020) == 0x0020 ? 0x00 : 0x01;
                    var _heartlessClassic = (_configRead & 0x8000) == 0x8000 ? 0x00 : 0x01;
                    var _autoSave = (_configRead & 0x0002) == 0x0002 ? 0x01 : ((_configRead & 0x0004) == 0x0004 ? 0x00 : 0x02);
                    var _promptMode = (_configRead & 0x2000) == 0x2000 ? 0x00 : ((_configRead & 0x4000) == 0x4000 ? 0x01 : 0x02);
                    var _langAudio = (_configRead & 0x0800) == 0x0800 ? (DETECTED_EXTRA_DUB ? 0x02 : 0x01) : ((_configRead & 0x1000) == 0x1000 ? 0x01 : 0x00);

                    // Shape the array in which the menu will use.
                    SETTINGS_FIRST = new List<byte>
                    {
                        (byte)(_cameraAuto == 0x01 ? 0x00 : 0x01),
                        _cameraVRev,
                        _cameraHRev,
                        (byte)(_summonEffect == 0x02 ? 0x00 : (_summonEffect == 0x01 ? 0x01 : 0x02)),
                        (byte)(_naviMap == 0x01 ? 0x00 : 0x01),
                        Convert.ToByte(_autoSave),
                        Convert.ToByte(_langAudio),
                        Convert.ToByte(_musicClassic),
                        Convert.ToByte(_heartlessClassic),
                        Convert.ToByte(_promptMode),
                        (byte)(_vibrationOn == 0x01 ? 0x00 : 0x01),
                        (byte)(_commandKH2 == 0x01 ? 0x00 : 0x01),
                        0x00
                    };

                    // Remove addons accordingly.

                    if (CONFIG_REMOVE[2] == 0x01)
                        SETTINGS_FIRST.RemoveAt(0x08);

                    if (CONFIG_REMOVE[1] == 0x01)
                        SETTINGS_FIRST.RemoveAt(0x07);

                    if (CONFIG_REMOVE[0] == 0x01)
                        SETTINGS_FIRST.RemoveAt(0x06);

                    // Write said config to the menu.

                    Hypervisor.WriteArray(_settingsPoint, SETTINGS_FIRST.ToArray(), true);

                    Helpers.Log("Options set! Correcting the Difficulty...", 0);

                    Hypervisor.Write(_offsetTitle, _findTitle, true);
                    Hypervisor.Write(_offsetDesc, _findDesc, true);

                    Variables.SharpHook[(IntPtr)0x363380].Execute();
                    Variables.SharpHook[(IntPtr)0x363340].Execute();

                    Helpers.Log("Difficulty corrected!", 0);

                    DEBOUNCE[6] = true;
                }

                // We have entered the menu.
                if (_loadRead == 0x01)
                    ENTER_CONFIG = true;


                SETTINGS_SECOND = Hypervisor.ReadArray(_settingsPoint, SETTINGS_FIRST.Count(), true);

                /*
                 *
                 * THIS ENTIRE BLOCK IS JUST FOR THE FUCKING SCROLL BAR!
                 * 
                 * This was quite the endavour to get working. Turns out that Square Enix
                 * decided to do something correctly here and kill the ENTIRE scroll bar
                 * rather than just hiding it. So I am doing my best here using **black magic**
                 * to bring it back to life.
                 * 
                 * It works quite well, though not 1:1 with the original. Could not be bothered THAT much.
                 *
                 */

                var _pageIndex = Hypervisor.Read<byte>(_selectPoint + 0x12, true);

                var _pageCount = (CONFIG_COUNT - 0x09);
                var _pageFactor = 0x18 * _pageCount;

                var _pageOffset = (_pageFactor / _pageCount) * _pageIndex;

                Hypervisor.Write(_layoutPointer + 0x21498, 0x64 + _pageOffset, true);
                Hypervisor.Write(_layoutPointer + 0x2149C, 0x64 + _pageOffset, true);

                Hypervisor.Write(_layoutPointer + 0x21528, 0x64 + _pageOffset, true);
                Hypervisor.Write(_layoutPointer + 0x2152C, 0x64 + _pageOffset, true);

                Hypervisor.Write(_layoutPointer + 0x215B8, 0x64 - (_pageFactor + 1) + _pageOffset, true);
                Hypervisor.Write(_layoutPointer + 0x215BC, 0x64 - (_pageFactor + 1) + _pageOffset, true);

                Hypervisor.Write(_layoutPointer + 0x21568, (0xC0 - _pageFactor) * 0.01F, true);
                Hypervisor.Write(_layoutPointer + 0x2156C, (0xC0 - _pageFactor) * 0.01F, true);

                // ======================================================================================= //

                // Calculate the set config:

                // If the addons are present: Set them up accordingly.
                if (CONFIG_REMOVE[2] == 0x00)
                    Variables.ENEMY_VANILLA = Convert.ToBoolean(SETTINGS_SECOND[0x08 - CONFIG_REMOVE[1] + CONFIG_REMOVE[0]] == 0x00 ? 0x01 : 0x00);

                if (CONFIG_REMOVE[1] == 0x00)
                    Variables.MUSIC_VANILLA = Convert.ToBoolean(SETTINGS_SECOND[0x07 - CONFIG_REMOVE[0]] == 0x00 ? 0x01 : 0x00);

                if (CONFIG_REMOVE[0] == 0x00)
                    Variables.AUDIO_MODE = (byte)(DETECTED_EXTRA_DUB ? (SETTINGS_SECOND[0x06] == 0x01 ? 0x02 : 
                                                                       (SETTINGS_SECOND[0x06] == 0x02 ? 0x01 : 0x00)) : SETTINGS_SECOND[0x06]);

                // Calculate the offset for removed addons.
                var _configOffset = CONFIG_REMOVE[0] + CONFIG_REMOVE[1] + CONFIG_REMOVE[2];

                // Set the variables.
                Variables.SAVE_MODE = SETTINGS_SECOND[0x05];
                Variables.CONTROLLER_MODE = SETTINGS_SECOND[0x09 - _configOffset];

                // Calculate the bitwise to write.

                var _fieldCamBit = SETTINGS_SECOND[0x00] == 0x01 ? Variables.CONFIG_BITWISE.FIELD_CAM : Variables.CONFIG_BITWISE.OFF;
                var _cameraVerticalBit = SETTINGS_SECOND[0x01] == 0x01 ? Variables.CONFIG_BITWISE.CAMERA_V : Variables.CONFIG_BITWISE.OFF;
                var _cameraHorizontalBit = SETTINGS_SECOND[0x02] == 0x01 ? Variables.CONFIG_BITWISE.CAMERA_H : Variables.CONFIG_BITWISE.OFF;

                var _summonBit = SETTINGS_SECOND[0x03] == 0x01 ? Variables.CONFIG_BITWISE.SUMMON_PARTIAL : 
                                (SETTINGS_SECOND[0x03] == 0x00 ? Variables.CONFIG_BITWISE.SUMMON_FULL : Variables.CONFIG_BITWISE.OFF);

                var _mapBit = SETTINGS_SECOND[0x04] == 0x00 ? Variables.CONFIG_BITWISE.NAVI_MAP : Variables.CONFIG_BITWISE.OFF;

                var _autoSaveBit = SETTINGS_SECOND[0x05] == 0x00 ? Variables.CONFIG_BITWISE.AUTOSAVE_INDICATOR : 
                                  (SETTINGS_SECOND[0x05] == 0x01 ? Variables.CONFIG_BITWISE.AUTOSAVE_SILENT : Variables.CONFIG_BITWISE.OFF);

                var _audioBit = CONFIG_REMOVE[0] == 0x00 ?
                               (SETTINGS_SECOND[0x06] == (DETECTED_EXTRA_DUB ? 0x02 : 0x01) ? Variables.CONFIG_BITWISE.AUDIO_JAPANESE :
                               (SETTINGS_SECOND[0x06] == 0x01 ? Variables.CONFIG_BITWISE.AUDIO_OTHER : Variables.CONFIG_BITWISE.OFF)) : Variables.CONFIG_BITWISE.OFF;

                var _musicBit = CONFIG_REMOVE[1] == 0x00 ? 
                               (SETTINGS_SECOND[0x07 - CONFIG_REMOVE[0]] == 0x00 ? Variables.CONFIG_BITWISE.MUSIC_VANILLA : Variables.CONFIG_BITWISE.OFF) : Variables.CONFIG_BITWISE.OFF;

                var _enemyBit = CONFIG_REMOVE[2] == 0x00 ? 
                               (SETTINGS_SECOND[0x08 - (CONFIG_REMOVE[0] + CONFIG_REMOVE[1])] == 0x00 ? Variables.CONFIG_BITWISE.HEARTLESS_VANILLA : Variables.CONFIG_BITWISE.OFF) : Variables.CONFIG_BITWISE.OFF;

                var _controllerBit = SETTINGS_SECOND[0x09 - _configOffset] == 0x00 ? Variables.CONFIG_BITWISE.PROMPT_CONTROLLER :
                                    (SETTINGS_SECOND[0x09 - _configOffset] == 0x01 ? Variables.CONFIG_BITWISE.PROMPT_KEYBOARD : Variables.CONFIG_BITWISE.OFF);

                var _vibrationBit = SETTINGS_SECOND[0x0A - _configOffset] == 0x00 ? Variables.CONFIG_BITWISE.VIBRATION : Variables.CONFIG_BITWISE.OFF;
                var _commandBit = SETTINGS_SECOND[0x0B - _configOffset] == 0x01 ? Variables.CONFIG_BITWISE.COMMAND_KH2 : Variables.CONFIG_BITWISE.OFF;


                var _configBitwise =
                    _fieldCamBit |
                    _cameraVerticalBit |
                    _cameraHorizontalBit |
                    _summonBit |
                    _mapBit |
                    _autoSaveBit |
                    _audioBit |
                    _musicBit |
                    _enemyBit |
                    _controllerBit |
                    _vibrationBit |
                    _commandBit;

                // Write the current config to the save file.
                Hypervisor.Write(Variables.ADDR_Config, (ushort)_configBitwise);
            }

            // If we have exited the menu:
            else if (_selectPoint == 0x00 && ENTER_CONFIG && SETTINGS_FIRST != null && SETTINGS_SECOND != null)
            {
                if (!SETTINGS_SECOND.SequenceEqual(SETTINGS_FIRST.ToArray()))
                {
                    LOCK_AUTOSAVE = true;

                    Helpers.Log("Configuration Menu closed! Initializing full reload!", 0);
                    Helpers.Log("THE GAME MAY CRASH DURING THIS PROCESS!", 1);

                    ENTER_CONFIG = false;
                    DEBOUNCE[6] = false;

                    // Give time for the Menu to close.
                    while (Hypervisor.Read<byte>(0x399952) != 0) ;

                    Helpers.Log("Killing the BGM and Fade Handlers...", 0);

                    // Unblock the fade handler and shut the music down.
                    Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x15493A, true);
                    Variables.SharpHook[(IntPtr)0x1305F0].Execute();

                    Helpers.Log("Noting down the current world...", 0);

                    // Read the current world and event data.
                    if (AREA_READ == null)
                        AREA_READ = Hypervisor.ReadArray(Variables.ADDR_Area, 0x0A);

                    // Make a new world data to be: Twilight Town - The Empty Realm.
                    var _newArray = new byte[] { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                    // If already in TT, change the target world to OC.
                    if (AREA_READ[0] == 0x02)
                        _newArray[0] = 0x01;

                    Helpers.Log("Jumping into a DUMMY world!", 0);

                    // Initiate the jump.
                    Hypervisor.WriteArray(Variables.ADDR_Area, _newArray);
                    Variables.SharpHook[(IntPtr)0x150590].Execute(BSharpConvention.MicrosoftX64, (long)(Hypervisor.BaseAddress + Variables.ADDR_Area), 2, 0, 0, 0);

                    // Wait until the fade has been completed.
                    while (Hypervisor.Read<byte>(0x55472D) != 0x80) ;

                    // Destroy the fade handler so it does not cause issues.
                    Hypervisor.WriteArray(Hypervisor.PureAddress + 0x15493A, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                    Hypervisor.Write<byte>(0x55472D, 0x80);

                    // Whilst not loaded, constantly shut off the music.
                    while (Hypervisor.Read<byte>(Variables.ADDR_LoadFlag) != 1)
                        Variables.SharpHook[(IntPtr)0x1305F0].Execute();

                    // Execute the function for JumpEffect again so we are stuck.
                    Variables.SharpHook[(IntPtr)0x154120].Execute(0x02);

                    Helpers.Log("Jump complete! Jumping back!", 0);

                    // Atfer load, jump back to where we came from.
                    Hypervisor.WriteArray(Variables.ADDR_Area, AREA_READ);
                    Variables.SharpHook[(IntPtr)0x150590].Execute(BSharpConvention.MicrosoftX64, (long)(Hypervisor.BaseAddress + Variables.ADDR_Area), 2, 0, 0, 0);

                    // Wait until load.
                    while (Hypervisor.Read<byte>(Variables.ADDR_LoadFlag) != 1) ;

                    // Restore the fade initiater after load.
                    Hypervisor.WriteArray(Hypervisor.PureAddress + 0x15493A, new byte[] { 0xF3, 0x0F, 0x11, 0x8F, 0x0C, 0x01, 0x00, 0x00 }, true);

                    Helpers.Log("If you can see this, the jump was successful without a crash!", 0);

                    // Flush the world data.
                    AREA_READ = null;
                    LOCK_AUTOSAVE = false;
                }

                else
                {
                    ENTER_CONFIG = false;
                    DEBOUNCE[6] = false;
                    AREA_READ = null;
                    LOCK_AUTOSAVE = false;
                }
            }
        }

        /// <summary>
        /// Determine whether to skip or go through Roxas' story.
        /// This is detemined by the Vibration option at the Title Screen.
        /// 
        /// Created by Y2K, improved upon and ported by TopazTK.
        /// </summary>
        public static void TutorialSkip()
        {
            if (Operations.CheckTitle() && SKIP_STAGE != 0)
            {
                Helpers.Log("Title Screen detected! Resetting Roxas Skip!", 0);
                SKIP_STAGE = 0;
                SKIP_ROXAS = false;
            }

            var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_GameDifficulty);
            var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleOptionSelect);

            if (!Operations.CheckTitle())
            {
                var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_Area);
                var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
                var _eventCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x04);

                var _checkRoxas = _worldCheck == 0x02 && _roomCheck == 0x01 && _eventCheck == 0x38;

                if (!_checkRoxas && _selectButton == 0x00 && SKIP_STAGE == 3)
                {
                    Helpers.Log("Loaded game abandoned. Re-enabling Roxas Skip!", 0);
                    SKIP_STAGE = 0;
                }

                if (_checkRoxas && SKIP_STAGE == 0)
                {
                    if (SKIP_ROXAS)
                    {
                        Helpers.Log("Room and Settings are correct! Initiating Roxas Skip's First Phase...", 0);

                        Hypervisor.Write<uint>(Variables.ADDR_Area, 0x322002);
                        Hypervisor.Write<uint>(Variables.ADDR_Area + 0x04, 0x01);
                        Hypervisor.Write<uint>(Variables.ADDR_Area + 0x08, 0x01);

                        Hypervisor.Write<uint>(0x444832, 0x1FF00001);
                        Hypervisor.Write<uint>(0x444832 + 0x04, 0x00000000);

                        SKIP_STAGE = 1;
                    }

                    else
                    {
                        Helpers.Log("Room is correct but settings are not! Disabling Roxas Skip...", 0);
                        SKIP_STAGE = 3;
                    }
                }

                else if (_worldCheck == 0x02 && _roomCheck == 0x20 && _eventCheck == 0x9A && SKIP_STAGE == 1)
                {
                    Helpers.Log("Room parameters correct! Skip was initiated! Initiating Roxas Skip's Second Phase...", 0);

                    Hypervisor.Write<uint>(Variables.ADDR_Area, 0x001702);
                    Hypervisor.Write<uint>(Variables.ADDR_Area + 0x04, (0x02 << 10) + 0x02);
                    Hypervisor.Write<uint>(Variables.ADDR_Area + 0x08, 0x02);

                    Hypervisor.Write<byte>(Variables.ADDR_InventoryFlag, 0x9F);
                    Hypervisor.WriteArray(Variables.ADDR_StoryFlag, Variables.VALUE_StoryFlag);

                    if (_diffRead == 0x03)
                    {
                        Hypervisor.Write<byte>(0x445056, 0x18);
                        Hypervisor.Write<byte>(0x445056 + 0x01, 0x18);

                        Hypervisor.WriteArray(
                        0x4450A6,
                        new byte[]
                        {
                            0xF8,
                            0x00,
                            0x89,
                            0x01,
                            0x88,
                            0x01,
                            0xA5,
                            0x01,
                            0x94,
                            0x81,
                            0x97,
                            0x01,
                            0x97,
                            0x01,
                            0x95,
                            0x01,
                            0x52,
                            0x00,
                            0x8A,
                            0x00,
                            0x9E,
                            0x00
                        }
                        );
                    }

                    else
                    {
                        Hypervisor.Write<byte>(0x445056, 0x1E);
                        Hypervisor.Write<byte>(0x445056 + 0x01, 0x1E);
                        Hypervisor.WriteArray(
                            0x4450A6,
                            new byte[] { 0x52, 0x00, 0x8A, 0x00, 0x9E, 0x00 }
                        );
                    }

                    Hypervisor.Write<byte>(0x446262, 0x04);

                    Hypervisor.Write<byte>(0x446262 + 0x08, 0x06);
                    Hypervisor.Write<byte>(0x446262 + 0x0A, 0x40);
                    Hypervisor.Write<byte>(0x446262 + 0x0D, 0x02);

                    Hypervisor.Write<byte>(0x446229, 0x80);
                    Hypervisor.Write<byte>(0x44622B, 0x80);

                    Helpers.Log("Roxas Skip has been completed!", 0);
                    SKIP_STAGE = 2;
                }

                else if (_selectButton == 0x01 && SKIP_STAGE == 0)
                {
                    Helpers.Log("Loaded game detected! Disabling Roxas Skip...", 0);
                    SKIP_STAGE = 3;
                }
            }
        }

        /// <summary>
        /// Handles the crown on Sora's HUD corresponding to the crown he wears on his model.
        /// Must be ran on a separate task.
        /// </summary>
        public static void CrownManager()
        {
            // Prepare the suffic according to the language.
            var _suffixFile = (Variables.AUDIO_MODE == 0x00) ? ".a.us" : (Variables.AUDIO_MODE == 0x01) ? ".a.jp" : ".a.ks";

            // Read the values.
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _formRead = Hypervisor.Read<byte>(Variables.ADDR_PlayerForm);
            var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

            // If on the title, or the room ain't loaded, or the form changed: Wipe the cache.
            if (Operations.CheckTitle() || _loadRead == 0x00 || PAST_FORM != _formRead)
            {
                LOAD_LIST = null;
                PAST_FORM = _formRead;
            }

            RELOAD_POINT:

            // If not in a cutscene or the Title Screen, and the room is loaded:
            if (!Operations.CheckTitle() && _loadRead == 0x01 && _cutsceneRead == 0x00)
            {
                // Find the .a.xx files in game cache.
                LOAD_LIST = new ulong[]
                {
                    Operations.FindFile("obj/P_EX100" + _suffixFile),
                    Operations.FindFile("obj/P_EX100_BTLF" + _suffixFile),
                    Operations.FindFile("obj/P_EX100_MAGF" + _suffixFile),
                    Operations.FindFile("obj/P_EX100_KH1F" + _suffixFile),
                    Operations.FindFile("obj/P_EX100_TRIF" + _suffixFile),
                    Operations.FindFile("obj/P_EX100_ULTF" + _suffixFile),
                    Operations.FindFile("obj/P_EX100_HTLF" + _suffixFile),
                };

                // Fetch the pointers to the files.
                var _soraPoints = new ulong[]
                {
                    LOAD_LIST[0] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[0] + 0x58, true) : 0x00,
                    LOAD_LIST[1] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[1] + 0x58, true) : 0x00,
                    LOAD_LIST[2] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[2] + 0x58, true) : 0x00,
                    LOAD_LIST[3] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[3] + 0x58, true) : 0x00,
                    LOAD_LIST[4] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[4] + 0x58, true) : 0x00,
                    LOAD_LIST[5] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[5] + 0x58, true) : 0x00,
                    LOAD_LIST[6] != Hypervisor.MemoryOffset ? Hypervisor.Read<ulong>(LOAD_LIST[6] + 0x58, true) : 0x00,
                };

                // Calculate the crown.
                var _crownRead = Hypervisor.ReadArray(0x442B62 + 0x36B2, 0x03);
                var _crownSum = _crownRead[0] + _crownRead[1] + _crownRead[2];

                // If something occured whilst reading: Re-fetch everything.
                if (LOAD_LIST[_formRead] == ULONG_MINIMAL || _soraPoints[_formRead] > 0x7FFF00000000)
                {
                    LOAD_LIST = null;
                    goto RELOAD_POINT;
                }

                // For every .a.xx found:
                foreach (var _point in _soraPoints)
                {
                    // If the .a.xx file is valid:
                    if (_point != 0x00)
                    {
                        // Fetch the offsets to the coords.
                        var _barOffset = Hypervisor.Read<uint>(_point + 0x08, true);
                        var _soraOffset = Hypervisor.Read<uint>(_point + 0x38, true) - _barOffset;

                        var _faceCheck = Hypervisor.Read<uint>(_point + 0x24, true);

                        // Ensure the .a.xx has a face.
                        if (_faceCheck != 0x65636166)
                            return;

                        // Calculate the positions.
                        var _topValue = 0x00 + _crownSum * 0x5A;
                        var _bottomValue = 0x5D + _crownSum * 0x5A;

                        // Write the positions.
                        for (uint i = 0; i < 3; i++)
                        {
                            Hypervisor.Write(_point + _soraOffset + 0x38 + (0x2C * i), _topValue, true);
                            Hypervisor.Write(_point + _soraOffset + 0x40 + (0x2C * i), _bottomValue, true);
                        }
                    }
                }
            }
        }


        #endregion

        #region Additional In-Game Functionality

        /// <summary>
        /// Adds the option to retry a past fight.
        /// It also adds the option to prepare before retrying.
        /// </summary>
        public static void RetryPrompt()
        {
            // So, we initialize all of this shit just to determine whether Retry will show up or not.

            var _menuRead = Hypervisor.Read<int>(Variables.ADDR_MenuType);
            var _selectRead = Hypervisor.Read<byte>(Variables.ADDR_MenuSelect);

            var _gameOverRead = Hypervisor.Read<ulong>(Variables.PINT_GameOver);
            var _gameOverOptions = Hypervisor.Read<ulong>(Variables.PINT_GameOverOptions);

            var _subMenuRead = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);
            var _finishRead = Hypervisor.Read<byte>(Variables.ADDR_FinishFlag);

            var _pauseRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);
            var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
            var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

            var _warpRead = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);

            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
            var _eventRead = Hypervisor.Read<ushort>(Variables.ADDR_Area + 0x04);

            var _nullArray = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 };

            // Check for Hades Escape
            var _isEscape = _worldRead == 0x06 && _roomRead == 0x05 && _eventRead == 0x6F;

            // No-No Areas for Retry.
            var _blacklistCheck =
               (_worldRead == 0x04 && _roomRead >= 0x15 && _roomRead <= 0x1A) // Cavern of Remembrance
            || (_worldRead == 0x06 && _roomRead == 0x09 && _eventRead >= 0xBD && _eventRead <= 0xC4); // Olympus Cups

            if (!_blacklistCheck && !Operations.CheckTitle())
            {
                // The battle state will also check **specifically** for Hades Escape.
                var _battleState = _battleRead == 0x02 && _cutsceneRead == 0x00;

                // Read the necessary shits at the start of a fight.
                if (_battleState && _pauseRead == 0x00 && !STATE_COPIED)
                {
                    Helpers.Log("Start of forced fight, making a copy of the current state...", 0);

                    byte[] _tempArray = new byte[18];
                    Buffer.BlockCopy(Variables.ARRY_ContinueOptions[Variables.RETRY_DEFAULT ? 0x01 : 0x02], 0, _tempArray, 0, 18);

                    if (_isEscape)
                    {
                        HADES_COUNT = 0;
                        Helpers.Log("Hades Escape Detected! Adjusting Retry Logic!", 0);
                    }

                    Hypervisor.WriteArray(_gameOverOptions + 0x34A, _tempArray, true);

                    var _currentSave = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x10FC0);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + 0x7A0000, _currentSave, true);

                    ROXAS_KEYBLADE = Hypervisor.Read<ushort>(Hypervisor.BaseAddress + 0x445052, true);
                    STATE_COPIED = true;

                    Helpers.Log("State copied successfully!", 0);
                }

                // Flush the memory post-fight.
                else if (!_battleState && _pauseRead == 0x00 && STATE_COPIED && !(_isEscape && _battleRead == 0x01))
                {
                    Helpers.Log("The player is out of battle. Flushing memory...", 0);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + 0x7A0000, new byte[0x10FC0], true);

                    if (RETRY_MODE != 0x00)
                    {
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);
                        RETRY_MODE = 0x00;
                    }

                    byte[] _tempArray = new byte[18];
                    Buffer.BlockCopy(Variables.ARRY_ContinueOptions[0x00], 0, _tempArray, 0, 18);

                    Hypervisor.WriteArray(_gameOverOptions + 0x34A, _tempArray, true);

                    STATE_COPIED = false;
                    ROXAS_KEYBLADE = 0x0000;
                }


                // Count the amount of "done" battles in Hades Escape.
                if (_isEscape && _battleRead == 0x01 && !DEBOUNCE[7])
                {
                    HADES_COUNT += 1;
                    DEBOUNCE[7] = true;
                    Helpers.Log("Hades Escape: Incrementing Clear Count!", 0);
                }

                if (_isEscape && _battleRead == 0x02 && DEBOUNCE[7])
                    DEBOUNCE[7] = false;

                // If the game is over and the state is copied beforehand:
                if (_gameOverRead != 0x00 && STATE_COPIED)
                {
                    // Restore Roxas' Keyblade.
                    var _roxasTemp = Hypervisor.Read<ushort>(0x445052);
                    var _charRead = Hypervisor.Read<ushort>(0x24BE4B2);

                    if (_roxasTemp != ROXAS_KEYBLADE && _charRead == 0x5A)
                    {
                        Hypervisor.Write(0x446B4C, _roxasTemp);
                        Hypervisor.Write(0x445052, ROXAS_KEYBLADE);
                    }

                    // Calculate what button will do what.
                    var _retryButton = Variables.RETRY_DEFAULT ? 0x00 : 0x01;
                    var _prepareButton = Variables.RETRY_DEFAULT ? 0x01 : 0x02;
                    var _continueButton = Variables.RETRY_DEFAULT ? 0x02 : 0x00;

                    // Calculate where the Continue function looks for the save, and our save.
                    var _calculatePointer = Hypervisor.PureAddress - Hypervisor.MemoryOffset + 0x39DECB;
                    var _calculateSave = Hypervisor.PureAddress - Hypervisor.MemoryOffset + 0x7A0000;

                    // Restore if the Load Menu is present.
                    if (_subMenuRead >= 0x04 && _warpRead == 0x90 && RETRY_MODE >= 0x01)
                    {
                        Helpers.Log("User is loading a save! Restoring functions...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                        RETRY_MODE = 0x00;
                    }

                    // Destroy if the button is Retry.
                    if (_selectRead == _retryButton && RETRY_MODE != 0x01)
                    {
                        Helpers.Log("Switched to Retry mode! Destroying ...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, new byte[] { 0x48, 0x8D, 0x15 }, true);

                        Hypervisor.Write(Hypervisor.PureAddress + Variables.ADDR_InventoryINST + 0x03, (uint)(_calculateSave - _calculatePointer), true);

                        RETRY_MODE = 0x01;
                        PREPARE_MODE = 0x00;
                    }

                    // Restore if the button is Continue.
                    else if (_selectRead == _continueButton && RETRY_MODE != 0x00)
                    {
                        Helpers.Log("Switched to Continue mode! Restoring...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                        RETRY_MODE = 0x00;
                        PREPARE_MODE = 0x00;
                    }

                    // Read the beginning save state, then destroy if the button is Prepare.
                    else if (_selectRead == _prepareButton && RETRY_MODE != 0x02)
                    {
                        Helpers.Log("Switched to Prepare Mode! Destroying...", 0);

                        var _currentSave = Hypervisor.ReadArray(Hypervisor.PureAddress + 0x7A0000, 0x10FC0, true);
                        Hypervisor.WriteArray(Variables.ADDR_SaveData, _currentSave);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, new byte[] { 0x48, 0x8D, 0x15 }, true);

                        Hypervisor.Write(Hypervisor.PureAddress + Variables.ADDR_InventoryINST + 0x03, (uint)(_calculateSave - _calculatePointer), true);

                        RETRY_MODE = 0x02;
                        PREPARE_MODE = 0x01;
                    }
                }

                // If the death menu is gone, and Prepare and Retry was executed:
                else
                {
                    // Call the menu.
                    if (PREPARE_MODE == 0x01 && _menuRead != 0x08)
                        Variables.SharpHook[(IntPtr)0x2E23A0].Execute(BSharpConvention.MicrosoftX64, 0, 0);

                    // Acknowledge the menu was called.
                    else if (_menuRead == 0x08 && PREPARE_MODE == 0x01)
                        PREPARE_MODE = 0x02;

                    else if (PREPARE_MODE == 0x02 && _menuRead != 0x08)
                    {
                        Helpers.Log("Prepare finished! Copying the save state...", 0);
                        var _currentSave = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x10FC0);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + 0x7A0000, _currentSave, true);
                        PREPARE_MODE = 0x03;
                    }
                }

                // If the battle has ended (Death, Kill, Hades Clear, or Cutscene), restore the functions.
                if (((_finishRead == 0x01 || _cutsceneRead != 0x00) && RETRY_MODE != 0x00) || (_isEscape && HADES_COUNT == 3))
                {
                    Helpers.Log("End of battle detected! Restoring...", 0);

                    if (_isEscape)
                    {
                        HADES_COUNT = 255;
                        Helpers.Log("Battle ended on a special case: Hades Escape.", 0);
                    }

                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                    RETRY_MODE = 0x00;
                }

                if (RETRY_BLOCK)
                {
                    Helpers.Log(String.Format("Out of the problematic area... Unlocking Retry Capabilities..."), 0);
                    RETRY_BLOCK = false;
                }
            }

            // If in the blacklisted areas: Do not run Retry.
            else if (!RETRY_BLOCK && !Operations.CheckTitle())
            {
                Helpers.Log(String.Format("Problematic area detected! Locking Retry Capabilities..."), 0);

                RETRY_BLOCK = true;
            }
        }

        /// <summary>
        /// The logic code for the Encounter Plus ability.
        /// Must have an accompanying patch.
        /// </summary>
        public static void LogicEP()
        {
            // The pointer for the current room's enemy information.
            var _roomPoint = Hypervisor.Read<ulong>(Variables.PINT_EnemyInfo) + 0x08;

            // Reading the current values into memory.
            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _abilityRead = Hypervisor.ReadArray(Variables.ADDR_AbilityStart, 0xC0);

            // Read in the current player's ability data.
            ushort[] _abilityData = new ushort[0x60];
            Buffer.BlockCopy(_abilityRead, 0, _abilityData, 0, 0xC0);

            // Check if the game is loaded, and if the player has Encounter Plus in anyway.
            if (!Operations.CheckTitle() && _roomRead == 0x01 && !DEBOUNCE[2] && !_abilityData.Contains<ushort>(0x80F8) && !_abilityData.Contains<ushort>(0x00F8))
            {
                // If not, give them the ability.
                var _fetchIndex = Array.FindIndex(_abilityData, x => x == 0x0000);

                Helpers.Log("Encounter Plus has been added to the inventory!", 0);

                Hypervisor.Write<ushort>(Variables.ADDR_AbilityStart + (ulong)(_fetchIndex * 0x02), 0x00F8);
                DEBOUNCE[2] = true;
            }

            else if (Operations.CheckTitle() && DEBOUNCE[2])
                DEBOUNCE[2] = false;

            if (_roomRead == 0x00 && _abilityData.Contains<ushort>(0x80F8) && !DEBOUNCE[3] && AREA_READ == null)
            {
                Helpers.Log("Enemy data has been cleared!", 0);
                Hypervisor.WriteArray(_roomPoint, new byte[0x100], true);
                DEBOUNCE[3] = true;
            }

            else if (_roomRead == 0x01 && DEBOUNCE[3])
                DEBOUNCE[3] = false;
        }

        #endregion

        #region Visual Enhancements

        /// <summary>
        /// Code to adjust the Aspect Ratio for Ultrawide users.
        /// May require tweaking. WIP, locked behind a config option.
        /// </summary>
        public static void AspectAdjust()
        {
            Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x100000, true);
            Hypervisor.WriteArray(Hypervisor.PureAddress + 0x12AA20, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);

            var _baseRatio = 16 / 9;
            ulong _widthPaxAddr = 0x89E9B0 - 0x56454E;

            ulong _widthWindowAddr = 0x89E9B8 - 0x56454E;
            ulong _heightWindowAddr = 0x89E9BC - 0x56454E;

            ulong _widthRenderAddr = 0x89EB04 - 0x56454E;
            ulong _heightRenderAddr = 0x89E9B4 - 0x56454E;

            ulong _widthViewportAddr = 0x89E9C0 - 0x56454E;
            ulong _heightViewportAddr = 0x89E9C4 - 0x56454E;

            ulong _uiWidthAddr = 0x89E9E8 - 0x56454E;
            float _prevWidthUI = 640F;

            var _widthRender = Hypervisor.Read<float>(_widthRenderAddr);
            var _heightRender = Hypervisor.Read<float>(_heightRenderAddr);

            var _readButton = Hypervisor.Read<ushort>(Variables.ADDR_Input);

            if (_readButton == 0x00AF)
            {
                PAST_WIDTH = 0x00;
                PAST_HEIGHT = 0x00;
                RATIO_DETECT = 0x00;
            }

            if (_widthRender == 0x00)
            {
                _widthRender = Hypervisor.Read<float>(_widthWindowAddr);
                Hypervisor.Write(_widthRenderAddr, _widthRender);
            }

            if (_heightRender == 0x00)
            {
                _heightRender = Hypervisor.Read<float>(_heightWindowAddr);
                Hypervisor.Write(_heightRenderAddr, _heightRender);
            }

            if ((PAST_WIDTH != _widthRender || PAST_HEIGHT != _heightRender))
            {
                var _widthExpect = 0.0F;
                var _heightExpect = (float)Math.Floor(_widthRender / _baseRatio + 0.5F);

                if (_heightExpect > _heightRender)
                {
                    _heightExpect = _heightRender;
                    _widthExpect = (float)Math.Floor(_heightRender * _baseRatio + 0.5F);
                }

                else
                    _widthExpect = _widthRender;

                if (_heightExpect < _heightRender)
                {
                    var _ratioHeight = _heightRender / _heightExpect;

                    Hypervisor.Write(_heightViewportAddr, _ratioHeight);
                    Hypervisor.Write(_widthViewportAddr, 1F);

                    Thread.Sleep(100);
                }

                else if (_widthExpect < _widthRender)
                {
                    var _ratioWidth = _widthRender / _widthExpect;

                    Hypervisor.Write(_widthViewportAddr, _ratioWidth);
                    Hypervisor.Write(_heightViewportAddr, 1F);

                    Thread.Sleep(100);

                    var _uiWidth = _prevWidthUI / _ratioWidth;
                    Hypervisor.Write(_uiWidthAddr, _uiWidth);

                    Thread.Sleep(100);
                }

                else
                {
                    Hypervisor.Write(_heightViewportAddr, 1F);
                    Hypervisor.Write(_widthViewportAddr, 1F);

                    Thread.Sleep(100);
                }

                Hypervisor.Write(_widthPaxAddr, _widthRender);

                PAST_WIDTH = _widthRender;
                PAST_HEIGHT = _heightRender;
            }

            int _positiveOffset = 0x55;
            int _negativeOffset = -0x55;

            switch (Math.Round(_widthRender / _heightRender, 2))
            {
                case 2.37:
                    _positiveOffset = 0x00C0;
                    _negativeOffset = -0x00C0;
                    break;
                case 3.55:
                case 3.56:
                    _positiveOffset = 0x01A5;
                    _negativeOffset = -0x01A5;
                    break;
            }

            if (_positiveOffset != RATIO_DETECT)
            {
                Hypervisor.Write(Hypervisor.PureAddress + 0x18B906, _negativeOffset, true); // Information Bar
                Hypervisor.Write(Hypervisor.PureAddress + 0x18B93A, _negativeOffset, true); // Information Bar

                Hypervisor.Write(Hypervisor.PureAddress + 0x15C15C, _negativeOffset, true); // Command Menu

                Hypervisor.Write(Hypervisor.PureAddress + 0x17F989, _positiveOffset, true); // Party HUD

                Hypervisor.Write(Hypervisor.PureAddress + 0x178186, _positiveOffset, true); // Radar Cursor
                Hypervisor.Write(Hypervisor.PureAddress + 0x1781C0, _positiveOffset, true); // Radar Map

                Hypervisor.Write(Hypervisor.PureAddress + 0x180A4C, _positiveOffset, true); // Enemy HP [Header]
                Hypervisor.Write(Hypervisor.PureAddress + 0x180A86, _positiveOffset, true); // Enemy HP ???
                Hypervisor.Write(Hypervisor.PureAddress + 0x180AB5, _positiveOffset, true); // Enemy HP [Backdrop]
                Hypervisor.Write(Hypervisor.PureAddress + 0x180AE4, _positiveOffset, true); // Enemy HP [Tail]
                Hypervisor.Write(Hypervisor.PureAddress + 0x180B13, _positiveOffset, true); // Enemy HP [Damage]
                Hypervisor.Write(Hypervisor.PureAddress + 0x180B42, _positiveOffset, true); // Enemy HP [Main Bar]
                Hypervisor.Write(Hypervisor.PureAddress + 0x180C49, _positiveOffset, true); // Enemy HP [Extra Bars]

                Hypervisor.Write(Hypervisor.PureAddress + 0x17CEBB, _positiveOffset, true); // Sora Backdrop
                Hypervisor.Write(Hypervisor.PureAddress + 0x17CF13, _positiveOffset, true); // Sora HP [Backdrop]
                Hypervisor.Write(Hypervisor.PureAddress + 0x17CF5A, _positiveOffset, true); // Sora HP [Complete]
                Hypervisor.Write(Hypervisor.PureAddress + 0x17CF7F, _positiveOffset, true); // Sora Face
                Hypervisor.Write(Hypervisor.PureAddress + 0x17CF99, _positiveOffset, true); // Sora MP [Header]
                Hypervisor.Write(Hypervisor.PureAddress + 0x17CFC9, _positiveOffset, true); // Sora MP [Backdrop]
                Hypervisor.Write(Hypervisor.PureAddress + 0x17CFF9, _positiveOffset, true); // Sora MP [Bar]
                Hypervisor.Write(Hypervisor.PureAddress + 0x17D029, _positiveOffset, true); // Sora ???
                Hypervisor.Write(Hypervisor.PureAddress + 0x17D1E5, _positiveOffset, true); // Sora Drive
                Hypervisor.Write(Hypervisor.PureAddress + 0x17D219, _positiveOffset, true); // Sora ???

                Hypervisor.Write(Hypervisor.PureAddress + 0x17E7BF, _positiveOffset, true); // Drive Return
                Hypervisor.Write(Hypervisor.PureAddress + 0x17F5A9, _positiveOffset, true); // Drive ???
                Hypervisor.Write(Hypervisor.PureAddress + 0x17F17D, _positiveOffset, true); // ???
                Hypervisor.Write(Hypervisor.PureAddress + 0x17E5F8, _positiveOffset, true); // Drive Transform

                Hypervisor.Write(Hypervisor.PureAddress + 0x17EA5A, _positiveOffset, true); // Summon Backdrop
                Hypervisor.Write(Hypervisor.PureAddress + 0x17EAFF, _positiveOffset, true); // Summon Face

                RATIO_DETECT = _positiveOffset;
            }
        }

        /// <summary>
        /// Enforces the controller prompts instead of detecting KBM when requested.
        /// </summary>
        public static void PromptSelector()
        {
            var _contCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);

            if (Variables.CONTROLLER_MODE != 0x02)
            {
                if (_contCheck != 0x90)
                {
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                    Helpers.Log("Manual prompt mode detected! Enforcing prompts...", 0);
                }

                Hypervisor.Write(Variables.ADDR_ControllerMode, Variables.CONTROLLER_MODE);
            }

            else
            {
                if (_contCheck == 0x90)
                {
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, new byte[] { 0xC7, 0x05, 0x64, 0xA4, 0x65, 0x02, 0x01, 0x00, 0x00, 0x00 }, true);
                    Helpers.Log("Automatic prompt mode detected! Restoring the seeker...", 0);
                }
            }
        }

        /// <summary>
        /// Fixes the behaviour of the Save Point to better reflect it's PS2 counterpart.
        /// </summary>
        public static void FixSavePoint()
        {
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _reactionRead = Hypervisor.Read<ushort>(Variables.ADDR_ReactionID);

            if (SAVE_RESET)
            {
                Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, 0x75, true);
                SAVE_RESET = false;
            }

            if (_loadRead == 0x00 && !SAVE_RESET)
                SAVE_RESET = true;

            if (_reactionRead == 0x0037)
            {
                var _healthRead = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP);
                var _magicRead = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x180);

                var _healthMax = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x04);
                var _magicMax = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x184);

                if (_healthRead == _healthMax && _magicRead == _magicMax)
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, 0x74, true);

                else
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, 0x75, true);
            }
        }

        #endregion

        #region Switch-Up Functions

        /// <summary>
        /// Switches between English and Japanese dialogue on demand.
        /// </summary>
        public static void SwitchAudio()
        {
            var _stringCheck = Hypervisor.ReadTerminate(Variables.ADDR_EVTFormatter);

            var _stringANM = "anm/{0}/";
            var _stringPAX = "obj/%s.a.{0}";
            // var _stringLMT = "limit/{0}/%s";
            var _stringEVT = "voice/{0}/event";
            var _stringBTL = "voice/{0}/battle";
            var _stringGMI = "voice/{0}/gumibattle/gumi.win32.scd";

            if (Variables.AUDIO_MODE == 0x01 && _stringCheck != "voice/jp/event/")
            {
                Helpers.Log("Switching to Japanese Audio...", 0);

                WL_SUFF = "jp";
                US_SUFF = "jp";
                FM_SUFF = "jp";
            }

            if (Variables.AUDIO_MODE == 0x02 && _stringCheck != "voice/ks/event/" && DETECTED_EXTRA_DUB)
            {
                Helpers.Log("Switching to Extra Audio...", 0);

                WL_SUFF = "ks";
                US_SUFF = "ks";
                FM_SUFF = "ks";
            }

            if (Variables.AUDIO_MODE == 0x00 && _stringCheck != "voice/us/event/")
            {
                Helpers.Log("Switching to English Audio...", 0);

                WL_SUFF = "%s";
                US_SUFF = "us";
                FM_SUFF = "fm";
            }

            if (_stringCheck != String.Format(_stringPAX, WL_SUFF))
            {
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter, String.Format(_stringPAX, WL_SUFF));
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter + 0x10, String.Format(_stringPAX, US_SUFF));

                Hypervisor.WriteString(Variables.ADDR_ANBFormatter, String.Format(_stringANM, US_SUFF));
                Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, String.Format(_stringANM, FM_SUFF));

                if (Variables.AUDIO_MODE == 0x02)
                {
                    Hypervisor.WriteString(Variables.ADDR_ANBFormatter, String.Format(_stringANM, "us"));
                    Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, String.Format(_stringANM, "fm"));
                }
                
                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, String.Format(_stringBTL, US_SUFF));
                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, String.Format(_stringEVT, US_SUFF));

                /*
                 * 
                if (Variables.AUDIO_MODE != 0x00)
                {
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x3E4FCF, 0xEB, true);
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x3E5960, 0xEB, true);
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x3E596A, 0xEB, true);
                }

                else
                {
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x3E4FCF, 0x74, true);
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x3E5960, 0x74, true);
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x3E596A, 0x74, true);
                }

                Hypervisor.WriteString(0x62A02, String.Format(_stringLMT, FM_SUFF));

                */

                Hypervisor.WriteString(0x4F27A, String.Format(_stringGMI, US_SUFF));
                Hypervisor.WriteString(0x81FF00, WL_SUFF);
            }
        }

        /// <summary>
        /// Switches between Classic and Remastered music on demand.
        /// </summary>
        public static void SwitchMusic()
        {
            if (Variables.MUSIC_VANILLA != PAST_MUSIC)
            {
                Helpers.Log(String.Format("Switching Music to {0}...", Variables.MUSIC_VANILLA ? "Vanilla" : "Remastered"), 0);

                var _musicPrefix = Variables.MUSIC_VANILLA ? new byte[] { 0x70, 0x73, 0x32, 0x6D, 0x64 } : new byte[] { 0x6D, 0x75, 0x73, 0x69, 0x63 };
                Hypervisor.WriteArray(Variables.ADDR_MusicPath, _musicPrefix);

                PAST_MUSIC = Variables.MUSIC_VANILLA;
            }
        }

        /// <summary>
        /// Switches between Classic and Final Mix Heartless on demand.
        /// </summary>
        public static void SwitchEnemies()
        {
            byte _bossPrefix = Variables.ENEMY_VANILLA ? (byte)0x56 : (byte)0x42;
            byte _enemyPrefix = Variables.ENEMY_VANILLA ? (byte)0x56 : (byte)0x4D;

            if (OBJENTRY_READ == null)
            {
                var _headerCheck = Hypervisor.Read<byte>(Variables.ADDR_ObjentryBASE);
                var _itemCount = Hypervisor.Read<int>(Variables.ADDR_ObjentryBASE + 0x04);

                if (_headerCheck == 0x03)
                    OBJENTRY_READ = Hypervisor.ReadArray(Variables.ADDR_ObjentryBASE + 0x08, 0x60 * _itemCount);
            }

            if (OBJENTRY_READ != null)
            {
                if (Operations.CheckTitle())
                    OBJENTRY_READ = null;

                else if (Variables.ENEMY_VANILLA != PAST_ENEMY)
                {
                    Helpers.Log(String.Format("Switching Enemies to the {0} Palette...", Variables.ENEMY_VANILLA ? "Classic" : "Special"), 0);

                    foreach (var _name in Variables.BOSSObjentry)
                    {
                        var _stringArr1 = Encoding.Default.GetBytes(_name);
                        var _stringArr2 = Encoding.Default.GetBytes(_name.Replace("B_", "V_"));

                        var _searchClassic = OBJENTRY_READ.FindValue(_stringArr2);
                        var _searchRemastered = OBJENTRY_READ.FindValue(_stringArr1);

                        if (_searchClassic == ULONG_MINIMAL && _searchRemastered == ULONG_MINIMAL)
                            break;

                        else
                            Hypervisor.Write(Variables.ADDR_ObjentryBASE + 0x08 + (_searchClassic == ULONG_MINIMAL ? _searchRemastered : _searchClassic), _bossPrefix);
                    }

                    foreach (var _name in Variables.ENEMYObjentry)
                    {
                        var _stringArr1 = Encoding.Default.GetBytes(_name);
                        var _stringArr2 = Encoding.Default.GetBytes(_name.Replace("M_", "V_"));

                        var _searchClassic = OBJENTRY_READ.FindValue(_stringArr2);
                        var _searchRemastered = OBJENTRY_READ.FindValue(_stringArr1);

                        if (_searchClassic == ULONG_MINIMAL && _searchRemastered == ULONG_MINIMAL)
                            break;

                        else
                            Hypervisor.Write(Variables.ADDR_ObjentryBASE + 0x08 + (_searchClassic == ULONG_MINIMAL ? _searchRemastered : _searchClassic), _enemyPrefix);
                    }

                    PAST_ENEMY = Variables.ENEMY_VANILLA;
                }
            }
        }

        #endregion


        public static void Execute()
        {
            try
            {
                if (!Variables.Initialized)
                    Initialization();

                else
                {
                    AllowGOA();
                    PromptSelector();

                    if (CONFIG_REMOVE[0] == 0x00)
                        SwitchAudio();

                    if (CONFIG_REMOVE[1] == 0x00)
                        SwitchMusic();

                    if (CONFIG_REMOVE[2] == 0x00)
                        SwitchEnemies();

                    RegisterMagic();

                    ExitFix();
                    LogicEP();
                    MagicSorting();
                    FixSavePoint();

                    if (!DISABLE_SKIP)
                        TutorialSkip();

                    ShortcutForms();
                    OverrideLimits();
                    OverrideLimiter();

                    RetryPrompt();

                    if (Variables.RATIO_ADJUST)
                        AspectAdjust();

                    ResetGame();
                    NewGameHandler();
                    ConfigHandler();

                    #region Tasks
                    if (Variables.DCTask == null || Variables.DCTask.IsFaulted || Variables.DCTask.IsCanceled)
                    {
                        Variables.DCTask = Task.Factory.StartNew(

                            delegate ()
                            {
                                while (!Variables.Token.IsCancellationRequested)
                                {
                                    if (!LOCK_AUTOSAVE)
                                        AutosaveEngine();

                                    DiscordEngine();
                                    CrownManager();

                                    Thread.Sleep(100);
                                }
                            },

                            Variables.Token
                        );
                    }

                    if (Variables.DUALSENSE_TOGGLE)
                    {
                        if (Variables.INTask == null || Variables.INTask.IsFaulted || Variables.INTask.IsCanceled)
                        {
                            Variables.INTask = Task.Factory.StartNew(

                                delegate ()
                                {
                                    while (!Variables.Token.IsCancellationRequested)
                                        ControllerIO.ExecuteInput();
                                },

                                Variables.Token
                            );
                        }

                        if (Variables.OUTTask == null || Variables.OUTTask.IsFaulted || Variables.OUTTask.IsCanceled)
                        {
                            Variables.OUTTask = Task.Factory.StartNew(

                                delegate ()
                                {
                                    while (!Variables.Token.IsCancellationRequested)
                                        ControllerIO.ExecuteOutput();
                                },

                                Variables.Token
                            );
                        }
                    }
                    #endregion
                }
            }

            catch (Exception ERROR)
            {
                Helpers.LogException(ERROR);

                var _messageResult = MessageBox.Show(
                    "Re:Fined has encountered a critical error and needs to be closed.\n" +
                    "Please send the log present in Documents/Kingdom Hearts/Logs for troubleshooting.\n" + 
                    "Re:Fined will now be terminated.",
                    "Error #005: Fatal Error.", MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Error);

                if (_messageResult == DialogResult.Cancel || _messageResult == DialogResult.OK)
                    Environment.Exit(-3);

                Helpers.Log("Re:Fined terminated with an exception!", 1);
                Environment.Exit(-1);
            }
        }
    }
}
