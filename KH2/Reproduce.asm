/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

### ALL OF THESE MUST BE DONE PRE-PATCH ###

-> Fix Framelimiter

VERSION 0.0:
    @0x1190CF => [C7 83 98 59 00 00 00 20 00 00] to [90 90 90 90 90 90 90 90 90 90]
VERSION 0.1:
    @0x1190DF => [C7 83 A0 59 00 00 00 20 00 00] to [90 90 90 90 90 90 90 90 90 90]

-> True Kingdom Hearts I Command Menu

VERSION 0.0:
    @ 0x5AD083 => "zz0command.2dd" to "tt1command.2dd"
    @ 0x5AD098 => "zz0command.2dd" to "tt1command.2dd"
VERSION 0.1:
    @ 0x5AD283 => "zz0command.2dd" to "tt1command.2dd"
    @ 0x5AD298 => "zz0command.2dd" to "tt1command.2dd"

-> Rename Config File

VERSION 0.0:
    @ 0x5F7810 => "/config1525.dat" to "/config_kh2.dat"
VERSION 0.1:
    @ 0x5F7A10 => "/config1525.dat" to "/config_kh2.dat"

-> Voice Channel Mute

VERSION 0.0:
    @ 0x5A6C94 => [CD CC CC 3D] to [00 00 00 00]
VERSION 0.1:
    @ 0x5A6E94 => [CD CC CC 3D] to [00 00 00 00]