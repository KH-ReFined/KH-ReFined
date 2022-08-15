/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

### ALL OF THESE MUST BE DONE PRE-PATCH ###

-> Rename Config File
    @ 0x6B2920 => "/config1525.dat" to "/config_bbs.dat"

-> Directory Forwarding
    @ 0x6B54E8 => "\Kingdom Hearts\Configuration"
    @ 0x809820 => "Kingdom Hearts\Save Data\KHBbSFM.png"
    @ 0x809860 => "Kingdom Hearts\Save Data\KH1.5+2.5_system.png"

-> Allow Offline Play [DRM Compliant]
    @ 0x544588 => [75 27] to [EB 27]

-> Voice Channel Mute
    @ 0x687D0C => [CD CC CC 3D] to [00 00 00 00]