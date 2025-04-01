using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DiscordClient = Discord.Discord;

namespace ReFined.COM
{
    public static class Boot
    {
        public static ulong FUNC_ISTRGROLLDECKR;
        public static ulong FUNC_ISTRGROLLDECKL;
        public static ulong FUNC_ISDOWNROLLDECKR;
        public static ulong FUNC_ISDOWNROLLDECKL;

        public static ulong[] FUNC_ISTRGLOCKON;

        public static ulong FUNC_ISRTRGLOCKON;
        public static ulong FUNC_ISTRGCANCELSTOCK;
        public static ulong FUNC_ISTRGSHORTCUTCARD;
        public static ulong FUNC_ISTRGSHORTCUTRELOAD;
 
        public static ulong FUNC_EXPDROP;
        public static ulong FUNC_DECKCHECK;
        public static ulong FUNC_CHECKCOPYRIGHT;

        public static void Initialize()
        {
            if (File.Exists("reFined.cfg"))
            {
                using (var _tomlFile = File.OpenText("reFined.cfg"))
                {
                    var _tomlTable = TOML.Parse(_tomlFile);

                    var _generalTable = _tomlTable["General"];

                    Variables.RPC_ACTIVE = _generalTable["discordRPC"].AsBoolean;
                    Variables.QUICK_BOOT = _generalTable["quickBoot"].AsBoolean;

                    Variables.RESET_COMBO = Variables.BUTTON.NONE;

                    foreach (TomlString _button in _generalTable["resetCombo"].AsArray)
                    {
                        Variables.BUTTON _buttonOut;
                        Enum.TryParse(_button, out _buttonOut);
                        Variables.RESET_COMBO |= _buttonOut;
                    }

                    var _gameTable = _tomlTable["Chain of Memories"];

                    Variables.IS_ZEROEXP = _gameTable["zeroEXP"].AsBoolean;
                    Variables.IS_ALTERNATIVE_BUTTONS = _gameTable["alternativeControls"].AsBoolean;
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
                    "",
                    "[\"Kingdom Hearts II\"]",
                    "driveShortcuts = true",
                    "resetPrompt = true",
                    "deathPrompt = \"retry\"",
                    "limitShortcuts = [\"sonic\", \"arcanum\", \"raid\", \"ragnarok\"]",
                };

                File.WriteAllLines("reFined.cfg", _outDefault);
            }

            Terminal.Log("Welcome to Re:Freshed v3.00!", 0);

            Terminal.Log("Trying to attach to the process...", 1);
            var _fetchProc = Process.GetProcessesByName("KINGDOM HEARTS Re_Chain of Memories")[0];
            Terminal.Log("Attached to the process successfully!", 0);

            Terminal.Log("Initializing the Hypervisor...", 1);
            Hypervisor.AttachProcess(_fetchProc);

            Terminal.Log("Initializing Discord GameSDK...", 1);
            Variables.DiscordClient = new DiscordClient(837938492976660481, 0x01);

            Terminal.Log("Trying to find all the hot-fix functions...", 1);

            FUNC_ISTRGROLLDECKR = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 57 48 83 EC 20 8B 39 33 D2 8B 49 3C 83 E7 01 E8 ?? ?? ?? ??");
            FUNC_ISTRGROLLDECKL = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 57 48 83 EC 20 8B 39 33 D2 8B 49 38 83 E7 01 E8 ?? ?? ?? ??");
            FUNC_ISDOWNROLLDECKR = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 8B 49 3C 83 E3 01 0F BA E9 0E E8 ?? ?? ?? ??");
            FUNC_ISDOWNROLLDECKL = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 8B 49 38 83 E3 01 0F BA E9 0C E8 ?? ?? ?? ??");

            FUNC_ISTRGLOCKON = Hypervisor.FindMultiSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 8B 49 44 83 E3 01 E8 ?? ?? ?? ?? 0F AF C3");
            FUNC_ISRTRGLOCKON = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 8B 49 44 83 E3 01 E8 ?? ?? ?? ?? 83 F3 01");
            FUNC_ISTRGCANCELSTOCK = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 8B 49 40 83 E3 01 E8 ?? ?? ?? ??");

            FUNC_ISTRGSHORTCUTCARD = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 B9 00 20 00 00 83 E3 01 E8 ?? ?? ?? ??");
            FUNC_ISTRGSHORTCUTRELOAD = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 8B 19 33 D2 B9 00 80 00 00 83 E3 01 E8 ?? ?? ?? ??");

            FUNC_EXPDROP = Hypervisor.FindSignature<ulong>("4C 8B DC 48 81 EC B8 00 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 50 48 83 B9 10 03 00 00 00");
            FUNC_DECKCHECK = Hypervisor.FindSignature<ulong>("40 56 41 57 48 83 EC 38 48 8B 15 ?? ?? ?? ?? 4C 8B 05 ?? ?? ?? ?? 48 89 5C 24 58 48 89 6C 24 60 8B 42 1C");
            FUNC_CHECKCOPYRIGHT = Hypervisor.FindSignature<ulong>("40 53 48 83 EC 20 48 8B D9 33 D2 33 C9 E8 ?? ?? ?? ?? 85 C0 74 3D 80 BB DC 00 00 00 00 48 89 7C 24 30 48 8D BB 28 01 00 00");

            Hypervisor.Write(FUNC_ISTRGROLLDECKL + 0x1C, 0x8000);
            Hypervisor.Write(FUNC_ISTRGROLLDECKR + 0x1C, 0x2000);

            Hypervisor.Write<byte>(FUNC_ISDOWNROLLDECKL + 0x13, 0x0F);
            Hypervisor.Write<byte>(FUNC_ISDOWNROLLDECKR + 0x13, 0x0D);

            Hypervisor.Write(FUNC_ISTRGSHORTCUTCARD + 0x0B, 0x4000);
            Hypervisor.Write(FUNC_ISTRGSHORTCUTRELOAD + 0x0B, 0x1000);

            Hypervisor.Write<byte>(FUNC_DECKCHECK + 0x127, 0xB8);

            if (Variables.IS_ALTERNATIVE_BUTTONS)
            {
                foreach (var _int in FUNC_ISTRGLOCKON)
                    Hypervisor.Write<byte>(_int + 0x0C, 0x3C);

                Hypervisor.Write<byte>(FUNC_ISRTRGLOCKON + 0x0C, 0x3C);
                Hypervisor.Write<byte>(FUNC_ISTRGCANCELSTOCK + 0x0C, 0x38);

                Hypervisor.Write<byte>(FUNC_ISDOWNROLLDECKL + 0x0C, 0x40);
                Hypervisor.Write<byte>(FUNC_ISDOWNROLLDECKR + 0x0C, 0x44);
            }

            if (Variables.QUICK_BOOT)
                Hypervisor.Write<byte>(FUNC_CHECKCOPYRIGHT + 0x029, 0x73);

            if (Variables.IS_ZEROEXP)
                Hypervisor.Write<byte>(FUNC_EXPDROP + 0x21, [0x90, 0xE9]);

            Terminal.Log("Re:Freshed initialized successfully!", 0);
        }
    }
}
