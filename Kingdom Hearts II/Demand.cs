using KH2FML;

namespace ReFined
{
    /// <summary>
    /// This class hosts functions that run only at will, either through a button combo or a config setting.
    /// </summary>
    public static class Demand
    {
        // Debounce Array Mapping:
        // 0x00 => Soft-Reset Check (TriggerReset)
        // 0x01 => Bulky Vendor Check (HandleEncounter)
        // 0x02 => Shortcut Switching Check (HandleShortcuts)
        // 0x03 => Files Loaded for Combat Mode (CombatMode)
        // 0x04 => Activate Combat Mode (Combat Mode)
        static bool[] DEBOUNCE = new bool[0x10];

        public static ulong FADESET_OFFSET;
        public static IntPtr FUNC_UPDATESHORTCUTS;

        static byte[] MAIN_TEXT;
        static byte[] SORA_TEXT;

        static bool TEXT_IS_SORA;
        static ulong SORA_MSG_POINT;

        static bool SUBMIT_SHORTCUTS;
        static byte CURRENT_SHORTCUT = 0x80;

        static ulong RESOLVED_POINT;
        static ulong ACTIVATE_POINT;

        static int ACTIVATE_SIZE;
        static int RESOLVED_SIZE;

        static bool VENDOR_GIVEN;

        /// <summary>
        /// Handles Autoattacking logic, since we can't just force it nilly willy!
        /// This checks a lot of the game's states and it may still not be perfect.
        /// We are working on adding shortcut automation as well but we ain't there yet.
        /// </summary>
        public static void HandleAutoattack()
        {
            // If Autoattacking is enabled;
            if (Locals.AUTOATTACK)
            {
                // Fetch the pointer to Sora's action.
                var _actionPoint = Hypervisor.GetPointer64(Variables.PINT_ActionEXE, [0x0A]);

                /* 
                 * Read the following:
                 * - Sora's current action.
                 * - The current Sub-Command Menu, if any.
                 * - The current Command Index.
                 * - Any active Party Member Limit.
                 * - The first Command in the current Menu.
                 * - The main Command Menu type.
                 */
                var _currAction = Hypervisor.Read<int>(_actionPoint, true);
                var _subCommand = Hypervisor.Read<ulong>(Variables.PINT_ChildMenu);
                var _partyLimit = Hypervisor.Read<ulong>(Variables.PINT_PartyLimit);
                var _currCommand = Hypervisor.Read<byte>(Hypervisor.GetPointer64(Variables.PINT_ChildMenu - 0x08, [0x74]), true);
                var _firstCommand = Hypervisor.Read<short>(Hypervisor.GetPointer64(Variables.PINT_ChildMenu - 0x08, [0x16]), true);
                var _mainCommandType = Hypervisor.Read<byte>(Hypervisor.GetPointer64(Variables.PINT_ChildMenu - 0x08, [0x00]), true);

                /* 
                 * See if the following are true:
                 * Is Action Good => The current Command Index is 0x00, there are no active Party Limits, there are no Sub-Command Menus present.
                 * Is Command Good => The first command is either "Attack", "Shoot", or "Hit", the Command Menu is either the Primary or Secondary Main Menu.
                 * Is Status Good => The game is not on the Title Screen or a Cutscene, there is an ongoing Battle.
                 */
                var _isActionGood = _subCommand == 0x00 && _partyLimit == 0x00 && _currCommand == 0x00;
                var _isCommandGood = (_firstCommand == 0x0001 || _firstCommand == 0x016D || _firstCommand == 0x0088) && (_mainCommandType == 0x00 || _mainCommandType == 0x06);
                var _isStatusGood = !Variables.IS_TITLE && !Variables.IS_CUTSCENE && Variables.BATTLE_MODE != Variables.BATTLE_TYPE.PEACEFUL;

                // Check if we are holding the Confirm Button, and the following were indeed true.
                var _autoCheck = Variables.IS_PRESSED(Variables.CONFIRM_BUTTON) && _isActionGood && _isCommandGood && _isStatusGood;

                // If so; Trigger Autoattack.
                if (_autoCheck && _currAction == 0x00)
                    Hypervisor.Write(_actionPoint, 0x01, true);

                // If not, and Autoattck is on, turn it off.
                else if (!_autoCheck && _currAction == 0x01)
                    Hypervisor.Write(_actionPoint, 0x00, true);
            }
        }

        /// <summary>
        /// Trigger Soft-Reset if requested.
        /// Shows a prompt giving a 2 second heads-up if enabled.
        /// </summary>
        public static void TriggerReset()
        {
            // Check if the game isn't already on the title screen, and the room is loaded.
            var _cancelRequest = false;
            var _canReset = !Variables.IS_TITLE && Variables.IS_LOADED;

            // If we press the Reset Combo given, we can reset, and the Debounce is not set;
            if (Variables.IS_PRESSED(Locals.RESET_COMBO) && _canReset && !DEBOUNCE[0x00])
            {
                Terminal.Log("Soft Reset requested! Checking the prequisites...", 1);

                // Fetch the date and time when the function executes.
                var _currentTime = DateTime.Now;

                // Set the Debounce.
                DEBOUNCE[0x00] = true;

                // If the prompt has been enabled;
                if (Locals.RESET_PROMPT)
                {
                    Terminal.Log("Prompt is enabled! Showing the Cancellation Prompt...", 1);

                    // Pop-up the Prize Popup, saying you can press [CONFIRM] to cancel.
                    Popup.PopupPrize(0x5700);

                    // Start a new task that will be active during the grace period of the prompt.
                    Task.Factory.StartNew(() =>
                    {
                        // Calculate the time that has passed since the reset was requested.
                        var _calculateTime = (DateTime.Now - _currentTime).TotalSeconds;

                        // While we are in the grace period;
                        while (_calculateTime < 2)
                        {
                            // If we press the Confirm Button;
                            if (Variables.IS_PRESSED(Variables.CONFIRM_BUTTON))
                            {
                                Terminal.Log("Soft Reset has been canceled!", 1);

                                // Signify that Soft-Reset has been canceled, and release the Debounce.
                                _cancelRequest = true;
                                DEBOUNCE[0x00] = false;

                                // Pop-up the Prize Popup that says the reset is indeed canceled.
                                Popup.PopupPrize(0x5701);
                                break;
                            };

                            _calculateTime = (DateTime.Now - _currentTime).TotalSeconds;
                        }

                        // If the Soft-Reset is NOT canceled;
                        if (!_cancelRequest)
                            InitiateReset();
                    });
                }

                else
                    InitiateReset();
            }
        }

        /// <summary>
        /// Does the actual Soft-Reset routine.
        /// </summary>
        public static void InitiateReset()
        {
            Terminal.Log("Initiating the Soft Reset...", 1);

            // Construct a new area memory to temporarily warp to.
            Terminal.Log("Constructing the warp-to point!", 1);
            var _warpArea = new byte[0x10];

            _warpArea[0x00] = 0x01;
            _warpArea[0x01] = 0x01;

            // Write the target area in question.
            Hypervisor.Write(Variables.ADDR_Area, _warpArea);

            // Kill the music, and execute the warp.
            Terminal.Log("Initiating warp to a dummy world!", 1);
            Sound.KillBGM();
            Shisutemu.ExecuteWarp(Variables.ADDR_Area, Shisutemu.FADE_TYPE.BLACKOUT);

            // Wait until the fade has been completed.
            Terminal.Log("Waiting for the fade to fully realize.", 1);
            while (Hypervisor.Read<byte>(Variables.ADDR_FadeValue) != 0x80) { }

            Terminal.Log("Noting and killing the fade-in instructions within the warp function.", 1);

            // Make a note of the Fade-In instruction.
            var _fadeAddress = (ulong)(Shisutemu.FUNC_MAPJUMP + 0x9F);
            var _fadeSetAddress = FADESET_OFFSET + 0x1C1;

            var _fadeInstruction = Hypervisor.Read<byte>(_fadeAddress, 0x05);
            var _fadeSetInstruction = Hypervisor.Read<byte>(_fadeSetAddress, 0x03);

            // Kill said instruction.
            Hypervisor.DeleteInstruction(_fadeAddress, 0x05);
            Hypervisor.DeleteInstruction(_fadeSetAddress, 0x03);

            while (!Variables.IS_LOADED)
            { }

            // Suspend the game to prepare for the warpback.
            Terminal.Log("ENTERING CRITICAL SECTION! Suspending the game!", 1);
            Axa.Suspend(false);

            // Trigger the reset and release the Debounce.
            Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
            DEBOUNCE[0x00] = false;

            Terminal.Log("CRITICAL SECTION has been completed! Resuming the game.", 1);
            Axa.Resume(false);

            Thread.Sleep(250);

            // Restore the instructions noted earlier.
            Terminal.Log("Restoring the deleted instructions!", 1);

            Hypervisor.Write(_fadeAddress, _fadeInstruction);
            Hypervisor.Write(_fadeSetAddress, _fadeSetInstruction);

            Terminal.Log("Soft Reset complete! Please rate your journey a 10/10 on TripAdvisor!", 0);
        }

        /// <summary>
        /// The logic for Enounter Plus!
        /// Not just the execution logic, this also handles the obtain logic and the luck up logic!
        /// Trying to figure out the room memory for the enemies was pain, and it was so worth it!
        /// </summary>
        public static void HandleEncounter()
        {
            // Read the Moogle Level, Room Pointer, and Sora's Abilities.
            var _roomPoint = Hypervisor.Read<ulong>(Variables.PINT_EnemyInfo) + 0x08;
            var _abilityRead = Hypervisor.Read<ushort>(Variables.ADDR_SaveData + 0x2544, 0x60);

            var _isMoogleLeveled = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x4C34) >= 0x04;
            var _hasAbility = _abilityRead.Contains<ushort>(0x80F8) || _abilityRead.Contains<ushort>(0x00F8);

            // If we are not in the Title Screen, the map is loaded, the Moogle Level is above 4, and we do NOT have Encounter Plus;
            if (!Variables.IS_TITLE && Variables.IS_LOADED && _isMoogleLeveled && !_hasAbility)
            {
                Terminal.Log("Conditions have been met for Encounter Plus! Administring it to the player.", 0);

                // Find the first empty Ability Spot.
                var _fetchIndex = Array.FindIndex(_abilityRead, x => x == 0x0000);

                // Give the ability on that spot, unequipped.
                Hypervisor.Write<ushort>(Variables.ADDR_SaveData + 0x2544 + (ulong)(_fetchIndex * 0x02), 0x00F8);

                Terminal.Log("Encounter Plus has been given successfully!", 0);
            }

            // Read the current world and room.
            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);

            // If the map is NOT loaded, Encounter Plus is enabled, and the Debounce is not set;
            if (!Variables.IS_LOADED && _abilityRead.Contains<ushort>(0x80F8) && !DEBOUNCE[0x01])
            {
                Terminal.Log("Suspending the game in preparation for Encounter Plus operations.", 1);

                Axa.Suspend(false);

                Terminal.Log("Clearing the Enemy Data in the room.", 1);

                // Clear the enemy data in the room.
                Hypervisor.Write(_roomPoint, new byte[0x100], true);

                // Check if we are in a Vendor Area.
                uint _vendorOffset = _worldRead == 0x05 && _roomRead == 0x08 ? 0x00U :
                                    (_worldRead == 0x08 && _roomRead == 0x02 ? 0x01U :
                                    (_worldRead == 0x06 && _roomRead == 0x07 ? 0x02U :
                                    (_worldRead == 0x07 && _roomRead == 0x01 ? 0x03U :
                                    (_worldRead == 0x0E && _roomRead == 0x06 ? 0x04U : 0xFF))));

                // If we indeed are in a Vendor Area;
                if (_vendorOffset != 0xFF)
                {
                    Terminal.Log("We are in a Vendor Area! Starting calculation...", 1);

                    // Read the Vendor Byte corresponding to the current room.
                    var _vendorRead = Hypervisor.Read<byte>(Variables.ADDR_VendorMem + _vendorOffset);

                    // If the Vendor did not appear yet;
                    if (_vendorRead != 0x02)
                    {
                        Terminal.Log("Vendor has not appeared yet! Rolling the dice...", 1);

                        // Rool a random number from 1 to 100.
                        var _randomCheck = new Random().Next(1, 100);

                        // If it is 80 or above, signifying a 20% chance;
                        if (_randomCheck >= 80)
                        {
                            Terminal.Log("It's a hit! Changing the room to a Vendor Room...", 1);

                            // Give the Vendor.
                            Hypervisor.Write<byte>(Variables.ADDR_VendorMem + _vendorOffset, 0x02);
                        }
                    }
                }

                // Set the Debounce.
                DEBOUNCE[0x01] = true;

                Terminal.Log("Resuming the game as Encounter Plus operations have concluded.", 0);

                Axa.Resume(false);
            }

            // If the room has been loaded, release the Debounce.
            else if (Variables.IS_LOADED && DEBOUNCE[0x01])
                DEBOUNCE[0x01] = false;
        }

        /// <summary>
        /// Multiple Shortcut Menus! Ain't that just DANDY?!
        /// This little piece of shit code took many years off of my life.
        /// "Everything is gotta be perfect!" Said Topaz, when writing this code.
        /// Not being aware it would suck the life out of him.
        /// </summary>
        public static void HandleShortcuts()
        {
            // Fetch the Command Menu pointer and read it's type.
            var _menuPointer = Hypervisor.GetPointer64(Variables.PINT_ChildMenu - 0x08);
            var _menuType = Hypervisor.Read<byte>(_menuPointer, true);

            // Fetch the Sub-Menu Type
            var _subMenuPointer = Hypervisor.GetPointer64(Variables.PINT_SubMenuOptionSelect, [0x00]);
            var _subSelection = Hypervisor.Read<byte>(_subMenuPointer, true);
            var _subMenuType = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);

            // Read the current shortcut menu, and the current Form.

            var _seenHelp = Hypervisor.Read<short>(Variables.ADDR_SaveData + 0x4270);
            var _seenShort = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE248);
            
            var _currShort = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0xE000);
            var _currForm = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x3524);

            var _fetchCommand = (Hypervisor.Read<Variables.CONFIG>(Variables.ADDR_Config) & Variables.CONFIG.COMMAND_KH1) == Variables.CONFIG.COMMAND_KH1 &&
                                 Hypervisor.Read<byte>(Variables.ADDR_Config + 0x03) == 0x00;

            /*
             * So, this function works by storing all the shortcut menus in a "fake" address, which is "_shortFake",
             * then transfering them accordingly to the "real" address, which is "_shortReal".
             * This makes more sense after having the whole picture.
             */
            var _shortReal = Variables.ADDR_SaveData + 0x36F8;
            var _shortFake = Variables.ADDR_SaveData + 0xE200;

            // If the pointer for the "Sora" text is not defined, fetch it.
            if (SORA_MSG_POINT == 0x00)
            {
                Terminal.Log("Fetching Text ID 0x051F for the Sora Text.", 0);
                SORA_MSG_POINT = Text.GetStringPointer(0x051F);
            }

            if (!Variables.IS_LOADED && CURRENT_SHORTCUT != _currShort)
            {
                Terminal.Log("Shortcut mishap! Correcting the current Shortcut Menu!", 0);
                CURRENT_SHORTCUT = _currShort;

                if (_fetchCommand)
                {
                    Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC, CURRENT_SHORTCUT == 0x00 ? 0x21 : 0x20);
                    Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC + 0x90, CURRENT_SHORTCUT == 0x01 ? 0x21 : 0x20);
                    Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC + 0x120, CURRENT_SHORTCUT == 0x02 ? 0x21 : 0x20);
                }
            }

            /*
             * Check if the following are true:
             * Is Customize Menu => The game is paused, and the Sub-Menu is "Customize"
             * Is Editing Shortcut => The game is paused, and we are editing any of the 4 shortcuts.
             */
            var _isCustomizeMenu = Variables.IS_PAUSED && _subMenuType == 0x19;
            var _isEditingShortcut = Variables.IS_PAUSED && (_subMenuType == 0x1A || _subMenuType == 0x1D || _subMenuType == 0x1E || _subMenuType == 0x1F);

            // If the main "Sora" text is not defined in memory, fetch them.
            if (MAIN_TEXT == null)
            {
                Terminal.Log("Fetching string literals for Text IDs 0x572E and 0x051F for the Shortcut text.", 0);
                SORA_TEXT = Text.GetStringLiteral(0x572E);
                MAIN_TEXT = Text.GetStringLiteral(0x051F);
            }

            // If we are not on Title Screen, and Lite Mode is NOT enabled;
            if (!Variables.IS_TITLE && !Locals.IS_LITE)
            {
                if ((_seenHelp & 0x0600) != 0x0600)
                    Hypervisor.Write(Variables.ADDR_SaveData + 0x4270, _seenHelp + 0x0F00);

                if (_isCustomizeMenu && _seenShort == 0x00)
                {
                    Popup.PopupHelp(0x04);
                    Hypervisor.Write<byte>(Variables.ADDR_SaveData + 0xE248, 0x01);
                }

                #region Shortcut Submitting
                // If in editing mode, allow Shortcuts to be submitted.
                if (_isEditingShortcut && !SUBMIT_SHORTCUTS)
                    SUBMIT_SHORTCUTS = true;

                // If no longer editing the shortcuts but submitting is enabled;
                else if (!_isEditingShortcut && SUBMIT_SHORTCUTS)
                {
                    Terminal.Log("Submitting Shortcut Data for Menu \"" + ('A' + CURRENT_SHORTCUT) + "\"!", 1);

                    // Take the "real" Shortcut Data, slap it on the current "fake" Shortcut Data, disallow submitting.
                    var _shortTake = Hypervisor.Read<byte>(_shortReal, 0x08);
                    Hypervisor.Write(_shortFake + (0x08U * CURRENT_SHORTCUT), _shortTake);
                    SUBMIT_SHORTCUTS = false;

                    Terminal.Log("Shortcut Data was successfully submitted!", 0);
                }
                #endregion

                #region Menu Text Handling
                // If we are not in anywhere Shortcut related, and the text is not "Sora";
                if (!_isCustomizeMenu && !_isEditingShortcut && !TEXT_IS_SORA)
                {
                    Terminal.Log("Switching the Character Name Text to \"Sora\".", 0);
                    // Make the text "Sora".
                    Hypervisor.Write(SORA_MSG_POINT, SORA_TEXT, true);
                    TEXT_IS_SORA = true;
                }

                // If we are in anywhere Shortcut Related;
                else if ((_isCustomizeMenu || _isEditingShortcut) && (MAIN_TEXT[MAIN_TEXT.Length - 0x02] != (byte)(0x2E + CURRENT_SHORTCUT) || TEXT_IS_SORA))
                {
                    var _shortChar = CURRENT_SHORTCUT == 0x01 ? "B" : (CURRENT_SHORTCUT == 0x02 ? "C" : "A");
                    Terminal.Log("Switching the Character Name Text to \"Shortcut Set " + _shortChar + "\"!", 0);

                    if (_fetchCommand)
                    {
                        Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC, CURRENT_SHORTCUT == 0x00 ? 0x21 : 0x20);
                        Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC + 0x90, CURRENT_SHORTCUT == 0x01 ? 0x21 : 0x20);
                        Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC + 0x120, CURRENT_SHORTCUT == 0x02 ? 0x21 : 0x20);
                    }

                    // Edit the main Shortcut Text to reflect the current set, and write it.
                    MAIN_TEXT[MAIN_TEXT.Length - 0x02] = (byte)(0x2E + CURRENT_SHORTCUT);
                    Hypervisor.Write(SORA_MSG_POINT, MAIN_TEXT, true);

                    // Signify the text is currently not "Sora".
                    TEXT_IS_SORA = false;
                }
                #endregion

                #region Input and Switching
                // If we are NOT in any shortcut related situation, or are not pressing UP, DOWN, L1, or R1; Release the Debounce.
                if (((_isCustomizeMenu || _isEditingShortcut) && !Variables.IS_PRESSED(Variables.BUTTON.L1) && !Variables.IS_PRESSED(Variables.BUTTON.R1)) ||
                    (_menuType == 0x05 && !Variables.IS_PRESSED(Variables.BUTTON.DOWN) && !Variables.IS_PRESSED(Variables.BUTTON.UP)))
                    DEBOUNCE[0x02] = false;

                // If the Debounce is not set;
                if (!DEBOUNCE[0x02])
                {
                    // If the Shortcut Menu is present;
                    if (_menuType == 0x05)
                    {
                        // If we are NOT in Limit or Anti Form;
                        if (_currForm != 0x03 && _currForm != 0x06)
                        {
                            // Check the input and determine which way the sets will flow.
                            var _flowDirection = Variables.IS_PRESSED(Variables.BUTTON.DOWN) ? 1 : (Variables.IS_PRESSED(Variables.BUTTON.UP) ? -1 : 0x00);

                            // If the shortcut menu will indeed flow;
                            if (_flowDirection != 0x00)
                            {
                                // Play the sound, set the current shortcut set.
                                Sound.PlaySFX(0x14);

                                CURRENT_SHORTCUT += (byte)_flowDirection;
                                var _shortChar = CURRENT_SHORTCUT == 0x01 ? "B" : (CURRENT_SHORTCUT == 0x00 ? "A" : "C");

                                Terminal.Log("Swtiching the Shortcut Menu to \"" + _shortChar + "\"!", 0);

                                // Set the Debounce.
                                DEBOUNCE[0x02] = true;
                            }
                        }

                        // Otherwise;
                        else
                        {
                            // Check the input and determine which way the sets will flow.
                            var _flowDirection = Variables.IS_PRESSED(Variables.BUTTON.DOWN) ? 1 : (Variables.IS_PRESSED(Variables.BUTTON.UP) ? -1 : 0x00);

                            // If the shortcut menu will indeed flow;
                            if (_flowDirection != 0x00)
                            {
                                // Deny any input.
                                Sound.PlaySFX(0x05);
                                DEBOUNCE[0x02] = true;
                            }
                        }

                        if (_fetchCommand)
                        {
                            Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC, CURRENT_SHORTCUT == 0x00 ? 0x21 : 0x20);
                            Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC + 0x90, CURRENT_SHORTCUT == 0x01 ? 0x21 : 0x20);
                            Hypervisor.Write(Variables.ADDR_CommandSEQD + 0x98FC + 0x120, CURRENT_SHORTCUT == 0x02 ? 0x21 : 0x20);
                        }
                    }

                    // If we are in "Customize" and we selected "Shortcut Set X";
                    else if (_isCustomizeMenu && _subSelection == 0x00)
                    {
                        // Check the input and determine which way the sets will flow.
                        var _flowDirection = Variables.IS_PRESSED(Variables.BUTTON.R1) ? 1 : (Variables.IS_PRESSED(Variables.BUTTON.L1) ? -1 : 0x00);

                        // If the shortcut menu will indeed flow;
                        if (_flowDirection != 0x00)
                        {
                            // Play the sound, set the current shortcut set.
                            Sound.PlaySFX(0x02);

                            CURRENT_SHORTCUT += (byte)_flowDirection;
                            var _shortChar = CURRENT_SHORTCUT == 0x01 ? "B" : (CURRENT_SHORTCUT == 0x00 ? "A" : "C");

                            Terminal.Log("Swtiching the Shortcut Menu to \"" + _shortChar + "\"!", 0);

                            // Set the Debounce.
                            DEBOUNCE[0x02] = true;
                        }
                    }
                }
                #endregion

                #region Shortcut Swapping
                // Basically, prevent overflows and read the Current Shortcut Set from the Save File if uninitialized.
                switch (CURRENT_SHORTCUT)
                {
                    case 0x03:
                        CURRENT_SHORTCUT = 0x00;
                        break;

                    case 0x80:
                        CURRENT_SHORTCUT = Hypervisor.Read<byte>(Variables.ADDR_ContinueData + 0xE000);
                        break;

                    case 0xFF:
                        CURRENT_SHORTCUT = 0x02;
                        break;
                }

                // If there is a mismatch between the shortcut set in-game vs in memory, and Debounce has been set:
                if (CURRENT_SHORTCUT != _currShort && DEBOUNCE[0x02])
                {
                    Terminal.Log("Shortcut Set switch detected! Committing the changes!", 1);

                    // Take the data for the current set.
                    var _shortTake = Hypervisor.Read<byte>(_shortFake + (0x08U * CURRENT_SHORTCUT), 0x08);

                    // Write it to the "real" shortcut data.
                    Hypervisor.Write(_shortReal, _shortTake);
                    Hypervisor.Write(Variables.ADDR_SaveData + 0xE000, CURRENT_SHORTCUT);

                    // If in "Customize", refresh the Shortcut List.
                    if (_isCustomizeMenu)
                    {
                        Terminal.Log("Refreshing the Shortcut Customize Menu!", 1);
                        Variables.SharpHook[FUNC_UPDATESHORTCUTS].Execute();
                    }

                    Terminal.Log("Commit to Shortcut Sets successful!", 0);


                }
                #endregion
            }

            // If we are in Lite Mode, enforce the text to always be "Sora" regardless of any input.
            else if (!Variables.IS_TITLE && Locals.IS_LITE && !TEXT_IS_SORA)
            {
                if ((_seenHelp & 0x0600) != 0x0600)
                    Hypervisor.Write(Variables.ADDR_SaveData + 0x4270, _seenHelp + 0x0600);

                Terminal.Log("Re:Freshed Detected! Locking Sora's name as-is.", 0);
                Hypervisor.Write(SORA_MSG_POINT, SORA_TEXT, true);
                TEXT_IS_SORA = true;
            }
        }

        /// <summary>
        /// Activates the Mare Menu with the SQUARE button on EGS versions of the game.
        /// See, Square Enix was stupid when programming on how to show this menu.
        /// So I took maximum advantage of it, and I hate my life because of it.
        /// </summary>
        public static void ActivateMare()
        {
            if (Locals.PLATFORM == "EPIC")
            {
                var _isMare = Hypervisor.Read<byte>(Variables.ADDR_Mare);

                var _isMenu = Hypervisor.Read<byte>(Variables.ADDR_MenuFlag);
                var _typeMenu = Hypervisor.Read<byte>(Variables.ADDR_MenuType);
                var _typeSubMenu = Hypervisor.Read<byte>(Variables.ADDR_SubMenuType);

                // Read the instruction that prevents Mare from showing up.
                var _readInstruction = Hypervisor.Read<byte>(0x103A80 + 0x1C, 0x06);

                /*
                 * Check if the following are true:
                 * Title Okay => Is on the Title Screen and no menus are present.
                 * Menu Okay => Is NOT on the Title Screen, is in the Camp Menu, are not in any sub-menus.
                 */
                var _titleOkay = Variables.IS_TITLE && _isMenu == 0x00;
                var _menuOkay = !Variables.IS_TITLE && _isMenu == 0x01 && _typeMenu == 0x08 && _typeSubMenu == 0x00;

                // If the conditions are met and SQUARE is pressed:
                if (Variables.IS_PRESSED(Variables.BUTTON.SQUARE) && (_titleOkay || _menuOkay))
                {
                    Terminal.Log("SQUARE has been pressed while in the menu in the EGS version! Activating the MARE Menu!", 1);

                    // Delete and restore the function that prevents Mare's presence with a 10ms break.
                    Hypervisor.DeleteInstruction(0x103A80 + 0x1C, 0x06);
                    Thread.Sleep(10);
                    Hypervisor.Write(0x103A80 + 0x1C, _readInstruction);

                    Terminal.Log("MARE has been activated successfully!", 0);
                }
            }
        }

        public static void CombatMode()
        {
            if (Locals.COMBAT_MODE && !Locals.CONFIG_RELOADING)
            {
                if (Variables.IS_LOADED && !Variables.IS_CUTSCENE && !Variables.IS_TITLE)
                {
                    if (Variables.BATTLE_MODE != Variables.BATTLE_TYPE.PEACEFUL && !DEBOUNCE[0x04])
                    {
                        Sound.PlayVSB("se/activateCombat.win32.scd");
                        DEBOUNCE[0x04] = true;
                    }

                    else if (Variables.BATTLE_MODE == Variables.BATTLE_TYPE.PEACEFUL && DEBOUNCE[0x04])
                    {
                        Sound.PlayVSB("se/conflictResolved.win32.scd");
                        DEBOUNCE[0x04] = false;
                    }
                }

                else if (!Variables.IS_LOADED && DEBOUNCE[0x03])
                    DEBOUNCE[0x03] = false;
            }
        }
    }
}
