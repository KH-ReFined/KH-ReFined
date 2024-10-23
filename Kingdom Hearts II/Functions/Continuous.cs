using ReFined.Common;
using ReFined.KH2.Information;
using ReFined.Libraries;

namespace ReFined.KH2.Functions
{
    public static class Continuous
    {
        public static ulong PROMPT_OFFSET;
        public static ulong LIMITER_OFFSET;

        public static byte[]? LIMITER_FUNCTION = null;

        public static void ToggleLimiter()
        {
            if (LIMITER_FUNCTION == null)
                LIMITER_FUNCTION = Hypervisor.Read<byte>(LIMITER_OFFSET, 0x06);

            var _fetchFramerate = Hypervisor.Read<byte>(Variables.ADDR_Framerate);
            var _fetchFunction = Hypervisor.Read<byte>(LIMITER_OFFSET);
            var _limiterInvert = (byte)(_fetchFramerate == 0x00 ? 0x01 : 0x00);

            if (_fetchFramerate == 0x00 && _fetchFunction == 0x90)
            {
                Terminal.Log("Toggling the framelimiter for 30FPS.", 0);
                Hypervisor.Write(LIMITER_OFFSET, LIMITER_FUNCTION);
            }

            else if (_fetchFramerate != 0x00 && _fetchFunction != 0x90)
            {
                Terminal.Log("Toggling the framelimiter for 60FPS.", 0);
                Hypervisor.DeleteInstruction(LIMITER_OFFSET, 0x06);
            }

            Hypervisor.Write(Variables.ADDR_Framelimiter, _limiterInvert);
        }

        public static void TogglePrompts()
        {
            var _promptCheck = Hypervisor.Read<byte>(PROMPT_OFFSET + 0x06);
            var _promptString = Variables.CONTROLLER_MODE == 0x00 ? "Manual" : "Automatic";

            if (_promptCheck != Variables.CONTROLLER_MODE)
            {
                Terminal.Log("Switching to " + _promptString + " Prompt Mode.", 0);
                Hypervisor.Write(PROMPT_OFFSET + 0x06, Variables.CONTROLLER_MODE);
            }

            Hypervisor.Write(Variables.ADDR_ControllerMode, Variables.CONTROLLER_MODE);
        }

    }
}
