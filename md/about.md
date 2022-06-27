![](https://user-images.githubusercontent.com/95656963/171788826-e2049957-b00b-4502-87e9-6cf7df9d8efd.png)

## Purpose

This project aims to improve the game in many ways  
whilst fixing the errors that the PC ports, and by extension, 1.5/2.5 had.  
  
Every legacy Kingdom Hearts game will get a Re:Fixed counterpart.

## Game Status

You can see which games are supported, and which patches are live below:

\- Kingdom Hearts I - Live!  
\- Kingdom Hearts II - Live!  
\- Birth by Sleep - Live!  
\- Chain of Memories - Not Available!  
\- Dream Drop Distance - Work in Progress!   

## Support

If you wish to support this project, consider becoming a Patron at <span style="color:#dd83f5" href="https://www.patreon.com/topaztk">https://www.patreon.com/topaztk</span>!  
It really helps a lot!

## Features

\- Disable cutscene framelimiter.  
\- The Mouse Cursor will now follow a VirtualBox-style capture mechanism.  
\- The game will no longer infinitely freeze while shutting down.  
\- The game will no longer crash OBS after a resize.  
\- Voice channels will now be muted when set to "1" in the PC settings menu.  
\- Autosaving functionality, utilizing Slot #99.  
\- An RPC Engine is now present for use with Discord.  
\- The MP bar will no longer be present until Sora learns his first magic. 
\- Press R1 + L1 + START + SELECT to soft reset.  
\- Every game has an update agent built-in, so Re:Fixed can keep itself updated.  
\- Create a game-specific configuration file named `config_xxx.dat` so that changes only have effects on a per-game basis.  

### Kingdom Hearts 1

\- The player can now open chests and examine objects while in combat.  
\- Fix an issue with cutscenes, preventing pausing + skipping during a fade effect.  
\- Fix the `Title Screen` option in the save menu so it no longer crashes the game.  
\- Add a toggle in the Camp Menu to switch between original and a KH2-like improved field of view.  
\- Abilities can now be toggled with Triangle just like in KH2.  
\- Autodetect the correct screen aspect ratio when in fullscreen.  


### Kingdom Hearts 2

\- Correct an issue with Limit Form's shortcuts being misplaced when O/B is used to confirm.  
\- Correct Limit Form's Limit names so that they are more accurately translated.  
\- The `Kingdom Hearts` Command Menu setting will now display a menu that is more resembling of the menu from the first game.  
\- Include an option upon selecting `New Game` to skip Roxas' introduction sequence.  
\- The player is now able to reorganize the Magic menu to their liking.  
\- Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals.  
\- The party will now switch up their attire at appropriate times.  
\- The player can now retry all forced encounters on command, and can choose to continue via the D-Pad.  


### Kingdom Hearts - Birth by Sleep

\- Fix an issue with 30fps being forced when talking to people.  
\- Add back the ability to rename finishers.  
\- Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals.  


## Installation

<iframe width="800" height="500" src="https://www.youtube.com/embed/CZvX4w_w4Q8?controls=0" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Credits

### All Games
\- Televo for the icons used in Discord Rich Presence and the Executables.  
\- KSX for his work on the MP Bar enhancement.  
\- Denhonator for his work on Soft Reset, Volume Mute, and the location of 1FM RAM Saves.  
\- Xeeynamo for the functions used to calculate the Save Checksum.  
\- Dekirai and RelaxedBatter for their immense testing.  
\- RukaPML for the German translation.  
\- Svenchu for the Spanish translation.  
\- SoraikoSan for the French translation.  
\- iTeoh for the Italian translation.  
\- TopazTK for anything that is not mentioned.  

### Kingdom Hearts 1
\- Denhonator for his work on Cutscene Skip.

### Kingdom Hearts 2
\- Stickman Sham for the idea that led to Magic Reorganization.  
\- Krakrenzo for the idea that led to Retry Bettle.  
\- CrazyCatz00 for the inspiration that led to Dual-Audio.  
\- TieuLink for proviging the Japanese voices for Dual Audio.  
\- DA and AntonioDePau for their help with injecting ".a.jp" files for Dual-Audio.  
\- Drahsid for his work on the 60FPS Framelock Issue.  
\- Y2K for his work on the Roxas Skip.  

## Third Party Libraries

Re:Fixed makes use of some third party libraries whilst trying comply with their licenses.  
For more info, please visit the Re:Fixed repository. 
