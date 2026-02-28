#include "save_indicator.h"

char* YS::SAVE_INDICATOR::image = nullptr;
char* YS::SAVE_INDICATOR::instance = nullptr;
char* YS::SAVE_INDICATOR::layoutFile = nullptr;

char* YS::SAVE_INDICATOR::VTABLE_CLASS = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x154);
char* YS::SAVE_INDICATOR::VTABLE_SPRITE = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x80);

void YS::SAVE_INDICATOR::init()
{
    if (!YS::SAVE_INDICATOR::instance)
    {
        auto _fetchSize = YS::FILE::GetSize("field2d/moogle.2dd");

        YS::SAVE_INDICATOR::layoutFile = (char*)malloc(_fetchSize);

        if (_fetchSize)
            YS::FILE::ReadBAR("field2d/moogle.2dd", YS::SAVE_INDICATOR::layoutFile);

        auto _intptrIMD = *reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(YS::SAVE_INDICATOR::layoutFile, 0x18, 0x65766173, 0) + 0x08);
        auto _intptrSQD = *reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(YS::SAVE_INDICATOR::layoutFile, 0x19, 0x65766173, 0) + 0x08);

        auto _pointerIMD = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_intptrIMD));
        auto _pointerSQD = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_intptrSQD));

        YS::SAVE_INDICATOR::image = (char*)malloc(0x200);
        YS::SAVE_INDICATOR::instance = (char*)malloc(0xDB8);

        if (YS::SAVE_INDICATOR::image && YS::SAVE_INDICATOR::instance)
        {
            memset(YS::SAVE_INDICATOR::image, 0x00, 0x200);
            memset(YS::SAVE_INDICATOR::instance, 0x00, 0xDB8);

            YI::IMAGE::_IMAGE(YS::SAVE_INDICATOR::image);
            YI::IMAGE::Init(YS::SAVE_INDICATOR::image, _pointerIMD);
            
            if (YI::IMAGE_CACHE::Load(YS::SAVE_INDICATOR::image))
            {
                *reinterpret_cast<uint64_t*>(YS::SAVE_INDICATOR::instance) = reinterpret_cast<uint64_t>(YS::SAVE_INDICATOR::VTABLE_CLASS);
                *reinterpret_cast<uint64_t*>(YS::SAVE_INDICATOR::instance + 0x38) = reinterpret_cast<uint64_t>(YS::SAVE_INDICATOR::VTABLE_SPRITE);

                YI::SEQUENCE::_SEQUENCE(YS::SAVE_INDICATOR::instance + 0x58);

                *reinterpret_cast<uint32_t*>(YS::SAVE_INDICATOR::instance + 0x048) = 0x00;
                *reinterpret_cast<uint32_t*>(YS::SAVE_INDICATOR::instance + 0x214) = 0x00;
            }
        }
    }
}

void YS::SAVE_INDICATOR::create(int offset16x9)
{
    YS::SAVE_INDICATOR::init();

    auto _infoSQD = YS::BINARC::get_info_by_tag(YS::SAVE_INDICATOR::layoutFile, 0x19, 0x65766173, 0);
    auto _addrSQD = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_infoSQD + 0x08)));

    if (!dk::Obj2D::isExist(YS::SAVE_INDICATOR::instance + 0x38))
    {
        dk::Sprite::create(YS::SAVE_INDICATOR::instance + 0x38, 5000, _addrSQD, YS::SAVE_INDICATOR::image, 0, 0, offset16x9);
        dk::Sprite::setNum(YS::SAVE_INDICATOR::instance + 0x38, 0x00);

        dk::Obj2D::commitSuicide(YS::SAVE_INDICATOR::instance + 0x38, 0x00);
        dk::Sprite::setPos(YS::SAVE_INDICATOR::instance + 0x38, 0x00, 0x00);

        dk::INFO_BASE::create(YS::SAVE_INDICATOR::instance, 0x13, 0, 5000, 0x00, 0x00);
    }
}