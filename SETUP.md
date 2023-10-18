# Installation
## Pre-requisities
To get started you need to download the following:

- [**KINGDOM HEARTS HD 1.5+2.5 ReMIX**](https://store.epicgames.com/en-US/p/kingdom-hearts-hd-1-5-2-5-remix)
   - If you are using an unofficial copy, please make sure your vanilla game works before installing Re:Fined!

- [**Latest build of Re:Fined**](https://github.com/TopazTK/KH-ReFined/releases)
   - Download the latest build of Re:Fined. 

- [**OpenKH**](https://github.com/OpenKH/OpenKh/releases)
   - Download `openkh.zip` from the latest build.

***Please also have the following downloaded and installed already if you do not already:***
- [**Visual C++ Redistributable Runtimes All-in-One**](https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/)
   - Download and run `install_all.bat`

## OpenKH Setup:

In this section, we will be setting up OpenKH's Mods Manager to patch the necessary files to run Re:Fined 4.00.

- Step 1: Right click `openkh.zip` and extract the folder somewhere where it won't accidently get deleted.
   - eg: `C:\Users\[UserName]\Documents\OpenKH\`

- Step 2: Go into your `openkh` folder and look for a program named `OpenKh.Tools.ModsManager.exe` and open it.

- Step 3: A window will pop up saying "Welcome to OpenKH Mods Manager"
   - You are going to click `Next >`, select `PC Release via Epic Game Store`, and then you are going to click the little folder icon to select the location of your game.
   - If you do not know where you have `KINGDOM HEARTS HD 1.5+2.5 ReMIX` installed, open up Epic Games Store, go into your library, right click `KINGDOM HEARTS HD 1.5+2.5 ReMIX`, click `Manage`, and then click the folder icon next to `Installation`. This will bring you to where it is downloaded.

- Step 4: Click `Next >` and choose wether you want to install OpenKH Panacea. For this tutorial, we ***will*** be installing and using Panacea.
   - OpenKH Panacea allows you to load your mods without modifying the game files.

- Step 5: - On the next screen, you may choose to `Launch via Epic Games`. If you would like to, check this and then click `Next >`.

- Step 6:
- Make sure `KH2-25GB` is checked, and then click `Extract game data`. This may take roughly fifteen minutes.
   - If you have already used OpenKH in the past, I recommend doing this step again with a completely fresh install of the game, that way any extracted data is guaranteed to not be accidently modified by a pre-existing modification.
   - If you encounter any errors extracting the game files, please either try again, or re-download your game!

- Step 7: After extraction, click `Next >` and then click `Finish`

When you are done, you should be at this screen!

![image](https://cdn.discordapp.com/attachments/1145843947571249152/1155913076319719577/image.png?ex=653fe271&is=652d6d71&hm=953a6d30bad30633bd4911fa1983364f45c61f4baf0f18e9a88e8dfd5e9ef07e&)

## Downloading and Patching in Re:Fined v4.xx's files using OpenKH:

In this section, we will be downloading the necessary patches to properly run Re:Fined v4.00.

- Step 1: In OpenKH, select the `Mods` button on the top, and then select `Install a new mod`.
   - Under `Add a new mod from GitHub`, we will be pasting in `KH-ReFined/KH2-MAIN`, as seen in the screenshot below, and then will click `Install`.
   - *Optional*: Feel free to also add `KH-ReFined/KH2-MultiAudio`, `KH-ReFined/KH2-VanillaOST`, and `KH-ReFined/KH2-VanillaEnemy` as well for extra Re:Fined v4.00 features!
   - *Note*: If you download additional add-ons, please make sure to place `KH2-MAIN` ***below*** the additional add-ons! 
   - *Note*: `KH-ReFined/KH2-MultiAudio` takes a ***very long time*** to download and install, as it is a rather large repository! If it looks like the program is unresponsive, it is simply because it is downloading the files.

![image](https://cdn.discordapp.com/attachments/1145843947571249152/1155913161036283965/image.png?ex=653fe285&is=652d6d85&hm=0df65e98672cc34ecefe4d07b656682f8f3034baf905d81425f5c3e6ce0c1166)

- Step 2: You will see `Kingdom Hearts II - Re:Fined v4.00` added into your mods list. Tick the little box so a checkmark appears in it.

- If you *did* install OpenKH Panacea, click `Mod Loader` ->  `Build Only`
   - If you have OpenKH set to run the game via EGS, select `Build and Run`. The game will load after it patches. 
   - The mod **will** persist after you close the game, so you don't have to re-open OpenKH.

- If you *did not* install OpenKH Panacea, click `Patching` -> `Build and Patch`
   - Sit back and let OpenKH do it's thing at this point. Depending on your PC, this might take between five to fifteen or more minutes. This may also take longer depending on if you downloaded and are implementing any additional addons as well.

- A debug window will then open, signaling that the game is currently being patched.

![](https://cdn.discordapp.com/attachments/1145843947571249152/1155913243232063558/image.png?ex=653fe299&is=652d6d99&hm=1e4d8e927f678fc75008346591f5440c914d67e718b8eeb233379ae16e6202bd)

![](https://cdn.discordapp.com/attachments/1145843947571249152/1155913243534037103/image.png?ex=653fe299&is=652d6d99&hm=33b1beb8e70c91f3ac9b52cec434afb8d2361f00520872bac3581213955dedc6)

## Setting up the .exe:

In this section, we will be placing our new Re:Fined v4.00 installation files into our game directory.

- Step 1: Locate your game installation.

- Step 2: In your game installation folder, backup your original .exe by either moving or renaming `KINGDOM HEARTS II FINAL MIX.exe` somewhere or something different.

- Step 3: Extract the Re:Fined .zip downloaded earlier, and place `KINGDOM HEARTS II FINAL MIX.exe` and `keystone.dll` into your game directory folder.
   - If you get a box asking you if you would like to overwrite any data, select `Yes`

Notes:
- The .exe's game icon may not update right away. This is due to Windows Icon caching and in no way affects Re:Fined.

This is what your game installation folder should now look like

![](https://cdn.discordapp.com/attachments/1156713816541909112/1156714283997085706/image.png?ex=6515f9a0&is=6514a820&hm=3f16c7f5ec498b9cbfb68e7b7a71c99300fc81161e41f8be250af53e9313607d&)

## Done!

Congratulations! Now you can just boot the game as you would normally and it will be there. You will know if it has worked when you select `NEW GAME` and you get the new options to select your controller prompts and skip the prologue! 

And there you go! Re:Fined v4.xx has been successfully installed!

## Updating Re:Fined v4.xx:

Updating is not a trivial task and does not take long to perform. 

- Step 1: Check if the updated version contains a note like this:

![image](https://github.com/SandeMC/KH-ReFined/assets/70141395/13287440-ee99-460d-85bd-4d2aea58aac4)

- Step 2:
- If the note *does exist*, open OpenKH, update the necessary patches (you can see them by looking for the `Update available` note) and [build the patch as you did previously](#downloading-and-patching-in-refined-v4xxs-files-using-openkh).
  - After that, download the updated version release and [extract the files again](#setting-up-the-exe).

- If the note *does not exist*, only download the updated version release and [extract the files again](#setting-up-the-exe).
