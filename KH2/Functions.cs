using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Collections.Generic;

using DiscordRPC;
using Binarysharp.MSharp;

using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;

namespace ReFined
{
    public class Functions
    {
        #region Variable Region

        static ulong ULONG_MINIMAL = 0xFFFFFFFFFFFFFFFF;

        static byte SAVE_ROOM;
        static byte SAVE_WORLD;
        static byte SKIP_STAGE;
        static byte SAVE_ITERATOR;

        static bool CONF_ENTER;
        static bool DETECT_DUB;
        static bool SETTING_LOAD;

        static bool PAST_ENEMY;
        static bool PAST_MUSIC;
        static bool SAVE_RESET;
        static bool ROOM_LOADED;

        static string WL_SUFF = "%s";
        static string US_SUFF = "us";
        static string FM_SUFF = "fm";

        static uint MAGIC_LV1;
        static ushort MAGIC_LV2;

        static int OFFSET_TITLE;
        static int OFFSET_DESC;

        static int PAST_FORM = -1;

        static ulong[] LOAD_LIST;

        static uint RETRY_ALLOC = 0x00;
        static int RETRY_MODE = 0x00;
        static int PREPARE_MODE = 0x00;
        static bool RETRY_LOCK;
        static bool RETRY_BLACKLIST;

        static byte RATIO_DETECT;

        static float PAST_WIDTH;
        static float PAST_HEIGHT;

        static short[] LIMIT_SHORT;

        static byte[] MAGIC_STORE;
        static byte[] OBJENTRY_READ;

        static byte CONFIG_COUNT;
        static ushort ROXAS_BLADE;

        static byte[] REMOVED_LIST = new byte[] { 0x00, 0x00, 0x00 };

        static byte[] WORLD_READ;
        static float[] POS_READ;
        static float[] CAM_READ;

        static bool[] DEBOUNCE = new bool[] { false, false, false, false, false, false, false, false, false, false };

        #endregion


        #region Basic Functionality

        /// <summary>
        /// Re:Fined's Initialization Function. Must be executed before everything else.
        /// </summary>
        public static void Initialization()
        {
            try
            {
                Helpers.InitConfig();

                Helpers.Log("Initializing Re:Fined...", 0);

                // Open the config file for game-specific configs.
                var _configIni = new TinyIni("reFined.ini");

                Helpers.Log("Initializing SharpHook...", 0);

                Variables.SharpHook = new MemorySharp(Hypervisor.Process);

                Helpers.Log("Initializing Configuration...", 0);

                // Parse the Festive Toggle, and the chosen Limit Form shortcuts, default Retry Setting, and the Drive Shortcut setting.
                Variables.festiveToggle = Convert.ToBoolean(_configIni.Read("festivityEngine", "Kingdom Hearts II"));
                Variables.driveToggle = Convert.ToBoolean(_configIni.Read("driveShortcuts", "Kingdom Hearts II"));
                Variables.resetPrompt = Convert.ToBoolean(_configIni.Read("resetPrompt", "Kingdom Hearts II"));
                Variables.adjustRatio = Convert.ToBoolean(_configIni.Read("adjustRatio", "Kingdom Hearts II"));
                Variables.limitShorts = _configIni.Read("limitShortcuts", "Kingdom Hearts II");
                Variables.retryDefault = _configIni.Read("deathPrompt", "Kingdom Hearts II") == "retry" ? true : false;

                Helpers.Log("Initializing Limit Shortcuts...", 0);

                // Should the shortcuts be parsed; Place them accordingly.
                if (Variables.limitShorts != "")
                {
                    LIMIT_SHORT = new short[4];

                    var _splitArr = Variables.limitShorts.Replace("[", "").Replace("]", "").Replace(", ", ",").Split(',');

                    // This code always presumes O is confirm.
                    LIMIT_SHORT[0] = Variables.LMTDictionary[_splitArr[0]];
                    LIMIT_SHORT[1] = Variables.LMTDictionary[_splitArr[1]];
                    LIMIT_SHORT[2] = Variables.LMTDictionary[_splitArr[2]];
                    LIMIT_SHORT[3] = Variables.LMTDictionary[_splitArr[3]];
                }

                Helpers.Log("Unlocking Memory Regions...", 0);

                // Unlock all the EXE-Related addresses.
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_LimiterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);

                Hypervisor.UnlockBlock(Variables.ADDR_PAXFormatter);
                Hypervisor.UnlockBlock(Variables.ADDR_ANBFormatter);
                Hypervisor.UnlockBlock(Variables.ADDR_BTLFormatter);
                Hypervisor.UnlockBlock(Variables.ADDR_EVTFormatter);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_LimiterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x18BA09, true);

                Hypervisor.UnlockBlock(Variables.ADDR_LimitShortcut);
                Hypervisor.UnlockBlock(Variables.ADDR_MusicPath);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortListFilterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortEquipFilterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortCategoryFilterINST, true);
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_ShortIconAssignINST, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x39EF31, true);                
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x39EF36, true);

                Helpers.Log("Initializing Save/Config/Log Directories...", 0);

                var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

                var _logsDir = Path.Combine(_documentsPath, "Kingdom Hearts/Logs");
                var _saveDir = Path.Combine(_documentsPath, "Kingdom Hearts/Save Data");
                var _configDir = Path.Combine(_documentsPath, "Kingdom Hearts/Configuration");

            EPIC_INIT:
                if (Directory.Exists(_logsDir))
                {
                    string[] _epicDirs = Directory.GetDirectories(_saveDir, "*", SearchOption.TopDirectoryOnly);

                    if (_epicDirs.Length == 0x00)
                        goto EPIC_INIT;

                    foreach (var _str in _epicDirs)
                    {
                        var _folderName = new DirectoryInfo(_str).Name;
                        Directory.CreateDirectory(Path.Combine(_configDir, _folderName));

                        Helpers.Log("Detected and Created directories for ID: " + _folderName, 0);
                    }
                }

                else
                {
                    Directory.CreateDirectory(_logsDir);
                    Directory.CreateDirectory(_saveDir);
                    Directory.CreateDirectory(_configDir);

                    var _messageResult = MessageBox.Show(
                        "Re:Fined has detected the mispresence of the necessary folders,\n" +
                        "and has taken necessary action to try and create them. If you see\n" +
                        "this message again, or the game crashes, please restart the game.\n\n" +
                        "Should the game continue to crash, write about it to the Re:Fined\n" +
                        "Discord Server immediately.",
                        "Save Folder Mispresence Detected!", MessageBoxButtons.OKCancel,
                        MessageBoxIcon.Warning);

                    if (_messageResult == DialogResult.Cancel)
                        Environment.Exit(0);

                    goto EPIC_INIT;
                }

                var _pathPrefix = Path.Combine(Path.GetDirectoryName(Hypervisor.Process.MainModule.FileName), "Image", "en");

                Helpers.Log("Scanning for Additional Features...", 0);

                // Remove the config options if the optionals are not patched in.

                var _audioItem = Variables.ARRY_ConfigMenu.First(x => x[1] == 0x01BD);
                var _musicItem = Variables.ARRY_ConfigMenu.First(x => x[1] == 0x01BE);
                var _heartItem = Variables.ARRY_ConfigMenu.First(x => x[1] == 0x01BF);

                if (Operations.GetFileSize("obj/P_EX110.a.jp") == 0x00)
                {
                    Helpers.Log("The Audio Add-On was not found! Removing the option from all menus...", 1);
                    Variables.ARRY_ConfigMenu.Remove(_audioItem);
                    REMOVED_LIST[0] = 0x01;
                }

                else if (Operations.GetFileSize("voice/xx/battle/abu.win32.scd") != 0x00)
                {
                    Helpers.Log("An additional Dub Patch was detected! Accommodating the config menu...", 1);

                    var _audioName = Operations.FetchPointerMSG(Variables.PINT_SystemBAR, 0x01D9);
                    var _audioDesc = Operations.FetchPointerMSG(Variables.PINT_SystemBAR, 0x01DA);

                    var _indexAudio = Variables.ARRY_ConfigMenu.IndexOf(_audioItem);
                    Variables.ARRY_ConfigMenu[_indexAudio] = Variables.VALUE_ConfigAudio;
                }

                if (Operations.GetFileSize("bgm/ps2md050.win32.scd") == 0x00)
                {
                    Helpers.Log("The BGM Add-On was not found! Removing the option from all menus...", 1);
                    Variables.ARRY_ConfigMenu.Remove(_musicItem);
                    REMOVED_LIST[1] = 0x01;
                }

                if (Operations.GetFileSize("obj/V_BB100.mdlx") == 0x00)
                {
                    Helpers.Log("The Enemy Palette Add-On was not found! Removing the option from all menus...", 1);
                    Variables.ARRY_ConfigMenu.Remove(_heartItem);
                    REMOVED_LIST[2] = 0x01;
                }


                Helpers.Log("Writing the Game Over Menu...", 0);

                var _optionsPoint = Hypervisor.Read<ulong>(Variables.PINT_DeadData);
                Hypervisor.WriteArray(_optionsPoint + 0x848, new byte[] { 0x20, 0x00, 0x04, 0x00, 0x02, 0x00, 0xB0, 0x8A, 0x03, 0x00, 0xB1, 0x8A, 0x0C, 0x00, 0x3A, 0xAF, 0x04, 0x00, 0xB2, 0x8A, 0x00, 0x00 }, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x350000, true);

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

                OFFSET_TITLE = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, 0x3738);
                OFFSET_DESC = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, 0x373B);

                // Initialize the source and the token for secondary tasks.
                Variables.Source = new CancellationTokenSource();
                Variables.Token = Variables.Source.Token;

                // Mark the initialization as complete.
                Variables.Initialized = true;

                Helpers.Log("Re:Fined initialized with no errors!", 0);
            }

            catch (Exception _caughtEx)
            {
                Helpers.LogException(_caughtEx);
                Helpers.Log("Re:Fined terminated with an exception!", 1);
                Environment.Exit(-1);
            }
        }

        /// <summary>
        /// When the proper input is given, returns to the title screen.
        /// When the option for it is toggled, prompts the user for a cancellation.
        /// </summary>
        public static void ResetGame()
        {
            var _buttRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _confirmRead = Hypervisor.Read<ushort>(Variables.ADDR_Confirm);
            var _timeCurr = DateTime.Now;

            if (_buttRead == 0x0003 && !DEBOUNCE[0])
            {
                Helpers.Log("Reset Sequence Initiated! Checking for the prompt...", 0);
                DEBOUNCE[0] = true;

                if (Variables.resetPrompt)
                {
                    Helpers.Log("Prompt found! Showing prompt...", 0);

                    Additions.ShowSmallObtained(0x01BA);
                    var _cancelRequest = false;

                    Task.Factory.StartNew(() =>
                    {
                        while ((DateTime.Now - _timeCurr) < TimeSpan.FromSeconds(2))
                        {
                            var _buttSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
                            var _readSecond = Hypervisor.Read<ushort>(Variables.ADDR_Input);

                            if ((_readSecond & _buttSeek) == _buttSeek)
                            {
                                Additions.ShowSmallObtained(0x01BB);
                                _cancelRequest = true;
                                break;
                            };
                        }

                        if (!_cancelRequest)
                            Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
                    });
                }

                else
                    Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
            }

            else if (_buttRead != 0x0003 && DEBOUNCE[0])
                DEBOUNCE[0] = false;
        }

        /// <summary>
        /// Adjusts and edits the current framelimiter, and the function which controls it,
        /// according to the current chosen framerate.
        /// </summary>
        public static void OverrideLimiter()
        {
            // Calculate the instruction address.
            var _instructionAddress = Hypervisor.PureAddress + Variables.ADDR_LimiterINST;

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
                Hypervisor.Write<byte>(Variables.ADDR_Limiter, 0x00);
            }
        }

        /// <summary>
        /// Overwrites the Limit Shortcuts with the ones given in the config file.
        /// </summary>
        public static void OverrideLimits()
        {
            var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);
            var _modeRead = Hypervisor.Read<ushort>(Variables.ADDR_ControllerMode);
            var _shortRead = Hypervisor.Read<ushort>(Variables.ADDR_LimitShortcut);

            if (_confirmRead == 0x00 && _shortRead != LIMIT_SHORT[0])
            {
                Helpers.Log("Overriding Limits for the Japanese scheme.", 0);

                Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[0]);
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[3]);
            }

            else if (_confirmRead == 0x01 && _shortRead != LIMIT_SHORT[3] && _modeRead == 0)
            {
                Helpers.Log("Overriding Limits for the English Scheme", 0);

                Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[3]);
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[0]);
            }

            Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x02, LIMIT_SHORT[1]);
            Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x04, LIMIT_SHORT[2]);
        }

        /// <summary>
        /// Fixes the exit functions on the Title Screen.
        /// </summary>
        public static void ExitFix()
        {
            if (Operations.CheckTitle())
            {
                var _countButton = Hypervisor.Read<byte>(Variables.ADDR_TitleCount);
                var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);

                var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
                var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

                var _buttonSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
                var _inputValue = _inputRead & _buttonSeek;

                if (_inputValue == _buttonSeek && _selectButton == _countButton - 0x01)
                {
                    Helpers.Log("Title to Exit detected! 2.5 second limit set! Initating exit...", 0);
                    Thread.Sleep(2500);

                    Helpers.Log("Re:Fined terminated with no errors.", 0);
                    Environment.Exit(0);
                }
            }
        }

        /// <summary>
        /// The function responsible for automatically saving the game.
        /// </summary>
        public static void AutosaveEngine()
        {
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);

            var _pauseCheck = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);

            if (!Operations.CheckTitle() && _loadRead == 0x01)
            {
                Thread.Sleep(100);

                var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
                var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

                var _saveableBool = (Variables.saveToggle != 0x02) && _battleRead == 0x00 && _loadRead == 0x01 && _cutsceneRead == 0x00 && _worldCheck >= 0x02 && _pauseCheck == 0x00;

                if (_saveableBool)
                {
                    if (SAVE_WORLD != _worldCheck)
                    {
                        Helpers.Log("World condition met! Writing Autosave...", 0);

                        Operations.GenerateSave();
                        SAVE_ITERATOR = 0;
                    }

                    else if (SAVE_ROOM != _roomCheck && _worldCheck >= 2)
                    {
                        SAVE_ITERATOR++;

                        if (SAVE_ITERATOR == 3)
                        {
                            Helpers.Log("Room condition met! Writing Autosave...", 0);

                            Operations.GenerateSave();
                            SAVE_ITERATOR = 0;
                        }
                    }

                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;
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
            var _healthValue = Hypervisor.Read<byte>(Variables.ADDR_SoraHP);
            var _magicValue = Hypervisor.Read<byte>(Variables.ADDR_SoraHP + 0x180);

            var _levelValue = Hypervisor.Read<byte>(Variables.ADDR_LevelStart);
            var _formValue = Hypervisor.Read<byte>(Variables.ADDR_SoraForm);

            var _stringState = string.Format(
                "Level {0} | Form: {1}",
                _levelValue,
                Variables.FRMDictionary.ElementAtOrDefault(_formValue)
            );

            var _stringDetail = string.Format("HP: {0} | MP: {1}", _healthValue, _magicValue);

            var _worldID = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _battleFlag = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);

            var _timeValue = Math.Floor(Hypervisor.Read<int>(0x00444FA6) / 60F);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            var _timeText = string.Format("In-Game Time: {0}", string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00")));
            var _diffValue = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);

            if (!Operations.CheckTitle())
            {
                Variables.DiscordClient.SetPresence(
                    new RichPresence
                    {
                        Details = _stringDetail,
                        State = _stringState,
                        Assets = new Assets
                        {
                            LargeImageKey = Variables.WRLDictionary.ElementAtOrDefault(_worldID),
                            LargeImageText = _timeText,
                            SmallImageKey = Variables.BTLDictionary.ElementAtOrDefault(_battleFlag),
                            SmallImageText = Variables.MDEDictionary.ElementAtOrDefault(_diffValue)
                        },
                    }
                );
            }

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
        /// Shifts the chosen spell in the Magic menu up or down,
        /// depending on the input. Executed with L2 + DOWN or L2 + UP.
        /// </summary>
        public static void MagicSorting()
        {
            // Fetch the status of the room. This will be necessary.
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            // Fetch the input and read the pointer to the second CM.
            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _menuPointer = Hypervisor.Read<ulong>(Variables.ADDR_MagicMenu[0]);

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
                    Hypervisor.WriteArray(Variables.ADDR_MagicMenu[1], MAGIC_STORE);
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
                    var _magicIndex = Hypervisor.Read<byte>(Variables.ADDR_MagicMenu[2]);
                    var _magicMax = Hypervisor.Read<byte>(_menuPointer + 0x10, true);

                    // Set to 0x01 if it's going up, set to 0x02 if it's going down.
                    var _inputCheck = (_inputRead & 0x01001) == 0x1001 ? 0x01 : (_inputRead & 0x4001) == 0x4001 ? 0x02 : 0x00;
                    var _triggerCheck = (_inputRead & 0x01) == 0x01;

                    // Read the instruction.
                    var _insCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[0], true);

                    // If L2 is being held down:
                    if (_triggerCheck && _insCheck != 0x90)
                    {
                        Helpers.Log("L2 Detected within Magic Menu! Disabling input registry.", 1);

                        // NOP out command selection, so it does not interfere with our input.
                        for (int _ins = 0; _ins < Variables.ADDR_CMDSelectINST.Length; _ins++)
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[_ins], new byte[] { 0x90, 0x90, 0x90 }, true);
                    }

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
                        var _subjectMagic = Hypervisor.Read<ushort>(Variables.ADDR_MagicMenu[1] + (ulong)_magicPointer);
                        var _targetMagic = _magicBounds >= 0 ? Hypervisor.Read<ushort>(Variables.ADDR_MagicMenu[1] + (ulong)_magicBounds) : (ushort)0x0000;

                        // If the move would be in bounds, and if the switchee is not "nothing":
                        if (_targetMagic != 0x0000)
                        {
                            // Make the switch
                            Hypervisor.Write(Variables.ADDR_MagicMenu[1] + (ulong)_magicPointer, _targetMagic);
                            Hypervisor.Write(Variables.ADDR_MagicMenu[1] + (ulong)_magicBounds, _subjectMagic);

                            // Move the magic index.
                            Hypervisor.Write(Variables.ADDR_MagicMenu[2], _magicIndex + (_inputCheck == 0x01 ? -0x01 : 0x01));
                            Hypervisor.Write(Variables.ADDR_MagicMenu[2] + 0x04, _subjectMagic);

                            Helpers.Log(String.Format("Moving Magic ID \"{0}\" {1} within the menu!", "0x" + _subjectMagic.ToString("X4"), _inputCheck == 0x01 ? "up" : "down"), 0);

                            // Read the entirety of the magic menu, and save it to memory.
                            // And also save it to the save data.
                            MAGIC_STORE = Hypervisor.ReadArray(Variables.ADDR_MagicMenu[1], _magicMax * 0x02);
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

            if (Variables.driveToggle && _instCheck != 0x90)
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
        /// Handles the configuration of the game.
        /// Prone to crashing.
        /// </summary>
        public static void ConfigHandler()
        {
            var _configRead = Hypervisor.Read<ushort>(Variables.ADDR_Config);
            var _selectPoint = Hypervisor.Read<ulong>(Variables.PINT_SubMenuSelect);

            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_SubMenu);
            var _pauseRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _settingsPoint = Hypervisor.Read<ulong>(Variables.PINT_ConfigSetting);
            var _difficultyRead = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);

            var _offsetTitle = Operations.FindInfoMSG(Variables.PINT_SystemBAR, 0x3738) + 0x04;
            var _offsetDesc = Operations.FindInfoMSG(Variables.PINT_SystemBAR, 0x373B) + 0x04;

            var _findTitle = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigTitle[_difficultyRead]);
            var _findDesc = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigDesc[_difficultyRead]);

            var _layoutPointer = Hypervisor.Read<ulong>(0x3A0952);

            if (_difficultyRead == 0x00)
            {
                _findTitle = OFFSET_TITLE;
                _findDesc = OFFSET_DESC;
            }

            if (!Operations.CheckTitle() && !SETTING_LOAD)
            {
                Helpers.Log("Fetching the current configuration from Save Data...", 0);

                Variables.audioMode = (byte)((_configRead & 0x0800) == 0x0800 ? 0x01 : ((_configRead & 0x1000) == 0x1000 ? 0x02 : 0x0000));
                Variables.vanillaMusic = (_configRead & 0x0020) == 0x0020;
                Variables.vanillaEnemy = (_configRead & 0x8000) == 0x8000;
                Variables.saveToggle = (byte)((_configRead & 0x0002) == 0x0002 ? 0x01 : ((_configRead & 0x0004) == 0x0004 ? 0x00 : 0x02));
                Variables.autoController = (byte)((_configRead & 0x2000) == 0x2000 ? 0x00 : ((_configRead & 0x4000) == 0x4000 ? 0x01 : 0x02));

                Helpers.Log("Configuration fetched!", 0);

                SETTING_LOAD = true;
            }

            else if (Operations.CheckTitle() && SETTING_LOAD)
                SETTING_LOAD = false;

            if (_menuRead == 0x24 && _pauseRead == 0x01 && _selectPoint != 0x00)
            {
                if (_settingsPoint != 0x00 && !DEBOUNCE[6])
                {
                    Helpers.Log("Config Menu Detected! Setting the necessary values...", 0);

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
                    var _langAudio = (_configRead & 0x0800) == 0x0800 ? (DETECT_DUB ? 0x02 : 0x01) : ((_configRead & 0x1000) == 0x1000 ? 0x01 : 0x00);

                    var _settingsArray = new List<byte>
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

                    if (REMOVED_LIST[2] == 0x01)
                        _settingsArray.RemoveAt(0x08);

                    if (REMOVED_LIST[1] == 0x01)
                        _settingsArray.RemoveAt(0x07);

                    if (REMOVED_LIST[0] == 0x01)
                        _settingsArray.RemoveAt(0x06);

                    Hypervisor.WriteArray(_settingsPoint, _settingsArray.ToArray(), true);

                    Helpers.Log("Options set! Correcting the Difficulty...", 0);

                    Hypervisor.Write(_offsetTitle, _findTitle, true);
                    Hypervisor.Write(_offsetDesc, _findDesc, true);

                    Variables.SharpHook[(IntPtr)0x363380].Execute();
                    Variables.SharpHook[(IntPtr)0x363340].Execute();

                    Helpers.Log("Difficulty corrected!", 0);

                    DEBOUNCE[6] = true;
                }

                if (_loadRead == 0x01)
                    CONF_ENTER = true;

                var _settingRead = Hypervisor.ReadArray(_settingsPoint, 0x10, true);

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

                if (REMOVED_LIST[2] == 0x00)
                    Variables.vanillaEnemy = Convert.ToBoolean(_settingRead[0x08 - REMOVED_LIST[1] + REMOVED_LIST[0]] == 0x00 ? 0x01 : 0x00);

                if (REMOVED_LIST[1] == 0x00)
                    Variables.vanillaMusic = Convert.ToBoolean(_settingRead[0x07 - REMOVED_LIST[0]] == 0x00 ? 0x01 : 0x00);

                if (REMOVED_LIST[0] == 0x00)
                    Variables.audioMode = _settingRead[0x06];

                var _configOffset = REMOVED_LIST[0] + REMOVED_LIST[1] + REMOVED_LIST[2];

                Variables.saveToggle = _settingRead[0x05];
                Variables.autoController = _settingRead[0x09 - _configOffset];

                var _configBitwise =
                    (_settingRead[0x00] == 0x01 ? Variables.CONFIG_BITWISE.FIELD_CAM : Variables.CONFIG_BITWISE.OFF) |
                    (_settingRead[0x01] == 0x01 ? Variables.CONFIG_BITWISE.CAMERA_V : Variables.CONFIG_BITWISE.OFF) |
                    (_settingRead[0x02] == 0x01 ? Variables.CONFIG_BITWISE.CAMERA_H : Variables.CONFIG_BITWISE.OFF) |
                    (_settingRead[0x03] == 0x01 ? Variables.CONFIG_BITWISE.SUMMON_PARTIAL : (_settingRead[0x03] == 0x00 ? Variables.CONFIG_BITWISE.SUMMON_FULL : Variables.CONFIG_BITWISE.OFF)) | 
                    (_settingRead[0x04] == 0x00 ? Variables.CONFIG_BITWISE.NAVI_MAP : Variables.CONFIG_BITWISE.OFF) |
                    (_settingRead[0x05] == 0x00 ? Variables.CONFIG_BITWISE.AUTOSAVE_INDICATOR : (_settingRead[0x05] == 0x01 ? Variables.CONFIG_BITWISE.AUTOSAVE_SILENT : Variables.CONFIG_BITWISE.OFF)) |
                    (REMOVED_LIST[0] == 0x00 ? (_settingRead[0x06] == (DETECT_DUB ? 0x02 : 0x01) ? Variables.CONFIG_BITWISE.AUDIO_JAPANESE : (_settingRead[0x06] == 0x01 ? Variables.CONFIG_BITWISE.AUDIO_OTHER : Variables.CONFIG_BITWISE.OFF)) : Variables.CONFIG_BITWISE.OFF) |
                    (REMOVED_LIST[1] == 0x00 ? (_settingRead[0x07 - REMOVED_LIST[0]] == 0x00 ? Variables.CONFIG_BITWISE.MUSIC_VANILLA : Variables.CONFIG_BITWISE.OFF) : Variables.CONFIG_BITWISE.OFF) |
                    (REMOVED_LIST[2] == 0x00 ? (_settingRead[0x08 - (REMOVED_LIST[1] + REMOVED_LIST[0])] == 0x00 ? Variables.CONFIG_BITWISE.HEARTLESS_VANILLA : Variables.CONFIG_BITWISE.OFF) : Variables.CONFIG_BITWISE.OFF) | 
                    (_settingRead[0x09 - _configOffset] == 0x00 ? Variables.CONFIG_BITWISE.PROMPT_CONTROLLER : (_settingRead[0x09 - _configOffset] == 0x01 ? Variables.CONFIG_BITWISE.PROMPT_KEYBOARD : Variables.CONFIG_BITWISE.OFF)) |
                    (_settingRead[0x0A - _configOffset] == 0x00 ? Variables.CONFIG_BITWISE.VIBRATION : Variables.CONFIG_BITWISE.OFF) |
                    (_settingRead[0x0B - _configOffset] == 0x01 ? Variables.CONFIG_BITWISE.COMMAND_KH2 : Variables.CONFIG_BITWISE.OFF); 

                Hypervisor.Write(Variables.ADDR_Config, (ushort)_configBitwise);
            }

            else if (_selectPoint == 0x00 && CONF_ENTER)
            {
                Helpers.Log("Configuration Menu closed! Initializing full reload!", 0);
                Helpers.Log("THE GAME MAY CRASH DURING THIS PROCESS!", 1);

                var _soraPoint = Hypervisor.Read<ulong>(0x55629A);
                CONF_ENTER = false;

                DEBOUNCE[6] = false;

                if (POS_READ == null)
                {
                    POS_READ = new float[]
                    {
                        Hypervisor.Read<float>(_soraPoint + 0x670, true),
                        Hypervisor.Read<float>(_soraPoint + 0x674, true),
                        Hypervisor.Read<float>(_soraPoint + 0x678, true),
                        Hypervisor.Read<float>(_soraPoint + 0x7A8, true),
                    };

                    CAM_READ = new float[]
                    {
                        Hypervisor.Read<float>(0x1B251A),
                        Hypervisor.Read<float>(0x1B251E),
                        Hypervisor.Read<float>(0x1B2522)
                    };
                }

                // Give time for the Menu to close.
                while(Hypervisor.Read<byte>(0x399952) != 0);

                Helpers.Log("Killing the BGM and Fade Handlers...", 0);

                // Unblock the fade handler and shut the music down.
                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x15493A, true);
                Variables.SharpHook[(IntPtr)0x1305F0].Execute();

                Helpers.Log("Noting down the current world...", 0);

                // Read the current world and event data.
                if (WORLD_READ == null)
                    WORLD_READ = Hypervisor.ReadArray(Variables.ADDR_World, 0x0A);

                // Make a new world data to be: Twilight Town - The Empty Realm.
                var _newArray = new byte[] { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                // If already in TT, change the target world to OC.
                if (WORLD_READ[0] == 0x02)
                    _newArray[0] = 0x01;

                Helpers.Log("Jumping into a DUMMY world!", 0);

                // Initiate the jump.
                Hypervisor.WriteArray(Variables.ADDR_World, _newArray);
                Variables.SharpHook[(IntPtr)0x150590].Execute(BSharpConvention.MicrosoftX64, (long)(Hypervisor.BaseAddress + Variables.ADDR_World), 2, 0, 0, 0);

                // Wait until the fade has been completed.
                while (Hypervisor.Read<byte>(0x55472D) != 0x80) ;

                // Destroy the fade handler so it does not cause issues.
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x15493A, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                Hypervisor.Write<byte>(0x55472D, 0x80);

                // Whilst not loaded, constantly shut off the music.
                while (Hypervisor.Read<byte>(Variables.ADDR_LoadFlag) != 1)
                    Variables.SharpHook[(IntPtr)0x1305F0].Execute();

                Helpers.Log("Jump complete! Jumping back!", 0);

                // Execute the function for JumpEffect again so we are stuck.
                Variables.SharpHook[(IntPtr)0x154120].Execute(0x02);

                // Wait 2.5 seconds.
                Thread.Sleep(2500);

                // Atfer load, jump back to where we came from.
                Hypervisor.WriteArray(Variables.ADDR_World, WORLD_READ);
                Variables.SharpHook[(IntPtr)0x150590].Execute(BSharpConvention.MicrosoftX64, (long)(Hypervisor.BaseAddress + Variables.ADDR_World), 2, 0, 0, 0);

                // Wait until load.
                while (Hypervisor.Read<byte>(Variables.ADDR_LoadFlag) != 1) ;

                // Restore the fade initiater after load.
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x15493A, new byte[] { 0xF3, 0x0F, 0x11, 0x8F, 0x0C, 0x01, 0x00, 0x00 }, true);

                Helpers.Log("Jump complete! Writing Sora's Position...", 0);

                // Wait until the fade is completed.
                while (Hypervisor.Read<byte>(0x55472D) != 0x00)
                {
                    if (Hypervisor.Read<byte>(0x55472D) > 0x70)
                    {
                        Hypervisor.Write(_soraPoint + 0x670, POS_READ[0], true);
                        Hypervisor.Write(_soraPoint + 0x674, POS_READ[1], true);
                        Hypervisor.Write(_soraPoint + 0x678, POS_READ[2], true);
                        Hypervisor.Write(_soraPoint + 0x7A8, POS_READ[3], true);

                        Hypervisor.Write(0x1B251A, CAM_READ[0]);
                        Hypervisor.Write(0x1B251E, CAM_READ[1]);
                        Hypervisor.Write(0x1B2522, CAM_READ[2]);
                    }
                }

                Helpers.Log("If you can see this, the jump was successful without a crash!", 0);

                // Flush the world data.
                WORLD_READ = null;
                POS_READ = null;
            }
        }

        /// <summary>
        /// Determine whether to skip or go through Roxas' story.
        /// This is detemined by the Vibration option at the Title Screen.
        /// </summary>
        public static void TutorialSkip()
        {
            if (Operations.CheckTitle() && SKIP_STAGE != 0)
            {
                Helpers.Log("Title Screen detected! Resetting Roxas Skip!", 0);
                SKIP_STAGE = 0;
            }

            var _vibRead = Hypervisor.Read<ushort>(0x5B6AC6);
            var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);

            var _countButton = Hypervisor.Read<byte>(Variables.ADDR_TitleCount);
            var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);

            if (!Operations.CheckTitle())
            {
                var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
                var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);
                var _eventCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x04);

                if (_worldCheck == 0x02 && _roomCheck == 0x01 && _eventCheck == 0x38 && SKIP_STAGE == 0)
                {
                    if (_vibRead == 0x01)
                    {
                        Helpers.Log("Room and Settings are correct! Initiating Roxas Skip's First Phase...", 0);

                        Hypervisor.Write<uint>(Variables.ADDR_World, 0x322002);
                        Hypervisor.Write<uint>(Variables.ADDR_World + 0x04, 0x01);
                        Hypervisor.Write<uint>(Variables.ADDR_World + 0x08, 0x01);

                        Hypervisor.Write<uint>(0x444832, 0x1FF00001);
                        Hypervisor.Write<uint>(0x444832 + 0x04, 0x00000000);

                        SKIP_STAGE = 1;
                    }

                    else
                    {
                        Helpers.Log("Room is correct but settings are not! Disabling Roxas Skip...", 0);
                        SKIP_STAGE = 2;
                    }
                }

                else if (_worldCheck == 0x02 && _roomCheck == 0x20 && _eventCheck == 0x9A && SKIP_STAGE == 1)
                {
                    Helpers.Log("Room parameters correct! Skip was initiated! Initiating Roxas Skip's Second Phase...", 0);

                    Hypervisor.Write<uint>(Variables.ADDR_World, 0x001702);
                    Hypervisor.Write<uint>(Variables.ADDR_World + 0x04, (0x02 << 10) + 0x02);
                    Hypervisor.Write<uint>(Variables.ADDR_World + 0x08, 0x02);

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
                                0x01,
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
                    SKIP_STAGE = 2;
                }
            }
        }

        /// <summary>
        /// Handles the crown on Sora's HUD corresponding to the crown he wears on his model.
        /// Only works on English Audio for now.
        /// </summary>
        public static void CrownManager()
        {
            var _suffixFile = (Variables.audioMode == 0x00) ? ".a.us" : (Variables.audioMode == 0x01) ? ".a.jp" : ".a.xx";

            var _formRead = Hypervisor.Read<byte>(Variables.ADDR_SoraForm);
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _cutsByte = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

            if (Operations.CheckTitle() || _loadRead == 0x00 || PAST_FORM != _formRead)
            {
                LOAD_LIST = null;
                PAST_FORM = _formRead;
            }

            RELOAD_POINT:

            if (!Operations.CheckTitle() && _loadRead == 0x01 && _cutsByte == 0x00)
            {
                var _memoryOffset = Hypervisor.PureAddress & 0x7FFF00000000;

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

                var _crownRead = Hypervisor.ReadArray(0x442B62 + 0x36B2, 0x03);
                var _crownSum = _crownRead[0] + _crownRead[1] + _crownRead[2];

                if (LOAD_LIST[_formRead] == ULONG_MINIMAL || _soraPoints[_formRead] > 0x7FFF00000000)
                {
                    LOAD_LIST = null;
                    goto RELOAD_POINT;
                }

                foreach (var _point in _soraPoints)
                {
                    if (_point != 0x00)
                    {
                        var _barOffset = Hypervisor.Read<uint>(_point + 0x08, true);
                        var _soraOffset = Hypervisor.Read<uint>(_point + 0x38, true) - _barOffset;

                        var _faceCheck = Hypervisor.Read<uint>(_point + 0x24, true);

                        if (_faceCheck != 0x65636166)
                            return;

                        var _topValue = 0x00 + _crownSum * 0x5A;
                        var _bottomValue = 0x5D + _crownSum * 0x5A;

                        for (uint i = 0; i < 3; i++)
                        {
                            Hypervisor.Write(_point + _soraOffset + 0x38 + (0x2C * i), _topValue, true);
                            Hypervisor.Write(_point + _soraOffset + 0x40 + (0x2C * i), _bottomValue, true);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Adds the option to retry a past fight.
        /// It also adds the option to prepare before retrying.
        /// </summary>
        public static void RetryPrompt()
        {
            // So, we initialize all of this shit just to determine whether Retry will show up or not.
            var _menuPoint = Hypervisor.Read<ulong>(Variables.PINT_DeadMenu);
            var _optionsPoint = Hypervisor.Read<ulong>(Variables.PINT_DeadData);

            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_DeadSelect);

            var _menuType = Hypervisor.Read<int>(0x3999C6);
            var _subType = Hypervisor.Read<byte>(Variables.ADDR_SubMenu);

            var _bttlByte = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
            var _cutsByte = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

            var _pausRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);
            var _fnshByte = Hypervisor.Read<byte>(Variables.ADDR_FinishFlag);

            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _warpRead = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);

            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);
            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_World);

            var _nullArray = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 };

            /*
                Okay, so I wasted a long ass time trying to make this work. But I figure I got it down to a fine art.
               
                This function messes with in-game functions and the save state, dangerous, but it works very well.
                Basically, when you are in a forced battle, it YEETS the functions responsible for swtiching rooms
                and reverting flags upon continuing, allowing one to retry the said battle instantly.

                But I also need to revert the save to the point where you first began the battle as well, or your entire
                save state will be reverted to when you were in the room before and that's a no-no and can cause massive
                issues such as missed items and what not.
            */

            // If one's on the Title Screen while Retry is active: Deactivate it.
            if (Operations.CheckTitle() && (RETRY_LOCK || RETRY_MODE == 0x01))
            {
                Helpers.Log("Title Screen detected on Retry Mode! Restoring ..", 0);

                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x39EF31, new byte[] { 0x66, 0x89, 0x01 }, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x39EF36, new byte[] { 0x66, 0x89, 0x05, 0x5D, 0xC1, 0x60, 0x00 }, true);

                ROXAS_BLADE = 0x0000;
                RETRY_ALLOC = 0x00;
                RETRY_MODE = 0;
                RETRY_LOCK = false;
            }

            // Cavern of Remembrance Blacklist.
            var _cavernCheck = _worldRead == 0x04 && (_roomRead >= 0x15 && _roomRead <= 0x1A);

            if (!_cavernCheck)
            {
                /*
                 * This entire section is resevred for Prepare and Retry. 
                 * For some stupid reason, I can't actually open a menu right as I retry.
                 * As such, I open a menu *constantly* until it succeeds.
                 * The consequences of this is yet to be seen.
                 */

                while (PREPARE_MODE == 0x01 && _menuType != 0x08 && _menuPoint == 0x00)
                {
                    try
                    {
                        Variables.SharpHook[(IntPtr)0x2E23A0].Execute(BSharpConvention.MicrosoftX64, 0, 0);
                        _menuType = Hypervisor.Read<int>(0x3999C6);
                    }

                    catch (Exception) { }
                }

                if (_menuType == 0x08 && PREPARE_MODE == 0x01)
                    PREPARE_MODE = 0x02;

                if (PREPARE_MODE == 0x02 && _menuType != 0x08)
                {
                    var _currData = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x10FC0);
                    Hypervisor.WriteArray(Hypervisor.MemoryOffset + RETRY_ALLOC, _currData, true);
                    PREPARE_MODE = 0x00;
                }

                // ====================================================================== //

                // Read the necessary shits at the start of a fight.
                if (RETRY_ALLOC == 0x00 && _bttlByte == 0x02 && _cutsByte == 0x00 && _pausRead == 0x00 && !Operations.CheckTitle())
                {
                    Helpers.Log("Start of forced fight, making a copy of the current state...", 0);

                    RETRY_ALLOC = Variables.SharpHook[(IntPtr)0x150030].Execute<uint>(0x10FC0);
                    
                    var _currData = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x10FC0);
                    Hypervisor.WriteArray(Hypervisor.MemoryOffset + RETRY_ALLOC, _currData, true);

                    Helpers.Log("State copied to: 0x" + RETRY_ALLOC.ToString("X8") + "!", 0);

                    ROXAS_BLADE = Hypervisor.Read<ushort>(Hypervisor.BaseAddress + 0x445052, true);

                    byte[] _tempArray = new byte[18];
                    Buffer.BlockCopy(Variables.ARRY_ContinueOptions[Variables.retryDefault ? 0x01 : 0x02], 0, _tempArray, 0, 18);

                    Hypervisor.WriteArray(_optionsPoint + 0x34A, _tempArray, true);
                }

                // Flush the memory post-fight.
                else if ((_bttlByte != 0x02 || _cutsByte != 0x00) && _pausRead == 0x00)
                {
                    Helpers.Log("The player is out of battle. Flushing memory...", 0);

                    byte[] _tempArray = new byte[18];
                    Buffer.BlockCopy(Variables.ARRY_ContinueOptions[0x00], 0, _tempArray, 0, 18);

                    Hypervisor.WriteArray(_optionsPoint + 0x34A, _tempArray, true);

                    RETRY_ALLOC = 0x00;
                    ROXAS_BLADE = 0x0000;
                }

                // This code blob is responsible for switching between Retry and Continue
                // and only runs if Sora is dead and some sort of a menu is present.
                if (_menuPoint != 0x00 && _bttlByte == 0x02)
                {
                    if (_menuRead == 0x03 && _warpRead == 0x90 && RETRY_MODE == 0x01)
                    {
                        Helpers.Log("User is going to load the game! Restoring...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                        RETRY_LOCK = true;
                    }

                    if (_subType < 0x04)
                    {
                        if ((Variables.retryDefault ? _menuRead == 0x00 : _menuRead == 0x01) && (_warpRead != 0x90 || PREPARE_MODE != 0x00))
                        {
                            Helpers.Log("Switched to Retry mode! Destroying ...", 0);

                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, new byte[] { 0x48, 0x8D, 0x15 }, true);

                            var _addressCalc = Hypervisor.PureAddress - Hypervisor.MemoryOffset;
                            Hypervisor.Write<uint>(Hypervisor.PureAddress + Variables.ADDR_InventoryINST + 0x03, RETRY_ALLOC - ((uint)_addressCalc + 0x39DECB), true);

                            RETRY_MODE = 0x01;
                            PREPARE_MODE = 0x00;
                            RETRY_LOCK = true;
                        }

                        else if ((!Variables.retryDefault ? _menuRead == 0x00 : _menuRead == 0x02) && _warpRead == 0x90)
                        {
                            Helpers.Log("Switched to Continue mode! Restoring...", 0);

                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                            RETRY_MODE = 0x00;
                            PREPARE_MODE = 0x00;
                            RETRY_LOCK = true;
                        }

                        else if ((Variables.retryDefault ? _menuRead == 0x01 : _menuRead == 0x02) && PREPARE_MODE != 0x01)
                        {
                            Helpers.Log("Switched to Prepare Mode! Destroying...", 0);

                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, new byte[] { 0x48, 0x8D, 0x15 }, true);

                            var _addressCalc = Hypervisor.PureAddress - Hypervisor.MemoryOffset;
                            Hypervisor.Write<uint>(Hypervisor.PureAddress + Variables.ADDR_InventoryINST + 0x03, RETRY_ALLOC - ((uint)_addressCalc + 0x39DECB), true);

                            RETRY_MODE = 0x01;
                            PREPARE_MODE = 0x01;
                            RETRY_LOCK = true;
                        }
                    }

                    else if (_warpRead == 0x90)
                    {
                        Helpers.Log("User is going to load the game! Restoring...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                        RETRY_LOCK = true;
                    }
                }

                var _menuCheck = Hypervisor.Read<ushort>(Variables.ADDR_DeadCount + 0x02);

                // If in a forced battle, and it is not finished, and Sora is dead, and it's in the Continue mode, and if Retry Locking ain't active;
                if (_bttlByte == 0x02 && _menuPoint != 0x00 && _cutsByte == 0x00 && !RETRY_LOCK)
                {
                    while (_menuCheck == 0xEFAC || _menuCheck == 0xCAFE)
                        _menuCheck = Hypervisor.Read<ushort>(Variables.ADDR_DeadCount + 0x02);

                    if (_menuCheck != 0x00)
                    {
                        Helpers.Log("Unknown Death Screen detected! Disabling the Retry function!", 0);
                        RETRY_ALLOC = 0x00;
                        RETRY_MODE = 0x03;
                    }

                    else
                    {
                        var _roxasTemp = Hypervisor.Read<ushort>(0x445052);

                        if (_roxasTemp != ROXAS_BLADE)
                        {
                            Hypervisor.Write(0x446B4C, _roxasTemp);
                            Hypervisor.Write(0x445052, ROXAS_BLADE);
                        }
                    }

                    RETRY_LOCK = true;
                }

                // If in a cutscene, or if not in a forced battle, or the forced battle is finished, and Sora is not dead, and Retry mode is active;
                else if (((_bttlByte != 0x02 && _menuPoint == 0x00) || _fnshByte == 0x01 || _cutsByte != 0x00) && RETRY_LOCK)
                {
                    if (_fnshByte != 0x01)
                        Helpers.Log("Death Screen is not present! Restoring ..", 0);

                    else
                        Helpers.Log("End of battle detected! Restoring ..", 0);

                    // Restore the functions responsible for switching rooms and reverting story flags.
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                    RETRY_ALLOC = 0x00;
                    RETRY_LOCK = false;
                }

                if (RETRY_BLACKLIST)
                {
                    Helpers.Log(String.Format("Out of the Cavern of Remembrance... Unlocking Retry Capabilities..."), 0);
                    RETRY_BLACKLIST = false;
                }
            }

            else if (!RETRY_BLACKLIST)
            {
                Helpers.Log(String.Format("Cavern of Remembrance detected! Locking Retry Capabilities..."), 0);
                RETRY_BLACKLIST = true;
            }
        }

        #endregion

        #region Additional In-Game Functionality

        /*
        public static void AchievementSeek()
        {
            var _struggleOrb = Hypervisor.Read<byte>(0x24A8BFA);

            var _fadeRead = Hypervisor.Read<byte>(0x55472D);
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);

            if (_struggleOrb == 200 && !DEBOUNCE[5] && WORLD_READ == null)
                DEBOUNCE[5] = true;

            if (DEBOUNCE[5] && _battleRead == 0x00 && _fadeRead == 0x00 && _loadRead == 0x01)
            {
                var _itemPoint = (long)(Hypervisor.PureAddress & 0xFFFF00000000) + Variables.SharpHook[(IntPtr)0x3E1210].Execute<long>(0xF9);
                Hypervisor.Write((ulong)_itemPoint + 0x14, 360, true);

                Hypervisor.UnlockBlock(Hypervisor.PureAddress + 0x39F556, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x39F556, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 }, true);

                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x800000, new byte[] { 0x00, 0x00, 0xF9, 0x00 }, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x810000, "Struggle Champion".ToKHSCII(), true);

                Variables.SharpHook[(IntPtr)0x39F7C0].Execute((long)(Hypervisor.PureAddress + 0x800000));

                var _y = new string[]
                {
                    "mov rcx, " + (long)(Hypervisor.PureAddress + 0x810000),
                    "mov rdx, " + 360,
                    "mov r8, " + (long)(Hypervisor.PureAddress + 0x157150),
                    "jmp r8"
                };

                Variables.SharpHook.Assembly.InjectAndExecute(_y);
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x39F556, new byte[] { 0xE8, 0x15, 0x22, 0x02, 0x00 }, true);

                Hypervisor.Write<byte>(0x24A8BFA, 0x00);
                DEBOUNCE[5] = false;
            }
        }
        */

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

            if (_roomRead == 0x00 && _abilityData.Contains<ushort>(0x80F8) && !DEBOUNCE[3] && WORLD_READ == null)
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
                }

                else if (_widthExpect < _widthRender)
                {
                    var _ratioWidth = _widthRender / _widthExpect;

                    Hypervisor.Write(_widthViewportAddr, _ratioWidth);
                    Hypervisor.Write(_heightViewportAddr, 1F);

                    var _uiWidth = _prevWidthUI / _ratioWidth;
                    Hypervisor.Write(_uiWidthAddr, _uiWidth);
                }

                else
                {
                    Hypervisor.Write(_heightViewportAddr, 1F);
                    Hypervisor.Write(_widthViewportAddr, 1F);
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
            }
        }

        /// <summary>
        /// Enforces the controller prompts instead of detecting KBM when requested.
        /// </summary>
        public static void PromptSelector()
        {
            var _contCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);

            if (Variables.autoController != 0x02)
            {
                if (_contCheck != 0x90)
                {
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                    Helpers.Log("Manual prompt mode detected! Enforcing prompts...", 0);
                }

                Hypervisor.Write<long>(Variables.ADDR_ControllerMode, Variables.autoController);
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
                var _healthRead = Hypervisor.Read<byte>(Variables.ADDR_SoraHP);
                var _magicRead = Hypervisor.Read<byte>(Variables.ADDR_SoraHP + 0x180);

                var _healthMax = Hypervisor.Read<byte>(Variables.ADDR_SoraHP + 0x04);
                var _magicMax = Hypervisor.Read<byte>(Variables.ADDR_SoraHP + 0x184);

                if (_healthRead == _healthMax && _magicRead == _magicMax)
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, 0x74, true);

                else
                    Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_SaveEffectINST, 0x75, true);
            }
        }

        /// <summary>
        /// Hides the Magic Bar until Sora is either in Limit Form, or has learned a Magic Spell.
        /// </summary>
        public static void MagicHide()
        {
            if (Hypervisor.Read<uint>(Variables.ADDR_MagicLV1) == 0x00000000 && Hypervisor.Read<ushort>(Variables.ADDR_MagicLV2) == 0x0000 && Hypervisor.Read<byte>(Variables.ADDR_SoraForm) != 0x03)
            {
                if (Hypervisor.Read<byte>(Variables.ADDR_MPSEQD[0]) != 0x00)
                {
                    Helpers.Log("No spells or Limit Form detected! Hiding the MP Bar...", 0);
                    for (int i = 0; i < Variables.ADDR_MPSEQD.Length; i++)
                        Hypervisor.Write<byte>(Variables.ADDR_MPSEQD[i], 0x00);
                }
            }

            else if (Hypervisor.Read<byte>(Variables.ADDR_MPSEQD[0]) == 0x00 || (Hypervisor.Read<byte>(Variables.ADDR_MPSEQD[0]) == 0x00 && Hypervisor.Read<byte>(Variables.ADDR_SoraForm) == 0x03))
            {
                Helpers.Log("A spell or Limit Form detected! Showing the MP Bar...", 0);

                for (int i = 0; i < Variables.ADDR_MPSEQD.Length; i++)
                    Hypervisor.Write<byte>(Variables.ADDR_MPSEQD[i], Variables.VALUE_MPSEQD[i]);
            }
        }

        #endregion

        #region Switch-Up Functions

        /// <summary>
        /// Switches between English and Japanese dialogue on demand.
        /// </summary>
        public static void SwitchAudio()
        {
            var _stringCheck = Hypervisor.ReadTerminate(Variables.ADDR_PAXFormatter);

            var _stringANM = "anm/{0}/";
            var _stringPAX = "obj/%s.a.{0}";
            var _stringEVT = "voice/{0}/event";
            var _stringBTL = "voice/{0}/battle";

            if (Variables.audioMode == 0x01 && _stringCheck != "obj/%s.a.jp")
            {
                Helpers.Log("Switching to Japanese Audio...", 0);

                WL_SUFF = "jp";
                US_SUFF = "jp";
                FM_SUFF = "jp";
            }

            if (Variables.audioMode == 0x02 && _stringCheck != "obj/%s.a.xx")
            {
                Helpers.Log("Switching to Extra Audio...", 0);

                WL_SUFF = "xx";
                US_SUFF = "xx";
                FM_SUFF = "xx";
            }

            else if (Variables.audioMode == 0x00 && _stringCheck != "obj/%s.a.%s")
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

                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, String.Format(_stringBTL, US_SUFF));
                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, String.Format(_stringEVT, US_SUFF));
            }
        }

        /// <summary>
        /// Switches between Classic and Remastered music on demand.
        /// </summary>
        public static void SwitchMusic()
        {
            if (Variables.vanillaMusic != PAST_MUSIC)
            {
                Helpers.Log(String.Format("Switching Music to {0}...", Variables.vanillaMusic ? "Vanilla" : "Remastered"), 0);

                var _musicPrefix = Variables.vanillaMusic ? new byte[] { 0x70, 0x73, 0x32, 0x6D, 0x64 } : new byte[] { 0x6D, 0x75, 0x73, 0x69, 0x63 };
                Hypervisor.WriteArray(Variables.ADDR_MusicPath, _musicPrefix);

                PAST_MUSIC = Variables.vanillaMusic;
            }
        }

        /// <summary>
        /// Switches between Classic and Final Mix Heartless on demand.
        /// </summary>
        public static void SwitchEnemies()
        {
            byte _bossPrefix = Variables.vanillaEnemy ? (byte)0x56 : (byte)0x42;
            byte _enemyPrefix = Variables.vanillaEnemy ? (byte)0x56 : (byte)0x4D;

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

                else if (Variables.vanillaEnemy != PAST_ENEMY)
                {
                    Helpers.Log(String.Format("Switching Enemies to the {0} Palette...", Variables.vanillaEnemy ? "Classic" : "Special"), 0);

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

                    PAST_ENEMY = Variables.vanillaEnemy;
                }
            }
        }

        #endregion

        public static void Execute()
        {
            if (!Variables.Initialized)
            {
                Initialization();
                Thread.Sleep(10000);
            }

            else
            {
                PromptSelector();

                if (REMOVED_LIST[0] == 0x00)
                    SwitchAudio();

                if (REMOVED_LIST[1] == 0x00)
                    SwitchMusic();

                if (REMOVED_LIST[2] == 0x00)
                    SwitchEnemies();

                ExitFix();
                LogicEP();
                MagicSorting();
                FixSavePoint();
                TutorialSkip();
                ShortcutForms();
                OverrideLimits();
                OverrideLimiter();
                
                MagicHide();
                RetryPrompt();
                
                if (Variables.adjustRatio)
                    AspectAdjust();

                ResetGame();
                ConfigHandler();

                #region Tasks
                if (Variables.ASTask == null)
                {
                    Variables.ASTask = Task.Factory.StartNew(

                        delegate ()
                        {
                            while (!Variables.Token.IsCancellationRequested)
                            {
                                AutosaveEngine();
                                Thread.Sleep(5);
                            }
                        },

                        Variables.Token
                    );
                }

                if (Variables.DCTask == null)
                {
                    Variables.DCTask = Task.Factory.StartNew(

                        delegate ()
                        {
                            while (!Variables.Token.IsCancellationRequested)
                            {
                                DiscordEngine();
                                Thread.Sleep(5);
                            }
                        },

                        Variables.Token
                    );
                }

                if (Variables.CRTask == null)
                {
                    Variables.CRTask = Task.Factory.StartNew(

                        delegate ()
                        {
                            while (!Variables.Token.IsCancellationRequested)
                            {
                                CrownManager();
                                Thread.Sleep(5);
                            }
                        },

                        Variables.Token
                    );
                }
                #endregion
            }
        }
    }
}
