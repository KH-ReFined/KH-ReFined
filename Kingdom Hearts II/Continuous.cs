using System;

using KH2FML;

namespace ReFined
{
    /// <summary>
    /// This class hosts functions that run all the time, that can either be toggled or not.
    /// </summary>
    public static class Continuous
    {
        public static bool[] DEBOUNCE = new bool[0x10];
        public static ulong SAVE_OFFSET;
        public static ulong PROMPT_OFFSET;
        public static ulong LIMITER_OFFSET;

        public static short[] LIMIT_SHORT;
        static byte[] LIMITER_FUNCTION;

        static bool LIMIT_SET = false;

        static byte PAST_TYPE = 0x00;

        static byte SAVE_CHECK = 0x75;

        static byte[][] _controlScheme = 
        [
            [ 0x78, 0x62, 0x6F ], // xbo
            [ 0x70, 0x73, 0x78 ], // psx
            [ 0x65, 0x74, 0x63 ]  // etc
        ];

        public static void CorrectTutorials()
        {
            var _controlType = Hypervisor.Read<byte>(Variables.ADDR_Confirm - 0x02);

            if (_controlType != PAST_TYPE)
            {
                Terminal.Log("A change in the Control Scheme has been detected! Adjusting the tutorial images...", 1);
                    
                Hypervisor.Write(Variables.MemoryKH["HELPIMAGE_FNAMES"] + 0x13, _controlScheme[_controlType - 0x01], true);
                Hypervisor.Write(Variables.MemoryKH["HELPIMAGE_FNAMES"] + 0x33, _controlScheme[_controlType - 0x01], true);
                Hypervisor.Write(Variables.MemoryKH["HELPIMAGE_FNAMES"] + 0x53, _controlScheme[_controlType - 0x01], true);
                Hypervisor.Write(Variables.MemoryKH["HELPIMAGE_FNAMES"] + 0x93, _controlScheme[_controlType - 0x01], true);
                Hypervisor.Write(Variables.MemoryKH["HELPIMAGE_FNAMES"] + 0xA3, _controlScheme[_controlType - 0x01], true);

                PAST_TYPE = _controlType;

                Terminal.Log("Tutorial images have been adjusted!", 0);
            }
        }

        /// <summary>
        /// Handles the modification of Limit Form's Shortcuts according to the config file.
        /// I really want to somehow allow this to happen in-game.
        /// </summary>
        public static void ModifyLimitShorts()
        {
            if (!LIMIT_SET)
            {
                Terminal.Log("Setting Limit Form's shortcuts according to \"reFined.cfg\"!", 1);

                // If confirm is set to "O", and the shortcuts are not set;
                if (Variables.CONFIRM_BUTTON == Variables.BUTTON.CIRCLE && Hypervisor.Read<ushort>(Variables.ADDR_LimitShortcut) != LIMIT_SHORT[3])
                {
                    Terminal.Log("Enforcing the JP Shortcut Scheme.", 1);
                    // Overwrite the first and last shortcut in the Japanese Scheme.
                    Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[3]);
                    Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[0]);
                }

                // If confirm is set to "X", and the shortcuts are not set;
                else if (Variables.CONFIRM_BUTTON == Variables.BUTTON.CROSS && Hypervisor.Read<ushort>(Variables.ADDR_LimitShortcut) != LIMIT_SHORT[0])
                {
                    Terminal.Log("Enforcing the US Shortcut Scheme.", 1);

                    // Overwrite the first and last shortcut in the International Scheme.
                    Hypervisor.Write(Variables.ADDR_LimitShortcut, LIMIT_SHORT[0]);
                    Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x06, LIMIT_SHORT[3]);
                }

                // Write the mid 2 shortcuts normally.
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x02, LIMIT_SHORT[1]);
                Hypervisor.Write(Variables.ADDR_LimitShortcut + 0x04, LIMIT_SHORT[2]);

                Terminal.Log("Limit Shorcuts have been set successfully!", 0);

                LIMIT_SET = true;
            }
        }

        /// <summary>
        /// Enforces Controller Prompts and Camera according to the setting.
        /// Should work without any issues.
        /// </summary>
        public static void EnforcePrompts()
        {
            // Check if the prompt has been enforced already.
            var _isFunctionEnforced = Hypervisor.Read<byte>(PROMPT_OFFSET + 0x06) == 0x00 ? true : false;

            // Make the adjustments according to what we want.
            if (_isFunctionEnforced != Locals.CONTROLLER_MODE)
            {
                Terminal.Log("Detected a change in Controller Prompt rule! Enforcing the change.", 1);
                var _targetMode = (byte)(Locals.CONTROLLER_MODE ? 0x00 : 0x01);

                Terminal.Log("Overwriting the Prompt instruction to reflect the change.", 1);
                Hypervisor.Write(PROMPT_OFFSET + 0x06, _targetMode);
                Hypervisor.Write(Variables.ADDR_ControllerMode, _targetMode);

                Terminal.Log("Controller Prompt rulechange has been reflected successfully!", 0);
            }
        }

        /// <summary>
        /// Handles the stupid Framelimiting function.
        /// Because SE did not bother to make a separate framelimiter for cutscenes, outright disabling it breaks 30FPS.
        /// So I have to see what framerate the game is set to, and enable/disable it manually.
        /// </summary>
        public static void HandleFramelimiter()
        {
            // If the original function code is not in memory, read it.
            if (LIMITER_FUNCTION == null)
            {
                Terminal.Log("Reading the Framelimiter function from memory for future use.", 1);
                LIMITER_FUNCTION = Hypervisor.Read<byte>(LIMITER_OFFSET, 0x06);
            }

            // Read the current framerate setting, and the first byte of the framelimiting function.
            var _fetchFunction = Hypervisor.Read<byte>(LIMITER_OFFSET);
            var _fetchFramerate = Hypervisor.Read<byte>(Variables.ADDR_Framerate);

            // If it's 30FPS, restore the framelimiter.
            if (_fetchFramerate == 0x00 && _fetchFunction == 0x90)
            {
                Terminal.Log("The framelimiter has been set!", 0);
                Hypervisor.Write(LIMITER_OFFSET, LIMITER_FUNCTION);
                Hypervisor.Write(Variables.ADDR_Framelimiter, 0x01);
            }

            // If it's 60FPS, destroy the framelimiter.
            else if (_fetchFramerate != 0x00 && _fetchFunction != 0x90)
            {
                Terminal.Log("The framelimiter has been released!", 0);
                Hypervisor.DeleteInstruction(LIMITER_OFFSET, 0x06);
                Hypervisor.Write(Variables.ADDR_Framelimiter, 0x00);
            }
        }

        /// <summary>
        /// Fixes the Save Point Behavior.
        /// Not much to say about this, really.
        /// </summary>
        public static void FixSavePoint()
        {
            // If the Reaction Command is "SAVE" and the byte is not yet set;
            if (Hypervisor.Read<ushort>(Variables.ADDR_ReactionID) == 0x0037)
            {
                if (Variables.IS_LOADED)
                {
                    var _healthArray = new List<int[]>()
                    {
                        Hypervisor.Read<int>(Variables.ADDR_PlayerStats, 0x02),
                        Hypervisor.Read<int>(Variables.ADDR_PlayerStats - 0x278, 0x02),
                        Hypervisor.Read<int>(Variables.ADDR_PlayerStats - 0x4F0, 0x02),
                    };

                    var _magicArray = new List<int[]>()
                    {
                        Hypervisor.Read<int>(Variables.ADDR_PlayerStats + 0x180, 0x02),
                        Hypervisor.Read<int>(Variables.ADDR_PlayerStats + 0x180 - 0x278, 0x02),
                        Hypervisor.Read<int>(Variables.ADDR_PlayerStats + 0x180 - 0x4F0, 0x02),
                    };

                    // Check if the HP/MP of the current party member is full.
                    var _isHealthFull = _healthArray.All(x => x[0] == x[1]);
                    var _isMagicFull = _magicArray.All(x => x[0] == x[1]);

                    // If not, break out of this loop.
                    if (!_isHealthFull || !_isMagicFull)
                        SAVE_CHECK = 0x75;

                    // If all the party members have full HP/MP, disable Save Point healing.
                    else if (SAVE_CHECK == 0x75)
                    {
                        Thread.Sleep(100);
                        SAVE_CHECK = 0xEB;
                    }
                }

                // Write the resulting byte to the Save Point function.
                Hypervisor.Write(SAVE_OFFSET + 0x25B, SAVE_CHECK);
            }

            else if (!Variables.IS_LOADED && SAVE_CHECK != 0x75)
            {
                SAVE_CHECK = 0x75;
                Hypervisor.Write(SAVE_OFFSET + 0x25B, SAVE_CHECK);
            }
        }

        /// <summary>
        /// Activates warping to the Garden of Assemblage from the World Map.
        /// </summary>
        public static void ActivateWarpGOA()
        {
            // Read the flag that signifies of GOA has been acknowledged and the flag that enables it's landing.
            var _isGardenKnown = (Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x231B) & 0x04) == 0x04;
            var _canGardenLand = (Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x1EF6) & 0x40) == 0x40;

            // If the flag ain't set and GOA has been acknowledged: Set the flag.
            if (_isGardenKnown && !_canGardenLand)
            {
                Terminal.Log("The Garden of Assemblage has been acknowledged! The warp flag has been set accordingly!", 0);
                Hypervisor.Write(Variables.ADDR_SaveData + 0x1EF6, (byte)(Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x1EF6) + 0x40));
            }
        }
    }
}