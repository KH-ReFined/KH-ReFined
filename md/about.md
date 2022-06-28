<img src="https://user-images.githubusercontent.com/95656963/171788826-e2049957-b00b-4502-87e9-6cf7df9d8efd.png" width="75%" height="75%"/>

## Purpose

This project aims to improve the game in many ways  
whilst fixing the errors that the PC ports, and by extension, 1.5/2.5 had.  
  
Every legacy Kingdom Hearts game will get a Re:Fixed counterpart.

## Game Status

You can see which games are supported, and which patches are live below:

<li>Kingdom Hearts I - <a style="color:#40ff40">Live!</a></li>
<li>Kingdom Hearts II - <a style="color:#40ff40">Live!</a></li>
<li>Birth by Sleep - <a style="color:#40ff40">Live!</a></li>
<li>Chain of Memories - <a style="color:#ff4040">Not Available!</a></li>
<li>Dream Drop Distance - <a style="color:#ff8040">Work in Progress!</a></li>

## Support

If you wish to support this project, consider becoming  
a Patron at <a style="color:#dd83f5" href="https://www.patreon.com/topaztk">https://www.patreon.com/topaztk</a>! It really helps a lot!

## Features

<li>Disable cutscene framelimiter.</li>
<li>The Mouse Cursor will now follow a VirtualBox-style capture mechanism.</li> 
<li>The game will no longer infinitely freeze while shutting down.</li>
<li>The game will no longer crash OBS after a resize.</li>
<li>Voice channels will now be muted when set to "1" in the PC settings menu.</li>
<li>Autosaving functionality, utilizing Slot #99.</li>
<li>An RPC Engine is now present for use with Discord.</li>
<li>The MP bar will no longer be present until Sora learns his first magic.</li>
<li>Press R1 + L1 + START + SELECT to soft reset.</li>
<li>Every game has an update agent built-in, so Re:Fixed can keep itself updated.</li>
<li>Create a game-specific configuration file named `config_xxx.dat` so that changes only have effects on a per-game basis.</li>

### Kingdom Hearts 1

<li>The player can now open chests and examine objects while in combat.</li>
<li>Fix an issue with cutscenes, preventing pausing + skipping during a fade effect.</li>
<li>Fix the `Title Screen` option in the save menu so it no longer crashes the game.</li>
<li>Add a toggle in the Camp Menu to switch between original and a KH2-like improved field of view.</li>
<li>Abilities can now be toggled with Triangle just like in KH2.</li>
<li>Autodetect the correct screen aspect ratio when in fullscreen.</li>


### Kingdom Hearts 2

<li>Correct an issue with Limit Form's shortcuts being misplaced when O/B is used to confirm.</li>
<li>Correct Limit Form's Limit names so that they are more accurately translated.</li>
<li>The `Kingdom Hearts` Command Menu setting will now display a menu that is more resembling of the menu from the first game.</li>
<li>Include an option upon selecting `New Game` to skip Roxas' introduction sequence.</li>
<li>The player is now able to reorganize the Magic menu to their liking.</li>
<li>Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals.</li>
<li>The party will now switch up their attire at appropriate times.</li>
<li>The player can now retry all forced encounters on command, and can choose to continue via the D-Pad.</li>


### Kingdom Hearts - Birth by Sleep

<li>Fix an issue with 30fps being forced when talking to people.</li>
<li>Add back the ability to rename finishers.</li>
<li>Dual-Audio capabilities, allowing one to switch from and to English or Japanese vocals.</li>


## Installation

<iframe width="75%" height="75%" src="https://www.youtube.com/embed/CZvX4w_w4Q8?controls=1" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Credits

### All Games
<li>Televo for the icons used in Discord Rich Presence and the Executables.</li>
<li>KSX for his work on the MP Bar enhancement.</li>
<li>Denhonator for his work on Soft Reset, Volume Mute, and the location of 1FM RAM Saves.</li>
<li>Xeeynamo for the functions used to calculate the Save Checksum.</li>
<li>Dekirai and RelaxedBatter for their immense testing.</li>
<li>RukaPML for the German translation.</li>
<li>Svenchu for the Spanish translation.</li>
<li>SoraikoSan for the French translation.</li>
<li>iTeoh for the Italian translation.</li>
<li>TopazTK for anything that is not mentioned.</li>

### Kingdom Hearts 1
<li>Denhonator for his work on Cutscene Skip.</li>

### Kingdom Hearts 2
<li>Stickman Sham for the idea that led to Magic Reorganization.</li>
<li>Krakrenzo for the idea that led to Retry Bettle.</li>
<li>CrazyCatz00 for the inspiration that led to Dual-Audio.</li>
<li>TieuLink for proviging the Japanese voices for Dual Audio.</li>
<li>DA and AntonioDePau for their help with injecting ".a.jp" files for Dual-Audio.</li>
<li>Drahsid for his work on the 60FPS Framelock Issue.</li>
<li>Y2K for his work on the Roxas Skip.</li>

## Third Party Libraries

Re:Fixed makes use of some third party libraries whilst trying comply with their licenses.  
For more info, please visit the Re:Fixed repository. 
