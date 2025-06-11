using KH2FML;

using System.Text;
using System.Reflection;

using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;
using System.Net.Http.Headers;
using System.Runtime.CompilerServices;
using System.ComponentModel.Design;

namespace ReFined
{
    /// <summary>
    /// These functions right here made me hate my life.
    /// </summary>
    public static class Critical
    {
        // Debounce Array Mapping:
        // 0x00 => Magic Sorting Check (SortMagic)
        // 0x01 => Locking Autosave (HandleAutosave)
        // 0x02 => Allowing Save Handling (HandleAutosave)
        // 0x03 => Applied Settings (HandleConfiguration)
        // 0x04 => Settings Parsed (HandleConfiguration)
        // 0x05 => First Initialization Settings (HandleConfiguration)
        // 0x06 => Save Bounce Back (HandleAutosave)
        // 0x07 => Intro Settings Written (HandleIntro)
        // 0x08 => Seek Settings for Intro (HandleIntro)
        // 0x09 => Prologue Skip Enable Flag (PrologueSkip)
        // 0x0A => Autosave-On-Load Prevention (HandleAutosave)
        // 0x0B => Magic Changes Applied (SortMagic)
        // 0x0C => Retry Blacklist (RetryBattles)
        // 0x0D => Retribution and Absolution Switch (RetributionLogic)
        // 0x0E => Magic Sort Disabler
        // 0x0F => Autosave Load-Save Preventer
        // 0x10 => Kill Sora Debounce
        // 0x11 => Shake Debounce
        // 0x12 => Autosave Initialization
        static bool[] DEBOUNCE = new bool[0x20];

        static ushort PAST_FORM;
        static int PAST_CROWN;
        static ulong SORA_PORTRAIT;

        public static ulong CONTROL_OFFSET;
        static byte[][] CONTROL_INSTRUCTIONS;

        public static ulong ICON_OFFSET;
        public static ulong LIST_OFFSET;
        public static ulong EQUIP_OFFSET;
        public static ulong CATEGORY_OFFSET;
        public static ulong INFORMATION_OFFSET;

        static byte SAVE_ROOM;
        static byte SAVE_WORLD;
        static byte SAVE_ITERATOR;
        static byte SAVE_CHECK_OFFSET;

        static uint MAGIC_LV1;
        static ushort MAGIC_LV2;

        static uint MAGIC_LV1_RANDO;
        static ushort MAGIC_LV2_RANDO;

        static Dictionary<ulong, ulong> MAGIC_REGISTRY = new Dictionary<ulong, ulong>();

        static byte[] MAGIC_STORE;
        static uint[] MAGIC_OFFSET = [0x1B1, 0x295, 0x2BD, 0x30C, 0x33C];

        static int ADDON_OFFSET;
        public static bool AUDIO_ACTIVE;
        public static bool SUB_AUDIO_ACTIVE;

        public static bool MUSIC_ACTIVE;
        public static bool ENEMY_ACTIVE;

        static byte SUB_AUDIO_MEMORY;
        static Locals.CONFIG SETTING_MEMORY;

        static int RETRY_MODE;
        static bool RETRY_WAIT;
        static byte[] RETRY_STATE;

        static byte ROXAS_SKIP_STAGE;

        static bool IS_ESCAPING;
        static bool HADES_CHANGE;
        static byte HADES_STATE = 0xFF;

        static bool DELAY_ASPECT;
        static DateTime DELAY_START;

        static int POSITIVE_OFFSET = 0x55;
        static int NEGATIVE_OFFSET = -0x55;

        static int HEIGHT_OFFSET = 0x00;
        static int COMMAND_DROP_Y = 0x00;

        static int INDEX_ABSOLUTION = 0xFF;
        static int INDEX_RETRIBUTION = 0xFF;

        static ushort PARAM_ABSOLUTION = 0xFFFF;
        static ushort PARAM_RETRIBUTION = 0xFFFF;

        static ulong RETRIBUTION_RAM;
        static ulong ABSOLUTION_RAM;

        static ushort[] ABILITY_ARRAY;

        static bool RETRIBUTION_INIT = false;

        public static List<ushort> PARAMS_ALL = new List<ushort>();
        static List<ushort> PARAMS_LIST = new List<ushort>();

        public static byte[] INST_CONTINUELOAD;
        public static byte[] INST_MAPJUMPTASK;

        public static ulong ASPECT_OFFSET;
        public static ulong AREAINIT_OFFSET;
        public static ulong CAMPINIT_OFFSET;
        public static ulong MENUSELECT_OFFSET;
        public static ulong MAGICCLEAR_OFFSET;

        public static ulong ADJUSTGLOW_OFFSET;
        public static ulong INITVIEWPORT_OFFSET;
        public static ulong ADJUSTVIEWPORT_OFFSET;

        public static byte[] CAMPINIT_FUNCTION;

        public static IntPtr FUNC_MAGICGETTABLE;
        public static IntPtr FUNC_ITEMSELECTUPDATE;
        public static IntPtr FUNC_CONFIGUPDATELIST;
        public static IntPtr FUNC_CONFIGUPDATEACTIVE;

        static List<byte[]> VIEWPORT_FUNCTIONS = new List<byte[]>();

        static Continue.Entry RETRY_ENTRY = new Continue.Entry()
        {
            Opcode = 0x0002,
            Label = 0x8AB1,
        };

        static Continue.Entry PREPARE_ENTRY = new Continue.Entry()
        {
            Opcode = 0x0002,
            Label = 0x5727,
        };

        /// <summary>
        /// Handles shaking the screen according to the shake registers, restoring PS2 functionality.
        /// </summary>
        public static void HandleShake()
        {
            // If we do not memorize the functions, do so.

            if (VIEWPORT_FUNCTIONS.Count == 0x00)
            {
                VIEWPORT_FUNCTIONS.Add(Hypervisor.Read<byte>(ADJUSTGLOW_OFFSET + 0xE6E, 0x10));
                VIEWPORT_FUNCTIONS.Add(Hypervisor.Read<byte>(INITVIEWPORT_OFFSET + 0x35, 0x07));
                VIEWPORT_FUNCTIONS.Add(Hypervisor.Read<byte>(ADJUSTVIEWPORT_OFFSET + 0x60, 0x08));
            }

            // We only want to do this if the map is loaded.
            if (Variables.IS_LOADED)
            {
                // Read everything there is to do with shake.

                var _shakeTimer = Hypervisor.Read<float>(Variables.ADDR_ShakeInfo);

                var _shakeRegisterX = Hypervisor.Read<short>(Variables.ADDR_ShakeRegister);
                var _shakeRegisterY = Hypervisor.Read<short>(Variables.ADDR_ShakeRegister + 0x04);

                var _currentW = Hypervisor.Read<uint>(Variables.ADDR_RenderResolution - 0x08);
                var _currentH = Hypervisor.Read<uint>(Variables.ADDR_RenderResolution - 0x04);

                // If a shake is present in any way:
                if (_shakeTimer > 0 || _shakeRegisterX != 0 || _shakeRegisterY != 0)
                {
                    // If the room has any glow;
                    if (Variables.HAS_GLOW)
                    {
                        // If we allow shaking with glow, adjust accordingly.
                        if (Locals.SHAKE_BLOOM)
                        {
                            if (!DEBOUNCE[0x11])
                            Hypervisor.DeleteInstruction(ADJUSTGLOW_OFFSET + 0xE6E, 0x10);

                            Hypervisor.Write<float>(Variables.ADDR_FrontRectangle, [1792 + _shakeRegisterX, 1840 + _shakeRegisterY]);
                        }

                        // Otherwise, do not shake.
                        else
                            return;
                    }

                    // Delete the instructions controlling the viewport and write the shake.

                    if (!DEBOUNCE[0x11])
                    {
                        Hypervisor.DeleteInstruction(INITVIEWPORT_OFFSET + 0x35, 0x07);
                        Hypervisor.DeleteInstruction(ADJUSTVIEWPORT_OFFSET + 0x60, 0x08);

                        DEBOUNCE[0x11] = true;
                    }

                    Hypervisor.Write<float>(Variables.ADDR_ViewportRectangle, [_shakeRegisterX, _shakeRegisterY, _currentW + _shakeRegisterX, _currentH + _shakeRegisterY]);
                }

                else if (DEBOUNCE[0x11])
                {
                    // Restore all functions and set the debounce.

                    Hypervisor.Write(ADJUSTGLOW_OFFSET + 0xE6E, VIEWPORT_FUNCTIONS[0]);
                    Hypervisor.Write(INITVIEWPORT_OFFSET + 0x35, VIEWPORT_FUNCTIONS[1]);
                    Hypervisor.Write(ADJUSTVIEWPORT_OFFSET + 0x60, VIEWPORT_FUNCTIONS[2]);

                    DEBOUNCE[0x11] = false;
                }
            }
        }

        /// <summary>
        /// Handles the registration of Magic when obtained outside of regular instances.
        /// Primarily for use with Rando and AP.
        /// </summary>
        public static void RegisterMagic()
        {
            if (!Variables.IS_TITLE)
            {
                // Read the levels of all current Magic.
                var _magicOne = Hypervisor.Read<uint>(Variables.ADDR_MagicLV1);
                var _magicTwo = Hypervisor.Read<ushort>(Variables.ADDR_MagicLV2);

                // If the game is not loaded but magic registry was run:
                if (!Variables.IS_LOADED && MAGIC_REGISTRY.Count != 0x00)
                {
                    Terminal.Log("Roomchange detected! Resetting the Magic Registry...", 1);

                    // Reset the magic registry.
                    MAGIC_REGISTRY = new Dictionary<ulong, ulong>();
                    Hypervisor.Write(Variables.ADDR_MagicInfo, new byte[0x1E0]);

                    // Fetch all magic files that are in the Allocator.
                    var _fetchItems = Variables.MemoryKH.MemoryBlock.Where(x => x.Key.Contains("magic/"));
                    var _keyArray = new List<string>();

                    // Array all the filenames.
                    foreach (var _memoryItem in _fetchItems)
                        _keyArray.Add(_memoryItem.Key);

                    // Free all the allocated magic.
                    foreach (var _keyName in _keyArray)
                        Variables.MemoryKH.Free(_keyName);
                }

                // The the game is loaded, and the magic levels do not match the last recorded:
                else if (Variables.IS_LOADED && _magicOne != MAGIC_LV1_RANDO || _magicTwo != MAGIC_LV2_RANDO)
                {
                    /*
                     *
                     * Quick note: I do not use "MAGIC_LVX" variables, rather
                     * opting to use "MAGIC_LVX_RANDO" variables, as SortMagic
                     * uses those aforementioned variables and I do not want
                     * to deal with a race condition should it occur.
                     * 
                     */
                    MAGIC_LV1_RANDO = _magicOne;
                    MAGIC_LV2_RANDO = _magicTwo;

                    Terminal.Log("New Magic obtained! Adjusting Registry...", 1);

                    // Denote which BAR name corresponds to which command.
                    var _magicCommands = new Dictionary<string, ushort>
                    {
                        { "fire", 0x0031 },
                        { "bliz", 0x0033 },
                        { "thun", 0x0032 },
                        { "cure", 0x0034 },
                        { "magn", 0x00AE },
                        { "refl", 0x00B1 }
                    };

                    // Reset the Magic Commands.
                    Terminal.Log("Resetting all Magic Commands for new Magic registration.", 1);
                    Hypervisor.Write(Variables.ADDR_MagicCommands, new byte[0x0C]);

                    // For every type of magic that is currently in the game (Don't support added magic rn):
                    for (uint i = 0; i < 0x06; i++)
                    {
                        // Get the information of said magic through its index.
                        var _magicTable = Variables.SharpHook[FUNC_MAGICGETTABLE].Execute<ulong>(i);

                        // If we were able to parse the information (Meaning we have the magic):
                        if (_magicTable != 0x00)
                        {
                            // Fetch the filename of the magic, and the magic table pointer.
                            var _fileName = Hypervisor.ReadString(_magicTable + 0x04, true);
                            var _currentPointer = Hypervisor.Read<ulong>(Variables.ADDR_MagicInfo + 0x50 * i);

                            // If the pointer contains our current assignment, skip to the next index.
                            if (_currentPointer == _magicTable)
                                continue;

                            Terminal.Log("Parsed Magic Filename: \"" + _fileName + "\".", 1);

                            // Load the MAG file into memory.
                            var _loadMemory = IO.LoadBAR(_fileName);

                            Terminal.Log("Loaded Magic File at 0x" + _loadMemory.ToString("X12") + "!", 0);

                            // Calculate the necessary addresses depending on the BAR offset.

                            var _barFileOffset = Hypervisor.Read<uint>(_loadMemory + 0x08, true);

                            var _paxOffset = Hypervisor.Read<uint>(_loadMemory + 0x18, true) - _barFileOffset;
                            var _bdxOffset = Hypervisor.Read<uint>(_loadMemory + 0x28, true) - _barFileOffset;

                            // Write everything necessary.

                            Hypervisor.Write(Variables.ADDR_MagicInfo + 0x50 * i, _loadMemory); // Pointer to the MAG file.
                            Hypervisor.Write(Variables.ADDR_MagicInfo + 0x08 + 0x50 * i, _loadMemory + _bdxOffset); // Pointer to the first AI.
                            Hypervisor.Write(Variables.ADDR_MagicInfo + 0x10 + 0x50 * i, _loadMemory + _bdxOffset); // Pointer to the second AI. [These should realistically be identical.]
                            Hypervisor.Write(Variables.ADDR_MagicInfo + 0x18 + 0x50 * i, _loadMemory + _paxOffset); // Pointer to the PAX.
                            Hypervisor.Write(Variables.ADDR_MagicInfo + 0x20 + 0x50 * i, _loadMemory + _paxOffset + 0x10); // Pointer to the PAX past it's header.

                            Hypervisor.Write(Variables.ADDR_MagicInfo + 0x48 + 0x50 * i, _magicTable); // Pointer to the magic table.

                            // Fetch the key we will use for the Magic's registration.
                            var _registryKey = Hypervisor.PureAddress + Variables.ADDR_MagicInfo + 0x18 + 0x50 * i;

                            // If the key exists in the registry, update it.
                            if (MAGIC_REGISTRY.ContainsKey(_registryKey))
                                MAGIC_REGISTRY[_registryKey] = _loadMemory + _paxOffset;

                            // Otherwise, add it in.
                            else
                                MAGIC_REGISTRY.Add(_registryKey, _loadMemory + _paxOffset);

                            Terminal.Log("All Magic Info has been written for file: \"" + _fileName + "\".", 0);
                        }
                    }

                    // Initialize the Magic Command list.
                    var _commandList = new List<ushort>();

                    // For all magic that could be registered:
                    foreach (var _magicItem in MAGIC_REGISTRY)
                    {
                        // Fetch the 4-letter name in the MAG file.
                        var _magicNameArray = Hypervisor.Read<byte>(_magicItem.Value - 0x2C, 0x04, true);
                        var _magicName = Encoding.Default.GetString(_magicNameArray);

                        // Initialize the PAX belonging to the magic.
                        Terminal.Log("Initalizing PAX with data at 0x" + _magicItem.Value.ToString("X12") + " in memory location 0x" + _magicItem.Key.ToString("X12") + "!", 1);
                        Effect.InitializePAX(_magicItem.Key, _magicItem.Value);

                        // Add the magic command to the list.
                        Terminal.Log("Writing Command ID: 0x" + _magicCommands[_magicName].ToString("X4") + " to the Magic Menu!", 1);
                        _commandList.Add(_magicCommands[_magicName]);
                    }

                    // Order the list according to "_magicCommands" and write it.
                    var _orderedList = _commandList.OrderBy(x => _magicCommands.Values.ToList().IndexOf(x)).ToArray();
                    Hypervisor.Write(Variables.ADDR_MagicCommands, _orderedList);

                    MAGIC_STORE = Hypervisor.Read<byte>(Variables.ADDR_MagicCommands, 0x0C);

                    Terminal.Log("The Magic Registry has been adjusted successfully!", 0);
                }
            }
        }

        /// <summary>
        /// Enables certain functions to be executed specifically for Archipelago use.
        /// Is not enabled unless Randomizer is detected.
        /// </summary>
        public static void EnablersAP()
        {
            // If these prequisites are met:
            // - Is not on Title.
            // - Is not in a Cutscene.
            // - Is not Paused.
            // - The game is loaded.
            if (!Variables.IS_TITLE && !Variables.IS_CUTSCENE && !Variables.IS_PAUSED && Variables.IS_LOADED)
            {
                // If the abilities are not read into memory: Do so.
                if (ABILITY_ARRAY == null)
                    ABILITY_ARRAY = Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x2544, 0x60);

                // Read the abilities to check with old array.
                var _readAbilities = Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x2544, 0x60);

                // If the ability array differs, Refresh Sora's abilities.
                if (!Enumerable.SequenceEqual(_readAbilities, ABILITY_ARRAY))
                {
                    Terminal.Log("Ability Mismatch! Refreshing...", 0);

                    Shisutemu.RefreshSora();
                    ABILITY_ARRAY = _readAbilities;
                }

                // Check the fade level and enable line.
                var _fadeCheck = Hypervisor.Read<byte>(Variables.ADDR_FadeValue);
                var _enableLine = Hypervisor.Read<byte>(0x800000);

                // If the fade does not exist and the enable line is set:
                if (_fadeCheck == 0x00 && _enableLine > 0x00)
                {
                    // Unset the enable line, we have it in memory.
                    Hypervisor.Write<byte>(0x800000, 0x00);

                    // According to the enable line:
                    switch (_enableLine)
                    {
                        // If "1", show the Information Bar with text at 0x800004.
                        case 1:
                            Terminal.Log("Information Bar has been requested externally! Showing...", 0);
                            Popup.PopupInformation(Hypervisor.PureAddress + 0x800004);
                            break;

                        // If "2", show the Prize Bar with text at 0x800104.
                        case 2:
                            Terminal.Log("Prize Bar has been requested externally! Showing...", 0);
                            Popup.PopupPrize(Hypervisor.PureAddress + 0x800104);
                            break;
                    }
                }

                // Fetch Sora's Gauge.
                var _soraGauge = Hypervisor.GetPointer64(Variables.PINT_PlayerGauge, [0x88, 0x00]);

                // Fetch Sora himself, and his stats.
                var _soraPointer = Hypervisor.Read<ulong>(Variables.PINT_Sora);
                var _currentHealth = Hypervisor.Read<byte>(Variables.ADDR_PlayerStats);
                var _currentPlayer = Hypervisor.Read<ushort>(Variables.ADDR_CurrentCharacter);

                // If Sora's Guage exists, Sora is not Mermaid Sora, and his HP is 0:
                if (_soraGauge != 0x00 && _currentPlayer != 0x03BE && _currentHealth == 0x00 && !DEBOUNCE[0x10])
                {
                    // Trigger Death.
                    Terminal.Log("Conditions are met and Sora's HP is 0! Updating status...", 0);
                    Shisutemu.AddHP(_soraPointer, 0x00);
                    DEBOUNCE[0x10] = true;
                }

                // If he has healed and triggered death before: Consider death triggerable.
                else if (_currentHealth != 0x00 && DEBOUNCE[0x10])
                    DEBOUNCE[0x10] = false;
            }
        }

        /// <summary>
        /// If requested, skips Roxas' Prologue segment.
        /// Alright, because of all the flag nonsense I could not be bothered to rewrite this.
        /// Boo hoo. Too bad! I have optimized it here and there but I will replace the flag writing nonsense with a file write someday.
        /// </summary>
        public static void PrologueSkip()
        {
            if (!Locals.RANDOMIZER)
            {
                // Read these for use later.
                var _diffRead = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x2498);
                var _selectButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);

                // If on the Title Screen;
                if (Variables.IS_TITLE)
                {
                    // If the selected button is NOT 0x00 (New Game) and Roxas Skip is marked as "Ready";
                    if (_selectButton != 0x00 && ROXAS_SKIP_STAGE == 0)
                    {
                        // Mark Roxas Skip as "Disabled".
                        Terminal.Log("Disabling the Prologue Skip, expecting a game load.", 0);
                        ROXAS_SKIP_STAGE = 4;
                    }

                    // If the selected button IS 0x00 (New Game) and Roxas Skip is marked as "Disabled";
                    else if (_selectButton == 0x00 && ROXAS_SKIP_STAGE > 0x00)
                    {
                        // Mark Roxas Skip as "Ready".
                        Terminal.Log("Enabling the Prologue Skip, expecting a new game.", 0);
                        ROXAS_SKIP_STAGE = 0;
                    }
                }

                // If not on the Title Screen;
                if (!Variables.IS_TITLE)
                {
                    // Read all the map-specific values
                    var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_Area);
                    var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
                    var _eventCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x04);
                    var _fadeCheck = Hypervisor.Read<byte>(Variables.ADDR_FadeValue);

                    // If the cutscene "Twilight Town - Waking from a Dream" is playing and Roxas Skip is marked "Ready";
                    if (_worldCheck == 0x02 && _roomCheck == 0x01 && _eventCheck == 0x38 && ROXAS_SKIP_STAGE == 0)
                    {
                        // If the Debounce is set;
                        if (DEBOUNCE[0x09])
                        {
                            Terminal.Log("Everything seems to be correct! Initiating Prologue Skip...", 1);

                            // Lock the autosave.
                            DEBOUNCE[0x01] = true;

                            // Suspend the game to run stuff in the back.
                            Terminal.Log("Entering the CRITICAL SECTION in anticipation of a warp! Suspending the game...", 1);
                            AxaInterface.Suspend(false);

                            // Construct the area code for "Dive into the Heart - Station of Awakening".
                            Terminal.Log("Constructing the warp-to point!", 1);
                            var _warpArea = new byte[] { 0x02, 0x20, 0x32, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };

                            // Write the target area in question.
                            Hypervisor.Write(Variables.ADDR_Area, _warpArea);

                            // Execute the warp.
                            Terminal.Log("Initiating warp to Roxas's Heart!", 1);
                            Shisutemu.ExecuteWarp(Variables.ADDR_Area, Locals.IS_FLASHBANG ? Shisutemu.FADE_TYPE.WHITEOUT : Shisutemu.FADE_TYPE.BLACKOUT);

                            // Everything critical has been completed. Resume the game.
                            Terminal.Log("CRITICAL SECTION has been completed! Resuming the game.", 1);
                            AxaInterface.Resume(false);

                            // Write the flags for Command Menu access.
                            Hypervisor.Write(Variables.ADDR_SaveData + 0x1CD0, 0x1FF00001);
                            Hypervisor.Write(Variables.ADDR_SaveData + 0x1CD4, 0x00000000);

                            Terminal.Log("Prologue Skip's First Stage has been completed!", 0);

                            // Mark Roxas Skip as "Dream Selection".
                            ROXAS_SKIP_STAGE = 1;
                        }

                        // If the Debounce is not set; Mark Roxas Skip as "Disabled".
                        else
                        {
                            Terminal.Log("Prologue Skip has not been requested! Not initiating the function.", 0);
                            ROXAS_SKIP_STAGE = 4;
                        }
                    }

                    // If we are at "Dive into the Heart - Station of Awakening" and any cutscene is playing, and Roxas Skip is marked "Dream Selection";
                    else if (_worldCheck == 0x02 && _roomCheck == 0x20 && _eventCheck == 0x01 && (Variables.IS_CUTSCENE || Variables.IS_EVENT) && ROXAS_SKIP_STAGE == 1)
                    {
                        Terminal.Log("Dream Weapon was chosen after Prologue Skip! Moving unto the Second Stage...", 1);

                        var _configRead = Hypervisor.Read<int>(Variables.ADDR_Config);

                        Terminal.Log("Entering the CRITICAL SECTION in anticipation of a warp! Suspending the game...", 1);
                        AxaInterface.Suspend(false);

                        // Construct a new area memory to temporarily warp to.
                        Terminal.Log("Constructing the warp-to point!", 1);
                        var _warpArea = new byte[] { 0x02, 0x0E, 0x32, 0x00, 0x02, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00 };

                        // Write the target area in question.
                        Hypervisor.Write(Variables.ADDR_Area, _warpArea);

                        Terminal.Log("Initiating warp to Sora's Twilight Town!", 1);
                        Shisutemu.ExecuteWarp(Variables.ADDR_Area, Locals.IS_FLASHBANG ? Shisutemu.FADE_TYPE.WHITEOUT : Shisutemu.FADE_TYPE.BLACKOUT);

                        Terminal.Log("Writing all the necessary flags for the game to function!", 1);

                        Hypervisor.Write(
                            Variables.ADDR_SaveData + 0x31C,
                            new byte[]
                            {
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x05,
                           0x00,
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x04,
                           0x00,
                           0x15,
                           0x00,
                           0x12,
                           0x00,
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x04,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x12,
                           0x00,
                           0x02,
                           0x00,
                           0x00,
                           0x00,
                           0x12
                            });

                        Hypervisor.Write(Variables.ADDR_SaveData + 0x03E8, 0x04);
                        Hypervisor.Write(Variables.ADDR_SaveData + 0x03EE, 0x04);

                        Hypervisor.Write(
                        Variables.ADDR_SaveData + 0x1CD0,
                        new byte[]
                        {
                            0x01,
                            0x00,
                            0xF0,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xDB,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0xFF,
                            0x3F,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0x00,
                            0xD0,
                            0x05,
                            0x08,
                            0x01,
                            0x00,
                            0x00,
                            0x81
                        });

                        Hypervisor.Write(Variables.ADDR_SaveData + 0x1CE2, 0x67);

                        var _assembly = Assembly.GetExecutingAssembly();
                        var _resourceName = _assembly.GetManifestResourceNames();

                        byte[] _resourceFile = Array.Empty<byte>();

                        using (var _resStream = _assembly.GetManifestResourceStream(_resourceName.First(x => x.Contains("prologue_flags"))))
                        using (var _resReader = new BinaryReader(_resStream))
                            _resourceFile = _resReader.ReadBytes((int)_resStream.Length);

                        Hypervisor.Write(Variables.ADDR_SaveData + 0x20E4, _resourceFile);
                        Hypervisor.Write(Variables.ADDR_Config, _configRead);

                        if (_diffRead == 0x03)
                        {
                            Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0x24F8, 0x32);
                            Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0x24F4, 0x18);
                            Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0x24F4 + 0x01, 0x18);
                            Hypervisor.Write(
                                Variables.ADDR_SaveData + 0x2544,
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
                            Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0x24F4, 0x1E);
                            Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0x24F4 + 0x01, 0x1E);
                            Hypervisor.Write(
                                Variables.ADDR_SaveData + 0x2544,
                                new byte[] { 0x52, 0x00, 0x8A, 0x00, 0x9E, 0x00 }
                            );
                        }

                        Terminal.Log("All the flags were written successfully!", 0);

                        // Everything critical has been completed. Resume the game.
                        Terminal.Log("CRITICAL SECTION has been completed! Resuming the game.", 1);
                        AxaInterface.Resume(false);

                        Terminal.Log("Prologue Skip has been completed! Enjoy the game as Sora!", 0);

                        DEBOUNCE[0x01] = false;
                        ROXAS_SKIP_STAGE = 3;
                    }
                }
            }
        }

        /// <summary>
        /// Adds the options "Retry" as well as "Prepare and Retry" to the game.
        /// This function is the most anticipated of all in Re:Fined, it is also fuckin' SHIT.
        /// This has been the bane of my existence, and I sort of which I never implemented it.
        /// But nyeh, 'tis what it is.
        /// </summary>
        public static void RetryBattles()
        {
            // Note the offsets for the instructions we are going to be toggling on and off.
            var _mapJumpOffset = (ulong)Shisutemu.FUNC_MAPJUMP + 0x1F2;
            var _continueOffset = (ulong)Shisutemu.FUNC_MAPJUMP + 0x1D8;

            // Read the area state to see the blacklisted areas, as well as Hades Escape.
            var _areaState = Hypervisor.Read<byte>(Variables.ADDR_Area, 0x10);
            var _currentCommands = Hypervisor.Read<byte>(Variables.ADDR_CommandType);

            var _blacklistCavern = _areaState[0x00] == 0x04 && _areaState[0x01] >= 0x15 && _areaState[0x01] <= 0x1A;
            var _blacklistFinal = _areaState[0x00] == 0x12 && ((_areaState[0x01] >= 0x16 && _areaState[0x01] <= 0x1C) || _areaState[0x01] == 0x14);

            // If the instructions have not been read:
            if (INST_MAPJUMPTASK == null)
            {
                Terminal.Log("Functions regarding Retry Functionality have been read into memory!", 0);

                // Read the instructions to memory.
                // NOTE: These are anchored to "MapJump" instead of "MapJumpTask". This can prove problematic in future versions.
                // I will get there when I get there.
                INST_MAPJUMPTASK = Hypervisor.Read<byte>(_mapJumpOffset, 0x05);
                INST_CONTINUELOAD = Hypervisor.Read<byte>(_continueOffset, 0x05);
            }

            if ((_blacklistCavern || _blacklistFinal) && !DEBOUNCE[0x0C])
            {
                Terminal.Log("This area is blacklisted in Retry! Disabling functions...", 1);
                DEBOUNCE[0x0C] = true;
            }

            else if (!_blacklistCavern && !_blacklistFinal && DEBOUNCE[0x0C])
            {
                Terminal.Log("Out of the blacklisted region! Enabling Retry functions...", 1);
                DEBOUNCE[0x0C] = false;
            }

            if (!DEBOUNCE[0x0C])
            {
                // If the battle is BOSS, There isn't a cutscene playing, and the Retry State hasn't been noted:
                if (Variables.BATTLE_MODE == Variables.BATTLE_TYPE.BOSS && !Variables.IS_CUTSCENE && RETRY_STATE == null)
                {
                    Terminal.Log("Forced Battle has been detected! Reading the game state into memory...", 0);

                    IS_ESCAPING = _areaState[0x00] == 0x06 && _areaState[0x01] == 0x05 && _areaState[0x04] == 0x6F;

                    if (IS_ESCAPING && HADES_STATE == 0xFF)
                    {
                        Terminal.Log("Hades Escape detected! Starting edge-case sub-routine.", 1);
                        HADES_STATE = 0x00;
                    }

                    // Read the Retry State.
                    RETRY_STATE = Hypervisor.Read<byte>(Variables.ADDR_SaveData, 0x10FC0);

                    // Insert the options to the Game Over screen.
                    Locals.MAIN_CONTINUE.Children.Insert(Locals.RETRY_FIRST ? 0x00 : 0x01, PREPARE_ENTRY);
                    Locals.MAIN_CONTINUE.Children.Insert(Locals.RETRY_FIRST ? 0x00 : 0x01, RETRY_ENTRY);
                }

                if (RETRY_STATE != null && Locals.MAIN_CONTINUE.Children.Count != 0x04)
                {
                    Terminal.Log("Retry is in memory but isn't applied! Applying menu options...", 1);

                    Locals.MAIN_CONTINUE.Children.Insert(Locals.RETRY_FIRST ? 0x00 : 0x01, PREPARE_ENTRY);
                    Locals.MAIN_CONTINUE.Children.Insert(Locals.RETRY_FIRST ? 0x00 : 0x01, RETRY_ENTRY);
                }

                // If Retry hasn't been triggered, is on Hades Escape, and the Escape is initialized;
                if (RETRY_MODE == 0x00 && IS_ESCAPING && HADES_STATE != 0xFF)
                {
                    // If the Battle Mode is field, and the counter hasn't been incremented;
                    if (Variables.BATTLE_MODE == Variables.BATTLE_TYPE.FIELD && !HADES_CHANGE)
                    {
                        // Increment the counter.

                        Terminal.Log("Hades Escape state change detected! Incrementing the counter!", 0);
                        HADES_STATE += 0x01;
                        HADES_CHANGE = true;
                    }

                    // If the Battle Mode is NOT Field, and the counter HAS been incremented;
                    if (Variables.BATTLE_MODE != Variables.BATTLE_TYPE.FIELD && HADES_CHANGE)
                    {
                        // Release the increment counter.
                        Terminal.Log("Hades Escape state change completed! Locking the counter!", 0);
                        HADES_CHANGE = false;
                    }

                    // If there has been 3 state changes;
                    if (HADES_STATE == 0x03)
                    {
                        Terminal.Log("Hades Escape successful! Releasing out-of-battle handler.", 0);

                        // Deinitialize the Escape and denote we are no longer escaping.
                        HADES_STATE = 0xFF;
                        IS_ESCAPING = false;

                        // Just in case, restore the load functions.
                        Hypervisor.Write(_mapJumpOffset, INST_MAPJUMPTASK);
                        Hypervisor.Write(_continueOffset, INST_CONTINUELOAD);
                    }
                }

                else if (Variables.IS_TITLE || Variables.BATTLE_MODE != Variables.BATTLE_TYPE.BOSS)
                {
                    // If Retry Mode is greater than 0 (so it's on), and Retry State has been noted, and we are NOT in the Title Screen:
                    if (RETRY_MODE > 0x00 && RETRY_STATE != null && !Variables.IS_TITLE)
                    {
                        Terminal.Log("Some kind of Retry has been requested! Suspending the game!", 1);

                        // Suspend the game to not have timing related problems. We are not going to suspend the audio engine with this one.
                        // Why? If done so it's really jarring.
                        AxaInterface.Suspend(false);

                        Terminal.Log("Writing back the Retry State to memory!", 1);

                        // Restore the retry state. This eliminates the need for a LOT of code.
                        Hypervisor.Write(Variables.ADDR_SaveData, RETRY_STATE);

                        // This restores the HP/MP of all Party Members. Apparently necessary as some battles don't do that when you start them.
                        Shisutemu.Recover();

                        // If Retry Mode is 0x02 (aka Prepare):
                        if (RETRY_MODE == 0x02)
                        {
                            Terminal.Log("Prepare and Retry has been requested specifically! Queueing the Camp Menu!", 1);

                            // Launch the CAMP Menu. Did you notice how we are still in a suspend state?
                            // Yes, this is to prevent the game from loading anything important, as doing so WILL cause a crash when you change your loadout.

                            DEBOUNCE[0x01] = true;

                            ulong _initOffset = Variables.PLATFORM == "STEAM" ? 0x517U : 0x4D7U;

                            var _campBitwise = Variables.CAMP_BITWISE.ITEMS |
                                               Variables.CAMP_BITWISE.ABILITIES |
                                               Variables.CAMP_BITWISE.CUSTOMIZE |
                                               Variables.CAMP_BITWISE.PARTY;

                            Hypervisor.DeleteInstruction(Popup.CAMP_OFFSET + 0x1A7, 0x07);
                            Hypervisor.DeleteInstruction(Popup.CAMPINIT_OFFSET + _initOffset, 0x08);

                            Hypervisor.Write(Variables.ADDR_CampBitwise, _campBitwise);

                            Popup.PopupMenu(Popup.MENU.CAMP, 0);
                        }

                        Terminal.Log("Retry state setup has been completed! Resuming the game.", 0);

                        // Resume the game.
                        AxaInterface.Resume(false);

                        // Reset the function calls.
                        Hypervisor.Write(_mapJumpOffset, INST_MAPJUMPTASK);
                        Hypervisor.Write(_continueOffset, INST_CONTINUELOAD);

                        // Reset the continue menu.
                        Locals.MAIN_CONTINUE = new Continue();

                        RETRY_MODE = 0x00;
                    }

                    else if (RETRY_MODE == 0x00 && RETRY_STATE != null && !Variables.IS_PAUSED)
                    {
                        Terminal.Log("Flushing the game state from memory...", 0);
                        RETRY_STATE = null;
                        RETRY_WAIT = false;

                        // Reset the function calls.
                        Hypervisor.Write(_mapJumpOffset, INST_MAPJUMPTASK);
                        Hypervisor.Write(_continueOffset, INST_CONTINUELOAD);

                        ulong _initOffset = Variables.PLATFORM == "STEAM" ? 0x517U : 0x4D7U;

                        Hypervisor.Write(Popup.CAMP_OFFSET + 0x1A7, Popup.CAMP_FUNCTION);
                        Hypervisor.Write(Popup.CAMPINIT_OFFSET + _initOffset, Popup.CAMPINIT_FUNCTION);

                        // Reset the continue menu.
                        Locals.MAIN_CONTINUE = new Continue();

                        DEBOUNCE[0x01] = false;
                    }
                }

                var _menuType = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);
                var _menuSelection = Hypervisor.Read<byte>(Variables.ADDR_MenuSelect);
                var _continuePointer = Hypervisor.Read<ulong>(Variables.PINT_GameOver);

                // If the "Game Over" menu is shown, and the selected option is not larger than the 4th one, and the Retry State has been read:
                if (_continuePointer != 0x00 && RETRY_STATE != null)
                {
                    // Suspend the game until everything is initialized.
                    if (!RETRY_WAIT)
                    {
                        Terminal.Log("Initializing Retry Variables on death! Suspending the game!", 1);
                        AxaInterface.Suspend(false);
                    }

                    // Check if we are escaping, if so:
                    if (IS_ESCAPING && HADES_STATE != 0x00)
                    {
                        // Reset the counter.
                        Terminal.Log("Death on Hades Escape detected! Resetting the counter!", 1);
                        HADES_STATE = 0x00;
                        HADES_CHANGE = false;
                    }

                    if (_menuSelection < 0x04)
                    {
                        // Set the Retry Mode according to the selected option.
                        RETRY_MODE = _menuSelection == 0x00 ? 0x01 : (_menuSelection == 0x01 ? 0x02 : 0x00);
                        RETRY_MODE = _menuType == 0xFF ? RETRY_MODE : 0x00;

                        // If Retry is active, destroy the function calls.
                        if (RETRY_MODE > 0x00)
                        {
                            Hypervisor.DeleteInstruction(_mapJumpOffset, 0x05);
                            Hypervisor.DeleteInstruction(_continueOffset, 0x05);
                        }

                        // Else, restore them.
                        else
                        {
                            Hypervisor.Write(_mapJumpOffset, INST_MAPJUMPTASK);
                            Hypervisor.Write(_continueOffset, INST_CONTINUELOAD);
                        }
                    }

                    // After all is said and done, resume the game.
                    if (!RETRY_WAIT)
                    {
                        Terminal.Log("Retry variables initialized! Resuming the game.", 0);
                        AxaInterface.Resume(false);
                        RETRY_WAIT = true;
                    }
                }

                // If Retry is on, but the Command Type is "MICKEY" (aka, rescued by Mickey):
                else if (RETRY_MODE > 0x00 && Variables.BATTLE_MODE == Variables.BATTLE_TYPE.BOSS && _currentCommands == 0x01)
                {
                    Terminal.Log("Mickey detected! Applying edge-case premeasures...", 1);

                    // Reset the function calls.
                    Hypervisor.Write(_mapJumpOffset, INST_MAPJUMPTASK);
                    Hypervisor.Write(_continueOffset, INST_CONTINUELOAD);

                    // Reset the continue menu.
                    Locals.MAIN_CONTINUE = new Continue();
                    RETRY_MODE = 0x00;
                }
            }
        }

        /// <summary>
        /// Fix-up the 2D and 3D elements in aspect ratios other than 16:9.
        /// This is fucky. I know that it's fucky. But eh, too bad!
        /// </summary>
        public static void AspectCorrection()
        {
            if (!DELAY_ASPECT)
            {
                if (DELAY_START == DateTime.MinValue)
                    DELAY_START = DateTime.Now;

                var _currTime = DateTime.Now;
                var _currSeconds = (_currTime - DELAY_START).TotalSeconds;

                if (_currSeconds >= 5)
                    DELAY_ASPECT = true;

                Hypervisor.DeleteInstruction(ASPECT_OFFSET + 0x0F6, 0x06);
                Hypervisor.DeleteInstruction(ASPECT_OFFSET + 0x101, 0x06);
            }

            else
            {
                var _renderWidth = Hypervisor.Read<float>(Variables.ADDR_RenderResolution);
                var _renderHeight = Hypervisor.Read<float>(Variables.ADDR_RenderResolution + 0x04);

                var _commonDivider = 0.00F;

                if (_renderWidth != 0x00 && _renderHeight != 0x00)
                {
                    POSITIVE_OFFSET = 0x0055;
                    NEGATIVE_OFFSET = -0x0055;

                    HEIGHT_OFFSET = 0x0000;

                    var _tempWidth = _renderWidth;
                    var _tempHeight = _renderHeight;

                    while (_tempWidth != 0 && _tempHeight != 0)
                    {
                        if (_tempWidth > _tempHeight)
                            _tempWidth %= _tempHeight;
                        else
                            _tempHeight %= _tempWidth;
                    }

                    _commonDivider = _tempWidth == 0 ? _tempHeight : _tempWidth;

                    var _ratioNumber = (float)Math.Round(_renderWidth / _renderHeight, 2);
                    var _ratioString = String.Format("{0}:{1}", _renderWidth / _commonDivider, _renderHeight / _commonDivider);


                    /*
                    if (_ratioString == "4:3" || _ratioString == "8:5")
                    {
                        Hypervisor.Write(Variables.ADDR_Viewspace3D + 0x04, _ratioString == "4:3" ? 1.4F : 1.15F);
                        HEIGHT_OFFSET = _ratioString == "4:3" ? 0x4B : 0x14; 
                    }
                    */

                    Hypervisor.Write(Variables.ADDR_Viewspace3D, _ratioNumber - 0.77F);

                    switch (_ratioString)
                    {
                        case "64:27":
                        case "43:18":
                            POSITIVE_OFFSET = 0x00C3;
                            NEGATIVE_OFFSET = -0x00C3;
                            break;
                        case "32:9":
                            POSITIVE_OFFSET = 0x01A5;
                            NEGATIVE_OFFSET = -0x01A5;
                            break;
                    }

                    /*
                    var _playerGauge = Hypervisor.GetPointer64(Variables.PINT_PlayerGauge, [0x1F04]);
                    var _commandDraw = Hypervisor.GetPointer64(Variables.PINT_CommandDraw, [0x0364]);
                    var _friendGauges = new ulong[] { Hypervisor.GetPointer64(Variables.PINT_FriendGauge, [0x0534]), Hypervisor.GetPointer64(Variables.PINT_FriendGauge + 0x08, [0x0534]) };

                    if (!Variables.IS_LOADED)
                        COMMAND_DROP_Y = 0x00;

                    if (COMMAND_DROP_Y == 0x00)
                        COMMAND_DROP_Y = Hypervisor.Read<int>(_commandDraw, true);

                    Hypervisor.Write(_playerGauge, HEIGHT_OFFSET, true);
                    Hypervisor.Write(_friendGauges[0], HEIGHT_OFFSET, true);
                    Hypervisor.Write(_friendGauges[1], HEIGHT_OFFSET - 0x30, true);
                    Hypervisor.Write(_commandDraw, HEIGHT_OFFSET + COMMAND_DROP_Y, true);

                    if (Locals.PLATFORM == "STEAM")
                    {
                        Hypervisor.Write(0x15E55C, NEGATIVE_OFFSET);

                        Hypervisor.Write(0x17A586, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17A5C0, POSITIVE_OFFSET);

                        Hypervisor.Write(0x17F2BB, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F313, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F35A, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F37F, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F399, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F3C9, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F3F9, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F429, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F5E5, POSITIVE_OFFSET);
                        Hypervisor.Write(0x17F619, POSITIVE_OFFSET);

                        Hypervisor.Write(0x180BBF, POSITIVE_OFFSET);
                        Hypervisor.Write(0x1819A9, POSITIVE_OFFSET);
                        Hypervisor.Write(0x18157D, POSITIVE_OFFSET);
                        Hypervisor.Write(0x1809F8, POSITIVE_OFFSET);

                        Hypervisor.Write(0x180E5A, POSITIVE_OFFSET);
                        Hypervisor.Write(0x180EFF, POSITIVE_OFFSET);

                        Hypervisor.Write(0x181D89, POSITIVE_OFFSET);

                        Hypervisor.Write(0x182E4C, POSITIVE_OFFSET);
                        Hypervisor.Write(0x182E86, POSITIVE_OFFSET);
                        Hypervisor.Write(0x182EB5, POSITIVE_OFFSET);
                        Hypervisor.Write(0x182EE4, POSITIVE_OFFSET);
                        Hypervisor.Write(0x182F13, POSITIVE_OFFSET);
                        Hypervisor.Write(0x182F42, POSITIVE_OFFSET);
                        Hypervisor.Write(0x183049, POSITIVE_OFFSET);

                        Hypervisor.Write(0x18BA66, POSITIVE_OFFSET);
                        Hypervisor.Write(0x18B757, NEGATIVE_OFFSET);

                        Hypervisor.Write(0x18C556, POSITIVE_OFFSET);
                        Hypervisor.Write(0x18C932, NEGATIVE_OFFSET);

                        Hypervisor.Write(0x18CEF6, NEGATIVE_OFFSET);
                        Hypervisor.Write(0x18D0D7, POSITIVE_OFFSET);

                        Hypervisor.Write(0x18DD06, NEGATIVE_OFFSET);
                        Hypervisor.Write(0x18DD3A, NEGATIVE_OFFSET);

                        Hypervisor.Write(0x18E58F, NEGATIVE_OFFSET);
                    }
                    */
                }
            }
        }

        /// <summary>
        /// Handles the logic behind the Intro (New Game) Menu.
        /// I hate this shit. I despise it. But eh, gotta look official!
        /// </summary>
        public static void HandleIntro()
        {
            // If the game's on the Title Screen;
            if (Variables.IS_TITLE)
            {
                // If the setting are written;
                if (DEBOUNCE[0x07])
                {
                    // Reset the settings written flag.
                    Terminal.Log("Title Screen detected, but the settings are written! Resetting settings...", 0);
                    DEBOUNCE[0x07] = false;
                }

                // Fetch the game state.
                var _titleButton = Hypervisor.Read<byte>(Variables.ADDR_TitleSelect);
                var _introState = Hypervisor.Read<int>(Variables.MemoryKH["INTRO_MEMORY"] + 0x200, 0x100, true);

                // See if we are on the "New Game" button, and thus, should enforce the settings.
                DEBOUNCE[0x08] = _titleButton == 0x00 ? true : false;

                // Get the offsets for the add-ons.
                var _audioOffset = Convert.ToInt32(AUDIO_ACTIVE) + Convert.ToInt32(SUB_AUDIO_ACTIVE);
                var _musicOffset = Convert.ToInt32(AUDIO_ACTIVE) + Convert.ToInt32(SUB_AUDIO_ACTIVE) + Convert.ToInt32(MUSIC_ACTIVE);

                ADDON_OFFSET = Convert.ToInt32(AUDIO_ACTIVE) + Convert.ToInt32(SUB_AUDIO_ACTIVE) + Convert.ToInt32(MUSIC_ACTIVE) + Convert.ToInt32(ENEMY_ACTIVE);

                // Set the settings in-line with the chosen options in the Intro Menu.
                SETTING_MEMORY = Locals.CONFIG.SUMMON_FULL | Locals.CONFIG.NAVI_MAP |
                                 (_introState[0x01] == 0x00 ? Locals.CONFIG.VIBRATION : Locals.CONFIG.OFF) |
                                 (_introState[0x02] == 0x00 ? Locals.CONFIG.AUTOSAVE_INDICATOR :
                                 (_introState[0x02] == 0x01 ? Locals.CONFIG.AUTOSAVE_SILENT : Locals.CONFIG.OFF)) |
                                 (_introState[0x03] == 0x00 ? Locals.CONFIG.PROMPT_CONTROLLER : Locals.CONFIG.OFF);

                // If the Audio Add-on is active;
                if (AUDIO_ACTIVE)
                {
                    // Account for the Audio add-on.
                    SETTING_MEMORY = SETTING_MEMORY |
                                    (_introState[0x04] == 0x01 ? Locals.CONFIG.AUDIO_PRIMARY :
                                    (_introState[0x04] == 0x02 ? Locals.CONFIG.AUDIO_SECONDARY : Locals.CONFIG.OFF));

                    // If the Sub-Menu for the Intro Menu exists, and it isn't Sub-Audio only;
                    if (Locals.AUDIO_SUB_INTRO != null && !Locals.AUDIO_SUB_ONLY)
                    {
                        // If the third option is chosen, and the Sub-Menu isn't active.
                        if (_introState[0x04] == 0x02 && !SUB_AUDIO_ACTIVE)
                        {
                            // Activate the Sub-Menu.
                            Locals.MAIN_INTRO.Children.Insert(0x05, Locals.AUDIO_SUB_INTRO);
                            SUB_AUDIO_ACTIVE = true;
                        }

                        // If the third option is NOT selected, and the Sub-Menu is active;
                        else if (_introState[0x04] != 0x02 && SUB_AUDIO_ACTIVE)
                        {
                            // Deactivate the Sub-Menu.
                            Locals.MAIN_INTRO.Children.Remove(Locals.AUDIO_SUB_INTRO);
                            SUB_AUDIO_ACTIVE = false;

                            // Reset the corresponding setting.
                            Hypervisor.Write(Variables.MemoryKH["INTRO_MEMORY"] + 0x200 + 0x14, 0x00, true);
                        }
                    }
                }

                // Account for the Music add-on.
                if (MUSIC_ACTIVE)
                    SETTING_MEMORY = SETTING_MEMORY | (_introState[0x04 + _audioOffset] == 0x00 ? Locals.CONFIG.BACKGROUND_MUSIC : Locals.CONFIG.OFF);

                // Account for the Enemy add-on.
                if (ENEMY_ACTIVE)
                    SETTING_MEMORY = SETTING_MEMORY | (_introState[0x04 + _musicOffset] == 0x00 ? Locals.CONFIG.HEARTLESS_PALETTE : Locals.CONFIG.OFF);

                // If the game isn't lite, set the Prologue Skip flag.
                if (!Locals.IS_LITE)
                    DEBOUNCE[0x09] = _introState[0x04 + ADDON_OFFSET] == 0x01;
            }

            // If we are not on the Title Screen anymore, the settings are not written, and the settings SHOULD be written;
            else if (!Variables.IS_TITLE && !DEBOUNCE[0x07] && DEBOUNCE[0x08])
            {
                // Read the current room.
                var _currentArea = Hypervisor.Read<ushort>(Variables.ADDR_Area);

                // If the current area is 02 01 (Twilight Town - Beginning FMV) or 02 20 (Dive into the Heart);
                if (_currentArea == 0x0102 || _currentArea == 0x2002)
                {
                    Terminal.Log("A new game has been detected! Writing the settings...", 1);

                    // Write the settings.
                    Hypervisor.Write(Variables.ADDR_Config, SETTING_MEMORY);

                    // Set all Re:Fined specific settings.

                    Locals.MUSIC_MODE = (SETTING_MEMORY & Locals.CONFIG.BACKGROUND_MUSIC) == Locals.CONFIG.BACKGROUND_MUSIC;
                    Locals.ENEMY_MODE = (SETTING_MEMORY & Locals.CONFIG.HEARTLESS_PALETTE) == Locals.CONFIG.HEARTLESS_PALETTE;
                    Locals.CONTROLLER_MODE = (SETTING_MEMORY & Locals.CONFIG.PROMPT_CONTROLLER) == Locals.CONFIG.PROMPT_CONTROLLER;

                    Locals.AUDIO_MODE = (SETTING_MEMORY & Locals.CONFIG.AUDIO_PRIMARY) == Locals.CONFIG.AUDIO_PRIMARY ? 0x01 :
                                       ((SETTING_MEMORY & Locals.CONFIG.AUDIO_SECONDARY) == Locals.CONFIG.AUDIO_SECONDARY ? 0x02 : 0x00);

                    Locals.SAVE_MODE = (SETTING_MEMORY & Locals.CONFIG.AUTOSAVE_INDICATOR) == Locals.CONFIG.AUTOSAVE_INDICATOR ? 0x00 : 
                                      ((SETTING_MEMORY & Locals.CONFIG.AUTOSAVE_SILENT) == Locals.CONFIG.AUTOSAVE_SILENT ? 0x01 : 0x02);

                    // Set the debounce flags accordingly.
                    DEBOUNCE[0x08] = false;
                    DEBOUNCE[0x07] = true;
                    DEBOUNCE[0x05] = true;

                    Terminal.Log("All settings have been written!", 0);
                }
            }
        }

        /// <summary>
        /// Since I refuse to ASM edit the logic behind the Configuration Menu, this code handles that.
        /// The first iteration was dirty as hell and had many, MANY bugs within it.
        /// I hope this rewrite will alleviate that.
        /// </summary>
        public static void HandleConfiguration()
        {
            var _configPoint = Hypervisor.Read<ulong>(Variables.PINT_ConfigMenu);
            var _configBitwise = Hypervisor.Read<Locals.CONFIG>(Variables.ADDR_Config);

            var _mainMenuType = Hypervisor.Read<byte>(Variables.ADDR_MenuType);
            var _subMenuType = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);
            var _mainMenuTiming = Hypervisor.Read<int>(Variables.ADDR_MenuType + 0x5C);
            var _subPrepareType = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType + 0x04);

            var _checkReFinedData = Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0xE000) != 0xFFFF && Hypervisor.Read<uint>(Variables.ADDR_SaveData + 0xE000) != 0xFFFFFFFF;

            var _isMenuOkay = _mainMenuType == 0x08 && (_subMenuType == 0x24 || _subPrepareType == 0x24);

            // If we are not in the Title Screen:
            if (!Variables.IS_TITLE)
            {
                if (!_checkReFinedData)
                {
                    Terminal.Log("Re:Fined Specific data portion is corrupt! Resetting!", 2);
                    Hypervisor.Write(Variables.ADDR_SaveData + 0xE000, new byte[0xF00]);
                }

                // If the first time config initialization was not done, and also the Title Screen did not initialize either;
                if (!DEBOUNCE[0x05] && !DEBOUNCE[0x08])
                {
                    // Read the settings bitwise. This is to prevent an immediate hot-reload on game load.
                    Terminal.Log("New Save Data has been loaded! Initializing the config variable.", 0);

                    // Suspend to make timing irrelevant.
                    AxaInterface.Suspend(false);

                    // Fetch and set the Re:Fined related settings.
                    Locals.MUSIC_MODE = (_configBitwise & Locals.CONFIG.BACKGROUND_MUSIC) == Locals.CONFIG.BACKGROUND_MUSIC;
                    Locals.ENEMY_MODE = (_configBitwise & Locals.CONFIG.HEARTLESS_PALETTE) == Locals.CONFIG.HEARTLESS_PALETTE;
                    Locals.CONTROLLER_MODE = (_configBitwise & Locals.CONFIG.PROMPT_CONTROLLER) == Locals.CONFIG.PROMPT_CONTROLLER;

                    Locals.AUDIO_MODE = (_configBitwise & Locals.CONFIG.AUDIO_PRIMARY) == Locals.CONFIG.AUDIO_PRIMARY ? 0x01 : 
                                       ((_configBitwise & Locals.CONFIG.AUDIO_SECONDARY) == Locals.CONFIG.AUDIO_SECONDARY ? 0x02 : 0x00);

                    Locals.SAVE_MODE = (_configBitwise & Locals.CONFIG.AUTOSAVE_INDICATOR) == Locals.CONFIG.AUTOSAVE_INDICATOR ? 0x00 : 
                                      ((_configBitwise & Locals.CONFIG.AUTOSAVE_SILENT) == Locals.CONFIG.AUTOSAVE_SILENT ? 0x01 : 0x02);

                    // Check for the Quadratum Config and set the byte if enabled.
                    var _vladConfig = Hypervisor.Read<byte>(Variables.ADDR_Config + 0x03);

                    Hypervisor.Write(Variables.ADDR_CommandMenu, _vladConfig == 0x01 ? "field2d/%s/qd0command.2dd" : "field2d/%s/zz0command.2dd");
                    Hypervisor.Write(Variables.ADDR_CommandMenu + 0x20, _vladConfig == 0x01 ? "qd0command.2dd" : "zz0command.2dd");

                    SUB_AUDIO_MEMORY = Hypervisor.Read<byte>(Variables.ADDR_Config + 0x02);

                    // Resume progress.
                    AxaInterface.Resume(false);

                    // Note the settings.
                    SETTING_MEMORY = _configBitwise;
                    DEBOUNCE[0x05] = true;
                }

                // If the game is loaded:
                else if (Variables.IS_LOADED)
                {
                    // If the menu is active and config menu is shown:
                    if (_isMenuOkay)
                    {
                        var _audioOffset = Convert.ToInt32(AUDIO_ACTIVE) + Convert.ToInt32(SUB_AUDIO_ACTIVE);
                        var _musicOffset = Convert.ToInt32(AUDIO_ACTIVE) + Convert.ToInt32(SUB_AUDIO_ACTIVE) + Convert.ToInt32(MUSIC_ACTIVE);

                        ADDON_OFFSET = Convert.ToInt32(AUDIO_ACTIVE) + Convert.ToInt32(SUB_AUDIO_ACTIVE) + Convert.ToInt32(MUSIC_ACTIVE) + Convert.ToInt32(ENEMY_ACTIVE);

                        // If the config is not yet applied:
                        if (!DEBOUNCE[0x03])
                        {
                            // Start the setup.
                            Terminal.Log("Config menu has been shown! Setting up the menu...", 1);
                            
                            // Read the setting memory. To prevent it being read everytime we go into the config menu before quitting out of a menu proper, we tie this to a debounce.
                            if (!DEBOUNCE[0x04])
                            {
                                Terminal.Log("First-Time Config Menu! Reading and noting the config variable.", 0);
                                SETTING_MEMORY = _configBitwise;
                                DEBOUNCE[0x04] = true;
                            }

                            // Parse the Extra Audio and Quadratum Byte for handling.
                            SUB_AUDIO_MEMORY = Hypervisor.Read<byte>(Variables.ADDR_Config + 0x02);
                            var _vladConfig = Hypervisor.Read<byte>(Variables.ADDR_Config + 0x03);

                            // Detect all installed addons.
                            byte _audioAddonValue = 0xFF;

                            if (AUDIO_ACTIVE)
                                _audioAddonValue = (byte)(((_configBitwise & Locals.CONFIG.AUDIO_PRIMARY) == Locals.CONFIG.AUDIO_PRIMARY) ? 0x01 :
                                                          ((_configBitwise & Locals.CONFIG.AUDIO_SECONDARY) == Locals.CONFIG.AUDIO_SECONDARY) ? 0x02 : 0x00);

                            // Disassemble the bitwise to a byte array, adding the Difficulty Byte at the end.
                            Terminal.Log("Disassembling and writing the config variable to the menu.", 1);

                            var _configArray = new List<byte>
                            {
                                (byte)((_configBitwise & Locals.CONFIG.FIELD_CAM) == Locals.CONFIG.FIELD_CAM ? 0x01 : 0x00),
                                (byte)((_configBitwise & Locals.CONFIG.RIGHT_STICK) == Locals.CONFIG.RIGHT_STICK ? 0x01 : 0x00),
                                (byte)((_configBitwise & Locals.CONFIG.CAMERA_H) == Locals.CONFIG.CAMERA_H ? 0x01 : 0x00),
                                (byte)((_configBitwise & Locals.CONFIG.CAMERA_V) == Locals.CONFIG.CAMERA_V ? 0x01 : 0x00),
                                (byte)((_configBitwise & Locals.CONFIG.SUMMON_PARTIAL) == Locals.CONFIG.SUMMON_PARTIAL ? 0x00 :
                                        ((_configBitwise & Locals.CONFIG.SUMMON_FULL) == Locals.CONFIG.SUMMON_FULL ? 0x01 : 0x02)),
                                (byte)((_configBitwise & Locals.CONFIG.NAVI_MAP) == Locals.CONFIG.NAVI_MAP ? 0x00 : 0x01),
                                (byte)((_configBitwise & Locals.CONFIG.AUTOSAVE_INDICATOR) == Locals.CONFIG.AUTOSAVE_INDICATOR ? 0x00 :
                                        ((_configBitwise & Locals.CONFIG.AUTOSAVE_SILENT) == Locals.CONFIG.AUTOSAVE_SILENT ? 0x01 : 0x02)),
                                (byte)((_configBitwise & Locals.CONFIG.PROMPT_CONTROLLER) == Locals.CONFIG.PROMPT_CONTROLLER ? 0x00 : 0x01),
                                (byte)((_configBitwise & Locals.CONFIG.VIBRATION) == Locals.CONFIG.VIBRATION ? 0x00 : 0x01),
                                (byte)(_vladConfig == 0x01 ? 0x02 :
                                        (_configBitwise.HasFlag(Locals.CONFIG.COMMAND_KH1) ? 0x01 : 0x00)),
                                Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x2498)
                            };

                            // Add all detected Addon Options.
                            if (_audioAddonValue != 0xFF)
                            {
                                _configArray.Insert(0x08, _audioAddonValue);
                                AUDIO_ACTIVE = true;
                            }
                             
                            // If the Sub-Languages Menu is set:
                            if (Locals.AUDIO_SUB_CONFIG != null && !Locals.AUDIO_SUB_ONLY)
                            {
                                // If sub-languages are enabled, but not active:
                                if (_audioAddonValue == 0x02 && !SUB_AUDIO_ACTIVE)
                                {
                                    // Activate the sub-languages menu.
                                    Locals.MAIN_CONFIG.Children.Insert(0x09, Locals.AUDIO_SUB_CONFIG);
                                    SUB_AUDIO_ACTIVE = true;
                                }

                                // If they are active, but disabled:
                                else if (_audioAddonValue != 0x02 && SUB_AUDIO_ACTIVE)
                                {
                                    // Deactivate the sub-languages menu.
                                    Locals.MAIN_CONFIG.Children.Remove(Locals.AUDIO_SUB_CONFIG);
                                    SUB_AUDIO_ACTIVE = false;
                                }

                                // Insert the config into the array.
                                _configArray.Insert(0x09, (byte) (SUB_AUDIO_ACTIVE ? SUB_AUDIO_MEMORY : 0xFF));
                            }

                            if (MUSIC_ACTIVE)
                                _configArray.Insert(0x08 + _audioOffset, (byte)((_configBitwise & Locals.CONFIG.BACKGROUND_MUSIC) == Locals.CONFIG.BACKGROUND_MUSIC ? 0x00 : 0x01));

                            if (ENEMY_ACTIVE)
                                _configArray.Insert(0x08 + _musicOffset, (byte)((_configBitwise & Locals.CONFIG.HEARTLESS_PALETTE) == Locals.CONFIG.HEARTLESS_PALETTE ? 0x00 : 0x01));

                            // Remove all the 0xFF from the array, as they denote invalid settings.
                            _configArray.RemoveAll(x => x == 0xFF);

                            // Write the settings array.
                            Hypervisor.Write(_configPoint, _configArray.ToArray(), true);
                          
                            // If the config menu is completely up:
                            if (_mainMenuTiming == 0xAF20)
                            {
                                Hypervisor.DeleteInstruction(MENUSELECT_OFFSET + 0x46, 0x04);

                                // Get the variables for page calculation.
                                var _pointPage = Hypervisor.GetPointer64(Variables.PINT_SubMenuOptionSelect, [0x12]);
                                var _pointCurrent = Hypervisor.GetPointer64(Variables.PINT_SubMenuOptionSelect, [0x00]);
                                var _pointMaximum = Hypervisor.GetPointer64(Variables.PINT_SubMenuOptionSelect, [0x16]);

                                // Write the maximum amount of settings.
                                Hypervisor.Write(_pointMaximum, (byte)_configArray.Count, true);

                                // Do the page calculation.
                                byte _pageAmount = (byte)(_configArray.Count - 0x09);
                                byte _pageCurrent = Hypervisor.Read<byte>(_pointPage, true);
                                byte _indexCurrent = Hypervisor.Read<byte>(_pointCurrent, true);

                                // Write the page data.
                                if (_pageCurrent >= _pageAmount)
                                {
                                    Hypervisor.Write(_pointPage, _pageAmount, true);
                                    Hypervisor.Write(_pointCurrent, _indexCurrent + (_pageCurrent - _pageAmount), true);
                                }

                                // Update the config list forcefully.
                                Variables.SharpHook[FUNC_CONFIGUPDATELIST].Execute();
                                Variables.SharpHook[FUNC_CONFIGUPDATEACTIVE].Execute();

                                Hypervisor.Write<byte>(MENUSELECT_OFFSET + 0x46, [0x66, 0x89, 0x53, 0x02]);
                            }

                            // Declare the config menu has been set-up!
                            Terminal.Log("Config menu has been setup successfully!", 0);
                            DEBOUNCE[0x03] = true;
                        }

                        // We only want to set out config variable once the menu has been fully realized. Otherwise, a total wipe can happen.
                        if (_mainMenuTiming == 0xAF20)
                        {
                            // While in the menu, fetch the status of it.
                            var _fetchMenu = Hypervisor.Read<byte>(_configPoint, Locals.MAIN_CONFIG.Children.Count, true);

                            var _configDifficulty = Hypervisor.Read<byte>(_configPoint + (ulong)ADDON_OFFSET + 0x0A, true);
                            var _realDifficulty = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x2498);

                            if (_configDifficulty != _realDifficulty)
                            {
                                Terminal.Log("Config Difficulty does not match the Real Difficulty! Adjusting...", 1);
                                Hypervisor.Write(_configPoint + (ulong)ADDON_OFFSET + 0x0A, _realDifficulty, true);

                                Variables.SharpHook[FUNC_CONFIGUPDATELIST].Execute();
                                Variables.SharpHook[FUNC_CONFIGUPDATEACTIVE].Execute();
                            }

                            // Construct the bitwise of the current state of the menu.
                            var _configCurrent = (_fetchMenu[0x00] == 0x01 ? Locals.CONFIG.FIELD_CAM : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x01] == 0x01 ? Locals.CONFIG.RIGHT_STICK : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x02] == 0x01 ? Locals.CONFIG.CAMERA_H : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x03] == 0x01 ? Locals.CONFIG.CAMERA_V : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x04] == 0x00 ? Locals.CONFIG.SUMMON_PARTIAL : 
                                                    (_fetchMenu[0x04] == 0x01 ? Locals.CONFIG.SUMMON_FULL : Locals.CONFIG.OFF)) |
                                                    (_fetchMenu[0x05] == 0x00 ? Locals.CONFIG.NAVI_MAP : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x06] == 0x00 ? Locals.CONFIG.AUTOSAVE_INDICATOR : 
                                                    (_fetchMenu[0x06] == 0x01 ? Locals.CONFIG.AUTOSAVE_SILENT : Locals.CONFIG.OFF)) |
                                                    (_fetchMenu[0x07] == 0x00 ? Locals.CONFIG.PROMPT_CONTROLLER : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x08 + ADDON_OFFSET] == 0x00 ? Locals.CONFIG.VIBRATION : Locals.CONFIG.OFF) |
                                                    (_fetchMenu[0x09 + ADDON_OFFSET] >= 0x01 ? Locals.CONFIG.COMMAND_KH1 : Locals.CONFIG.OFF);

                            // Account for the music add-on.
                            if (MUSIC_ACTIVE)
                                _configCurrent = _configCurrent | (_fetchMenu[0x08 + _audioOffset] == 0x00 ? Locals.CONFIG.BACKGROUND_MUSIC : Locals.CONFIG.OFF);

                            // Account for the enemy add-on.
                            if (ENEMY_ACTIVE)
                                _configCurrent = _configCurrent | (_fetchMenu[0x08 + _musicOffset] == 0x00 ? Locals.CONFIG.HEARTLESS_PALETTE : Locals.CONFIG.OFF);

                            if (AUDIO_ACTIVE)
                            {
                                // Account for the audio add-ons.
                                _configCurrent = _configCurrent | (_fetchMenu[0x08] == 0x01 ? Locals.CONFIG.AUDIO_PRIMARY : (_fetchMenu[0x08] == 0x02 ? Locals.CONFIG.AUDIO_SECONDARY : Locals.CONFIG.OFF));

                                // See if the menu should be refreshed due to sub-language toggling.
                                var _shouldRefresh = (_fetchMenu[0x08] == 0x02 && !SUB_AUDIO_ACTIVE) || (_fetchMenu[0x08] != 0x02 && SUB_AUDIO_ACTIVE);

                                // If it should be:
                                if (Locals.AUDIO_SUB_CONFIG != null && _shouldRefresh)
                                {
                                    // Toggle the debounce, write the config, and return out. This forces a reset.
                                    DEBOUNCE[0x03] = false;
                                    Hypervisor.Write(Variables.ADDR_Config, _configCurrent);

                                    return;
                                }

                                // Write the Sub-Audio Byte.
                                if (_fetchMenu[0x08] == 0x02 && SUB_AUDIO_ACTIVE)
                                    Hypervisor.Write(Variables.ADDR_Config + 0x02, _fetchMenu[0x09]);
                            }

                            // Set the Re:Fined specific variables.
                            Locals.MUSIC_MODE = (_configCurrent & Locals.CONFIG.BACKGROUND_MUSIC) == Locals.CONFIG.BACKGROUND_MUSIC;
                            Locals.ENEMY_MODE = (_configCurrent & Locals.CONFIG.HEARTLESS_PALETTE) == Locals.CONFIG.HEARTLESS_PALETTE;
                            Locals.CONTROLLER_MODE = (_configCurrent & Locals.CONFIG.PROMPT_CONTROLLER) == Locals.CONFIG.PROMPT_CONTROLLER;

                            Locals.AUDIO_MODE = (_configCurrent & Locals.CONFIG.AUDIO_PRIMARY) == Locals.CONFIG.AUDIO_PRIMARY ? 0x01 :
                                               ((_configCurrent & Locals.CONFIG.AUDIO_SECONDARY) == Locals.CONFIG.AUDIO_SECONDARY ? 0x02 : 0x00);

                            Locals.SAVE_MODE = (_configCurrent & Locals.CONFIG.AUTOSAVE_INDICATOR) == Locals.CONFIG.AUTOSAVE_INDICATOR ? 0x00 :
                                              ((_configCurrent & Locals.CONFIG.AUTOSAVE_SILENT) == Locals.CONFIG.AUTOSAVE_SILENT ? 0x01 : 0x02);

                            // Check for the Quadratum Config and set the byte if enabled.
                            Hypervisor.Write(Variables.ADDR_Config + 0x03, (byte)(_fetchMenu[0x09 + ADDON_OFFSET] == 0x02 ? 0x01 : 0x00));

                            // Switching mechanism to reroute all Config Menu calls to Kingdom Hearts I to Quadratum and vice-versa,
                            // depending on what we need and when we need it.

                            Hypervisor.Write(Variables.ADDR_CommandMenu, _fetchMenu[0x09 + ADDON_OFFSET] == 0x02 ? "field2d/%s/qd0command.2dd" : "field2d/%s/zz0command.2dd");
                            Hypervisor.Write(Variables.ADDR_CommandMenu + 0x20, _fetchMenu[0x09 + ADDON_OFFSET] == 0x02 ? "qd0command.2dd" : "zz0command.2dd");

                            // Signify the command menus have changed. This will always cause a Command Menu reset but ¯\_(ツ)_/¯.
                            Hypervisor.Write(Variables.ADDR_CommandFlag, 0x02);

                            // Write the bitwise.
                            Hypervisor.Write(Variables.ADDR_Config, _configCurrent);
                        }

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

                        var _layoutPointer = Hypervisor.Read<ulong>(Variables.PINT_Camp2LD);
                        var _configSelected = Hypervisor.GetPointer64(Variables.PINT_SubMenuOptionSelect, [0x12]);

                        var _pageIndex = Hypervisor.Read<byte>(_configSelected, true);

                        var _pageCount = (Locals.MAIN_CONFIG.Children.Count - 0x09);
                        var _pageFactor = 0x18 * _pageCount;

                        var _pageOffset = (_pageFactor / _pageCount) * _pageIndex;

                        Hypervisor.Write(_layoutPointer + 0x21498 - 0x1D0, 0x64 + _pageOffset, true);
                        Hypervisor.Write(_layoutPointer + 0x2149C - 0x1D0, 0x64 + _pageOffset, true);

                        Hypervisor.Write(_layoutPointer + 0x21528 - 0x1D0, 0x64 + _pageOffset, true);
                        Hypervisor.Write(_layoutPointer + 0x2152C - 0x1D0, 0x64 + _pageOffset, true);

                        Hypervisor.Write(_layoutPointer + 0x215B8 - 0x1D0, 0x64 - (_pageFactor + 1) + _pageOffset, true);
                        Hypervisor.Write(_layoutPointer + 0x215BC - 0x1D0, 0x64 - (_pageFactor + 1) + _pageOffset, true);

                        Hypervisor.Write(_layoutPointer + 0x21568 - 0x1D0, (0xC0 - _pageFactor) * 0.01F, true);
                        Hypervisor.Write(_layoutPointer + 0x2156C - 0x1D0, (0xC0 - _pageFactor) * 0.01F, true);

                        // ======================================================================================= //
                    }

                    // If the config menu is closed, we forget the config is written.
                    else if (!_isMenuOkay && DEBOUNCE[0x03])
                    {
                        Terminal.Log("The config menu is closed! Flushing menu memory.", 0);
                        DEBOUNCE[0x03] = false;
                        DEBOUNCE[0x04] = false;
                    }

                    var _audioChanged = (_configBitwise & Locals.CONFIG.AUDIO_PRIMARY) != (SETTING_MEMORY & Locals.CONFIG.AUDIO_PRIMARY) ||
                                        (_configBitwise & Locals.CONFIG.AUDIO_SECONDARY) != (SETTING_MEMORY & Locals.CONFIG.AUDIO_SECONDARY) ||
                                        (Hypervisor.Read<byte>(Variables.ADDR_Config + 0x02) != SUB_AUDIO_MEMORY);

                    var _musicChanged = (_configBitwise & Locals.CONFIG.BACKGROUND_MUSIC) != (SETTING_MEMORY & Locals.CONFIG.BACKGROUND_MUSIC);
                    var _enemyChanged = (_configBitwise & Locals.CONFIG.HEARTLESS_PALETTE) != (SETTING_MEMORY & Locals.CONFIG.HEARTLESS_PALETTE);

                    // If the menu is closed, the game is not paused, and the settings have changed:
                    
                    if (_mainMenuType == 0xFF && !Variables.IS_PAUSED && (_audioChanged || _musicChanged || _enemyChanged))
                    {
                        Locals.CONFIG_RELOADING = true;

                        Terminal.Log("Critical Config change detected! Starting hot-reload protocol!", 1);

                        // Make a mental note of the current area.
                        var _currentArea = Hypervisor.Read<byte>(Variables.ADDR_Area, 0x10);

                        // Construct a new area memory to temporarily warp to.
                        Terminal.Log("Constructing the warp-to point!", 1);
                        var _warpArea = new byte[0x10];
                        _warpArea[0x00] = (byte)(Hypervisor.Read<byte>(Variables.ADDR_Area) == 0x02 ? 0x06 : 0x02);

                        // Write the target area in question.
                        Hypervisor.Write(Variables.ADDR_Area, _warpArea);

                        // Kill the music, and execute the warp.
                        Terminal.Log("Initiating warp to a dummy world!", 1);
                        Sound.KillBGM();
                        Shisutemu.ExecuteWarp(Variables.ADDR_Area, Locals.IS_FLASHBANG ? Shisutemu.FADE_TYPE.WHITEOUT : Shisutemu.FADE_TYPE.BLACKOUT);

                        // Wait until the fade has been completed.
                        Terminal.Log("Waiting for the fade to fully realize.", 1);
                        while (Hypervisor.Read<byte>(Variables.ADDR_FadeValue) != 0x80) { }

                        // Suspend the game to prepare for the warpback.
                        Terminal.Log("ENTERING CRITICAL SECTION! Suspending the game!", 1);
                        AxaInterface.Suspend();

                        Terminal.Log("Noting and killing the fade-in instruction within the warp function.", 1);

                        // Make a note of the Fade-In instruction.
                        var _musicAddress = AREAINIT_OFFSET + 0x1B;
                        var _fadeAddress = (ulong)(Shisutemu.FUNC_MAPJUMP + 0x9F);
                        var _fadeInstruction = Hypervisor.Read<byte>(_fadeAddress, 0x05);
                        var _musicInstruction = Hypervisor.Read<byte>(_musicAddress, 0x02);

                        // Kill said instruction.
                        Hypervisor.DeleteInstruction(_fadeAddress, 0x05);
                        Hypervisor.DeleteInstruction(_musicAddress, 0x02);

                        // Write the current area back and warp to it.
                        Terminal.Log("Initiating the warp back to the starting point.", 1);
                        Hypervisor.Write(Variables.ADDR_Area, _currentArea);
                        Shisutemu.ExecuteWarp(Variables.ADDR_Area, Locals.IS_FLASHBANG ? Shisutemu.FADE_TYPE.WHITEOUT : Shisutemu.FADE_TYPE.BLACKOUT);

                        if (Locals.ENEMY_LOADING)
                        {
                            Terminal.Log("Vanilla Heartless is taking a bit to load. Waiting until it's complete.", 1);
                            
                            // Do fuck all until it's done.
                            while (Locals.ENEMY_LOADING) 
                            { }
                        }

                        // Everything critical has been completed. Resume the game.
                        Terminal.Log("CRITICAL SECTION has been completed! Resuming the game.", 1);
                        AxaInterface.Resume();

                        // Restore the instructions noted earlier.
                        Terminal.Log("Restoring the deleted instruction!", 1);
                        Hypervisor.Write(_fadeAddress, _fadeInstruction);
                        
                        while (Hypervisor.Read<byte>(Variables.ADDR_FadeValue) > 0x40) { }

                        // Note that the settings are final.
                        Terminal.Log("Hot-reload has been successfully completed! Thanks for choosing Topaz' Reality Inc.!", 0);
                        SETTING_MEMORY = _configBitwise;
                        Hypervisor.Write(_musicAddress, _musicInstruction);
                        SUB_AUDIO_MEMORY = Hypervisor.Read<byte>(Variables.ADDR_Config + 0x02);

                        Locals.CONFIG_RELOADING = false;
                    }
                }
            }

            // If we are on the title screen, and the config variable has been initialized, destroy it.
            else if (Variables.IS_TITLE && DEBOUNCE[0x05])
            {
                Terminal.Log("Title Screen detected! Resetting config variable initialization!", 0);
                DEBOUNCE[0x05] = false;
            }
        }

        /// <summary>
        /// The logic behind Magic Sorting.
        /// I do not know what I was thinking implementing this to the game.
        /// This code has been the bane of my existence.
        /// </summary>
        public static void SortMagic()
        {
            // Read the pointer to the current Child Menu.
            var _menuPointer = Hypervisor.Read<ulong>(Variables.PINT_ChildMenu);

            // Seek the levels of all Magic currently obtained.
            var _magicOne = Hypervisor.Read<uint>(Variables.ADDR_MagicLV1);
            var _magicTwo = Hypervisor.Read<ushort>(Variables.ADDR_MagicLV2);

            // Read the Magic Sort data in the Save File, and fetches the first magic from the array.
            var _readMagic = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE500, 0x0C);
            
            var _firstMagic = BitConverter.ToUInt16(_readMagic, 0x00);
            var _checkCommand = Hypervisor.Read<short>(Variables.ADDR_MagicCommands);

            // If the Command Control instructions are not in memory, fetch them.
            // We do this because we want to not be able to control the Command Menu when we activate sorting.
            // Why? Because the sort will do it by itself due to a bunch of things I do not understand.
            // To be honest; I do not want to anyways.
            if (CONTROL_INSTRUCTIONS == null)
            {
                CONTROL_INSTRUCTIONS =
                [
                    Hypervisor.Read<byte>(CONTROL_OFFSET + MAGIC_OFFSET[0], 0x03),
                    Hypervisor.Read<byte>(CONTROL_OFFSET + MAGIC_OFFSET[1], 0x03),
                    Hypervisor.Read<byte>(CONTROL_OFFSET + MAGIC_OFFSET[2], 0x03),
                    Hypervisor.Read<byte>(CONTROL_OFFSET + MAGIC_OFFSET[3], 0x03),
                    Hypervisor.Read<byte>(CONTROL_OFFSET + MAGIC_OFFSET[4], 0x03),
                ];
            }

            if (MAGIC_STORE != null)
            {
                if (Variables.IS_TITLE)
                {
                    Terminal.Log("Return to Title Screen detected! Resetting Magic Sort variables...", 1);

                    MAGIC_STORE = null;

                    MAGIC_LV1 = 0x00;
                    MAGIC_LV2 = 0x00;

                    _firstMagic = 0x00;
                    _readMagic = new byte[0x0C];

                    DEBOUNCE[0x00] = false;
                    DEBOUNCE[0x0B] = false;
                }

                if (!Variables.IS_LOADED && (MAGIC_LV1 != _magicOne || MAGIC_LV2 != _magicTwo))
                {
                    Terminal.Log("Gained a new magic and changed rooms! Resetting Magic Sort memory...", 1);

                    MAGIC_STORE = null;

                    MAGIC_LV1 = _magicOne;
                    MAGIC_LV2 = _magicTwo;

                    _firstMagic = 0x00;
                    _readMagic = new byte[0x0C];

                    DEBOUNCE[0x00] = false;
                    DEBOUNCE[0x0B] = false;
                    DEBOUNCE[0x0E] = true;

                    Hypervisor.Write(Variables.ADDR_SaveData + 0xE500, _readMagic);
                }
            }

            // If a Child Menu is up, and the room is loaded;
            if (_menuPointer != 0x00 && Variables.IS_LOADED)
            {
                // Read the menu type.
                var _menuRead = Hypervisor.Read<byte>(_menuPointer, true);

                // If it is indeed the Magic Menu;
                if (_menuRead == 0x01)
                {
                    if (MAGIC_STORE == null && DEBOUNCE[0x0E])
                    {
                        Terminal.Log("Sub-Menu Opened after Magic Gain, reconstructing sort memory...", 1);

                        MAGIC_STORE = Hypervisor.Read<byte>(Variables.ADDR_MagicCommands, 0x0C);
                        Hypervisor.Write(Variables.ADDR_SaveData + 0xE500, MAGIC_STORE);
                        DEBOUNCE[0x0E] = false;

                        return;
                    }

                    if (_firstMagic != 0x00 && DEBOUNCE[0x0E])
                    {
                        Terminal.Log("Roomchange w/ existing sort, applying sort memory...", 1);

                        Hypervisor.Write<byte>(Variables.ADDR_MagicCommands, _readMagic);
                        MAGIC_STORE = _readMagic;

                        DEBOUNCE[0x0E] = false;
                    }

                    // Read the index that we are on.
                    var _magicIndex = Hypervisor.Read<byte>(Variables.ADDR_MagicIndex);
                    var _magicMax = Hypervisor.Read<byte>(_menuPointer + 0x10, true);

                    // Flow for Digital and Analog controls.
                    var _pressDigital = Variables.IS_PRESSED(Variables.BUTTON.L2 | Variables.BUTTON.UP) ? 0x01 : (Variables.IS_PRESSED(Variables.BUTTON.L2 | Variables.BUTTON.DOWN) ? 0x02 : 0x00);
                    var _pressAnalog = Variables.IS_PRESSED(Variables.BUTTON.R2) && Variables.IS_PRESSED(Variables.ANALOG.R_UP) ? 0x01 : (Variables.IS_PRESSED(Variables.BUTTON.R2) && Variables.IS_PRESSED(Variables.ANALOG.R_DOWN) ? 0x02 : 0x00);

                    // Depending on the RIGHT STICK config, set which one of the input methdos we will observe.
                    var _inputCheck = (Hypervisor.Read<Variables.CONFIG>(Variables.ADDR_Config) & Variables.CONFIG.RIGHT_STICK) == Variables.CONFIG.RIGHT_STICK ? _pressAnalog : _pressDigital;
                    var _triggerCheck = (Hypervisor.Read<Variables.CONFIG>(Variables.ADDR_Config) & Variables.CONFIG.RIGHT_STICK) == Variables.CONFIG.RIGHT_STICK ? Variables.IS_PRESSED(Variables.BUTTON.R2) : Variables.IS_PRESSED(Variables.BUTTON.L2);

                    // Read the first byte of the control instructions to avoid constant writing.
                    var _insCheck = Hypervisor.Read<byte>(CONTROL_OFFSET + 0x1B1);

                    // If the controls match with the conditions;
                    if (_triggerCheck && _insCheck != 0x90)
                    {
                        Terminal.Log("The control key has been held! Cutting all control instructions...", 1);

                        // Delete all the control instructions.
                        foreach (var _off in MAGIC_OFFSET)
                            Hypervisor.DeleteInstruction(CONTROL_OFFSET + _off, 0x03);
                    }

                    // If that's no longer the case;
                    else if (!_triggerCheck && _insCheck == 0x90)
                    {
                        Terminal.Log("The control key has been let go! Restoring all control instructions...", 1);

                        // Restore them.
                        for (int i = 0; i < MAGIC_OFFSET.Length; i++)
                            Hypervisor.Write(CONTROL_OFFSET + MAGIC_OFFSET[i], CONTROL_INSTRUCTIONS[i]);
                    }

                    // If there is any input we expect and the Debounce is not set;
                    if (!DEBOUNCE[0x00] && _inputCheck != 0x00)
                    {
                        // Set the Debounce.
                        DEBOUNCE[0x00] = true;

                        // Fetch the pointer to the magic command, and set the bounds of the menu.
                        var _magicPointer = (0x02 * _magicIndex);
                        var _magicBounds = _magicPointer + (_inputCheck == 0x01 ? -0x02 : 0x02);

                        // Fetch the subject and the target Magic for the sort.
                        var _subjectMagic = Hypervisor.Read<ushort>(Variables.ADDR_MagicCommands + (ulong)_magicPointer);
                        var _targetMagic = _magicBounds >= 0 ? Hypervisor.Read<ushort>(Variables.ADDR_MagicCommands + (ulong)_magicBounds) : (ushort)0x0000;

                        // If there is a target Magic;
                        if (_targetMagic != 0x0000)
                        {
                            Terminal.Log("Moving Magic ID 0x" + _subjectMagic.ToString("X4") + " through the list!", 1);

                            // Make the swap in the command memory.
                            Hypervisor.Write(Variables.ADDR_MagicCommands + (ulong)_magicPointer, _targetMagic);
                            Hypervisor.Write(Variables.ADDR_MagicCommands + (ulong)_magicBounds, _subjectMagic);

                            // Make the swap in index memory.
                            Hypervisor.Write(Variables.ADDR_MagicIndex, _magicIndex + (_inputCheck == 0x01 ? -0x01 : 0x01));
                            Hypervisor.Write(Variables.ADDR_MagicIndex + 0x04, _subjectMagic);
                            
                            // Save the changes to memory and save data.
                            MAGIC_STORE = Hypervisor.Read<byte>(Variables.ADDR_MagicCommands, _magicMax * 0x02);
                            Hypervisor.Write(Variables.ADDR_SaveData + 0xE500, MAGIC_STORE);

                            Terminal.Log("Magic moved successfully!", 0);
                        }
                    }

                    else if (DEBOUNCE[0x00] && _inputCheck == 0x00)
                        DEBOUNCE[0x00] = false;
                }
            }

            // If that's not the case; Restore the control functions.
            else
            {
                for (int i = 0; i < MAGIC_OFFSET.Length; i++)
                    Hypervisor.Write(CONTROL_OFFSET + MAGIC_OFFSET[i], CONTROL_INSTRUCTIONS[i]);

                if (!Variables.IS_LOADED)
                    DEBOUNCE[0x0E] = true;
            }
        }

        /// <summary>
        /// Allows forms to be shortcutted.
        /// I am not going to comment this code because it's all an ASM Hack.
        /// The only reason it's even here is for toggleability.
        /// </summary>
        public static void AllowFormShorcuts()
        {
            var _parityCheck = Hypervisor.Read<byte>(ICON_OFFSET + 0x1A);

            if (Locals.FORM_SHORTCUT && _parityCheck != 0xEB)
            {
                var _copyInst = Hypervisor.Read<byte>(ICON_OFFSET + 0x1D, 0x19);

                Hypervisor.Write<byte>(ICON_OFFSET + 0x1A, [0xEB, 0x19]);

                Hypervisor.Write(ICON_OFFSET + 0x1C, _copyInst);

                Hypervisor.Write<byte>(ICON_OFFSET + 0x35, [0x3C, 0x0B, 0x75, 0x02, 0xB0]);
                Hypervisor.Write<byte>(ICON_OFFSET + 0x3B, [0x88, 0x47, 0x01, 0xEB, 0xDC]);

                Hypervisor.Write<byte>(ICON_OFFSET + 0x3A, 0xCE);

                Hypervisor.Write<byte>(LIST_OFFSET + 0x18E, [0xEB, 0xAA]);
                Hypervisor.Write<byte>(LIST_OFFSET + 0x138, [0xEB, 0x4E, 0x90, 0x90]);
                Hypervisor.Write<byte>(LIST_OFFSET + 0x188, [0x81, 0xCB, 0x00, 0x00, 0x24, 0x00]);

                Hypervisor.Write<byte>(EQUIP_OFFSET + 0x33, [0x80, 0xF9, 0x15, 0x74, 0xF2]);
                Hypervisor.Write<byte>(EQUIP_OFFSET + 0x16, [0xEB, 0x1B, 0x90, 0x90, 0x90, 0x90, 0x90]);
                Hypervisor.Write<byte>(EQUIP_OFFSET + 0x38, [0x31, 0xC0, 0x48, 0x83, 0xC4, 0x28, 0xC3]);

                Hypervisor.Write<byte>(LIST_OFFSET + 0x12C, [0xEB, 0x45, 0x90, 0x90]);
                Hypervisor.Write<byte>(LIST_OFFSET + 0x173, [0x81, 0xC3, 0x00, 0x00, 0x20, 0x00, 0xEB, 0xB5]);

                Hypervisor.DeleteInstruction(CATEGORY_OFFSET + 0x4AF, 0x02);
            }
        }

        /// <summary>
        /// Handles the crowns on Sora's HUD.
        /// This function sometimes seem to not wanna work. I do not know why, but I don't like that.
        /// This function blocks execution, and thus requires it's own task like a sissy little baby. 
        /// </summary>
        public static void ApplyCrowns()
        {
            // Read the character to ensure it's actually Sora.
            var _charRead = Hypervisor.Read<short>(Variables.ADDR_CurrentCharacter);

            // Read the current file formatter to ensure execution with other language packs.
            var _fileFormatter = Hypervisor.ReadString(Variables.DATA_PAXPath + 0x10);

            // Read the current form.
            var _formRead = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x3524);

            // Read the current world.
            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);

            // Read the inventory for all the proofs and calculate the crown.
            var _crownRead = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x36B2, 0x03);
            var _crownSum = _crownRead[0] + _crownRead[1] + _crownRead[2];

            // If the game is on the Title Screen, or isn't loaded, or the crown has updated, or the form has changed;
            if ((Variables.IS_TITLE || !Variables.IS_LOADED || PAST_FORM != _formRead || PAST_CROWN != _crownSum) && SORA_PORTRAIT != 0x00)
            {
                Terminal.Log("Resetting all Crown Data due to a change!", 1);

                // Reset the memory.
                SORA_PORTRAIT = 0x00;
                PAST_FORM = _formRead;
                PAST_CROWN = _crownSum;
            }

            PARSE_BEGIN:

            // If the game is loaded, not on the title screen or a cutscene, not in the World Map, and the Crown Engine hasn't done SHIT:
            if (!Variables.IS_TITLE && Variables.IS_LOADED && !Variables.IS_MOVIE && !Variables.IS_CUTSCENE && _worldRead >= 0x02 && _worldRead != 0x0F && SORA_PORTRAIT == 0x00 && _crownSum > 0x00 && !Variables.IS_PAUSED && _charRead == 0x0054)
            {
                // Fetch all of Sora's Assets from the Buffer. I should be able to simplify this but eh, I can't bother right this moment.
                var _fileName = _fileFormatter.Replace("%s", Locals.SORA_FORMS[_formRead]);

                Terminal.Log("Trying to fetch from the Cache Buffer: \"" + _fileName + "\".", 1);
                SORA_PORTRAIT = IO.FindFileBuffer(_fileName);

                if (SORA_PORTRAIT == 0x00)
                {
                    var _fileString = "obj/" + Locals.SORA_FORMS[_formRead] + ".a.us";

                    Terminal.Log("Trying to fetch from the Cache Buffer: \"" + _fileString + "\".", 1);
                    SORA_PORTRAIT = IO.FindFileBuffer(_fileString);

                    if (SORA_PORTRAIT == 0x00)
                    {
                        foreach (var _lang in Locals.LOADED_LANGS)
                        {
                            if (_lang.Value != 0x00)
                            {
                                var _otherString = _fileString.Replace("us", _lang.Key.ToLower());
                                Terminal.Log("Trying to fetch from the Cache Buffer: \"" + _fileString + "\".", 1);
                                SORA_PORTRAIT = IO.FindFileBuffer(_fileString);

                                if (SORA_PORTRAIT != 0x00)
                                    break;
                            }
                        }

                        if (SORA_PORTRAIT == 0x00)
                        {
                            Terminal.Log("Could not catch from the Cache Buffer! Trying again!", 1);
                            goto PARSE_BEGIN;
                        }
                    }
                }

                Terminal.Log("Caught file in Cache Buffer: 0x" + SORA_PORTRAIT.ToString("X16"), 0);

                // If the Cache Buffer address is 0x00, reset the whole thing.
                if (SORA_PORTRAIT == 0x00)
                {
                    Terminal.Log("Cache Buffer Catch is invalid! Trying again!", 1);
                    goto PARSE_BEGIN;
                }

                // Read and calculate the pointers for all of Sora's assets in memory.
                var _soraPointer = SORA_PORTRAIT != 0x00 ? Hypervisor.Read<ulong>(SORA_PORTRAIT + 0x58, true) : 0x00;

                Terminal.Log("Caught pointer to the PAX File: 0x" + _soraPointer.ToString("X16"), 0);

                // If the PAX Pointer is 0x00, reset the whole thing.
                if (_soraPointer == 0x00)
                {
                    Terminal.Log("PAX Pointer is invalid! Trying again!", 1);
                    goto PARSE_BEGIN;
                }

                Terminal.Log("Fetched Crown Data: 0x" + _crownSum.ToString("X2"), 0);

                // If the pointer does exist;
                if (_soraPointer != 0x00)
                {
                    Terminal.Log("Trying to navigate to the FACE file in Portrait Data...", 1);

                    Thread.Sleep(100);

                    // Calculate the BAR Offset and Sora's FACE file in said BAR.
                    ulong _readSizes = 0x40;
                    var _barOffset = Hypervisor.Read<uint>(_soraPointer + 0x08, true);

                    // We read the sizes. Why? Sometimes, the fuckin' offsets are B O R K E D !
                    for (uint i = 0; i < 2; i++)
                        _readSizes += Hypervisor.Read<uint>(_soraPointer + 0x1CU + (0x10 * i), true);

                    var _checkFaceSize = Hypervisor.Read<uint>(_soraPointer + 0x3C, true);

                    if (_checkFaceSize != 0x04EC && _checkFaceSize != 0x0000)
                    {
                        Terminal.Log("The FACE size is NOT what's expected! Aborting!", 2);
                        return;
                    }

                    // If the BAR ain't initialized yet, reset the whole thing.
                    if (_barOffset == 0x00 || _readSizes == 0x00)
                        goto PARSE_BEGIN;

                    // Check the FACE.
                    Terminal.Log("Checking for FACE file at: 0x" + (_soraPointer + 0x24).ToString("X16"), 1);
                    var _faceCheck = Hypervisor.Read<uint>(_soraPointer + 0x24, true);

                    // If the FACE isn't the FACE, reset the whole thing.
                    if (_faceCheck != 0x65636166)
                    {
                        Terminal.Log("FACE is invalid! Trying again!", 1);
                        goto PARSE_BEGIN;
                    }

                    Terminal.Log("FACE found! Applying the Crown...", 1);

                    // Calculate the TOP and BOTTOM values of the FACE SEQD according to the crown.
                    var _topValue = 0x00 + _crownSum * 0x5A;
                    var _bottomValue = 0x5D + _crownSum * 0x5A;

                    var _topAddress = _soraPointer + _readSizes + 0x38;
                    var _bottomAddress = _soraPointer + _readSizes + 0x40;

                    // If the bottom coords are 0x00, reset the whole thing.
                    if (Hypervisor.Read<short>(_bottomAddress, true) == 0x00)
                        goto PARSE_BEGIN;

                    // Apply the values for all 3 indexes.
                    for (uint i = 0; i < 3; i++)
                    {
                        Hypervisor.Write(_topAddress + (0x2C * i), _topValue, true);
                        Hypervisor.Write(_bottomAddress + (0x2C * i), _bottomValue, true);
                    }

                    Terminal.Log("Crown Coordinates applied successfully!", 0);
                }
            }
        }

        /// <summary>
        /// Autosaving logic. Not much to say about this on the KH2 side of things.
        /// </summary>
        public static void HandleAutosave()
        {
            // Read the world and the room.
            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _fadeCheck = Hypervisor.Read<byte>(Variables.ADDR_FadeValue);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
            var _menuCheck = Hypervisor.Read<byte>(Variables.ADDR_MenuFlag);

            var _gaugeTypePointer = Hypervisor.GetPointer64(Variables.PINT_PlayerGauge, [0x90]);
            var _gaugeExistsPointer = Hypervisor.GetPointer64(Variables.PINT_PlayerGauge, [0x88, 0x00]);

            var _readGaugeType = Hypervisor.Read<ushort>(_gaugeTypePointer, true);

            var _blacklistCheck =
                (_worldCheck == 0x0F) ||
                (_worldCheck == 0x08 && _roomCheck == 0x03) ||
                (_worldCheck == 0x0C && _roomCheck == 0x02) ||
                (_worldCheck == 0x02 && _roomCheck <= 0x01) ||
                (_worldCheck == 0x04 && _roomCheck == 0x10) ||
                (_worldCheck == 0x12 && _roomCheck >= 0x13 && _roomCheck <= 0x1D);

            if (!DEBOUNCE[0x12])
            {
                var _systemInfo = Hypervisor.GetPointer64(Variables.PINT_SaveInformation, [0x10, 0x10]);
                var _systemString = Hypervisor.ReadString(_systemInfo, true);
                
                if (_systemString != "BISLPM-66675FM-SYS")
                {
                    Terminal.Log("Save File is not initialized! Initializing...", 1);

                    var _currDate = DateTime.Now.ToUniversalTime();
                    var _unix = new DateTime(1970, 1, 1);
                    var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

                    Hypervisor.Write(_systemInfo, "BISLPM-66675FM-SYS", true);

                    Hypervisor.Write(_systemInfo + 0x40, _writeDate, true);
                    Hypervisor.Write(_systemInfo + 0x48, _writeDate, true);

                    Hypervisor.Write(_systemInfo + 0x50, 0x400, true);
                }

                DEBOUNCE[0x12] = true;
            }

            // If we are not in the Title Screen, the game is loaded, and the room we are at is NOT blacklisted;
            if (!Variables.IS_TITLE && Variables.IS_LOADED && !_blacklistCheck)
            {
                if (!DEBOUNCE[0x0F])
                {
                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;
                    SAVE_ITERATOR = 0;
                    DEBOUNCE[0x0F] = true;
                    return;
                }

                var _statusCheck = Locals.SAVE_MODE != 0x02 && !Variables.IS_PAUSED && Variables.IS_LOADED && !Locals.CONFIG_RELOADING && !DEBOUNCE[0x01] && !DEBOUNCE[0x02] && DEBOUNCE[0x0A] && _worldCheck >= 0x02 && _fadeCheck == 0x00;

                if (_statusCheck)
                {
                    var _saveableCheck = !Variables.IS_CUTSCENE && !Variables.IS_EVENT && !Variables.IS_MOVIE && Variables.BATTLE_MODE == Variables.BATTLE_TYPE.PEACEFUL && _menuCheck == 0x00 && _gaugeExistsPointer != 0x00 && (_readGaugeType == 0x1A || _readGaugeType == 0x3A);

                    if (!_saveableCheck)
                        return;

                    // Check if the world we are in is not acknowledged;
                    if (SAVE_WORLD != _worldCheck)
                    {
                        // Allow saving and reset the iterator.
                        DEBOUNCE[0x02] = true;
                        SAVE_ITERATOR = 0;
                        Terminal.Log("Initiating an autosave...", 1);
                    }

                    // Check if the room we are in is not acknowledged;
                    else if (SAVE_ROOM != _roomCheck)
                    {
                        // Increase the iterator.
                        SAVE_ITERATOR++;

                        // If the room changed 3 times;
                        if (SAVE_ITERATOR == 3)
                        {
                            // Allow saving and reset the iterator.
                            DEBOUNCE[0x02] = true;
                            SAVE_ITERATOR = 0;
                            Terminal.Log("Initiating an autosave...", 1);
                        }
                    }

                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;

                    DEBOUNCE[0x06] = false;
                }

                if (Variables.IS_LOADED && !Variables.IS_TITLE && !DEBOUNCE[0x0A])
                    DEBOUNCE[0x0A] = true;

                else if (Variables.IS_TITLE && DEBOUNCE[0x0A])
                {
                    DEBOUNCE[0x0A] = false;

                    SAVE_ROOM = _roomCheck;
                    SAVE_WORLD = _worldCheck;
                }

                if (DEBOUNCE[0x02])
                {
                    // Prepare the CRC32 Table we will use.
                    IEnumerable<uint> _checksumTable()
                    {
                        for (var x = 0; ; x++)
                        {
                            var r = x << 24;

                            for (var j = 0; j < 0xff; j++)
                                r = r << 1 ^ (r < 0 ? 0x4C11DB7 : 0);

                            yield return (uint)r;
                        }
                    }

                START_SAVE:

                    // Prepare the strings.
                    var _saveID = "66675FM-" + (Locals.SAVE_SLOT - SAVE_CHECK_OFFSET).ToString("00");
                    var _saveName = "BISLPM-66675FM-" + (Locals.SAVE_SLOT - SAVE_CHECK_OFFSET).ToString("00");
                    var _savePath = Hypervisor.ReadString(Hypervisor.GetPointer64(Variables.PINT_SaveInformation, [0x40]), true) + (Locals.PLATFORM == "EPIC" ? "\\KHIIFM.png" : "\\KHIIFM_WW.png");

                    // Calculate the Unix Date.
                    var _currDate = DateTime.Now.ToUniversalTime();
                    var _unix = new DateTime(1970, 1, 1);
                    var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

                    // Prepare the variables for Save Info.
                    var _saveSlot = 0;
                    var _saveInfoLength = 0x158;
                    var _saveDataLength = 0x10FC0;

                    var _saveInfoStartRAM = Hypervisor.GetPointer64(Variables.PINT_SaveInformation, [0x10, 0x168]);
                    var _saveDataStartRAM = Hypervisor.GetPointer64(Variables.PINT_SaveInformation, [0x10, 0x19630]);

                    var _saveInfoStartFILE = 0x1C8;
                    var _saveDataStartFILE = 0x19690;

                    // Read the world.
                    var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);

                    // Read the save from RAM.

                    Hypervisor.Write(Variables.ADDR_SaveData + 0x10, 0xFFFFFFFF);
                    var _saveData = Hypervisor.Read<byte>(Variables.ADDR_SaveData, _saveDataLength);

                    // Read the save slot.
                    var _saveSlotRAM = Hypervisor.Read<byte>(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);

                    // Seek out the physical slot of the save to make.
                    while (_saveSlotRAM[0] != 0x00 && !Encoding.Default.GetString(_saveSlotRAM).Contains(_saveID))
                    {
                        _saveSlot++;
                        _saveSlotRAM = Hypervisor.Read<byte>(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);
                    }

                    // Calculate the checksums.
                    var _magicArray = _saveData.Take(0x08).ToArray();
                    var _dataArray = _saveData.Skip(0x0C).ToArray();

                    uint _checkMagic()
                    {
                        uint[] array = _checksumTable().Take(0x100).ToArray();
                        var _checksum = uint.MaxValue;

                        for (var i = 0; i < 0x08; i++)
                            _checksum = array[(_checksum >> 24) ^ _magicArray[i]] ^ (_checksum << 8);

                        return _checksum ^ uint.MaxValue;
                    }

                    uint _checkData()
                    {
                        uint[] array = _checksumTable().Take(0x100).ToArray();
                        var _checksum = _checkMagic() ^ uint.MaxValue;

                        for (var i = 0; i < _dataArray.Length; i++)
                            _checksum = array[(_checksum >> 24) ^ _dataArray[i]] ^ (_checksum << 8);

                        return _checksum ^ uint.MaxValue;
                    }

                    #region RAM Save
                    // Fetch the address for the save info.
                    var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot);
                    var _saveDataAddrRAM = _saveDataStartRAM + (ulong)(_saveDataLength * _saveSlot);

                    if (_saveSlotRAM[0] != 0x00)
                    {
                        var _autoSaveCheck = Hypervisor.Read<uint>(_saveDataAddrRAM + 0x10, true);

                        if (_autoSaveCheck != 0xFFFFFFFF)
                        {
                            var _slotCalc = Locals.SAVE_SLOT + 0x01 - SAVE_CHECK_OFFSET;

                            if (SAVE_CHECK_OFFSET <= 0x00 || _slotCalc >= 0x00)
                            {
                                SAVE_CHECK_OFFSET++;
                                Terminal.Log("Parsed Slot is NOT an Autosave! Offsetting to slot #" + (Locals.SAVE_SLOT + 0x01 - SAVE_CHECK_OFFSET).ToString("00"), 1);
                                goto START_SAVE;
                            }

                            else if (_slotCalc < 0x00)
                            {
                                Terminal.Log("No valid slots to create an Autosave found! Terminating...", 2);
                                SAVE_CHECK_OFFSET = 0x00;
                                DEBOUNCE[0x02] = false;
                                return;
                            }
                        }
                    }

                    // Write out the save information.
                    Hypervisor.Write(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

                    // Write the date in which the save was made.
                    Hypervisor.Write(_saveInfoAddrRAM + 0x40, _writeDate, true);
                    Hypervisor.Write(_saveInfoAddrRAM + 0x48, _writeDate, true);

                    // Write the length of the save.
                    Hypervisor.Write(_saveInfoAddrRAM + 0x50, _saveDataLength, true);

                    // Write the header.
                    Hypervisor.Write(_saveDataAddrRAM, Encoding.ASCII.GetBytes("KH2J"), true);
                    Hypervisor.Write<uint>(_saveDataAddrRAM + 0x04, 0x3A, true);

                    // Write the checksum.
                    Hypervisor.Write(_saveDataAddrRAM + 0x08, _checkData(), true);

                    // Write, the save.
                    Hypervisor.Write(_saveDataAddrRAM + 0x0C, _dataArray, true);

                    // Write the Autosave indicator.
                    Hypervisor.Write(Variables.ADDR_SaveData + 0x10, 0x00);
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
                        _write.Write(_checkData());

                        // Write, the save.
                        _stream.Position = _saveDataAddr + 0x0C;
                        _write.Write(_dataArray);
                    }
                    #endregion

                    // If the autosave mode is "Informative";
                    if (Locals.SAVE_MODE == 0x00)
                    {
                        // Inform the user of the successful save.
                        Hypervisor.Write<byte>(INFORMATION_OFFSET + 0x029, 0x28); Thread.Sleep(5);
                        Popup.PopupInformation(0x5702);

                        Hypervisor.Write<byte>(INFORMATION_OFFSET + 0x029, 0x22);
                    }

                    SAVE_CHECK_OFFSET = 0x00;
                    DEBOUNCE[0x02] = false;
                }
            }

            else if (Variables.IS_TITLE && DEBOUNCE[0x0F])
                DEBOUNCE[0x0F] = false;
        }

        /// <summary>
        /// Handles the core logic within Retribution and Absolution.
        /// This was a pain just FYI.
        /// </summary>
        public static void RetributionLogic()
        {
            if (Locals.DETECTED_ABSOLUTION || Locals.DETECTED_RETRIBUTION)
            {
                if (!Variables.IS_TITLE)
                {
                    var _readMenu = Hypervisor.Read<byte>(Variables.ADDR_MenuFlag);
                    var _readSubMenu = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);

                    var _readPicture = Hypervisor.Read<ushort>(Variables.ADDR_LoadedPicture);
                    var _seekItem = _readPicture == 420 ? 0x300 : 0x301;

                    var _isMenuGood = _readMenu == 0x01 && (_readSubMenu == 0x02 || _readSubMenu == 0x05);
                    var _isStatusGood = _readPicture == 420 || _readPicture == 421;

                    if (!RETRIBUTION_INIT)
                    {
                        Terminal.Log("Fetching WENT from 03system.bin...", 1);

                        var _systemAddress = Hypervisor.Read<ulong>(Variables.PINT_SystemFile);
                        var _wentFile = IO.FetchBARSubfile(_systemAddress, "went", true);

                        if (_wentFile == 0x00)
                            return;

                        Terminal.Log("Writing the values for Retribution and Absolution to WENT...", 1);

                        Hypervisor.Write(_wentFile + 0x017C, [0x0A03, 0x0A07], true);
                        Hypervisor.Write(_wentFile + 0x030C, [0x0A04, 0x0A08], true);
                        Hypervisor.Write(_wentFile + 0x0900, [0x0A05, 0x0A09], true);
                        Hypervisor.Write(_wentFile + 0x0A90, [0x0A06, 0x0A0A], true);

                        Terminal.Log("Special Weapon initialization is complete!", 0);

                        RETRIBUTION_INIT = true;
                    }

                    if (PARAMS_ALL.Count == 0x00)
                    {
                        Terminal.Log("Fetching Keyblade Information to use with Addon Keyblades...", 0);

                        for (int i = 0; i < Locals.KEY_DICTIONARY.Count; i++)
                        {
                            var _keyData = Locals.KEY_DICTIONARY.ElementAt(i);

                            var _getParam = Shisutemu.FetchItemParams(_keyData.Value);
                            var _paramID = Hypervisor.Read<ushort>(_getParam, true);

                            PARAMS_ALL.Add(_paramID);
                        }
                    }

                    if (PARAM_ABSOLUTION == 0xFFFF)
                    {
                        ABSOLUTION_RAM = Shisutemu.FetchItem(0x0301);
                        RETRIBUTION_RAM = Shisutemu.FetchItem(0x0300);

                        PARAM_ABSOLUTION = Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0xE804);
                        PARAM_RETRIBUTION = Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0xE800);

                        if (PARAM_ABSOLUTION == 0x0000)
                        {
                            Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0xE804, 0x50);
                            PARAM_ABSOLUTION = 0x50;
                        }

                        if (PARAM_RETRIBUTION == 0x0000)
                        {
                            Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0xE800, 0x50);
                            PARAM_RETRIBUTION = 0x50;
                        }

                        Hypervisor.Write(RETRIBUTION_RAM + 0x06, PARAM_RETRIBUTION, true);
                        Hypervisor.Write(ABSOLUTION_RAM + 0x06, PARAM_ABSOLUTION, true);
                    }

                    var _driveBitwise = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x36C0);

                    var _equipArray = new ushort[]
                    {
                        Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x24F0),
                        (_driveBitwise & 0x02) == 0x02 ? Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x32F4) : (ushort)0x00,
                        (_driveBitwise & 0x40) == 0x40 ? Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x33D4) : (ushort)0x00,
                        (_driveBitwise & 0x10) == 0x10 ? Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x339C) : (ushort)0x00
                    };

                    var _absolutionCheck = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE311) == 0x00 && _equipArray.FirstOrDefault(x => x == 0x0301) == 0x00;
                    var _retributionCheck = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE310) == 0x00 && _equipArray.FirstOrDefault(x => x == 0x0300) == 0x00;

                    if (_absolutionCheck && Locals.DETECTED_ABSOLUTION)
                    {
                        Terminal.Log("Giving Absolution...", 0);
                        Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0xE311, 0x01);
                    }

                    if (_retributionCheck && Locals.DETECTED_RETRIBUTION)
                    {
                        Terminal.Log("Giving Retribution...", 0);
                        Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0xE310, 0x01);
                    }

                    if (Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE311) != 0x00 && _equipArray.FirstOrDefault(x => x == 0x0301) != 0x00)
                    {
                        Terminal.Log("Absolution exists in more than one place! Adjusting...", 0);
                        Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0xE311, 0x00);
                    }

                    if (Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE310) != 0x00 && _equipArray.FirstOrDefault(x => x == 0x0300) != 0x00)
                    {
                        Terminal.Log("Retribution exists in more than one place! Adjusting...", 0);
                        Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0xE310, 0x00);
                    }

                    if (INDEX_ABSOLUTION == 0xFF)
                        INDEX_ABSOLUTION = PARAMS_ALL.IndexOf(PARAM_ABSOLUTION);

                    if (INDEX_RETRIBUTION == 0xFF)
                        INDEX_RETRIBUTION = PARAMS_ALL.IndexOf(PARAM_RETRIBUTION);

                    if (_readMenu == 0x01 && _readSubMenu != 0xFF && PARAMS_LIST.Count == 0)
                    {
                        Terminal.Log("Creating the Key Inventory Array...", 0);

                        for (int i = 0; i < Locals.KEY_DICTIONARY.Count; i++)
                        {
                            var _keyData = Locals.KEY_DICTIONARY.ElementAt(i);
                            var _slotCheck = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x3580 + _keyData.Key);
                            var _tryFetch = _equipArray.FirstOrDefault(x => x == _keyData.Value);

                            if (_slotCheck == 0x01)
                                PARAMS_LIST.Add(PARAMS_ALL[i]);

                            else if (_tryFetch != 0x00)
                                PARAMS_LIST.Add(PARAMS_ALL[i]);
                        }
                    }

                    else if (_readMenu == 0x00 && PARAMS_LIST.Count > 0)
                    {
                        PARAMS_LIST.Clear();
                        DEBOUNCE[2] = false;
                    }

                    if (_isMenuGood)
                    {
                        if (_isStatusGood && Variables.IS_PRESSED(Variables.BUTTON.TRIANGLE) && !DEBOUNCE[0x0D])
                        {
                            if (PARAMS_LIST.Count == 0x01)
                                Sound.PlaySFX(0x05);

                            else
                            {
                                var _currParam = _seekItem == 0x0300 ? INDEX_RETRIBUTION : INDEX_ABSOLUTION;
                                Terminal.Log("Cycling the mimic of " + (_seekItem == 0x0300 ? "Retribution." : "Absolution."), 0);

                                _currParam++;

                                if (_currParam >= PARAMS_LIST.Count)
                                    _currParam = 0x00;

                                Hypervisor.Write((_seekItem == 0x0300 ? RETRIBUTION_RAM : ABSOLUTION_RAM) + 0x06, PARAMS_LIST[_currParam], true);

                                if (_seekItem == 0x0300)
                                {
                                    Hypervisor.Write(Variables.ADDR_SaveData + 0xE800, PARAMS_LIST[_currParam]);
                                    PARAM_RETRIBUTION = PARAMS_LIST[_currParam];
                                    INDEX_RETRIBUTION = _currParam;
                                }

                                else
                                {
                                    Hypervisor.Write(Variables.ADDR_SaveData + 0xE804, PARAMS_LIST[_currParam]);
                                    PARAM_ABSOLUTION = PARAMS_LIST[_currParam];
                                    INDEX_ABSOLUTION = _currParam;
                                }

                                Sound.PlaySFX(0x02);
                                Variables.SharpHook[FUNC_ITEMSELECTUPDATE].Execute();
                            }

                            DEBOUNCE[0x0D] = true;
                        }

                        else if ((!_isStatusGood || !Variables.IS_PRESSED(Variables.BUTTON.TRIANGLE)) && DEBOUNCE[0x0D])
                            DEBOUNCE[0x0D] = false;
                    }
                }

                else
                {
                    PARAM_ABSOLUTION = 0xFFFF;
                    PARAM_RETRIBUTION = 0xFFFF;
                }
            }
        }
    }
}
