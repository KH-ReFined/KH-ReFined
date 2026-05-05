#define _CRT_SECURE_NO_WARNINGS

#include <cassert>

#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <discord.h>

#include "axa.h"
#include "area.h"
#include "areainfo.h"
#include "cache_buff.h"
#include "binarc.h"
#include "cmconfig.h"
#include "command_draw.h"
#include "command_elem.h"
#include "hookconfig.h"
#include "converter.h"
#include "egs.h"
#include "exp.h"
#include "event.h"
#include "field2dd.h"
#include "field.h"
#include "file.h"
#include "form_level.h"
#include "friend.h"
#include "gauge.h"
#include "hardpad.h"
#include "info_base.h"
#include "information.h"
#include "item.h"
#include "itempic.h"
#include "jumpeffect.h"
#include "lockon.h"
#include "magic.h"
#include "item_param.h"
#include "member.h"
#include "member_table.h"
#include "menu.h"
#include "message.h"
#include "obj2d.h"
#include "objentry.h"
#include "panacea_alloc.h"
#include "party.h"
#include "pax.h"
#include "region.h"
#include "sequence.h"
#include "shake.h"
#include "save_indicator.h"
#include "softreset.h"
#include "sora.h"
#include "sound.h"
#include "iteminfo.h"
#include "sprite.h"
#include "steam.h"
#include "title.h"
#include "treasure_info.h"
#include "vsync.h"
#include "weapon.h"
#include "weapon_mset.h"
#include "weapon_entry.h"
#include "world.h"
#include "item_table.h"
#include "voice.h"
#include "messagedraw.h"
#include "select.h"
#include "spritemessage.h"
#include "hookintro.h"
#include "field.h"
#include "sheet.h"
#include "fvector.h"
#include "cmdata.h"

#include "memorymgr.h"
#include "continue_menu.h"

#include "ini.h"


using namespace std;
using namespace discord;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

multimap<uint8_t, void(*)(), std::greater<uint8_t>> _execModule;
multimap<uint8_t, void(*)(const wchar_t*), std::greater<uint8_t>> _initModule;

map<string, void(*)()> FUNCTION_ARRAY;

wchar_t* MOD_PATH;

bool IS_STEAM = false;
bool INITIALIZED = false;

bool IS_FASTBOOT = false;
bool IS_NOASPECT = false;

bool DEBOUNCE_HUDSTOP = false;

bool IS_RESETING = false;

uint16_t CURRENT_MUSIC = 0xFFFF;

bool TRANSFER_FIELD = false;
bool TRANSFER_BATTLE = false;

char* FIELD_ALLOC = nullptr;
char* BATTLE_ALLOC = nullptr;

uint16_t CURRENT_RESOURCE = 0xFFFF;

string CURRENT_AUDIO = "voice/us/battle";

bool QUEUE_VSB = false;
int CURRENT_VSB = 0x0000;

char* ALLOCATE_VSB = nullptr;

uint32_t PAST_LOCKON;
uint8_t LOCKON_TYPE = 0x00;

bool LOCKON_PLAY = false;

vector<uint8_t> LOCKON_VANILLA;
vector<uint8_t> LOCKON_EDITED;

char* LOCKON_FUNCTION = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8B", "xxxxxxxxxxxxxxxx");
char* LOCKON_FLOATS = SignatureScan<char*>("\x00\x00\x80\xBF\xF3\x04\xB5\xBF\x00\x00\x00\x00\x00\x00\xE0\xBF", "xxxxxxxxxxxxxxxx");

char* LOCKON_CHANGE = ResolveFunctionFromCall<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8B", "xxxxxxxxxxxxxxxx", 0x16A);
uint32_t* LOCKON_TARGET = ResolveRelativeAddress<uint32_t*>(LOCKON_CHANGE, 0x0B);

vector<char> LIMITER_FUNCTION;

Core* Discord;

Activity RICH_PRESENCE;
Timestamp BEGIN_TIMESTAMP;

vector<string> TEXT_PRESENCE;
vector<string> TEXT_MODE;
vector<string> TEXT_FORM;

bool IS_MIRAGE;
bool IS_IN_FORM = false;

char* ADJUST_GLOW_FUNCTION = SignatureScan<char*>("\x4C\x8B\xDC\x49\x89\x5B\x20\x55\x56\x57\x41\x54\x41\x56\x49\x8D\xAB\x18\xF2\xFF\xFF\x48\x81\xEC\xC0\x0E\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
char* INIT_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x38\xE8\x00\x00\x00\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x0F\x10\x54\x24\x20\xF3\x0F\x10\x48\x10\xF3\x0F\x10\x40\x14\x0F\xC6\xD2\xD2\xF3\x0F\x10\xD1\x0F\xC6\xD2\x27\xF3\x0F\x10\xD0\x0F\xC6\xD2\x39\x0F\x11\x90\x5C\x01\x00\x00\x48\x83\xC4\x38\xC3", "xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* ADJUST_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x78\x0F\x29\x74\x24\x60\x0F\x28\xF1\x0F\x29\x7C\x24\x50\x0F\x28\xFA\x44\x0F\x29\x44\x24\x40\x44\x0F\x28\xC3\x44\x0F\x29\x4C\x24\x30\x44\x0F\x28\xC8\xE8\x00\x00\x00\x00\x45\x0F\xC6\xC9\xE1\xF3\x44\x0F\x10\xCE\x0F\x28\x74\x24\x60\x45\x0F\xC6\xC9\xC6\xF3\x44\x0F\x10\xCF\x0F\x28\x7C\x24\x50\x45\x0F\xC6\xC9\x27\xF3\x45\x0F\x10\xC8\x44\x0F\x28\x44\x24\x40\x45\x0F\xC6\xC9\x39\x44\x0F\x11\x88\x5C\x01\x00\x00\x44\x0F\x28\x4C\x24\x30\x48\x83\xC4\x78\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

bool SHAKE_WRITTEN;

vector<char> ADJUST_GLOW_ARRAY;
vector<char> INIT_VIEWPORT_ARRAY;
vector<char> ADJUST_VIEWPORT_ARRAY;

char* VIEWPORT3D_ADDR = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

bool* PROMPT_MODE;
char* PROMPT_INSTRUCTION = SignatureScan<char*>("\xC7\x05\x00\x00\x00\x00\x01\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00", "xx????xxxxx????xx????");

vector<uint32_t> CHECKSUM_TABLE;

bool SYSTEM_WRITTEN = false;
bool SYSTEM_LOADED = false;
bool SAVE_INITIATE = false;
bool SAVE_ROUNDBACK = false;

int SAVE_ITERATOR = 0;

AREA::INFO SAVE_AREA;

uint8_t SAVE_REACTION = 0xEB;
int SAVE_REACTION_FRAME = 0;

char* SAVE_REACTION_FUNCTION = SignatureScan<char*>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\xC8\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x3F\x48\x8B\xD9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????");

uint32_t MAGIC_FIRST;
uint16_t MAGIC_SECOND;

map<uint32_t, char*> MAGIC_FILES;

vector<uint16_t> ABILITY_ARRAY;

bool IS_PICTURE_EDITED = false;

char* PICTURE_APPEAR_FUNC = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x48\x63\x41\x34\x48\x8B\xD9\x3B\x41\x30\x0F\x84\x00\x00\x00\x00\x48\x69\xD0\x60\x05\x00\x00\x48\x89\x7C\x24\x48", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx");

bool IS_DEAD = false;

uint32_t POSITIVE_ASPECT_OFFSET = 0x55;
uint32_t NEGATIVE_ASPECT_OFFSET = 0xFFFFFFAB;

vector<char> INSTRUCTION_LIMIT_ASPECT;

char* VIEWPORT_LIMIT = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x00\x00\x00\x00\xF3\x0F\x10\x15\x00\x00\x00\x00\x48\x8B\xC8\xF3\x0F\x10\x25\x00\x00\x00\x00\xF3\x0F\x5D\x50\x28", "xxxxxxxxxx????xxxx????xxxxxxx????xxxxx");
char** RADAR_STRUCT = ResolveRelativeAddress<char**>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x56\x57\x41\x54\x48\x83", "xxxxxxxxxxxxxxxx", 0xD4);

vector<uint8_t> INST_CAMPINIT;
vector<uint8_t> INST_CAMPBITWISE;

vector<uint8_t> INST_MAPJUMPTASK;
vector<uint8_t> INST_CONTINUELOAD;

char* CMENU_OFFSET = SignatureScan<char*>("\x48\x8B\xC4\x48\x81\xEC\x88\x00\x00\x00\x48\x89\x58\x18\xBA\x02\x00\x00\x00\x48\x89\x68\xF8\x48\x89\x70\xF0", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* CMENUINIT_OFFSET = SignatureScan<char*>("\x66\x44\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x44\x88\x35\x00\x00\x00\x00\x0F\x95\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\xC7\x44\x24\x30\x8C\x00\x00\x00", "xxxx????x????xxxxx????xxx????x????xxx????xxxxxxxx");

uint8_t RETRY_MODE;
bool RETRY_BLACKLIST;

vector<uint8_t> RETRY_STATE;

bool HADES_ESCAPE;
bool HADES_CHANGED;

uint8_t HADES_ITERATOR = 0xFF;

ReFined::Continue::Entry RETRY_ENTRY(0x0002, 0x8AB1);
ReFined::Continue::Entry PREPARE_ENTRY(0x0002, 0x5727);

uint8_t* COMMAND_TYPE = ResolveRelativeAddress<uint8_t*>("\x48\x83\xEC\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9\x00\x00\x00\x00\xCC\xCC\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxxx????x????xxx????xxx????xxx????xxxxx????xxxxx????xxx????x", 0x1A);

void(*ITEM_COMMIT)() = nullptr;

bool KEYBLADE_DEBOUNCE = false;
bool PENDING_KEYBLADE_UPDATE = false;
bool CAN_PROCESS_FORM_KEYBLADES = false;

uint16_t TARGET_KEYBLADE = 0x0000;
uint16_t TARGET_CURRENT_FORM_KEYBLADE = 0x0000;

char* CURRENT_SUBMENU = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????x????", 0xF0);
char** MENU_ITEMS = ResolveRelativeAddress<char**>("\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxx????xxx????xxx", 0x26);

char TITLE_FILENAME[0x30];

bool RETRIBUTION_INIT = false;

bool HAS_ABSOLUTION = false;
bool HAS_RETRIBUTION = false;

uint16_t PARAM_ABSOLUTION = UINT16_MAX;
uint16_t PARAM_RETRIBUTION = UINT16_MAX;

uint16_t INDEX_ABSOLUTION = 0x0000;
uint16_t INDEX_RETRIBUTION = 0x0000;

bool DEBOUNCE_RETRIBUTION = false;

vector<uint16_t*> WEAPON_MEMORY;

bool SYNC_LIMIT;

// Configuration Values.

bool DISCORD_ENABLED = true;

bool ALLOW_NOHUD = false;
bool ALLOW_TIMESTOP = false;

uint8_t ROOM_AMOUNT = 3;
uint8_t SAVE_SLOT_OFFSET = 99;

uint16_t RESET_COMBO = YS::HARDPAD::BUTTONS::NONE;

// Function Block. Everything is here now :D

void SOFT_RESET()
{
    bool _canReset = *YS::COMMAND_DRAW::CommandDraw != 0x00 && *AREA::IsInMap && !*YS::TITLE::IsTitle && !*YS::MENU::IsMenu && RESET_COMBO != 0x00;

    // If the buttons are pushed, a reset can happen and it isn't happening:
    if (RESET_COMBO != YS::HARDPAD::BUTTONS::NONE && *YS::HARDPAD::Input == RESET_COMBO && _canReset && !IS_RESETING)
    {
        // Declare the reset is happening for timing purposes.
        IS_RESETING = true;

        // Initiate the fadeout for BGMs.
        SOUND::BGMFadeOut(200, 0x00);
        SOUND::BGMFadeOut(200, 0x01);

        // Initiate fade-to-black.
        dk::JUMPEFFECT::Out(0x01);
    }

    // If it's resetting, and the fade is complete:
    if (IS_RESETING && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x04)
    {
        // Set the reset byte and declare we are no longer resetting.
        *(dk::SOFTRESET::RESET) = 0x01;
        IS_RESETING = false;
    }
}

void HANDLE_MUSIC()
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
    auto _fetchMusic = (_fetchConfig & 0x0080) == 0x0080 ? 0x0080 : ((_fetchConfig & 0x0100) == 0x0100 ? 0x0100 : 0x0000);

    if (*YS::TITLE::IsTitle)
        CURRENT_MUSIC = 0xFFFF;

    if (*AREA::IsInMap)
    {
        if (CURRENT_MUSIC == 0xFFFF)
            CURRENT_MUSIC = _fetchMusic;

        else if (CURRENT_MUSIC != _fetchMusic)
        {
            if (_fetchMusic == 0x0080 && !YS::MESSAGE::GetData(0x571B))
            {
                _fetchMusic -= 0x0080;
                _fetchConfig -= 0x0080;

                *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0080;
            }

            if (_fetchMusic == 0x0100 && !YS::MESSAGE::GetData(0x571D))
            {
                _fetchMusic -= 0x0100;
                _fetchConfig -= 0x0100;

                *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0100;
            }

            string _fetchPath = _fetchMusic == 0x0080 ? "bgm_2nd/music%03d.win32.scd" : (_fetchMusic == 0x0100 ? "bgm_3rd/music%03d.win32.scd" : "bgm/music%03d.win32.scd");

            auto _fetchMode = *AREA::BattleStatus == 0x00 ? 0x00 : 0x01;

            auto _fetchVolumeStart = *reinterpret_cast<uint32_t*>(SOUND::CurrentMusic + 0x04);
            auto _fetchVolumeFinish = *reinterpret_cast<uint32_t*>(SOUND::CurrentMusic + 0x08);

            auto _fetchCurrentField = *reinterpret_cast<uint16_t*>(SOUND::CurrentMusic);
            auto _fetchCurrentBattle = *reinterpret_cast<uint16_t*>(SOUND::CurrentMusic + 0x10);

            char _fieldMusicPath[0x28];
            char _battleMusicPath[0x28];

            sprintf(_fieldMusicPath, _fetchPath.c_str(), _fetchCurrentField);

            if (YS::FILE::GetSize(_fieldMusicPath) == 0x00)
                sprintf(_fieldMusicPath, "bgm/music%03d.win32.scd", _fetchCurrentField);

            sprintf(_battleMusicPath, _fetchPath.c_str(), _fetchCurrentBattle);

            if (YS::FILE::GetSize(_battleMusicPath) == 0x00)
                sprintf(_battleMusicPath, "bgm/music%03d.win32.scd", _fetchCurrentBattle);

            auto _sizeField = YS::FILE::GetSize(_fieldMusicPath);
            auto _sizeBattle = YS::FILE::GetSize(_battleMusicPath);

            if (TRANSFER_FIELD)
                goto FIELD_AFTERMATH;

            if (TRANSFER_BATTLE)
                goto BATTLE_AFTERMATH;

            FIELD_ALLOC = (char*)malloc(_sizeField);

            if (FIELD_ALLOC != nullptr)
            {
                auto _loadField = YS::FILE::Read(_fieldMusicPath, FIELD_ALLOC);

                if (_fetchMode == 0x00 && _fetchVolumeStart != 0x00)
                    SOUND::KillBGM(0x00);

                SOUND::SetTransfer(0x00, 0x02, FIELD_ALLOC, _sizeField, nullptr, nullptr);
                TRANSFER_FIELD = true;
            }

        FIELD_AFTERMATH:

            if (*SOUND::IsTransferActive != 0x00)
                return;

            free(FIELD_ALLOC);
            TRANSFER_FIELD = false;

            if (_fetchMode == 0x00 && _fetchVolumeStart != 0x00)
                SOUND::StartBGM(0x00, 0x3000, 0x3000, 0x00);

            BATTLE_ALLOC = (char*)malloc(_sizeBattle);

            if (BATTLE_ALLOC != nullptr)
            {
                auto _loadBattle = YS::FILE::Read(_battleMusicPath, BATTLE_ALLOC);

                if (_fetchMode == 0x01 && _fetchVolumeStart != 0x00)
                    SOUND::KillBGM(0x01);

                SOUND::SetTransfer(0x01, 0x02, BATTLE_ALLOC, _sizeBattle, nullptr, nullptr);
                TRANSFER_BATTLE = true;
            }

        BATTLE_AFTERMATH:

            if (*SOUND::IsTransferActive != 0x00)
                return;

            free(BATTLE_ALLOC);
            TRANSFER_BATTLE = false;

            if (_fetchMode == 0x01 && _fetchVolumeStart != 0x00)
                SOUND::StartBGM(0x01, 0x3000, 0x3000, 0x00);

            CURRENT_MUSIC = _fetchMusic;
        }
    }

    else if (!*AREA::IsInMap && !*YS::TITLE::IsTitle && CURRENT_MUSIC != _fetchMusic)
    {
        if (CURRENT_MUSIC == 0xFFFF)
            CURRENT_MUSIC = _fetchMusic;

        else if (_fetchMusic == 0x0080 && !YS::MESSAGE::GetData(0x571B))
            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0080;

        if (_fetchMusic == 0x0100 && !YS::MESSAGE::GetData(0x571D))
            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0100;

        CURRENT_MUSIC = _fetchMusic;
    }
}

void HANDLE_RESOURCE()
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
    auto _fetchObject = (_fetchConfig & 0x0200) == 0x0200 ? 0x0200 : ((_fetchConfig & 0x0400) == 0x0400 ? 0x0400 : 0x0000);

    if (!*AREA::IsInMap)
    {
        if (!*YS::TITLE::IsTitle)
        {
            if (CURRENT_RESOURCE == 0xFFFF)
                CURRENT_RESOURCE = _fetchObject;

            if (_fetchObject == 0x0200 && !YS::MESSAGE::GetData(0x573C))
                *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0080;

            else if (_fetchObject == 0x0400 && !YS::MESSAGE::GetData(0x573E))
                *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0100;
        }

        else
            CURRENT_RESOURCE = 0xFFFF;
    }

    if (*AREA::IsInMap)
    {
        if (CURRENT_RESOURCE == 0xFFFF)
            CURRENT_RESOURCE = _fetchObject;

        if (_fetchObject == 0x0200 && !YS::MESSAGE::GetData(0x573C))
            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0080;

        else if (_fetchObject == 0x0400 && !YS::MESSAGE::GetData(0x573E))
            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6) -= 0x0100;

        else if (CURRENT_RESOURCE != _fetchObject && !*YS::MENU::IsMenu)
        {
            AREA::MapJump(AREA::Current, 0x01, 0x00, false);
            CURRENT_RESOURCE = _fetchObject;
        }
    }
}

void HANDLE_AUDIO()
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    string _constructPath = _fetchConfig & 0x0004 ? "voice/jp/battle" :
                           (_fetchConfig & 0x0008 ? "voice/es/battle" :
                           (_fetchConfig & 0x0010 ? "voice/de/battle" :
                           (_fetchConfig & 0x0020 ? "voice/bg/battle" : "voice/us/battle")));

    if (!*YS::TITLE::IsTitle)
    {
        if (_constructPath != CURRENT_AUDIO)
        {
            if (*YS::MENU::IsMenu)
            {
                SOUND::StreamAllStop(true);

                auto _fetchSora = *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable);

                char _loadBuff[64];

                string _pathAppend = _constructPath;
                sprintf(_loadBuff, _pathAppend.append("/%s.win32.scd").c_str(), _fetchSora == 0x5A ? "preview_roxas" : "preview_sora");

                auto _fetchSize = YS::FILE::GetSize(_loadBuff);

                if (_fetchSize)
                {
                    auto _allocLoad = (char*)malloc(_fetchSize);
                    YS::FILE::Read(_loadBuff, _allocLoad);

                    SOUND::PlayVSB(_allocLoad, _fetchSize, 0x3FAC, 0x00);
                    QUEUE_VSB = true;
                }
            }

            CURRENT_AUDIO = _constructPath;
        }

        else if (QUEUE_VSB && !*YS::MENU::IsMenu)
        {
            if (CURRENT_VSB == 0x0000)
                CURRENT_VSB = 0x0236;

            else if (CURRENT_VSB != 0x0000 && CURRENT_VSB <= 0x0238)
            {
                if (*SOUND::IsTransferActive != 0x00)
                    return;

                else
                {
                    if (ALLOCATE_VSB)
                    {
                        free(ALLOCATE_VSB);
                        ALLOCATE_VSB = nullptr;
                    }

                    CURRENT_VSB++;
                }
            }

            else if (CURRENT_VSB == 0x0239)
            {
                QUEUE_VSB = false;
                CURRENT_VSB = 0x0000;
                return;
            }

            auto _fetchObject = YS::OBJENTRY::Get(CURRENT_VSB);

            if (_fetchObject)
            {
                char _loadBuff[64];

                auto _entryPart = *reinterpret_cast<uint16_t*>(_fetchObject + 0x4C);
                auto _worldName = WORLD::GetName(AREA::Current->World);

                auto _areaInfo = YS::AREAINFO::Get(-1, -1);
                auto _areaVoice = *reinterpret_cast<uint16_t*>(_areaInfo + 0x30);

                string _pathAppend = _constructPath;
                sprintf(_loadBuff, _pathAppend.append("/%s%d_%s.win32.scd").c_str(), _worldName, _areaVoice, YS::VOICE::Part[_entryPart]);

                auto _fetchSize = YS::FILE::GetSize(_loadBuff);
                ALLOCATE_VSB = (char*)malloc(_fetchSize);

                YS::FILE::Read(_loadBuff, ALLOCATE_VSB);
                SOUND::SetTransfer(CURRENT_VSB - 0x0236, 0x03, ALLOCATE_VSB, _fetchSize, nullptr, nullptr);

                return;
            }
        }
    }
}

void ENFORCE_LOCKON()
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A4);
    auto _fetchControl = (_fetchConfig & 0x4000) == 0x4000 ? 0x00 : ((_fetchConfig & 0x8000) == 0x8000 ? 0x01 : 0x02);

    auto _fetchChange = *(YS::HARDPAD::Input - 0x02);
    auto _fetchTarget = *LOCKON_TARGET;

    if (LOCKON_VANILLA.size() == 0x00)
    {
        LOCKON_EDITED =
        {
            0x41, 0x8B, 0x06,                               // mov eax, [r14]
            0x66, 0xA9, 0x00, 0x01,                         // test ax, 0x0100
            0x74, 0x0A,                                     // je 0x0A
            0xF3, 0x0F, 0x10, 0x1D, 0x00, 0x00, 0x00, 0x00, // movss xmm3, [_floatCalcFirst]
            0xEB, 0x19,                                     // jmp 0x19
            0x66, 0xA9, 0x00, 0x02,                         // test ax, 0x0200
            0x75, 0x0B,                                     // jne 0x0B
            0x41, 0xC7, 0x46, 0xFC, 0x00, 0x00, 0x00, 0x00, // mov [r14 - 0x04], 0x00
            0xEB, 0x59,                                     // jmp 0x59
            0x90,                                           // nop
            0xF3, 0x0F, 0x10, 0x1D, 0x00, 0x00, 0x00, 0x00, // movss xmm3, [_floatCalcSecond]
            0x41, 0x8B, 0x4E, 0xFC,                         // mov ecx, [r14 - 0x04]
            0x85, 0xC9,                                     // test ecx, ecx
            0x75, 0x48,                                     // jne 0x48
            0x41, 0xFF, 0x46, 0xFC                          // inc [r14 - 0x04]
        };

        LOCKON_VANILLA.resize(LOCKON_EDITED.size());
        memcpy(LOCKON_VANILLA.data(), LOCKON_FUNCTION + 0xF3, LOCKON_EDITED.size());
    }

    if (_fetchControl != LOCKON_TYPE)
    {
        if (_fetchControl == 0x02)
            memcpy(LOCKON_FUNCTION + 0xF3, LOCKON_VANILLA.data(), LOCKON_VANILLA.size());

        else
        {
            uint32_t _floatCalcFirst = LOCKON_FLOATS - (LOCKON_FUNCTION + 0x104) - (_fetchControl == 0x00 ? 0x00 : 0x1C8);
            uint32_t _floatCalcSecond = (LOCKON_FLOATS - 0x1C8) - (LOCKON_FUNCTION + 0x11F) + (_fetchControl == 0x00 ? 0x00 : 0x1C8);

            memcpy(LOCKON_FUNCTION + 0xF3, LOCKON_EDITED.data(), LOCKON_EDITED.size());

            memcpy(LOCKON_FUNCTION + 0x100, &_floatCalcFirst, 0x04);
            memcpy(LOCKON_FUNCTION + 0x11B, &_floatCalcSecond, 0x04);
        }

        LOCKON_TYPE = _fetchControl;
    }

    if (PAST_LOCKON != _fetchTarget && !LOCKON_PLAY)
    {
        SOUND::PlaySFX(7);
        LOCKON_PLAY = true;
    }

    else if (LOCKON_PLAY)
        LOCKON_PLAY = false;

    PAST_LOCKON = _fetchTarget;
}

void HANDLE_GOA_LAND()
{
    auto _gardenKnown = *(AREA::SaveData + 0x231B) & 0x04;
    auto _canLandGarden = *(AREA::SaveData + 0x1EF6) & 0x40;

    if (_gardenKnown && !_canLandGarden)
        *(AREA::SaveData + 0x1EF6) += 0x40;
}

void ENFORCE_FRAMERATE()
{
    auto _limiterOffset = reinterpret_cast<char*>(dk::Vsync::setLimit) + 0x20;

    if (LIMITER_FUNCTION.size() == 0x00)
    {
        LIMITER_FUNCTION.resize(0x06);
        memcpy(LIMITER_FUNCTION.data(), _limiterOffset, 0x06);
    }

    uint8_t _fetchFunction = *_limiterOffset;
    auto _fetchFramerate = IS_STEAM ? *(PC::STEAM::MareConfig + 0x0A) : *(PC::EGS::MareConfig + 0x0A);

    if (_fetchFramerate == 0x00 && _fetchFunction == 0x90)
    {
        memcpy(_limiterOffset, LIMITER_FUNCTION.data(), 0x06);
        *(dk::Vsync::IsFrameLimited) = 0x01;
    }

    else if (_fetchFramerate != 0x00 && _fetchFunction != 0x90)
    {
        char* _nopArray = new char[0x06];
        fill(_nopArray, _nopArray + 0x06, 0x90);

        memcpy(_limiterOffset, _nopArray, 0x06);
        *(dk::Vsync::IsFrameLimited) = 0x00;
    }
}

void DISCORD_RPC()
{
    int _resultant = 0xFF;

    if (Discord == nullptr)
    {
        discord::Core::Create(833511404274974740, DiscordCreateFlags_NoRequireDiscord, &Discord);

        if (Discord == nullptr)
        {
            DISCORD_ENABLED = false;
            return;
        }

        const auto _currTime = chrono::system_clock::now();
        auto _unixTime = static_cast<time_t>(chrono::duration_cast<chrono::seconds>(_currTime.time_since_epoch()).count());

        BEGIN_TIMESTAMP = _unixTime;

        RICH_PRESENCE.SetApplicationId(833511404274974740);
        RICH_PRESENCE.GetTimestamps().SetStart(BEGIN_TIMESTAMP);

        for (uint16_t i = 0x5740; i < 0x5745; i++)
        {
            auto _msgData = YS::MESSAGE::GetData(i);
            auto _msgConvert = YS::MESSAGE::DecodeKHSCII(_msgData);

            TEXT_PRESENCE.push_back(_msgConvert);
        }

        for (auto i = 0x00; i < 0x04; i++)
        {
            uint16_t _stringID = 0x3738 + i;

            if (i == 0x03)
                _stringID = 0x4E30;

            auto _msgData = YS::MESSAGE::GetData(_stringID);
            auto _msgConvert = YS::MESSAGE::DecodeKHSCII(_msgData);

            TEXT_MODE.push_back(_msgConvert);
        }

        for (auto i = 0x00; i < 0x06; i++)
        {
            uint16_t _stringID = 0x01E5 + (i >= 0x02 ? i - 1 : i);

            if (i == 0x02)
                _stringID = 0x4E7F;

            auto _msgData = YS::MESSAGE::GetData(_stringID);
            auto _msgConvert = YS::MESSAGE::DecodeKHSCII(_msgData);

            TEXT_FORM.push_back(_msgConvert);
        }

        if (YS::FILE::GetSize("mirageArena.bin") != 0x00)
            IS_MIRAGE = true;
    }


    if (DISCORD_ENABLED)
    {
        if (*YS::TITLE::IsTitle)
        {
            RICH_PRESENCE.GetAssets().SetLargeImage("title");

            RICH_PRESENCE.SetState("");
            RICH_PRESENCE.SetDetails("");

            RICH_PRESENCE.GetAssets().SetLargeText("");
            RICH_PRESENCE.GetAssets().SetSmallText("");

            RICH_PRESENCE.GetAssets().SetSmallImage("");
        }

        else if (AREA::Current->World >= 0x02 && AREA::Current->World <= 0x12)
        {
            bool _checkUnderdrome = AREA::Current->World == 0x06 && AREA::Current->Room == 0x09 && AREA::Current->Set.Map >= 0xBD && AREA::Current->Set.Map >= 0xC4;

            auto _detailText = _checkUnderdrome ? TEXT_PRESENCE.at(0x02) : TEXT_PRESENCE.at(0x00);

            _detailText.replace(_detailText.find("[0]"), 0x03, to_string(*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308)));
            _detailText.replace(_detailText.find("[1]"), 0x03, *(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x180) > 0x00 ? to_string(*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x180)) : TEXT_PRESENCE.at(0x04));

            if (_checkUnderdrome)
                _detailText.replace(_detailText.find("[2]"), 0x03, to_string(AREA::Current->Entrance));

            RICH_PRESENCE.SetDetails(_detailText.c_str());

            auto _stateText = TEXT_PRESENCE.at(0x01);

            _stateText.replace(_stateText.find("[0]"), 0x03, to_string(*(AREA::SaveData + 0x24FF)));
            _stateText.replace(_stateText.find("[1]"), 0x03, *(AREA::SaveData + 0x3524) == 0x00 ? "N/A" : (*COMMAND_TYPE == 0x01 ? "Mickey" : TEXT_FORM.at(*(AREA::SaveData + 0x3524) - 0x01)));

            RICH_PRESENCE.SetState(_stateText.c_str());

            auto _fetchTime = floorf(*reinterpret_cast<const uint32_t*>(AREA::SaveData + 0x2444) / 60.0F);

            auto _playHours = floorf(_fetchTime / 3600.0F);
            auto _playMinutes = floorf(fmodf(_fetchTime, 3600.0F) / 60.0F);

            ostringstream _timeStream;
            auto _timeText = TEXT_PRESENCE.at(0x03);

            _timeStream << std::setw(2) << std::setfill('0') << _playHours << ":"
                << std::setw(2) << std::setfill('0') << _playMinutes;

            _timeText.replace(_timeText.find("[0]"), 0x03, _timeStream.str());

            RICH_PRESENCE.GetAssets().SetLargeText(_timeText.c_str());
            RICH_PRESENCE.GetAssets().SetSmallText(TEXT_MODE.at(*(AREA::SaveData + 0x2498)).c_str());

            RICH_PRESENCE.GetAssets().SetSmallImage(*AREA::BattleStatus == 0x00 ? "safe" : (*AREA::BattleStatus == 0x01 ? "mob" : "boss"));
            RICH_PRESENCE.GetAssets().SetLargeImage(IS_MIRAGE && AREA::Current->World == 0x0B ? "ma" : string(WORLD::GetName(AREA::Current->World), 0x02).c_str());
        }

        Discord->ActivityManager().UpdateActivity(RICH_PRESENCE, [&_resultant](discord::Result v) { _resultant = (int)v; });
        Discord->RunCallbacks();
    }
}

void HANDLE_SHAKE()
{
    if (ADJUST_GLOW_ARRAY.size() == 0x00)
    {
        ADJUST_GLOW_ARRAY.resize(0x10);
        INIT_VIEWPORT_ARRAY.resize(0x07);
        ADJUST_VIEWPORT_ARRAY.resize(0x08);

        memcpy(ADJUST_GLOW_ARRAY.data(), ADJUST_GLOW_FUNCTION + 0xE6E, 0x10);
        memcpy(INIT_VIEWPORT_ARRAY.data(), INIT_VIEWPORT_FUNCTION + 0x35, 0x07);
        memcpy(ADJUST_VIEWPORT_ARRAY.data(), ADJUST_VIEWPORT_FUNCTION + 0x60, 0x08);
    }

    if (*ryj::SHAKE::ShakeTimer > 0 || *ryj::SHAKE::ShakeCoords > 0x00 || *(ryj::SHAKE::ShakeCoords + 0x02) > 0x00)
    {
        if (!SHAKE_WRITTEN)
        {
            char _nopArray[0x08];
            fill(_nopArray, _nopArray + 0x08, 0x90);

            memcpy(INIT_VIEWPORT_FUNCTION + 0x35, _nopArray, 0x07);
            memcpy(ADJUST_VIEWPORT_FUNCTION + 0x60, _nopArray, 0x08);

            SHAKE_WRITTEN = true;
        }

        float _writeFloat[0x04];

        _writeFloat[0x00] = *ryj::SHAKE::ShakeCoords;
        _writeFloat[0x01] = *(ryj::SHAKE::ShakeCoords + 0x02);
        _writeFloat[0x02] = *(reinterpret_cast<int*>(VIEWPORT3D_ADDR + 0x08)) + *ryj::SHAKE::ShakeCoords;
        _writeFloat[0x03] = *(reinterpret_cast<int*>(VIEWPORT3D_ADDR + 0x0C)) + *(ryj::SHAKE::ShakeCoords + 0x02);

        memcpy(VIEWPORT3D_ADDR + 0x15C, _writeFloat, 0x10);
    }

    else if (SHAKE_WRITTEN)
    {
        memcpy(ADJUST_GLOW_FUNCTION + 0xE6E, ADJUST_GLOW_ARRAY.data(), 0x10);
        memcpy(INIT_VIEWPORT_FUNCTION + 0x35, INIT_VIEWPORT_ARRAY.data(), 0x07);
        memcpy(ADJUST_VIEWPORT_FUNCTION + 0x60, ADJUST_VIEWPORT_ARRAY.data(), 0x08);

        SHAKE_WRITTEN = false;
    }
}

void ENFORCE_PROMPTS()
{
    bool _isEnforced = *(PROMPT_INSTRUCTION + 0x06) == 0x00 ? true : false;
    bool _fetchConfig = *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A4) & 0x2000;

    if (_isEnforced != _fetchConfig)
    {
        bool _fetchConfig = *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A4) & 0x2000;

        bool _targetMode = _fetchConfig ? 0x00 : 0x01;

        memcpy(PROMPT_INSTRUCTION + 0x06, &_targetMode, 0x01);
        memcpy(PROMPT_MODE, &_targetMode, 0x01);
    }
}

void AUTOSAVE()
{
    if (CHECKSUM_TABLE.size() == 0x00)
    {
        for (auto x = 0; x <= 0xFF; x++)
        {
            auto r = x << 24;

            for (auto j = 0; j < 0xFF; j++)
                r = r << 1 ^ (r < 0 ? 0x4C11DB7 : 0);

            CHECKSUM_TABLE.push_back(r);
        }
    }

    auto _commandPointer = *YS::COMMAND_DRAW::CommandDraw;
    auto _gaugeTypePointer = *dk::GAUGE::PlayerGauge ? *reinterpret_cast<char**>(*dk::GAUGE::PlayerGauge + 0x88) : nullptr;
    auto _mainPointer = *YS::EVENT::Event;

    auto _savePointer = IS_STEAM ? PC::STEAM::MareSave : PC::EGS::MareSave;

    if (!SYSTEM_WRITTEN)
    {
        char* _systemInfo = *reinterpret_cast<char**>(*_savePointer + 0x10) + 0x10;

        const char* _systemText = "BISLPM-66675FM-SYS";
        uint32_t _systemLength = 0x400;

        if (strcmp(_systemInfo, _systemText) != 0x00)
        {
            const auto _currTime = chrono::system_clock::now();
            auto _unixTime = static_cast<time_t>(chrono::duration_cast<chrono::seconds>(_currTime.time_since_epoch()).count());

            memcpy(_systemInfo, _systemText, 0x12);

            memcpy(_systemInfo + 0x40, &_unixTime, 4);
            memcpy(_systemInfo + 0x48, &_unixTime, 4);

            memcpy(_systemInfo + 0x50, &_systemLength, 4);
        }

        SYSTEM_WRITTEN = true;
    }

    if (*AREA::IsInMap && !*YS::TITLE::IsTitle && !SYSTEM_LOADED)
        SYSTEM_LOADED = true;

    else if (*YS::TITLE::IsTitle && SYSTEM_LOADED)
    {
        SAVE_ITERATOR = 0;
        SYSTEM_LOADED = false;
        SAVE_AREA = *AREA::Current;
    }

    if (_commandPointer != 0x00 && _gaugeTypePointer != 0x00)
    {
        bool _checkBlacklist = AREA::Current->World == 0x0F || AREA::Current->World == 0x0B ||
            (AREA::Current->World == 0x08 && AREA::Current->Room == 0x03) ||
            (AREA::Current->World == 0x0C && AREA::Current->Room == 0x02) ||
            (AREA::Current->World == 0x02 && AREA::Current->Room <= 0x01) ||
            (AREA::Current->World == 0x04 && AREA::Current->Room == 0x10) ||
            (AREA::Current->World == 0x12 && AREA::Current->Room >= 0x13 && AREA::Current->Room <= 0x1D);

        if (!*YS::TITLE::IsTitle && *AREA::IsInMap && !_checkBlacklist)
        {
            if (SAVE_AREA.World == 0x00)
                SAVE_AREA = *AREA::Current;

            bool _isAutosave = (*reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A4) & 0x0002) || (*reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A4) & 0x0004);
            bool _checkStatus = !*YS::MENU::IsMenu && _commandPointer != 0x00 && *AREA::IsInMap && _mainPointer == 0x00 && *AREA::BattleStatus == 0x00 && SAVE_AREA.World >= 0x02 && SYSTEM_LOADED && _isAutosave && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x00;

            if (!_checkStatus)
            {
                SAVE_INITIATE = false;
                return;
            }

            if (SAVE_AREA.World != AREA::Current->World)
            {
                SAVE_INITIATE = true;
                SAVE_ITERATOR = 0;
            }

            if (SAVE_AREA.Room != AREA::Current->Room)
            {
                SAVE_ITERATOR++;

                if (SAVE_ITERATOR == ROOM_AMOUNT)
                {
                    SAVE_INITIATE = true;
                    SAVE_ITERATOR = 0;
                }
            }

            SAVE_AREA = *AREA::Current;
        }
    }

    if (SAVE_INITIATE)
    {
        auto _saveOffset = SAVE_SLOT_OFFSET;

    START_FUNC:

        ostringstream _stringStream;
        _stringStream << setw(2) << setfill('0') << (_saveOffset - 1);

        string _saveName = "BISLPM-66675FM-" + _stringStream.str();
        const char* _saveHeader = "KH2J";

        vector<string> _usedSlots;

        char* _saveFilePath = *_savePointer + 0x40;
        string _saveFileString(_saveFilePath);

        _saveFileString = _saveFileString.append(IS_STEAM ? "\\KHIIFM_WW.png" : "\\KHIIFM.png");

        const auto _currTime = chrono::system_clock::now();
        auto _unixTime = static_cast<time_t>(chrono::duration_cast<chrono::seconds>(_currTime.time_since_epoch()).count());

        uint32_t _saveSlot = 0x00;
        uint32_t _saveHeaderID = 0x3A;
        uint32_t _saveInfoLength = 0x158;
        uint32_t _saveDataLength = 0x10FC0;

        uint32_t _autoSaveTag = 0xFFFFFFFF;
        uint32_t _regularSaveTag = 0x00000000;

        uint32_t _saveInfoStartFILE = 0x1C8;
        uint32_t _saveDataStartFILE = 0x19690;

        char* _saveInfoStartRAM = *reinterpret_cast<char**>(*_savePointer + 0x10) + 0x168;
        char* _saveDataStartRAM = *reinterpret_cast<char**>(*_savePointer + 0x10) + 0x19630;

        memcpy(AREA::SaveData + 0x10, &_autoSaveTag, 0x04);

        const char* _saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);

        for (int i = 0; i < 99; i++)
            if (*(_saveInfoStartRAM + (_saveInfoLength * i)) != 0x00)
                _usedSlots.push_back(string(_saveInfoStartRAM + (_saveInfoLength * i)));

        while (_saveSlotRAM[0] != 0x00 && strcmp(_saveSlotRAM, _saveName.c_str()) != 0x00)
        {
            _saveSlot++;
            _saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
        }

        auto _fetchCheck = *reinterpret_cast<uint32_t*>(_saveDataStartRAM + (_saveDataLength * _saveSlot) + 0x10);

        while (_fetchCheck != _autoSaveTag && _saveSlotRAM[0] != 0x00)
        {
            _saveSlot++;
            _saveSlotRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
            _fetchCheck = *reinterpret_cast<uint32_t*>(_saveDataStartRAM + (_saveDataLength * _saveSlot) + 0x10);

            while (find(_usedSlots.begin(), _usedSlots.end(), _saveName) != _usedSlots.end())
            {
                _saveOffset--;

                if (_saveOffset == 0x00)
                {
                    if (!SAVE_ROUNDBACK)
                    {
                        _saveOffset = 99;
                        SAVE_ROUNDBACK = true;
                    }

                    else
                    {
                        const char* _unableMessage = YS::MESSAGE::GetData(0x5703);
                        dk::INFORMATION::openInformationWindow(_unableMessage);
                        SAVE_ROUNDBACK = false;
                        return;
                    }
                }

                _stringStream.str("");
                _stringStream << setw(2) << setfill('0') << (_saveOffset - 1);

                _saveName = "BISLPM-66675FM-" + _stringStream.str();
                
                if (_fetchCheck == UINT32_MAX)
                    break;
            }

            if (_saveSlot >= 99)
            {
                const char* _unableMessage = YS::MESSAGE::GetData(0x5703);
                dk::INFORMATION::openInformationWindow(_unableMessage);
                return;
            }
        }

        char* _magicData = (char*)malloc(0x08);
        char* _saveData = (char*)malloc(0x10FB4);

        memcpy(_magicData, AREA::SaveData, 0x08);
        memcpy(_saveData, AREA::SaveData + 0x0C, 0x10FB4);

        auto _calculateChecksum = [](uint32_t _startChecksum, char* _dataArray, int _dataLength)
            {
                uint32_t _checksum = _startChecksum;

                for (uint32_t i = 0; i < _dataLength; i++)
                    _checksum = CHECKSUM_TABLE[(_checksum >> 24) ^ static_cast<unsigned char>(_dataArray[i])] ^ (_checksum << 8);

                return _checksum ^ 0xFFFFFFFF;
            };

        auto _magicChecksum = _calculateChecksum(0xFFFFFFFF, _magicData, 0x08);
        auto _dataChecksum = _calculateChecksum(_magicChecksum ^ 0xFFFFFFFF, _saveData, 0x10FB4);

        char* _saveInfoAddrRAM = _saveInfoStartRAM + (_saveInfoLength * _saveSlot);
        char* _saveDataAddrRAM = _saveDataStartRAM + (_saveDataLength * _saveSlot);

        memcpy(_saveInfoAddrRAM, _saveName.c_str(), 0x11);

        memcpy(_saveInfoAddrRAM + 0x40, &_unixTime, 4);
        memcpy(_saveInfoAddrRAM + 0x48, &_unixTime, 4);

        memcpy(_saveInfoAddrRAM + 0x50, &_saveDataLength, 4);

        memcpy(_saveDataAddrRAM, _saveHeader, 4);

        memcpy(_saveDataAddrRAM + 0x04, &_saveHeaderID, 4);
        memcpy(_saveDataAddrRAM + 0x08, &_dataChecksum, 4);

        memcpy(_saveDataAddrRAM + 0x0c, _saveData, 0x10FB4);

        memcpy(AREA::SaveData + 0x10, &_regularSaveTag, 0x04);

        uint32_t _saveInfoAddr = _saveInfoStartFILE + _saveInfoLength * _saveSlot;
        uint32_t _saveDataAddr = _saveDataStartFILE + _saveDataLength * _saveSlot;

        ofstream _stream(_saveFileString, ios::in | ios::out | ios::binary);

        _stream.seekp(_saveInfoAddr);
        _stream.write(_saveName.c_str(), 0x11);

        _stream.seekp(_saveInfoAddr + 0x40);
        _stream.write(reinterpret_cast<const char*>(&_unixTime), 0x04);
        _stream.seekp(_saveInfoAddr + 0x48);
        _stream.write(reinterpret_cast<const char*>(&_unixTime), 0x04);

        _stream.seekp(_saveInfoAddr + 0x50);
        _stream.write(reinterpret_cast<const char*>(&_saveDataLength), 0x04);

        _stream.seekp(_saveDataAddr);
        _stream.write(_saveHeader, 0x04);

        _stream.seekp(_saveDataAddr + 0x04);
        _stream.write(reinterpret_cast<const char*>(&_saveHeaderID), 0x04);
        _stream.seekp(_saveDataAddr + 0x08);
        _stream.write(reinterpret_cast<const char*>(&_dataChecksum), 0x04);

        _stream.seekp(_saveDataAddr + 0x0C);
        _stream.write(_saveData, 0x10FB4);

        _stream.close();

        free(_saveData);
        free(_magicData);

        if (*reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A4) & 0x0004)
        {
            const char* _saveMessage = YS::MESSAGE::GetData(0x5702);
            dk::INFORMATION::openInformationWindow(_saveMessage);
        }

        SAVE_INITIATE = false;
    }
}

void FIX_SAVE_POINT()
{
    if (*reinterpret_cast<const uint16_t*>(YS::COMMAND_ELEM::ReactionID) == 0x0037 && *AREA::IsInMap && !*YS::TITLE::IsTitle)
    {
        auto _statsSlot1 = YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C;
        auto _statsSlot2 = YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C - 0x278;
        auto _statsSlot3 = YS::MEMBER_TABLE::MemberStatsAnchor + 0xC30C - 0x4F0;

        uint8_t _healthChecks = 0x00;
        uint8_t _magicChecks = 0x00;

        _healthChecks += *_statsSlot3 == 0x00 ? 0x00 : (*_statsSlot3 != *(_statsSlot3 - 0x04) ? 0x01 : 0x00);
        _healthChecks += *_statsSlot2 == 0x00 ? 0x00 : (*_statsSlot2 != *(_statsSlot2 - 0x04) ? 0x01 : 0x00);
        _healthChecks += *_statsSlot1 == 0x00 ? 0x00 : (*_statsSlot1 != *(_statsSlot1 - 0x04) ? 0x01 : 0x00);

        _magicChecks += *_statsSlot3 == 0x00 ? 0x00 : (*(_statsSlot3 + 0x180) != *(_statsSlot3 + 0x180 - 0x04) ? 0x01 : 0x00);
        _magicChecks += *_statsSlot2 == 0x00 ? 0x00 : (*(_statsSlot2 + 0x180) != *(_statsSlot2 + 0x180 - 0x04) ? 0x01 : 0x00);
        _magicChecks += *_statsSlot1 == 0x00 ? 0x00 : (*(_statsSlot1 + 0x180) != *(_statsSlot1 + 0x180 - 0x04) ? 0x01 : 0x00);

        if (SAVE_REACTION == 0x75)
            SAVE_REACTION_FRAME++;

        if (_healthChecks > 0x00 || _magicChecks > 0x00)
            SAVE_REACTION = 0x75;

        else if (SAVE_REACTION == 0x75 && SAVE_REACTION_FRAME >= 80)
        {
            SAVE_REACTION = 0xEB;
            SAVE_REACTION_FRAME = 0;
        }
    }

    else if (!*AREA::IsInMap || *YS::TITLE::IsTitle)
        SAVE_REACTION = 0x75;

    memcpy(SAVE_REACTION_FUNCTION + 0x25B, &SAVE_REACTION, 0x01);
}

void REGISTER_MAGIC()
{
    // Fetch the current levels or magic.

    uint32_t _magicFirst = *reinterpret_cast<const uint32_t*>(AREA::SaveData + 0x3594);
    uint16_t _magicSecond = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x35CF);

    // If the game is loaded, and if the Tier 1 Magic or the Tier 2 Magic levels do not match what is previously recorded.

    if (*AREA::IsInMap && (_magicFirst != MAGIC_FIRST || _magicSecond != MAGIC_SECOND))
    {
        // Initialize the array we will be using for the commands.
        vector<uint16_t> _commandArray;

        // For 6 Magic (Fire, Blizzard, Thunder, Cure, Magnet, Reflect):
        for (uint32_t i = 0; i < 0x06; i++)
        {
            // Get the current table and denote the pointer.

            auto _currentTable = YS::MAGIC::GetTable(i);
            auto _currentTablePtr = reinterpret_cast<uint64_t>(_currentTable);

            // If the current table exists (Meaning we have the Magic):

            if (_currentTable != 0x00)
            {
                // Fetch the previously denoted level of the current Magic, and fetch the current Magic table pointer.

                char _fetchMagicLevel = i <= 0x03 ? *(&MAGIC_FIRST + i) : *(&MAGIC_SECOND + (i - 0x04));
                auto _currentPointer = *reinterpret_cast<const uint64_t*>(YS::MAGIC::MagicInfo + 0x48 + 0x50 * i);

                // If the current Magic table is the one we fetched and the current Magic level is not zero (Meaning we have the Magic and it was processed), or the denoted level equals to the current level:
                if ((_currentPointer == _currentTablePtr && _fetchMagicLevel != 0x00) || _fetchMagicLevel == _currentTable->Level)
                {
                    // Denote the command of the current Magic to the array, and skip to the next entry.
                    _commandArray.push_back(_currentTable->Command);
                    continue;
                }

                // If a previous of the Magic was processed by this function, free it and remove it from the process list.
                if (MAGIC_FILES.count(i) != 0x00)
                {
                    free(MAGIC_FILES.at(i));
                    MAGIC_FILES.erase(i);
                }

                // Get the size of the .mag file and allocate memory for it.
                size_t _fetchSize = YS::FILE::GetSize(_currentTable->Filename);
                auto _loadBAR = (char*)malloc(_fetchSize);

                // If the magic file can load (Meaning it exists and we have allocated the memory successfully):
                if (YS::FILE::ReadBAR(_currentTable->Filename, _loadBAR) != 0x00)
                {
                    // Denote the address of the loaded file for future handling.
                    MAGIC_FILES[i] = _loadBAR;

                    // Get the address of the BAR as a uint64_t for calculations, and get the 32-bit offset of the BAR.
                    uint64_t _barAddress = reinterpret_cast<uint64_t>(_loadBAR);
                    uint32_t _barFileOffset = *reinterpret_cast<const uint32_t*>(_loadBAR + 0x08);

                    // Calculate the absolute addresses of PAX and BDX files.
                    uint64_t _paxAddress = _barAddress + *reinterpret_cast<const uint32_t*>(_loadBAR + 0x18) - _barFileOffset;
                    uint64_t _bdxAddress = _barAddress + *reinterpret_cast<const uint32_t*>(_loadBAR + 0x28) - _barFileOffset;

                    // As well as the address of which PAX actually starts in.
                    uint64_t _paxStartAddress = _barAddress + *reinterpret_cast<const uint32_t*>(_loadBAR + 0x18) + 0x10 - _barFileOffset;

                    // Write all of the info needed for the Magic to be parsed, processed, and executed.
                    memcpy(YS::MAGIC::MagicInfo + 0x50 * i, &_barAddress, 0x08);
                    memcpy(YS::MAGIC::MagicInfo + 0x08 + 0x50 * i, &_bdxAddress, 0x08);
                    memcpy(YS::MAGIC::MagicInfo + 0x10 + 0x50 * i, &_bdxAddress, 0x08);
                    memcpy(YS::MAGIC::MagicInfo + 0x18 + 0x50 * i, &_paxAddress, 0x08);
                    memcpy(YS::MAGIC::MagicInfo + 0x20 + 0x50 * i, &_paxStartAddress, 0x08);

                    // Overwrite the pointer to the current Magic table.
                    memcpy(YS::MAGIC::MagicInfo + 0x48 + 0x50 * i, &_currentTablePtr, 0x08);

                    // Initialize the current Magic PAX.
                    ryj::PAX::Init(YS::MAGIC::MagicInfo + 0x18 + 0x50 * i, reinterpret_cast<char*>(_paxAddress));

                    // Denote the command of the current Magic to the array.
                    _commandArray.push_back(_currentTable->Command);
                };
            }
        }

        // Copy the commands array to where the Magic Commands are stored.
        _commandArray.resize(0x06);
        memcpy(YS::MAGIC::MagicCommands, _commandArray.data(), 0x0C);

        // Denote the current Magic levels for both tiers of Magic.
        MAGIC_FIRST = _magicFirst;
        MAGIC_SECOND = _magicSecond;
    }

    // If the game isn't loaded, free every single magic that we have processed ourselves.
    else if (!*AREA::IsInMap)
    {
        for (auto _magicEntry : MAGIC_FILES)
            free(_magicEntry.second);

        MAGIC_FILES.clear();
    }
}

void REGISTER_ABILITY()
{
    // Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
    auto _soraGauge = *dk::GAUGE::PlayerGauge ? *reinterpret_cast<char**>(*dk::GAUGE::PlayerGauge + 0x88) : nullptr;

    // See if there is specifically a Cutscene playing.
    bool _isCutscene = *YS::EVENT::Event && *reinterpret_cast<int*>(*YS::EVENT::Event + 0x04) != 0xCAFEEFAC
                                         && *reinterpret_cast<int*>(*YS::EVENT::Event + 0x04) != 0xEFACCAFE;

    auto _commandPointer = *YS::COMMAND_DRAW::CommandDraw;

    // If the game is loaded:
    if (*AREA::IsInMap && _commandPointer != 0x00 && _soraGauge != 0x00 && !_isCutscene)
    {
        // If  the ability denotation is not initialized:
        if (ABILITY_ARRAY.size() == 0x00)
        {
            // Resize the denotation to be 0x60 elements.
            ABILITY_ARRAY.resize(0x60);

            // Denote all of Sora's current abilities and sort them.
            memcpy(ABILITY_ARRAY.data(), AREA::SaveData + 0x2544, 0xC0);
            sort(ABILITY_ARRAY.begin(), ABILITY_ARRAY.end());
        }

        // Create the vectors for current ability calculations.
        vector<uint16_t> _abilityDiff;
        vector<uint16_t> _currentAbility(0x60);

        // Denote all of Sora's current abilities and sort them.
        memcpy(_currentAbility.data(), AREA::SaveData + 0x2544, 0xC0);
        sort(_currentAbility.begin(), _currentAbility.end());

        // Get all the abilities that differ between the old and current denotation.
        set_difference(_currentAbility.begin(), _currentAbility.end(), ABILITY_ARRAY.begin(), ABILITY_ARRAY.end(), inserter(_abilityDiff, _abilityDiff.begin()));

        // Check if any of the different abilities contain movement.
        bool _fetchMovement = any_of(_abilityDiff.begin(), _abilityDiff.end(), [](int x) {
            return (x >= 0x805E && x <= 0x806D) || (x >= 0x8234 && x <= 0x8237) || (x == 0x0194 || x == 0x8194);
            });

        // If they do:
        if (_fetchMovement)
        {
            // Refresh all of Sora's stats, which in turn, will commit all movement changes.
            YS::SHEET::Save(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308);
            YS::SHEET::Load(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308, nullptr);
        }

        // Copy over the current ability list to the denotation array.
        ABILITY_ARRAY.assign(_currentAbility.begin(), _currentAbility.end());
    }
}

void SHOW_INFORMATION()
{
    // Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
    auto _soraGauge = *dk::GAUGE::PlayerGauge ? *reinterpret_cast<char**>(*dk::GAUGE::PlayerGauge + 0x88) : nullptr;

    auto _commandPointer = *YS::COMMAND_DRAW::CommandDraw;
    auto _reactionCommand = *reinterpret_cast<const uint16_t*>(YS::COMMAND_ELEM::ReactionID);

    // See if there is specifically a Cutscene playing.
    bool _isCutscene = *YS::EVENT::Event && *reinterpret_cast<int*>(*YS::EVENT::Event + 0x04) != 0xCAFEEFAC
                                         && *reinterpret_cast<int*>(*YS::EVENT::Event + 0x04) != 0xEFACCAFE;

    // If the game is loaded, and there isn't a menu present, and it's not a cutscene:
    if (*AREA::IsInMap && _commandPointer != 0x00 && !*YS::MENU::IsMenu && !_isCutscene && _soraGauge != 0x00)
    {
        // Fetch the fade status and the enable line.
        auto _fetchFade = *(dk::JUMPEFFECT::FadeStatus + 0x108);
        auto _fetchEnable = moduleInfo.startAddr[0x800000];

        // If there is no fade, and the enable line is set:
        if (_fetchFade == 0x00 && _fetchEnable != 0x00)
        {
            // Reset the enable line.
            *const_cast<char*>(moduleInfo.startAddr + 0x800000) = 0x00;

            // If the enable line is 0x01, summon INFORMATION. If it's 0x02, summon PRIZE.
            switch (_fetchEnable)
            {
                case 0x01:
                    dk::INFORMATION::openInformationWindow(moduleInfo.startAddr + 0x800004);
                    break;

                case 0x02:
                    dk::TREASURE_INFO::openPrizeWindow(moduleInfo.startAddr + 0x800104);
                    break;

                case 0x03:
                    dk::TREASURE_INFO::openBoxWindow(moduleInfo.startAddr + 0x800154, *reinterpret_cast<const uint16_t*>(moduleInfo.startAddr + 0x800150));
            }
        }
    }

    if (_reactionCommand == 0x0020 && !IS_PICTURE_EDITED)
    {
        uint32_t _fetchAddress = *reinterpret_cast<const uint32_t*>(PICTURE_APPEAR_FUNC + 0x36);
        _fetchAddress += 0x90;

        memcpy(PICTURE_APPEAR_FUNC + 0x36, &_fetchAddress, 0x04);
        IS_PICTURE_EDITED = true;
    }

    else if (_reactionCommand != 0x0020 && IS_PICTURE_EDITED)
    {
        uint32_t _fetchAddress = *reinterpret_cast<const uint32_t*>(PICTURE_APPEAR_FUNC + 0x36);
        _fetchAddress -= 0x90;

        memcpy(PICTURE_APPEAR_FUNC + 0x36, &_fetchAddress, 0x04);
        IS_PICTURE_EDITED = false;
    }
}

void PROCESS_DEATH()
{
    // Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
    auto _soraGauge = *dk::GAUGE::PlayerGauge ? *reinterpret_cast<char**>(*dk::GAUGE::PlayerGauge + 0x88) : nullptr;

    // Fetch Sora's pointer as well as his UCM.
    auto _soraSelf = *YS::SORA::Sora;
    auto _fetchSora = *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable);

    // If Sora's HP is 0, and he isn't Mermaid Sora, and his gauge is present, and he isn't dead:
    if (*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308) == 0x00 && *AREA::IsInMap && !*YS::MENU::IsMenu && (_fetchSora != 0x03BE && _fetchSora != 0x0656) && _soraGauge != 0x00 && !IS_DEAD)
    {
        // Process his death and mark it.
        YS::SORA::AddHP(reinterpret_cast<char*>(_soraSelf), 0x00, 0x00, false);
        IS_DEAD = true;
    }

    // If Sora's HP is NOT 0 but he is dead, mark him as not.
    else if (*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308) != 0x00 && IS_DEAD)
        IS_DEAD = false;
}

void HANDLE_ASPECT()
{
    float _resolutionHorizontal = *reinterpret_cast<float*>(VIEWPORT3D_ADDR + 0x10);
    float _resolutionVertical = *reinterpret_cast<float*>(VIEWPORT3D_ADDR + 0x14);

    float _commonDiv = 0x00F;

    if (_resolutionHorizontal != 0x00 && _resolutionVertical != 0x00)
    {
        POSITIVE_ASPECT_OFFSET = 0x55;
        NEGATIVE_ASPECT_OFFSET = 0xFFFFFFAB;

        uint16_t _offsetInformation = 0xFFFF;

        auto _tempWidth = _resolutionHorizontal;
        auto _tempHeight = _resolutionVertical;

        while (_tempWidth != 0 && _tempHeight != 0)
        {
            if (_tempWidth > _tempHeight)
                _tempWidth = fmodf(_tempWidth, _tempHeight);

            else
                _tempHeight = fmodf(_tempHeight, _tempWidth);
        }

        _commonDiv = _tempWidth == 0 ? _tempHeight : _tempWidth;

        auto _ratioNum = _resolutionHorizontal / _commonDiv;
        auto _checkRatio = _resolutionVertical / _commonDiv;

        if (_checkRatio != 9)
        {
            auto _commonMulp = 9 / _checkRatio;
            _ratioNum = _ratioNum * _commonMulp;
        }

        auto _ratioMultiplier = 0.0625F * _ratioNum;

        memcpy(VIEWPORT3D_ADDR + 0x20, &_ratioMultiplier, 0x04);

        auto _heightFactor = _resolutionVertical / 1080;
        auto _widthCalc = floorf(_resolutionHorizontal / _heightFactor);

        if (INSTRUCTION_LIMIT_ASPECT.size() == 0x00)
        {
            INSTRUCTION_LIMIT_ASPECT.resize(0x09);
            memcpy(INSTRUCTION_LIMIT_ASPECT.data(), VIEWPORT_LIMIT + 0x98, 0x09);
        }

        auto _zoomMultiplier = 16 / _ratioNum;

        if (_ratioMultiplier < 1)
        {
            memset(VIEWPORT_LIMIT + 0x98, 0x90, 0x09);
            memcpy(VIEWPORT3D_ADDR + 0x28, &_zoomMultiplier, 0x04);
        }

        else
        {
            memcpy(VIEWPORT_LIMIT + 0x98, INSTRUCTION_LIMIT_ASPECT.data(), 0x09);
            memcpy(VIEWPORT3D_ADDR + 0x28, "\x00\x00\x80\x3F", 0x04);
        }

        POSITIVE_ASPECT_OFFSET = ceilf(0.177F * (_widthCalc - 1440));
        NEGATIVE_ASPECT_OFFSET = POSITIVE_ASPECT_OFFSET * -1;

        auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");

        if (_fetchMemory)
        {
            memcpy(_fetchMemory, &POSITIVE_ASPECT_OFFSET, 0x04);
            memcpy(_fetchMemory + 0x04, &NEGATIVE_ASPECT_OFFSET, 0x04);
        }

        if (*RADAR_STRUCT)
        {
            auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
            auto _isHudDraw = true;

            if (_fetchHudDraw)
                memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

            *reinterpret_cast<int*>(*RADAR_STRUCT + 0xBBC) = POSITIVE_ASPECT_OFFSET;
            *reinterpret_cast<int*>(*RADAR_STRUCT + 0xBE0) = POSITIVE_ASPECT_OFFSET;

            *reinterpret_cast<char*>(*RADAR_STRUCT + 0xBE5) = _isHudDraw ? 0x60 : 0x00;
            *reinterpret_cast<float*>(*RADAR_STRUCT + 0xBC0) = _isHudDraw ? 1.0 : 0.0;
        }
    }
}

void RETRY_BATTLES()
{
    // Declare the worlds and rooms in which Retry **cannot** execute.
    auto _checkBlacklist = (AREA::Current->World == 0x04 && AREA::Current->Room >= 0x15 && AREA::Current->Room <= 0x1A) ||
        (AREA::Current->World == 0x12 && ((AREA::Current->Room >= 0x16 && AREA::Current->Room <= 0x1C) || AREA::Current->Room == 0x14));

    // If the arrays are not yet initialize, initialize them.
    if (INST_MAPJUMPTASK.size() == 0x00)
    {
        INST_MAPJUMPTASK.resize(0x05);
        INST_CONTINUELOAD.resize(0x05);

        INST_CAMPBITWISE.resize(0x07);
        INST_CAMPINIT.resize(0x08);

        memcpy(INST_MAPJUMPTASK.data(), reinterpret_cast<char*>(AREA::MapJump) + 0x1F2, 0x05);
        memcpy(INST_CONTINUELOAD.data(), reinterpret_cast<char*>(AREA::MapJump) + 0x1D8, 0x05);

        memcpy(INST_CAMPBITWISE.data(), reinterpret_cast<char*>(CMENU_OFFSET) + 0x1A7, 0x07);
        memcpy(INST_CAMPINIT.data(), reinterpret_cast<char*>(CMENUINIT_OFFSET), 0x08);
    }

    // If the room we are in is not blacklisted:
    if (!_checkBlacklist)
    {
        // Check if we are in a cutscene.
        bool _isCutscene = *YS::EVENT::Event && *reinterpret_cast<int*>(*YS::EVENT::Event + 0x04) != 0xCAFEEFAC
                                             && *reinterpret_cast<int*>(*YS::EVENT::Event + 0x04) != 0xEFACCAFE;

        // If not in a cutscene, and is in a Boss Battle, and Retry State is not denoted:
        if (!_isCutscene && *AREA::BattleStatus == 0x02 && RETRY_STATE.size() == 0x00)
        {
            // Check if on Hades Escape.
            HADES_ESCAPE = AREA::Current->World == 0x06 && AREA::Current->Room == 0x05 && AREA::Current->Set.Event == 0x6F;

            // If on Hades Escape, denote and initialize it.
            if (HADES_ESCAPE && HADES_ITERATOR == 0xFF)
                HADES_ITERATOR = 0x00;

            // Read the current state of the room for Retry.
            RETRY_STATE.resize(0x10FC0);
            memcpy(RETRY_STATE.data(), AREA::SaveData, 0x10FC0);

            // Add the entries for Retry as well as Prepare.
            ReFined::Continue::Add(0x00, PREPARE_ENTRY);
            ReFined::Continue::Add(0x00, RETRY_ENTRY);
        }

        // If not retrying, and is Hades Escape:
        if (RETRY_MODE == 0x00 && HADES_ESCAPE && HADES_ITERATOR != 0xFF)
        {
            // If Hades Escape hit an intermission, note it down.
            if (*AREA::BattleStatus == 0x01 && !HADES_CHANGED)
            {
                HADES_ITERATOR++;
                HADES_CHANGED = true;
            }

            // If Hades Escape is in battle once again, note it down.
            if (*AREA::BattleStatus != 0x01 && HADES_CHANGED)
                HADES_CHANGED = false;

            // If there have been 3 intermissions:
            if (HADES_ITERATOR == 0x03)
            {
                // Reset Hades Escape variables.
                HADES_ITERATOR = 0xFF;
                HADES_ESCAPE = false;

                // Reinstate Area Initializers to allow for game progression.
                memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
                memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);
            }
        }

        // If on title or the battle isn't a Boss Battle:
        else if (*YS::TITLE::IsTitle || *AREA::BattleStatus != 0x02)
        {
            // If retrying, not on title, and Retry State has been noted:
            if (RETRY_MODE != 0x00 && RETRY_STATE.size() != 0x00 && !*YS::TITLE::IsTitle)
            {
                // Restore the Retry State.
                memcpy(const_cast<char*>(AREA::SaveData), RETRY_STATE.data(), 0x10FC0);

                // If Retry Mode is 0x02, meaning a Prepare Menu has been requested:
                if (RETRY_MODE == 0x02)
                {
                    // Prepare the NOP state.
                    vector<uint8_t> _nopArrayCamp(0x08);
                    fill(_nopArrayCamp.begin(), _nopArrayCamp.begin() + 0x08, 0x90);

                    // Write the camp menu options bitwise (Items, Abilities, Customize, Party)
                    uint8_t _campBitwise = 0x0F;
                    memcpy(YS::MENU::CampOptions, &_campBitwise, 0x01);

                    // NOP the instructions that set and refresh the camp menu options bitwise.
                    memcpy(CMENUINIT_OFFSET, _nopArrayCamp.data(), 0x08);
                    memcpy(CMENU_OFFSET + 0x1A7, _nopArrayCamp.data(), 0x07);

                    // Summon the camp menu.
                    YS::MENU::CampStart(0x00, 0x00);
                }

                // Denote we are no longer retrying.
                RETRY_MODE = 0x00;
            }

            // If we are NOT retrying and not in a menu, but the Retry State has been denoted:
            if (RETRY_MODE == 0x00 && RETRY_STATE.size() != 0x00 && !*YS::MENU::IsMenu)
            {
                // Restore the Area Init functions so we can progress.
                memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
                memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);

                // Restore all Camp initialization instructions.
                memcpy(CMENUINIT_OFFSET, INST_CAMPINIT.data(), 0x08);
                memcpy(CMENU_OFFSET + 0x1A7, INST_CAMPBITWISE.data(), 0x07);

                // If the continue menu has been edited, remove all Retry elements.
                if (ReFined::Continue::Children.size() > 0x02)
                {
                    ReFined::Continue::Remove(0x01);
                    ReFined::Continue::Remove(0x00);
                }

                // Clear the Retry State.
                RETRY_STATE.clear();
            }
        }


        // Fetch Menu selection and the pointer to the Game Over screen.
        uint8_t _fetchSelectMenu = *YS::MENU::DialogBase ? ((*YS::MENU::DialogBase + 0xD48) ? *(*YS::MENU::DialogBase + 0xD48) : 0x80) : 0x80;

        // If the Game Over menu exists and the Retry State has been noted:
        if (*YS::MENU::GameOver && RETRY_STATE.size() != 0x00)
        {
            // If on Hades Escape, reset all Hades Escape variables.
            if (HADES_ESCAPE && HADES_ITERATOR != 0x00)
            {
                HADES_ITERATOR = 0x00;
                HADES_CHANGED = false;
            }

            // If the menu selection is valid:
            if (_fetchSelectMenu <= 0x04)
            {
                // Determine the Retry Mode based on selection.
                RETRY_MODE = _fetchSelectMenu == 0x00 ? 0x01 : (_fetchSelectMenu == 0x01 ? 0x02 : 0x00);
                RETRY_MODE = *reinterpret_cast<const uint8_t*>(YS::MENU::SubMenuType) == 0xFF ? RETRY_MODE : 0x00;

                // If we are retrying, NOP all Area Init instructions to not change the room state.
                if (RETRY_MODE != 0x00)
                {
                    char* _nopArray = new char[0x05];
                    fill(_nopArray, _nopArray + 0x05, 0x90);

                    memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1F2, _nopArray, 0x05);
                    memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1D8, _nopArray, 0x05);
                }

                // If we are not, restore the prior instructions.
                else
                {
                    memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
                    memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);
                }
            }
        }

        // If we are retrying, we are still in a boss battle, and the command menu type is "MICKEY"
        else if (RETRY_MODE > 0x00 && *AREA::BattleStatus == 0x02 && *COMMAND_TYPE == 0x01)
        {
            // Restore the Area Init functions so we don't loop after Mickey leaves.
            memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
            memcpy(reinterpret_cast<char*>(AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);

            // If the continue menu has been edited, remove all Retry elements.
            if (ReFined::Continue::Children.size() > 0x02)
            {
                ReFined::Continue::Remove(0x01);
                ReFined::Continue::Remove(0x00);
            }

            // Denote we are no longer retrying.
            RETRY_MODE = 0x00;
        }
    }
}

void PROCESS_FORM_KEYBLADES()
{
    if (*YS::MENU::IsMenu && *YS::MENU::SubMenuType == 0x02 && *CURRENT_SUBMENU == 0x00 && *YS::HARDPAD::Input & YS::HARDPAD::BUTTONS::TRIANGLE && !KEYBLADE_DEBOUNCE)
    {
        auto _fetchSelect = *YS::MENU::SubOptionSel;

        // Calculate the maximum selection we can make.
        auto _calculateForms = YS::ITEM::GetNumBackyard(0x001A) + YS::ITEM::GetNumBackyard(0x001D) + YS::ITEM::GetNumBackyard(0x001F);

        if (_fetchSelect)
        {
            if (*_fetchSelect != 0x00 && *_fetchSelect <= _calculateForms)
            {
                uint16_t* _currentFormKeyPtr = nullptr;
                uint16_t* _currentRegularKeyPtr = reinterpret_cast<uint16_t*>(AREA::SaveData + 0x24F0);

                auto _fetchItemTable = YS::ITEM_TABLE::Each(nullptr);
                bool _isCurrentForm = false;

                for (int i = 1; i <= 5; i++)
                {
                    auto _fetchFormKey = reinterpret_cast<uint16_t*>(AREA::SaveData + 0x32BC + 0x38 * i);
                    _fetchItemTable = YS::ITEM_TABLE::Get(*_fetchFormKey);

                    if (*_fetchFormKey == 0x0000)
                        continue;

                    auto _fetchFormItempic = *reinterpret_cast<uint16_t*>(_fetchItemTable + 0x14);

                    if (_fetchFormItempic == *YS::ITEMPIC::LoadedId)
                    {
                        _currentFormKeyPtr = _fetchFormKey;

                        if (*(AREA::SaveData + 0x3524) == i)
                            _isCurrentForm = true;

                        break;
                    }
                }

                if (!_currentFormKeyPtr)
                    return;

                auto _fetchFormKey = *_currentFormKeyPtr;
                auto _fetchRegularKey = *_currentRegularKeyPtr;

                *_currentRegularKeyPtr = _fetchFormKey;
                *_currentFormKeyPtr = _fetchRegularKey;

                TARGET_KEYBLADE = _fetchFormKey;

                if (_isCurrentForm)
                    TARGET_CURRENT_FORM_KEYBLADE = _fetchRegularKey;

                Tz::ItemInfo::SetItemInfo(*MENU_ITEMS, 0x05, 0x00);
                Tz::ItemInfo::MakeMsgTbl(*MENU_ITEMS, 0x00, 0x00);

                ITEM_COMMIT();

                SOUND::PlaySFX(0x02);

                PENDING_KEYBLADE_UPDATE = true;
            }
        }

        KEYBLADE_DEBOUNCE = true;
    }

    else if (KEYBLADE_DEBOUNCE && (*YS::HARDPAD::Input & YS::HARDPAD::BUTTONS::TRIANGLE) == 0x0000)
        KEYBLADE_DEBOUNCE = false;

    if (!*YS::MENU::IsMenu && PENDING_KEYBLADE_UPDATE)
    {
        YS::PARTY::ChangeWeapon(nullptr, 0x01, false, TARGET_KEYBLADE);

        if (TARGET_CURRENT_FORM_KEYBLADE != 0x0000)
            YS::PARTY::ChangeWeapon(nullptr, 0x01, true, TARGET_CURRENT_FORM_KEYBLADE);

        TARGET_KEYBLADE = 0x0000;
        TARGET_CURRENT_FORM_KEYBLADE = 0x0000;

        PENDING_KEYBLADE_UPDATE = false;
    }
}

void FIX_UP_CONFIG()
{
    if (*YS::TITLE::IsTitle)
    { 
        if (TITLE_FILENAME[0x00] == 0x00)
            Tz::CmData::MakeFname(TITLE_FILENAME, const_cast<char*>(IS_FASTBOOT ? "title_fast.2ld" : "title.2ld"));

        auto _fetchCacheBuff = YS::CACHE_BUFF::SearchByName(TITLE_FILENAME, -1);

        if (!_fetchCacheBuff)
            return;

        auto _fetchBinarc = *reinterpret_cast<char**>(_fetchCacheBuff + 0x58);

        if (!_fetchBinarc)
            return;

        auto _fetchMagic = *reinterpret_cast<int*>(_fetchBinarc);
        auto _fetchPrimary = *reinterpret_cast<int*>(_fetchBinarc + 0x08);
        auto _fetchLocation = *reinterpret_cast<int*>(_fetchBinarc + 0x0C);

        auto _isValid = _fetchMagic == 0x01524142 && _fetchPrimary != 0x00 && _fetchLocation != 0x00;

        if (!_isValid)
            return;
        
        if (YS::BINARC::get_info_by_tag(_fetchBinarc, 0x1C, 0x6C746974, 0))
        {
            auto _fetchLaydIntPtr = *reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(_fetchBinarc, 0x1C, 0x6C746974, 0) + 0x08);

            if (_fetchLaydIntPtr == 0x00)
                return;

            auto _fetchLaydTrue = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchLaydIntPtr));

            if (!_fetchLaydTrue)
                return;

            auto _fetchOffsetSeqd = *reinterpret_cast<uint32_t*>(_fetchLaydTrue + 0x0B8C);
            auto _coordsSettingsStart = _fetchLaydTrue + _fetchOffsetSeqd + 0x06BC;

            if (*PROMPT_MODE)
            {
                memcpy(_coordsSettingsStart + 0x04, "\xE8\x00\x00\x00", 0x04);
                memcpy(_coordsSettingsStart + 0x0C, "\xFE\x00\x00\x00", 0x04);
            }

            else
            {
                auto _fetchConfig = IS_STEAM ? *(PC::STEAM::MareConfig + 0x1C) : *(PC::EGS::MareConfig + 0x1C);

                memcpy(_coordsSettingsStart + 0x04, _fetchConfig == 0x01 ? "\xBC\x00\x00\x00" : "\xD2\x00\x00\x00", 0x04);
                memcpy(_coordsSettingsStart + 0x0C, _fetchConfig == 0x01 ? "\xD2\x00\x00\x00" : "\xE8\x00\x00\x00", 0x04);
            }
        }
    }
}

void RETRIBUTION_LOGIC()
{
    if (!RETRIBUTION_INIT)
    {
        memcpy(*YS::ITEM::WeaponEntry + 0x017C, "\x03\x0A\x00\x00\x07\x0A\x00\x00", 0x08);
        memcpy(*YS::ITEM::WeaponEntry + 0x030C, "\x04\x0A\x00\x00\x08\x0A\x00\x00", 0x08);
        memcpy(*YS::ITEM::WeaponEntry + 0x0900, "\x05\x0A\x00\x00\x09\x0A\x00\x00", 0x08);
        memcpy(*YS::ITEM::WeaponEntry + 0x0A90, "\x06\x0A\x00\x00\x0A\x0A\x00\x00", 0x08);

        RETRIBUTION_INIT = true;
    }

    if (!*YS::TITLE::IsTitle && *AREA::IsInMap)
    {
        vector<uint16_t> _soraEquip =
        {
            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x24F0),
            YS::ITEM::GetNumBackyard(0x001A) ? *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x32F4) : UINT16_MAX,
            YS::ITEM::GetNumBackyard(0x001D) ? *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x33D4) : UINT16_MAX,
            YS::ITEM::GetNumBackyard(0x001F) ? *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x339C) : UINT16_MAX
        };

        if (WEAPON_MEMORY.size() == 0x00)
        {
            auto _fetchItem = YS::ITEM_TABLE::Each(nullptr);

            while (_fetchItem)
            {
                auto _fetchId = *reinterpret_cast<uint16_t*>(_fetchItem);
                auto _fetchType = *reinterpret_cast<uint16_t*>(_fetchItem + 0x02);

                if (_fetchType == 0x0002 && _fetchId != 0x0300 && _fetchId != 0x0301)
                    WEAPON_MEMORY.push_back(reinterpret_cast<uint16_t*>(_fetchItem));

                _fetchItem = YS::ITEM_TABLE::Each(_fetchItem);
            }
        }

        if (PARAM_RETRIBUTION == UINT16_MAX)
        {
            auto _itemTableAbsolution = YS::ITEM_TABLE::Get(0x0301);
            auto _itemTableRetribution = YS::ITEM_TABLE::Get(0x0300);

            if (!_itemTableAbsolution || !_itemTableRetribution)
                return;

            auto _paramAbsolution = reinterpret_cast<uint16_t*>(AREA::SaveData + 0x10024);
            auto _paramRetribution = reinterpret_cast<uint16_t*>(AREA::SaveData + 0x10020);

            if (*_paramAbsolution == 0x0000)
                *_paramAbsolution = 0x0050;

            if (*_paramRetribution == 0x0000)
                *_paramRetribution = 0x0050;

            *(_itemTableAbsolution + 0x06) = *_paramAbsolution;
            *(_itemTableRetribution + 0x06) = *_paramRetribution;

            PARAM_ABSOLUTION = *_paramAbsolution;
            PARAM_RETRIBUTION = *_paramRetribution;
        }

        else
        {
            auto _amountAbsolution = YS::ITEM::GetNumBackyard(0x0301);
            auto _amountRetribution = YS::ITEM::GetNumBackyard(0x0300);

            if (_amountRetribution != 0x01 && HAS_RETRIBUTION)
            {
                auto _findKeyEquip = find_if(_soraEquip.begin(), _soraEquip.end(), [](uint16_t x) { return x == 0x0300; });

                if (_findKeyEquip != _soraEquip.end() && _amountRetribution != 0x00)
                    YS::ITEM::ReduceBackyard(0x0300, _amountRetribution);

                else if (_findKeyEquip == _soraEquip.end() && _amountRetribution != 0x01)
                {
                    YS::ITEM::ReduceBackyard(0x0300, _amountRetribution);
                    YS::ITEM::GetBackyard(0x0300, 0x01);
                }
            }

            if (_amountAbsolution != 0x01 && HAS_ABSOLUTION)
            {
                auto _findKeyEquip = find_if(_soraEquip.begin(), _soraEquip.end(), [](uint16_t x) { return x == 0x0301; });

                if (_findKeyEquip != _soraEquip.end() && _amountAbsolution != 0x00)
                    YS::ITEM::ReduceBackyard(0x0301, _amountAbsolution);

                else if (_findKeyEquip == _soraEquip.end() && _amountAbsolution != 0x01)
                {
                    YS::ITEM::ReduceBackyard(0x0301, _amountAbsolution);
                    YS::ITEM::GetBackyard(0x0301, 0x01);
                }
            }

            if (*YS::MENU::IsMenu && (*YS::MENU::SubMenuType == 0x02 || *YS::MENU::SubMenuType == 0x05))
            {
                if ((*YS::ITEMPIC::LoadedId == 420 || *YS::ITEMPIC::LoadedId == 421) && *YS::HARDPAD::Input & YS::HARDPAD::BUTTONS::L3 && !DEBOUNCE_RETRIBUTION)
                {
                    DEBOUNCE_RETRIBUTION = true;

                    auto _seekItem = *YS::ITEMPIC::LoadedId == 420 ? 0x0300 : 0x0301;
                    auto _seekParamIndex = *YS::ITEMPIC::LoadedId == 420 ? INDEX_RETRIBUTION : INDEX_ABSOLUTION;

                    auto _beginIndex = _seekParamIndex;

                    _seekParamIndex++;

                    while (_beginIndex != _seekParamIndex)
                    {
                        if (_seekParamIndex >= WEAPON_MEMORY.size())
                            _seekParamIndex = 0x0000;

                        if (_beginIndex == _seekParamIndex)
                            break;

                        auto _fetchItemId = *WEAPON_MEMORY[_seekParamIndex];
                        auto _findKeyEquip = find_if(_soraEquip.begin(), _soraEquip.end(), [_fetchItemId](uint16_t x) { return x == _fetchItemId; });

                        if (_findKeyEquip != _soraEquip.end() || YS::ITEM::GetNum(_fetchItemId, 0x01) == 0x00)
                        {
                            _seekParamIndex++;
                            continue;
                        }

                        if (_seekItem == 0x0300)
                        {
                            PARAM_RETRIBUTION = *(YS::ITEM_TABLE::Get(_fetchItemId) + 0x06);

                            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x10020) = PARAM_RETRIBUTION;
                            *(YS::ITEM_TABLE::Get(0x0300) + 0x06) = PARAM_RETRIBUTION;

                            INDEX_RETRIBUTION = _seekParamIndex;
                        }

                        if (_seekItem == 0x0301)
                        {
                            PARAM_ABSOLUTION = *(YS::ITEM_TABLE::Get(_fetchItemId) + 0x06);

                            *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x10024) = PARAM_ABSOLUTION;
                            *(YS::ITEM_TABLE::Get(0x0301) + 0x06) = PARAM_ABSOLUTION;

                            INDEX_ABSOLUTION = _seekParamIndex;
                        }

                        SOUND::PlaySFX(0x02);

                        Tz::ItemInfo::SetItemInfo(*MENU_ITEMS, 0x05, 0x00);
                        Tz::ItemInfo::MakeMsgTbl(*MENU_ITEMS, 0x00, 0x00);

                        ITEM_COMMIT();

                        break;
                    }

                    if (_beginIndex == _seekParamIndex)
                        SOUND::PlaySFX(0x05);
                }

                else if ((*YS::ITEMPIC::LoadedId != 420 && *YS::ITEMPIC::LoadedId != 421) || (*YS::HARDPAD::Input & YS::HARDPAD::BUTTONS::L3) == 0x0000 && DEBOUNCE_RETRIBUTION)
                    DEBOUNCE_RETRIBUTION = false;
            }
        }
    }

    else if (*YS::TITLE::IsTitle)
    {
        PARAM_ABSOLUTION = UINT16_MAX;
        PARAM_RETRIBUTION = UINT16_MAX;
    }
}

void HANDLE_SYNC_LIMIT()
{
    uint16_t _defaultLimits[] = { 0x02BA, 0x02BD, 0x02C0, 0x02AB };

    if (*YS::TITLE::IsTitle && SYNC_LIMIT)
    {
        memcpy(Tz::CmCustom::LS_KH1F_Shortcuts, _defaultLimits, 0x08);
        SYNC_LIMIT = false;
    }

    else if (*AREA::IsInMap && !SYNC_LIMIT)
    {
        for (int i = 0; i < 0x04; i++)
        {
            auto _fetchCommand = *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x10030 + 0x02 * i);

            if (_fetchCommand != 0x0000)
            {
                memcpy(Tz::CmCustom::LS_KH1F_Shortcuts, AREA::SaveData + 0x10030, 0x08);
                SYNC_LIMIT = true;
                return;
            }
        }

        memcpy(AREA::SaveData + 0x10030, _defaultLimits, 0x08);
        SYNC_LIMIT = true;
    }
}

extern "C"
{
    __declspec(dllexport) void OnInit(wchar_t* mod_path)
    {
        FUNCTION_ARRAY =
        {
            {"SOFT_RESET", SOFT_RESET},
            {"AUTOSAVE", AUTOSAVE},
            {"ENFORCE_FRAMERATE", ENFORCE_FRAMERATE},
            {"FIX_UP_CONFIG", FIX_UP_CONFIG},
            {"HANDLE_MUSIC", HANDLE_MUSIC},
            {"HANDLE_RESOURCE", HANDLE_RESOURCE},
            {"HANDLE_AUDIO", HANDLE_AUDIO},
            {"RETRY_BATTLES", RETRY_BATTLES},
            {"HANDLE_SHAKE", HANDLE_SHAKE},
            {"ENFORCE_PROMPTS", ENFORCE_PROMPTS},
            {"FIX_SAVE_POINT", FIX_SAVE_POINT},
            {"DISCORD_RPC", DISCORD_RPC},
            {"HANDLE_ASPECT", HANDLE_ASPECT},
            {"REGISTER_MAGIC", REGISTER_MAGIC},
            {"REGISTER_ABILITY", REGISTER_ABILITY},
            {"SHOW_INFORMATION", SHOW_INFORMATION},
            {"PROCESS_DEATH", PROCESS_DEATH},
            {"ENFORCE_LOCKON", ENFORCE_LOCKON},
            {"HANDLE_GOA_LAND", HANDLE_GOA_LAND},
            {"PROCESS_FORM_KEYBLADES", PROCESS_FORM_KEYBLADES},
            {"RETRIBUTION_LOGIC", RETRIBUTION_LOGIC}, 
            {"HANDLE_SYNC_LIMIT", HANDLE_SYNC_LIMIT}
        };

        // Determine if the MOD is running on STEAM or EPIC.
        IS_STEAM = FindModule("steam_api64.dll");

        // Nullify the camp.2ld adjustment functions.
        if (IS_STEAM)
        {
            auto _campSwitcherFunction = SignatureScan<char*>("\x40\x53\x55\x56\x57\x41\x57\x48\x83\xEC\x40\xE8\x00\x00\x00\x00\x48\x8B\x3D", "xxxxxxxxxxxx????xxx");
            auto _campCopierFunction = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x40\x4C\x8D\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

            memset(_campSwitcherFunction + 0xAF, 0xEB, 0x01);
            memset(_campCopierFunction + 0x1A, 0x90, 0x4E);

            auto _titleCopierFunction = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x57\x48\x81\xEC\xB0\x00\x00\x00\x48\x8B\x05", "xxxxxxxxxxxxxxxx");
            auto _titleSwitcherFunction = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B\x3D", "xxxxxxxxxxxxxxxx????xxx");

            memset(_titleSwitcherFunction + 0xAE, 0xEB, 0x01);
            memset(_titleCopierFunction + 0x2C, 0x90, 0x4E);
        }

        // Nullify all SaveID checks according to the platform in use.

        auto _saveCheckFunction = IS_STEAM ? SignatureScan<char*>("\x40\x55\x56\x57\x48\x81\xEC\xA0\x00\x00\x00\x48\xC7\x44\x24\x38\xFE\xFF\xFF\xFF\x48\x89\x9C\x24\xD0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x90\x00\x00\x00\x8B\xF1\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xED\x8D\x5D\x01\x48\x39\x2D\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x85\xC0\x74\x1D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxx????xx????xxxxxxxx????xx????xxxxxx????xxxxxxxxxx")
                                           : SignatureScan<char*>("\x40\x57\x48\x83\xEC\x50\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x70\x48\x89\x74\x24\x78\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x8B\xF9\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xF6\x48\x39\x35\x00\x00\x00\x00\x0F\x85\x3D\x01\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x38\x48\x85\xC0\x74\x1D\x45\x33\xC9\x44\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx????xx????xxxxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxxx");

        memset(_saveCheckFunction + (IS_STEAM ? 0x189 : 0x138), 0x90, 0x05);
        memset(_saveCheckFunction + (IS_STEAM ? 0x196 : 0x145), 0x90, 0x02);

        memcpy(_saveCheckFunction + (IS_STEAM ? 0x1A1 : 0x150), "\xEB", 0x01);

        // Prevent SOFTRESET from resetting Fade status for a smooth-ass transition.

        memset(reinterpret_cast<char*>(dk::SOFTRESET::SoftResetThread) + 0x1ED, 0x90, 0x05);

        #ifndef BUILD_ARCHIPELAGO_LITE
        Tz::HookIntro::Submit();
        Tz::HookConfig::Submit();
        #endif

        #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
        MOD_PATH = mod_path;

        // Fetch the prompt mode byte according to the game version.
        PROMPT_MODE = ResolveRelativeAddress<bool*>("\x40\x57\x48\x83\xEC\x20\x4C\x8B\x0D\x00\x00\x00\x00\x33\xD2\x4D\x85\xC9\x49\x8D\x81\xA0\x12\x00\x00\x48\x0F\x45\xD0\x4D\x8D\x81\x3C\x02\x00\x00\x48\x85\xD2\x0F\x84\x2B\x01\x00\x00\x33\xC0\x4D\x85\xC9\x49\x0F\x45\xC0\x48\x85\xC0\x74\x16\x48\x63\x82\x00\x02\x00\x00\x48\xC1\xE0\x08\x80\x7C\x10\x3B\x00\x0F\x85\x00\x00\x00\x00\x48\x85\xD2\x0F\x84\xFE\x00\x00\x00\x33\xC0\x4D\x85\xC9\x49\x0F\x45\xC0\x48\x85\xC0\x74\x16", "xxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxx", IS_STEAM ? 0x969 : 0x959);

        // Fetch the launch parameters and parse them.

        auto _commandLine = wstring(GetCommandLine());

        IS_FASTBOOT = _commandLine.find(L"-fastboot") != wstring::npos;
        IS_NOASPECT = _commandLine.find(L"-noaspect") != wstring::npos;

        // If FASTBOOT is active, replace the Title Screen accordingly.

        if (IS_FASTBOOT)
            memcpy(YS::TITLE::Title2LD, "title_fast.2ld", 0x0E);

        // If NOASPECT is active, remove the Aspect Handler.
        if (IS_NOASPECT)
            FUNCTION_ARRAY.erase("HANDLE_ASPECT");

        vector<uint8_t> _absoluteInstructionJMP =
        {
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        auto _hotpatchNullTask = SignatureScan<char*>("\x49\x8B\x40\x18\xC7\x40\x04\x40\x3F\x3F\x3F\x49\x8B\x48\x18\x48\x8D\x41\x08\x49\x89\x40\x18\x8B\x02\x89\x01\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx");

        memcpy(_hotpatchNullTask + 0x17, "\xEB\x22\x89\x01\xC3", 0x05);
        memcpy(_hotpatchNullTask + 0x3B, "\x83\xFA\x04\x74\xDB\x8B\x02\xEB\xD5", 0x09);

        // Patch the voice line thingie.
        auto _voiceLinePatch = SignatureScan<char*>("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xE0\x48\x81\xEC\x20\x01\x00\x00\x48\xC7\x44\x24\x60\xFE\xFF\xFF\xFF\x48\x89\x9C\x24\x60\x01\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
        memcpy(_voiceLinePatch + 0x162, "\x31\xC0\x90\x90\x90", 0x05);

        // Decouple the camera from delta time.

        uint32_t _fetchAddress = 0x00;
        auto _fpsCameraUpdate = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x60\x8B\x81\xE0\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx");
        auto _addressCameraFloat = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x60\x8B\x81\xE0\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx", 0x4B);

        *reinterpret_cast<float*>(_addressCameraFloat - 0x04) = 1.0;

        memcpy(&_fetchAddress, _fpsCameraUpdate + 0x4B, 0x04);
        _fetchAddress -= 0x04;

        memcpy(_fpsCameraUpdate + 0x4B, &_fetchAddress, 0x04);

        // Initialization of all MENU handlers [INTRO, CONFIG, CONTINUE]

        YS::PANACEA_ALLOC::Allocate("IS_HUDDRAW", 0x04);
        YS::PANACEA_ALLOC::Allocate("IS_TIMESTOP", 0x04);

        YS::PANACEA_ALLOC::Allocate("ASPECT_INFORMATION", 0x08);
        YS::PANACEA_ALLOC::Allocate("CHANGE_WEAPON_QUEUE", 0x140);

        auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
        memset(_fetchHudDraw, 0x01, 0x01);

        ReFined::Continue::Submit();

        // If NO_ASPECT is not called, handle all aspect modifications.

        if (!IS_NOASPECT)
        { 
            // Prevent the game from adjusting the aspect automatically.

            auto _fetchAdjustment = SignatureScan<char*>("\x48\x83\xEC\x28\x0F\x10\x41\x48\x4C\x8B\xC9\x4C\x8B\xD2\xF3\x0F\x10\x25\x00\x00\x00\x00\x0F\x57\xED\x0F\x11\x02\x41\x0F\x10\x00\x49\x8B\x41\x40", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx");

            memset(_fetchAdjustment + 0xF6, 0x90, 0x06);
            memset(_fetchAdjustment + 0x101, 0x90, 0x06);

            // Kill the enforcer in dk::MISSION_GAUGE::update so I don't want to kill myself.
            
            auto _fetchMissionUpdate = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\xF9\xE8", "xxxxxxxxxxxxxx");
            memset(_fetchMissionUpdate + 0x119, 0xEB, 0x01);

            // Fetch all functions that handle fade-in and fade-outs in any way within the 2dFade rectangle.

            auto _fetchAllFade = MultiSignatureScan("\x41\xB8\xFF\xFF\xFF\xFF\x48\x8D\x0D\x00\x00\x00\x00\x0F\xB7\xD3\x66\xF7\xD2\xE8\x00\x00\x00\x00\xB8\x01\x01\x00\x00", "xxxxxxxxx????xxxxxxx????xxxxx");

            for (auto _function : _fetchAllFade)
            {
                uint32_t _fadeValue = 0x800;
                vector<uint8_t> _replaceFade{ 0xBA, 0x00, 0xFF, 0x00, 0x00 };

                memcpy(_function + 0x0D, "\x90\x90\x90\x90\x90\x90", 0x06);
                memcpy(_function + 0x0D, _replaceFade.data(), 0x05);

                memcpy(_function + 0x19, &_fadeValue, 0x04);
            }

            // Disables culling. Causes some side effects that I don't believe anyone will notice.

            auto _fetchCulling3D = SignatureScan<char*>("\x48\x8B\xC4\x48\x89\x58\x18\x48\x89\x70\x20\x55\x57\x41\x54\x41", "xxxxxxxxxxxxxxxx");

            memset(_fetchCulling3D + 0x11D, 0xEB, 0x01);
            memset(_fetchCulling3D + 0x12B, 0xEB, 0x01);
            memset(_fetchCulling3D + 0x133, 0xEB, 0x01);
            memset(_fetchCulling3D + 0x141, 0xEB, 0x01);
            memset(_fetchCulling3D + 0x149, 0xEB, 0x01);
            memset(_fetchCulling3D + 0x152, 0xEB, 0x01);

            auto _fetchCulling2D = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xFA\xE8", "xxxxxxxxxxxxxxxxxxx");

            memset(_fetchCulling2D + 0x06C, 0x00, 0x01);
        }
        #endif

        #ifndef BUILD_NMC
            // Prevent MAGIC clearing since we handle that now, and because it causes a crash.

            auto _funcMagicClear = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x48\x89\x74\x24\x50\x48\x8B\xD8\x4C\x89\x74\x24\x58\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x0F\x29\x74\x24\x30\xF3\x0F\x10\x35\x00\x00\x00\x00\x0F\x29\x7C\x24\x20\x0F\x57\xFF\x48\x85\xDB\x75\x08", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxxxxxxxxxxx");
            memset(_funcMagicClear + 0x18A, 0x90, 0x05);

        #ifndef BUILD_ARCHIPELAGO_LITE
            // Handle reFined.cfg file.

            wchar_t _configPath[MAX_PATH];

            wcscpy(_configPath, mod_path);
            wcscat(_configPath, L"\\dll\\reFined.cfg");

            auto _wideStr = wstring(_configPath);

            mINI::INIFile _configFile(string(_wideStr.begin(), _wideStr.end()));
            mINI::INIStructure _configStruct;

            _configFile.read(_configStruct);

            auto _fetchButtons = _configStruct["General"]["resetCombo"];

            if (_fetchButtons.find("NONE") == string::npos)
            {
                size_t _buttonPos = 0;
                string _buttonToken;
                string _tempStr = _fetchButtons;

                while ((_buttonPos = _tempStr.find(" + ")) != string::npos)
                {
                    _buttonToken = _tempStr.substr(0, _buttonPos);
                    _tempStr.erase(0, _buttonPos + 3);

                    transform(_buttonToken.begin(), _buttonToken.end(), _buttonToken.begin(), ::toupper);

                    RESET_COMBO |= YS::HARDPAD::BUTTONS_MAP[_buttonToken];

                    if (_tempStr.find(" + ") == string::npos)
                        RESET_COMBO |= YS::HARDPAD::BUTTONS_MAP[_tempStr];
                }
            }

            DISCORD_ENABLED = _configStruct["General"]["discordRPC"] == "true" ? true : false;

            ALLOW_NOHUD = _configStruct["General"]["allowNoHud"] == "true" ? true : false;
            ALLOW_TIMESTOP = _configStruct["General"]["allowTimeStop"] == "true" ? true : false;

            if (!DISCORD_ENABLED)
                FUNCTION_ARRAY.erase("DISCORD_RPC");

            ROOM_AMOUNT = atoi(_configStruct["General"]["saveRoomAmount"].c_str());
            SAVE_SLOT_OFFSET = atoi(_configStruct["General"]["saveSlot"].c_str());

            if (ROOM_AMOUNT == 0x00)
                ROOM_AMOUNT = 1;
        #endif
        #endif
    }

    __declspec(dllexport) void OnFrame()
    {
        // If Re:Fined's post-initialization-initialization is not done:
        if (!INITIALIZED)
        {
            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            // Abort function if the game is not loaded fully yet.
            auto _fetchFake = YS::MESSAGE::GetData(0x8ADC);

            if (!_fetchFake)
                return;

            // Trying to initialize this in OnInit causes moduleInfo to get corrupt. I have no fucking idea why.
            if (!ITEM_COMMIT)
                ITEM_COMMIT = SignatureScan<void(*)()>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x40\x45\x32", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

            if (YS::FILE::GetSize("obj/W_EX010_RF.mdlx"))
                HAS_RETRIBUTION = true;

            if (YS::FILE::GetSize("obj/W_EX010_RX.mdlx"))
                HAS_ABSOLUTION = true;

            // Allocate space for "00shopface.bin".
            if (!YS::PANACEA_ALLOC::Get("00shopface.bin"))
            {
                YS::PANACEA_ALLOC::Allocate("00shopface.bin", YS::FILE::GetSize("00shopface.bin"));
                YS::FILE::Read("00shopface.bin", YS::PANACEA_ALLOC::Get("00shopface.bin"));
            }

            if (!HAS_RETRIBUTION && !HAS_ABSOLUTION)
                FUNCTION_ARRAY.erase("RETRIBUTION_LOGIC");

            static Tz::HookConfig::Entry _musicConfig{ 0x01, 0x5718, vector<uint16_t>{ 0x5719 }, vector<uint16_t>{ 0x571A }, vector<uint16_t>{ 0x0000 }, 0x0000, nullptr };
            static Tz::HookIntro::Entry _musicIntro{ 0x01, 0x5735, 0x5718, vector<uint32_t>{ }, vector<uint32_t>{ }, vector<uint32_t>{ }, 0x0000, nullptr };

            static Tz::HookConfig::Entry _resourceConfig{ 0x01, 0x571F, vector<uint16_t>{ 0x5719 }, vector<uint16_t>{ 0x573B }, vector<uint16_t>{ 0x0000 }, 0x0000, nullptr };
            static Tz::HookIntro::Entry _resourceIntro{ 0x01, 0x5736, 0x571F, vector<uint32_t>{ }, vector<uint32_t>{ }, vector<uint32_t>{ }, 0x0000, nullptr };

            // This code block handles AUDIO packs.

            vector<size_t> _loadedLangs
            {
                YS::FILE::GetSize("voice/jp/battle/tt0_sora.win32.scd"), 
                YS::FILE::GetSize("voice/es/battle/tt0_sora.win32.scd"),
                YS::FILE::GetSize("voice/de/battle/tt0_sora.win32.scd"),
                YS::FILE::GetSize("voice/bg/battle/tt0_sora.win32.scd"),
            };

            static Tz::HookConfig::Entry _subAudioConfig{ 0x0000, 0x572B, vector<uint16_t>(), vector<uint16_t>(), vector<uint16_t>(), 0x0000, nullptr };
            static Tz::HookIntro::Entry _subAudioIntro{ 0x0000, 0x572B, 0x570B, vector<uint32_t>(), vector<uint32_t>(), vector<uint32_t>(), 0x0000, nullptr };

            static Tz::HookConfig::Entry _mainAudioConfig{ 0x01, 0x570B, vector<uint16_t>{ 0x570C }, vector<uint16_t>{ 0x570D }, vector<uint16_t> { 0x0000 } };
            static Tz::HookIntro::Entry _mainAudioIntro{ 0x01, 0x5734, 0x570B, vector<uint32_t>{ }, vector<uint32_t>{ }, vector<uint32_t> { } };

            for (int i = 1; i < 4; i++)
            {
                if (_loadedLangs[i] != 0x0000)
                {
                    _subAudioConfig.Buttons.push_back(0x570E + 0x02 * i);
                    _subAudioConfig.Descriptions.push_back(0x570F + 0x02 * i);
                    _subAudioConfig.Count += 1;
                }
            }

            if (_loadedLangs[0] != 0x0000)
            {
                _mainAudioConfig.Buttons.push_back(0x570E);
                _mainAudioConfig.Descriptions.push_back(0x570F);
                _mainAudioConfig.Toggles.push_back(0x0004);

                _mainAudioConfig.Count += 0x01;

                if (_subAudioConfig.Count >= 0x01)
                {
                    _mainAudioConfig.Buttons.push_back(_subAudioConfig.Count == 0x01 ? _subAudioConfig.Buttons[0] : 0x5716);
                    _mainAudioConfig.Descriptions.push_back(_subAudioConfig.Count == 0x01 ? _subAudioConfig.Descriptions[0] : 0x5717);

                    uint16_t _fetchBitwise = pow(2, ((_subAudioConfig.Buttons[0] - 0x570E) / 2) + 0x02);
                    _mainAudioConfig.Toggles.push_back(_subAudioConfig.Count == 0x01 ? _fetchBitwise : 0x0002);

                    _mainAudioConfig.Count += 0x01;
                }
            }

            else if (_subAudioConfig.Count >= 0x01)
            {
                if (_subAudioConfig.Count < 0x03)
                {
                    for (int i = 0; i < _subAudioConfig.Count; i++)
                    {
                        _mainAudioConfig.Buttons.push_back(_subAudioConfig.Buttons[i]);
                        _mainAudioConfig.Descriptions.push_back(_subAudioConfig.Descriptions[i]);
                        _mainAudioConfig.Toggles.push_back(pow(2, ((_subAudioConfig.Buttons[i] - 0x570E) / 2) + 0x02));

                        _mainAudioConfig.Count += 0x01;
                    }
                }
            }

            if (_subAudioConfig.Count >= 0x01)
            {
                for (int i = 0; i < _subAudioConfig.Count; i++)
                {
                    uint16_t _fetchBitwise = pow(2, ((_subAudioConfig.Buttons[i] - 0x570E) / 2) + 0x02);
                    _subAudioConfig.Toggles.push_back(_fetchBitwise);
                }

                _subAudioIntro.Count = _subAudioConfig.Count;

                for (int i = 0; i < _subAudioConfig.Count; i++)
                {
                    _subAudioIntro.Buttons.push_back(_subAudioConfig.Buttons[i]);
                    _subAudioIntro.Descriptions.push_back(_subAudioConfig.Descriptions[i]);
                    _subAudioIntro.Toggles.push_back(_subAudioConfig.Toggles[i]);
                }

                _mainAudioConfig.SubToggle = 0x0002;
                _mainAudioConfig.SubEntry = &_subAudioConfig;

                _mainAudioIntro.SubToggle = 0x0002;
                _mainAudioIntro.SubEntry = &_subAudioIntro;
            }

            if (_mainAudioConfig.Count > 0x01)
            {
                _mainAudioIntro.Count = _mainAudioConfig.Count;

                for (int i = 0; i < _mainAudioConfig.Count; i++)
                {
                    _mainAudioIntro.Buttons.push_back(_mainAudioConfig.Buttons[i]);
                    _mainAudioIntro.Descriptions.push_back(_mainAudioConfig.Descriptions[i]);
                    _mainAudioIntro.Toggles.push_back(_mainAudioConfig.Toggles[i]);
                }

                Tz::HookIntro::Add(UINT32_MAX, _mainAudioIntro);
                Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _mainAudioConfig);
            }

            // This code block handles RESOURCE packs.

            if (YS::MESSAGE::GetData(0x573C) != _fetchFake)
            {
                _resourceConfig.Count += 1;

                _resourceConfig.Buttons.push_back(0x573C);
                _resourceConfig.Descriptions.push_back(0x573D);

                _resourceConfig.Toggles.push_back(0x0200);
            }

            if (YS::MESSAGE::GetData(0x573E) != _fetchFake)
            {
                _resourceConfig.Count += 1;

                _resourceConfig.Buttons.push_back(0x573E);
                _resourceConfig.Descriptions.push_back(0x573F);

                _resourceConfig.Toggles.push_back(0x0400);
            }

           
            if (_resourceConfig.Count > 1)
            {
                _resourceIntro.Count = _resourceConfig.Count;

                for (int i = 0; i < _resourceConfig.Count; i++)
                {
                    _resourceIntro.Buttons.push_back(_resourceConfig.Buttons[i]);
                    _resourceIntro.Descriptions.push_back(_resourceConfig.Descriptions[i]);
                    _resourceIntro.Toggles.push_back(_resourceConfig.Toggles[i]);
                }

                Tz::HookIntro::Add(UINT32_MAX, _resourceIntro);
                Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _resourceConfig);
            }

            // This code block handles MUSIC packs.
            
            if (YS::MESSAGE::GetData(0x571B) != _fetchFake)
            {
                _musicConfig.Count += 1;

                _musicConfig.Buttons.push_back(0x571B);
                _musicConfig.Descriptions.push_back(0x571C);

                _musicConfig.Toggles.push_back(0x0080);
            }

            if (YS::MESSAGE::GetData(0x571D) != _fetchFake)
            {
                _musicConfig.Count += 1;

                _musicConfig.Buttons.push_back(0x571D);
                _musicConfig.Descriptions.push_back(0x571E);

                _musicConfig.Toggles.push_back(0x0100);
            }

            if (_musicConfig.Count > 1)
            {
                _musicIntro.Count = _musicConfig.Count;

                for (int i = 0; i < _musicConfig.Count; i++)
                {
                    _musicIntro.Buttons.push_back(_musicConfig.Buttons[i]);
                    _musicIntro.Descriptions.push_back(_musicConfig.Descriptions[i]);
                    _musicIntro.Toggles.push_back(_musicConfig.Toggles[i]);
                }

                Tz::HookIntro::Add(UINT32_MAX, _musicIntro);
                Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _musicConfig);
            }

            // Re:Fined Module Initialization, brought to you by Topaz' Reality (Patent Pending!)

            WIN32_FIND_DATAW _foundFile;
            wchar_t _modulePath[MAX_PATH];

            wcscpy(_modulePath, MOD_PATH);
            wcscat(_modulePath, L"\\dll\\modules\\ModuleRF-*.dll");

            auto _foundFileHandle = FindFirstFileW(_modulePath, &_foundFile);

            if (_foundFileHandle != INVALID_HANDLE_VALUE)
            {
                do
                {
                    wchar_t _foundFilePath[MAX_PATH];

                    wcscpy(_foundFilePath, MOD_PATH);
                    wcscat(_foundFilePath, L"\\dll\\modules\\");
                    wcscat(_foundFilePath, _foundFile.cFileName);

                    auto _moduleHandle = LoadLibraryW(_foundFilePath);

                    if (_moduleHandle)
                    {
                        uint8_t _importance = 0x00;

                        void (*funcExec)() = (void(*)())GetProcAddress(_moduleHandle, "RF_ModuleExecute");
                        void (*funcInit)(const wchar_t*) = (void(*)(const wchar_t*))GetProcAddress(_moduleHandle, "RF_ModuleInit");

                        uint8_t* _moduleImportance = (uint8_t*)GetProcAddress(_moduleHandle, "RF_ImportanceLevel");

                        bool (*_moduleEnsure)(const wchar_t*) = (bool(*)(const wchar_t*))GetProcAddress(_moduleHandle, "RF_EnsurePrerequisites");

                        if (_moduleEnsure)
                        {
                            auto _checkEnsure = _moduleEnsure(MOD_PATH);

                            if (!_checkEnsure)
                                continue;
                        }

                        if (_moduleImportance)
                            _importance = *_moduleImportance;

                        char* (*_excludeFunctions)() = (char*(*)())GetProcAddress(_moduleHandle, "RF_ExcludeFunctions");

                        if (_excludeFunctions)
                        {
                            auto _fetchFunctions = string(_excludeFunctions());

                            vector<string> _funcNames;

                            size_t _currPos = 0;
                            string _currFunc;

                            while ((_currPos = _fetchFunctions.find('|')) != std::string::npos) {
                                _currFunc = _fetchFunctions.substr(0, _currPos);
                                _funcNames.push_back(_currFunc);
                                _fetchFunctions.erase(0, _currPos + 1);
                            }

                            _funcNames.push_back(_fetchFunctions);

                            for (auto _fetchName : _funcNames)
                                FUNCTION_ARRAY.erase(_fetchName);
                        }

                        uint32_t* (*_fetchIntro)() = reinterpret_cast<uint32_t*(*)()>(GetProcAddress(_moduleHandle, "RF_CheckIntro"));
                        uint16_t* (*_fetchConfig)() = reinterpret_cast<uint16_t*(*)()>(GetProcAddress(_moduleHandle, "RF_CheckConfig"));

                        if (_fetchIntro)
                        {
                            Tz::HookIntro::Entry _constEntry;
                            auto _introPtr = _fetchIntro();

                            _constEntry.Count = *_introPtr;

                            _constEntry.Title = *(_introPtr + 0x01);
                            _constEntry.Flair = *(_introPtr + 0x02);

                            for (int i = 0; i < *_introPtr; i++)
                            {
                                _constEntry.Buttons.push_back(*(_introPtr + 0x03 + i));
                                _constEntry.Descriptions.push_back(*(_introPtr + 0x03 + *_introPtr + i));
                                _constEntry.Toggles.push_back(*(_introPtr + 0x03 + (*_introPtr * 0x02) + i));
                            }

                            _constEntry.SubToggle = *(_introPtr + 0x03 + (*_introPtr * 0x03));
                            _constEntry.SubEntry = *reinterpret_cast<Tz::HookIntro::Entry**>(_introPtr + 0x04 + (*_introPtr * 0x03));

                            Tz::HookIntro::Add(UINT32_MAX, _constEntry);

                            uint32_t** introSeek = (uint32_t**)GetProcAddress(_moduleHandle, "INTRO_SEEK");

                            assert(introSeek != nullptr);
                            *introSeek = reinterpret_cast<uint32_t*>(YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x400 + ((Tz::HookIntro::Entries.size() - 1) * 0x04));

                            Tz::HookIntro::IntroSeeks.insert(pair<uint32_t**, Tz::HookIntro::Entry>(introSeek, _constEntry));
                        }

                        if (_fetchConfig)
                        {
                            Tz::HookConfig::Entry _constEntry;
                            auto _configPtr = _fetchConfig();

                            _constEntry.Count = *_configPtr;
                            _constEntry.Title = *(_configPtr + 0x01);

                            for (int i = 0; i < *_configPtr; i++)
                            {
                                _constEntry.Buttons.push_back(*(_configPtr + 0x02 + i));
                                _constEntry.Descriptions.push_back(*(_configPtr + 0x02 + *_configPtr + i));
                                _constEntry.Toggles.push_back(*(_configPtr + 0x02 + (*_configPtr * 0x02) + i));
                            }

                            _constEntry.SubToggle = *(_configPtr + 0x02 + (*_configPtr * 0x03));
                            _constEntry.SubEntry = *reinterpret_cast<Tz::HookConfig::Entry**>(_configPtr + 0x03 + (*_configPtr * 0x03));

                            auto _seekConfig = Tz::HookConfig::Entries.size() - 1;

                            Tz::HookConfig::Add(UINT32_MAX, _constEntry);

                            uint16_t** configSeek = (uint16_t**)GetProcAddress(_moduleHandle, "CONFIG_SEEK");

                            assert(configSeek != nullptr);
                            {
                                uint16_t _checkBitwise = 0x00;

                                for (int i = 0; i < Tz::HookConfig::Entries.size(); i++)
                                {
                                    auto _fetchCount = Tz::HookConfig::Entries[i].Count;

                                    for (int z = 0; z < _fetchCount; z++)
                                    {
                                        auto _fetchBitwise = Tz::HookConfig::Entries[i].Toggles[z];

                                        if (_checkBitwise & _fetchBitwise && i == _seekConfig)
                                        {
                                            *configSeek = reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A6);
                                            goto CONFIG_LOOP_END;
                                        }

                                        else if (i == Tz::HookConfig::Entries.size() - 1)
                                        {
                                            *configSeek = reinterpret_cast<uint16_t*>(AREA::SaveData + 0x41A4);
                                            goto CONFIG_LOOP_END;
                                        }

                                        _checkBitwise |= _fetchBitwise;
                                    }
                                }
                            }
                        }

                    CONFIG_LOOP_END:

                        if (funcInit)
                            _initModule.insert(_initModule.end(), { _importance, funcInit });

                        if (funcExec)
                            _execModule.insert(_execModule.end(), { _importance, funcExec });
                    }

                } while (FindNextFileW(_foundFileHandle, &_foundFile));

                FindClose(_foundFileHandle);
            }

            for (auto _initPair : _initModule)
                _initPair.second(MOD_PATH);

            // This is an edge-case handler just in case someone don't wanna use shortcut sets.

            _modulePath[MAX_PATH];

            wcscpy(_modulePath, MOD_PATH);
            wcscat(_modulePath, L"\\dll\\modules\\ModuleRF-ShortcutSets.dll");

            _foundFileHandle = FindFirstFileW(_modulePath, &_foundFile);

            if (_foundFileHandle == INVALID_HANDLE_VALUE)
            {
                auto _currentTextPtr = YS::MESSAGE::GetData(0x051F);

                auto _soraText = YS::MESSAGE::GetData(0x572E);
                auto _soraSize = YS::MESSAGE::GetSize(_soraText);

                memcpy(const_cast<char*>(_currentTextPtr), _soraText, _soraSize + 0x01);
            }

            _modulePath[MAX_PATH];

            wcscpy(_modulePath, MOD_PATH);
            wcscat(_modulePath, L"\\dll\\modules\\ModuleRF-KeybladeSwitching.dll");

            _foundFileHandle = FindFirstFileW(_modulePath, &_foundFile);

            if (_foundFileHandle != INVALID_HANDLE_VALUE)
                CAN_PROCESS_FORM_KEYBLADES = true;

            #endif

            INITIALIZED = true;
        }
    
        else
        {
            #ifndef BUILD_ARCHIPELAGO_LITE
            Tz::HookIntro::Handle();
            Tz::HookConfig::Handle();
            #endif

            for (auto _funcRefined : FUNCTION_ARRAY)
                _funcRefined.second();

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            for (auto _execPair : _execModule)
                _execPair.second();
            #endif
            
            if (ALLOW_NOHUD || ALLOW_TIMESTOP)
            {
                if (!*YS::MENU::IsMenu && *YS::HARDPAD::Input & YS::HARDPAD::BUTTONS::L3 && !DEBOUNCE_HUDSTOP)
                {
                    auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
                    auto _fetchTimeStop = YS::PANACEA_ALLOC::Get("IS_TIMESTOP");

                    auto _isHudDraw = true;
                    auto _isTimeStop = false;

                    if (_fetchHudDraw)
                    {
                        if (ALLOW_NOHUD)
                        {
                            memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

                            _isHudDraw = !_isHudDraw; 
                            memcpy(_fetchHudDraw, &_isHudDraw, 0x01);
                        }

                        if (ALLOW_TIMESTOP)
                        {
                            memcpy(&_isTimeStop, _fetchTimeStop, 0x01);

                            _isTimeStop = !_isTimeStop;
                            memcpy(_fetchTimeStop, &_isTimeStop, 0x01);

                            *dk::Vsync::GameSpeed = _isTimeStop ? 0.0 : 1.0;
                        }

                        SOUND::PlaySFX(0x06);
                    }

                    DEBOUNCE_HUDSTOP = true;
                }

                else if ((*YS::HARDPAD::Input & YS::HARDPAD::BUTTONS::L3) == 0x00 && DEBOUNCE_HUDSTOP)
                    DEBOUNCE_HUDSTOP = false;
            }
        }
    }
}