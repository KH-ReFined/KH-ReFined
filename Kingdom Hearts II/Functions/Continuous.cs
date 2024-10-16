using ReFined.Common;
using ReFined.KH2.Information;

namespace ReFined.KH2.Functions
{
    public static class Continuous
    {
        public static ulong OffsetLimiter;

        /// <summary>
        /// Adjusts and edits the current framelimiter, and the function which controls it,
        /// according to the current chosen framerate.
        /// </summary>
        public static void OverrideLimiter()
        {

            // Fetch the framerate, and the first byte of the instruction.
            var _framerateRead = Hypervisor.Read<byte>(Variables.ADDR_Framerate);
            var _instructionRead = Hypervisor.Read<byte>(OffsetLimiter);

            // If the framerate is set to 30FPS, and the limiter is NOP'd out: Rewrite the instruction.
            if (_framerateRead == 0x00 && _instructionRead == 0x90)
            {
                Terminal.Log("Switched to 30FPS! Restoring the framelimiter.", 0);
                Hypervisor.WriteArray(OffsetLimiter, Variables.INST_FrameLimiter);
            }

            // Otherwise, if the framerate is not set to 30FPS, and the limiter is present:
            else if (_framerateRead != 0x00 && _instructionRead != 0x90)
            {
                Terminal.Log("Switched to 60FPS! Destroying the Framelimiter.", 0);

                // NOP the instruction.
                Hypervisor.WriteArray(OffsetLimiter, [ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 ]);

                // Set the current limiter to be off.
                Hypervisor.Write<byte>(Variables.ADDR_Framelimiter, 0x00);
            }
        }
    }
}
