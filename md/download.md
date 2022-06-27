Given below, you will find all the changelogs and release notes of every Re:Fixed version to date.

---

## Re:Fixed v2.90 - STABLE

**IMPORTANT:** Install this update manually! I am serious, the updater may not actually install this due to a bug!

### General

<li>Fixed an issue with the updater where it would fail on systems which used comma as the decimal point.</li>  
<li>Fixed an issue with the updater where it would update under some circumstances.</li>  
<li>Fixed some mouse operations.</li>

### Kingdom Hearts I

<li>Fixed the MULTI5 Engine so it doesn't crash.</li>

### Kingdom Hearts II

<li>Fixed Retry not resetting the EXP Earned.</li>

### <a style="color:#dd83f5" href="https://github.com/TopazTK/KH-ReFixed/releases/download/v2.90/ReFixed-v2.90.zip">Download Here!</a>

---

## Re:Fixed v2.80 - STABLE

### General

<li>Fixed massive oversights which caused undocumented behavior.</li>  
<li>"reFixed.ini" will now be auto-generated if non-existent.</li>  

### Kingdom Hearts II

<li>Fixed an issue where the camera would not work on KH2 if clicked back to the window.</li>

### <a style="color:#dd83f5" href="https://github.com/TopazTK/KH-ReFixed/releases/download/v2.80/ReFixed-v2.80.zip">Download Here!</a>

---

## Re:Fixed v2.75 - STABLE

### General

<li>Improved Hypervisor stability.</li>  
<li>Complete overhaul to the codebase.</li>  
<li>Fixed an issue where Discord RPC would not initialize.</li>  
<li>Fixed an issue where Autosave would not save correctly.</li>  
<li>Removed the shutdown prompt to allow the game to close and not give up whilst trying.</li>  
<li>Added support for the MULTI5 standard, OpenKH patched games, and text altering mods.</li>  
<li>Fixed an issue where mods that edited the text of the game would cause the engine to break.</li>  
<li>LuaBackendHook is now supported, but it is recommended that you use LuaFrontend instead.</li>  
<li>Reworked how SFX are handled within Re:Fixed. The features which use sound will now obey the Master and SFX Volume Control.</li>  
<li>The game now has a config file to set numerous things! Be sure to extract "reFixed.ini" to the game directory if you wanna make use of 'em.</li>  
<li>Rewrote how the game handles the mouse cursor. It will now act like a VirtualBox VM. Capture on boot, release when out of focus, recapture on clicking.</li>  
<li>Capture can be toggled with Ctrl + Alt.</li>  

### Kingdom Hearts I

<li>A few fixes to the Field of View toggle.</li>  
<li>Fixed the Auto-Save saving in battles or cutscenes, breaking the autosave made.</li>

### Kingdom Hearts II

<li>Improved stability to many of the functions.</li>  
<li>Fixed an issue where the MP bar would stay invisible in Limit Form.</li>  
<li>Fixed a bounce condition with many of the input-detecting functions.</li>  
<li>Fixed an issue with Discord RPC where the Title Screen would not be reported.</li>  
<li>Removed the removal of the 60FPS limiter due to the massive instability it caused. Sorry, Drahsid.</li>  
<li>Fixed an issue where gaining a new spell whilst having an organized magic menu broke the game.</li>  
<li>Added the functionality to retry battles instead of continuing them. You can switch between the two with the D-Pad.</li>  

### Birth by Sleep

<li>Improved Finisher Rename readability.</li>  
<li>Some under the hood shit that will put me at ease.</li>  
<li>Fixed a bounce condition with the Finisher Renaming.</li>  
<li>Fixed the Finisher Rename prompt showing up in unrelated places.</li>  
<li>Fixed the Auto-Save saving in battles or cutscenes, breaking the autosave made.</li>  

### <a style="color:#dd83f5" href="https://github.com/TopazTK/KH-ReFixed/releases/download/v2.75/ReFixed-v2.75.zip">Download Here!</a>

---

## Re:Fixed v2.10 - STABLE

### General

<li>Autosave now makes a sound to indicate a successful save.</li>  
<li>Fixed an oversight where KH1 and BBS ran at every tick, resulting in CPU Usage going over the roof.</li>  
<li>Moved the Discord RPC Engine to a separate task to ensure it does not meddle with the main Re:Fixed task.</li>  
<li>Fixed an issue with Autosave where it locked down all operations if a brand new Save PNG was used, generally on a first install.</li>  
<li>Fix an issue where the update system did not report the correct version with countries which use the comma as the decimal point.</li>  

### Kingdom Hearts I

<li>Bring back the toggle for Field of View since many liked it and were sad to see it go.</li>  
<li>Introduced a feature that allows one to toggle abilities simply by pressing Triangle/Y.</li>  

### Kingdom Hearts II

<li>The MP Bar will now stay invisible until Sora/Roxas gains a magic spell.</li>  
<li>Fixed an issue where Autosave saved on cutscenes. This caused some lockdown saves.</li>  
<li>Implementation of the Dual Audio engine, inspired by CrazyCatz00's work on KH2 PS2! Allowing one to switch between English and Japanese voice acting. Requires a world change to take full effect. </li> 
<li>Fixed an issue which caused the Limit Form shortcuts to flicker when X/A was set to confirm.</li>  
<li>Fixed an issue with Roxas' Story Skip where the skip triggered mid-story when Roxas' Story was set to NOT skip.</li>
<li>Implementation of the Festivity Engine, switching Sora, Donald, and Goofy to their Halloween Outfits from October 31st to November 2nd, and to their Christmas Outfits from December 24th to January 1st (inclusive).</li> 

### Birth by Sleep

<li>Implementation of the Dual Audio engine, inspired by CrazyCatz00's work on KH2 PS2! Allowing one to switch between English and Japanese voice acting. Requires a significant load to take full effect.</li>

### <a style="color:#dd83f5" href="https://github.com/TopazTK/KH-ReFixed/releases/download/v2.10/ReFixed-v2.10.zip">Download Here!</a>

---