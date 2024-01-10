/*
==================================================
      KINGDOM HEARTS - RE:FINED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFined
{
    public class Functions
    {
        /*
            Variable Space!
          
            Yes, this class has one, too!
        */

        static byte[] MAGIC_STORE;
        static uint MAGIC_LV1;
        static ushort MAGIC_LV2;

        static bool[] DEBOUNCE = new bool[] { false, false, false, false, false };
        static bool ROOM_LOADED;

        static bool SKIP_INITIATED;
        static bool SKIP_COMPLETE;
        static byte SKIP_STAGE;

        static byte SAVE_ROOM;
        static byte SAVE_WORLD;
        static byte SAVE_ITERATOR;

        static byte LANGUAGE = 0xFF;

        static byte[] SYSBAR_HEADER;
        static ulong SYSBAR_POINTER;

        static List<ulong> ROXAS_OFFSETS;
        static List<ulong> SETTING_OFFSETS;
        static List<ulong> LIMIT_OFFSETS;

        static ulong ROXAS_TITLE_OFFSET;
        static ulong ROXAS_NO_OFFSET;

        static ulong SET_YESDESC_OFFSET;

        static ulong OPENKH_OFFSET;

        static ulong ON_OFFSET;
        static ulong OFF_OFFSET; 
        static ulong FULL_OFFSET;
        static ulong NONE_OFFSET;

        static bool DUB_FOUND;
        static bool ATTACK_SWITCH;

        static ulong CONTINUE_OFFSET;
        static ulong RETRY_OFFSET;

        static int RETRY_MODE = 0x00;
        static bool RETRY_LOCK;
        static bool RETRY_BLACKLIST;

        static short[] LIMIT_SHORT;
        
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

        static byte SUMM_LVL_READ;
        static byte SUMM_EXP_READ;

        static byte[] LIBRETTO_READ;
        static byte[] BARFILE_READ;

        static byte[] OBJENTRY_READ;

        static DateTime SAVE_TIME;
        static bool SAVE_RESET = true;

        /*
            Initialization:

            Serves to initialize stuffs.
        */

        public static void Initialization()
        {
            try
            {
                Helpers.Log("Initializing Re:Fined...", 0);
                
                // Create the TEMP Path to store our sound files.
                if (!Directory.Exists(Path.GetTempPath() + "ReFined"))
                    Directory.CreateDirectory(Path.GetTempPath() + "ReFined");
                    
                // Check if the sound files actually exist.
                if (!File.Exists(Variables.SwitchSFXPath))
                {
                    // Should they not, extract the sound files.
                    var _saveStream = File.Create(Variables.SaveSFXPath);
                    var _switchStream = File.Create(Variables.SwitchSFXPath);

                    Variables.SaveSFX.CopyTo(_saveStream);
                    Variables.SwitchSFX.CopyTo(_switchStream);
                }
                
                // Open the config file for game-specific configs.
                var _configIni = new TinyIni("reFinedLegacy.ini");

                // Parse the Festive Toggle, and the chosen Limit Form shortcuts, default Retry Setting, and the Drive Shortcut setting.
                Variables.festiveToggle = Convert.ToBoolean(_configIni.Read("festivityEngine", "Kingdom Hearts II"));
                Variables.driveToggle = Convert.ToBoolean(_configIni.Read("driveShortcuts", "Kingdom Hearts II"));
                Variables.limitShorts = _configIni.Read("limitShortcuts", "Kingdom Hearts II");
                Variables.retryDefault = _configIni.Read("defaultPrompt", "Kingdom Hearts II") == "retry" ? true : false;
                
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
                
                Hypervisor.UnlockBlock(Variables.ADDR_LimitShortcut);

                Hypervisor.UnlockBlock(Variables.ADDR_ShortListFilterINST);
                Hypervisor.UnlockBlock(Variables.ADDR_ShortEquipFilterINST);
                Hypervisor.UnlockBlock(Variables.ADDR_ShortCategoryFilterINST);
                Hypervisor.UnlockBlock(Variables.ADDR_ShortIconAssignINST);

                var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

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

        /*
            MusicAdjust:

            Changes the music to their PS2 variants if commanded.
        */
        public static void MusicAdjust()
        {
            if (Variables.vanillaMusic)
                Hypervisor.WriteArray(Variables.ADDR_MusicPath, new byte[] { 0x70, 0x73, 0x32, 0x6D, 0x64 });
        }

        public static void EnemyAdjust()
        {
            if (OBJENTRY_READ == null)
            {
                var _headerCheck = Hypervisor.Read<byte>(Variables.ADDR_ObjentryBASE);
                var _itemCount = Hypervisor.Read<int>(Variables.ADDR_ObjentryBASE + 0x04);

                if (_headerCheck == 0x03)
                    OBJENTRY_READ = Hypervisor.ReadArray(Variables.ADDR_ObjentryBASE + 0x08, 0x60 * _itemCount);
            }

            if (OBJENTRY_READ != null && Variables.vanillaEnemy)
            {
                foreach (var _name in Variables.BOSSObjentry)
                {
                    var _strArr = BitConverter.GetBytes(_name);
                    var _searchOffset = OBJENTRY_READ.FindValue(_strArr);

                    if (_searchOffset != 0xFFFFFFFFFFFFFFFF)
                        Hypervisor.Write<byte>(Variables.ADDR_ObjentryBASE + 0x08 + _searchOffset, 0x56);

                    else
                    {
                        Variables.vanillaEnemy = false;
                        break;
                    }
                }

                foreach(var _name in Variables.ENEMYObjentry)
                {
                    var _strArr = BitConverter.GetBytes(_name);
                    var _searchOffset = OBJENTRY_READ.FindValue<string>(_strArr);
                    
                    if (_searchOffset != 0xFFFFFFFFFFFFFFFF)
                        Hypervisor.Write<byte>(Variables.ADDR_ObjentryBASE + 0x08 + _searchOffset, 0x56);
                    
                    else
                    {
                        Variables.vanillaEnemy = false;
                        break;
                    }
                }
            }
        }

        /*
            DriveShortcuts:

            Allows Drive Forms to be shortcutted.
            This was a major pain in the ass to implement. I hope you guys enjoy this one.
            Tied to "driveShortcuts" in the config.
        */
        public static void DriveShortcuts()
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
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ShortCategoryFilterINST, new byte[] {0x90, 0x90}, true);
            }
        }

        /*
            Autoattack:

            Allows automatic attacking by holding down the action button. 
            Used primarily for accessibility purposes, tied to "autoAttack" in the config.
        */
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

        /*
            CheckTitle:

            Checks certain points in RAM to see if the player is in the Title Screen.
            Returns **true** if so, returns **false** otherwise. 
        */
        public static bool CheckTitle() =>
            Hypervisor.Read<uint>(Variables.ADDR_World) == 0x00FFFFFF
         || Hypervisor.Read<uint>(Variables.ADDR_World) == 0x00000101
         || Hypervisor.Read<uint>(Variables.ADDR_Title) == 0x00000001
         || Hypervisor.Read<uint>(Variables.ADDR_Reset) == 0x00000001;

       
        /*
            ResetGame:

            Triggers a soft-reset if the proper input is given.
            The input is sought in Execute().

            INPUT: L1 + R1 + START + SELECT.
        */
        public static void ResetGame()
        {
            var _buttRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);

            if (_buttRead == 0x090C && !DEBOUNCE[0])
            {
                Helpers.Log("Initiating a Soft Reset.", 0);

                Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);

                DEBOUNCE[0] = true;
            }

            else if (_buttRead != 0x090C && DEBOUNCE[0])
                DEBOUNCE[0] = false;
        }

        /*
            AdjustController:

            Allows the use of Controller Prompts and Camera on MKB and vice versa.
        */
        public static void AdjustControler()
        {
            if (!Variables.autoController)
            {
                var _contCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);

                if (_contCheck != 0x90)
                {
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                    Helpers.Log("Manual prompt mode detected! Enforcing prompts...", 0);
                }

                Hypervisor.Write<byte>(Variables.ADDR_ControllerMode, (byte)(Variables.contToggle ? 0 : 1));
            }
        }

        /*
            SortMagic:

            Shifts the chosen spell in the Magic menu up or down,
            depending on the input.

            INPUT: L2 + DOWN or L2 + UP.
        */
        public static void SortMagic()
        {
            // Fetch the status of the room. This will be necessary.
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            // Fetch the input and read the pointer to the second CM.
            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _menuPointer = Hypervisor.Read<ulong>(Variables.ADDR_MagicMenu[0]);

            // Fetch the current Magic Level arrays.
            var _magicOne = Hypervisor.Read<uint>(Variables.ADDR_MagicLV1);
            var _magicTwo = Hypervisor.Read<ushort>(Variables.ADDR_MagicLV2);

            // If a new spell is learned: Forget the sorting.
            if (_magicOne != MAGIC_LV1 || _magicTwo != MAGIC_LV2)
            {
                Helpers.Log("Spell change detected! Resetting sort memory.", 1);

                MAGIC_STORE = null;

                MAGIC_LV1 = _magicOne;
                MAGIC_LV2 = _magicTwo;
            }

            // If a soft reset id detected: Forget the sorting.
            if (Hypervisor.Read<byte>(Variables.ADDR_Reset) == 0x01)
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
                    var _insCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[0], true );

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
                            MAGIC_STORE = Hypervisor.ReadArray(Variables.ADDR_MagicMenu[1], _magicMax * 0x02);
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

        /*
            SkipRoxas:

            Determine whether to skip or go through Roxas' story.
            This is detemined by the **Vibration** option at the Title Screen.
        */
        public static void SkipRoxas()
        {
            if (CheckTitle() && SKIP_STAGE != 0)
            {
                Helpers.Log("Title Screen detected! Resetting Roxas Skip!", 0);
                SKIP_STAGE = 0;
            }

            var _vibRead = Hypervisor.Read<ushort>(Variables.ADDR_Config) & 0x01;
            var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);

            var _countButton = Hypervisor.Read<byte>(Variables.ADDR_TitleCount);
            var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);

            if (!CheckTitle())
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

        /*
            FrameOverride:

            Overwrites the frame limiter, and the instruction forcing it, according
            to the framerate chosen by the player.

            So you may be asking:
            "Topaz, why the fuck is this not an ASM Overwrite, and is an IL Hack?"

            Well you see, KH2 took a lazy approach for limiting the cutscenes to 30FPS.
            They just enable the framelimiter for the *whole game* instead of just limitting
            the cutscene like KH1 does. So if I overwrite that function in ASM, it will
            completely disable the 30FPS function. That is a big no-no since some do use it.

            So, this is the best solution to this problem. Just write a function to NOP
            that instruction whilst in 60FPS or above, but recover it once 30FPS is selected.

            These solutions have also paved the way to fixing the L2+Pad input problem of Magic Sort.
        */
        public static void FrameOverride()
        {
            // Calculate the instruction address.
            var _instructionAddress = Hypervisor.PureAddress + Variables.ADDR_LimiterINST;

            // Fetch the framerate, and the first byte of the instruction.
            var _framerateRead = Hypervisor.Read<byte>(Variables.ADDR_Framerate);
            var _instructionRead = Hypervisor.Read<byte>(_instructionAddress, true);

            // If the framerate is set to 30FPS, and the limiter is NOP'd out: Rewrite the instruction.
            if (_framerateRead == 0x00 && _instructionRead == 0x90)
            {
                Helpers.Log("30FPS Detected! Restoring the Framelimiter.", 0);
                Hypervisor.WriteArray(_instructionAddress, Variables.INST_FrameLimiter, true);
            }

            // Otherwise, if the framerate is not set to 30FPS, and the limiter is present:
            else if (_framerateRead != 0x00 && _instructionRead != 0x90)
            {
                Helpers.Log("60FPS Detected! Destroying the Framelimiter.", 0);

                // NOP the instruction.
                Hypervisor.WriteArray(_instructionAddress, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);

                // Set the current limiter to be off.
                Hypervisor.Write<byte>(Variables.ADDR_Limiter, 0x00);
            }
        }

        public static void ClassicSave()
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

        /*
            AudioSwap:

            Enforces English or Japanese Audio depending on player preference.
            This is detemined by the **Vibration** option at the Camp Menu.

            This function is reliant on a patch.
        */
        public static void AudioSwap()
        {
            var _toggleCheck = Hypervisor.Read<ushort>(Variables.ADDR_Config) & 0x01;
            var _stringCheck = Hypervisor.ReadTerminate(Variables.ADDR_PAXFormatter);

            if (_toggleCheck == 0x00 && _stringCheck != "obj/%s.a.jp")
            {
                Helpers.Log("Dual Audio switching to Japanese...", 0);

                Hypervisor.WriteString(Variables.ADDR_PAXFormatter, "obj/%s.a.jp");
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter + 0x10, "obj/%s.a.jp");

                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, "voice/jp/battle");

                Hypervisor.WriteString(Variables.ADDR_ANBFormatter, "anm/jp/");
                Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, "anm/jp/");

                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, "voice/jp/event");
            }

            else if (_toggleCheck == 0x01 && _stringCheck != "obj/%s.a.%s")
            {
                Helpers.Log("Dual Audio switching to English...", 0);

                Hypervisor.WriteString(Variables.ADDR_PAXFormatter, "obj/%s.a.%s");
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter + 0x10, "obj/%s.a.us");

                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, "voice/us/battle");

                Hypervisor.WriteString(Variables.ADDR_ANBFormatter, "anm/us/");
                Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, "anm/fm/");

                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, "voice/us/event");
            }
        }

        /*
            FixExit:

            I sorta kinda unknowningly broke the Exit function in KH.
            To fix this, this function exists.
        */
        public static void FixExit()
        {
            if (CheckTitle())
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

        /*
            RetryPrompt:

            Allows one to retry a forced fight instead of continuing.
            Toggling between Retry and Continue is done with D-Pad Sides.

            P.S. from Future Topaz: Fuck this function. It was a hellish experience to
            implement, test, fix, and stabilize it. But eh, it is worth it.
        */
        public static void RetryPrompt()
        {
            // So, we initialize all of this shit just to determine whether Retry will show up or not.
            var _menuPoint = Hypervisor.Read<ulong>(Variables.PINT_DeadMenu);

            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_MenuSelect);
            var _optionRead = Hypervisor.Read<byte>(Variables.ADDR_MenuCount);

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

            var _continueID = Strings.ContinueID;
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

            // If the offset for the text is not set, and we loaded the text file:
            // Do get the offset for the "Continue" text. And do something dumb, which is to
            // switch that text to Continue or Retry in KH2 memory, whichever is longer.
            if (RETRY_OFFSET == 0x00 && SYSBAR_HEADER != null)
            {
                switch (LANGUAGE)
                {
                    case 0x00:
                    case 0x03:
                    case 0x04:
                        RETRY_OFFSET = Hypervisor.Read<uint>(SYSBAR_POINTER + SYSBAR_HEADER.FindValue((uint)(_continueID)) + 0x04, true);
                    break;

                    case 0x01:
                    case 0x02:
                        RETRY_OFFSET = Hypervisor.Read<uint>(SYSBAR_POINTER + SYSBAR_HEADER.FindValue((uint)(_continueID + 0x01)) + 0x04, true);
                        CONTINUE_OFFSET = SYSBAR_HEADER.FindValue(_continueID) + 0x04;

                        Hypervisor.Write(SYSBAR_POINTER + CONTINUE_OFFSET, (uint)RETRY_OFFSET, true);
                    break;
                }
                
            }

            // If one's on the Title Screen while Retry is active: Deactivate it.
            if (CheckTitle() && (RETRY_LOCK || RETRY_MODE == 0x01))
            {
                Helpers.Log("Title Screen detected on Retry Mode! Restoring functions...", 0);

                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

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

                RETRY_MODE = 0;
                RETRY_LOCK = false;
            }

            // Cavern of Remembrance Blacklist.
            var _cavernCheck = _worldRead == 0x04 && (_roomRead >= 0x15 && _roomRead <= 0x1A);

            if (!_cavernCheck)
            {
                // Read the necessary shits at the start of a fight.
                if (DRIVE_READ == null && _bttlByte == 0x02 && _cutsByte == 0x00 && _pausRead == 0x00 && !CheckTitle())
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

                    if (DRIVE_READ[2] == 0x00)
                        goto READ_STUFF;

                    else
                        Helpers.Log(String.Format("Start of forced fight, reading necessary values into memory..."), 0);
                }

                // Flush the memory post-fight.
                else if ((_bttlByte != 0x02 || _cutsByte != 0x00) && _pausRead == 0x00 && DRIVE_READ != null)
                {
                    Helpers.Log(String.Format("The player is out of battle. Flushing memory..."), 0);

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
                }

                // This code blob is responsible for switching between Retry and Continue
                // and only runs if Sora is dead and some sort of a menu is present.
                if (_menuPoint != 0x00 && _bttlByte == 0x02)
                {
                    if (_menuRead == 0x01 && _warpRead == 0x90 && RETRY_MODE == 0x01)
                    {
                        Helpers.Log("User is going to load the game! Restoring functions...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                        RETRY_LOCK = true;
                    }

                    else if (_menuRead == 0x00 && _optionRead == 0x02 && _warpRead != 0x90 && RETRY_MODE == 0x01)
                    {
                        Helpers.Log("User defected from loading! Destroying functions...", 0);

                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, _nullArray, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, _nullArray, true);

                        RETRY_LOCK = true;
                    }

                    // If D-Pad sides are pressed;
                    if (((_buttRead & 0x2000) == 0x2000 || (_buttRead & 0x8000) == 0x8000) && !DEBOUNCE[2] && _menuRead == 0x00 && RETRY_MODE != 0x03)
                    {
                        // Play the sound so that it seems **authentic**.
                        Helpers.PlaySFX(Variables.SwitchSFXPath);

                        Helpers.Log(String.Format("Switching to \"{0}\" mode.", RETRY_MODE == 0x00 ? "Retry" : "Continue"), 0);

                        // Retry Mode Switch!
                        RETRY_MODE = RETRY_MODE == 0x00 ? 0x01 : 0x00;

                        // This handles the "Continue" mode and restores the function to do room switching.
                        if (RETRY_MODE == 0x00 && _warpRead == 0x90)
                        {
                            Helpers.Log("Switched to Continue mode! Restoring functions...", 0);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);
                        }

                        // This handles the "Retry" mode and destroys the function to do room switching.
                        else if (RETRY_MODE == 0x01 && _warpRead != 0x90)
                        {
                            Helpers.Log("Switched to Retry mode! Destroying functions....", 0);
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

                var _menuCheck = Hypervisor.Read<ushort>(Variables.ADDR_MenuCount + 0x02);

                // If in a forced battle, and it is not finished, and Sora is dead, and it's in the Continue mode, and if Retry Locking ain't active;
                if (_bttlByte == 0x02 && _menuPoint != 0x00 && _cutsByte == 0x00 && !RETRY_LOCK)
                {
                    while (_menuCheck == 0xEFAC || _menuCheck == 0xCAFE)
                    _menuCheck = Hypervisor.Read<ushort>(Variables.ADDR_MenuCount + 0x02);

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

                            Helpers.Log("Death Screen detected! Retry is default! Destroying functions...", 0);

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
                    }

                    RETRY_LOCK = true;
                }

                // If in a cutscene, or if not in a forced battle, or the forced battle is finished, and Sora is not dead, and Retry mode is active;
                else if (((_bttlByte != 0x02 && _menuPoint == 0x00) || _fnshByte == 0x01 || _cutsByte != 0x00) && RETRY_LOCK)
                {
                    if (_fnshByte != 0x01)
                        Helpers.Log("Death Screen is not present! Restoring functions...", 0);

                    else
                        Helpers.Log("End of battle detected! Restoring functions...", 0);

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
            if (RETRY_OFFSET != 0x00 && RETRY_MODE != 0x03)
            {
                if (_menuPoint == 0x00 || _cutsByte != 0x00 || _bttlByte != 0x02) 
                    Hypervisor.WriteArray(SYSBAR_POINTER + RETRY_OFFSET, Strings.RetryPrompt[LANGUAGE][0].ToKHSCII(), true);

                else
                    Hypervisor.WriteArray(SYSBAR_POINTER + RETRY_OFFSET, Strings.RetryPrompt[LANGUAGE][RETRY_MODE].ToKHSCII(), true);
            }
        }

        /*
            Limit Override:

            Fixes the misplacement of Limit Form shortcuts.
            It also overrides Limit Form names if in English.
        */
        public static void LimitOverride()
        {
            var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);
            var _modeRead = Hypervisor.Read<ushort>(Variables.ADDR_ControllerMode);
            var _shortRead = Hypervisor.Read<ushort>(Variables.ADDR_LimitShortcut);

            if (_confirmRead == 0x00 && _shortRead != LIMIT_SHORT[0])
            {
                Hypervisor.Write<short>(Variables.ADDR_LimitShortcut, LIMIT_SHORT[0]);
                Hypervisor.Write<short>(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[3]);
            }

            else if (_confirmRead == 0x01 && _shortRead != LIMIT_SHORT[3] && _modeRead == 0)
            {
                Hypervisor.Write<short>(Variables.ADDR_LimitShortcut, LIMIT_SHORT[3]);
                Hypervisor.Write<short>(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[0]);
            }

            Hypervisor.Write<short>(Variables.ADDR_LimitShortcut + 0x02, LIMIT_SHORT[1]);
            Hypervisor.Write<short>(Variables.ADDR_LimitShortcut + 0x04, LIMIT_SHORT[2]);

            if (LANGUAGE == 0x00)
            {
                var _limitText = Strings.LimitText;

                if (LIMIT_OFFSETS == null)
                {
                    LIMIT_OFFSETS = new List<ulong>();

                    for (int i = 0; i < 6; i++)
                        LIMIT_OFFSETS.Add(Hypervisor.Read<uint>(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[i]) + 0x04), true));

                    var _arsCheck = Hypervisor.ReadArray(SYSBAR_POINTER + LIMIT_OFFSETS[1], 0x0C, true);

                    if (!_arsCheck.SequenceEqual(Strings.LimitText[1].ToKHSCII()))
                    {
                        var _raveOffset = Hypervisor.Read<uint>(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[0x06]) + 0x04), true);
                        var _finishOffset = Hypervisor.Read<uint>(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[0x08]) + 0x04), true);
                        var _descOffset = Hypervisor.Read<uint>(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[0x09]) + 0x04), true);

                        Hypervisor.Write(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[0x06]) + 0x04), _raveOffset + 0x01, true);
                        Hypervisor.Write(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[0x07]) + 0x04), _finishOffset, true);
                        Hypervisor.Write(SYSBAR_POINTER + (SYSBAR_HEADER.FindValue(Strings.LimitIDs[0x09]) + 0x04), _descOffset + 0x02, true);

                        Hypervisor.WriteArray(SYSBAR_POINTER + _raveOffset + 0x01, Strings.LimitText[3].ToKHSCII(), true);

                        for (int i = 0; i < 3; i++)
                        {
                            Hypervisor.WriteArray(SYSBAR_POINTER + LIMIT_OFFSETS[i], Strings.LimitText[i].ToKHSCII(), true);
                            Hypervisor.WriteArray(SYSBAR_POINTER + LIMIT_OFFSETS[i + 0x03], Strings.LimitText[i].ToKHSCII(), true);
                        }
                    }
                }
            }
        }

        /*
            GenerateSave:

            Only to be triggered by AutosaveEngine(), generate and write a save to
            both RAM and ROM portions, effectively saving the game.
        */
        public static void GenerateSave()
        {
            // Prepare the pointers.
            var _pointerBase = Hypervisor.Read<ulong>(Variables.PINT_SaveInformation);
            var _pointerSecond = Hypervisor.Read<ulong>(_pointerBase + 0x10, true);

            // Prepare the strings.
            var _saveName = "BISLPM-66675FM-98";
            var _savePath = Hypervisor.ReadTerminate(_pointerBase + 0x40, true) + "\\KHIIFM.png";

            // Calculate the Unix Date.
            var _currDate = DateTime.Now;
            var _unix = new DateTime(1970, 1, 1);
            var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

            // Prepare the variables for Save Info.
            var _saveSlot = 0;
            var _saveInfoLength = 0x158;
            var _saveDataLength = 0x10FC0;

            var _saveInfoStartRAM = _pointerSecond + 0x168;
            var _saveDataStartRAM = _pointerSecond + 0x19630;

            var _saveInfoStartFILE = 0x1C8;
            var _saveDataStartFILE = 0x19690;

            // Read the save from RAM.
            var _saveData = Hypervisor.ReadArray(Variables.ADDR_SaveData, _saveDataLength);

            // Read the save slot.
            var _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);

            // If the file does not bear a save; terminate the operation.
            if (!Encoding.Default.GetString(_saveSlotRAM).Contains("66675FM"))
            {
                Helpers.Log("File does not bare a save! Autosave aborted to stop corruption!", 1);
                return;
            }

            // Seek out the physical slot of the save to make.
            while (_saveSlotRAM[0] != 0x00 && !Encoding.ASCII.GetString(_saveSlotRAM).Contains("66675FM-98"))
            {
                _saveSlot++;
                _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);
            }

            // Calculate the checksums.
            var _magicArray = _saveData.Take(0x08).ToArray();
            var _dataArray = _saveData.Skip(0x0C).ToArray();

            var _checkMagic = Extensions.SaveCRC32(_magicArray, 8, uint.MaxValue);
            var _checkData = Extensions.SaveCRC32(_dataArray, _dataArray.Length, _checkMagic ^ uint.MaxValue);

            #region RAM Save
            // Fetch the address for the save info.
            var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot);
            var _saveDataAddrRAM = _saveDataStartRAM + (ulong)(_saveDataLength * _saveSlot);

            // Write out the save information.
            Hypervisor.WriteArray(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

            // Write the date in which the save was made.
            Hypervisor.Write(_saveInfoAddrRAM + 0x40, _writeDate, true);
            Hypervisor.Write(_saveInfoAddrRAM + 0x48, _writeDate, true);

            // Write the length of the save.
            Hypervisor.Write(_saveInfoAddrRAM + 0x50, _saveDataLength, true);

            // Write the header.
            Hypervisor.WriteArray(_saveDataAddrRAM, Encoding.ASCII.GetBytes("KH2J"), true);
            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x04, 0x3A, true);

            // Write the checksum.
            Hypervisor.Write(_saveDataAddrRAM + 0x08, _checkData, true);

            // Write, the save.
            Hypervisor.WriteArray(_saveDataAddrRAM + 0x0C, _dataArray, true);
            #endregion

            #region File Save

            // Fetch the address for the save info.
            var _saveInfoAddr = _saveInfoStartFILE + _saveInfoLength * _saveSlot;
            var _saveDataAddr = _saveDataStartFILE + _saveDataLength * _saveSlot;

            // Create the writer.
            using (var _stream = new FileStream(_savePath, FileMode.Open))
            using (var _write = new BinaryWriter(_stream))
            {
                // Write out the save information.
                _stream.Position = _saveInfoAddr;
                _write.Write(Encoding.ASCII.GetBytes(_saveName));

                // The date in which the save was made.
                _stream.Position = _saveInfoAddr + 0x40;
                _write.Write(_writeDate);
                _stream.Position = _saveInfoAddr + 0x48;
                _write.Write(_writeDate);

                // The length of the save.
                _stream.Position = _saveInfoAddr + 0x50;
                _write.Write(_saveDataLength);

                // Write the header.
                _stream.Position = _saveDataAddr;
                _write.Write(Encoding.ASCII.GetBytes("KH2J"));
                _stream.Position = _saveDataAddr + 0x04;
                _write.Write(0x3A);

                // Write the checksum.
                _stream.Position = _saveDataAddr + 0x08;
                _write.Write(_checkData);

                // Write, the save.
                _stream.Position = _saveDataAddr + 0x0C;
                _write.Write(_dataArray);
            }
            #endregion

            // Play a sound, dictating that the save was a success!
            if (Variables.sfxToggle)
                Helpers.PlaySFX(Variables.SaveSFXPath);
        }

        /*
            AutosaveEngine:

            As the name suggests, handle the logic behind Autosave functionality.
        */
        public static void AutosaveEngine()
        {
            var _toggleCheck = Hypervisor.Read<ushort>(Variables.ADDR_Config) & 0x01;
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);

            var _pauseCheck = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);

            if (!CheckTitle() && _loadRead == 0x01)
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

                        GenerateSave();
                        SAVE_ITERATOR = 0;
                    }

                    else if (SAVE_ROOM != _roomCheck && _worldCheck >= 2)
                    {
                        SAVE_ITERATOR++;

                        if (SAVE_ITERATOR == 3)
                        {
                            Helpers.Log("Room condition met! Writing Autosave...", 0);

                            GenerateSave();
                            SAVE_ITERATOR = 0;
                        }
                    }

                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;
                }
            }
        }

        /*
            ObjentrySwap:

            Only to be triggered by HolidayEngine(), generate and write the
            suffixes to Sora's and the Party's models, swaping them accordingly.
        */
        public static void ObjentrySwap(string Input)
        {
            for (ulong _formIterator = 0; _formIterator < (ulong)Variables.SORAObjentry.Length; _formIterator++)
            {
                var _formatForm = String.Format(Variables.SORAObjentry[_formIterator], Input);
                var _convertForm = Encoding.ASCII.GetBytes(_formatForm);

                if (_convertForm.Length < 16)
                {
                    var _byteList = new List<byte>();
                    _byteList.AddRange(_convertForm);
                    _byteList.AddRange(new byte[16 - _convertForm.Length]);

                    _convertForm = _byteList.ToArray();
                }

                Hypervisor.WriteArray(Variables.ADDR_Objentry[0] + 0x60 * _formIterator, _convertForm);
            }

            var _limitFormat = String.Format("P_EX100{0}_KH1F", Input);
            var _limitConvert = Encoding.ASCII.GetBytes(_limitFormat);

            Hypervisor.WriteArray(Variables.ADDR_Objentry[2], _limitConvert);

            for (ulong _friendIterator = 0; _friendIterator < (ulong)Variables.FRIENDObjentry.Length; _friendIterator++)
            {
                var _formatFriend = String.Format(Variables.FRIENDObjentry[_friendIterator], Input);
                var _convertFriend = Encoding.ASCII.GetBytes(_formatFriend);

                if (_convertFriend.Length < 16)
                {
                    var _byteList = new List<byte>();
                    _byteList.AddRange(_convertFriend);
                    _byteList.AddRange(new byte[16 - _convertFriend.Length]);

                    _convertFriend = _byteList.ToArray();
                }

                Hypervisor.WriteArray(Variables.ADDR_Objentry[1] + 0x60 * _friendIterator, _convertFriend);
            }
        }

        /* 
            HolidayEngine:

            Handle switching to the festive outfits on their respective holidays.
        */
        public static void HolidayEngine()
        {
            if (Variables.festiveToggle)
            {
                var _suffixRead = Hypervisor.Read<byte>(Variables.ADDR_Objentry[0] + 0x07);
                var _dateCurrent = DateTime.Now;

                var _dateHalloweenStart = new DateTime(_dateCurrent.Year, 10, 31);
                var _dateHalloweenEnd = new DateTime(_dateCurrent.Year, 11, 03);

                var _dateChristmasStart = new DateTime(_dateCurrent.Year, 12, 24);
                var _dateChristmasEnd = new DateTime(_dateCurrent.Year, 01, 02);

                var _suffixWrite = "";

                if (_dateCurrent > _dateHalloweenStart && _dateCurrent < _dateHalloweenEnd)
                    _suffixWrite = "_NM";

                else if (_dateCurrent > _dateChristmasStart && _dateCurrent > _dateChristmasEnd)
                    _suffixWrite = "_XM";


                if (_suffixWrite != "" && _suffixRead == 0x00)
                    ObjentrySwap(_suffixWrite);

                else if (_suffixWrite == "" && _suffixRead != 0x00)
                    ObjentrySwap(_suffixWrite);
            }
        }

        /*
            MagicHide:

            Hides the MP Bar until Sora learns a spell.
        */
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

        /*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fined.
            To be executed on a separate thread.
        */
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

            if (!CheckTitle())
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

        /*
            Execute:

            Executes the main logic within Re:Fined.
        */
        public static void Execute()
        {
            try
            {
                #region High Priority
                if (!Variables.Initialized)
                    Initialization();

                SkipRoxas();
                ResetGame();
                DriveShortcuts();
                RetryPrompt();
                FixExit();
                #endregion

                #region Mid Priority
                if (Variables.DualAudio)
                    AudioSwap();

                Autoattack();
                SortMagic();
                MusicAdjust();
                AdjustControler();
                FrameOverride();
                #endregion

                #region Low Priority
                MagicHide();
                ClassicSave();
                HolidayEngine();
                LimitOverride();
                #endregion

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

                if (Variables.DCTask == null && Variables.rpcToggle)
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
