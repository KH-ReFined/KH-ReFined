# Kingdom Hearts - Re:Fixed

## Download

Get the latest release by clicking the button below.

[![KH-ReFixed](https://img.shields.io/badge/KH--ReFixed-Download-blue.svg)](https://github.com/TopazTK/KH-ReFixed/releases)

## Purpose

Because these PC ports of Kingdom Hearts deserve much better than what we got.  This project aims to improve the game in many ways whilst fixing the errors that the PC ports, and by extension, 1.5/2.5 had.  
  
Every legacy Kingdom Hearts game will get a Re:Fixed counterpart.

## Features

_Features are separated into two categories: "Error/Bug Fix" or "Enhancement". They are organized as follows:_

### All Games

| Error/Bug Fix | Enhancement | Description |
|---------------|-------------|-------------|
| :heavy_check_mark: | - | Disable cutscene framelimiter [[1]](#reference) |
| - | :heavy_check_mark: | Voice channels will now be muted when set to "1" in the PC settings menu |
| - | :heavy_check_mark: | An RPC Engine is now present for use with Discord |
| - | :heavy_check_mark: | Press START + SELECT to soft reset |
| - | :heavy_check_mark: | Creates a game-specific configuration file named `config_xxx.dat` so that changes only have effects on a per-game basis |

### Kingdom Hearts 1

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | The player can now open chests and examine objects while in combat |
| :heavy_check_mark: | - | Fixes an issue with cutscenes, preventing pausing + skipping during a fade effect |
| :heavy_check_mark: | - | Fix the `Title Screen` option in the save menu so it no longer crashes the game |
| - | :heavy_check_mark: | MP bar will no longer be present until Sora learns his first magic [[2]](#reference) | 
| - | :heavy_check_mark: | Add a toggle in the `Config` menu to switch between original and a KH2-like improved field of view |
| - | :heavy_check_mark: | Autodetect the correct screen aspect ratio when in fullscreen |

### Kingdom Hearts 2

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Fix an issue with 60fps being locked in certain scenarios | 
| :heavy_check_mark: | - | Corrects an issue with Limit Form's shortcuts being misplaced when O/B is used to confirm |
| :heavy_check_mark: | - | Corrects Limit Form's Limit names so that they are more accurately translated [[3]](#reference) |
| - | :heavy_check_mark: | The `Kingdom Hearts` Command Menu setting will now display a menu that is more resembling of the menu from the first game.
| - | :heavy_check_mark: | Include an option upon selecting `New Game` to skip Roxas' introduction sequence |
| - | :heavy_check_mark: | The player is now able to reorganize the Magic menu to their liking [[4]](#reference) | 

#### Reference

1. Cutscenes will by default lock the game to 30fps. This alleviates that.
2. As soon as the player picks their Dream Weapon, Sora has MP, despite having no magic. This fix will now make said MP bar invisible until the player learns their first magic spell. (Usually Fire.)
3. In the HD remaster of KH2FM, the localization team changed the translation of certain attacks to contrast with their usual translations from Japanese. E.g. Ragnarok -> Infinity.
4. In KH3, the player can reorganize their magic menu. This feature is neat, so now it's in KH2. To do this, highlight your magic command and press R2/RT to shift down and L2/LT to shift up.

## Credits

### All Games
- Televo for the icons used in Discord Rich Presence.

### Kingdom Hearts 1
- Denhonator for his work on Soft Reset, Cutscene Skip, and Volume Mute.
- KSX for his work on the MP Bar, and Field of View.
- TopazTK for everything else.

### Kingdom Hearts 2
- Stickman Sham for the idea that led to Magic Reorganization.
- Denhonator for his work on Soft Reset and Volume Mute.
- Drahsid for his work on the 60FPS Framelock Issue.
- Y2K for his work on the Roxas Skip.
- TopazTK for everything else.

## Disclaimer
In order to compile from source, a compiled version of [DiscordRPC by Lachee](https://github.com/Lachee/discord-rpc-csharp) must be injected through dnSpy, after being merged with Newtonsoft.JSON through ILMerge.

