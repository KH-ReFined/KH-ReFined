using System.Numerics;
namespace ReFined.COM
{
    /// <summary>
    /// This class handles anything and everything that will happen *outside* of the game.
    /// An example to this is the Discord Rich Presence implementation.
    /// </summary>
    public static class External
    {
        static Discord.Activity RICH_PRESENCE;
        static Discord.ActivityTimestamps BEGIN_TIMESTAMP = new Discord.ActivityTimestamps();
        static Discord.ActivityManager ACTIVITY_MANAGER = Variables.DiscordClient.GetActivityManager();

        static int LAST_VALID_HP = 0x00;
        static int LAST_VALID_LVL = 0x00;

        public static void DiscordRPC()
        {
            var CAN_UPDATE = false;

            if (BEGIN_TIMESTAMP.Start == 0x00)
                BEGIN_TIMESTAMP.Start = ((DateTimeOffset)DateTime.UtcNow).ToUnixTimeSeconds();

            var _diffDictionary = new string[]
            {
                "Beginner Mode",
                "Standard Mode",
                "Proud Mode"
            };

            var _world = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _isBattle = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);

            var _playerPINT = Hypervisor.GetPointer64(Variables.PINT_PlayerInfo, [0x00]);
            var _saveFirstPINT = Hypervisor.GetPointer64(Variables.PINT_SaveTable1, [0x08, 0x00]);
            var _saveSecondPINT = Hypervisor.GetPointer64(Variables.PINT_SaveTable2, [0x08, 0x00]);

            var _currentHP = Hypervisor.Read<byte>(_playerPINT + 0x42C, true);
            var _currentLevel = Hypervisor.Read<byte>(_playerPINT + 0x444, true);

            var _playTime = Hypervisor.Read<int>(_saveFirstPINT + 0x300, true);

            var _playHours = Math.Floor(_playTime / 3600F);
            var _playMinutes = Math.Floor(_playTime % 60F);

            var _timeFormat = string.Format("{0}:{1}", _playHours.ToString("00"), _playMinutes.ToString("00"));

            var _playMode = Hypervisor.Read<byte>(_saveSecondPINT + 0x60, true);
            var _difficulty = Hypervisor.Read<byte>(_saveSecondPINT + 0x61, true);

            RICH_PRESENCE.Timestamps = BEGIN_TIMESTAMP;
            RICH_PRESENCE.ApplicationId = 837938492976660481;

            if (!Variables.IS_TITLE && Variables.IS_LOADED)
            {
                RICH_PRESENCE.Details = string.Format("HP: {0} | Level: {1} | Floor: {2}", _currentHP, _currentLevel, _world);
                RICH_PRESENCE.State = "Playing as " + (_playMode == 0x00 ? "Sora." : "Riku.");

                RICH_PRESENCE.Assets = new Discord.ActivityAssets
                {
                    LargeText = string.Format("In-Game Time: {0}", _timeFormat),
                    LargeImage = Variables.WORLD_DICT.ElementAtOrDefault(_world),

                    SmallText = _diffDictionary[_difficulty],
                    SmallImage = Variables.BATTLE_DICT[_isBattle]
                };

                CAN_UPDATE = true;
            }

            else if (Variables.IS_TITLE)
            {
                RICH_PRESENCE.Details = null;
                RICH_PRESENCE.State = null;

                RICH_PRESENCE.Assets = new Discord.ActivityAssets { LargeImage = "title" };

                CAN_UPDATE = true;
            }

            if (CAN_UPDATE)
            {
                ACTIVITY_MANAGER.UpdateActivity(RICH_PRESENCE, (result) =>
                {
                    if (result != Discord.Result.Ok)
                        Terminal.Log("Discord RPC threw an error whilst updating!", 1);
                });
            }

            Variables.DiscordClient.RunCallbacks();
        }
    }
}
