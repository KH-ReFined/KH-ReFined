![combined_no_shadow_or_logo](https://github.com/TopazTK/KH-ReFined/assets/95656963/482d02f4-66f6-4b7e-8b73-06c1635da2ef)

## Download

Get the latest release by clicking the button below.

[![KH-ReFixed](https://img.shields.io/badge/Kingdom_Hearts_Re:Fined-Download-blue.svg)](https://github.com/TopazTK/KH-ReFined/releases)

### Installation

Installing Re:Fined is easy, even though it may seem like a monumential task. Please refer to [this page](SETUP.md) for full instructions.

If you're installing a pre-v4.xx version of Re:Fined, simply extract the archive into the folder with your game and rename the exe's to the vanilla names. Support for them is not provided per https://github.com/KH-ReFined/KH-ReFined/issues/117.

## Purpose

This project aims to improve the game in many ways whilst fixing the errors that the PC ports, and by extension, 1.5/2.5 had.  
Every legacy Kingdom Hearts game will get a Re:Fined counterpart.

## Game Status

You can see which games are supported, and which patches are live below:

[![Kingdom Hearts I](https://img.shields.io/badge/Kingdom%20Hearts%20I-Rewrite%20Required!-red)](https://github.com/TopazTK/KH-ReFixed/tree/mother/KH1)  
[![Kingdom Hearts II](https://img.shields.io/badge/Kingdom%20Hearts%20II-Live!-brightgreen)](https://github.com/TopazTK/KH-ReFixed/tree/mother/KH2)  
[![Birth by Sleep](https://img.shields.io/badge/Birth%20by%20Sleep-Rewrite%20Required!-red)](https://github.com/TopazTK/KH-ReFixed/tree/mother/BBS)  
[![Dream Drop Distance](https://img.shields.io/badge/Dream%20Drop%20Distance-Rewrite%20Required!-red)](https://github.com/TopazTK/KH-ReFixed/tree/mother/DDD)  
[![Melody of Memory](https://img.shields.io/badge/Melody%20of%20Memory-Not%20Present!-red)](https://github.com/TopazTK/KH-ReFixed/tree/mother/MOM)  

## Support

This project relies solely on Patrons for development. The Patrons' contributions go directly to living so that I have time to develop Re:Fined. Below you can find the button for support:
  
[![Patreon](https://img.shields.io/endpoint?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%2F%3Fusername%3Dtopaztk%26type%3Dpatrons)](https://www.patreon.com/topaztk)  

And of course, the contributions of these Patrons need acknowledgement specifically, since their lifetime support exceeds 200€ [As of 09.28.23]:  

- Xendra
- Snappygus
- Draxie
- Explode
- oli1428

## Discord

For all kinds of technical support and discussion, join the Discord Server:  

[![Discord](https://img.shields.io/badge/Kingdom_Hearts_Re:Fined-Discord-purple)](https://discord.gg/8JWQjuxqbB)

## Features

### All Games

| Error/Bug Fix | Enhancement | Description |
|---------------|-------------|-------------|
| :heavy_check_mark: | - | Disable cutscene framelimiter. [[1]](#reference) |
| :heavy_check_mark: | - | The Mouse Cursor will now follow a VirtualBox-style capture mechanism. |
| :heavy_check_mark: | - | The game will no longer infinitely freeze while shutting down. |
| :heavy_check_mark: | - | The game will no longer crash OBS after a resize. |
| :heavy_check_mark: | - | Added the ability to launch and play the game whilst offline. |
| :heavy_check_mark: | - | Removed savefile Epic ID check. [[4]](#reference) |
| - | :heavy_check_mark: | Voice channels will now be muted when set to "1" in the PC settings menu. |
| - | :heavy_check_mark: | Autosaving functionality, utilizing Slot #99. |
| - | :heavy_check_mark: | An RPC Engine is now present for use with Discord. |
| - | :heavy_check_mark: | The MP bar will no longer be present until Sora learns his first magic. [[2]](#reference)| 
| - | :heavy_check_mark: | Soft reset implementation (R1 + L1 + START + SELECT on v3.00; R2 + L2 by default on v4.00). Can be cancelled on v4.00 by pressing the CONFIRM button. |
| - | :heavy_check_mark: | Auto-updating. |
| - | :heavy_check_mark: | Create a game-specific configuration file named `config_xxx.dat` so that changes only have effects on a per-game basis. |
| - | :heavy_check_mark: | Moved savefiles to a more suitable place. Please move them if you had some already! [[3]](#reference) |

### Kingdom Hearts 1 (v3.00, v4.00 is T.B.A.)

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Fix an issue with cutscenes, preventing pausing + skipping during a fade effect. |
| :heavy_check_mark: | - | Fix the `Title Screen` option in the save menu so it no longer crashes the game. |
| :heavy_check_mark: | - | Restore the ability to exit the game from the title screen. |
| - | :heavy_check_mark: | Add a toggle in the Camp Menu to switch between original and a KH2-like improved field of view. |
| - | :heavy_check_mark: | Abilities can now be toggled with Triangle just like in KH2. | 
| - | :heavy_check_mark: | Autodetect the correct screen aspect ratio when in fullscreen. |
| - | :heavy_check_mark: | Added the ability to Auto Attack by holding down the configured attack button. It will trigger everywhere except the World Map. Toggleable through the config file. |
| - | :heavy_check_mark: | Added the ability to enforce either Controller or MKB Prompts through the config file. |

### Kingdom Hearts 2 (v4.00)

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Correct Limit Form's Limit names so that they are more accurately translated. [[5]](#reference) |
| :heavy_check_mark: | - | Restore the ability to exit the game from the title screen. |
| :heavy_check_mark: | - | The Credits FMV can now be skipped. |
| - | :heavy_check_mark: | Implemented 21:9/32:9 compatibility. |
| - | :heavy_check_mark: | Removed the FMV on the Title Screen. |
| - | :heavy_check_mark: | The player can now retry all forced encounters. [[6]](#reference) |
| - | :heavy_check_mark: | Added the ability to "Prepare and Retry". [[7]](#reference) |
| - | :heavy_check_mark: | Added the Encounter Plus ability to the game. [[8]](#reference) |
| - | :heavy_check_mark: | Added a warp point to Garden of Assemblage. |
| - | :heavy_check_mark: | Sora's Crown will now also appear in the HUD Image according to the crown he has. |
| - | :heavy_check_mark: | Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals plus one more language of choice. [[9]](#reference) |
| - | :heavy_check_mark: | Atlantica Musical segments can be unpaused now. |
| - | :heavy_check_mark: | Include an option upon selecting `New Game` to skip Roxas' introduction sequence. |
| - | :heavy_check_mark: | The player is now able to re-organize the Magic menu to their liking. This is done on a per-save basis. [[10]](#reference) |
| - | :heavy_check_mark: | Drive Forms will now appear in the inventory. |
| - | :heavy_check_mark: | Drive Forms can now be placed in the Shortcut Menu. |
| - | :heavy_check_mark: | Limit Form's Shortcuts can now be reorganized through the config file. |
| - | :heavy_check_mark: | In-game config options will now persist on a per-save basis. |
| - | :heavy_check_mark: | The `Kingdom Hearts` Command Menu setting is now more resembling of the menu from the first game. |
| - | :heavy_check_mark: | Added the ability to enforce either Controller or MKB Prompts through the config menu. |
| - | :heavy_check_mark: | Implemented the Resource Pack Engine. Peers can now make 3 types of Resource Packs for Re:Fined. |

### Kingdom Hearts - Dream Drop Distance (v3.00, v4.00 is T.B.A.)

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| - | :heavy_check_mark: | Add the ability to toggle the Drop Gauge. |
| - | :heavy_check_mark: | Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals. [[7]](#reference) |  

#### Reference

1. Cutscenes will by default lock the game to 30fps. This alleviates that.
2. Sora/Roxas immediately have MP Bars when the game begins, despite having no magic. This fix will now make said MP bar invisible until the player learns their first magic spell. (Usually Fire in KH1, Blizzard in KH2.)
3. The game originally saves everything to `Documents/KINGDOM HEARTS HD 1.5+2.5 ReMIX\Epic Games Store\[EGS ID]`. Re:Fined changes the path to `Documents\Kingdom Hearts\Save Data\[EGS ID]`.
4. The savefiles are DRM'd normally - meaning you can't share savefiles with other people. Re:Fined removes that DRM.
5. In the HD remaster of KH2, the localization team changed the translation of certain attacks to contrast with their usual translations from Japanese. E.g. Ragnarok -> Infinity. This fix reverts that awful decision. 
6. Starting in Birth by Sleep, the player is given an option to retry a forced encounter instead of being sent to the room before. Re:Fined implements this feature to KH2.
7. In KH3, the player is able to press "Prepare and Retry" after dying to enter the loadout menu. Re:Fined implements this feature into KH2.
8. In KH1, there was a convenient "Encounter Plus" ability to make the grinding experience more bearable. Re:Fined implements this ability into KH2.
9. CrazyCatz00's English Patch for Kingdom Hearts 2 - Final Mix on the PS2 had an option in the menu to switch from and to English or Japanese voice acting. This mod reimplements that to the games. There's also extra patches to have a 3rd language - it can be German, French or Spanish.
10. In KH3, the player can reorganize their magic menu. This feature is neat, so now it's in KH2. To do this, highlight your magic command and press L2+DPAD to shift around.

## Credits

### All Games
- **Televo** for almost every asset Re:Fined uses.
- **Denhonator** for his work on Soft Reset, Volume Mute, and the location of 1FM RAM Saves.
- **Xeeynamo** for the functions used to calculate the Save Checksum.
- **RukaPML** and **Ruby-Chan** for the German translation.
- **Svenchu** and **Mikote11** for the Spanish translation.
- **blobdash** for the French translation.
- **iTeoh** for the Italian translation.

### Kingdom Hearts 1
- **Denhonator** for his work on Cutscene Skip.

### Kingdom Hearts 2
- **CrazyCatz00** for his contributions to Ultrawide Compatibility.
- **TieuLink** for providing the Japanese voices for the Multi Audio Add-on.
- **DA** and **AntonioDePau** for their help with injecting ".a.jp" files for the Multi Audio Add-on.
- **Y2K** for his work on the Prologue Skip.
- **VladABDF** for his work on the Crown Engine.
- **Mikote111** for his work on Alternative Title Screens.
- **Some1fromthedark** for his contributions to the new Configuration System.
- **gg3502** for his help in the SharpHook Engine.
- **MainMemory** for her help in fixing Panacea related issues.

### Kingdom Hearts - Dream Drop Distance
- **Explode** for his work on Drop Bar Toggling.

## Third Party Libraries

[DiscordRPC by Lachee](https://github.com/Lachee/discord-rpc-csharp)  
