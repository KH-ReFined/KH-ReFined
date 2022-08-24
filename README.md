![combined_no_shadow_or_logo](https://user-images.githubusercontent.com/95656963/186497978-4a72da59-04d7-4e1f-932b-ad806a4a9adb.png)

## Download

Get the latest release by clicking the button below.

[![KH-ReFixed](https://img.shields.io/badge/KH--ReFixed-Download-blue.svg)](https://github.com/TopazTK/KH-ReFixed/releases)

## Purpose

This project aims to improve the game in many ways whilst fixing the errors that the PC ports, and by extension, 1.5/2.5 had.  
  
Every legacy Kingdom Hearts game will get a Re:Fined counterpart.

## Game Status

You can see which games are supported, and which patches are live below:
  
[![Kingdom Hearts I](https://img.shields.io/badge/Kingdom%20Hearts%20I-Live!-brightgreen)](https://github.com/TopazTK/KH-ReFixed/tree/mother/KH1)  
[![Kingdom Hearts II](https://img.shields.io/badge/Kingdom%20Hearts%20II-Live!-brightgreen)](https://github.com/TopazTK/KH-ReFixed/tree/mother/KH2)  
![Chain of Memories](https://img.shields.io/badge/Chain%20of%20Memories-Not%20Present!-red)  
[![Birth by Sleep](https://img.shields.io/badge/Birth%20by%20Sleep-Live!-brightgreen)](https://github.com/TopazTK/KH-ReFixed/tree/mother/BBS)  
![Dream Drop Distance](https://img.shields.io/badge/Dream%20Drop%20Distance-Work%20in%20Progress!-yellow)  

## Wiki
Please refer to the [wiki](https://github.com/TopazTK/KH-ReFixed/wiki/Main-Wiki-Page) for all of your questions.

## Support
If you wish to support this project, consider becoming a Patron.  
  
[![Patreon](https://img.shields.io/endpoint?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%2F%3Fusername%3Dtopaztk%26type%3Dpatrons)](https://www.patreon.com/topaztk)

## Features

_Features are separated into two categories: "Error/Bug Fix" or "Enhancement". They are organized as follows:_ 
_All features indicating a version number will be released in that said version._

### All Games

| Error/Bug Fix | Enhancement | Description |
|---------------|-------------|-------------|
| :heavy_check_mark: | - | Disable cutscene framelimiter. [[1]](#reference) |
| :heavy_check_mark: | - | The Mouse Cursor will now follow a VirtualBox-style capture mechanism. |
| :heavy_check_mark: | - | The game will no longer infinitely freeze while shutting down. |
| :heavy_check_mark: | - | The game will no longer crash OBS after a resize. |
| - | :heavy_check_mark: | Voice channels will now be muted when set to "1" in the PC settings menu. |
| - | :heavy_check_mark: | Autosaving functionality, utilizing Slot #99. |
| - | :heavy_check_mark: | An RPC Engine is now present for use with Discord. |
| - | :heavy_check_mark: | The MP bar will no longer be present until Sora learns his first magic. [[2]](#reference)| 
| - | :heavy_check_mark: | Press R1 + L1 + START + SELECT to soft reset. |
| - | :heavy_check_mark: | Every game has an update agent built-in, so Re:Fined can keep itself updated. |
| - | :heavy_check_mark: | Create a game-specific configuration file named `config_xxx.dat` so that changes only have effects on a per-game basis. |

### Kingdom Hearts 1

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | The player can now open chests and examine objects while in combat. |
| :heavy_check_mark: | - | Fix an issue with cutscenes, preventing pausing + skipping during a fade effect. |
| :heavy_check_mark: | - | Fix the `Title Screen` option in the save menu so it no longer crashes the game. |
| - | :heavy_check_mark: | Add a toggle in the Camp Menu to switch between original and a KH2-like improved field of view. |
| - | :heavy_check_mark: | Abilities can now be toggled with Triangle just like in KH2. | 
| - | :heavy_check_mark: | Autodetect the correct screen aspect ratio when in fullscreen. |
| - | :heavy_check_mark: | ??? |


### Kingdom Hearts 2

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Correct an issue with Limit Form's shortcuts being misplaced when O/B is used to confirm. |
| :heavy_check_mark: | - | Correct Limit Form's Limit names so that they are more accurately translated. [[3]](#reference) |
| - | :heavy_check_mark: | The `Kingdom Hearts` Command Menu setting will now display a menu that is more resembling of the menu from the first game. |
| - | :heavy_check_mark: | Include an option upon selecting `New Game` to skip Roxas' introduction sequence. |
| - | :heavy_check_mark: | The player is now able to reorganize the Magic menu to their liking. [[4]](#reference) | 
| - | :heavy_check_mark: | Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals. [[6]](#reference) |  
| - | :heavy_check_mark: | The party will now switch up their attire at appropriate times. [[7]](#reference) | 
| - | :heavy_check_mark: | The player can now retry all forced encounters on command, and can choose to continue via the D-Pad. [[8]](#reference) |


### Kingdom Hearts - Birth by Sleep

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Fix an issue with 30fps being forced when talking to people. | 
| :heavy_check_mark: | - | Add back the ability to rename finishers. [[5]](#reference) |
| - | :heavy_check_mark: | Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals. [[6]](#reference) |  

#### Reference

1. Cutscenes will by default lock the game to 30fps. This alleviates that.
2. Sora/Roxas immediately have MP Bars when the game begins, despite having no magic. This fix will now make said MP bar invisible until the player learns their first magic spell. (Usually Fire in KH1, Blizzard in KH2.)
3. In the HD remaster of KH2, the localization team changed the translation of certain attacks to contrast with their usual translations from Japanese. E.g. Ragnarok -> Infinity. This fix reverts that awful decision. 
4. In KH3, the player can reorganize their magic menu. This feature is neat, so now it's in KH2. To do this, highlight your magic command and press L2/LT+DPAD to shift around.
5. In the PC and Xbox ports of Birth by Sleep, the ability to rename the finishers has been removed. This fix adds that feature back, bound to Tri/Y, popping up a window to allow for renames. (Before you judge me for using a window, this is basically how the PSP goes about doing this, too.)
6. CrazyCatz00's English Patch for Kingdom Hearts 2 - Final Mix on the PS2 had an option in the menu to switch from and to English or Japanese voice acting. This mod reimplements that to the games.
7. The party will wear their Halloween costumes in the days between October 31st and November 2nd (inclusive). Whilst they wear their Christmas costumes in the days between December 24th and January 1st (also inclusive).
8. Starting in Birth by Sleep, the player is given an option to retry a forced encounter instead of being sent to the room before. Re:Fined implements this feature to KH2 and it will replace the continue option. To revert back to and from the continue option, you can use the left and right D-Pad buttons.

## Installation

### Without Dual Audio
- Drag the EXE unto the game directory.
- Rename the dragged EXE to the name of the original ones (Ex. "KINGDOM HEARTS FINAL MIX" for KH1)
- Tada! You have installed it.

### With Dual Audio
- Drag the EXE unto the game directory.
- Rename the dragged EXE to the name of the original ones (Ex. "KINGDOM HEARTS II FINAL MIX" for KH2)
- Download the patch in https://bit.ly/DualAudioBBS for BBS, the one in https://bit.ly/DualAudioKH2 for KH2.  	
- Download KHPCPatchManager v1.2.0 or higher from https://github.com/AntonioDePau/KHPCPatchManager/releases.
- Patch both the games with the patches you downloaded. Instructions are at: https://github.com/AntonioDePau/KHPCPatchManager/blob/main/README.md
- Done! You now have Re:Fined with Dual Audio!

## Credits

### All Games
- Televo for the icons used in Discord Rich Presence and the Executables.
- Denhonator for his work on Soft Reset, Volume Mute, and the location of 1FM RAM Saves.
- Xeeynamo for the functions used to calculate the Save Checksum.
- Dekirai and RelaxedBatter for their immense testing.
- RukaPML for the German translation.
- Svenchu for the Spanish translation.
- SoraikoSan for the French translation.
- iTeoh for the Italian translation.

### Kingdom Hearts 1
- Denhonator for his work on Cutscene Skip.
- TopazTK for everything else.

### Kingdom Hearts 2
- Stickman Sham for the idea that led to Magic Reorganization.
- Krakrenzo for the idea that led to Retry Bettle.
- CrazyCatz00 for the inspiration that led to Dual-Audio.
- TieuLink for proviging the Japanese voices for Dual Audio.
- DA and AntonioDePau for their help with injecting ".a.jp" files for Dual-Audio.
- Drahsid for his work on the 60FPS Framelock Issue.
- Y2K for his work on the Roxas Skip.
- TopazTK for everything else.


### Kingdom Hearts - Birth by Sleep
- TopazTK for everything so far.

## Third Party Libraries

[DiscordRPC by Lachee](https://github.com/Lachee/discord-rpc-csharp)  
[DotNetZip by DinoChiesa](https://github.com/DinoChiesa/DotNetZip)  
[Octokit.NET by GitHub](https://github.com/octokit/octokit.net)
[NAudio by Mark Heath](https://github.com/naudio/NAudio)

