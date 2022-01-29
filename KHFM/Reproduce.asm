/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

### ALL OF THESE MUST BE DONE PRE-PATCH ###

-> Cutscene Limiter Removal

VERSION 0.0:
    @ 0x16C199 => [89 0D 2D FD 1C 02] to [90 90 90 90 90 90]
VERSION 0.1:
    @ 0x16C349 => [89 0D 7D FB 1C 02] to [90 90 90 90 90 90]

-> Cutscene Skip Fix

VERSION 0.0:
    @ 0x1752AC => [0F 94 C0 C3] to [0F 9E C0 C3]
    @ 0x17A0F3 => [74 10] to [7E 10]
VERSION 0.1:
    @ 0x175452 => [0F 94 C0 C3] to [0F 9E C0 C3]
    @ 0x17A2A0 => [74 10] to [7E 10]

-> Examine and Open Chest Mid-Battle

VERSION 0.0:
    @ 0x2B0514 => [74 3A B9 03] to [7D 3A B9 03]
VERSION 0.1:
    @ 0x2B06C4 => [74 3A B9 03] to [7D 3A B9 03]

-> Rename Config File

VERSION 0.0:
    @ 0x3DA258 => "/config1525.dat" to "/config_kh1.dat"
VERSION 0.1:
    @ 0x3DA458 => "/config1525.dat" to "/config_kh1.dat"

-> Voice Channel Mute

VERSION 0.0:
    @ 0x3D4CBC => [CD CC CC 3D] to [00 00 00 00]
VERSION 0.1:
    @ 0x3D4EBC => [CD CC CC 3D] to [00 00 00 00]