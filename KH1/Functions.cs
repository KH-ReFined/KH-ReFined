/*
==================================================
      KINGDOM HEARTS - RE:FINED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Generic;

using DiscordRPC;

namespace ReFined
{
    public class Functions
    {
        /*
            Variable Space!
          
            Yes, this class has one, too!
        */

        static bool[] DEBOUNCE = new bool[] { false, false, false, false, false };

        static long ABILITY_POINT = -1392;

        static bool SAVE_ONCE;

        static byte SAVE_ROOM;
        static byte SAVE_WORLD;
        static byte SAVE_ITERATOR;

        static byte LANGUAGE = 0xFF;

        static int PHASE_ITERATOR;

        static byte[] SYSBAR_HEADER;
        static byte[] SYSBAR_FILE;
        
        static bool ATTACK_SWITCH;
        static ulong VIBRATION_OFFSET = 0xFFFFFFFFFFFFFFFF;

        /*
            Initialization:

            Serves to initialize stuffs.
        */
        public static void Initialization()
        {
            Helpers.Log("Initializing Re:Fined...", 0);

            if (!Directory.Exists(Path.GetTempPath() + "ReFined"))
                Directory.CreateDirectory(Path.GetTempPath() + "ReFined");
                
            if (!File.Exists(Variables.ToggleSFXPath))
            {
                var _saveStream = File.Create(Variables.SaveSFXPath);
                var _denyStream = File.Create(Variables.DenySFXPath);
                var _toggleStream = File.Create(Variables.ToggleSFXPath);

                Variables.SaveSFX.CopyTo(_saveStream);
                Variables.DenySFX.CopyTo(_denyStream);
                Variables.ToggleSFX.CopyTo(_toggleStream);
            }

            var _configIni = new TinyIni("reFinedLegacy.ini");
            Variables.chestToggle = Convert.ToBoolean(_configIni.Read("battleChests", "Kingdom Hearts"));

            Hypervisor.UnlockBlock(Variables.ADDR_ChestCheck);
            Hypervisor.UnlockBlock(Variables.ADDR_Viewport);

            var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            var _saveDir = Path.Combine(_documentsPath, "Kingdom Hearts/Save Data/");

            EPIC_INIT:
            if (Directory.Exists(_saveDir))
            {
                string[] _epicDirs = Directory.GetDirectories(_saveDir, "*", SearchOption.TopDirectoryOnly);

                if (_epicDirs.Length == 0x00)
                goto EPIC_INIT; 

                foreach (var _str in _epicDirs)
                {
                    var _folderName = new DirectoryInfo(_str).Name;
                    Directory.CreateDirectory(Path.Combine(_documentsPath, "Kingdom Hearts/Configuration/" + _folderName));

                    Helpers.Log("Detected and Created directories for ID: " + _folderName, 0);
                }
            }

            else
                goto EPIC_INIT;

            if (Variables.chestToggle)
                Hypervisor.Write<byte>(Variables.ADDR_ChestCheck, 0x7D);

            Variables.Source = new CancellationTokenSource();
            Variables.Token = Variables.Source.Token;

            Variables.Initialized = true;

            Helpers.Log("Re:Fined initialized with no errors!", 0);
        }

        /*
            Autoattack:

            Allows automatic attacking by holding down the action button. 
            Used primarily for accessibility purposes, tied to "autoAttack" in the config.
        */
        public static void Autoattack()
        {
            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

            var _commandRead = Hypervisor.Read<byte>(0x184F86);
            var _dialogRead = Hypervisor.Read<byte>(0x25F7B82);

            var _buttonSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
            var _inputValue = _inputRead & _buttonSeek;

            var _autoBool = _inputValue == _buttonSeek && _commandRead == 0x00 && _dialogRead != 0x00 && Variables.attackToggle == true;
            var _actionRead = Hypervisor.Read<byte>(Variables.ADDR_ActionFirst);

            if (_autoBool && _actionRead != 0x01)
            {
                Hypervisor.Write(Variables.ADDR_ActionFirst, 0x01);
                Hypervisor.Write(Variables.ADDR_ActionSecond, 0x01);
            }
        }
        /*
            CheckTitle:

            Checks certain points in RAM to see if the player is in the Title Screen.
            Returns **true** if so, returns **false** otherwise. 
        */
        public static bool CheckTitle() => Hypervisor.Read<byte>(Variables.ADDR_World) == 0xFF
            || Hypervisor.Read<byte>(Variables.ADDR_World + 0x68) == 0xFF
            || Hypervisor.Read<byte>(Variables.ADDR_World) == 0x00
            || Hypervisor.Read<byte>(Variables.ADDR_Level) == 0 
            || Hypervisor.Read<byte>(Variables.ADDR_TitleFlag) == 0x01;

        /*
            TextAdjust:

            Change the text in-game to give the feel that this is an official SE mod
            and not some jank put together by a 20-year-old.
        */
        public static void TextAdjust()
        {
            var _menuPointer = Hypervisor.Read<ulong>(Variables.PINT_SystemBAR) - 0x10;

            if (Hypervisor.Read<byte>(_menuPointer, true) == 0x20)
            {
                var _fileSize = (ulong)Hypervisor.Read<int>(_menuPointer + 0x0C, true);
                var _headerSize = (ulong)Hypervisor.Read<int>(_menuPointer + 0x08, true);

                if (LANGUAGE == 0xFF)
                {
                    SYSBAR_HEADER = Hypervisor.ReadArray(_menuPointer + 0x10, (int)_headerSize, true);
                    SYSBAR_FILE = Hypervisor.ReadArray(_menuPointer + _headerSize + 0x10, (int)_fileSize, true);

                    for (byte i = 0; i < 5; i++)
                    {
                        var _yesOffset = SYSBAR_FILE.FindValue(Strings.OptionOG[i].ToKHSCII());

                        if (_yesOffset != 0xFFFFFFFFFFFFFFFF)
                        {
                            var _vibrationFind = SYSBAR_FILE.FindValue(Strings.VibrationOG[i].ToKHSCII());
                            VIBRATION_OFFSET = SYSBAR_HEADER.FindValue((ushort)_vibrationFind) + 0x10;
                            LANGUAGE = i;
                            break;
                        }
                    }
                }

                else
                {
                    var _repText = Strings.TextFOV[LANGUAGE];
                    var _ogText = Strings.VibrationOG[LANGUAGE].ToKHSCII();

                    var _listText = new List<byte>();
                    ulong _addLength = 0x00;

                    foreach (var _e in _repText)
                        _listText.AddRange(_e.ToKHSCII());

                    Hypervisor.WriteArray(_menuPointer + _headerSize + _fileSize + 0x10, _listText.ToArray(), true);

                    for (ulong i = 0; i < 4; i++)
                    {
                        if (i > 0)
                            _addLength += (ulong)_repText[i - 1].Length + 0x01;

                        Hypervisor.Write(_menuPointer + (VIBRATION_OFFSET + 0x02 * i), (ushort)(_fileSize + _addLength), true);
                    }
                }
            }
        }

        public static void AdjustControler()
        {
            if (!Variables.autoController)
            {
                var _contCheck = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, true);

                if (_contCheck != 0x90)
                {
                    Helpers.Log("Manual prompt mode detected! Enforcing prompts...", 0);
                    Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_ControllerINST, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, true);
                }   
                
                Hypervisor.Write<byte>(Variables.ADDR_ControllerMode, (byte)(Variables.contToggle ? 0 : 1));
            }
        }

        /*
            ResetGame:

            Triggers a soft-reset if the proper input is given.
            The input is sought in Execute().

            INPUT: L1 + R1 + START + SELECT.
        */
        public static void ResetGame()
        {            
            var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
			var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

            var _selectRead = Hypervisor.Read<byte>(Variables.ADDR_SaveSelect);
            var _amountRead = Hypervisor.Read<byte>(Variables.ADDR_SaveSelect + 0x044);

            var _buttonRead = _inputRead == 0x0C09;
            var _buttonSeek = (_confirmRead == 0x01 ? 0x2000 : 0x4000);

            var _saveMenuRead = (_selectRead == _amountRead - 0x01) && (_inputRead & _buttonSeek) == _buttonSeek;

            if ((_buttonRead || _saveMenuRead) && !DEBOUNCE[0])
            {
                Helpers.Log("Soft Reset requested!", 0);

                Hypervisor.Write<byte>(Variables.ADDR_Reset[0], 0x01);
                Hypervisor.Write<byte>(Variables.ADDR_Reset[1], 0x01);

                DEBOUNCE[0] = true;
            }

            else
                DEBOUNCE[0] = false;
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
                var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);

                var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
                var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

                var _buttonSeek = (_confirmRead == 0x01 ? 0x2000 : 0x4000);
                var _inputValue = _inputRead & _buttonSeek;

                if (_inputValue == _buttonSeek && _selectButton == 0x03)
                {
                    Helpers.Log("Exit fix initiating...", 0);

                    Thread.Sleep(2500);

                    if (File.Exists("KINGDOM HEARTS HD 1.5+2.5 Launcher.exe"))
                    {
                        Helpers.Log("Redirecting to the launcher...", 0);
                        Process.Start("KINGDOM HEARTS HD 1.5+2.5 Launcher");
                    }
                    
                    Environment.Exit(0);
                }
            }
        }

        /*
            AbilityToggle:

            Allows toggling the selected ability on/off if the proper
            input is given.

            INPUT: Triangle

            NOTE: This is a mess. It works perfectly, no doubt.
            However, FUCK did this take a lot of effort to make.

            So, PLEASE excuse the dirtiness of the code here.
        */
        public static void AbilityToggle()
        {
            var _buttonRead = Hypervisor.Read<ushort>(Variables.ADDR_Input) == 0x1000;

            var _slotRead = Hypervisor.ReadArray(Variables.ADDR_FRIENDAbility - 0x01, 0x04);

            var _menuCheck = Hypervisor.Read<byte>(Variables.ADDR_GameState);
            var _toggleRead = Hypervisor.Read<byte>(Variables.ADDR_AbilityMenu);

            var _menuSelect = Hypervisor.Read<byte>(Variables.ADDR_AbilityMenu + 0x30);
            var _pageSelect = Hypervisor.Read<byte>(Variables.ADDR_AbilityMenu + 0x34);
            var _skillSelect = Hypervisor.Read<byte>(Variables.ADDR_AbilityMenu + 0x40);

            if (_toggleRead == 0x01 && _menuCheck == 0x01)
                Hypervisor.Write<byte>(Variables.ADDR_AbilityMenu, 0x00);

            if (_toggleRead == 0x01 && _menuCheck == 0x00)
            {
                if (_buttonRead && !DEBOUNCE[1])
                {
                    var _pointRead = 0x00;
                    var _abilityOffset =
                        Variables.ADDR_ALLAbility + (ulong)(_skillSelect);

                    if (_menuSelect != 0x04 && _slotRead[_menuSelect] != 0xFF)
                    {
                        ABILITY_POINT = 0x00;
                        _skillSelect = Hypervisor.Read<byte>(Variables.ADDR_AbilityMenu + 0x38);

                        var _currentMember = _menuSelect == 0x00 ? 0x00 : _slotRead[_menuSelect];
                        var _memberOffset = (ulong)(0x74 * _currentMember);
                        var _apOffset = (ulong)(0x100 * _currentMember);

                        var _skillTree = Hypervisor.ReadArray(Variables.ADDR_SORAAbility + _memberOffset, 0x30);
                        byte[] _skillEquip = _skillTree.Where(x => x < 0x80 && x != 0x00).ToArray();

                        for (int i = 0; i < _skillEquip.Length; i++)
                            ABILITY_POINT += Variables.APDictionary[_skillEquip[i]];

                        _pointRead = Hypervisor.Read<byte>(Variables.ADDR_AbilityPoint + _apOffset);
                        _abilityOffset = Variables.ADDR_SORAAbility + _memberOffset + (ulong)(_skillSelect + _pageSelect);
                    }

                    var _readSkill = Hypervisor.Read<byte>(_abilityOffset);
                    var _skillCost =  (_readSkill & 0x80) == 0x80 ? Variables.APDictionary[(byte)(_readSkill - 0x80)] : Variables.APDictionary[_readSkill];

                    if ((_readSkill & 0x80) == 0x80 && ((_pointRead - ABILITY_POINT - _skillCost) >= 0 || ABILITY_POINT == -1392))
                    {
                        Hypervisor.Write(_abilityOffset, (byte)(_readSkill - 0x80));
                        ABILITY_POINT += _skillCost;

                        Helpers.Log(String.Format("Enabling Ability ID: {0} with {1} cost.", _readSkill - 0x80, _skillCost), 0);

                        Helpers.PlaySFX(Variables.ToggleSFXPath);
                    }

                    else if ((_readSkill & 0x80) == 0x00)
                    {
                        Hypervisor.Write(_abilityOffset, (byte)(_readSkill + 0x80));
                        ABILITY_POINT -= _skillCost;

                        Helpers.Log(String.Format("Disabling Ability ID: {0} with {1} cost.", _readSkill - 0x80, _skillCost), 0);

                        Helpers.PlaySFX(Variables.ToggleSFXPath);
                    }

                    else
                    {
                        Helpers.Log(String.Format("Not enough AP to enable Ability ID: {0} with {1} cost.", _readSkill - 0x80, _skillCost), 0);
                        Helpers.PlaySFX(Variables.DenySFXPath);
                    }

                    DEBOUNCE[1] = true;
                }

                if (!_buttonRead && DEBOUNCE[1])
                    DEBOUNCE[1] = false;
            }

            else
            {
                DEBOUNCE[1] = false;
                ABILITY_POINT = -1392;
            }
        }

        /*
            AspectCorrection:

            Automatically detect the aspect ratio in Full Screen
            and accommodate for it.
        */
        public static void AspectCorrection(float InputValue)
        {
            float _floatValue = 9F;

            switch (InputValue)
            {
                case 3.5F:
                    _floatValue = 4.5F;
                    break;
                case 2.3F:
                    _floatValue = 6.75F;
                    break;
                case 1.7F:
                    _floatValue = 9F;
                    break;
                case 1.6F:
                    _floatValue = 10F;
                    break;
                case 1.3F:
                    _floatValue = 12F;
                    break;
            }

            Helpers.Log("Aspect Correction has been applied!", 0);
            Hypervisor.Write(Variables.ADDR_Viewport, _floatValue);
        }

        /*
            GenerateSave:

            Only to be triggered by AutosaveEngine(), generate and write a save to
            both RAM and ROM portions, effectively saving the game.
        */
        public static void GenerateSave()
        {
            if (SAVE_ONCE)
            {
                // Prepare the pointers.
                var _pointerBase = Hypervisor.Read<ulong>(Variables.PINT_SaveInformation);
                var _pointerSecond = Hypervisor.Read<ulong>(_pointerBase + 0x10, true);

                // Prepare the strings.
                var _descName = "-99/system.bin";
                var _saveName = "BISLPS-25198-99";
                var _savePath = Hypervisor.ReadTerminate(_pointerBase + 0x40, true) + "\\KHFM.png";

                // Calculate the Unix Date.
                var _currDate = DateTime.Now;
                var _unix = new DateTime(1970, 1, 1);
                var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

                // Prepare the variables for Save Info.
                var _saveSlot = 0;
                var _saveInfoLength = 0x158;
                var _saveDataLength = 0x16C40;

                var _saveInfoStartRAM = _pointerSecond + 0x10;
                var _saveDataStartRAM = _pointerSecond + 0x10CD0;

                var _saveInfoStartFILE = 0x320;
                var _saveDataStartFILE = 0x10D30;

                // Read the save from RAM.
                var _saveData = Hypervisor.ReadArray(Variables.ADDR_SaveData, _saveDataLength);

                // Read the save slot.
                var _saveSlotRAM = Hypervisor.ReadArray(
                    _saveInfoStartRAM + (ulong)((_saveInfoLength * 2) * _saveSlot),
                    0x11,
                    true
                );

                // If the file does not bear a save; terminate the operation.
                if (!Encoding.Default.GetString(_saveSlotRAM).Contains("25198"))
                    return;

                // Seek out the physical slot of the save to make.
                while (
                    _saveSlotRAM[0] != 0x00
                    && !Encoding.Default.GetString(_saveSlotRAM).Contains("25198-99")
                )
                {
                    _saveSlot++;
                    _saveSlotRAM = Hypervisor.ReadArray(
                        _saveInfoStartRAM + (ulong)((_saveInfoLength * 2) * _saveSlot),
                        0x11,
                        true
                    );
                }

                // Read the necessary values.
                var _timeRead = Hypervisor.Read<uint>(Variables.ADDR_Time);
                var _levelRead = Hypervisor.Read<byte>(Variables.ADDR_Level);
                var _munnyRead = Hypervisor.Read<uint>(Variables.ADDR_Munny);
                var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_World);
                var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_Difficulty);
                var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_World + 0x68);

                #region RAM Save
                // Fetch the address for the save info.
                var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)((_saveInfoLength * 2) * _saveSlot);
                var _saveDataAddrRAM = _saveDataStartRAM + (ulong)((_saveDataLength * 2) * _saveSlot);

                // Write out the save information.
                Hypervisor.WriteArray(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

                // Write the date in which the save was made.
                Hypervisor.Write(_saveInfoAddrRAM + 0x40, _writeDate, true);
                Hypervisor.Write(_saveInfoAddrRAM + 0x48, _writeDate, true);

                // Write the length of the save.
                Hypervisor.Write(_saveInfoAddrRAM + 0x50, _saveDataLength - 0x40, true);

                // Write out the descriptor information.
                Hypervisor.WriteArray(
                    _saveInfoAddrRAM + (ulong)_saveInfoLength,
                    Encoding.Default.GetBytes(_descName),
                    true
                );

                // Write the date in which the descriptor was written.
                Hypervisor.Write(_saveInfoAddrRAM + (ulong)(_saveInfoLength + 0x40), _writeDate, true);
                Hypervisor.Write(_saveInfoAddrRAM + (ulong)(_saveInfoLength + 0x48), _writeDate, true);

                // Write the length of the descriptor.
                // This should be 0x40, but Team Osaka is STUPID so it's 0x400 instead.
                // No, nothing after the 0x40 bit is utilized.
                Hypervisor.Write(_saveInfoAddrRAM + (ulong)(_saveInfoLength + 0x50), 0x400, true);

                // Write, the save.
                Hypervisor.WriteArray(_saveDataAddrRAM, _saveData, true);

                // Write the Magic Values for the descriptor.
                Hypervisor.WriteArray(_saveDataAddrRAM + (ulong)_saveDataLength, Encoding.Default.GetBytes("KHSQ"), true);
                Hypervisor.Write<uint>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x04), 0x1317F14, true);

                // Write the actual descriptor data.
                Hypervisor.Write(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x08), _levelRead, true);
                Hypervisor.Write(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x0C), _munnyRead, true);
                Hypervisor.Write(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x10), _timeRead, true);

                Hypervisor.Write(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x38), _diffRead, true);
                Hypervisor.Write(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x3C), _worldRead, true);
                Hypervisor.Write(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x3E), _roomRead, true);
                #endregion

                #region File Save
                // Fetch the address for the save info.
                var _saveInfoAddr = _saveInfoStartFILE + (_saveInfoLength * 2) * (_saveSlot - 0x01);
                var _saveDataAddr = _saveDataStartFILE + (_saveDataLength * 2) * _saveSlot;

                // Create the writer.
                using (var _stream = new FileStream(_savePath, FileMode.Open))
                using (var _write = new BinaryWriter(_stream))
                {
                    // Write out the save information.
                    _stream.Position = _saveInfoAddr;
                    _write.Write(Encoding.Default.GetBytes(_saveName));

                    // The date in which the save was made.
                    _stream.Position = _saveInfoAddr + 0x40;
                    _write.Write(_writeDate);
                    _stream.Position = _saveInfoAddr + 0x48;
                    _write.Write(_writeDate);

                    // The length of the save.
                    _stream.Position = _saveInfoAddr + 0x50;
                    _write.Write(_saveDataLength - 0x40);

                    // Write out the descriptor information.
                    _stream.Position = _saveInfoAddr + _saveInfoLength;
                    _write.Write(Encoding.Default.GetBytes(_descName));

                    // The date in which the descriptor was written.
                    _stream.Position = _saveInfoAddr + _saveInfoLength + 0x40;
                    _write.Write(_writeDate);
                    _stream.Position = _saveInfoAddr + _saveInfoLength + 0x48;
                    _write.Write(_writeDate);

                    // The length of the descriptor.
                    _stream.Position = _saveInfoAddr + _saveInfoLength + 0x50;
                    _write.Write(0x400);

                    // Write, the save.
                    _stream.Position = _saveDataAddr;
                    _write.Write(_saveData);

                    // Write the Magic Values for the descriptor.
                    _stream.Position = _saveDataAddr + _saveDataLength;
                    _write.Write(Encoding.Default.GetBytes("KHSQ"));
                    _stream.Position = _saveDataAddr + _saveDataLength + 0x04;
                    _write.Write(0x1317F14);

                    // Write the actual descriptor data.
                    _stream.Position = _saveDataAddr + _saveDataLength + 0x08;
                    _write.Write(_levelRead);
                    _stream.Position = _saveDataAddr + _saveDataLength + 0x0C;
                    _write.Write(_munnyRead);
                    _stream.Position = _saveDataAddr + _saveDataLength + 0x10;
                    _write.Write(_timeRead);

                    _stream.Position = _saveDataAddr + _saveDataLength + 0x38;
                    _write.Write(_diffRead);
                    _stream.Position = _saveDataAddr + _saveDataLength + 0x3C;
                    _write.Write(_worldRead);
                    _stream.Position = _saveDataAddr + _saveDataLength + 0x3E;
                    _write.Write(_roomRead);
                }
                #endregion

                // Play a sound, dictating that the save was a success!
                if (Variables.sfxToggle)
                    Helpers.PlaySFX(Variables.SaveSFXPath);
            }

            else
                SAVE_ONCE = true;
        }

        /*
            AutosaveEngine:

            As the name suggests, handle the logic behind Autosave functionality.
        */
        public static void AutosaveEngine()
        {
            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x68);

            var _battleCheck = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
            var _cutsceneCheck = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

            var _menuCheck = Hypervisor.Read<ulong>(Variables.PINT_MenuState);
            var _gummiCheck = Hypervisor.Read<byte>(Variables.ADDR_GummiFlag);

            var _loadCheck = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _blackCheck = Hypervisor.Read<byte>(Variables.ADDR_Blackness);
            var _whiteCheck = Hypervisor.Read<byte>(Variables.ADDR_Whiteness);

            var _isLoading = _loadCheck == 0x01 || _blackCheck != 0x80 || _whiteCheck != 0x00;
            var _isViable = (_battleCheck % 0x02) == 0x00 && _cutsceneCheck == 0x00 && _menuCheck == 0x00 && _gummiCheck == 0x00;

            var _saveableBool = Variables.saveToggle && _isViable && !_isLoading;

            if (_saveableBool && !CheckTitle())
            {
                if (SAVE_WORLD != _worldCheck)
                {
                    Helpers.Log("World condition met! Writing Autosave...", 0);

                    GenerateSave();
                    SAVE_ITERATOR = 0;
                }

                else if (SAVE_ROOM != _roomCheck && _worldCheck >= 1)
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

        /*
            FieldOfView:

            Toggle between the original and a KH2-esque Field of View.
            This is detemined by the **Vibration** option at the Camp Menu.
		*/
        public static void FieldOfView()
        {
            if (Hypervisor.Read<int>(Variables.ADDR_Config) == 0x01)
                for (int i = 0; i < Variables.ADDR_FieldOfView.Length; i++)
                    Hypervisor.Write(Variables.ADDR_FieldOfView[i], Variables.VALUE_EnhancedFOV[i]);

            else
                for (uint i = 0; i < Variables.ADDR_FieldOfView.Length; i++)
                    Hypervisor.Write(Variables.ADDR_FieldOfView[i], Variables.VALUE_DefaultFOV[i]);
        }

        /*
            AudioSwap:

            Enforces English or Japanese Audio depending on player preference.
            This is detemined by the **Vibration** option at the Camp Menu.

            This function is reliant on a patch.
        */
        public static void AudioSwap()
        {
            switch (Hypervisor.Read<int>(Variables.ADDR_Config))
            {
                case 0:
                {
                    if (Hypervisor.Read<byte>(Variables.ADDR_PartyAudioPath) == 0x6A)
                    {
                        Hypervisor.WriteArray(Variables.ADDR_PartyAudioPath, new byte[] { 0x76, 0x6F, 0x69, 0x63, 0x65, 0x2F });
                        Hypervisor.WriteArray(Variables.ADDR_EventAudioPath, new byte[] { 0x76, 0x6F, 0x69, 0x63, 0x65, 0x2F });
                    }
                    break;
                }

                case 1:
                {
                    if (Hypervisor.Read<byte>(Variables.ADDR_PartyAudioPath) == 0x76)
                    {
                        Hypervisor.WriteArray(Variables.ADDR_PartyAudioPath, new byte[] { 0x6A, 0x61, 0x70, 0x61, 0x6E, 0x2F });
                        Hypervisor.WriteArray(Variables.ADDR_EventAudioPath, new byte[] { 0x6A, 0x61, 0x70, 0x61, 0x6E, 0x2F });
                    }
                    break;
                }
            }
        }

        /*
            MagicHide:

            Hides the MP Bar until Sora learns a spell.
        */
        public static void MagicHide()
        {
            var _catchMagic = Hypervisor.Read<byte>(Variables.ADDR_Magic);

            switch (_catchMagic)
            {
                case 0:
                    Hypervisor.Write<byte>(Variables.ADDR_SoraMP, 0x00);
                    break;
                default:
                    if (Hypervisor.Read<byte>(Variables.ADDR_SoraMP) == 0x00)
                        Hypervisor.Write<byte>(Variables.ADDR_SoraMP, 0x01);
                    break;
            }
        }

        /*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fined.
            To be executed on a separate thread.
        */
        public static void DiscordEngine()
        {
            var _stringState = string.Format(
                "Level {0} | {1} Mode",
                Hypervisor.Read<byte>(Variables.ADDR_Level),
                Variables.MDEDictionary.ElementAtOrDefault(Hypervisor.Read<byte>(Variables.ADDR_Difficulty))
            );

            var _timeValue = Math.Floor(Hypervisor.Read<int>(Variables.ADDR_Time) / 60F);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            var _timeText = string.Format(
                "In-Game Time: {0}",
                string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00"))
            );

            var _stringDetail = string.Format("HP: {0} | MP: {1}", Hypervisor.Read<byte>(Variables.ADDR_SoraStats), Hypervisor.Read<byte>(Variables.ADDR_SoraStats + 0x08));

            if (!CheckTitle())
            {
                Variables.DiscordClient.SetPresence(
                    new RichPresence
                    {
                        Details = _stringDetail,
                        State = _stringState,
                        Assets = new Assets
                        {
                            LargeImageKey =
                                Hypervisor.Read<byte>(Variables.ADDR_GummiFlag) == 0
                                    ? Variables.WRLDictionary.ElementAtOrDefault(Hypervisor.Read<byte>(Variables.ADDR_World))
                                    : "wm",
                            LargeImageText = _timeText,
                            SmallImageKey = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag) % 2 == 0 ? "safe" : "battle",
                            SmallImageText = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag) % 2 == 0 ? "Safe" : "In Battle"
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

                ResetGame();
                FixExit();
                #endregion

                #region Mid Priority
                MagicHide();
                Autoattack();
                TextAdjust();
                AdjustControler();
                FieldOfView();
                AbilityToggle();
                #endregion

                #region Tasks

                if (Variables.chestToggle)
                {
                    Hypervisor.UnlockBlock(Variables.ADDR_ChestCheck);
                    Hypervisor.Write<byte>(Variables.ADDR_ChestCheck, 0x7D);
                }

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
