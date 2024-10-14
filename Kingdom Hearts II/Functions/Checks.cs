using ReFined.Common;
using ReFined.KH2.Information;

namespace ReFined.KH2.Functions
{
    public class Checks
    {
        /// <summary>
        /// Check to see if the game is in the title screen.
        /// </summary>
        /// <returns>"True" if it's in the title, "False" otherwise.</returns>
        public static bool CheckTitle() =>
            Hypervisor.Read<uint>(Variables.ADDR_Area) == 0x00FFFFFF
         || Hypervisor.Read<uint>(Variables.ADDR_Area) == 0x00000101
         || Hypervisor.Read<uint>(Variables.ADDR_Reset) == 0x00000001;
    }
}
