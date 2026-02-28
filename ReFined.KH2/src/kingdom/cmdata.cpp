#include "cmdata.h"

char* Tz::CmData::MENU_FNAME_BUFFER = nullptr;
Tz::CmData::staticInitializer Tz::CmData::initialize;

char* Tz::CmData::MakeFname(char* buff, char* fileName)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
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