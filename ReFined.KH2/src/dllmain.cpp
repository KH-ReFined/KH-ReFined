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
#include "cache_buff.h"
#include "cmconfig.h"
#include "command_draw.h"
#include "command_elem.h"
#include "hookconfig.h"
#include "converter.h"
#include "egs.h"
#include "exp.h"
#include "event.h"
#include "field2dd.h"
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
#include "member.h"
#include "member_table.h"
#include "menu.h"
#include "message.h"
#include "next_form.h"
#include "obj2d.h"
#include "objentry.h"
#include "panacea_alloc.h"
#include "party.h"
#include "pax.h"
#include "region.h"
#include "sequence.h"
#include "shake.h"
#include "softreset.h"
#include "sora.h"
#include "sound.h"
#include "sprite.h"
#include "steam.h"
#include "title.h"
#include "treasure_info.h"
#include "vsync.h"
#include "weapon.h"
#include "weapon_mset.h"
#include "weapon_entry.h"
#include "world.h"

#include "SigScan.h"
#include "continue_menu.h"

#include "ini.h"
#include <hookintro.h>
#include <field.h>
#include <sheet.h>
#include <fvector.h>

bool COLOR_SWAPPED = false;
bool IS_KEYBLADE_MENU = false;
bool KEYBLADE_DEBOUNCE = false;

int TARGET_KEY = 0x00;

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

// Redirected Constructors live here! //

char* MDLX_WRITE_BUFFER = ResolveRelativeAddress<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????", 0x0E);
char* APDX_WRITE_BUFFER = MDLX_WRITE_BUFFER + 0x28;
char* MSET_WRITE_BUFFER = APDX_WRITE_BUFFER + 0x28;

char* BGM_WRITE_BUFFER;

char* MENU_FNAME_BUFFER = ResolveRelativeAddress<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0D);
char* FAC_WRITE_BUFFER = ResolveRelativeAddress<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x", 0x1F9);

// 3D Path Constructors.

char* ConstructMDLX(char* objentryEntry, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.mdlx" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mdlx" : "obj/%s.mdlx");

    char* _mdlxName = objentryEntry + 0x08;
    char* _useBuff = !buff ? MDLX_WRITE_BUFFER : buff;

    sprintf(_useBuff, _constructPath.c_str(), _mdlxName);

    if (!YS::FILE::GetSize(_useBuff))
        sprintf(_useBuff, "obj/%s.mdlx", _mdlxName);

    return _useBuff;
}

char* ConstructAPDX(char* objentryEntry, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);

    char* _apdxName = objentryEntry + 0x08;
    char* _useBuff = !buff ? APDX_WRITE_BUFFER : buff;

    const char* _regionStr = !YS::REGION::Get() || YS::REGION::Get() == 0x07 ? "fm" : reinterpret_cast<char*>(*YS::REGION::pint_region);

    string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.a.%s" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.a.%s" : "obj/%s.a.%s");

    if ((*(objentryEntry + 0x48) & 0x01) != 0x00)
        return nullptr;

    sprintf(_useBuff, _constructPath.c_str(), _apdxName, _regionStr);

    if (!YS::FILE::GetSize(_useBuff))
        sprintf(_useBuff, _constructPath.c_str(), _apdxName, "us");

    if (!YS::FILE::GetSize(_useBuff))
    {
        sprintf(_useBuff, "obj/%s.a.%s", _apdxName, _regionStr);

        if (!YS::FILE::GetSize(_useBuff))
            sprintf(_useBuff, "obj/%s.a.us", _apdxName);
    }

    return _useBuff;

}

char* ConstructMSET(char* objentryEntry, uint32_t objectID, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);

    char* _mdlxName = objentryEntry + 0x08;
    char* _msetName = objentryEntry + 0x28;

    char* _useBuff = !buff ? MSET_WRITE_BUFFER : buff;

    string _fetchMSET = _fetchConfig & 0x0200 ? "obj_2nd/%s.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mset" : "obj/%s.mset");
    string _fetchMEMO = _fetchConfig & 0x0200 ? "obj_2nd/%s_MEMO.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s_MEMO.mset" : "obj/%s_MEMO.mset");

    if (!*_msetName)
    {
        if ((objectID & 0x10000000) == 0x00)
            return nullptr;

        sprintf(_useBuff, _fetchMEMO.c_str(), _mdlxName);

        if (!YS::FILE::GetSize(_useBuff))
            sprintf(_useBuff, "obj/%s_MEMO.mset", _mdlxName);

        return _useBuff;
    }

    auto _fetchNameMSET = string(_msetName);
    _fetchNameMSET.resize(_fetchNameMSET.size() - 0x05);

    sprintf(_useBuff, _fetchMSET.c_str(), _fetchNameMSET.c_str());

    if (!YS::FILE::GetSize(_useBuff))
        sprintf(_useBuff, "obj/%s.mset", _fetchNameMSET.c_str());

    if ((objectID & 0x10000000) != 0x00)
    {
    LABEL_12:
        sprintf(_useBuff, _fetchMEMO.c_str(), _fetchNameMSET.c_str());

        if (!YS::FILE::GetSize(_useBuff))
            sprintf(_useBuff, "obj/%s_MEMO.mset", _fetchNameMSET.c_str());

        return _useBuff;
    }

    if (objectID <= 0x31A)
    {
        if (objectID > 0x318)
            goto LABEL_10;

    LABEL_9:
        if ((objectID & 0x20000000) == 0x00)
            return _useBuff;

        goto LABEL_10;
    }

    if (objectID != 0x03EE)
        goto LABEL_9;

LABEL_10:
    if (*(objentryEntry + 0x04) != 0x00 && YS::CACHE_BUFF::GetStatus(_useBuff) < 2)
        goto LABEL_12;

    return _useBuff;
}

// Misc. Path Constructors. 

char* ConstructBGM(int number)
{
    auto _calcNumber = number;

    if (BGM_WRITE_BUFFER == nullptr)
        BGM_WRITE_BUFFER = (char*)malloc(0x28);

    if (YS::AREA::Current->World == 0x0B)
    {
        _calcNumber = 517;

        if (number != 117)
            _calcNumber = number;

        if (number == 121)
            _calcNumber = 521;
    }

    if (YS::REGION::Get() && YS::REGION::Get() != 0x07 && (_calcNumber <= 3 || _calcNumber == 113))
        _calcNumber += 400;

    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    auto _fetchMusic = (_fetchConfig & 0x0080) == 0x0080 ? 0x0080 : ((_fetchConfig & 0x0100) == 0x0100 ? 0x0100 : 0x0000);

    string _constructPath = _fetchConfig & 0x0080 ? "bgm_2nd/music%03d.win32.scd" : (_fetchConfig & 0x0100 ? "bgm_3rd/music%03d.win32.scd" : "bgm/music%03d.win32.scd");

    sprintf(BGM_WRITE_BUFFER, _constructPath.c_str(), _calcNumber);

    if (YS::FILE::GetSize(BGM_WRITE_BUFFER) == 0x00)
        sprintf(BGM_WRITE_BUFFER, "bgm/music%03d.win32.scd", _calcNumber);

    return BGM_WRITE_BUFFER;
}

char* ConstructMENU(char* buff, char* fileName)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    auto _regionPointer = (!YS::REGION::Get() || YS::REGION::Get() == 7) ? reinterpret_cast<char*>(*YS::REGION::pint_region_default) : reinterpret_cast<char*>(*YS::REGION::pint_region);

    if (!strcmp(*reinterpret_cast<char**>(MENU_FNAME_BUFFER), fileName))
    {
        string _constructFile = _fetchConfig & 0x0200 ? "file_2nd/%s/%s" : (_fetchConfig & 0x0400 ? "file_3rd/%s/%s" : "file/%s/%s");

        sprintf(buff, _constructFile.c_str(), _regionPointer, fileName);

        if (!YS::FILE::GetSize(buff))
        {
            _constructFile.resize(_constructFile.size() - 0x03);
            sprintf(buff, _constructFile.c_str(), fileName);

            if (!YS::FILE::GetSize(buff))
            {
                sprintf(buff, "file/%s/%s", _regionPointer, fileName);

                if (!YS::FILE::GetSize(buff))
                    sprintf(buff, "file/%s", fileName);
            }
        }

        return buff;
    }

    else
    {
        string _constructMenu = _fetchConfig & 0x0200 ? "menu_2nd/%s/%s" : (_fetchConfig & 0x0400 ? "menu_3rd/%s/%s" : "menu/%s/%s");

        auto _checkPhoto = strstr(fileName, "jm_photo/");
        sprintf(buff, _constructMenu.c_str(), _regionPointer, fileName);

        if (!YS::FILE::GetSize(buff))
        {
            if (_checkPhoto)
                goto SKIP_PHOTO;

            _constructMenu.resize(_constructMenu.size() - 0x03);
            sprintf(buff, _constructMenu.c_str(), fileName);

            if (!YS::FILE::GetSize(buff))
            {
            SKIP_PHOTO:
                sprintf(buff, "menu/%s/%s", _regionPointer, fileName);

                if (!YS::FILE::GetSize(buff) && !_checkPhoto)
                    sprintf(buff, "menu/%s", fileName);
            }
        }

        return buff;
    }
}

// 2D Path Constructors.

void ConstructFAC(uint16_t id)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    auto _writeBuff = *reinterpret_cast<char**>(FAC_WRITE_BUFFER);

    string _fetchFilePath(_writeBuff);

    _fetchFilePath = _fetchFilePath.substr(0x04, _fetchFilePath.size() - 0x04);
    _fetchFilePath = _fetchFilePath.substr(0x00, _fetchFilePath.size() - 0x04);

    string _constructPath = _fetchConfig & 0x0200 ? "menu_2nd" : (_fetchConfig & 0x0400 ? "menu_3rd" : "menu");

    _constructPath = _constructPath.append(_fetchFilePath);
    _constructPath = _constructPath.append("%d.imd");

    sprintf(_writeBuff, _constructPath.c_str(), id);

    if (!YS::FILE::GetSize(_writeBuff))
    {
        _constructPath = "menu";

        _constructPath = _constructPath.append(_fetchFilePath);
        _constructPath = _constructPath.append("%d.imd");

        sprintf(_writeBuff, _constructPath.c_str(), id);
    }
}

void ConstructITEMPIC(char* buff, uint16_t id)
{
    auto _fetchPicturePtr = *YS::ITEMPIC::ToLoadID;
    auto _fetchPictureID = 0x00;

    if (_fetchPicturePtr != nullptr)
        _fetchPictureID = *_fetchPicturePtr;

    else
        _fetchPictureID = id;

    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    string _constructPath = _fetchConfig & 0x0200 ? "itempic_2nd/item-%03d.imd" : (_fetchConfig & 0x0400 ? "itempic_3rd/item-%03d.imd" : "itempic/item-%03d.imd");

    sprintf(buff, _constructPath.c_str(), _fetchPictureID);

    if (!YS::FILE::GetSize(buff))
        sprintf(buff, "itempic/item-%03d.imd", _fetchPictureID);
}

// =================================== //

wchar_t* MOD_PATH;

multimap<uint8_t, void(*)(), std::greater<uint8_t>> _execModule;
multimap<uint8_t, void(*)(const wchar_t*), std::greater<uint8_t>> _initModule;

bool INITIALIZED = false;

bool IS_FASTBOOT = false;
bool IS_NOASPECT = false;

bool IS_STEAM = false;

bool IS_DEAD = false;
bool IS_RESETING = false;

uint16_t CURRENT_MUSIC = 0xFFFF;
uint16_t CURRENT_OBJECTS = 0xFFFF;

bool TRANSFER_FIELD = false;
bool TRANSFER_BATTLE = false;

char* FIELD_ALLOC = nullptr;
char* BATTLE_ALLOC = nullptr;

uint32_t PAST_LOCKON;
uint8_t LOCKON_TYPE = 0x00;

bool LOCKON_PLAY = false;

vector<uint8_t> LOCKON_VANILLA;
vector<uint8_t> LOCKON_EDITED;

char* LOCKON_FUNCTION = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8B", "xxxxxxxxxxxxxxxx");
char* LOCKON_FLOATS = SignatureScan<char*>("\x00\x00\x80\xBF\xF3\x04\xB5\xBF\x00\x00\x00\x00\x00\x00\xE0\xBF", "xxxxxxxxxxxxxxxx");

char* LOCKON_CHANGE = ResolveFunctionFromCall<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x55\x57\x41\x56\x48\x8B", "xxxxxxxxxxxxxxxx", 0x16A);
uint32_t* LOCKON_TARGET = ResolveRelativeAddress<uint32_t*>(LOCKON_CHANGE, 0x0B);

uint64_t PAST_EXP_FORM = 0;
uint64_t PAST_EXP_SUMM = 0;

vector<char> LIMITER_FUNCTION;
char* VSYNC_SETLIMIT_FUNCTION = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8D\x79\x01\x8B\xD9\x8B\xCF\xE8\x00\x00\x00\x00\xF3\x0F\x10\x0D\x00\x00\x00\x00\x33\xC0\x00\x00\x00\x00\x00\x00\x0F\x57\xC0\x89\x05\x00\x00\x00\x00\x0F\x57\xD2\x83\xFF\x08", "xxxxxxxxxxxxxxxxxx????xxxx????xx??????xxxxx????xxxxxx");

Core* Discord;

Activity RICH_PRESENCE;
Timestamp BEGIN_TIMESTAMP;

vector<string> TEXT_PRESENCE;
vector<string> TEXT_MODE;
vector<string> TEXT_FORM;

bool IS_MIRAGE;
bool IS_IN_FORM = false;
bool RPC_ENABLED = true;

uint8_t* COMMAND_TYPE = ResolveRelativeAddress<uint8_t*>("\x48\x83\xEC\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9\x00\x00\x00\x00\xCC\xCC\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxxx????x????xxx????xxx????xxx????xxxxx????xxxxx????xxx????x", 0x1A);

char* ADJUST_GLOW_FUNCTION = SignatureScan<char*>("\x4C\x8B\xDC\x49\x89\x5B\x20\x55\x56\x57\x41\x54\x41\x56\x49\x8D\xAB\x18\xF2\xFF\xFF\x48\x81\xEC\xC0\x0E\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
char* INIT_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x38\xE8\x00\x00\x00\x00\x48\xC7\x44\x24\x20\x00\x00\x00\x00\x0F\x10\x54\x24\x20\xF3\x0F\x10\x48\x10\xF3\x0F\x10\x40\x14\x0F\xC6\xD2\xD2\xF3\x0F\x10\xD1\x0F\xC6\xD2\x27\xF3\x0F\x10\xD0\x0F\xC6\xD2\x39\x0F\x11\x90\x5C\x01\x00\x00\x48\x83\xC4\x38\xC3", "xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* ADJUST_VIEWPORT_FUNCTION = SignatureScan<char*>("\x48\x83\xEC\x78\x0F\x29\x74\x24\x60\x0F\x28\xF1\x0F\x29\x7C\x24\x50\x0F\x28\xFA\x44\x0F\x29\x44\x24\x40\x44\x0F\x28\xC3\x44\x0F\x29\x4C\x24\x30\x44\x0F\x28\xC8\xE8\x00\x00\x00\x00\x45\x0F\xC6\xC9\xE1\xF3\x44\x0F\x10\xCE\x0F\x28\x74\x24\x60\x45\x0F\xC6\xC9\xC6\xF3\x44\x0F\x10\xCF\x0F\x28\x7C\x24\x50\x45\x0F\xC6\xC9\x27\xF3\x45\x0F\x10\xC8\x44\x0F\x28\x44\x24\x40\x45\x0F\xC6\xC9\x39\x44\x0F\x11\x88\x5C\x01\x00\x00\x44\x0F\x28\x4C\x24\x30\x48\x83\xC4\x78\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

bool SHAKE_WRITTEN;

vector<char> CALL_FUNC_MUNNY;

vector<char> ADJUST_GLOW_ARRAY;
vector<char> INIT_VIEWPORT_ARRAY;
vector<char> ADJUST_VIEWPORT_ARRAY;

char* VIEWPORT3D = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

char* PROMPT_INSTRUCTION = SignatureScan<char*>("\xC7\x05\x00\x00\x00\x00\x01\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00", "xx????xxxxx????xx????");
bool* PROMPT_MODE;

vector<uint32_t> CHECKSUM_TABLE;

bool IS_SHOWN = false;
bool SYSTEM_LOADED = false;
bool SAVE_INITIATE = false;
bool SYSTEM_WRITTEN = false;

bool ROUND_BACK = false;

YS::AREA::INFO SAVE_AREA;
int SAVE_ITERATOR = 0;

int SAVE_FRAME_ITERATOR = 0;

uint8_t SAVE_CHECK = 0xEB;
char* SAVE_OFFSET = SignatureScan<char*>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\xC8\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x3F\x48\x8B\xD9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????");

uint32_t MAGIC_FIRST;
uint16_t MAGIC_SECOND;

map<uint32_t, char*> MAGIC_FILES;

vector<uint16_t> ABILITY_ARRAY;

char* PICTURE_APPEAR_FUNC = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x48\x63\x41\x34\x48\x8B\xD9\x3B\x41\x30\x0F\x84\x00\x00\x00\x00\x48\x69\xD0\x60\x05\x00\x00\x48\x89\x7C\x24\x48", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx");
bool IS_PICTURE_EDITED = false;

uint32_t POSITIVE_ASPECT_OFFSET = 0x55;
uint32_t NEGATIVE_ASPECT_OFFSET = 0xFFFFFFAB;

vector<char*> POSITIVE_ASPECT_SHORT;
vector<char*> NEGATIVE_ASPECT_SHORT = MultiSignatureScan("\xC7\x00\x00\x00\xAB\xFF\xFF\xFF", "x???xxxx");
vector<char*> POSITIVE_ASPECT_LONG;
vector<char*> NEGATIVE_ASPECT_LONG = MultiSignatureScan("\xC7\x00\x00\x00\x00\x00\xAB\xFF\xFF\xFF", "x?????xxxx");

vector<char*> POSITIVE_ASPECT_BYTE;
vector<char*> NEGATIVE_ASPECT_BYTE;

char* VIEWPORT3D_ADDR = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);
char* INFORMATION_OFFSET = SignatureScan<char*>("\x41\xB8\x40\x00\x00\x00\xB9\xAA\x00\x00\x00\x66\x2B\xC1\x66\x44\x89\x44\x24\x20\x44\x0F\xB7\x43\x2C\x48\x8D\x8B\x60\x02\x00\x00\x44\x0F\xB7\xC8\x33\xD2", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

auto SIMPLE_COUNTER_FUNC = MultiSignatureScan("\x41\x8D\x46\xAB\x41\x89\x84\x3F\x8C\x0D\x00\x00", "xxxxxxxxxxxx");

char* CMENU_OFFSET = SignatureScan<char*>("\x48\x8B\xC4\x48\x81\xEC\x88\x00\x00\x00\x48\x89\x58\x18\xBA\x02\x00\x00\x00\x48\x89\x68\xF8\x48\x89\x70\xF0", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
char* CMENUINIT_OFFSET = SignatureScan<char*>("\x66\x44\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x44\x88\x35\x00\x00\x00\x00\x0F\x95\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\xC7\x44\x24\x30\x8C\x00\x00\x00", "xxxx????x????xxxxx????xxx????x????xxx????xxxxxxxx");

vector<uint8_t> INST_CAMPBITWISE;
vector<uint8_t> INST_CAMPINIT;

vector<uint8_t> INST_MAPJUMPTASK;
vector<uint8_t> INST_CONTINUELOAD;

vector<uint8_t> RETRY_STATE;

bool HADES_ESCAPE;
bool HADES_CHANGED;
uint8_t HADES_ITERATOR = 0xFF;

bool RETRY_BLACKLIST;
uint8_t RETRY_MODE;

ReFined::Continue::Entry RETRY_ENTRY(0x0002, 0x8AB1);
ReFined::Continue::Entry PREPARE_ENTRY(0x0002, 0x5727);

char* CURRENT_SUBMENU = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????x????", 0xF0);

// Configuration Values.

bool DISCORD_ENABLED = true;

uint8_t ROOM_AMOUNT = 3;
uint8_t SAVE_SLOT_OFFSET = 99;

uint16_t RESET_COMBO = YS::HARDPAD::BUTTONS::NONE;

// Function Block. Everything is here now :D

uint32_t trap_obj_get_entry_id(uint32_t* bdvalue)
{
    if (!bdvalue || (char*)bdvalue > moduleInfo.endAddr || *bdvalue == 0x00 || *bdvalue == UINT32_MAX)
        return 0x00;

    auto _fetchObjectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*bdvalue);

    if (!_fetchObjectPtr || (char*)_fetchObjectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchObjectActual = *reinterpret_cast<uint32_t*>(_fetchObjectPtr + 0x04);

    if (_fetchObjectActual == 0x00 || _fetchObjectActual == UINT32_MAX)
        return 0x00;

    auto _acutalObjectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchObjectActual);

    if (!_acutalObjectPtr || (char*)_acutalObjectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchEntryPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_acutalObjectPtr + 0x08));
    auto _fetchEntryId = *reinterpret_cast<uint32_t*>(_fetchEntryPtr);

    *bdvalue = _fetchEntryId;
    bdvalue[1] = 1414416704;

    return _fetchEntryId;
}

uint32_t trap_obj_effect_start_bind(uint32_t* bdvalue)
{
    auto _id = bdvalue[2];
    auto _flag = bdvalue[4];
    auto _priority = bdvalue[6];

    auto _fetchArg1 = *bdvalue;

    if (_fetchArg1 == 0x00 || _fetchArg1 == UINT32_MAX)
        return 0x00;

    auto _objectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchArg1);

    if (!_objectPtr || (char*)_objectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchArg2 = *reinterpret_cast<uint32_t*>(_objectPtr + 0x04);

    if (_fetchArg2 == 0x00 || _fetchArg2 == UINT32_MAX)
        return 0x00;

    auto _objectActual = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchArg2));

    if (!_objectActual || _objectActual > moduleInfo.endAddr)
        return 0x00;

    auto _fetchPAX = reinterpret_cast<char*>(_objectActual + 0x80);

    if (!_fetchPAX)
        return 0x00;

    auto _paxReturn = ryj::PAX::StartBind(_fetchPAX, _id, _flag, 0x01, _priority, _objectActual);
    auto _dwordReturn = PC::CONVERTER::LONG_TO_INT_ADDRESS(reinterpret_cast<uint64_t>(_paxReturn));

    *bdvalue = _dwordReturn;
    bdvalue[1] = 1380204864;

    return _dwordReturn;
}

uint32_t trap_obj_effect_start_bind_other(uint32_t* bdvalue)
{
    auto _id = bdvalue[2];
    auto _targetObj = bdvalue[4];
    auto _flag = bdvalue[6];
    auto _priority = bdvalue[8];

    if (*bdvalue == 0x00 || *bdvalue == UINT32_MAX)
        return 0x00;

    if (_targetObj == 0x00 || _targetObj == UINT32_MAX)
        return 0x00;

    auto _objectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*bdvalue);
    auto _targetObjectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(_targetObj);

    if (!_objectPtr || (char*)_objectPtr > moduleInfo.endAddr || !_targetObjectPtr || (char*)_targetObjectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchActual = *reinterpret_cast<uint32_t*>(_objectPtr + 0x04);
    auto _fetchTargetActual = *reinterpret_cast<uint32_t*>(_targetObjectPtr + 0x04);

    if (_fetchActual == 0x00 || _fetchActual == UINT32_MAX || _fetchTargetActual == 0x00 || _fetchTargetActual == UINT32_MAX)
        return 0x00;

    auto _objectActual = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchActual));
    auto _targetObjectActual = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchTargetActual));

    if (!_objectActual || _objectActual > moduleInfo.endAddr || !_targetObjectActual || _targetObjectActual > moduleInfo.endAddr)
        return 0x00;

    auto _fetchPAX = reinterpret_cast<char*>(_objectActual + 0x80);

    if (!_fetchPAX)
        return 0x00;

    auto _paxReturn = ryj::PAX::StartBind(_fetchPAX, _id, _flag, 0x01, _priority, _targetObjectActual);
    auto _dwordReturn = PC::CONVERTER::LONG_TO_INT_ADDRESS(reinterpret_cast<uint64_t>(_paxReturn));

    *bdvalue = _dwordReturn;
    bdvalue[1] = 1380204864;

    return _dwordReturn;
}

void SOFT_RESET()
{
    auto _fetchButtons = *YS::HARDPAD::Input;
    auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

    bool _canReset = _commandPointer != 0x00 && *YS::AREA::IsInMap && !*YS::TITLE::IsTitle && !*YS::MENU::IsMenu && RESET_COMBO != 0x00;

    // If the buttons are pushed, a reset can happen and it isn't happening:
    if (RESET_COMBO != YS::HARDPAD::BUTTONS::NONE && _fetchButtons == RESET_COMBO && _canReset && !IS_RESETING)
    {
        // Declare the reset is happening for timing purposes.
        IS_RESETING = true;

        // Initiate the fadeout for BGMs.
        YS::SOUND::BGMFadeOut(200, 0x00);
        YS::SOUND::BGMFadeOut(200, 0x01);

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
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    auto _fetchMusic = (_fetchConfig & 0x0080) == 0x0080 ? 0x0080 : ((_fetchConfig & 0x0100) == 0x0100 ? 0x0100 : 0x0000);

    if (*YS::TITLE::IsTitle)
        CURRENT_MUSIC = 0xFFFF;

    if (*YS::AREA::IsInMap)
    {
        if (CURRENT_MUSIC == 0xFFFF)
            CURRENT_MUSIC = _fetchMusic;

        else if (CURRENT_MUSIC != _fetchMusic)
        {
            if (_fetchMusic == 0x0080 && !YS::MESSAGE::GetData(0x571B))
            {
                _fetchMusic -= 0x0080;
                _fetchConfig -= 0x0080;

                *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;
            }

            if (_fetchMusic == 0x0100 && !YS::MESSAGE::GetData(0x571D))
            {
                _fetchMusic -= 0x0100;
                _fetchConfig -= 0x0100;

                *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;
            }

            string _fetchPath = _fetchMusic == 0x0080 ? "bgm_2nd/music%03d.win32.scd" : (_fetchMusic == 0x0100 ? "bgm_3rd/music%03d.win32.scd" : "bgm/music%03d.win32.scd");

            auto _fetchMode = *YS::AREA::BattleStatus == 0x00 ? 0x00 : 0x01;

            auto _fetchVolumeStart = *reinterpret_cast<uint32_t*>(YS::SOUND::CurrentMusic + 0x04);
            auto _fetchVolumeFinish = *reinterpret_cast<uint32_t*>(YS::SOUND::CurrentMusic + 0x08);

            auto _fetchCurrentField = *reinterpret_cast<uint16_t*>(YS::SOUND::CurrentMusic);
            auto _fetchCurrentBattle = *reinterpret_cast<uint16_t*>(YS::SOUND::CurrentMusic + 0x10);

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
                    YS::SOUND::KillBGM(0x00);

                YS::SOUND::SetTransfer(0x00, 0x02, FIELD_ALLOC, _sizeField, nullptr, nullptr);
                TRANSFER_FIELD = true;
            }

        FIELD_AFTERMATH:

            if (*YS::SOUND::IsTransferActive != 0x00)
                return;

            free(FIELD_ALLOC);
            TRANSFER_FIELD = false;

            if (_fetchMode == 0x00 && _fetchVolumeStart != 0x00)
                YS::SOUND::StartBGM(0x00, 0x3000, 0x3000, 0x00);

            BATTLE_ALLOC = (char*)malloc(_sizeBattle);

            if (BATTLE_ALLOC != nullptr)
            {
                auto _loadBattle = YS::FILE::Read(_battleMusicPath, BATTLE_ALLOC);

                if (_fetchMode == 0x01 && _fetchVolumeStart != 0x00)
                    YS::SOUND::KillBGM(0x01);

                YS::SOUND::SetTransfer(0x01, 0x02, BATTLE_ALLOC, _sizeBattle, nullptr, nullptr);
                TRANSFER_BATTLE = true;
            }

        BATTLE_AFTERMATH:

            if (*YS::SOUND::IsTransferActive != 0x00)
                return;

            free(BATTLE_ALLOC);
            TRANSFER_BATTLE = false;

            if (_fetchMode == 0x01 && _fetchVolumeStart != 0x00)
                YS::SOUND::StartBGM(0x01, 0x3000, 0x3000, 0x00);

            CURRENT_MUSIC = _fetchMusic;
        }
    }

    else if (!*YS::AREA::IsInMap && !*YS::TITLE::IsTitle && CURRENT_MUSIC != _fetchMusic)
    {
        if (CURRENT_MUSIC == 0xFFFF)
            CURRENT_MUSIC = _fetchMusic;

        else if (_fetchMusic == 0x0080 && !YS::MESSAGE::GetData(0x571B))
            *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;

        if (_fetchMusic == 0x0100 && !YS::MESSAGE::GetData(0x571D))
            *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;

        CURRENT_MUSIC = _fetchMusic;
    }
}

void HANDLE_RESOURCE()
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    auto _fetchObject = (_fetchConfig & 0x0200) == 0x0200 ? 0x0200 : ((_fetchConfig & 0x0400) == 0x0400 ? 0x0400 : 0x0000);

    if (!*YS::AREA::IsInMap)
    {
        if (!*YS::TITLE::IsTitle)
        {
            if (CURRENT_OBJECTS == 0xFFFF)
                CURRENT_OBJECTS = _fetchObject;

            if (_fetchObject == 0x0200 && !YS::MESSAGE::GetData(0x573C))
                *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;

            else if (_fetchObject == 0x0400 && !YS::MESSAGE::GetData(0x573E))
                *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;
        }

        else
            CURRENT_OBJECTS = 0xFFFF;
    }

    if (*YS::AREA::IsInMap)
    {
        if (CURRENT_OBJECTS == 0xFFFF)
            CURRENT_OBJECTS = _fetchObject;

        if (_fetchObject == 0x0200 && !YS::MESSAGE::GetData(0x573C))
            *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0080;

        else if (_fetchObject == 0x0400 && !YS::MESSAGE::GetData(0x573E))
            *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6) -= 0x0100;

        else if (CURRENT_OBJECTS != _fetchObject && !*YS::MENU::IsMenu)
        {
            YS::AREA::MapJump(YS::AREA::Current, 0x01, 0x00, false);
            CURRENT_OBJECTS = _fetchObject;
        }
    }
}

void ENFORCE_LOCKON()
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A4);
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
        YS::SOUND::PlaySFX(7);
        LOCKON_PLAY = true;
    }

    else if (LOCKON_PLAY)
        LOCKON_PLAY = false;

    PAST_LOCKON = _fetchTarget;
}

void DISPLAY_NEXT_EXP()
{
    auto _maxSummLevel = 0x02 + YS::ITEM::GetNumBackyard(0x0019) + YS::ITEM::GetNumBackyard(0x017F) + YS::ITEM::GetNumBackyard(0x009F) + YS::ITEM::GetNumBackyard(0x00A0);
    auto _maxFormLevel = 0x02 + YS::ITEM::GetNumBackyard(0x001A) + YS::ITEM::GetNumBackyard(0x001B) + YS::ITEM::GetNumBackyard(0x001D) + YS::ITEM::GetNumBackyard(0x001F) + YS::ITEM::GetNumBackyard(0x0233);

    if (_maxSummLevel == 0x06)
        _maxSummLevel = 0x07;

    if (*YS::AREA::IsInMap && !*YS::TITLE::IsTitle)
    {
        if (*(YS::AREA::SaveData + 0x3524) != 0x00)
        {
            uint8_t _currLevel = *(YS::AREA::SaveData + 0x32F4 + (0x38 * (*(YS::AREA::SaveData + 0x3524) - 1)) + 0x02);

            if (_maxFormLevel == _currLevel && PAST_EXP_FORM != 0x00)
            {
                dk::NEXT_FORM::create(0x00, NEGATIVE_ASPECT_OFFSET);
                PAST_EXP_FORM = 0x00;
            }

            uint32_t _currExp = *reinterpret_cast<uint32_t*>(YS::AREA::SaveData + 0x32F4 + (0x38 * (*(YS::AREA::SaveData + 0x3524) - 1)) + 0x04);
            uint32_t _expFetch = *reinterpret_cast<uint32_t*>(YS::FORM_LEVEL::Search(*(YS::AREA::SaveData + 0x3524), _currLevel) + 0x04);

            if (PAST_EXP_FORM == 0x00)
                PAST_EXP_FORM = _currExp;

            else if (PAST_EXP_FORM != _currExp)
            {
                dk::NEXT_FORM::create(_expFetch - _currExp, NEGATIVE_ASPECT_OFFSET);
                PAST_EXP_FORM = _currExp;
            }
        }

        else if (*(YS::AREA::SaveData + 0x3525) != 0x00)
        {
            uint8_t _currLevel = *(YS::AREA::SaveData + 0x3526);

            if (_maxSummLevel == _currLevel && PAST_EXP_SUMM != 0x00)
            {
                dk::NEXT_FORM::create(0x00, NEGATIVE_ASPECT_OFFSET);
                PAST_EXP_SUMM = 0x00;
            }

            uint32_t _currExp = *reinterpret_cast<uint32_t*>(YS::AREA::SaveData + 0x36E4);
            uint32_t _expFetch = *reinterpret_cast<uint32_t*>(YS::FORM_LEVEL::GetSummonTable() + 0x04);

            if (PAST_EXP_SUMM == 0x00)
                PAST_EXP_SUMM = _currExp;

            else if (PAST_EXP_SUMM != _currExp)
            {
                dk::NEXT_FORM::create(_expFetch - _currExp, NEGATIVE_ASPECT_OFFSET);
                PAST_EXP_SUMM = _currExp;
            }
        }

        else
        {
            PAST_EXP_FORM = 0x00;
            PAST_EXP_SUMM = 0x00;
        }
    }

    else if (*(YS::AREA::IsInMap) == 0 || *(YS::TITLE::IsTitle) == 1)
    { 
        PAST_EXP_FORM = 0x00;
        PAST_EXP_SUMM = 0x00;
    }
}

void HANDLE_GOA_LAND()
{
    auto _gardenKnown = (*(YS::AREA::SaveData + 0x231B) & 0x04) == 0x04;
    auto _canLandGarden = (*(YS::AREA::SaveData + 0x1EF6) & 0x40) == 0x40;

    if (_gardenKnown && !_canLandGarden)
        *(YS::AREA::SaveData + 0x1EF6) += 0x40;
}

void ENFORCE_FRAMERATE()
{
    auto _limiterOffset = VSYNC_SETLIMIT_FUNCTION + 0x20;

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
        *(dk::VSYNC::IsFrameLimited) = 0x01;
    }

    else if (_fetchFramerate != 0x00 && _fetchFunction != 0x90)
    {
        char* _nopArray = new char[0x06];
        fill(_nopArray, _nopArray + 0x06, 0x90);

        memcpy(_limiterOffset, _nopArray, 0x06);
        *(dk::VSYNC::IsFrameLimited) = 0x00;
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
            RPC_ENABLED = false;
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

    if (*YS::TITLE::IsTitle)
    {
        RICH_PRESENCE.GetAssets().SetLargeImage("title");

        RICH_PRESENCE.SetState("");
        RICH_PRESENCE.SetDetails("");

        RICH_PRESENCE.GetAssets().SetLargeText("");
        RICH_PRESENCE.GetAssets().SetSmallText("");

        RICH_PRESENCE.GetAssets().SetSmallImage("");
    }

    else if (YS::AREA::Current->World >= 0x02 && YS::AREA::Current->World <= 0x12)
    {
        bool _checkUnderdrome = YS::AREA::Current->World == 0x06 && YS::AREA::Current->Room == 0x09 && YS::AREA::Current->Set.Map >= 0xBD && YS::AREA::Current->Set.Map >= 0xC4;

        auto _detailText = _checkUnderdrome ? TEXT_PRESENCE.at(0x02) : TEXT_PRESENCE.at(0x00);

        _detailText.replace(_detailText.find("[0]"), 0x03, to_string(*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308)));
        _detailText.replace(_detailText.find("[1]"), 0x03, *(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x180) > 0x00 ? to_string(*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308 + 0x180)) : TEXT_PRESENCE.at(0x04));

        if (_checkUnderdrome)
            _detailText.replace(_detailText.find("[2]"), 0x03, to_string(YS::AREA::Current->Entrance));

        RICH_PRESENCE.SetDetails(_detailText.c_str());

        auto _stateText = TEXT_PRESENCE.at(0x01);

        _stateText.replace(_stateText.find("[0]"), 0x03, to_string(*(YS::AREA::SaveData + 0x24FF)));
        _stateText.replace(_stateText.find("[1]"), 0x03, *(YS::AREA::SaveData + 0x3524) == 0x00 ? "N/A" : (*COMMAND_TYPE == 0x01 ? "Mickey" : TEXT_FORM.at(*(YS::AREA::SaveData + 0x3524) - 0x01)));

        RICH_PRESENCE.SetState(_stateText.c_str());

        auto _fetchTime = floorf(*reinterpret_cast<const uint32_t*>(YS::AREA::SaveData + 0x2444) / 60.0F);

        auto _playHours = floorf(_fetchTime / 3600.0F);
        auto _playMinutes = floorf(fmodf(_fetchTime, 3600.0F) / 60.0F);

        ostringstream _timeStream;
        auto _timeText = TEXT_PRESENCE.at(0x03);

        _timeStream << std::setw(2) << std::setfill('0') << _playHours << ":"
            << std::setw(2) << std::setfill('0') << _playMinutes;

        _timeText.replace(_timeText.find("[0]"), 0x03, _timeStream.str());

        RICH_PRESENCE.GetAssets().SetLargeText(_timeText.c_str());
        RICH_PRESENCE.GetAssets().SetSmallText(TEXT_MODE.at(*(YS::AREA::SaveData + 0x2498)).c_str());

        RICH_PRESENCE.GetAssets().SetSmallImage(*YS::AREA::BattleStatus == 0x00 ? "safe" : (*YS::AREA::BattleStatus == 0x01 ? "mob" : "boss"));
        RICH_PRESENCE.GetAssets().SetLargeImage(IS_MIRAGE && YS::AREA::Current->World == 0x0B ? "ma" : string(YS::WORLD::GetName(YS::AREA::Current->World), 0x02).c_str());
    }

    Discord->ActivityManager().UpdateActivity(RICH_PRESENCE, [&_resultant](discord::Result v) { _resultant = (int)v; });
    Discord->RunCallbacks();
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
        _writeFloat[0x02] = *(reinterpret_cast<int*>(VIEWPORT3D + 0x08)) + *ryj::SHAKE::ShakeCoords;
        _writeFloat[0x03] = *(reinterpret_cast<int*>(VIEWPORT3D + 0x0C)) + *(ryj::SHAKE::ShakeCoords + 0x02);

        memcpy(VIEWPORT3D + 0x15C, _writeFloat, 0x10);
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
    bool _fetchConfig = *reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x2000;

    if (_isEnforced != _fetchConfig)
    {
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

    auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);
    const char* _gaugeTypePointer = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88 });
    const char* _mainPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);

    uint64_t _savePointer = IS_STEAM ? PC::STEAM::pint_saveinformation : PC::EGS::pint_saveinformation;

    if (!SYSTEM_WRITTEN)
    {
        char* _systemInfo = const_cast<char*>(CalculatePointer(_savePointer, { 0x10, 0x10 }));

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

    if (*YS::AREA::IsInMap && !*YS::TITLE::IsTitle && !SYSTEM_LOADED)
        SYSTEM_LOADED = true;

    else if (*YS::TITLE::IsTitle && SYSTEM_LOADED)
    {
        SAVE_ITERATOR = 0;
        SYSTEM_LOADED = false;
        SAVE_AREA = *YS::AREA::Current;
    }

    if (_commandPointer != 0x00 && _gaugeTypePointer != 0x00)
    {
        bool _checkBlacklist = YS::AREA::Current->World == 0x0F || YS::AREA::Current->World == 0x0B ||
            (YS::AREA::Current->World == 0x08 && YS::AREA::Current->Room == 0x03) ||
            (YS::AREA::Current->World == 0x0C && YS::AREA::Current->Room == 0x02) ||
            (YS::AREA::Current->World == 0x02 && YS::AREA::Current->Room <= 0x01) ||
            (YS::AREA::Current->World == 0x04 && YS::AREA::Current->Room == 0x10) ||
            (YS::AREA::Current->World == 0x12 && YS::AREA::Current->Room >= 0x13 && YS::AREA::Current->Room <= 0x1D);

        if (!*YS::TITLE::IsTitle && *YS::AREA::IsInMap && !_checkBlacklist)
        {
            if (SAVE_AREA.World == 0x00)
                SAVE_AREA = *YS::AREA::Current;

            bool _isAutosave = (*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x0002) || (*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x0004);
            bool _checkStatus = !*YS::MENU::IsMenu && _commandPointer != 0x00 && *YS::AREA::IsInMap && _mainPointer == 0x00 && *YS::AREA::BattleStatus == 0x00 && SAVE_AREA.World >= 0x02 && SYSTEM_LOADED && _isAutosave && *(dk::JUMPEFFECT::FadeStatus + 0x108) == 0x00;

            if (!_checkStatus)
            {
                SAVE_INITIATE = false;
                return;
            }

            if (SAVE_AREA.World != YS::AREA::Current->World)
            {
                SAVE_INITIATE = true;
                SAVE_ITERATOR = 0;
            }

            if (SAVE_AREA.Room != YS::AREA::Current->Room)
            {
                SAVE_ITERATOR++;

                if (SAVE_ITERATOR == ROOM_AMOUNT)
                {
                    SAVE_INITIATE = true;
                    SAVE_ITERATOR = 0;
                }
            }

            SAVE_AREA = *YS::AREA::Current;
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

        char* _saveFilePath = const_cast<char*>(CalculatePointer(_savePointer, { 0x40 }));
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

        char* _saveInfoStartRAM = CalculatePointer(_savePointer, { 0x10, 0x168 });
        char* _saveDataStartRAM = CalculatePointer(_savePointer, { 0x10, 0x19630 });

        memcpy(YS::AREA::SaveData + 0x10, &_autoSaveTag, 0x04);

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
                    if (!ROUND_BACK)
                    {
                        _saveOffset = 99;
                        ROUND_BACK = true;
                    }

                    else
                    {
                        const char* _unableMessage = YS::MESSAGE::GetData(0x5703);
                        dk::INFORMATION::openInformationWindow(_unableMessage);
                        ROUND_BACK = false;
                        return;
                    }
                }

                _stringStream.str("");
                _stringStream << setw(2) << setfill('0') << (_saveOffset - 1);

                _saveName = "BISLPM-66675FM-" + _stringStream.str();
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

        memcpy(_magicData, YS::AREA::SaveData, 0x08);
        memcpy(_saveData, YS::AREA::SaveData + 0x0C, 0x10FB4);

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

        memcpy(YS::AREA::SaveData + 0x10, &_regularSaveTag, 0x04);

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

        if (*reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4) & 0x0004)
        {
            const char* _saveMessage = YS::MESSAGE::GetData(0x5702);
            dk::INFORMATION::openInformationWindow(_saveMessage);
        }

        SAVE_INITIATE = false;
    }
}

void FIX_SAVE_POINT()
{
    if (*reinterpret_cast<const uint16_t*>(YS::COMMAND_ELEM::ReactionID) == 0x0037 && *YS::AREA::IsInMap && !*YS::TITLE::IsTitle)
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

        if (SAVE_CHECK == 0x75)
            SAVE_FRAME_ITERATOR++;

        if (_healthChecks > 0x00 || _magicChecks > 0x00)
            SAVE_CHECK = 0x75;

        else if (SAVE_CHECK == 0x75 && SAVE_FRAME_ITERATOR >= 80)
        {
            SAVE_CHECK = 0xEB;
            SAVE_FRAME_ITERATOR = 0;
        }
    }

    else if (!*YS::AREA::IsInMap || *YS::TITLE::IsTitle)
        SAVE_CHECK = 0x75;

    memcpy(SAVE_OFFSET + 0x25B, &SAVE_CHECK, 0x01);
}

void REGISTER_MAGIC()
{
    // Fetch the current levels or magic.

    uint32_t _magicFirst = *reinterpret_cast<const uint32_t*>(YS::AREA::SaveData + 0x3594);
    uint16_t _magicSecond = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x35CF);

    // If the game is loaded, and if the Tier 1 Magic or the Tier 2 Magic levels do not match what is previously recorded.

    if (*YS::AREA::IsInMap && (_magicFirst != MAGIC_FIRST || _magicSecond != MAGIC_SECOND))
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
                if (YS::FILE::LoadBAR(_currentTable->Filename, _loadBAR) != 0x00)
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
    else if (!*YS::AREA::IsInMap)
    {
        for (auto _magicEntry : MAGIC_FILES)
            free(_magicEntry.second);

        MAGIC_FILES.clear();
    }
}

void REGISTER_ABILITY()
{
    // Fetch the presence of Sora's Gauge [Edge Case for 100 Acre Woods minigames.]
    auto _soraGauge = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

    // See if there is specifically a Cutscene playing.
    auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);

    auto _fetchEvent = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });
    bool _isCutscene = _fetchEvent != 0x00 && *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xCAFEEFAC &&
        *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xEFACCAFE;

    auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

    // If the game is loaded:
    if (*YS::AREA::IsInMap && _commandPointer != 0x00 && _soraGauge != 0x00 && !_isCutscene && _eventPointer == 0x00)
    {
        // If  the ability denotation is not initialized:
        if (ABILITY_ARRAY.size() == 0x00)
        {
            // Resize the denotation to be 0x60 elements.
            ABILITY_ARRAY.resize(0x60);

            // Denote all of Sora's current abilities and sort them.
            memcpy(ABILITY_ARRAY.data(), YS::AREA::SaveData + 0x2544, 0xC0);
            sort(ABILITY_ARRAY.begin(), ABILITY_ARRAY.end());
        }

        // Create the vectors for current ability calculations.
        vector<uint16_t> _abilityDiff;
        vector<uint16_t> _currentAbility(0x60);

        // Denote all of Sora's current abilities and sort them.
        memcpy(_currentAbility.data(), YS::AREA::SaveData + 0x2544, 0xC0);
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
    auto _soraGauge = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

    auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);
    auto _reactionCommand = *reinterpret_cast<const uint16_t*>(YS::COMMAND_ELEM::ReactionID);

    // See if there is specifically a Cutscene playing.
    auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);

    auto _fetchEvent = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });
    bool _isCutscene = _eventPointer != nullptr && _fetchEvent != 0x00 && *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xCAFEEFAC &&
                                                                          *reinterpret_cast<const uint32_t*>(_fetchEvent) != 0xEFACCAFE;

    // If the game is loaded, and there isn't a menu present, and it's not a cutscene:
    if (*YS::AREA::IsInMap && _commandPointer != 0x00 && !*YS::MENU::IsMenu && !_isCutscene && _soraGauge != 0x00 && _eventPointer == 0x00)
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
    auto _soraGauge = CalculatePointer(dk::GAUGE::pint_playergauge, { 0x88, 0x00 });

    // Fetch Sora's pointer as well as his UCM.
    auto _soraSelf = *reinterpret_cast<const uint64_t*>(YS::SORA::pint_sora);
    auto _fetchSora = *reinterpret_cast<const uint16_t*>(YS::MEMBER_TABLE::MemberTable);

    // If Sora's HP is 0, and he isn't Mermaid Sora, and his gauge is present, and he isn't dead:
    if (*(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308) == 0x00 && *YS::AREA::IsInMap && !*YS::MENU::IsMenu && (_fetchSora != 0x03BE && _fetchSora != 0x0656) && _soraGauge != 0x00 && !IS_DEAD)
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

        POSITIVE_ASPECT_OFFSET = ceilf(0.177F * (_widthCalc - 1440));
        NEGATIVE_ASPECT_OFFSET = POSITIVE_ASPECT_OFFSET * -1;

        _offsetInformation = 0xFFFFU - floorf((((_widthCalc - 1440) - 480) * 0.1780F));

        if (POSITIVE_ASPECT_BYTE.size() == 0x00)
        {
            for (int i = 0xB0; i < 0xC0; i++)
            {
                stringstream stream;
                stream << char(i);

                auto _listPos = MultiSignatureScan(stream.str().append("\x55\x00\x00\x00").c_str(), "xxxxx");

                for (auto _ptr : _listPos)
                {
                    if (_ptr > moduleInfo.startAddr + 0x1E0000)
                        continue;

                    POSITIVE_ASPECT_BYTE.push_back(_ptr);
                }
            }

            for (int i = 0xB0; i < 0xC0; i++)
            {
                stringstream stream;
                stream << char(i);

                auto _listNeg = MultiSignatureScan(stream.str().append("\xAB\xFF\xFF\xFF").c_str(), "xxxxx");

                for (auto _ptr : _listNeg)
                {
                    if (_ptr > moduleInfo.startAddr + 0x1E0000)
                        continue;

                    NEGATIVE_ASPECT_BYTE.push_back(_ptr);
                }
            }
        }

        if (POSITIVE_ASPECT_SHORT.size() == 0x00)
        {
            auto _listPos = MultiSignatureScan("\xC7\x00\x00\x00\x55\x00\x00\x00", "x???xxxx");

            for (auto _ptr : _listPos)
            {
                if (_ptr > moduleInfo.startAddr + 0x1E0000)
                    continue;

                POSITIVE_ASPECT_SHORT.push_back(_ptr);
            }
        }

        if (POSITIVE_ASPECT_LONG.size() == 0x00)
        {
            auto _listPos = MultiSignatureScan("\xC7\x00\x00\x00\x00\x00\x55\x00\x00\x00", "x?????xxxx");

            for (auto _ptr : _listPos)
            {
                if (_ptr > moduleInfo.startAddr + 0x1E0000)
                    continue;

                POSITIVE_ASPECT_LONG.push_back(_ptr);
            }
        }

        for (int i = 0; i < POSITIVE_ASPECT_SHORT.size(); i++)
            memcpy(POSITIVE_ASPECT_SHORT[i] + 0x04, &POSITIVE_ASPECT_OFFSET, 0x04);
        for (int i = 0; i < NEGATIVE_ASPECT_SHORT.size(); i++)
            memcpy(NEGATIVE_ASPECT_SHORT[i] + 0x04, &NEGATIVE_ASPECT_OFFSET, 0x04);

        for (int i = 0; i < POSITIVE_ASPECT_LONG.size(); i++)
            memcpy(POSITIVE_ASPECT_LONG[i] + 0x06, &POSITIVE_ASPECT_OFFSET, 0x04);
        for (int i = 0; i < NEGATIVE_ASPECT_LONG.size(); i++)
            memcpy(NEGATIVE_ASPECT_LONG[i] + 0x06, &NEGATIVE_ASPECT_OFFSET, 0x04);

        for (int i = 0; i < POSITIVE_ASPECT_BYTE.size(); i++)
            memcpy(POSITIVE_ASPECT_BYTE[i] + 0x01, &POSITIVE_ASPECT_OFFSET, 0x04);
        for (int i = 0; i < NEGATIVE_ASPECT_BYTE.size(); i++)
            memcpy(NEGATIVE_ASPECT_BYTE[i] + 0x01, &NEGATIVE_ASPECT_OFFSET, 0x04);

        for (int i = 0; i < SIMPLE_COUNTER_FUNC.size(); i++)
            memcpy(SIMPLE_COUNTER_FUNC[i] + 0x03, &NEGATIVE_ASPECT_OFFSET, 0x04);

        memcpy(INFORMATION_OFFSET + 0x0B, &_offsetInformation, 0x02);

        auto _eventPointer = *reinterpret_cast<const char**>(YS::EVENT::pint_eventinfo);
        auto _commandPointer = *reinterpret_cast<const char**>(YS::COMMAND_DRAW::pint_commanddraw);

        if ((_commandPointer == 0x00 || _eventPointer != 0x00 || !*YS::AREA::IsInMap) && POSITIVE_ASPECT_OFFSET != 0x55)
        {
            vector<char*> _fetchMission =
            {
                YS::CACHE_BUFF::SearchByName("obj/B_EX150.mdlx", -1),
                YS::CACHE_BUFF::SearchByName("obj/B_EX150_LV99.mdlx", -1),
                YS::CACHE_BUFF::SearchByName("obj/N_CM020_BTL.mdlx", -1),
                YS::CACHE_BUFF::SearchByName("msn/us/EH26_MS108.bar", -1),
            };

            bool _isFetched = any_of(_fetchMission.begin(), _fetchMission.end(), [](char* x) {
                return (x != nullptr);
                });

            if (_isFetched)
            {
                uint32_t _positiveDefault = 0x55;
                uint32_t _negativeDefault = -0x55;

                memcpy(YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE") + 0x21, &_positiveDefault, 0x04);
                memcpy(YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE") + 0x29, &_negativeDefault, 0x04);
            }

            else
            {
                memcpy(YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE") + 0x21, &POSITIVE_ASPECT_OFFSET, 0x04);
                memcpy(YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE") + 0x29, &NEGATIVE_ASPECT_OFFSET, 0x04);
            }
        }
    }
}

void RETRY_BATTLES()
{
    // Declare the worlds and rooms in which Retry **cannot** execute.
    auto _checkBlacklist = (YS::AREA::Current->World == 0x04 && YS::AREA::Current->Room >= 0x15 && YS::AREA::Current->Room <= 0x1A) ||
        (YS::AREA::Current->World == 0x12 && ((YS::AREA::Current->Room >= 0x16 && YS::AREA::Current->Room <= 0x1C) || YS::AREA::Current->Room == 0x14));

    // If the arrays are not yet initialize, initialize them.
    if (INST_MAPJUMPTASK.size() == 0x00)
    {
        INST_MAPJUMPTASK.resize(0x05);
        INST_CONTINUELOAD.resize(0x05);

        INST_CAMPBITWISE.resize(0x07);
        INST_CAMPINIT.resize(0x08);

        memcpy(INST_MAPJUMPTASK.data(), reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, 0x05);
        memcpy(INST_CONTINUELOAD.data(), reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, 0x05);

        memcpy(INST_CAMPBITWISE.data(), reinterpret_cast<char*>(CMENU_OFFSET) + 0x1A7, 0x07);
        memcpy(INST_CAMPINIT.data(), reinterpret_cast<char*>(CMENUINIT_OFFSET), 0x08);
    }

    // If the room we are in is not blacklisted:
    if (!_checkBlacklist)
    {
        // Check if we are in a cutscene.
        const char* _eventPointer = CalculatePointer(YS::EVENT::pint_eventinfo, { 0x04 });
        bool _isCutscene = _eventPointer != 0x00 && *reinterpret_cast<const uint32_t*>(_eventPointer) != 0xCAFEEFAC && *reinterpret_cast<const uint32_t*>(_eventPointer) != 0xEFACCAFE;

        // If not in a cutscene, and is in a Boss Battle, and Retry State is not denoted:
        if (!_isCutscene && *YS::AREA::BattleStatus == 0x02 && RETRY_STATE.size() == 0x00)
        {
            // Check if on Hades Escape.
            HADES_ESCAPE = YS::AREA::Current->World == 0x06 && YS::AREA::Current->Room == 0x05 && YS::AREA::Current->Set.Event == 0x6F;

            // If on Hades Escape, denote and initialize it.
            if (HADES_ESCAPE && HADES_ITERATOR == 0xFF)
                HADES_ITERATOR = 0x00;

            // Read the current state of the room for Retry.
            RETRY_STATE.resize(0x10FC0);
            memcpy(RETRY_STATE.data(), YS::AREA::SaveData, 0x10FC0);

            // Add the entries for Retry as well as Prepare.
            ReFined::Continue::Add(0x00, PREPARE_ENTRY);
            ReFined::Continue::Add(0x00, RETRY_ENTRY);
        }

        // If not retrying, and is Hades Escape:
        if (RETRY_MODE == 0x00 && HADES_ESCAPE && HADES_ITERATOR != 0xFF)
        {
            // If Hades Escape hit an intermission, note it down.
            if (*YS::AREA::BattleStatus == 0x01 && !HADES_CHANGED)
            {
                HADES_ITERATOR++;
                HADES_CHANGED = true;
            }

            // If Hades Escape is in battle once again, note it down.
            if (*YS::AREA::BattleStatus != 0x01 && HADES_CHANGED)
                HADES_CHANGED = false;

            // If there have been 3 intermissions:
            if (HADES_ITERATOR == 0x03)
            {
                // Reset Hades Escape variables.
                HADES_ITERATOR = 0xFF;
                HADES_ESCAPE = false;

                // Reinstate Area Initializers to allow for game progression.
                memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
                memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);
            }
        }

        // If on title or the battle isn't a Boss Battle:
        else if (*YS::TITLE::IsTitle || *YS::AREA::BattleStatus != 0x02)
        {
            // If retrying, not on title, and Retry State has been noted:
            if (RETRY_MODE != 0x00 && RETRY_STATE.size() != 0x00 && !*YS::TITLE::IsTitle)
            {
                // Restore the Retry State.
                memcpy(const_cast<char*>(YS::AREA::SaveData), RETRY_STATE.data(), 0x10FC0);

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
                memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
                memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);

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

        // Fetch the current dialog state (Necessary for Game Over screen.)
        auto _fetchDialog = CalculatePointer(YS::MENU::pint_dialogbase, { 0xD48 });

        // Fetch Menu selection and the pointer to the Game Over screen.
        uint8_t _fetchSelectMenu = _fetchDialog != 0x00 ? *_fetchDialog : 0x80;
        uint64_t _continuePoint = *reinterpret_cast<const uint64_t*>(YS::MENU::pint_gameover);

        // If the Game Over menu exists and the Retry State has been noted:
        if (_continuePoint != 0x00 && RETRY_STATE.size() != 0x00)
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

                    memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, _nopArray, 0x05);
                    memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, _nopArray, 0x05);
                }

                // If we are not, restore the prior instructions.
                else
                {
                    memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
                    memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);
                }
            }
        }

        // If we are retrying, we are still in a boss battle, and the command menu type is "MICKEY"
        else if (RETRY_MODE > 0x00 && *YS::AREA::BattleStatus == 0x02 && *COMMAND_TYPE == 0x01)
        {
            // Restore the Area Init functions so we don't loop after Mickey leaves.
            memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1F2, INST_MAPJUMPTASK.data(), 0x05);
            memcpy(reinterpret_cast<char*>(YS::AREA::MapJump) + 0x1D8, INST_CONTINUELOAD.data(), 0x05);

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

extern "C"
{
    __declspec(dllexport) void OnInit(wchar_t* mod_path)
    {
        // Determine if the MOD is running on STEAM or EPIC.
        IS_STEAM = FindModule("steam_api64.dll");

        // Nullify all SaveID checks according to the platform in use.

        auto _saveCheckFunction = IS_STEAM ? SignatureScan<char*>("\x40\x55\x56\x57\x48\x81\xEC\xA0\x00\x00\x00\x48\xC7\x44\x24\x38\xFE\xFF\xFF\xFF\x48\x89\x9C\x24\xD0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x90\x00\x00\x00\x8B\xF1\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xED\x8D\x5D\x01\x48\x39\x2D\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x85\xC0\x74\x1D", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxx????xx????xxxxxxxx????xx????xxxxxx????xxxxxxxxxx")
            : SignatureScan<char*>("\x40\x57\x48\x83\xEC\x50\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x70\x48\x89\x74\x24\x78\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x8B\xF9\x89\x0D\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x33\xF6\x48\x39\x35\x00\x00\x00\x00\x0F\x85\x3D\x01\x00\x00\xB9\x78\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x38\x48\x85\xC0\x74\x1D\x45\x33\xC9\x44\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx????xx????xxxxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxxx");

        memcpy(_saveCheckFunction + (IS_STEAM ? 0x189 : 0x138), "\x90\x90\x90\x90\x90", 0x05);
        memcpy(_saveCheckFunction + (IS_STEAM ? 0x196 : 0x145), "\x90\x90", 0x02);

        memcpy(_saveCheckFunction + (IS_STEAM ? 0x1A1 : 0x150), "\xEB", 0x01);

        // Prevent SOFTRESET from resetting Fade status for a smooth-ass transition.

        memcpy(reinterpret_cast<char*>(dk::SOFTRESET::SoftResetThread) + 0x1ED, "\x90\x90\x90\x90\x90", 0x05);

        // I do not remember what this fucking does. But I believe it is important.

        auto _fetchAdjustment = SignatureScan<char*>("\x48\x83\xEC\x28\x0F\x10\x41\x48\x4C\x8B\xC9\x4C\x8B\xD2\xF3\x0F\x10\x25\x00\x00\x00\x00\x0F\x57\xED\x0F\x11\x02\x41\x0F\x10\x00\x49\x8B\x41\x40", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx");

        memcpy(_fetchAdjustment + 0xF6, "\x90\x90\x90\x90\x90\x90", 0x06);
        memcpy(_fetchAdjustment + 0x101, "\x90\x90\x90\x90\x90\x90", 0x06);

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

        auto _addrGetMDLX = reinterpret_cast<char*>(YS::OBJENTRY::get_mdlx);
        auto _addrGetAPDX = reinterpret_cast<char*>(YS::OBJENTRY::get_apdx);
        auto _addrGetMSET = reinterpret_cast<char*>(YS::OBJENTRY::get_mset);

        // Fixes a crash with switching weapons without a thread present.

        auto _fixWeaponHotswap = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x33\xC0\x0F\x1F\x44\x00\x00\x48\x85\xC0\x75\x09\x48\x8B\x05\x00\x00\x00\x00\xEB\x08\x8B\x48\x70\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x15\x48\x39\x58\x58\x75\xDF\xB9\xFF\xFF\x00\x00\x66\x01\x48\x02\x48\x83\xC4\x20\x5B\xC3\xB9\xFF\xFF\x00\x00\x66\x01\x48\x02\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxxxxxxxxx????xxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

        auto _jumpSpace = _fixWeaponHotswap + 0x294;
        auto _moduleStart = reinterpret_cast<uint64_t>(moduleInfo.startAddr) - reinterpret_cast<uint64_t>(_jumpSpace) - 0x07;

        vector<uint8_t> _instJumpWorkaround
        {
            0xE9, 0x59, 0x02, 0x00, 0x00,
            0x90, 0x90, 0x90, 0x90, 0x90,
            0xB9, 0xFF, 0xFF, 0x00, 0x00,
            0xE9, 0x4A, 0x02, 0x00, 0x00
        };

        memcpy(_fixWeaponHotswap + 0x36, _instJumpWorkaround.data(), 0x14);

        vector<uint8_t> _instHotpatchWeapon
        {
            0x4C, 0x8D, 0x15, 0x00, 0x00, 0x00, 0x00,
            0x4C, 0x39, 0xD0,
            0xEB, 0x03,
            0xC2, 0x00, 0x00,
            0x7C, 0x04,
            0x66, 0x01, 0x48, 0x02,
            0x4D, 0x31, 0xD2,
            0xEB, 0x8B
        };

        vector<uint8_t> _instHotpatchCont
        {
            0x48, 0x83, 0xC4, 0x20,
            0x5B, 0xC3
        };

        memcpy(_instHotpatchWeapon.data() + 0x03, &_moduleStart, 0x04);

        memcpy(_jumpSpace, _instHotpatchWeapon.data(), 0x1A);
        memcpy(_jumpSpace - 0x5B, _instHotpatchCont.data(), 0x06);

        // Fixes key-bound PAX effects crashing whilst switching weapons.

        char* _funcCheckWeapon = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x33\xDB\x48\x8B\xF2\x48\x8B\xF9\x48\x39\x99", "xxxxxxxxxxxxxxxxxxxxxxxxxx");

        fill(_funcCheckWeapon + 0x17, _funcCheckWeapon + 0x1D, 0x90);
        memcpy(_funcCheckWeapon + 0x17, "\xEB\xB8", 0x02);

        vector<uint8_t> _firstPatchPax
        {
            0x48, 0x85, 0xC9, // test rcx, rcx
            0x0F, 0x84, 0x95, 0x00, 0x00, 0x00, // je 0x95
            0xEB, 0x19 // jmp 0x19
        };

        memcpy(_funcCheckWeapon - 0x2F, _firstPatchPax.data(), 0x0B);

        vector<uint8_t> _secondPatchPax
        {
            0x48, 0x39, 0x99, 0xB8, 0x0A, 0x00, 0x00, // cmp [rcx + 0x0AB8], rbx
            0xEB, 0x20 // jmp 0x20
        };

        memcpy(_funcCheckWeapon - 0x0B, _secondPatchPax.data(), 0x09);

        // Redirect MDLX and APDX file construction (within YS::OBJENTRY::GetCacheBuffStatus) to [YS::OBJENTRY::get_mdlx] and [YS::OBJENTRY::get_apdx] instead.

        vector<uint8_t> _instructionREPLACE =
        {
            0x48, 0x8D, 0x0E,               // lea rcx, [rsi/r15]
            0x48, 0x31, 0xD2,               // xor rdx, rdx
            0xE8, 0x00, 0x00, 0x00, 0x00,   // call [someFunction]
            0x48, 0x31, 0xC0                // xor rax, rax
        };

        auto _getCacheBuffStatusMDLX = SignatureScan<char*>("\x4C\x8D\x46\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxx?x????");
        uint32_t _calculateGetMDLX = _addrGetMDLX - (_getCacheBuffStatusMDLX + 0x0B);
        
        fill(_getCacheBuffStatusMDLX, _getCacheBuffStatusMDLX + 0x56, 0x90);

        memcpy(_instructionREPLACE.data() + 0x07, &_calculateGetMDLX, 0x04);
        memcpy(_getCacheBuffStatusMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());

        auto _getCacheBuffStatusAPDX = _getCacheBuffStatusMDLX + 0xDA;
        uint32_t _calculateGetAPDX = _addrGetAPDX - (_getCacheBuffStatusAPDX + 0x0B);

        fill(_getCacheBuffStatusAPDX, _getCacheBuffStatusAPDX + 0x62, 0x90);

        memcpy(_instructionREPLACE.data() + 0x07, &_calculateGetAPDX, 0x04);
        memcpy(_getCacheBuffStatusAPDX, _instructionREPLACE.data(), _instructionREPLACE.size());

        // Redirect MDLX and APDX file construction (within YS::OBJENTRY::ReadRequestSub) to [YS::OBJENTRY::get_mdlx] and [YS::OBJENTRY::get_apdx] instead.

        auto _readRequestSubMDLX = SignatureScan<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxx?x????");
        _calculateGetMDLX = _addrGetMDLX - (_readRequestSubMDLX + 0x0B);

        _instructionREPLACE[0] += 0x01;
        _instructionREPLACE[2] += 0x01;

        fill(_readRequestSubMDLX, _readRequestSubMDLX + 0x56, 0x90);

        memcpy(_instructionREPLACE.data() + 0x07, &_calculateGetMDLX, 0x04);
        memcpy(_readRequestSubMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());
        
        auto _readRequestSubAPDX = _readRequestSubMDLX + 0xCA;      
        _calculateGetAPDX = _addrGetAPDX - (_readRequestSubAPDX + 0x0B);

        fill(_readRequestSubAPDX, _readRequestSubAPDX + 0x62, 0x90);

        memcpy(_instructionREPLACE.data() + 0x07, &_calculateGetAPDX, 0x04);
        memcpy(_readRequestSubAPDX, _instructionREPLACE.data(), _instructionREPLACE.size());

        vector<uint8_t> _absoluteInstructionJMP =
        {
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        auto _trapObjEffectStartBindOther = (uint64_t)trap_obj_effect_start_bind;
        auto _trapObjEffectStartBindOtherFunc = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x30\x8B\x59\x18\x4C\x8B\xF1\x8B\x79\x10\x8B\x71\x08\x8B\x09\xE8\x00\x00\x00\x00\x8B\x48\x04", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx");
        
        fill(_trapObjEffectStartBindOtherFunc, _trapObjEffectStartBindOtherFunc + 0x72, 0x90);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_trapObjEffectStartBindOther, 0x08);
        memcpy(_trapObjEffectStartBindOtherFunc, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

        auto _trapObjEffectStartBind = (uint64_t)trap_obj_effect_start_bind_other;
        auto _trapObjEffectStartBindFunc = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x30\x4C\x8B\xF1\x8B\x49\x10\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x41\x8B\x0E\x48\x8B\xD8\x41\x8B\x7E\x20\x41\x8B\x76\x18\x41\x8B\x6E\x08\xE8\x00\x00\x00\x00\x8B\x48\x04", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxx????xxx");

        fill(_trapObjEffectStartBindFunc, _trapObjEffectStartBindFunc + 0x8E, 0x90);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_trapObjEffectStartBind, 0x08);
        memcpy(_trapObjEffectStartBindFunc, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

        auto _fVectorAddr = (uint64_t)kn::FVector::Init;
        auto _fVectorInitialize = SignatureScan<char*>("\x48\x3B\xCA\x74\x16\x8B\x02\x89\x01", "xxxxxxxxx");

        fill(_fVectorInitialize, _fVectorInitialize + 0x1F, 0x90);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_fVectorAddr, 0x08);
        memcpy(_fVectorInitialize, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

        auto _trapObjGetEntryId = (uint64_t)trap_obj_get_entry_id;
        auto _trapObjGetEntryIdFunc = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x8B\x09\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00\x89\x03\xC7\x43\x04\x40\x49\x4E\x54\x48\x83\xC4\x20\x5B\xC3\xCC\x48", "xxxxxxxxxxxx????xxxx????xxxx????xxxxxxxxxxxxxxxxx");
        
        fill(_trapObjGetEntryIdFunc, _trapObjGetEntryIdFunc + 0x2F, 0x90);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_trapObjGetEntryId, 0x08);
        memcpy(_trapObjGetEntryIdFunc, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

        auto _hotpatchNullTask = SignatureScan<char*>("\x49\x8B\x40\x18\xC7\x40\x04\x40\x3F\x3F\x3F\x49\x8B\x48\x18\x48\x8D\x41\x08\x49\x89\x40\x18\x8B\x02\x89\x01\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx");

        memcpy(_hotpatchNullTask + 0x17, "\xEB\x22\x89\x01\xC3", 0x05);
        memcpy(_hotpatchNullTask + 0x3B, "\x83\xFA\x04\x74\xDB\x8B\x02\xEB\xD5", 0x09);

        // Redirect all [YS::OBJENTRY::get_mdlx], [YS::OBJENTRY::get_apdx], and [YS::OBJENTRY::get_mset] calls to Re:Fined code.

        fill(_addrGetMDLX, _addrGetMDLX + 0x71, 0x90);
        fill(_addrGetAPDX, _addrGetAPDX + 0x92, 0x90);
        fill(_addrGetMSET, _addrGetMSET + 0xE3, 0x90);

        for (int i = 0; i < 3; i++)
        {
            auto _constFunction = i == 1 ? (uint64_t)ConstructAPDX : (i == 2 ? (uint64_t)ConstructMSET : (uint64_t)ConstructMDLX);
            auto _constWrite = i == 1 ? _addrGetAPDX : (i == 2 ? _addrGetMSET : _addrGetMDLX);

            memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
            memcpy(_constWrite, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
        }

        // Redirect ITEMPIC and FAC file construction to Re:Fined code.

        auto _funcOpenEventBox = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx") + 0x6F;
        auto _funcReadThreadIMD = SignatureScan<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x") + 0x7D;
        auto _funcReadThreadFAC = SignatureScan<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x") + 0x231;

        auto _treasurePtr = ResolveRelativeAddress<char*>(_funcOpenEventBox, 0x7A);

        fill(_funcOpenEventBox, _funcOpenEventBox + 0x21, 0x90);
        fill(_funcReadThreadIMD, _funcReadThreadIMD + 0x14, 0x90);
        fill(_funcReadThreadFAC, _funcReadThreadFAC + 0x14, 0x90);

        auto _constFunction = reinterpret_cast<uint64_t>(ConstructITEMPIC);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
        memcpy(_addrGetMDLX + 0x10, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

        vector<uint8_t> _patchEventBox =
        {
            0xC7, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // mov [0x00], 0x00000001
            0x48, 0x8B, 0x53, 0x18,                                     // mov rdx, [rbx + 0x18]
            0x48, 0x8D, 0x4C, 0x24, 0x20,                               // lea rcx, [rsp + 0x20]
            0x48, 0x8B, 0xFA,                                           // mov rdi, rdx
            0xE8, 0x00, 0x00, 0x00, 0x00                                // call [someFunction]
        };

        uint32_t _calculateTrsr = _treasurePtr - (_funcOpenEventBox + 0x0A);
        uint32_t _calculateCall = (_addrGetMDLX + 0x10) - (_funcOpenEventBox + 0x1B);

        memcpy(_patchEventBox.data() + 0x17, &_calculateCall, 0x04);
        memcpy(_patchEventBox.data() + 0x02, &_calculateTrsr, 0x04);

        memcpy(_funcOpenEventBox, _patchEventBox.data(), _patchEventBox.size());

        vector<uint8_t> _patchThreadIMD =
        {
            0x48, 0x8D, 0x4C, 0x24, 0x30, // lea rcx, [rsp + 0x30]
            0xE8, 0x00, 0x00, 0x00, 0x00  // call [someFunction] 
        };

        _calculateCall = (_addrGetMDLX + 0x10) - (_funcReadThreadIMD + 0x0A);

        memcpy(_patchThreadIMD.data() + 0x06, &_calculateCall, 0x04);
        memcpy(_funcReadThreadIMD, _patchThreadIMD.data(), _patchThreadIMD.size());

        vector<uint8_t> _patchThreadFAC =
        {
            0x48, 0x8B, 0xCD,                                                                               // mov rcx, rbp
            0xFF, 0x15, 0x02, 0x00, 0x00, 0x00, 0xEB, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // call [someAbsoluteFunction]
        };

        _constFunction = reinterpret_cast<uint64_t>(ConstructFAC);

        memcpy(_patchThreadFAC.data() + 0x0B, &_constFunction, 0x08);
        memcpy(_funcReadThreadFAC, _patchThreadFAC.data(), _patchThreadFAC.size());

        // Redirect BGM file construction to Re:Fined code.

        auto _constrcutBGM = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x80\x3D\x00\x00\x00\x00\x0D\x8B\xD9\x75\x00\x83\xF9\x75\xBB\x05\x02\x00\x00\xB8\x09\x02\x00\x00\x0F\x45", "xxxxxxxx????xxxx?xxxxxxxxxxxxxxx");
        _constFunction = reinterpret_cast<uint64_t>(ConstructBGM);

        fill(_constrcutBGM, _constrcutBGM + 0x78, 0x90);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
        memcpy(_constrcutBGM, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
        
        // Redirect "file/%s/%s" and "menu/%s/%s" construction to Re:Fined code.

        auto _constructMENU = SignatureScan<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx");
        _constFunction = reinterpret_cast<uint64_t>(ConstructMENU);

        fill(_constructMENU, _constructMENU + 0x191, 0x90);

        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
        memcpy(_constructMENU, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

        // This code addresses an issue with PARTY_LIMIT crashing the game at the end of a fight.

        auto _funcLimitCheck = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x8B\x79\x10\x48\x8B\xF1\x0F\x29\x74\x24\x20\xF3\x0F\x10\x71\x18\x8B\x49\x08\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x8B\x0E\x48\x8B\xD8\xE8\x00\x00\x00\x00\x0F\x28\xDE\x44\x8B\xC7\x48\x8B\xD3\x48\x8B\xC8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxx????xxxxxxxxxxxx");
        auto _funcLimitPoint = ResolveRelativeAddress<uint64_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x30\x48\x8B\xFA\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\xE8\x0F\xB7\x08\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x85\xC0\x0F\x84\xCF\x00\x00\x00\xB9\x28\x01\x00\x00\xE8\x00\x00\x00\x00\x33\xDB\x48\x85\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxx", 0x6A);

        // This patch basically checks at any point this is called if the PARTY_LIMIT is null. If it is, it skips the whole function.
        vector<uint8_t> _patchLimitCheck
        {
            0x4C, 0x8B, 0x1D, 0x00, 0x00, 0x00, 0x00, // mov r11, [someAddress]
            0x4D, 0x85, 0xDB,                         // test r11, r11
            0x75, 0x0A,                               // jne 0x0A
            0xEB, 0x02,                               // jmp 0x02
            0xEB, 0xF0,                               // jmp 0xF0
            0x4D, 0x31, 0xDB,                         // xor r11,r11
            0xC3,                                     // ret
            0x90,                                     // nop
            0x90                                      // nop
        };

        auto _limitAddrCalc = _funcLimitPoint - reinterpret_cast<uint64_t>(_funcLimitCheck - 0x0E) - 0x07;
        memcpy(_patchLimitCheck.data() + 0x03, &_limitAddrCalc, 0x04);

        // In here we fetch the function that exists where we are going to place our patch so that we can move it.
        vector<uint64_t> _fetchFunction(0x68);
        memcpy(_fetchFunction.data(), _funcLimitCheck, 0x68);

        // We move the thing by 8 bytes/
        memcpy(_funcLimitCheck + 0x08, _fetchFunction.data(), 0x68);

        // Address correction for the 8 byte offset.

        auto _tempPtr = reinterpret_cast<uint32_t*>(_funcLimitCheck + 0x4E);
        *_tempPtr -= 0x08;

        _tempPtr = reinterpret_cast<uint32_t*>(_funcLimitCheck + 0x2B);
        *_tempPtr -= 0x08;
        _tempPtr = reinterpret_cast<uint32_t*>(_funcLimitCheck + 0x33);
        *_tempPtr -= 0x08;
        _tempPtr = reinterpret_cast<uint32_t*>(_funcLimitCheck + 0x3D);
        *_tempPtr -= 0x08;

        memcpy(_funcLimitCheck - 0x0E, _patchLimitCheck.data(), 0x16);

        // Initialization of all MENU handlers [INTRO, CONFIG, CONTINUE]

        YS::PANACEA_ALLOC::Allocate("CHANGE_WEAPON_QUEUE", 0x140);

        ReFined::Continue::Submit();

        // If NO_ASPECT is not called, handle all aspect modifications.

        if (!IS_NOASPECT)
        {
            // Create a memory space for the Gauge overrider. This replacement function is long, and replacing it for each function space requires exactly that. Space.
            // As such I just create an all-encompassing function in there, and redirect and and all functions that use this very specific maths to here.

            YS::PANACEA_ALLOC::Allocate("GAUGE_ASPECT_OVERRIDE", 0x100);

            // The function that will do the math.
            // P.S. - Please excuse me using literal byte arrays for this. I do not have nor want to use an assembler.

            vector<uint8_t> _gaugeFunc =
            {
                0x45, 0x89, 0xE2,				// mov r10d, r12d
                0x41, 0x83, 0xFA, 0x10,			// cmp r10d, 0x10
                0x72, 0x0C,						// jb 0x0C
                0x41, 0x89, 0xEA,				// mov r10d, ebp
                0x41, 0x83, 0xFA, 0x10,			// cmp r10d, 0x10
                0x72, 0x03,						// jb 0x03
                0x41, 0x89, 0xF2,				// mov r10d, esi
                0x45, 0x85, 0xD2,				// test r10d, r10d
                0x74, 0x0E,						// je 0x0E
                0x41, 0x83, 0xFA, 0x01,			// cmp r10d, 0x01
                0x75, 0x0D,						// jne 0x0D
                0x05, 0x55, 0x00, 0x00, 0x00,	// add eax, 0x00000055
                0x41, 0xFF, 0xE1,				// jmp r9
                0x05, 0xAB, 0xFF, 0xFF, 0xFF,   // add eax, 0xFFFFFFAB
                0x41, 0xFF, 0xE1				// jmp r9
            };

            memcpy(YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE"), _gaugeFunc.data(), _gaugeFunc.size());

            // Fetch all functions that will make use of the function above.

            vector<char*> _fetchFunctions =
            {
                SignatureScan<char*>("\x85\xF6\x74\x0A\x83\xFE\x01\x75\x08\x83\xC0\x55\xEB\x03\x83\xC0\xAB", "xxxxxxxxxxxxxxxxx"),
                SignatureScan<char*>("\x85\xED\x74\x0A\x83\xFD\x01\x75\x08\x83\xC0\x55\xEB\x03\x83\xC0\xAB", "xxxxxxxxxxxxxxxxx"),
                SignatureScan<char*>("\x45\x85\xE4\x74\x0B\x41\x83\xFC\x01\x75\x08\x83\xC0\x55\xEB\x03\x83\xC0\xAB", "xxxxxxxxxxxxxxxxxxx")
            };

            // Redirect all the parts that do the math individually to that big function block.

            for (int i = 0; i < 0x03; i++)
            {
                auto _funcLength = i == 0x02 ? 0x13 : 0x11;

                char* _nopFunction = new char[_funcLength];
                fill(_nopFunction, _nopFunction + _funcLength, 0x90);

                memcpy(_fetchFunctions[i], _nopFunction, _funcLength);

                vector<uint8_t> _redirectFunc =
                {
                    0x4C, 0x8B, 0x4C, 0x24, 0xF8,	// mov r9, [rsp-0x08]
                    0x49, 0x83, 0xC1, 0x0E,			// add r9, 0x0E
                    0xE9							// jmp
                };

                memcpy(_fetchFunctions[i], _redirectFunc.data(), 0x0A);

                uint32_t _funcDifference = YS::PANACEA_ALLOC::Get("GAUGE_ASPECT_OVERRIDE") - (_fetchFunctions[i] + 0x0E);
                memcpy(_fetchFunctions[i] + 0x0A, &_funcDifference, 0x04);
            }

            // Fetch the function that handles the fade-in and fade-out of the Information Text.

            auto _fetchInformation = SignatureScan<char*>("\x41\xB8\x40\x00\x00\x00\xB9\xAA\x00\x00\x00\x66\x2B\xC1\x66\x44\x89\x44\x24\x20\x44\x0F\xB7\x43\x2C\x48\x8D\x8B\x60\x02\x00\x00\x44\x0F\xB7\xC8\x33\xD2", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

            // This basically forces the in and out values of the text to be a specific value.
            // No, I do not know why this works. I did random shit and struck gold. No, I am not complaining. 

            vector<uint8_t> _informationFunc =
            {
                0x41, 0x89, 0xC1,		 // mov r9d, eax
                0x66, 0xBA, 0xFF, 0xFF   // mov dx, 0xFFFF
            };

            vector<uint8_t> _redirInformation =
            {
                0x41, 0xB9, 0xC0, 0x00, 0x00, 0x00 // mov r9d, 0xC0
            };

            memcpy(_fetchInformation + 0x06, "\x90\x90\x90\x90\x90\x90\x90\x90", 0x08);
            memcpy(_fetchInformation + 0x20, "\x90\x90\x90\x90\x90\x90", 0x06);

            memcpy(_fetchInformation + 0x06, _informationFunc.data(), 0x07);
            memcpy(_fetchInformation + 0x20, _redirInformation.data(), 0x06);

            // Fetch all functions that handle fade-in and fade-outs in any way within the 2dFade rectangle.

            auto _fetchAllFade = MultiSignatureScan("\x41\xB8\xFF\xFF\xFF\xFF\x48\x8D\x0D\x00\x00\x00\x00\x0F\xB7\xD3\x66\xF7\xD2\xE8\x00\x00\x00\x00\xB8\x01\x01\x00\x00", "xxxxxxxxx????xxxxxxx????xxxxx");

            for (auto _function : _fetchAllFade)
            {
                uint32_t _fadeValue = 0x500;
                vector<uint8_t> _replaceFade{ 0xBA, 0x00, 0xFF, 0x00, 0x00 };

                memcpy(_function + 0x0D, "\x90\x90\x90\x90\x90\x90", 0x06);
                memcpy(_function + 0x0D, _replaceFade.data(), 0x05);

                memcpy(_function + 0x19, &_fadeValue, 0x04);
            }

            // Simple counter offset hack.

            auto _fetchSimpleCont = MultiSignatureScan("\x41\x8D\x46\xAB\x41\x89\x84\x3F\x8C\x0D\x00\x00", "xxxxxxxxxxxx");

            for (auto _function : _fetchSimpleCont)
            {
                uint64_t _fetchCall = ResolveCallRelative(_function + 0x31);

                vector<uint8_t> _replaceCounter
                {
                    0x41, 0x8D, 0x86, 0x40, 0xFF, 0xFF, 0xFF,		// lea eax, [r14-0xC0]
                    0x41, 0x89, 0x84, 0x3F, 0x8C, 0x0D, 0x00, 0x00,	// mov [r15+rdi+0x0D8C], eax
                    0x3B, 0xB5, 0xB0, 0x0D, 0x00, 0x00,				// cmp esi, [rbp+0x0DB0];
                    0x7C, 0xB6,										// jl "_function - 0x33"
                    0x4C, 0x8B, 0xBC, 0x24, 0x88, 0x00, 0x00, 0x00,	// mov r15, [rsp+0x88]
                    0x48, 0x8B, 0xBC, 0x24, 0x80, 0x00, 0x00, 0x00,	// mov rdi, [rsp+0x80]
                    0x48, 0x8B, 0x5C, 0x24, 0x78,					// mov rbx, [rsp+0x78]
                    0x48, 0x8B, 0x4C, 0x24, 0x40,					// mov rcx, [rsp+0x40]
                    0x48, 0x31, 0xE1,								// xor rcx, rsp
                    0xE8, 0x00, 0x00, 0x00, 0x00,					// call "_fetchCall"
                    0x48, 0x83, 0xC4, 0x50,							// add rsp, 0x50
                    0x41, 0x5E,										// pop r14
                    0x5E,											// pop rsi
                    0x5D,											// pop rbp
                    0xC3											// ret
                };

                uint32_t _fetchCalc = static_cast<uint32_t>(_fetchCall - reinterpret_cast<uint64_t>(_function + 0x39));

                memcpy(_function, _replaceCounter.data(), _replaceCounter.size());
                memcpy(_function + 0x35, &_fetchCalc, 0x04);
            }

            // Disables culling. Causes some side effects that I don't believe anyone will notice.

            auto _fetchCulling3D = SignatureScan<char*>("\x48\x8B\xC4\x48\x89\x58\x18\x48\x89\x70\x20\x55\x57\x41\x54\x41", "xxxxxxxxxxxxxxxx");

            memcpy(_fetchCulling3D + 0x11D, "\xEB", 0x01);
            memcpy(_fetchCulling3D + 0x12B, "\xEB", 0x01);
            memcpy(_fetchCulling3D + 0x133, "\xEB", 0x01);
            memcpy(_fetchCulling3D + 0x141, "\xEB", 0x01);
            memcpy(_fetchCulling3D + 0x149, "\xEB", 0x01);
            memcpy(_fetchCulling3D + 0x152, "\xEB", 0x01);

            auto _fetchCulling2D = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xFA\xE8", "xxxxxxxxxxxxxxxxxxx");

            memcpy(_fetchCulling2D + 0x06C, "\x00", 0x01);
        }
    
        // This block initializes NEXT_FORM.

        dk::NEXT_FORM::instance = (char*)malloc(0xDB8);

        if (dk::NEXT_FORM::instance != nullptr)
        {
            memset(dk::NEXT_FORM::instance, 0x00, 0xDB8);

            *reinterpret_cast<uint64_t*>(dk::NEXT_FORM::instance) = reinterpret_cast<uint64_t>(dk::NEXT_FORM::VTABLE_CLASS);
            *reinterpret_cast<uint64_t*>(dk::NEXT_FORM::instance + 0x38) = reinterpret_cast<uint64_t>(dk::NEXT_FORM::VTABLE_SPRITE);

            YI::SEQUENCE::CreateNew(dk::NEXT_FORM::instance + 0x58);

            char* _seqdChain = dk::NEXT_FORM::instance + 0x230;

            for (int i = 0; i < 7; i++)
            {
                YI::SEQUENCE::CreateNew(_seqdChain);
                _seqdChain += 0x1A0;
            }

            *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0x048) = 0x00;
            *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0x214) = 0x00;
        }
        #endif

        #ifndef BUILD_NMC
            // Prevent MAGIC clearing since we handle that now, and because it causes a crash.

            auto _funcMagicClear = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x48\x89\x74\x24\x50\x48\x8B\xD8\x4C\x89\x74\x24\x58\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x0F\x29\x74\x24\x30\xF3\x0F\x10\x35\x00\x00\x00\x00\x0F\x29\x7C\x24\x20\x0F\x57\xFF\x48\x85\xDB\x75\x08", "xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxxxxxxxxxxx");
            memcpy(_funcMagicClear + 0x18A, "\x90\x90\x90\x90\x90", 0x05);

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

            // If "00shopface.bin" exists, patch all SHOPFACE functions to use the file instead.
            if (YS::FILE::GetSize("00shopface.bin"))
            {
                char* _patchShopfaceFirst = SignatureScan<char*>("\x40\x56\x48\x81\xEC\xC0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xA0\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x33\xF6\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxxxxx????xxx????");
                char* _patchShopfaceSecond = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xDA\x8B\xF9\x48\x8B\xCB\x48\x8D\x15\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????x????");
                char* _patchShopfaceThird = SignatureScan<char*>("\x40\x57\x48\x81\xEC\xB0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xA0\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x33\xFF\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxxxxx????xxx????");

                char* _allocShopface = (char*)malloc(YS::FILE::GetSize("00shopface.bin"));
                auto _readShopface = YS::FILE::Read("00shopface.bin", _allocShopface);

                if (_readShopface != 0x00)
                {
                    uint8_t _faceCount = *_allocShopface;

                    YS::PANACEA_ALLOC::Allocate("SHOPFACE_NAMES", 0x10 * _faceCount);
                    YS::PANACEA_ALLOC::Allocate("SHOPFACE_STRUCTS", 0x10 * _faceCount);

                    for (int i = 0; i < _faceCount; i++)
                    {
                        auto _objectID = *reinterpret_cast<uint16_t*>(_allocShopface + 0x10 + (0x10 * i));
                        char _faceName[0x0E];

                        memcpy(_faceName, _allocShopface + 0x02 + 0x10 + (0x10 * i), 0x0E);

                        auto _calculateNameAddr = reinterpret_cast<uint64_t>(YS::PANACEA_ALLOC::Get("SHOPFACE_NAMES") + 0x10 * i);

                        memcpy(YS::PANACEA_ALLOC::Get("SHOPFACE_STRUCTS") + 0x10 * i, &_objectID, 0x02);
                        memcpy(YS::PANACEA_ALLOC::Get("SHOPFACE_STRUCTS") + (0x10 * i) + 0x08, &_calculateNameAddr, 0x08);

                        memcpy(YS::PANACEA_ALLOC::Get("SHOPFACE_NAMES") + 0x10 * i, _faceName, 0x0E);
                    }

                    char* _structOffset = reinterpret_cast<char*>(YS::PANACEA_ALLOC::Get("SHOPFACE_STRUCTS"));

                    RedirectLEA(_patchShopfaceFirst + 0x53, _structOffset);
                    RedirectLEA(_patchShopfaceThird + 0x50, _structOffset);
                    RedirectLEA(_patchShopfaceSecond + 0x28, _structOffset);

                    RedirectLEA(_patchShopfaceFirst + 0x62, _structOffset + 0x08);
                    RedirectLEA(_patchShopfaceThird + 0x41, _structOffset + 0x08);
                    RedirectLEA(_patchShopfaceSecond + 0x83, _structOffset + 0x08);
                }
            }

            #ifdef BUILD_EXPERIMENTAL
            // If "00helpimage.bin" exists, patch all HELPIMAGE functions to use the file instead.
            if (YS::FILE::GetSize("00helpimage.bin"))
            {
                vector<char*> _patchHelpImage
                {
                    SignatureScan<char*>("\x40\x53\x57\x41\x57\x48\x83\xEC\x50\x48\x8B\x0D\x00\x00\x00\x00\x48\x81\xC1\x40\x05\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxx????"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x8B\xF1\xE8\x00\x00\x00\x00\x48\x8B\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x8B\xE8\x4C\x8D\x35\x00\x00\x00\x00\x33\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????xxx????xxxxxx????xx"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x63\xFA\x48\x8D\x2D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx????"),
                    SignatureScan<char*>("\x48\x83\xEC\x28\x4C\x8B\x05\x00\x00\x00\x00\x49\x0F\xBE\x50\x0E\x41\x0F\xBE\x48\x0D\x8D\x42\x01\x3B\xC1\x7C\x23\x48\x8B\x0D\x00\x00\x00\x00\xBA\x14\x00\x00\x00", "xxxxxxx????xxxxxxxxxxxxxxxxxxxx????xxxxx"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x40\x32\xF6\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x81\xEC\xD0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xC0\x00\x00\x00\x33\xC9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx????"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x0F\xB7\x0D\x00\x00\x00\x00\x4C\x8D\x35\x00\x00\x00\x00\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????xxx????xx"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xF9\x48\x8D\x1D\x00\x00\x00\x00\x83\xFF\xFF\x74\x08\x0F\xB6\x43\x02\x3B\xC7\x75\x0C\x0F\xB7\x0B\xE8\xC8\x34\x03\x00\x84\xC0\x75\x17", "xxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxx"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x50\x48\x8B\x0D\x00\x00\x00\x00\x48\x81\xC1\x20\x03\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxx????xxxxxxxx????"),
                    SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x8B\xF1\x8B\xDA\xB1\x01\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????"),
                };

                char* _allocHelpimage = (char*)malloc(YS::FILE::GetSize("00helpimage.bin"));
                auto _readHelpimage = YS::FILE::Read("00helpimage.bin", _allocHelpimage);

                if (_readHelpimage != 0x00)
                {
                    uint8_t _structCount = *_allocHelpimage;
                    uint8_t _stringCount = *(_allocHelpimage + 0x08);

                    uint32_t _structStart = *reinterpret_cast<uint32_t*>(_allocHelpimage + 0x04);
                    uint32_t _stringStart = *reinterpret_cast<uint32_t*>(_allocHelpimage + 0x0C);

                    YS::PANACEA_ALLOC::Allocate("HELPIMAGE_STRUCT", 0x10 * _structCount);
                    YS::PANACEA_ALLOC::Allocate("HELPIMAGE_STRING", 0x04 * _stringCount);
                    YS::PANACEA_ALLOC::Allocate("HELPIMAGE_FNAMES", 0x10 * _structCount);

                    for (int i = 0; i < _structCount; i++)
                    {
                        auto _namePtr = _allocHelpimage + _structStart + (0x10 * i);
                        auto _structPtr = reinterpret_cast<uint32_t*>(_allocHelpimage + _structStart + 0x0C + (0x10 * i));

                        auto _fileNamePtr = reinterpret_cast<uint64_t>(YS::PANACEA_ALLOC::Get("HELPIMAGE_FNAMES") + 0x10 * i);

                        memcpy(YS::PANACEA_ALLOC::Get("HELPIMAGE_FNAMES") + 0x10 * i, _namePtr, 0x0C);

                        memcpy(YS::PANACEA_ALLOC::Get("HELPIMAGE_STRUCT") + 0x10 * i, &_fileNamePtr, 0x08);
                        memcpy(YS::PANACEA_ALLOC::Get("HELPIMAGE_STRUCT") + 0x10 * i + 0x08, _structPtr, 0x04);
                    }

                    for (int i = 0; i < _stringCount; i++)
                    {
                        auto _stringPtr = reinterpret_cast<uint16_t*>(_allocHelpimage + _stringStart + (0x02 * i));
                        memcpy(YS::PANACEA_ALLOC::Get("HELPIMAGE_STRING") + 0x02 * i, _stringPtr, 0x02);
                    }

                    char* _startAddr = const_cast<char*>(moduleInfo.startAddr);
                    char* _getStructPtr = YS::PANACEA_ALLOC::Get("HELPIMAGE_STRUCT");

                    uint32_t _structOffset = static_cast<uint32_t>(YS::PANACEA_ALLOC::Get("HELPIMAGE_STRUCT") - _startAddr);
                    uint32_t _stringOffset = static_cast<uint32_t>(YS::PANACEA_ALLOC::Get("HELPIMAGE_STRING") - _startAddr);

                    RedirectLEA(_patchHelpImage[9] + 0x03A, _getStructPtr);
                    RedirectLEA(_patchHelpImage[9] + 0x053, _getStructPtr + 0x08);

                    RedirectLEA(_patchHelpImage[0] + 0x0A0, _getStructPtr);
                    RedirectLEA(_patchHelpImage[1] + 0x032, _getStructPtr);
                    RedirectLEA(_patchHelpImage[3] + 0x045, _getStructPtr);
                    RedirectLEA(_patchHelpImage[4] + 0x024, _getStructPtr);

                    RedirectLEA(_patchHelpImage[1] + 0x028, _getStructPtr + 0x08);
                    RedirectLEA(_patchHelpImage[6] + 0x026, _getStructPtr + 0x08);
                    RedirectLEA(_patchHelpImage[7] + 0x00C, _getStructPtr + 0x08);

                    RedirectMOVZX(_patchHelpImage[5] + 0x171, _getStructPtr + 0x08);
                    RedirectMOVZX(_patchHelpImage[6] + 0x01F, _getStructPtr + 0x08);
                    RedirectMOVZX(_patchHelpImage[4] + 0x030, _getStructPtr + 0x08);
                    RedirectMOVZX(_patchHelpImage[4] + 0x05A, _getStructPtr + 0x08);

                    memcpy(_patchHelpImage[8] + 0x1DE + 0x04, &_stringOffset, 0x04);
                    memcpy(_patchHelpImage[2] + 0x099 + 0x04, &_structOffset, 0x04);

                    auto _offsetCalc01 = _structOffset + 0x08;
                    memcpy(_patchHelpImage[5] + 0x1B5 + 0x04, &_offsetCalc01, 0x04);

                    auto _offsetCalc02 = _structOffset + 0x0A;
                    memcpy(_patchHelpImage[5] + 0x101 + 0x03, &_offsetCalc02, 0x04);
                    memcpy(_patchHelpImage[5] + 0x3F6 + 0x03, &_offsetCalc02, 0x04);
                    memcpy(_patchHelpImage[8] + 0x1D6 + 0x04, &_offsetCalc02, 0x04);
                    memcpy(_patchHelpImage[5] + 0x0F9 + 0x04, &_offsetCalc02, 0x04);
                    memcpy(_patchHelpImage[5] + 0x3EA + 0x05, &_offsetCalc02, 0x04);

                    _offsetCalc02++;
                    memcpy(_patchHelpImage[2] + 0x029 + 0x04, &_offsetCalc02, 0x04);
                }
            }
            #endif

            vector<uint16_t> _musicConfig{ 0x01, 0x5718, 0x5719, 0x571A, 0x0000 };
            vector<uint16_t> _resourceConfig{ 0x01, 0x571F, 0x573A, 0x573B, 0x0000 };

            // This code block handles RESOURCE packs.

            if (YS::MESSAGE::GetData(0x573C) != _fetchFake)
            {
                _resourceConfig[0] += 1;

                auto _indexName = find(_resourceConfig.begin(), _resourceConfig.end(), 0x573B);
                _resourceConfig.insert(_indexName, 0x573C);

                auto _indexBitwise = find(_resourceConfig.begin(), _resourceConfig.end(), 0x0000);
                _resourceConfig.insert(_indexBitwise, 0x573D);

                _resourceConfig.push_back(0x0200);
            }

            if (YS::MESSAGE::GetData(0x573E) != _fetchFake)
            {
                _resourceConfig[0] += 1;

                auto _indexName = find(_resourceConfig.begin(), _resourceConfig.end(), 0x573B);
                _resourceConfig.insert(_indexName, 0x573E);

                auto _indexBitwise = find(_resourceConfig.begin(), _resourceConfig.end(), 0x0000);
                _resourceConfig.insert(_indexBitwise, 0x573F);

                _resourceConfig.push_back(0x0400);
            }

            if (_resourceConfig[0] > 1)
            {
                Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _resourceConfig);

                std::vector<uint32_t> _resourceIntro;

                for (auto _element : _resourceConfig)
                    _resourceIntro.push_back(_element);

                _resourceIntro.insert(_resourceIntro.begin() + 0x02, 0xFFFF);
                _resourceIntro[0x01] = 0x5736;

                Tz::HookIntro::Add(UINT32_MAX, _resourceIntro);
            }

            // This code block handles MUSIC packs.

            if (YS::MESSAGE::GetData(0x571B) != _fetchFake)
            {
                _musicConfig[0] += 1;

                auto _indexName = find(_musicConfig.begin(), _musicConfig.end(), 0x571A);
                _musicConfig.insert(_indexName, 0x571B);

                auto _indexBitwise = find(_musicConfig.begin(), _musicConfig.end(), 0x0000);
                _musicConfig.insert(_indexBitwise, 0x571C);

                _musicConfig.push_back(0x0080);
            }

            if (YS::MESSAGE::GetData(0x571D) != _fetchFake)
            {
                _musicConfig[0] += 1;

                auto _indexName = find(_musicConfig.begin(), _musicConfig.end(), 0x571A);
                _musicConfig.insert(_indexName, 0x571D);

                auto _indexBitwise = find(_musicConfig.begin(), _musicConfig.end(), 0x0000);
                _musicConfig.insert(_indexBitwise, 0x571E);

                _musicConfig.push_back(0x0100);
            }

            if (_musicConfig[0] > 1)
            {
                Tz::HookConfig::Add(Tz::HookConfig::Entries.size() - 0x03, _musicConfig);

                std::vector<uint32_t> _musicIntro;

                for (auto _element : _musicConfig)
                    _musicIntro.push_back(_element);

                _musicIntro.insert(_musicIntro.begin() + 0x02, 0xFFFF);
                _musicIntro[0x01] = 0x5735;

                Tz::HookIntro::Add(UINT32_MAX, _musicIntro);
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

                        uint32_t* (*checkIntro)() = (uint32_t * (*)())GetProcAddress(_moduleHandle, "RF_CheckIntro");
                        uint16_t* (*checkConfig)() = (uint16_t * (*)())GetProcAddress(_moduleHandle, "RF_CheckConfig");

                        vector<uint32_t> _vectorIntro(0);
                        vector<uint16_t> _vectorConfig(0);

                        if (checkIntro)
                        {
                            auto _fetchIntro = checkIntro();
                            auto _sizeVector = 0x0C + 0x04 * (_fetchIntro[0] * 3);

                            _vectorIntro.resize(_sizeVector / 4);
                            memcpy(_vectorIntro.data(), _fetchIntro, _sizeVector);

                            Tz::HookIntro::Add(UINT32_MAX, _vectorIntro);

                            uint32_t** introSeek = (uint32_t**)GetProcAddress(_moduleHandle, "INTRO_SEEK");

                            assert(introSeek != nullptr);
                            *introSeek = reinterpret_cast<uint32_t*>(YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x200 + ((Tz::HookIntro::Entries.size() - 1) * 0x04));
                        }

                        if (checkConfig)
                        {
                            auto _fetchConfig = checkConfig();
                            auto _sizeVector = 0x04 + 0x02 * (_fetchConfig[0] * 3);

                            _vectorConfig.resize(_sizeVector / 2);
                            memcpy(_vectorConfig.data(), _fetchConfig, _sizeVector);

                            Tz::HookConfig::Add(UINT32_MAX, _vectorConfig);

                            uint16_t** configSeek = (uint16_t**)GetProcAddress(_moduleHandle, "CONFIG_SEEK");

                            assert(configSeek != nullptr);
                            {
                                uint16_t _checkBitwise = 0x00;

                                for (int i = 0; i < Tz::HookConfig::Entries.size(); i++)
                                {
                                    auto _fetchCount = Tz::HookConfig::Entries[i][0];

                                    for (int z = 0; z < _fetchCount; z++)
                                    {
                                        auto _fetchBitwise = Tz::HookConfig::Entries[i][0x04 + (0x02 * _fetchCount * 0x02) + z];

                                        if (_checkBitwise & _fetchBitwise)
                                        {
                                            if (i == Tz::HookConfig::Entries.size() - 1)
                                                *configSeek = reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A6);

                                            else
                                                continue;
                                        }

                                        if (i == Tz::HookConfig::Entries.size() - 1)
                                        {
                                            *configSeek = reinterpret_cast<uint16_t*>(YS::AREA::SaveData + 0x41A4);
                                            break;
                                        }

                                        _checkBitwise |= _fetchBitwise;
                                    }
                                }
                            }
                        }

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
            #endif

            INITIALIZED = true;
        }
    
        else
        {
            #ifndef BUILD_ARCHIPELAGO_LITE
            Tz::HookIntro::Handle();
            Tz::HookConfig::Handle();

            SOFT_RESET();
            ENFORCE_FRAMERATE();
            AUTOSAVE();
            #endif

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            HANDLE_MUSIC();
            HANDLE_RESOURCE();
            RETRY_BATTLES();
            DISPLAY_NEXT_EXP();
            HANDLE_SHAKE();
            ENFORCE_PROMPTS();
            FIX_SAVE_POINT();
            #endif

            #ifndef BUILD_NMC
            REGISTER_MAGIC();
            REGISTER_ABILITY();
            SHOW_INFORMATION();
            PROCESS_DEATH();

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            HANDLE_GOA_LAND();
            ENFORCE_LOCKON();
            #endif
            #endif

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            for (auto _execPair : _execModule)
                _execPair.second();


            if (DISCORD_ENABLED)
                DISCORD_RPC();

            if (!IS_NOASPECT)
                HANDLE_ASPECT();
            #endif
        }
    }
}