#include "cmdata.h"

char* Tz::CmData::MENU_FNAME_BUFFER = nullptr;
char* Tz::CmData::FAC_WRITE_BUFFER = nullptr;

char Tz::CmData::call_statement[0x20];

Tz::CmData::staticInitializer Tz::CmData::initialize;

char* Tz::CmData::MakeFname(char* buff, char* fileName)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
    auto _regionPointer = (!YS::REGION::Get() || YS::REGION::Get() == 7) ? reinterpret_cast<char*>(*YS::REGION::DefaultRegion) : reinterpret_cast<char*>(*YS::REGION::Region);

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

        printf("[Tz::CmData::MakeFname] | Fulfilling FILE request for: \"%s\"\n", buff);
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

        printf("[Tz::CmData::MakeFname] | Fulfilling MENU request for: \"%s\"\n", buff);
        return buff;
    }
}

void Tz::CmData::MakeFnameFace(uint16_t id)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
    auto _writeBuff = *reinterpret_cast<char**>(Tz::CmData::FAC_WRITE_BUFFER);

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

    printf("[Tz::CmData::MakeFnameFace] | Fulfilling FAC request for: \"%s\"\n", _writeBuff);
}

void Tz::CmData::MakeFnameItempic(char* buff, uint16_t id)
{
    buff = buff + 0x30;

    auto _fetchPicturePtr = *YS::ITEMPIC::ToLoadID;
    auto _fetchPictureID = 0x00;

    if (_fetchPicturePtr != nullptr)
        _fetchPictureID = *_fetchPicturePtr;

    else
        _fetchPictureID = id;

    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
    string _constructPath = _fetchConfig & 0x0200 ? "itempic_2nd/item-%03d.imd" : (_fetchConfig & 0x0400 ? "itempic_3rd/item-%03d.imd" : "itempic/item-%03d.imd");

    sprintf(buff, _constructPath.c_str(), _fetchPictureID);

    if (!YS::FILE::GetSize(buff))
        sprintf(buff, "itempic/item-%03d.imd", _fetchPictureID);

    printf("[Tz::CmData::MakeFnameItempic] | Fulfilling ITEMPIC request for: \"%s\"\n", buff);

}