#include "itempic.h"

int* YS::ITEMPIC::Phase =	 ResolveRelativeAddress<int*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xE9);
int* YS::ITEMPIC::UserNum =	 ResolveRelativeAddress<int*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xEF);
int* YS::ITEMPIC::CachePic = ResolveRelativeAddress<int*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xE3);
char* YS::ITEMPIC::Task =	 ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0x105);

YS::ITEMPIC::ReadImage_t YS::ITEMPIC::ReadImage = SignatureScan<YS::ITEMPIC::ReadImage_t>("\x83\xF9\xFF\x74\x4D\x53\x48\x83\xEC\x40\x48\x8D\x05\x00\x00\x00\x00\xC7\x44\x24\x30\x8C\x00\x00\x00", "xxxxxxxxxxxxx????xxxxxxxx");
YS::ITEMPIC::FreeImageData_t YS::ITEMPIC::FreeImageData = SignatureScan<YS::ITEMPIC::FreeImageData_t>("\x48\x83\xEC\x28\x83\x2D\x00\x00\x00\x00\x01\xB9\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x0F\x44\xC1\x83\x3D\x00\x00\x00\x00\xFF\x89\x05\x00\x00\x00\x00\x74\x2E", "xxxxxx????xxxxxxxx????xxxxx????xxx????xx");

char* YS::ITEMPIC::ImageBuff = ResolveRelativeAddress<char*>("\x48\x83\xEC\x28\x83\x2D\x00\x00\x00\x00\x01\xB9\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x0F\x44\xC1\x83\x3D\x00\x00\x00\x00\xFF\x89\x05\x00\x00\x00\x00\x74\x2E", "xxxxxx????xxxxxxxx????xxxxx????xxx????xx", 0x2B);
uint16_t* YS::ITEMPIC::LoadedId = ResolveRelativeAddress<uint16_t*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\x0D\x00\x00\x00\x00\xB8\xFF\xFF\xFF\xFF\x33\xDB", "xxxxxxxxx????xxxxxxx", 0x2F);
uint16_t** YS::ITEMPIC::ToLoadID = ResolveRelativeAddress<uint16_t**>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x", 0x4D);

YS::ITEMPIC::staticInitializer YS::ITEMPIC::initialize;

char* YS::ITEMPIC::ReadImageThread(char* task)
{
    char nameBuff[40];
    auto _itempicID = *reinterpret_cast<uint16_t*>(task + 0x18);

	if (*Phase != 0x00)
		TASK::sleep(task, 0);

    if (*CachePic != -1)
    {
        auto _releaseResult = YI::IMAGE::ReleaseImage(ImageBuff);

        if (_releaseResult)
            *(_releaseResult + 0x99) = 0;

        if (*reinterpret_cast<uint16_t*>(ImageBuff + 0x04) == 0x0100 && *reinterpret_cast<uint32_t*>(ImageBuff + 0x10) == UINT32_MAX)
        {
            auto _fetchAddr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(ImageBuff + 0x08));

            if (*reinterpret_cast<uint32_t*>(_fetchAddr + 0x10) == 0x02F02EB5)
            {
                auto _compareNoName = strncmp((const char*)(_fetchAddr + 0x14), "no_name", 0x07);

                if (*reinterpret_cast<uint32_t*>(_fetchAddr + 0xB4) != UINT32_MAX)
                {
                    *reinterpret_cast<uint32_t*>(_fetchAddr + 0xB0) = 0x0A;
                    *reinterpret_cast<uint32_t*>(_fetchAddr + 0xB4) = 0x01;

                    if (_compareNoName)
                        *reinterpret_cast<uint64_t*>(_fetchAddr + 0xB8) = 0;

                    else
                        *reinterpret_cast<uint64_t*>(_fetchAddr + 0xB8) = UINT64_MAX;
                }
            }
        }

        *CachePic = -1;
    }

    *Phase = 1;

    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);
    string _constructPath = _fetchConfig & 0x0200 ? "itempic_2nd/item-%03d.imd" : (_fetchConfig & 0x0400 ? "itempic_3rd/item-%03d.imd" : "itempic/item-%03d.imd");

    sprintf(nameBuff, _constructPath.c_str(), _itempicID);

    if (!YS::FILE::GetSize(nameBuff))
        sprintf(nameBuff, "itempic/item-%03d.imd", _itempicID);

    YS::FILE::Read(nameBuff, ImageBuff);
    YI::IMAGE::CreateImage(ImageBuff);

    *CachePic = _itempicID;
    *Phase = 0;
    *UserNum = 0;

    return nullptr;
}