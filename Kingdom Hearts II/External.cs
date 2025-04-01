using KH2FML;
using System.Numerics;
namespace ReFined.KH2
{
    /// <summary>
    /// This class handles anything and everything that will happen *outside* of the game.
    /// An example to this is the Discord Rich Presence implementation.
    /// </summary>
    public static class External
    {
        static Discord.Activity RICH_PRESENCE;
        static Discord.ActivityTimestamps BEGIN_TIMESTAMP = new Discord.ActivityTimestamps();
        static Discord.ActivityManager ACTIVITY_MANAGER = Locals.DiscordClient.GetActivityManager();

        public static void DiscordRPC()
        {
            if (BEGIN_TIMESTAMP.Start == 0x00)
                BEGIN_TIMESTAMP.Start = ((DateTimeOffset)DateTime.UtcNow).ToUnixTimeSeconds();

            var _world = Hypervisor.Read<byte>(Variables.ADDR_Area);

            var _room = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
            var _round = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x02);
            var _event = Hypervisor.Read<ushort>(Variables.ADDR_Area + 0x04);

            var _currentHP = Hypervisor.Read<byte>(Variables.ADDR_PlayerStats);
            var _currentMP = Hypervisor.Read<byte>(Variables.ADDR_PlayerStats + 0x180);

            var _difficulty = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x2498);
            var _currentForm = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x3524);
            var _currentLevel = Hypervisor.Read<byte>(Variables.ADDR_SaveData + 0x24FF);

            var _playTime = Math.Floor(Hypervisor.Read<int>(Variables.ADDR_SaveData + 0x2444) / 60F);

            var _playHours = Math.Floor(_playTime / 3600F);
            var _playMinutes = Math.Floor((_playTime % 3600F) / 60F);
            
            var _timeFormat = string.Format("{0}:{1}", _playHours.ToString("00"), _playMinutes.ToString("00"));

            var _isUnderdrome = _world == 0x06 && _room == 0x09 && (_event >= 0xBD && _event <= 0xC4);

            RICH_PRESENCE.ApplicationId = 833511404274974740;
            RICH_PRESENCE.Timestamps = BEGIN_TIMESTAMP;

            if (!Variables.IS_TITLE)
            {
                RICH_PRESENCE.Details = _isUnderdrome ? 

                Locals.RPC_TEXTS[2]
                .Replace("[0]", _currentHP.ToString())
                .Replace("[1]", _currentMP > 0 ? _currentMP.ToString() : Locals.RPC_TEXTS[4]).Replace("[2]", _round.ToString()) 
                
                :
                
                Locals.RPC_TEXTS[0]
                .Replace("[0]", _currentHP.ToString())
                .Replace("[1]", _currentMP > 0 ? _currentMP.ToString() : Locals.RPC_TEXTS[4]);

                RICH_PRESENCE.State = Locals.RPC_TEXTS[1]
                                      .Replace("[0]", _currentLevel.ToString())
                                      .Replace("[1]", _currentForm == 0x00 ? "N/A" : (_currentForm == 0x07 ? "Mickey" : Locals.FORM_TEXTS[_currentForm - 0x01]));

                RICH_PRESENCE.Assets = new Discord.ActivityAssets
                {
                    LargeText = Locals.RPC_TEXTS[3].Replace("[0]", _timeFormat),
                    LargeImage = Locals.MIRAGE_ARENA == true && _world == 0x0B ? "ma" :
                                 (_isUnderdrome ? Locals.CUP_DICT.ElementAtOrDefault(_event < 0xC1 ? _event - 0xBD : _event - 0xC1) : 
                                                  Locals.WORLD_DICT.ElementAtOrDefault(_world)),

                    SmallText = Locals.MODE_TEXTS[_difficulty],
                    SmallImage = Locals.BATTLE_DICT[(byte)Variables.BATTLE_MODE]
                };
            }

            else
            {
                RICH_PRESENCE.Details = null;
                RICH_PRESENCE.State = null;

                RICH_PRESENCE.Assets = new Discord.ActivityAssets { LargeImage = "title" };
            }

            ACTIVITY_MANAGER.UpdateActivity(RICH_PRESENCE, (result) =>
            {
                if (result != Discord.Result.Ok)
                    Terminal.Log("Discord RPC threw an error whilst updating!", 1);
            });

            Locals.DiscordClient.RunCallbacks();
        }
    }
}
