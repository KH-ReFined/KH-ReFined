using ReFined.Common;
using ReFined.KH2.Information;
using ReFined.KH2.Menus;
using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;

namespace ReFined.KH2.Functions
{
    internal class Critical
    {
        public static IntPtr OffsetCampMenu;

        static Variables.CONFIG_BITWISE CONFIG_BIT;
        static bool CONFIG_WRITTEN;

        static bool[] DEBOUNCE = new bool[0x20];

        static ushort SAVE_ROOM;
        static ushort SAVE_WORLD;
        static byte SAVE_ITERATOR;

        static bool LOADED_SETTINGS;

        static int POSITIVE_OFFSET = 0x55;
        static int NEGATIVE_OFFSET = -0x55;

        static List<byte>? SETTINGS_READ;
        static byte[]? SETTINGS_WRITE;

        static bool STATE_COPIED;
        static byte HADES_COUNT;
        static ushort ROXAS_KEYBLADE;
        static byte RETRY_MODE;
        static byte PREPARE_MODE;
        static bool RETRY_BLOCK;

        public static void HandleAutosave()
        {
            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _pauseCheck = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);

            var _blacklistCheck =
                (_worldCheck == 0x08 && _roomCheck == 0x03) ||
                (_worldCheck == 0x0C && _roomCheck == 0x02) ||
                (_worldCheck == 0x12 && _roomCheck >= 0x13 && _roomCheck <= 0x1D) ||
                (_worldCheck == 0x02 && _roomCheck <= 0x01);

            if (!Variables.IS_TITLE && _loadRead == 0x01 && !_blacklistCheck)
            {
                Thread.Sleep(250);

                var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
                var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);


                var _saveableBool = (Variables.SAVE_MODE != 0x02) && 
                                    _battleRead == 0x00 && 
                                    _loadRead == 0x01 && 
                                    _cutsceneRead == 0x00 && 
                                    _worldCheck >= 0x02 && 
                                    _pauseCheck == 0x00;

                if (_saveableBool)
                {
                    if (SAVE_WORLD != _worldCheck && _worldCheck != 0x07)
                    {
                        Terminal.Log("Attempting to Autosave.", 0);

                        Generators.GenerateSave();
                        SAVE_ITERATOR = 0;
                    }

                    // If the room has changed, and the world is valid:
                    else if (SAVE_ROOM != _roomCheck && _worldCheck >= 2)
                    {
                        SAVE_ITERATOR++;

                        if (SAVE_ITERATOR == 3)
                        {
                            Terminal.Log("Attempting to Autosave.", 0);

                            Generators.GenerateSave();
                            SAVE_ITERATOR = 0;
                        }
                    }

                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;
                }
            }
        }

        public static void HandleIntro()
        {
            if (Variables.IS_TITLE)
            {
                var _readState = Hypervisor.ReadArray(Variables.ADDR_NewGame, 0x20);

                var _vibration = _readState[0x04] == 0x00 ? Variables.CONFIG_BITWISE.VIBRATION : Variables.CONFIG_BITWISE.OFF;

                var _autoSave = _readState[0x08] == 0x00 ? Variables.CONFIG_BITWISE.AUTOSAVE_INDICATOR :
                               (_readState[0x08] == 0x01 ? Variables.CONFIG_BITWISE.AUTOSAVE_SILENT : Variables.CONFIG_BITWISE.OFF);

                var _controlPrompt = _readState[0x18] == 0x00 ? Variables.CONFIG_BITWISE.PROMPT_CONTROLLER : Variables.CONFIG_BITWISE.OFF;

                CONFIG_BIT = CONFIG_BIT | Variables.CONFIG_BITWISE.SUMMON_FULL | Variables.CONFIG_BITWISE.NAVI_MAP | _vibration | _autoSave | _controlPrompt;
            }

            if (!Variables.IS_TITLE && !CONFIG_WRITTEN)
            {
                var _areaRead = Hypervisor.Read<uint>(Variables.ADDR_Area);

                if (_areaRead == 0x0102 || (_areaRead == 0x2002))
                {
                    Terminal.Log("Intro Handler detected a new game! Writing the configuration.", 0);

                    var _configBitwise = (ushort)CONFIG_BIT;
                    Hypervisor.Write(Variables.ADDR_Config, _configBitwise);

                    Variables.SAVE_MODE = _configBitwise.GetBitwise(0x0004, 0x0002, 0xCAFE);
                    Variables.CONTROLLER_MODE = _configBitwise.GetBitwise(0xCAFE, 0x2000);

                    CONFIG_WRITTEN = true;
                }

                else
                {
                    Terminal.Log("Intro Handler detected a game load! Skipping config writing..", 0);
                    CONFIG_WRITTEN = true;
                }
            }
        }

        public static void HandleConfig()
        {
            var _configRead = Hypervisor.Read<ushort>(Variables.ADDR_Config);
            var _selectPoint = Hypervisor.Read<ulong>(Variables.PINT_SubMenuOptionSelect);

            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
            var _pauseRead = Hypervisor.Read<byte>(Variables.ADDR_PauseFlag);
            var _menuRead = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);

            var _settingsPoint = Hypervisor.Read<ulong>(Variables.PINT_ConfigMenu);
            var _difficultyRead = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x2498);

            if (!Variables.IS_TITLE && !LOADED_SETTINGS)
            {
                Terminal.Log("Fetching the config from the save file.", 0);

                Variables.CONTROLLER_MODE = _configRead.GetBitwise(0xCAFE, 0x2000);
                Variables.SAVE_MODE = _configRead.GetBitwise(0x0004, 0x0002, 0xCAFE);

                Terminal.Log("Fetch successful!", 0);

                LOADED_SETTINGS = true;
            }

            else if (Variables.IS_TITLE && LOADED_SETTINGS)
                LOADED_SETTINGS = false;

            if (_menuRead == 0x24 && _pauseRead == 0x01 && _selectPoint != 0x00)
            {
                if (_settingsPoint != 0x00 && !DEBOUNCE[6])
                {
                    Terminal.Log("Config Menu has been opened! Setting up necessary stuff.", 0);

                    var _naviMap = _configRead.GetBitwise(0x0008);
                    var _cameraAuto = _configRead.GetBitwise(0x0010);
                    var _cameraHRev = _configRead.GetBitwise(0x0080);
                    var _cameraVRev = _configRead.GetBitwise(0x0100);
                    var _commandKH2 = _configRead.GetBitwise(0x0040);
                    var _vibrationOn = _configRead.GetBitwise(0x0001);
                    var _summonEffect = _configRead.GetBitwise(0x0200, 0x0400);

                    var _autoSave = _configRead.GetBitwise(0x0004, 0x0002, 0xCAFE);
                    var _promptMode = _configRead.GetBitwise(0x2000, 0xCAFE);

                    SETTINGS_READ = new List<byte>
                    {
                        Convert.ToByte(_cameraAuto),
                        Convert.ToByte(_cameraVRev),
                        Convert.ToByte(_cameraHRev),
                        Convert.ToByte(_summonEffect),
                        Convert.ToByte(_naviMap),
                        Convert.ToByte(_autoSave),
                        Convert.ToByte(_promptMode),
                        Convert.ToByte(_vibrationOn),
                        Convert.ToByte(_commandKH2),
                        _difficultyRead
                    };

                    Hypervisor.WriteArray(_settingsPoint, SETTINGS_READ.ToArray(), true);

                    Variables.SharpHook[0x365A20].Execute();
                    Variables.SharpHook[0x3659E0].Execute();

                    DEBOUNCE[6] = true;
                }

                SETTINGS_WRITE = Hypervisor.ReadArray(_settingsPoint, SETTINGS_READ.Count(), true);

                Variables.SAVE_MODE = SETTINGS_WRITE[0x05];
                Variables.CONTROLLER_MODE = SETTINGS_WRITE[0x06];

                var _fieldCamBit = SETTINGS_WRITE[0x00] == 0x01 ? Variables.CONFIG_BITWISE.FIELD_CAM : Variables.CONFIG_BITWISE.OFF;
                var _cameraVerticalBit = SETTINGS_WRITE[0x01] == 0x01 ? Variables.CONFIG_BITWISE.CAMERA_V : Variables.CONFIG_BITWISE.OFF;
                var _cameraHorizontalBit = SETTINGS_WRITE[0x02] == 0x01 ? Variables.CONFIG_BITWISE.CAMERA_H : Variables.CONFIG_BITWISE.OFF;

                var _summonBit = SETTINGS_WRITE[0x03] == 0x01 ? Variables.CONFIG_BITWISE.SUMMON_PARTIAL :
                                (SETTINGS_WRITE[0x03] == 0x00 ? Variables.CONFIG_BITWISE.SUMMON_FULL : Variables.CONFIG_BITWISE.OFF);

                var _mapBit = SETTINGS_WRITE[0x04] == 0x00 ? Variables.CONFIG_BITWISE.NAVI_MAP : Variables.CONFIG_BITWISE.OFF;

                var _autoSaveBit = SETTINGS_WRITE[0x05] == 0x00 ? Variables.CONFIG_BITWISE.AUTOSAVE_INDICATOR :
                                  (SETTINGS_WRITE[0x05] == 0x01 ? Variables.CONFIG_BITWISE.AUTOSAVE_SILENT : Variables.CONFIG_BITWISE.OFF);

                var _controllerBit = SETTINGS_WRITE[0x06] == 0x00 ? Variables.CONFIG_BITWISE.PROMPT_CONTROLLER : Variables.CONFIG_BITWISE.OFF;

                var _vibrationBit = SETTINGS_WRITE[0x07] == 0x00 ? Variables.CONFIG_BITWISE.VIBRATION : Variables.CONFIG_BITWISE.OFF;
                var _commandBit = SETTINGS_WRITE[0x08] == 0x01 ? Variables.CONFIG_BITWISE.COMMAND_KH2 : Variables.CONFIG_BITWISE.OFF;


                var _configBitwise =
                    _fieldCamBit |
                    _cameraVerticalBit |
                    _cameraHorizontalBit |
                    _summonBit |
                    _mapBit |
                    _autoSaveBit |
                    _controllerBit |
                    _vibrationBit |
                    _commandBit;

                Hypervisor.Write(Variables.ADDR_Config, (ushort)_configBitwise);
            }

            else if (DEBOUNCE[6])
                DEBOUNCE[6] = false;
        }

        public static void HandleRatio()
        {
            ulong _2ldAddr = 0x8A0974;
            ulong _renderAddr = 0x8A098C;
            ulong _viewportAddr = 0x8A099C;

            var _renderWidth = Hypervisor.Read<float>(_renderAddr);
            var _renderHeight = Hypervisor.Read<float>(_renderAddr + 0x04);

            if (_renderWidth != 0x00 && _renderHeight != 0x00)
            {
                var _fetchRatio = (float)Math.Round(_renderWidth / _renderHeight, 2);

                Hypervisor.Write(_viewportAddr, _fetchRatio);

                if (_fetchRatio == 1.6F)
                    Hypervisor.Write(_viewportAddr + 4, 2F);

                Hypervisor.Write(_2ldAddr, 640F / _fetchRatio);

                POSITIVE_OFFSET = 0x0055;
                NEGATIVE_OFFSET = -0x0055;

                switch (_fetchRatio)
                {
                    case 2.37F:
                    case 2.38F:
                    case 2.39F:
                        POSITIVE_OFFSET = 0x00C3;
                        NEGATIVE_OFFSET = -0x00C3;
                        break;
                    case 3.55F:
                    case 3.56F:
                        POSITIVE_OFFSET = 0x01A5;
                        NEGATIVE_OFFSET = -0x01A5;
                        break;
                }

                Hypervisor.Write(0x18B906 + 0x2400, NEGATIVE_OFFSET); // Information Bar
                Hypervisor.Write(0x18B93A + 0x2400, NEGATIVE_OFFSET); // Information Bar

                Hypervisor.Write(0x15C15C + 0x2400, NEGATIVE_OFFSET); // Command Menu

                Hypervisor.Write(0x17F989 + 0x2400, POSITIVE_OFFSET); // Party HUD

                Hypervisor.Write(0x178186 + 0x2400, POSITIVE_OFFSET); // Radar Cursor
                Hypervisor.Write(0x1781C0 + 0x2400, POSITIVE_OFFSET); // Radar Map

                Hypervisor.Write(0x180A4C + 0x2400, POSITIVE_OFFSET); // Enemy HP [Header]
                Hypervisor.Write(0x180A86 + 0x2400, POSITIVE_OFFSET); // Enemy HP ???
                Hypervisor.Write(0x180AB5 + 0x2400, POSITIVE_OFFSET); // Enemy HP [Backdrop]
                Hypervisor.Write(0x180AE4 + 0x2400, POSITIVE_OFFSET); // Enemy HP [Tail]
                Hypervisor.Write(0x180B13 + 0x2400, POSITIVE_OFFSET); // Enemy HP [Damage]
                Hypervisor.Write(0x180B42 + 0x2400, POSITIVE_OFFSET); // Enemy HP [Main Bar]
                Hypervisor.Write(0x180C49 + 0x2400, POSITIVE_OFFSET); // Enemy HP [Extra Bars]

                Hypervisor.Write(0x17CEBB + 0x2400, POSITIVE_OFFSET); // Sora Backdrop
                Hypervisor.Write(0x17CF13 + 0x2400, POSITIVE_OFFSET); // Sora HP [Backdrop]
                Hypervisor.Write(0x17CF5A + 0x2400, POSITIVE_OFFSET); // Sora HP [Complete]
                Hypervisor.Write(0x17CF7F + 0x2400, POSITIVE_OFFSET); // Sora Face
                Hypervisor.Write(0x17CF99 + 0x2400, POSITIVE_OFFSET); // Sora MP [Header]
                Hypervisor.Write(0x17CFC9 + 0x2400, POSITIVE_OFFSET); // Sora MP [Backdrop]
                Hypervisor.Write(0x17CFF9 + 0x2400, POSITIVE_OFFSET); // Sora MP [Bar]
                Hypervisor.Write(0x17D029 + 0x2400, POSITIVE_OFFSET); // Sora ???
                Hypervisor.Write(0x17D1E5 + 0x2400, POSITIVE_OFFSET); // Sora Drive
                Hypervisor.Write(0x17D219 + 0x2400, POSITIVE_OFFSET); // Sora ???

                Hypervisor.Write(0x17E7BF + 0x2400, POSITIVE_OFFSET); // Drive Return
                Hypervisor.Write(0x17F5A9 + 0x2400, POSITIVE_OFFSET); // Drive ???
                Hypervisor.Write(0x17F17D + 0x2400, POSITIVE_OFFSET); // ???
                Hypervisor.Write(0x17E5F8 + 0x2400, POSITIVE_OFFSET); // Drive Transform

                Hypervisor.Write(0x17EA5A + 0x2400, POSITIVE_OFFSET); // Summon Backdrop
                Hypervisor.Write(0x17EAFF + 0x2400, POSITIVE_OFFSET); // Summon Face

                Hypervisor.Write(0x18A156 + 0x2400, POSITIVE_OFFSET); // Mission Gauge - Align Right
                Hypervisor.Write(0x18A532 + 0x2400, NEGATIVE_OFFSET); // Mission Gauge - Align Left

                Hypervisor.Write(0x189666 + 0x2400, POSITIVE_OFFSET); // Mission Count - Align Right
                Hypervisor.Write(0x189357 + 0x2400, NEGATIVE_OFFSET); // Mission Count - Align Left

                Hypervisor.Write(0x18ACD7 + 0x2400, POSITIVE_OFFSET); // All Timers - Align Right
                Hypervisor.Write(0x18AAF6 + 0x2400, NEGATIVE_OFFSET); // All Timers - Align Left

                Hypervisor.Write(0x18C18F + 0x2400, NEGATIVE_OFFSET); // Get Munny
            }
        }

        /*
        public static void HandleRetry()
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

            var _warpRead = Hypervisor.Read<byte>(Variables.ADDR_WarpINST);

            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
            var _eventRead = Hypervisor.Read<ushort>(Variables.ADDR_Area + 0x04);

            var _entRetry = new Continue.Entry()
            {
                Opcode = 0x0002,
                Label = 0x8AB1,
            };

            var _entPrepare = new Continue.Entry()
            {
                Opcode = 0x0002,
                Label = 0x01DE,
            };

            var _nullArray = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 };

            // Check for Hades Escape
            var _isEscape = _worldRead == 0x06 && _roomRead == 0x05 && _eventRead == 0x6F;

            // No-No Areas for Retry.
            var _blacklistCheck =
               (_worldRead == 0x04 && _roomRead >= 0x15 && _roomRead <= 0x1A) // Cavern of Remembrance
            || (_worldRead == 0x06 && _roomRead == 0x09 && _eventRead >= 0xBD && _eventRead <= 0xC4); // Olympus Cups

            if (!_blacklistCheck && !Variables.IS_TITLE)
            {
                // The battle state will also check **specifically** for Hades Escape.
                var _battleState = _battleRead == 0x02 && _cutsceneRead == 0x00;

                // Read the necessary shits at the start of a fight.
                if (_battleState && _pauseRead == 0x00 && !STATE_COPIED)
                {
                    Terminal.Log("Forced fight was detected! Copying the current state...", 0);

                    if (_isEscape)
                    {
                        HADES_COUNT = 0;
                        Terminal.Log("Hades Escape Detected! Adjusting Retry Logic!", 0);
                    }
                    
                    var _currentSave = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x10FC0);
                    Hypervisor.WriteArray(0x7A0000, _currentSave);

                    ROXAS_KEYBLADE = Hypervisor.Read<ushort>(0x445052);
                    STATE_COPIED = true;

                    Terminal.Log("Current state has been copied!", 0);

                    var _insertIndex = Variables.RETRY_DEFAULT ? 0 : 1;

                    Variables.CONTINUE_MENU.Children.Insert(_insertIndex, _entPrepare);
                    Variables.CONTINUE_MENU.Children.Insert(_insertIndex, _entRetry);
                }

                // Flush the memory post-fight.
                else if (!_battleState && _pauseRead == 0x00 && STATE_COPIED && !(_isEscape && _battleRead == 0x01))
                {
                    Terminal.Log("The player is out of battle, resetting the copied state...", 0);
                    Hypervisor.WriteArray(0x7A0000, new byte[0x10FC0]);

                    if (RETRY_MODE != 0x00)
                    {
                        Hypervisor.WriteArray(Variables.ADDR_WarpINST, Variables.INST_RoomWarp);
                        Hypervisor.WriteArray(Variables.ADDR_InventoryINST, Variables.INST_InvRevert);
                        RETRY_MODE = 0x00;
                    }

                    Variables.CONTINUE_MENU = new Continue();

                    STATE_COPIED = false;
                    ROXAS_KEYBLADE = 0x0000;
                }


                // Count the amount of "done" battles in Hades Escape.
                if (_isEscape && _battleRead == 0x01 && !DEBOUNCE[7])
                {
                    HADES_COUNT += 1;
                    DEBOUNCE[7] = true;
                    Terminal.Log("Hades Escape: Incrementing Clear Count!", 0);
                }

                if (_isEscape && _battleRead == 0x02 && DEBOUNCE[7])
                    DEBOUNCE[7] = false;

               
                // If the battle has ended (Death, Kill, Hades Clear, or Cutscene), restore the functions.
                if (_gameOverRead == 0x00 && ((_finishRead == 0x01 || _cutsceneRead != 0x00) && RETRY_MODE != 0x00) || (_isEscape && HADES_COUNT == 3))
                {
                    Terminal.Log("The battle has ended. Restoring functions...", 0);

                    if (_isEscape)
                    {
                        HADES_COUNT = 255;
                        Terminal.Log("Battle ended because of Hades Escape.", 0);
                    }

                    Hypervisor.WriteArray(Variables.ADDR_WarpINST, Variables.INST_RoomWarp);
                    Hypervisor.WriteArray(Variables.ADDR_InventoryINST, Variables.INST_InvRevert);

                    RETRY_MODE = 0x00;
                }

                if (RETRY_BLOCK)
                {
                    Terminal.Log("Out of blacklisted area. Re-activating Retry.", 0);
                    RETRY_BLOCK = false;
                }

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
                    if ((_subMenuRead == 0x04 || _subMenuRead == 0x0C) && _warpRead == 0x90 && RETRY_MODE >= 0x01)
                    {
                        Terminal.Log("Load Menu has been detected. Restoring functions...", 0);

                        Hypervisor.WriteArray(Variables.ADDR_WarpINST, Variables.INST_RoomWarp);
                        Hypervisor.WriteArray(Variables.ADDR_InventoryINST, Variables.INST_InvRevert);

                        RETRY_MODE = 0x00;
                    }

                    // Destroy if the button is Retry.
                    else if (_selectRead == _retryButton && RETRY_MODE != 0x01)
                    {
                        Terminal.Log("Switched to Retry mode! Destroying functions...", 0);

                        Hypervisor.WriteArray(Variables.ADDR_WarpINST, _nullArray);
                        Hypervisor.WriteArray(Variables.ADDR_InventoryINST, [ 0x48, 0x8D, 0x15 ]);

                        Hypervisor.Write(Variables.ADDR_InventoryINST + 0x03, (uint)(_calculateSave - _calculatePointer));

                        RETRY_MODE = 0x01;
                        PREPARE_MODE = 0x00;
                    }

                    // Restore if the button is Continue.
                    else if (_selectRead == _continueButton && RETRY_MODE != 0x00)
                    {
                        Terminal.Log("Switched to Continue mode! Restoring functions...", 0);

                        Hypervisor.WriteArray(Variables.ADDR_WarpINST, Variables.INST_RoomWarp);
                        Hypervisor.WriteArray(Variables.ADDR_InventoryINST, Variables.INST_InvRevert);

                        RETRY_MODE = 0x00;
                        PREPARE_MODE = 0x00;
                    }

                    // Read the beginning save state, then destroy if the button is Prepare.
                    else if (_selectRead == _prepareButton && RETRY_MODE != 0x02)
                    {
                        Terminal.Log("Switched to Prepare Mode! Destroying functions...", 0);

                        var _currentSave = Hypervisor.ReadArray(0x7A0000, 0x10FC0);
                        Hypervisor.WriteArray(Variables.ADDR_SaveData, _currentSave);

                        Hypervisor.WriteArray(Variables.ADDR_WarpINST, _nullArray);
                        Hypervisor.WriteArray(Variables.ADDR_InventoryINST, [ 0x48, 0x8D, 0x15 ]);

                        Hypervisor.Write(Variables.ADDR_InventoryINST + 0x03, (uint)(_calculateSave - _calculatePointer));

                        RETRY_MODE = 0x02;
                        PREPARE_MODE = 0x01;
                    }
                }

                // If the death menu is gone, and Prepare and Retry was executed:
                else
                {
                    // Call the menu.
                    if (PREPARE_MODE == 0x01 && _menuRead != 0x08)
                    {
                        Terminal.Log("Prepare request detected! Opening the Camp Menu...", 0);
                        Variables.SharpHook[OffsetCampMenu].Execute(BSharpConvention.MicrosoftX64, 0, 0);
                    }

                    // Acknowledge the menu was called.
                    else if (_menuRead == 0x08 && PREPARE_MODE == 0x01)
                        PREPARE_MODE = 0x02;

                    else if (PREPARE_MODE == 0x02 && _menuRead != 0x08)
                    {
                        Terminal.Log("Prepare finished! Copying the save state...", 0);
                        var _currentSave = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x10FC0);
                        Hypervisor.WriteArray(0x7A0000, _currentSave);
                        PREPARE_MODE = 0x03;
                    }
                }

            }

            // If in the blacklisted areas: Do not run Retry.
            else if (!RETRY_BLOCK && !Variables.IS_TITLE)
            {
                Terminal.Log("Blacklisted area! Disabling Retry...", 0);

                RETRY_BLOCK = true;
            }
        }
        */
    }
}
