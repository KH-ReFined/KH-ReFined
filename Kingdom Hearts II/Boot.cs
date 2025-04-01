using KH2FML;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using DiscordClient = Discord.Discord;

namespace ReFined.KH2
{
    public static class Boot
    {
        public static void Initialization()
        {
            if (File.Exists("reFined.cfg"))
            {
                using (var _tomlFile = File.OpenText("reFined.cfg"))
                {
                    var _tomlTable = TOML.Parse(_tomlFile);

                    var _generalTable = _tomlTable["General"];

                    Locals.IS_LITE = _generalTable["liteMode"].AsBoolean;
                    Locals.RPC_ENABLED = _generalTable["discordRPC"].AsBoolean;
                    Locals.IS_FLASHBANG = _generalTable["isFlashbang"].AsBoolean;

                    Locals.RESET_COMBO = Variables.BUTTON.NONE;

                    foreach (TomlString _button in _generalTable["resetCombo"].AsArray)
                    {
                        Variables.BUTTON _buttonOut;
                        Enum.TryParse(_button, out _buttonOut);
                        Locals.RESET_COMBO |= _buttonOut;
                    }

                    var _accessTable = _tomlTable["Accessibility"];

                    Locals.AUTOATTACK = _accessTable["autoAttack"].AsBoolean;

                    var _gameTable = _tomlTable["Kingdom Hearts II"];

                    Locals.FORM_SHORTCUT = _gameTable["driveShortcuts"].AsBoolean;
                    Locals.RESET_PROMPT = _gameTable["resetPrompt"].AsBoolean;
                    Locals.RETRY_FIRST = _gameTable["deathPrompt"].AsString == "retry" ? true : false;

                    var _limitTable = _gameTable["limitShortcuts"].AsArray;
                    Continuous.LIMIT_SHORT = new short[4];

                    for (int i = 0; i < _limitTable.ChildrenCount; i++)
                        Continuous.LIMIT_SHORT[i] = Locals.DICTIONARY_LMT[_limitTable[i].AsString];
                }
            }

            else
            {
                var _outDefault = new string[]
                {
                    "[General]",
                    "liteMode = false",
                    "discordRPC = true",
                    "resetCombo = [\"L2\", \"R2\"]",
                    "isFlashbang = true",
                    "quickBoot = false",
                    "",
                    "[Accessibility]",
                    "autoAttack = false",
                    "",
                    "[\"Chain of Memories\"]",
                    "alternativeControls = false",
                    "zeroEXP = false",
                    "autoSave = true",
                    "",
                    "[\"Kingdom Hearts II\"]",
                    "driveShortcuts = true",
                    "resetPrompt = true",
                    "deathPrompt = \"retry\"",
                    "limitShortcuts = [\"sonic\", \"arcanum\", \"raid\", \"ragnarok\"]",
                };

                File.WriteAllLines("reFined.cfg", _outDefault);
            }

            Terminal.Log("Welcome to " + (Locals.IS_LITE ? "Re:Freshed" : "Re:Fined") + " v3.00!", 0);

            Terminal.Log("Trying to attach to the process...", 1);
            var _fetchProc = Process.GetProcessesByName("KINGDOM HEARTS II FINAL MIX")[0];
            Terminal.Log("Attached to the process successfully!", 0);

            Terminal.Log("Initializing Kingdom Hearts II - Flexible Modding Library...", 1);
            Entry.Initialize(_fetchProc);

            Terminal.Log("Initializing the Hypervisor...", 1);
            Hypervisor.AttachProcess(_fetchProc);

            Terminal.Log("Initializing Discord GameSDK...", 1);
            Locals.DiscordClient = new DiscordClient(833511404274974740, 0x01);

            Terminal.Log("Trying to find all the hot-fix functions...", 1);

            Demand.FUNC_UPDATESHORTCUTS = Hypervisor.FindSignature<IntPtr>("48 83 EC 28 E8 97 F9 FF FF 48 83 C4 28 E9 DE 02 00 00");
            Demand.FADESET_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 57 48 83 EC 20 E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? F3 48 0F 2C F8");

            Continuous.SAVE_OFFSET = Hypervisor.FindSignature<ulong>("40 55 53 48 8D 6C 24 B1 48 81 EC C8 00 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 3F 48 8B D9 E8 ?? ?? ?? ??");
            Continuous.PROMPT_OFFSET = Hypervisor.FindSignature<ulong>("C7 05 ?? ?? ?? ?? 01 00 00 00 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ??");
            Continuous.LIMITER_OFFSET = Hypervisor.FindSignature<ulong>("89 1D ?? ?? ?? ?? 0F 57 C0 89 05 ?? ?? ?? ?? 0F 57 D2 83 FF 08");

            Critical.LIST_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 57 48 83 EC 20 33 DB 48 89 6C 24 30 41 8B F8 48 8B E9");
            Critical.ICON_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 57 48 83 EC 20 48 8B FA 41 0F B6 D9 41 0F B6 D0 E8 ?? ?? ?? ??");
            Critical.EQUIP_OFFSET = Hypervisor.FindSignature<ulong>("48 83 EC 28 E8 ?? ?? ?? ?? 0F B6 48 02 84 C9 74 19");
            Critical.CATEGORY_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 54 41 55 41 56 41 57 48 81 EC 90 01 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 80 01 00 00 33 F6 89 4C 24 28 85 C9 48 8D 05 ?? ?? ?? ??");

            Critical.ASPECT_OFFSET = Hypervisor.FindSignature<ulong>("48 83 EC 28 0F 10 41 48 4C 8B C9 4C 8B D2 F3 0F 10 25 ?? ?? ?? ?? 0F 57 ED 0F 11 02");
            Critical.AREAINIT_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 10 56 48 83 EC 30 33 F6 48 89 7C 24 40 F6 05 B4 53 96 00 01 48 8B F9 74 28 8B 1D A1 53 96 00 8B CB");

            Critical.CONTROL_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 55 41 56 41 57 48 83 EC 20 4C 8B 41 08 48 8B D9");
            Critical.INFORMATION_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 57 48 81 EC D0 00 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 C0 00 00 00 48 8B DA 48 8B F9");
            
            Critical.FUNC_ITEMSELECTUPDATE = Hypervisor.FindSignature<IntPtr>("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 40 45 32 E4 E8 ?? ?? ?? ??");
            Critical.FUNC_CONFIGUPDATELIST = Hypervisor.FindSignature<IntPtr>("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 58 E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 4C 8B F8 E8 ?? ?? ?? ??");

            Terminal.Log("All functions found successfully!", 0);

            if (IO.GetFileSize("mirageArena.bin") != 0x00)
            {
                Terminal.Log("Mirage Arena Detected! Adjusting Discord RPC accordingly...", 0);
                Locals.MIRAGE_ARENA = true;
            }

            if (IO.GetFileSize("techniColor.bin") != 0x00)
            {
                Terminal.Log("Technicolor Detected! Disabling Summon Accommodation...", 0);
                Locals.TECHNICOLOR = true;
            }

            Terminal.Log("Initializing all the menus...", 1);

            Locals.MAIN_INTRO = new Intro();
            Locals.MAIN_CONFIG = new Config();
            Locals.MAIN_CONTINUE = new Continue();

            Terminal.Log("Menus initialized successfully!", 0);

            Terminal.Log("Adding base options to all menus...", 1);

            var _saveConfig = new Config.Entry(3, 0x5704, [0x5705, 0x5707, 0x5709], [0x5706, 0x5708, 0x570A]);
            var _promptConfig = new Config.Entry(2, 0x5722, [0x5723, 0x5725], [0x5724, 0x5726]);

            var _saveIntro = new Intro.Entry(3, 0x5733, 0xFFFF, [0x5705, 0x5707, 0x5709], [0x5706, 0x5708, 0x570A]);
            var _promptIntro = new Intro.Entry(2, 0x5737, 0xFFFF, [0x5723, 0x5725], [0x5724, 0x5726]);

            Locals.MAIN_CONFIG.Children.Insert(0x06, _promptConfig);
            Locals.MAIN_CONFIG.Children.Insert(0x06, _saveConfig);

            Locals.MAIN_INTRO.Children.Add(_saveIntro);
            Locals.MAIN_INTRO.Children.Add(_promptIntro);

            Terminal.Log("Options added successfully!", 0);

            if (!Locals.IS_LITE)
            {
                if (IO.GetFileSize("se/activateCombat.win32.scd") != 0x00)
                {
                    Terminal.Log("Battle Start/Stop Lines Detected! Enabling functions...", 0);
                    Locals.COMBAT_MODE = true;
                }

                if (IO.GetFileSize("obj/H_ZZ020_DC.mdlx") != 0x00)
                {
                    Terminal.Log("Garden of Assemblage Rndomizer Detected! Adjusting some functionality accordingly...", 0);
                    Locals.RANDOMIZER = true;
                }

                if (IO.GetFileSize("obj/W_EX010_RF.mdlx") != 0x00)
                {
                    Terminal.Log("Ticket to Retribution detected! Enabling necessary flags...", 0);
                    Locals.DETECTED_RETRIBUTION = true;
                }

                if (IO.GetFileSize("obj/W_EX010_RX.mdlx") != 0x00)
                {
                    Terminal.Log("Ticket to Absolution detected! Enabling necessary flags...", 0);
                    Locals.DETECTED_ABSOLUTION = true;
                }

                Terminal.Log("Adding in the option for the Quadratum Menu...", 1);

                var _configCount = Locals.MAIN_CONFIG.Children.Count;
                var _commandConfig = new Config.Entry(3, 0xB71C, [0xB734, 0xB735, 0x572C], [0xB736, 0xB737, 0x572D]);

                Locals.MAIN_CONFIG.Children.RemoveAt(_configCount - 0x02);
                Locals.MAIN_CONFIG.Children.Insert(_configCount - 0x02, _commandConfig);

                Terminal.Log("Trying to detect language packs...", 1);

                Locals.LOADED_LANGS = new Dictionary<string, int>()
                {
                    { "JP", IO.GetFileSize("voice/jp/battle/tt0_sora.win32.scd") },
                    { "ES", IO.GetFileSize("voice/es/battle/tt0_sora.win32.scd") },
                    { "DE", IO.GetFileSize("voice/de/battle/tt0_sora.win32.scd") },
                    { "BG", IO.GetFileSize("voice/bg/battle/tt0_sora.win32.scd") } 
                };

                var _configButtons = new List<ushort>
                {
                    (ushort) (Locals.LOADED_LANGS["ES"] != 0x00 ? 0x5710 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["DE"] != 0x00 ? 0x5712 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["BG"] != 0x00 ? 0x5714 : 0x0000),
                };

                var _configDescriptions = new List<ushort>
                {
                    (ushort) (Locals.LOADED_LANGS["ES"] != 0x00 ? 0x5711 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["DE"] != 0x00 ? 0x5713 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["BG"] != 0x00 ? 0x5715 : 0x0000),
                };

                _configButtons.RemoveAll(x => x == 0x0000);
                _configDescriptions.RemoveAll(x => x == 0x0000);

                if (_configButtons.Count > 0)
                    Terminal.Log("Detected MULTI5 Languages! Adjusting the menus accordingly...", 1);

                var _audioSubConfig = new Config.Entry(0, 0x572B, [], []);
                var _audioMainConfig = new Config.Entry(1, 0x570B, [0x570C], [0x570D]);

                var _audioSubIntro = new Intro.Entry(0, 0x5734, 0xFFFF, [], []);
                var _audioMainIntro = new Intro.Entry(1, 0x5734, 0xFFFF, [0x570C], [0x570D]);

                foreach (var _button in _configButtons)
                {
                    _audioSubIntro.Buttons.Add(_button);
                    _audioSubConfig.Buttons.Add(_button);
                }

                foreach (var _description in _configDescriptions)
                {
                    _audioSubIntro.Descriptions.Add(_description);
                    _audioSubConfig.Descriptions.Add(_description);
                }

                if (Locals.LOADED_LANGS["JP"] != 0x00)
                {
                    Terminal.Log("Detected Japanese Langauge Pack! Adjusting the menus accordingly...", 1);

                    _audioMainConfig.Buttons.Add(0x570E);
                    _audioMainConfig.Descriptions.Add(0x570F);

                    _audioMainIntro.Buttons.Add(0x570E);
                    _audioMainIntro.Descriptions.Add(0x570F);

                    if (_configButtons.Count >= 1)
                    {
                        var _buttonAdd = (ushort)(_configButtons.Count == 0x01 ? _configButtons[0] : 0x5716);
                        var _descriptionAdd = (ushort)(_configDescriptions.Count == 0x01 ? _configDescriptions[0] : 0x5717);

                        _audioMainConfig.Buttons.Add(_buttonAdd);
                        _audioMainConfig.Descriptions.Add(_descriptionAdd);

                        _audioMainIntro.Buttons.Add(_buttonAdd);
                        _audioMainIntro.Descriptions.Add(_descriptionAdd);
                    }
                }

                else if (_configButtons.Count >= 1) 
                {
                    if (_configButtons.Count < 0x03)
                        Locals.AUDIO_SUB_ONLY = true;

                    ushort[] _buttonAdd = Locals.AUDIO_SUB_ONLY ? [_configButtons[0], (ushort)(_configButtons.Count == 0x01 ? 0xFFFF : _configButtons[1])] : [0x5716];
                    ushort[] _descriptionAdd = Locals.AUDIO_SUB_ONLY ? [_configDescriptions[0], (ushort)(_configDescriptions.Count == 0x01 ? 0xFFFF : _configDescriptions[1])] : [0x5717];

                    foreach (var _button in _buttonAdd)
                    {
                        if (_button != 0xFFFF)
                        {
                            _audioMainIntro.Buttons.Add(_button);
                            _audioMainConfig.Buttons.Add(_button);
                        }
                    }

                    foreach (var _description in _descriptionAdd)
                    {
                        if (_description != 0xFFFF)
                        {
                            _audioMainIntro.Descriptions.Add(_description);
                            _audioMainConfig.Descriptions.Add(_description);
                        }
                    }
                }

                if (_audioMainConfig.Count > 1)
                {
                    Locals.MAIN_INTRO.Children.Add(_audioMainIntro);
                    Locals.MAIN_CONFIG.Children.Insert(Locals.MAIN_CONFIG.Children.Count - 0x03, _audioMainConfig);

                    Critical.AUDIO_ACTIVE = true;
                }

                if (_audioMainConfig.Buttons.Contains(0x5716))
                {
                    Locals.AUDIO_SUB_INTRO = _audioSubIntro;
                    Locals.AUDIO_SUB_CONFIG = _audioSubConfig;
                }

                Terminal.Log("Trying to detect addons...", 1);

                if (IO.GetFileSize("bgm/ps2md050.win32.scd") != 0x00)
                {
                    Terminal.Log("A Music Pack was located! Adding the options for it...", 1);

                    var _musicConfig = new Config.Entry(2, 0x5718, [0x5719, 0x571B], [0x571A, 0x571C]);
                    var _musicIntro = new Intro.Entry(2, 0x5735, 0x0000, [0x5719, 0x571B], [0x571A, 0x571C]);

                    Locals.MAIN_INTRO.Children.Add(_musicIntro);
                    Locals.MAIN_CONFIG.Children.Insert(Locals.MAIN_CONFIG.Children.Count - 0x03, _musicConfig);

                    Critical.MUSIC_ACTIVE = true;
                }

                if (IO.GetFileSize("obj/V_BB100.mdlx") != 0x00)
                {
                    Terminal.Log("An Enemy Palette Pack was located! Adding the options for it...", 1);

                    var _enemyConfig = new Config.Entry(2, 0x571D, [0x571E, 0x5720], [0x571F, 0x5721]);
                    var _enemyIntro = new Intro.Entry(2, 0x5736, 0x0000, [0x571E, 0x5720], [0x571F, 0x5721]);

                    Locals.MAIN_INTRO.Children.Add(_enemyIntro);
                    Locals.MAIN_CONFIG.Children.Insert(Locals.MAIN_CONFIG.Children.Count - 0x03, _enemyConfig);

                    Critical.ENEMY_ACTIVE = true;
                }

                if (!Locals.RANDOMIZER)
                {
                    Terminal.Log("Adding in the option to skip the prologue...", 1);

                    var _skipIntro = new Intro.Entry(2, 0x5730, 0xFFFF, [0x5738, 0x5739], [0x5731, 0x5732]);
                    Locals.MAIN_INTRO.Children.Add(_skipIntro);
                }

                Terminal.Log("All adjustments have been done successfully!", 0);
            }

            if (Locals.RPC_TEXTS == null)
            {
                Terminal.Log("Parsing all of the Discord RPC related texts...", 1);

                Locals.RPC_TEXTS = new List<string>();
                Locals.MODE_TEXTS = new List<string>();
                Locals.FORM_TEXTS = new List<string>();

                for (short i = 0x5740; i < 0x5745; i++)
                    Locals.RPC_TEXTS.Add(Text.GetStringHuman(i).Replace("/", "|"));

                for (var i = 0; i < 4; i++)
                {
                    short _stringID = (short)(0x3738 + i);

                    if (i == 0x03)
                        _stringID = 0x4E30;

                    Locals.MODE_TEXTS.Add(Text.GetStringHuman(_stringID));
                }

                for (var i = 0; i < 6; i++)
                {
                    short _stringID = (short)(0x01E5 + (i >= 0x02 ? i - 1 : i));

                    if (i == 0x02)
                        _stringID = 0x4E7F;

                    var _formString = Text.GetStringHuman(_stringID);
                    var _splitString = _formString.Split(' ');

                    Locals.FORM_TEXTS.Add(_splitString[0]);
                }

                Terminal.Log("Discord RPC texts have been parsed successfully!", 0);
            }

            Terminal.Log("Initialization completed! Enjoy!", 0);
        }
    }
}
