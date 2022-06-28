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

-> Allow Offline Play [DRM Compliant]
    @ 0x544588 => [75 27] to [EB 27]

-> Voice Channel Mute
    @ 0x687D0C => [CD CC CC 3D] to [00 00 00 00]