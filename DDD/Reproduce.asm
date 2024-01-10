/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR DDD!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

### ALL OF THESE MUST BE DONE PRE-PATCH ###

-> Allow Critical Mode Gameplay
    @ 0x4CFEF4 => [74 48] to [72 48]

-> Allow Offline Play [DRM Compliant]
    @ 0x6C8278 => [75 27] to [EB 27]

-> Rename Config File
    @ 0x832750 => "/config28.dat" to "/config_ddd.se"

-> Directory Forwarding
    @ 0x8352B8 => "\Kingdom Hearts\Configuration"
    @ 0x9B3E20 => "Kingdom Hearts\Save Data\KH3DHD.png"
    @ 0x9B3E70 => "Kingdom Hearts\Save Data\KH2.8_system.png"

-> Voice Channel Mute
    @ 0x81F53C => [CD CC CC 3D] to [00 00 00 00]

-> Remove EpicID Check from Save Files
    @ 0x6611D8 => [E8 93 F9 05 00] to [90 90 90 90 90]
    @ 0x6611E5 => [75 0B] to [90 90]
    @ 0x6611F0 => [74 30] to [EB 30]