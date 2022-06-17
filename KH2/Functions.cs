/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
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
using System.Collections.Generic;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
    public class Functions
    {
        /*
            Variable Space!
          
            Yes, this class has one, too!
        */

		[DllImport("kernel32")]
		static extern bool AllocConsole();

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

        static ulong CONTINUE_OFFSET;
        static ulong RETRY_OFFSET;

        static int RETRY_MODE = 0x00;
        static bool RETRY_LOCK;
        
        static int EXP_READ;
        static byte FORM_READ;
        static byte[] DRIVE_READ;
        static byte[] FORM_STAT_READ;
        static List<byte[]> ITEM_READ;

        /*
            Initialization:

            Serves to initialize stuffs.
        */
        public static void Initialization()
        {
            if (!Directory.Exists(Path.GetTempPath() + "ReFixed"))
                Directory.CreateDirectory(Path.GetTempPath() + "ReFixed");
                
            if (!File.Exists(Variables.SwitchSFXPath))
            {
                var _saveStream = File.Create(Variables.SaveSFXPath);
                var _switchStream = File.Create(Variables.SwitchSFXPath);

                Variables.SaveSFX.CopyTo(_saveStream);
                Variables.SwitchSFX.CopyTo(_switchStream);
            }

            if (Variables.devMode)
            AllocConsole();

            var _configIni = new TinyIni("reFixed.ini");
            Variables.festiveToggle = Convert.ToBoolean(_configIni.Read("festivityEngine", "Kingdom Hearts II"));

            Hypervisor.UnlockBlock(Variables.ADDR_PAXFormatter);
            Hypervisor.UnlockBlock(Variables.ADDR_ANBFormatter);
            Hypervisor.UnlockBlock(Variables.ADDR_BTLFormatter);
            Hypervisor.UnlockBlock(Variables.ADDR_EVTFormatter);

            Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_LimiterINST, true);
            Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_WarpINST, true);
            
            Hypervisor.UnlockBlock(Variables.ADDR_LimitShortcut);

            Variables.Source = new CancellationTokenSource();
            Variables.Token = Variables.Source.Token;

            Variables.Initialized = true;
        }

        /*
            TextAdjust:
        
            Change the text in-game to give the feel that this is an official SE mod
            and not some jank put together by a 20-year-old.
        */
        public static void TextAdjust()
        {
            var _langList = new string[]
            {
                "English",
                "German",
                "Spanish",
                "French",
                "Italian"
            };

            SYSBAR_POINTER = Hypervisor.Read<ulong>(Variables.PINT_SystemBAR);

            var _strSize = Hypervisor.Read<int>(SYSBAR_POINTER - 0x14, true);
            SYSBAR_HEADER = Hypervisor.ReadArray(SYSBAR_POINTER, _strSize, true);

            if (SYSBAR_POINTER != 0x00 && _strSize != 0x00)
            { 
                var _strOffset = Hypervisor.Read<uint>(SYSBAR_POINTER + SYSBAR_HEADER.FindValue<uint>(0x1E0) + 0x04, true);
                var _strRead = Hypervisor.ReadArray(SYSBAR_POINTER + _strOffset, 0x06, true);

                if (LANGUAGE == 0xFF)
                {
                    if (_strRead.SequenceEqual(new byte[] { 0x2E, 0xAD, 0xAD, 0x9A, 0x9C, 0xA4 }))
                        LANGUAGE = 0x00;

                    else if (_strRead.SequenceEqual(new byte[] { 0x2E, 0xA7, 0xA0, 0xAB, 0xA2, 0x9F }))
                        LANGUAGE = 0x01;

                    else if (_strRead.SequenceEqual(new byte[] { 0x2E, 0xAD, 0x9A, 0x9C, 0x9A, 0xAB }))
                        LANGUAGE = 0x02;

                    else if (_strRead.SequenceEqual(new byte[] { 0x2E, 0xAD, 0xAD, 0x9A, 0xAA, 0xAE }))
                        LANGUAGE = 0x03;

                    else if (_strRead.SequenceEqual(new byte[] { 0x2E, 0xAD, 0xAD, 0x9A, 0x9C, 0x9C }))
                        LANGUAGE = 0x04;

                    Console.WriteLine(String.Format("DEBUG: The detected language is \"{0}\"!", _langList[LANGUAGE]));
                }

                #region Roxas Skip Text
                if (CheckTitle())
                {
                    var _roxasText = Strings.RoxasSkip[LANGUAGE];

                    if (ROXAS_OFFSETS == null)
                    {
                        ROXAS_OFFSETS = new List<ulong>();

                        for (int i = 0; i < 9; i++)
                            ROXAS_OFFSETS.Add(SYSBAR_HEADER.FindValue(Strings.RoxasIDs[i]) + 0x04);
                    }

                    var _roxOffsetTitle = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[0], true);
                    var _roxOffsetYES = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[1], true);
                    var _roxOffsetNO = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[2], true);

                    var _roxOffsetDescYES = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[3], true);
                    var _roxOffsetDescNO = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[4], true);

                    var _roxOffsetConfirm = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[5], true);
                    var _roxOffsetFix = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[6], true);

                    var _roxOffsetGameYES = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[7], true);
                    var _roxOffsetGameFIX = Hypervisor.Read<uint>(SYSBAR_POINTER + ROXAS_OFFSETS[8], true);

                    switch (LANGUAGE)
                    {
                        case 0x00:
                            {
                                if (ROXAS_NO_OFFSET == 0x00)
                                    ROXAS_NO_OFFSET = _roxOffsetNO;

                                if (_roxOffsetNO != ROXAS_NO_OFFSET + 0x01)
                                    Hypervisor.Write(SYSBAR_POINTER + ROXAS_OFFSETS[2], (uint)(ROXAS_NO_OFFSET + 0x01), true);

                                break;
                            }

                        case 0x01:
                            {
                                if (_roxOffsetYES != _roxOffsetFix && _roxOffsetGameYES != _roxOffsetGameFIX)
                                {
                                    Hypervisor.Write(SYSBAR_POINTER + ROXAS_OFFSETS[1], _roxOffsetFix, true);
                                    Hypervisor.Write(SYSBAR_POINTER + ROXAS_OFFSETS[2], _roxOffsetFix + 0x03, true);

                                    _roxOffsetYES = _roxOffsetFix;
                                    _roxOffsetNO = _roxOffsetFix + 0x03;
                                }

                                if (ROXAS_TITLE_OFFSET == 0x00)
                                    ROXAS_TITLE_OFFSET = _roxOffsetTitle;

                                if (_roxOffsetDescYES != ROXAS_TITLE_OFFSET + 0x27)
                                {
                                    Hypervisor.Write(SYSBAR_POINTER + ROXAS_OFFSETS[3], (uint)(ROXAS_TITLE_OFFSET + 0x27), true);
                                    _roxOffsetDescYES = (uint)(ROXAS_TITLE_OFFSET + 0x27);
                                }

                                if (_roxOffsetDescNO != ROXAS_TITLE_OFFSET + 0x7A)
                                {
                                    Hypervisor.Write(SYSBAR_POINTER + ROXAS_OFFSETS[4], (uint)(ROXAS_TITLE_OFFSET + 0x7A), true);
                                    _roxOffsetDescNO = (uint)(ROXAS_TITLE_OFFSET + 0x7A);
                                }

                                break;
                            }
                    }

                    if (_roxOffsetGameYES != _roxOffsetGameFIX)
                        Hypervisor.Write(SYSBAR_POINTER + ROXAS_OFFSETS[7], _roxOffsetGameFIX, true);

                    Hypervisor.WriteArray(SYSBAR_POINTER + _roxOffsetTitle, _roxasText[0].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _roxOffsetConfirm, _roxasText[5].ToKHSCII(), true);

                    Hypervisor.WriteArray(SYSBAR_POINTER + _roxOffsetYES, _roxasText[1].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _roxOffsetNO, _roxasText[2].ToKHSCII(), true);

                    Hypervisor.WriteArray(SYSBAR_POINTER + _roxOffsetDescYES, _roxasText[3].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _roxOffsetDescNO, _roxasText[4].ToKHSCII(), true);
                }
                #endregion

                if (SETTING_OFFSETS == null)
                {
                    SETTING_OFFSETS = new List<ulong>();

                    for (int i = 0; i < 9; i++)
                        SETTING_OFFSETS.Add(SYSBAR_HEADER.FindValue(Strings.SettingIDs[i]) + 0x04);
                }

                var _setOffsetTitle = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[0], true);
                var _setOffsetYES = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[1], true);
                var _setOffsetNO = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[2], true);

                var _setOffsetDescYes = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[3], true);
                var _setOffsetDescNo = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[4], true);

                var _setOffsetDesc = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[5], true);
                var _setOffsetBack = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[6], true);

                var _setOffsetCMD = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[7], true);
                var _setOffsetFix = Hypervisor.Read<uint>(SYSBAR_POINTER + SETTING_OFFSETS[8], true);

                #region Dual Audio Text
                if (!CheckTitle() && Variables.DualAudio)
                {
                    var _audioText = Strings.DualAudio[LANGUAGE];

                    if (SET_YESDESC_OFFSET == 0x00)
                        SET_YESDESC_OFFSET = _setOffsetDescYes;

                    if (SET_YESDESC_OFFSET != _setOffsetDesc)
                    {
                        Hypervisor.Write(SYSBAR_POINTER + SETTING_OFFSETS[3], _setOffsetDesc, true);
                        Hypervisor.Write(SYSBAR_POINTER + SETTING_OFFSETS[4], _setOffsetDesc, true);

                        SET_YESDESC_OFFSET = _setOffsetDesc;
                    }

                    if (LANGUAGE == 0x04)
                    {
                        if (_setOffsetYES != _setOffsetBack)
                        {
                            Hypervisor.Write(SYSBAR_POINTER + SETTING_OFFSETS[1], _setOffsetBack, true);
                            _setOffsetYES = _setOffsetBack;
                        }
                    }

                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetTitle, _audioText[0].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetYES, _audioText[1].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetNO, _audioText[2].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetDesc, _audioText[3].ToKHSCII(), true);
                }
                #endregion

                #region Auto-Save Text
                else if (!CheckTitle() && !Variables.DualAudio)
                {
                    var _saveText = Strings.AutoSave[LANGUAGE];

                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetTitle, _saveText[0].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetYES, _saveText[1].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetNO, _saveText[2].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetDescYes, _saveText[3].ToKHSCII(), true);
                    Hypervisor.WriteArray(SYSBAR_POINTER + _setOffsetDescNo, _saveText[4].ToKHSCII(), true);
                }
                #endregion

                if (_setOffsetCMD != _setOffsetFix && !CheckTitle())
                    Hypervisor.Write(SYSBAR_POINTER + SETTING_OFFSETS[7], _setOffsetFix, true);

                /*
                    What is this?
                
                    Well you see, OpenKH optimizes text so that all duplicate text is only present once
                    in the SYS.BAR file, so when I write "ON" to be "JP", every instance of "ON" gets
                    overridden. This is a no go.

                    This is a smart yet a stupid way to solve this very issue.
                */

                #region OpenKH Text Correction
                if (!CheckTitle())
                {
                    var _openKHText = Strings.FixText[LANGUAGE];
                    var _openKHOffset = Hypervisor.Read<uint>(SYSBAR_POINTER + SYSBAR_HEADER.FindValue(Strings.OpenKHID) + 0x04, true);

                    if (_openKHText != null)
                    {
                        if (ON_OFFSET == 0x00)
                        {
                            ON_OFFSET = SYSBAR_HEADER.FindValue(Strings.OnID) + 0x04;
                            OFF_OFFSET = SYSBAR_HEADER.FindValue(Strings.OffID) + 0x04;

                            FULL_OFFSET = SYSBAR_HEADER.FindValue(Strings.FullID) + 0x04;
                            NONE_OFFSET = SYSBAR_HEADER.FindValue(Strings.NoneID) + 0x04;
                        }

                        switch (LANGUAGE)
                        {
                            case 0x00:
                            case 0x01:
                            case 0x04:
                                {
                                    if (OPENKH_OFFSET == 0x00)
                                        OPENKH_OFFSET = _openKHOffset;

                                    if (OPENKH_OFFSET != ON_OFFSET)
                                    {
                                        Hypervisor.Write(SYSBAR_POINTER + ON_OFFSET, _openKHOffset, true);
                                        Hypervisor.Write(SYSBAR_POINTER + OFF_OFFSET, _openKHOffset + 0x03, true);

                                        if (LANGUAGE == 0x01)
                                        {
                                            Hypervisor.Write(SYSBAR_POINTER + FULL_OFFSET, _openKHOffset, true);
                                            Hypervisor.Write(SYSBAR_POINTER + NONE_OFFSET, _openKHOffset + 0x03, true);
                                        }

                                        ON_OFFSET = OPENKH_OFFSET;
                                    }

                                    break;
                                }
                        }

                        Hypervisor.WriteArray(SYSBAR_POINTER + _openKHOffset, _openKHText[0].ToKHSCII(), true);
                        Hypervisor.WriteArray(SYSBAR_POINTER + _openKHOffset + 0x03, _openKHText[1].ToKHSCII(), true);
                    }
                }
                #endregion
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
                Console.WriteLine("DEBUG: Initiating a Soft Reset.");

                Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);

                DEBOUNCE[0] = true;
            }

            else if (_buttRead != 0x090C && DEBOUNCE[0])
                DEBOUNCE[0] = false;
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
                Console.WriteLine("DEBUG: Spell change detected! Resetting sort memory.");

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
                    Console.WriteLine("DEBUG: Roomchange detected! Restoring the Magic Menu.");
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
                        Console.WriteLine("DEBUG: L2 Detected within Magic Menu! Disabling input registry.");

                        // NOP out command selection, so it does not interfere with our input.
                        for (int _ins = 0; _ins < Variables.ADDR_CMDSelectINST.Length; _ins++)
                            Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_CMDSelectINST[_ins], new byte[] { 0x90, 0x90, 0x90 }, true);
                    }

                    else if (!_triggerCheck && _insCheck == 0x90)
                    {
                        Console.WriteLine("DEBUG: L2 has been let go! Enabling input registry.");

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
                        }

                        // Move the magic index.
                        Hypervisor.Write(Variables.ADDR_MagicMenu[2], _magicIndex + (_inputCheck == 0x01 ? -0x01 : 0x01));
                        Hypervisor.Write(Variables.ADDR_MagicMenu[2] + 0x04, _subjectMagic);

                        Console.WriteLine(String.Format("DEBUG: Moving Magic ID \"{0}\" {1} within the menu!", "0x" + _subjectMagic.ToString("X4"), _inputCheck == 0x01 ? "up" : "down"));

                        // Read the entirety of the magic menu, and save it to memory.
                        MAGIC_STORE = Hypervisor.ReadArray(Variables.ADDR_MagicMenu[1], _magicMax * 0x02);
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
            var _skipBool = !SKIP_INITIATED && !SKIP_COMPLETE;

            if (CheckTitle() && !_skipBool)
            {
                Console.WriteLine("DEBUG: Title screen detected! Resetting Roxas Skip!");

                SKIP_INITIATED = false;
                SKIP_COMPLETE = false;

                SKIP_STAGE = 0;

                _skipBool = true;
            }

            var _vibRead = Hypervisor.Read<ushort>(Variables.ADDR_Config) & 0x01;
            var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);

            if (_skipBool && !CheckTitle())
            {
                switch (_vibRead)
                {
                    case 0x01:
                        Console.WriteLine("DEBUG: Disabling Roxas Skip!");
                        SKIP_COMPLETE = true;
                        break;
                    case 0x00:
                        Console.WriteLine("DEBUG: Initiating Roxas Skip!");
                        SKIP_INITIATED = true;
                        SKIP_COMPLETE = false;
                        break;
                }
            }

            if (SKIP_INITIATED && !SKIP_COMPLETE)
            {
                var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
                var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);
                var _eventCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x04);

                if (_worldCheck == 0x02 && _roomCheck == 0x01 && _eventCheck != 0x34 && SKIP_STAGE == 0)
                {
                    Console.WriteLine("DEBUG: Room parameters correct! Initiating Roxas Skip's First Phase...");

                    Hypervisor.Write<uint>(Variables.ADDR_World, 0x322002);
                    Hypervisor.Write<uint>(Variables.ADDR_World + 0x04, 0x01);
                    Hypervisor.Write<uint>(Variables.ADDR_World + 0x08, 0x01);

                    Hypervisor.Write<uint>(0x444832, 0x1FF00001);
                    Hypervisor.Write<uint>(0x444832 + 0x04, 0x00000000);

                    Hypervisor.Write(Variables.ADDR_Config, (ushort)(_vibRead + 0x01));

                    SKIP_STAGE = 1;
                }

                if (_worldCheck == 0x02 && _roomCheck == 0x20 && _eventCheck == 0x9A)
                {
                    if (SKIP_STAGE == 1)
                    {
                        Console.WriteLine("DEBUG: Room parameters correct! Skip was initiated! Initiating Roxas Skip's Second Phase...");

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

                        Console.WriteLine("DEBUG: Skip completed!");

                        SKIP_INITIATED = false;
                        SKIP_COMPLETE = true;
                        SKIP_STAGE = 2;
                    }

                    else
                    {
                        Console.WriteLine("DEBUG: Skip was not initiated! Marking as completed...");

                        SKIP_INITIATED = false;
                        SKIP_COMPLETE = true;
                    }
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
                Hypervisor.WriteArray(_instructionAddress, Variables.INST_FrameLimiter, true);

            // Otherwise, if the framerate is not set to 30FPS, and the limiter is present:
            else if (_framerateRead != 0x00 && _instructionRead != 0x90)
            {
                // NOP the instruction.
                Hypervisor.WriteArray(_instructionAddress, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);

                // Set the current limiter to be off.
                Hypervisor.Write<byte>(Variables.ADDR_Limiter, 0x00);
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
                Console.WriteLine("DEBUG: Dual Audio switching to Japanese...");

                Hypervisor.WriteString(Variables.ADDR_PAXFormatter, "obj/%s.a.jp");
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter + 0x10, "obj/%s.a.jp");

                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, "voice/jp/battle");

                Hypervisor.WriteString(Variables.ADDR_ANBFormatter, "anm/jp/");
                Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, "anm/jp/");

                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, "voice/jp/event");
            }

            else if (_toggleCheck == 0x01 && _stringCheck != "obj/%s.a.%s")
            {
                Console.WriteLine("DEBUG: Dual Audio switching to English...");

                Hypervisor.WriteString(Variables.ADDR_PAXFormatter, "obj/%s.a.%s");
                Hypervisor.WriteString(Variables.ADDR_PAXFormatter + 0x10, "obj/%s.a.us");

                Hypervisor.WriteString(Variables.ADDR_BTLFormatter, "voice/us/battle");

                Hypervisor.WriteString(Variables.ADDR_ANBFormatter, "anm/us/");
                Hypervisor.WriteString(Variables.ADDR_ANBFormatter + 0x08, "anm/fm/");

                Hypervisor.WriteString(Variables.ADDR_EVTFormatter, "voice/us/event");
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

            var _continueID = Strings.ContinueID;
            var _nullArray = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 };

            /*
                Okay, so I wasted a long ass time trying to make this work. But I figure I got it down to a fine art.
               
                This fuınction messes with in-game functions and the save state, dangerous, but it works very well.
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
                Console.WriteLine("DEBUG: Title Screen detected on Retry Mode! Restoring...");

                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                EXP_READ = -1;
                FORM_READ = 0;
                DRIVE_READ = null;
                FORM_STAT_READ = null;
                ITEM_READ = new List<byte[]>();

                RETRY_LOCK = false;
            }

            if (DRIVE_READ == null && _bttlByte == 0x02 && _cutsByte == 0x00 && _pausRead == 0x00)
            {
                Console.WriteLine(String.Format("DEBUG: Start of forced fight, reading necessary values."));

                ITEM_READ = new List<byte[]>();

                for (int i = 0; i < 4; i++)
                    ITEM_READ.Add(Hypervisor.ReadArray(Variables.ADDR_ItemStart + (ulong)(0x114 * i), 0x10));

                EXP_READ = Hypervisor.Read<int>(Variables.ADDR_EXPStart);
                FORM_READ = Hypervisor.Read<byte>(Variables.ADDR_SoraForm);
                DRIVE_READ = Hypervisor.ReadArray(Variables.ADDR_DriveStart, 0x0C);
                FORM_STAT_READ = Hypervisor.ReadArray(Variables.ADDR_FormStart, 0x38 * 0x0A);

                if (DRIVE_READ[2] == 0x00)
                    DRIVE_READ = null;
            }

            else if ((_bttlByte != 0x02 || _cutsByte != 0x00) && _pausRead == 0x00 && DRIVE_READ != null)
            {
                Console.WriteLine(String.Format("DEBUG: Out of battle. Erasing value memory."));

                EXP_READ = -1;
                FORM_READ = 0;
                DRIVE_READ = null;
                FORM_STAT_READ = null;
                ITEM_READ = new List<byte[]>();
            }

            // This code blob is responsible for switching between Retry and Continue
            // and only runs if Sora is dead and some sort of a menu is present.
            if (_menuPoint != 0x00 && _bttlByte == 0x02)
            {
                if (_menuRead == 0x01 && _warpRead == 0x90 && RETRY_MODE == 0x01)
                {
                    Console.WriteLine("DEBUG: User is going to load the game! Reverting warp functionality...");

                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                    RETRY_LOCK = true;
                }

                else if (_menuRead == 0x00 && _optionRead == 0x02 && _warpRead != 0x90 && RETRY_MODE == 0x01)
                {
                    Console.WriteLine("DEBUG: User defected from loading! Destroying warp functionality...");

                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, _nullArray, true);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, _nullArray, true);

                    RETRY_LOCK = true;
                }

                // If D-Pad sides are pressed;
                if (((_buttRead & 0x2000) == 0x2000 || (_buttRead & 0x8000) == 0x8000) && !DEBOUNCE[2] && _menuRead == 0x00)
                {
                    // Play the sound so that it seems **authentic**.
                    Helpers.PlaySFX(Variables.SwitchSFXPath);

                    Console.WriteLine(String.Format("DEBUG: Switching to \"{0}\" mode.", RETRY_MODE == 0x00 ? "Retry" : "Continue"));

                    // Retry Mode Switch!
                    RETRY_MODE = RETRY_MODE == 0x00 ? 0x01 : 0x00;

                    // This handles the "Continue" mode and restores the function to do room switching.
                    if (RETRY_MODE == 0x00 && _warpRead == 0x90)
                    {
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);
                    }

                    // This handles the "Retry" mode and destroys the function to do room switching.
                    else if (RETRY_MODE == 0x01 && _warpRead != 0x90)
                    {
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

            // If in a forced battle, and it is not finished, and Sora is dead, and it's in the Continue mode, and if Retry Locking ain't active;
            if (_bttlByte == 0x02 && _menuPoint != 0x00 && _cutsByte == 0x00 && !RETRY_LOCK)
            {
                // Retry Mode active.
                RETRY_MODE = 0x01;

                Console.WriteLine("DEBUG: Sora is dead. Destroying warp functionality.");

                // Destroy the functions responsible for switching rooms and reverting story flags.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, _nullArray, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, _nullArray, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, _nullArray, true);

                // This must be written at death instead of at retry.
                // Do not ask me why.
                Hypervisor.Write(Variables.ADDR_SoraForm, FORM_READ);

                RETRY_LOCK = true;
            }

            // If in a cutscene, or if not in a forced battle, or the forced battle is finished, and Sora is not dead, and Retry mode is active;
            else if (((_bttlByte != 0x02 && _menuPoint == 0x00) || _fnshByte == 0x01 || _cutsByte != 0x00) && RETRY_LOCK)
            {
                if (_fnshByte != 0x01)
                    Console.WriteLine("DEBUG: Sora is alive. Restoring warp functionality.");

                else
                    Console.WriteLine("DEBUG: End of battle detected! Restoring warp functionality.");

                // Restore the functions responsible for switching rooms and reverting story flags.
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_WarpINST, Variables.INST_RoomWarp, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_RevertINST, Variables.INST_FlagRevert, true);
                Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_InventoryINST, Variables.INST_InvRevert, true);

                if (RETRY_MODE == 0x01 && _cutsByte == 0x00 && DRIVE_READ != null)
                {
                    while (_pausRead == 0x01)
                        _pausRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);

                    for (int i = 0; i < 4; i++)
                        Hypervisor.WriteArray(Variables.ADDR_ItemStart + (ulong)(0x114 * i), ITEM_READ[i]);
                        
                    Hypervisor.Write(Variables.ADDR_EXPStart, EXP_READ);
                    Hypervisor.WriteArray(Variables.ADDR_DriveStart, DRIVE_READ);
                    Hypervisor.WriteArray(Variables.ADDR_FormStart, FORM_STAT_READ);
                }

                RETRY_LOCK = false;
            }

            // If the retry text offset is set, write the text necessary according to the mode.
            if (RETRY_OFFSET != 0x00)
            {
                if (_menuPoint == 0x00 || _cutsByte != 0x00)
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
            var _shortRead = Hypervisor.Read<ushort>(Variables.ADDR_LimitShortcut);

            if (_confirmRead == 0x00 && _shortRead != 0x02BA)
            {
                Hypervisor.Write<ushort>(Variables.ADDR_LimitShortcut, 0x02BA);
                Hypervisor.Write<ushort>(Variables.ADDR_LimitShortcut + 0x06, 0x02AB);
            }

            else if (_confirmRead == 0x01 && _shortRead != 0x02AB)
            {
                Hypervisor.Write<ushort>(Variables.ADDR_LimitShortcut, 0x02AB);
                Hypervisor.Write<ushort>(Variables.ADDR_LimitShortcut + 0x06, 0x02BA);
            }

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
                return;

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

            if (!CheckTitle() && _loadRead == 0x01)
            {
                Thread.Sleep(100);

                var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
                var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

                var _saveConfig = Variables.DualAudio && Variables.saveToggle;
                var _saveableBool = (_saveConfig ? _saveConfig : _toggleCheck == 0x01) && _battleRead == 0x00 && _loadRead == 0x01 && _cutsceneRead == 0x00;

                if (_saveableBool)
                {
                    if (SAVE_WORLD != _worldCheck)
                    {
                        Console.WriteLine("DEBUG: World condition met! Writing Autosave...");

                        GenerateSave();
                        SAVE_ITERATOR = 0;
                    }

                    else if (SAVE_ROOM != _roomCheck && _worldCheck >= 2)
                    {
                        SAVE_ITERATOR++;

                        if (SAVE_ITERATOR == 3)
                        {
                            Console.WriteLine("DEBUG: Room condition met! Writing Autosave...");

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
                    for (int i = 0; i < Variables.ADDR_MPSEQD.Length; i++)
                        Hypervisor.Write<byte>(Variables.ADDR_MPSEQD[i], 0x00);
                }
            }

            else if (Hypervisor.Read<byte>(Variables.ADDR_MPSEQD[0]) == 0x00 || Hypervisor.Read<byte>(Variables.ADDR_SoraForm) == 0x03)
            {
                for (int i = 0; i < Variables.ADDR_MPSEQD.Length; i++)
                    Hypervisor.Write<byte>(Variables.ADDR_MPSEQD[i], Variables.VALUE_MPSEQD[i]);
            }
        }

        /*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fixed.
            To be executed on a separate thread.
        */
        public static void DiscordEngine()
        {
            var _healthValue = Hypervisor.Read<byte>(Variables.ADDR_SoraHP);
            var _magicValue = Hypervisor.Read<byte>(Variables.ADDR_SoraHP + 0x180);

            var _levelValue = Hypervisor.Read<byte>(0x00445061);
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

            var _rpcButtons = new DiscordRPC.Button[]
            {
                new DiscordRPC.Button
                {
                    Label = "== Powered by Re:Fixed ==",
                    Url = "https://github.com/TopazTK/KH-ReFixed"
                },

                new DiscordRPC.Button
                {
                    Label = "== Icons by Televo ==",
                    Url = "https://github.com/Televo/kingdom-hearts-recollection"
                }
            };

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
                        Buttons = _rpcButtons
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
                        Buttons = _rpcButtons
                    }
                );
            }
        }

        /*
            Execute:

            Executes the main logic within Re:Fixed.
        */
        public static void Execute()
        {
            #region High Priority
            if (!Variables.Initialized)
                Initialization();

            SkipRoxas();
            ResetGame();
            RetryPrompt();
            #endregion

            #region Mid Priority
            if (Variables.DualAudio)
                AudioSwap();

            SortMagic();
            TextAdjust();
            FrameOverride();
            #endregion

            #region Low Priority
            MagicHide();
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
    }
}
