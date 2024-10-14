using ReFined.Common;
using ReFined.KH2.InGame;
using ReFined.KH2.Information;

namespace ReFined.KH2.Functions
{
    public static class Demand
    {
        public static ulong OffsetPrompts;
        static bool[] DEBOUNCE = new bool[0x20];

        /// <summary>
        /// When the proper input is given, returns to the title screen.
        /// When the option for it is toggled, prompts the user for a cancellation.
        /// </summary>
        public static void ResetGame()
        {
            var _currentTime = DateTime.Now;

            var _buttonRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _confirmRead = Hypervisor.Read<ushort>(Variables.ADDR_Confirm);

            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _canReset = !Checks.CheckTitle() && _loadRead == 0x01;

            // If the button combo was exactly as requested, and a menu isn't present:
            if (_buttonRead == Variables.RESET_COMBO && _canReset && !DEBOUNCE[0])
            {
                Terminal.Log("Reset Sequence Initiated! Checking for the prompt...", 0);
                DEBOUNCE[0] = true;

                // If the prompt has been requested:
                if (Variables.RESET_PROMPT)
                {
                    Terminal.Log("Reset prompt requested! Showing prompt...", 0);

                    // Show the prompt.
                    Message.ShowSmallObtained(0x01BA);
                    var _cancelRequest = false;

                    // Start the prompt task.
                    Task.Factory.StartNew(() =>
                    {
                        Terminal.Log("Prompt shown! Waiting 2 seconds...", 0);

                        // For the next 2 seconds:
                        while ((DateTime.Now - _currentTime) < TimeSpan.FromSeconds(2))
                        {
                            // Monitor the buttons, and if pressed:
                            var _buttonSeek = (_confirmRead == 0x01 ? 0x20 : 0x40);
                            var _buttonSecond = Hypervisor.Read<ushort>(Variables.ADDR_Input);

                            // Cancel the reset.
                            if ((_buttonSecond & _buttonSeek) == _buttonSeek)
                            {
                                Terminal.Log("Reset cancelled!", 0);
                                Message.ShowSmallObtained(0x01BB);
                                _cancelRequest = true;
                                DEBOUNCE[0] = false;
                                break;
                            };
                        }

                        // If not cancelled: Initiate the reset.
                        if (!_cancelRequest)
                        {
                            Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
                            Terminal.Log("Soft Reset was successful!", 0);
                            DEBOUNCE[0] = false;
                        }
                    });
                }

                // If the prompt isn't requested: Reset instantly.
                else
                {
                    Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);
                    Terminal.Log("Soft Reset was successful!", 0);
                    DEBOUNCE[0] = false;
                }
            }
        }

        /// <summary>
        /// Enforces the controller prompts instead of detecting KBM when requested.
        /// </summary>
        public static void PromptSelector()
        {
            var _contCheck = Hypervisor.Read<byte>(OffsetPrompts + 0x06);

            if (Variables.CONTROLLER_MODE != 0x02)
            {
                if (_contCheck != 0x00)
                {
                    Hypervisor.Write(OffsetPrompts + 0x06, 0x00);
                    Terminal.Log("Manual prompt mode detected! Enforcing prompts...", 0);
                }

                Hypervisor.Write(Variables.ADDR_ControllerMode, Variables.CONTROLLER_MODE);
            }

            else
            {
                if (_contCheck != 0x01)
                {
                    Hypervisor.Write(OffsetPrompts + 0x06, 0x01);
                    Terminal.Log("Automatic prompt mode detected! Restoring the seeker...", 0);
                }
            }
        }

    }
}
