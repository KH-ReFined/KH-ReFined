# Kingdom Hearts - Re:Fixed

## Why?

Because these PC ports of Kingdom Hearts deserve much better than what we got.  This project aims to improve the game in many ways whilst fixing the errors that the PC ports, and by extension, 1.5/2.5 had.  
  
Every legacy Kingdom Hearts game will get a Re:Fixed counterpart.

## Features

_(Features presented with the suffix of [E] are bug or error fixes, while those with [I] are improvements.)_

### All Games
- Disable the framelimiter in cutscenes. [E]
- Voice channels will now be muted when set to "1". [I]
- An RPC Engine is now present for Discord Activities. [I]
- Add a feature that allows soft resetting upon pressing START+SELECT. [I]
- The config for the game is now named "config_xxx.dat" so that any changes do not affect other games. [I]

### Kingdom Hearts 1
- The MP Bar will not be present until Sora learns a spell. [I]
- Opening Chests and Examining Objects can now be done in battle. [E]
- Add in a toggle to the Config menu which toggles between KH1 and KH2 style Field of View. [I]
- Fix an issue with the cutscenes, preventing pause and skipping whilst they have a fade effect. [E]
- The aspect ratio will now be adjusted upon entering Fullscreen. (4:3, 16:9, 16:10, 29:9, and 32:9) [I]
- Fix the Title Screen option on the Save Menu so it actually returns to the Title Screen instead of crashing the game. [E]

### Kingdom Hearts 2
- Fix an issue with 60FPS and disable the framelimiter in cutscenes. [E]
- When beginning a new game, there is now an opiton to skip Roxas' Story. [I]
- In the Shortcuts Menu, Limit Form's Limits are now where they are supposed to be. [E]
- Limit Form's Limits now have their actual names instead of the poorly translated ones. [E]
- The player is able to reorganize the Magic menu to their liking with R2 to shift down and L2 to shift up. [I]
- The "Kingdom Hearts I" Command Menu will now show a Command Menu that is actually imitating the KH1 style. [E]

## Credits

### All Games
- Tevolo for the icons used in Discord Rich Presence.
- Stickman Sham for the idea that led to Magic Reorganization in KH2FM.

### Kingdom Hearts 1
- Denhonator for his work on Soft Reset, Cutscene Skip, and Volume Mute.
- KSX for his work on the MP Bar, and Field of View.
- TopazTK for everything else.

## Kingdom Hearts 2
- Denhonator for his work on Soft Reset and Volume Mute.
- Drahsid for his work on the 60FPS Framelock Issue.
- Y2K for his work on the Roxas Skip.
- TopazTK for everything else.

## Disclaimer
In order to compile from source, a compiled version of [DiscordRPC by Lachee](https://github.com/Lachee/discord-rpc-csharp) must be injected through dnSpy, after being merged with Newtonsoft.JSON through ILMerge.

## Download

Releases tab has all the downloads.
