<div align="center">

# Kingdom Hearts - Re:Fined
![License](https://img.shields.io/badge/License-GPL_v3.0-blue) 
![Version](https://img.shields.io/badge/Current_Version-Panacea--DLL-dark_lime)
[![Discord](https://img.shields.io/discord/978975211481354251?logo=Discord&logoColor=%23f0f0f0&label=Discord%20Server&color=6A7EC2)](https://discord.gg/kh-refined)
[![Patreon](https://img.shields.io/endpoint?url=https%3A%2F%2Fshieldsio-Patreon.vercel.app%2Fapi%2F%3Fusername%3Dtopaztk%26type%3DPatrons)](https://www.patreon.com/topaztk)  

![KHI](https://img.shields.io/badge/Kingdom_Hearts_I-Not_Live!-red)
![KHCOM](https://img.shields.io/badge/Re:_Chain_of_Memories-Not_Live!-red)
![KHII](https://img.shields.io/badge/Kingdom_Hearts_II-r010826--0320--FC-dark_lime)
![KHBBS](https://img.shields.io/badge/Birth_by_Sleep-Not_Live!-red)
![KHDDD](https://img.shields.io/badge/Dream_Drop_Distance-Not_Live!-red)

**Kingdom Hearts - Re:Fined** is a mod that aims to improve upon the Kingdom Hearts experience by introducing Quality of Life improvements, as well as fixing many of the glaring problems of this port.</div>
</div>

> [!WARNING]  
> Every game within the mod library is currently being rewritten. Releases may take time as a result. Thank you for understanding!

## The Premise

The project has three layers into it in every version of Kingdom Hearts:
- XXX-MAIN: All of the features that can be considered to be basic is included.
- MODULES: External modlets that add extra functionality to Re:Fined. Some included within MAIN. Can be added/removed at will.
- PACKS: External Mods that add customization to Re:Fined. More is explained down below!

The project is currently compatible with the worldwide releases on **Epic Games Store** and **Steam**.  
Compatibility for the Japanese releases is underway.  

The project also supports the Steam Deck and most all Linux distros. But setting it up on those may require extra steps. An install guide will be uploaded to the Wiki soon!

> [!IMPORTANT]  
> The games utilize OpenKH in order to add in new features and fix some bugs. Please refer to the [wiki](https://github.com/KH-ReFined/KH-ReFined/wiki) for **installation guides** and general how-to stuff.
> If you need extensive help, <ins>please do not make an issue</ins>, but rather, join the Discord and make a ticket!

## The Featureset

### All Games

| Error/Bug Fix | Enhancement | Description |
|---------------|-------------|-------------|
| :heavy_check_mark: | - | Disable cutscene framelimiter. [[1]](#reference) |
| :heavy_check_mark: | - | The game will no longer infinitely freeze while shutting down. |
| :heavy_check_mark: | - | Removed Save File Restrictions. [[2]](#reference) |
| :heavy_check_mark: | - | Fixed broken and/or missing sound effects and quotes. [[3]](#reference) |
| - | :heavy_check_mark: | Added an option to skip Copyright Notices [For legal reasons, they will always play on First Boot]. |
| - | :heavy_check_mark: | Added in the option to force controller prompts and camera at will. [[13]](#reference) |
| - | :heavy_check_mark: | Autosaving functionality, utilizing Slot #99. |
| - | :heavy_check_mark: | An RPC Engine is now present for use with Discord. |
| - | :heavy_check_mark: | Soft Reset implementation [L1 + L2 + R1 + R2 + SELECT; Customizable]. |
| - | :heavy_check_mark: | Advanced Ratio Support, aiming to improve the experience on 32:9, 21:9, 16:10, and 4:3 aspect ratios. |

### Kingdom Hearts II

| Error/Bug Fix | Enhancement | Description |
|---------|-------------|-------------|
| :heavy_check_mark: | - | Fixed Save Point VFX |
| :heavy_check_mark: | - | Restored Missing Boss Battle Dialogue & SFX |
| :heavy_check_mark: | - | Corrected Limit Form Names [[4]](#reference) |
| :heavy_check_mark: | - | Restored Right Stick Command Menu Control |
| :heavy_check_mark: | - | Restored Screen Shake |
| - | :heavy_check_mark: | Restored Roxas' Original Voicelines and Quotes |
| - | :heavy_check_mark: | The player can now retry all forced encounters. [[5]](#reference) |
| - | :heavy_check_mark: | Prepare & Retry [[6]](#reference) |
| - | :heavy_check_mark: | Can Now Skip Ending FMV |
| - | :heavy_check_mark: | Custom Shibuya Command Menu |
| - | :heavy_check_mark: | New GoA Gummi Ship Landing Spot |
| - | :heavy_check_mark: | Unpause Atlantica Mini-Games |
| - | :heavy_check_mark: | English Translation of Sora's Album Notes |
| - | :heavy_check_mark: | The ability to challenge Sephiroth again has been added [[10]](#reference) |
| - | :heavy_check_mark: | Pirate & Disney Outfits |
| - | :heavy_check_mark: | Next Form & Summon Level Indicators |
| - | :heavy_check_mark: | Ability to display Switch or Steam Controller prompts on Type 3 |
| - | :heavy_check_mark: | HUD Disable [[11]](#reference) |
| - | :heavy_check_mark: | Time Stop [[12]](#reference) |
| - | :heavy_check_mark: | Crown Engine (Sora's HUD Icon will now display a crown when a proof is obtained) |
| - | :heavy_check_mark: | Magic that is obtained outside of room transitions are registered immediately. [Useful for Rando/AP] |
| - | :heavy_check_mark: | Movement Upgrades obtained outside of level-ups are registered immediately. [Useful for Rando/AP] |
| - | :heavy_check_mark: | Sora/Roxas can now die when their HP is set to 0 by an outside factor.  [Needed for AP Deathlink] |

| Extra | Module/Pack | Description |
|---------|-------------|-------------|
| Encounter Plus | Module | Mobs will respawn one (1) room away, instead of two (2). (***New Abillity unlocked via Moogle Level 5***) [[7]](#reference) |
| Prolouge Skip | Module | Skip Roxas' Story and go straight into playing as Sora |
| Shortcut Forms | Module | Can set Drive Forms in the Shortcut Menu |
| Shortcut Sets | Module | Can utilize three different Shortcut Sets [[8]](#reference) |
| Auto-Attack | Module | Hold down the attack button to immediately start the next attack string [[15]](#reference) |
| Vanilla Heartless | Pack | Enable non-Final Mix Heartless Colors |
| Vanilla OST | Pack | Enable Original PlayStation 2 Soundtrack |
| Japanese VO | Pack | Enable Japanese Voice Overs [[9]](#reference) |
| German VO | Pack | Enable German Voice Overs [[9]](#reference) |
| French VO | Pack | Enable French Voice Overs [[9]](#reference) |
| Spanish VO | Pack | Enable Spanish Voice Overs [[9]](#reference) |
| Revert Outfits | Pack | Disables the custom Pirate/Disney World Outfits |

### Additional Content

| Mod Name | OpenKH Mod Link | Description | Other |
|---------|-------------|-------------|-------------|
| Vanilla OST | `KH-ReFined/KH2-VanillaMusicS1` *or* `KH-ReFined/KH2-VanillaMusicS2` [[13]](#reference) |  The Vanilla Music Pack for Kingdom Hearts II - Re:Fined. | Enable in Config Menu |
| Vanilla Heartless | `KH-ReFined/KH2-VanillaHeartlessS1` *or* `KH-ReFined/KH2-VanillaHeartlessS2` [[14]](#reference) | The Vanilla Heartless Pack for Kingdom Hearts II - Re:Fined. | Enable in Config Menu |
| Japanese Voice Over | `KH-ReFined/KH2-JapaneseVO` | The Japanese Audio Pack for Kingdom Hearts II - Re:Fined. | Enable in Config Menu |
| German Voice Over | `KH-ReFined/KH2-GermanVO` | The German Audio Pack for Kingdom Hearts II - Re:Fined. | Enable in Config Menu |
| French Voice Over | `KH-ReFined/KH2-FrenchVO` | The French Audio Pack for Kingdom Hearts II - Re:Fined. | Enable in Config Menu |
| Spanish Voice Over | `KH-ReFined/KH2-SpanishVO` | The Spanish Audio Pack for Kingdom Hearts II - Re:Fined. | Enable in Config Menu |

#### Reference

1. Cutscenes will by default lock the game to 30fps. This alleviates that.
2. The Save Data is normally locked to a specific account. Re:Fined removes this restriction.
3. Most sound effects are broken (Like Sephiroth's Deflect and Xigbar's Shots) and some boss quotes are missing (Genie Jafar's Beginning Quote, Saix's Desperation Move). Re:Fined fixes this.
4. In the HD remaster of Kingdom Hearts II, the localization team changed the translation of certain attacks to contrast with their usual translations from Japanese. E.g. Ragnarok -> Infinity. This fix reverts that decision.
5. Starting in Birth by Sleep, the player is given an option to retry a forced encounter instead of being sent to the room before. Re:Fined implements this feature to Kingdom Hearts II.
6. In Kingdom Hearts III, the player is able to press "Prepare and Retry" after dying to enter the loadout menu. Re:Fined implements this feature into Kingdom Hearts II.
7. In Kingdom Hearts, there was a convenient "Encounter Plus" ability to make the grinding experience more bearable. Re:Fined implements this ability into Kingdom Hearts II.
8. In Kingdom Hearts III, the player can have up to 3 shortcut sets and can cycle between them by using the DPAD. This feature is neat, so now it's in Kingdom Hearts II.
9. CrazyCatz00's English Patch for Kingdom Hearts II - Final Mix on the PS2 had an option in the menu to switch from and to English or Japanese voice acting. This mod reimplements that to the games, along with the other respective language paks chosen to be installed.
10. After defeating Sephiroth and obtaining Fenrir, you can now interact with an Absent Silhouette-like object in the Dark Depths to challenge Sepiroth again.
11. You can now hide all of the HUD (besides off-action menus and some oddities) by pressing L3. You have to set "allowNoHud" in reFined.cfg to true for this to work.
12. You can now halt the game to take pictures and cool action shots by pressing L3. You have to set "allowTimeStop" in reFined.cfg to true for this to work.
13. If you are ***not*** using another music pack for Re:Fined, you only have to install one pack.
14. If you are ***not*** using another resource pack for Re:Fined, you only have to install one pack.
15. To enable this module, please go into `openkh/mods/kh2/KH-ReFined/KH2-MAIN/mod.yml`, and delete the `#` in lines 22-25, and then rebuild.

## Support

All of Topaz' Kingdom Hearts projects are now his full time gig, and thus a Patreon exists to support him and any of his projects you may like.  
You can use the badge that is present at the start of this ReadME to check the Patreon out. There are benefits like private conversation, early BETA testing, and mentions in releases!  
Thank you for your consideration, all support is appreciated!

### Patreon supporters are also shown during the opening copyright screens!

## Credits

### All Games
- **KHOmega** for his countless hours of contributions to Re:Fined overall.
- **Televo** and **deltaemeraldin** for almost every asset Re:Fined uses.
- **Denhonator** for his work on Soft Reset, and Volume Mute in the past.
- **Xeeynamo** for the functions used to calculate the Save Checksum.
- **Alison** for her work on Enemy Journal Entries.
- **UtenaSW**, **dedede123**, and **Ruby-Chan** for the German translation.
- **Svenchu**, **Mikote11**, and **Abelatox** for the Spanish translation.
- **blobdash** and **toreizfotia** for the French translation.
- **iTeoh**, **kuxir**, and **atlas_0x** for the Italian translation.
- **SapphireSaphic** and **ALOE** for the upcoming Japanese translation.

### Kingdom Hearts
- **Some1fromthedark** for his contributions to the new Configuration System.

### Re: Chain of Memories
- **TheLegendOfLame** and **GICU** for their input on making the No Experience toggle.

### Kingdom Hearts 2
- **CrazyCatz00** for his contributions to Ultrawide Compatibility and Boss Quote Fixes.
- **TieuLink** for providing the Japanese voices for the Multi Audio Add-on.
- **DA** and **AntonioDePau** for their help with injecting ".a.jp" files for the Multi Audio Add-on.
- **Y2K** for his past work on the Prologue Skip.
- **gg3502** for his help in the SharpHook Engine.
- **Claire the Rabbit** for her help in fixing Panacea related issues.
- **TieuLink** for the origins of all Voice Packs.
- **Napstio** for implementing FrenchVO and GermanVO.
- **Jaster** for adding in Sora's Scribbles back.
- **cam ✰ your local ram!** for Autosave Sound
- **Zack (The Puppy) Fair** and **Mr. Puzzles** for the Shibuya Command Menu.
- **ATKOtter** for Morgan the Moogle (Autosave Asset)

## Third Party Libraries

Re:Fined makes use of the following third-party libraries for it's functionality:

[mINI](https://github.com/metayeti/mINI) - metayeti
  
[Discord Game SDK](https://docs.discord.com/developers/developer-tools/game-sdk)
