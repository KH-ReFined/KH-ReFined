#include "objentry.h"

char* YS::OBJENTRY::MDLX_WRITE_BUFFER = nullptr;
char* YS::OBJENTRY::APDX_WRITE_BUFFER = nullptr;
char* YS::OBJENTRY::MSET_WRITE_BUFFER = nullptr;

YS::OBJENTRY::Get_t YS::OBJENTRY::Get = ResolveFunctionFromCall<YS::OBJENTRY::Get_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x1E2);
YS::OBJENTRY::GetFriendPriority_t YS::OBJENTRY::GetFriendPriority = ResolveFunctionFromCall<YS::OBJENTRY::GetFriendPriority_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x111);
YS::OBJENTRY::ReadRequestWeapon_t YS::OBJENTRY::ReadRequestWeapon = ResolveFunctionFromCall<YS::OBJENTRY::ReadRequestWeapon_t>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x169);

char* YS::OBJENTRY::get_mdlx(char* objentryEntry, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
    string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.mdlx" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mdlx" : "obj/%s.mdlx");

    char* _mdlxName = objentryEntry + 0x08;
    char* _useBuff = !buff ? YS::OBJENTRY::MDLX_WRITE_BUFFER : buff;

    sprintf(_useBuff, _constructPath.c_str(), _mdlxName);

    if (!YS::FILE::GetSize(_useBuff))
        sprintf(_useBuff, "obj/%s.mdlx", _mdlxName);

    printf("[YS::OBJENTRY::get_mdlx] | Fulfilling MDLX request for: \"%s\"\n", _useBuff);
    return _useBuff;
}

char* YS::OBJENTRY::get_apdx(char* objentryEntry, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    string _encodedRegion = _fetchConfig & 0x0004 ? "jp" :
        (_fetchConfig & 0x0008 ? "es" :
            (_fetchConfig & 0x0010 ? "de" :
                (_fetchConfig & 0x0020 ? "bg" : "us")));

    char* _apdxName = objentryEntry + 0x08;
    char* _useBuff = !buff ? YS::OBJENTRY::APDX_WRITE_BUFFER : buff;

    const char* _regionStr = !YS::REGION::Get() || YS::REGION::Get() == 0x07 ? "fm" : reinterpret_cast<char*>(*YS::REGION::Region);

    string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.a.%s" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.a.%s" : "obj/%s.a.%s");

    if ((*(objentryEntry + 0x48) & 0x01) != 0x00)
        return nullptr;

    sprintf(_useBuff, _constructPath.c_str(), _apdxName, _encodedRegion.c_str());

    if (!YS::FILE::GetSize(_useBuff))
        sprintf(_useBuff, _constructPath.c_str(), _apdxName, _regionStr);

    if (!YS::FILE::GetSize(_useBuff))
        sprintf(_useBuff, _constructPath.c_str(), _apdxName, "us");

    if (!YS::FILE::GetSize(_useBuff))
    {
        sprintf(_useBuff, "obj/%s.a.%s", _apdxName, _encodedRegion.c_str());

        if (!YS::FILE::GetSize(_useBuff))
            sprintf(_useBuff, "obj/%s.a.%s", _apdxName, _regionStr);

        if (!YS::FILE::GetSize(_useBuff))
            sprintf(_useBuff, "obj/%s.a.us", _apdxName);
    }

    printf("[YS::OBJENTRY::get_apdx] | Fulfilling APDX request for: \"%s\"\n", _useBuff);
    return _useBuff;
}

char* YS::OBJENTRY::get_mset(char* objentryEntry, uint32_t objectID, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    char* _mdlxName = objentryEntry + 0x08;
    char* _msetName = objentryEntry + 0x28;

    char* _useBuff = !buff ? YS::OBJENTRY::MSET_WRITE_BUFFER : buff;

    string _fetchMSET = _fetchConfig & 0x0200 ? "obj_2nd/%s.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mset" : "obj/%s.mset");
    string _fetchMEMO = _fetchConfig & 0x0200 ? "obj_2nd/%s_MEMO.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s_MEMO.mset" : "obj/%s_MEMO.mset");

    if (!*_msetName)
    {
        if ((objectID & 0x10000000) == 0x00)
            return nullptr;

        sprintf(_useBuff, _fetchMEMO.c_str(), _mdlxName);

        if (!YS::FILE::GetSize(_useBuff))
            sprintf(_useBuff, "obj/%s_MEMO.mset", _mdlxName);

        LABEL_14:
        printf("[YS::OBJENTRY::get_mset] | Fulfilling MEMO_MSET request for: \"%s\"\n", _useBuff);
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

        goto LABEL_14;
    }

    if (objectID <= 0x31A)
    {
        if (objectID > 0x318)
            goto LABEL_10;

    LABEL_9:
        if ((objectID & 0x20000000) == 0x00)
            goto LABEL_13;

        goto LABEL_10;
    }

    if (objectID != 0x03EE)
        goto LABEL_9;

LABEL_10:
    if (*(objentryEntry + 0x04) != 0x00 && YS::CACHE_BUFF::GetStatus(_useBuff) < 2)
        goto LABEL_12;

    LABEL_13:
    printf("[YS::OBJENTRY::get_mset] | Fulfilling MSET request for: \"%s\"\n", _useBuff);
    return _useBuff;
}

YS::OBJENTRY::staticInitializer YS::OBJENTRY::initialize;