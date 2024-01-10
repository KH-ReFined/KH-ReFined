## Features (v3.15)

### All Games

| Error/Bug Fix | Enhancement | Description |
|---------------|-------------|-------------|
| :heavy_check_mark: | - | Disable cutscene framelimiter. [[1]](#reference) |
| :heavy_check_mark: | - | The Mouse Cursor will now follow a VirtualBox-style capture mechanism. |
| :heavy_check_mark: | - | The game will no longer infinitely freeze while shutting down. |
| :heavy_check_mark: | - | The game will no longer crash OBS after a resize. |
| - | :heavy_check_mark: | Voice channels will now be muted when set to "1" in the PC settings menu. |
| - | :heavy_check_mark: | Autosaving functionality, utilizing Slot #99. (Disabled by default due to instability) |
| - | :heavy_check_mark: | An RPC Engine is now present for use with Discord. |
| - | :heavy_check_mark: | The MP bar will no longer be present until Sora learns his first magic. [[3]](#reference)|
| - | :heavy_check_mark: | Soft reset implementation (R1 + L1 + START + SELECT) |
| - | :heavy_check_mark: | Create a game-specific configuration file named `config_xxx.dat` so that changes only have effects on a per-game basis. |
| - | :heavy_check_mark: | Moved savefiles to a more suitable place. Please move them if you had some already! [[4]](#reference) |

### Kingdom Hearts 1

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Fixed an issue with cutscenes, preventing pausing + skipping during a fade effect. |
| :heavy_check_mark: | - | Fixed the `Title Screen` option in the save menu so it no longer crashes the game. |
| :heavy_check_mark: | - | Restore the ability to exit the game from the title screen. |
| - | :heavy_check_mark: | Added a toggle in the Camp Menu to switch between original and a KH2-like improved field of view. |
| - | :heavy_check_mark: | Abilities can now be toggled with Triangle just like in KH2. | 
| - | :heavy_check_mark: | Autodetect the correct screen aspect ratio when in fullscreen. |
| - | :heavy_check_mark: | Added the ability to Auto Attack by holding down the configured attack button. It will trigger everywhere except the World Map. Toggleable through the config file. |
| - | :heavy_check_mark: | Added the ability to enforce either Controller or MKB Prompts through the config file. |

### Kingdom Hearts - Birth by Sleep
#### BBS support is currently broken and you *will* experience issues, such as crashes.

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Fix an issue with 30fps being forced when talking to people. | 
| :heavy_check_mark: | - | Add back the ability to rename finishers. |
| - | :heavy_check_mark: | Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals. [[7]](#reference) |  

### Kingdom Hearts - Dream Drop Distance

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| - | :heavy_check_mark: | Added the ability to toggle the Drop Gauge. |
| - | :heavy_check_mark: | Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals. [[7]](#reference) | 
| - | :heavy_check_mark: | Added the ability to start with Critical difficulty without playing Proud first. |

#### Reference

1. Cutscenes will by default lock the game to 30fps. This alleviates that.
2. The savefiles are DRM'd normally - meaning you can't share savefiles with other people. Re:Freshed removes that DRM.
3. Sora/Roxas immediately have MP Bars when the game begins, despite having no magic. This fix will now make said MP bar invisible until the player learns their first magic spell. (Usually Fire in KH1, Blizzard in KH2.)
4. The game originally saves everything to `Documents\KINGDOM HEARTS HD 1.5+2.5 ReMIX\Epic Games Store\[EGS ID]`. Re:Freshed changes the path to `Documents\Kingdom Hearts\Save Data\[EGS ID]`.
5. In the HD remaster of KH2, the localization team changed the translation of certain attacks to contrast with their usual translations from Japanese. E.g. Ragnarok -> Infinity. This fix reverts that awful decision.
6. In KH3, the player can reorganize their magic menu. This feature is neat, so now it's in KH2. To do this, highlight your magic command and press L2+DPAD to shift around.
7. CrazyCatz00's English Patch for Kingdom Hearts 2 - Final Mix on the PS2 had an option in the menu to switch from and to English or Japanese voice acting. This mod reimplements that to the games.
8. The party will wear their Halloween costumes in the days between October 31st and November 2nd (inclusive). Whilst they wear their Christmas costumes in the days between December 24th and January 1st (also inclusive).
9. Starting in Birth by Sleep, the player is given an option to retry a forced encounter instead of being sent to the room before. Re:Freshed implements this feature to KH2 and it will replace the continue option. To revert back to and from the continue option, you can use the left and right D-Pad buttons.

## Credits (v3.15)

### All Games
- **TopazTK** for the original work on [Re:Fined](https://github.com/TopazTK/KH-ReFined).
- **Televo** for almost every asset Re:Freshed uses.
- **Denhonator** for his work on Soft Reset, Volume Mute, and the location of 1FM RAM Saves.
- **Xeeynamo** for the functions used to calculate the Save Checksum.
- **RukaPML** and **Ruby-Chan** for the German translation.
- **Svenchu** and **Mikote11** for the Spanish translation.
- **blobdash** for the French translation.
- **iTeoh** for the Italian translation.

### Kingdom Hearts 1
- **Denhonator** for his work on Cutscene Skip.

### Kingdom Hearts - Birth by Sleep
- **TopazTK** for everything so far.

### Kingdom Hearts - Dream Drop Distance
- **Explode** for his work on Drop Bar Toggling.

## Third Party Libraries (v3.15)

- [DiscordRPC by Lachee](https://github.com/Lachee/discord-rpc-csharp)
- [DotNetZip by DinoChiesa](https://github.com/DinoChiesa/DotNetZip)
- [Octokit.NET by GitHub](https://github.com/octokit/octokit.net)
- [NAudio by Mark Heath](https://github.com/naudio/NAudio)
