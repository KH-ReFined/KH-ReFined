/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

### ALL OF THESE MUST BE DONE PRE-PATCH ###

-> Cutscene Limiter Removal
    @ 0x16C349 => [89 0D 7D FB 1C 02] to [90 90 90 90 90 90]

-> Cutscene Skip Fix
    @ 0x17545C => [0F 94 C0 C3] to [0F 9E C0 C3]
    @ 0x17A2A3 => [74 10] to [7E 10]

-> Allow Offline Play [DRM Compliant]
    @ 0x1273B8 => [75 27] to [EB 27]

-> Rename Config File
    @ 0x3DA458 => "/config1525.dat" to "/config_kh1.dat"

-> Voice Channel Mute
    @ 0x3D4EBC => [CD CC CC 3D] to [00 00 00 00]