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

-> Directory Forwarding
    @ 0x5FA690 => "\Kingdom Hearts\Configuration"
    @ 0x7110C0 => "Kingdom Hearts\Save Data\KHIIFM.png"
    @ 0x711100 => "Kingdom Hearts\Save Data\KH1.5+2.5_system.png"

-> Voice Channel Mute
    @ 0x5A6E94 => [CD CC CC 3D] to [00 00 00 00]

# No, I do not know why this jank worked but I ain't gonna question it.
-> Show Forms in the Inventory
    @ 0x348B0C => [0F BA EB 14] to [EB 45 90 90]
    @ 0x348B53 => [CC CC CC CC CC CC CC CC] to [81 C3 00 00 20 00 EB B5]

# This took WAY too long to do.
-> Remove EpicID Check from Save Files
    @ 0x470EF8 => [E8 43 0C CD FF] to [90 90 90 90 90]
    @ 0x470F05 => [75 0B] to [90 90]
    @ 0x470F10 => [74 30] to [EB 30]