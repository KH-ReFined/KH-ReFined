using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Collections.Generic;

using Binarysharp.MSharp;

namespace ReFined
{
    public class Functions
    {
        #region Variable Region

        static ulong ULONG_MINIMAL = 0xFFFFFFFFFFFFFFFF;

        static byte SAVE_ROOM;
        static byte SAVE_WORLD;
        static byte SKIP_STAGE;
        static sbyte CURR_CONFIG;
        static sbyte CONF_INDEX;
        static byte SAVE_ITERATOR;

        static bool PAST_ENEMY;
        static bool SAVE_RESET;
        static bool ROOM_LOADED;
        static bool ATTACK_SWITCH;

        static uint MAGIC_LV1;
        static ushort MAGIC_LV2;
        static int OFFSET_TITLE;
        static int OFFSET_DESC;
        static int OFFSET_OPTION_ONE;
        static int OFFSET_OPTION_TWO;

        static int RETRY_MODE = 0x00;
        static bool RETRY_LOCK;
        static bool RETRY_BLACKLIST;

        static byte SUMM_LVL_READ;
        static byte SUMM_EXP_READ;

        static short[] LIMIT_SHORT;

        static byte[] MAGIC_STORE;
        static byte[] OBJENTRY_READ;

        static byte[] RETRY_TEXT;
        static byte[] CONTINUE_TEXT;

        static ushort ROXAS_BLADE;

        static int EXP_READ;
        static byte FORM_READ;
        static byte[] DRIVE_READ;
        static byte[] CHEST_READ;
        static byte[] PARTY_READ;
        static byte[] FORM_STAT_READ;
        static byte[] INVENTORY_READ;
        static List<byte[]> LVL_READ;
        static List<byte[]> ITEM_READ;
        static List<byte[]> ABILITY_READ;


        static bool[] DEBOUNCE = new bool[] { false, false, false, false, false };

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

                // Create the TEMP Path to store our sound files.
                if (!Directory.Exists(Path.GetTempPath() + "ReFined"))
                    Directory.CreateDirectory(Path.GetTempPath() + "ReFined");

                // Open the config file for game-specific configs.
                var _configIni = new TinyIni("reFined.ini");

                Variables.SharpHook = new MemorySharp(Hypervisor.Process);

                // Parse the Festive Toggle, and the chosen Limit Form shortcuts, default Retry Setting, and the Drive Shortcut setting.
                Variables.festiveToggle = Convert.ToBoolean(_configIni.Read("festivityEngine", "Kingdom Hearts II"));
                Variables.driveToggle = Convert.ToBoolean(_configIni.Read("driveShortcuts", "Kingdom Hearts II"));
                Variables.resetPrompt = Convert.ToBoolean(_configIni.Read("resetPrompt", "Kingdom Hearts II"));
                Variables.limitShorts = _configIni.Read("limitShortcuts", "Kingdom Hearts II");
                Variables.retryDefault = _configIni.Read("deathPrompt", "Kingdom Hearts II") == "retry" ? true : false;

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

                var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

                var _logsDir = Path.Combine(_documentsPath, "Kingdom Hearts/Logs");
                var _saveDir = Path.Combine(_documentsPath, "Kingdom Hearts/Save Data");
                var _configDir = Path.Combine(_documentsPath, "Kingdom Hearts/Configuration");

            EPIC_INIT:
                if (Directory.Exists(_saveDir))
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

                // Remove the config options if the optionals are not patched in.
                if (!Operations.ScanHash(Path.Combine(_pathPrefix, "kh2_first.hed"), Variables.HASH_SwapMusic))
                {
                    Helpers.Log("Music Addon not found! Removing from config...", 1);
                    Variables.VALUE_ConfigIndex.Remove(0x02);
                }

                if (!Operations.ScanHash(Path.Combine(_pathPrefix, "kh2_fifth.hed"), Variables.HASH_SwapEnemy))
                {
                    Helpers.Log("Heartless Addon not found! Removing from config...", 1);
                    Variables.VALUE_ConfigIndex.Remove(0x03);
                }

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

            if (_buttRead == 0x090C && !DEBOUNCE[0])
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

            else if (_buttRead != 0x090C && DEBOUNCE[0])
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
                Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[0]);
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[3]);
            }

            else if (_confirmRead == 0x01 && _shortRead != LIMIT_SHORT[3] && _modeRead == 0)
            {
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
            var _toggleCheck = Hypervisor.Read<ushort>(Variables.ADDR_Config) & 0x01;
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

                var _saveConfig = Variables.DualAudio && Variables.saveToggle;
                var _saveableBool = (_saveConfig ? _saveConfig : _toggleCheck == 0x01) && _battleRead == 0x00 && _loadRead == 0x01 && _cutsceneRead == 0x00 && _worldCheck >= 0x02 && _pauseCheck == 0x00;

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
            var _iconByte = Hypervisor.Read<byte>(0x2506F7D);

            if (Variables.driveToggle && _instCheck != 0x90 && _iconByte != 0x00)
            {
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
            }
        }

        /// <summary>
        /// Allows the vibration option to house a lot more than just itself.
        /// Switching is done with L2/R2.
        /// </summary>
        public static void ConfigSwap()
        {
            var _selectPoint = Hypervisor.Read<ulong>(Variables.PINT_SubMenuSelect);
            var _optionPoint = Hypervisor.Read<ulong>(Variables.PINT_SubOptionSelect);

            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_SubMenu);

            var _selectRead = Hypervisor.Read<byte>(_selectPoint, true);
            var _optionRead = Hypervisor.Read<byte>(_optionPoint + 0xF9, true);

            var _offsetTitle = Operations.FindInfoMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigTitle[0]) + 0x04;
            var _offsetDesc = Operations.FindInfoMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigDesc[0]) + 0x04;

            var _offsetOption1 = Operations.FindInfoMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigOption1[0]) + 0x04;
            var _offsetOption2 = Operations.FindInfoMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigOption2[0]) + 0x04;

            if (OFFSET_TITLE == 0x00)
            {
                OFFSET_DESC = Hypervisor.Read<int>(_offsetDesc, true);
                OFFSET_TITLE = Hypervisor.Read<int>(_offsetTitle, true);
                OFFSET_OPTION_ONE = Hypervisor.Read<int>(_offsetOption1, true);
                OFFSET_OPTION_TWO = Hypervisor.Read<int>(_offsetOption2, true);
            }

            if (_menuRead == 0x24)
            {
                var _buttRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);

                var _seekL2 = _buttRead & 0x0001;
                var _seekR2 = _buttRead & 0x0002;

                var _iniFetch = new TinyIni("reFined.ini");

                if (_selectRead == 0x05)
                {
                    switch (CURR_CONFIG)
                    {
                        case 0:
                                _iniFetch.Write("vibration", Convert.ToBoolean(_optionRead) ? "false" : "true", "General");
                            break;
                        case 1:
                                _iniFetch.Write("audioLanguage", Convert.ToBoolean(_optionRead) ? "japanese" : "english", "General");
                                Variables.japaneseAudio = Convert.ToBoolean(_optionRead);
                            break;
                        case 2:
                                _iniFetch.Write("musicMode", Convert.ToBoolean(_optionRead) ? "vanilla" : "remastered", "General");
                                Variables.vanillaMusic = !Convert.ToBoolean(_optionRead);
                            break;
                        case 3:
                                _iniFetch.Write("heartlessColors", Convert.ToBoolean(_optionRead) ? "special" : "classic", "General");
                                Variables.vanillaEnemy = !Convert.ToBoolean(_optionRead);
                            break;
                        case 4:
                                _iniFetch.Write("controllerPrompt", Convert.ToBoolean(_optionRead) ? "true" : "auto", "General");
                                Variables.autoController = Convert.ToBoolean(_optionRead);
                            break;
                    }
                }

                if (_selectRead == 0x05 && (_seekL2 == 0x01 || _seekR2 == 0x02) && !DEBOUNCE[4])
                {
                    DEBOUNCE[4] = true;

                    if (_seekR2 == 0x02)
                    {
                        CONF_INDEX++;

                        if (CONF_INDEX >= Variables.VALUE_ConfigIndex.Count())
                            CONF_INDEX = 0;
                    }

                    else if (_seekL2 == 0x01)
                    {
                        CONF_INDEX--;

                        if (CONF_INDEX <= -1)
                            CONF_INDEX = (sbyte)Variables.VALUE_ConfigIndex.Count();
                    }

                    CURR_CONFIG = Variables.VALUE_ConfigIndex[CONF_INDEX];

                    var _replaceDesc = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigDesc[CURR_CONFIG]);
                    var _replaceTitle = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigTitle[CURR_CONFIG]);
                    var _replaceOption1 = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigOption1[CURR_CONFIG]);
                    var _replaceOption2 = Operations.FetchOffsetMSG(Variables.PINT_SystemBAR, Variables.VALUE_ConfigOption2[CURR_CONFIG]);

                    if (CURR_CONFIG == 0x00)
                    {
                        _replaceDesc = OFFSET_DESC;
                        _replaceTitle = OFFSET_TITLE;
                        _replaceOption1 = OFFSET_OPTION_ONE;
                        _replaceOption2 = OFFSET_OPTION_TWO;
                    }

                    Additions.PlaySFX(1);

                    Hypervisor.Write(_offsetTitle, _replaceTitle, true);

                    Hypervisor.Write(_offsetDesc, _replaceDesc, true);
                    Hypervisor.Write(_offsetDesc + 0x08, _replaceDesc, true);

                    Hypervisor.Write(_offsetOption1, _replaceOption1, true);
                    Hypervisor.Write(_offsetOption2, _replaceOption2, true);

                    Variables.SharpHook[(IntPtr)0x363380].Execute();
                    Variables.SharpHook[(IntPtr)0x363340].Execute();
                }

                else if (_buttRead == 0x00 && DEBOUNCE[4])
                    DEBOUNCE[4] = false;
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

            var _vibRead = Hypervisor.Read<ushort>(Variables.ADDR_Config) & 0x01;
            var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);

            var _countButton = Hypervisor.Read<byte>(Variables.ADDR_TitleCount);
            var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);

            if (!Operations.CheckTitle())
            {
                var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
                var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);
                var _eventCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x04);

                var _cutsceneCheck = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                if (_worldCheck == 0x02 && _roomCheck == 0x01 && _eventCheck == 0x38 && SKIP_STAGE == 0)
                {
                    if (_vibRead == 0x00)
                    {
                        Helpers.Log("Room and Settings are correct! Initiating Roxas Skip's First Phase...", 0);

                        Hypervisor.Write<uint>(Variables.ADDR_World, 0x322002);
                        Hypervisor.Write<uint>(Variables.ADDR_World + 0x04, 0x01);
                        Hypervisor.Write<uint>(Variables.ADDR_World + 0x08, 0x01);

                        Hypervisor.Write<uint>(0x444832, 0x1FF00001);
                        Hypervisor.Write<uint>(0x444832 + 0x04, 0x00000000);

                        Hypervisor.Write(Variables.ADDR_Config, (ushort)(_vibRead + 0x01));

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
        /// Adds the option to retry a past fight.
        /// </summary>
        public static void RetryPrompt()
        {
            if (CONTINUE_TEXT == null)
            {
                RETRY_TEXT = Operations.FetchStringMSG(Variables.PINT_SystemBAR, 0x0AB1);
                CONTINUE_TEXT = Operations.FetchStringMSG(Variables.PINT_SystemBAR, 0x0AB4);
            }

            // So, we initialize all of this shit just to determine whether Retry will show up or not.
            var _menuPoint = Hypervisor.Read<ulong>(Variables.PINT_DeadMenu);

            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_DeadSelect);
            var _optionRead = Hypervisor.Read<byte>(Variables.ADDR_DeadCount);

            var _bttlByte = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
            var _cutsByte = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);
            var _loadByte = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _pausRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);

            var _formByte = Hypervisor.Read<byte>(Variables.ADDR_SoraForm);
            var _fnshByte = Hypervisor.Read<byte>(Variables.ADDR_FinishFlag);

            var _buttRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
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

                This currently DOES NOT work for alternative lose conditions. Oh well, that can be fixed another day I suppose.
                But hey! At least this is currently Mickey Compliant!
            */


            // If one's on the Title Screen while Retry is active: Deactivate it.
            if (Operations.CheckTitle() && (RETRY_LOCK || RETRY_MODE == 0x01))
            {
                Helpers.Log("Title Screen detected on Retry Mode! Restoring ..", 0);

                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x39EF31, new byte[] { 0x66, 0x89, 0x01 }, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x39EF36, new byte[] { 0x66, 0x89, 0x05, 0x5D, 0xC1, 0x60, 0x00 }, true);

                EXP_READ = -1;
                FORM_READ = 0;

                DRIVE_READ = null;
                CHEST_READ = null;
                PARTY_READ = null;

                FORM_STAT_READ = null;
                INVENTORY_READ = null;

                LVL_READ = new List<byte[]>();
                ITEM_READ = new List<byte[]>();
                ABILITY_READ = new List<byte[]>();

                SUMM_LVL_READ = 0xFF;
                SUMM_EXP_READ = 0xFF;

                ROXAS_BLADE = 0x0000;

                RETRY_MODE = 0;
                RETRY_LOCK = false;
            }

            // Cavern of Remembrance Blacklist.
            var _cavernCheck = _worldRead == 0x04 && (_roomRead >= 0x15 && _roomRead <= 0x1A);

            if (!_cavernCheck)
            {
                // Read the necessary shits at the start of a fight.
                if (DRIVE_READ == null && _bttlByte == 0x02 && _cutsByte == 0x00 && _pausRead == 0x00 && !Operations.CheckTitle())
                {
                READ_STUFF:

                    LVL_READ = new List<byte[]>();
                    ITEM_READ = new List<byte[]>();
                    ABILITY_READ = new List<byte[]>();

                    for (int i = 0; i < 13; i++)
                        ITEM_READ.Add(Hypervisor.ReadArray(Variables.ADDR_ItemStart + (ulong)(0x114 * i), 0x10));

                    for (int i = 0; i < 13; i++)
                        ABILITY_READ.Add(Hypervisor.ReadArray(Variables.ADDR_AbilityStart + (ulong)(0x114 * i), 0xC0));

                    for (int i = 0; i < 13; i++)
                        LVL_READ.Add(Hypervisor.ReadArray(Variables.ADDR_LevelStart + (ulong)(0x114 * i), 0x04));

                    EXP_READ = Hypervisor.Read<int>(Variables.ADDR_EXPStart);
                    FORM_READ = Hypervisor.Read<byte>(Variables.ADDR_SoraForm);

                    DRIVE_READ = Hypervisor.ReadArray(Variables.ADDR_DriveStart, 0x0C);
                    PARTY_READ = Hypervisor.ReadArray(Variables.ADDR_PartyStart, 0x08);
                    CHEST_READ = Hypervisor.ReadArray(Variables.ADDR_ChestStart, 0x33);

                    FORM_STAT_READ = Hypervisor.ReadArray(Variables.ADDR_FormStart, 0x38 * 0x0A);
                    INVENTORY_READ = Hypervisor.ReadArray(Variables.ADDR_Inventory, 0x140);

                    SUMM_LVL_READ = Hypervisor.Read<byte>(Variables.ADDR_SummonLevel);
                    SUMM_EXP_READ = Hypervisor.Read<byte>(Variables.ADDR_SummonEXP);

                    ROXAS_BLADE = Hypervisor.Read<ushort>(Hypervisor.BaseAddress + 0x445052, true);

                    if (DRIVE_READ[2] == 0x00)
                        goto READ_STUFF;

                    else
                        Helpers.Log("Start of forced fight, reading necessary values into memory...", 0);
                }

                // Flush the memory post-fight.
                else if ((_bttlByte != 0x02 || _cutsByte != 0x00) && _pausRead == 0x00 && DRIVE_READ != null)
                {
                    Helpers.Log("The player is out of battle. Flushing memory...", 0);

                    EXP_READ = -1;
                    FORM_READ = 0;

                    DRIVE_READ = null;
                    CHEST_READ = null;
                    PARTY_READ = null;

                    FORM_STAT_READ = null;
                    INVENTORY_READ = null;

                    LVL_READ = new List<byte[]>();
                    ITEM_READ = new List<byte[]>();
                    ABILITY_READ = new List<byte[]>();

                    SUMM_LVL_READ = 0xFF;
                    SUMM_EXP_READ = 0xFF;

                    ROXAS_BLADE = 0x0000;
                }

                // This code blob is responsible for switching between Retry and Continue
                // and only runs if Sora is dead and some sort of a menu is present.
                if (_menuPoint != 0x00 && _bttlByte == 0x02)
                {
                    if (_menuRead == 0x01 && _warpRead == 0x90 && RETRY_MODE == 0x01)
                    {
                        Helpers.Log("User is going to load the game! Restoring ..", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                        RETRY_LOCK = true;
                    }

                    else if (_menuRead == 0x00 && _optionRead == 0x02 && _warpRead != 0x90 && RETRY_MODE == 0x01)
                    {
                        Helpers.Log("User defected from loading! Destroying ..", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, _nullArray, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, _nullArray, true);

                        RETRY_LOCK = true;
                    }

                    // If D-Pad sides are pressed;
                    if (((_buttRead & 0x2000) == 0x2000 || (_buttRead & 0x8000) == 0x8000) && !DEBOUNCE[2] && _menuRead == 0x00 && RETRY_MODE != 0x03)
                    {
                        // Play the sound so that it seems **authentic**.
                        Additions.PlaySFX(0x01);

                        Helpers.Log(String.Format("Switching to \"{0}\" mode.", RETRY_MODE == 0x00 ? "Retry" : "Continue"), 0);

                        // Retry Mode Switch!
                        RETRY_MODE = RETRY_MODE == 0x00 ? 0x01 : 0x00;

                        // This handles the "Continue" mode and restores the function to do room switching.
                        if (RETRY_MODE == 0x00 && _warpRead == 0x90)
                        {
                            Helpers.Log("Switched to Continue mode! Restoring ..", 0);

                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);
                        }

                        // This handles the "Retry" mode and destroys the function to do room switching.
                        else if (RETRY_MODE == 0x01 && _warpRead != 0x90)
                        {
                            Helpers.Log("Switched to Retry mode! Destroying ...", 0);

                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, _nullArray, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, _nullArray, true);
                        }

                        RETRY_LOCK = true;
                        DEBOUNCE[2] = true;
                    }

                    // Debouncing.
                    else if (((_buttRead & 0x2000) != 0x2000 && (_buttRead & 0x8000) != 0x8000) && DEBOUNCE[2])
                        DEBOUNCE[2] = false;
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
                        RETRY_MODE = 0x03;
                    }

                    else
                    {
                        if (Variables.retryDefault)
                        {
                            RETRY_MODE = 0x01;

                            Helpers.Log("Death Screen detected! Retry is default! Destroying ..", 0);

                            // Destroy the functions responsible for switching rooms and reverting story flags.
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, _nullArray, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, _nullArray, true);

                            Hypervisor.Write(Variables.ADDR_SoraForm, FORM_READ);
                        }

                        else
                        {
                            RETRY_MODE = 0x00;
                            Helpers.Log("Death Screen detected! Continue is default! Changing nothing...", 0);
                        }

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
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                    if (RETRY_MODE == 0x01 && _cutsByte == 0x00 && _fnshByte != 0x01 && DRIVE_READ != null)
                    {
                        while (_pausRead == 0x01)
                            _pausRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);

                        for (int i = 0; i < 13; i++)
                            Hypervisor.WriteArray(Variables.ADDR_ItemStart + (ulong)(0x114 * i), ITEM_READ[i]);

                        for (int i = 0; i < 13; i++)
                            Hypervisor.WriteArray(Variables.ADDR_AbilityStart + (ulong)(0x114 * i), ABILITY_READ[i]);

                        for (int i = 0; i < 13; i++)
                            Hypervisor.WriteArray(Variables.ADDR_LevelStart + (ulong)(0x114 * i), LVL_READ[i]);

                        Hypervisor.WriteArray(Variables.ADDR_PartyStart, PARTY_READ);

                        Hypervisor.Write(Variables.ADDR_SummonLevel, SUMM_LVL_READ);
                        Hypervisor.Write(Variables.ADDR_SummonEXP, SUMM_EXP_READ);

                        Hypervisor.WriteArray(Variables.ADDR_ChestStart, CHEST_READ);

                        Hypervisor.Write(Variables.ADDR_EXPStart, EXP_READ);
                        Hypervisor.WriteArray(Variables.ADDR_FormStart, FORM_STAT_READ);
                        Hypervisor.WriteArray(Variables.ADDR_Inventory, INVENTORY_READ);
                        Hypervisor.WriteArray(Variables.ADDR_DriveStart, DRIVE_READ);
                    }

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

            // If the retry text offset is set, write the text necessary according to the mode.
            if (RETRY_MODE != 0x03)
            {
                var _continuePointer = (ulong)Operations.FetchPointerMSG(Variables.PINT_SystemBAR, 0x0AB0);

                if (_menuPoint == 0x00 || _cutsByte != 0x00 || _bttlByte != 0x02)
                    Hypervisor.WriteArray(_continuePointer, CONTINUE_TEXT, true);

                else
                    Hypervisor.WriteArray(_continuePointer, RETRY_MODE == 0x01 ? RETRY_TEXT : CONTINUE_TEXT, true);
            }
        }

        #endregion

        #region Additional In-Game Functionality

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

            if (_roomRead == 0x00 && _abilityData.Contains<ushort>(0x80F8) && !DEBOUNCE[3])
            {
                Helpers.Log("Enemy data has been cleared!", 0);
                Hypervisor.WriteArray(_roomPoint, new byte[0x100], true);
                DEBOUNCE[3] = true;
            }

            else if (_roomRead == 0x01 && DEBOUNCE[3])
                DEBOUNCE[3] = false;
        }

        #endregion


        #region Accessibility

        /// <summary>
        /// Allows for Automatically Attacking by holding CONFIRM.
        /// </summary>
        public static void Autoattack()
        {
            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

            var _menuRead = Hypervisor.Read<byte>(0x24AA3A6);

            var _primaryRead = Hypervisor.Read<byte>(0x24A986E);
            var _secondaryRead = Hypervisor.Read<byte>(0x24A9B6E);

            var _dialogRead = Hypervisor.Read<byte>(0x24AF4C2);

            var _buttonSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
            var _inputValue = _inputRead & _buttonSeek;

            var _autoBool = _inputValue == _buttonSeek && ((_menuRead == 0x00 && _primaryRead == 0x00) || (_menuRead == 0x06 && _secondaryRead == 0x00)) && _dialogRead == 0x00 && _worldCheck != 0x0F && Variables.attackToggle == true;
            var _actionRead = Hypervisor.Read<byte>(Variables.ADDR_ActionExe);

            if (_autoBool && _actionRead != 0x01 && !ATTACK_SWITCH)
            {
                Hypervisor.Write(Variables.ADDR_ActionExe, 0x01);
                ATTACK_SWITCH = true;
            }

            else if (!_autoBool && _actionRead != 0x00 && ATTACK_SWITCH)
            {
                Hypervisor.Write(Variables.ADDR_ActionExe, 0x00);
                ATTACK_SWITCH = false;
            }
        }

        #endregion


        #region Visual Enhancements

        /// <summary>
        /// Enforces the controller prompts instead of detecting KBM when requested.
        /// </summary>
        public static void PromptSelector()
        {
            var _contCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);

            if (!Variables.autoController)
            {
                if (_contCheck != 0x90)
                {
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                    Helpers.Log("Manual prompt mode detected! Enforcing prompts...", 0);
                }

                Hypervisor.Write(Variables.ADDR_ControllerMode, (byte)(Variables.contToggle ? 0 : 1));
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

        #endregion

        #region Switch-Up Functions

        /// <summary>
        /// Switches between English and Japanese dialogue on demand.
        /// </summary>
        public static void SwitchAudio()
        {
            var _stringCheck = Hypervisor.ReadTerminate(Variables.ADDR_PAXFormatter);

            var _wildIndicator = "%s";
            var _usIndicator = "us";
            var _fmIndicator = "fm";

            var _stringANM = "anm/{0}/";
            var _stringPAX = "obj/%s.a.{0}";
            var _stringEVT = "voice/{0}/event";
            var _stringBTL = "voice/{0}/battle";

            if (Variables.japaneseAudio && _stringCheck != "obj/%s.a.jp")
            {
                Helpers.Log("Switching to Japanese Audio...", 0);

                _wildIndicator = "jp";
                _usIndicator = "jp";
                _fmIndicator = "jp";
            }

            else if (!Variables.japaneseAudio && _stringCheck != "obj/%s.a.%s")
            {
                Helpers.Log("Switching to English Audio...", 0);

                _wildIndicator = "%s";
                _usIndicator = "us";
                _fmIndicator = "fm";
            }

            if (_stringCheck != String.Format(_stringPAX, _wildIndicator))
            {
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter, String.Format(_stringPAX, _wildIndicator));
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter + 0x10, String.Format(_stringPAX, _usIndicator));

                Hypervisor.WriteString(Variables.ADDR_ANBFormatter, String.Format(_stringANM, _usIndicator));
                Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, String.Format(_stringANM, _fmIndicator));

                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, String.Format(_stringBTL, _usIndicator));
                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, String.Format(_stringEVT, _usIndicator));
            }
        }

        /// <summary>
        /// Switches between Classic and Remastered music on demand.
        /// </summary>
        public static void SwitchMusic()
        {
            var _musicPrefix = Variables.vanillaMusic ? new byte[] { 0x70, 0x73, 0x32, 0x6D, 0x64 } : new byte[] { 0x6D, 0x75, 0x73, 0x69, 0x63 };
            Hypervisor.WriteArray(Variables.ADDR_MusicPath, _musicPrefix);
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
                    foreach (var _name in Variables.BOSSObjentry)
                    {
                        var _stringArr1 = Encoding.Default.GetBytes(_name);
                        var _stringArr2 = Encoding.Default.GetBytes(_name.Replace("B_", "V_"));

                        var _searchClassic = OBJENTRY_READ.FindValue(_stringArr2);
                        var _searchRemastered = OBJENTRY_READ.FindValue(_stringArr1);

                        if (_searchClassic == ULONG_MINIMAL && _searchRemastered == ULONG_MINIMAL)
                            break;

                        else
                            Hypervisor.Write(Variables.ADDR_ObjentryBASE + 0x08 + (_searchClassic == ULONG_MINIMAL ? _searchRemastered : _searchClassic), _enemyPrefix);
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
            try
            {
                PromptSelector();
                SwitchMusic();
                SwitchEnemies();

                ExitFix();
                LogicEP();
                MagicSorting();
                FixSavePoint();
                OverrideLimiter();
                ShortcutForms();
                OverrideLimits();

                RetryPrompt();

                ResetGame();
                ConfigSwap();

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
                #endregion
            }

            catch (Exception _caughtEx)
            {
                Helpers.LogException(_caughtEx);
                Helpers.Log("Re:Fined terminated with an exception!", 1);
                Environment.Exit(-1);
            }
        }
    }
}
