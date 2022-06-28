/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

### ALL OF THESE MUST BE DONE PRE-PATCH ###

-> True Kingdom Hearts I Command Menu
    @ 0x5AD283 => "zz0command.2dd" to "tt1command.2dd"
    @ 0x5AD298 => "zz0command.2dd" to "tt1command.2dd"

-> Allow Offline Play [DRM Compliant]
    @ 0x1493C8 => [75 27] to [EB 27]

-> Disable Title Screen FMV
    @ 0x2B066A => [F3 0F 11 0D 82 9B 86 00] to [90 90 90 90 90 90 90 90]

-> Rename Config File
    @ 0x5F7A10 => "/config1525.dat" to "/config_kh2.dat"

-> Voice Channel Mute
    @ 0x5A6E94 => [CD CC CC 3D] to [00 00 00 00]