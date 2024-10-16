using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReFined.Common
{
    internal class Configuration
    {
        public static void Initialize()
        {
            if (!File.Exists("reFined.cfg"))
            {
                var _outDefault = new string[]
                {
                    "[General]",
                    "discordRPC = true",
                    "resetCombo = 0x0300",
                    "",
                    "[Kingdom Hearts II]",
                    "driveShortcuts = true",
                    "resetPrompt = true",
                    "",
                    "# Options: retry, continue",
                    "deathPrompt = retry",
                    "",
                    "# Options: sonic, arcanum, raid, ragnarok",
                    "# Order: [CONFIRM], TRI, SQU, [JUMP]",
                    "# Duplicates are allowed. All 4 slots must be filled.",
                    "limitShortcuts = [sonic, arcanum, raid, ragnarok]",
                };

                File.WriteAllLines("reFined.cfg", _outDefault);
            }
        }
    }
}
