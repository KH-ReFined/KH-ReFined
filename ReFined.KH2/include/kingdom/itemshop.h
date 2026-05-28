#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "member.h"
#include "memorymgr.h"
#include "panacea_alloc.h"

extern "C"
{
	namespace OvlShop
	{
		class DLL_EXPORT ItemShop
		{
		    public:
			    static void GetFaceFileNmae(int part, char* buff)
                {
                    auto _fetchShopface = YS::PANACEA_ALLOC::Get("00shopface.bin");

                    if (_fetchShopface)
                    {
                        auto _fetchPartId = YS::MEMBER::PartToEntryID(part);
                        auto _fetchCount = *reinterpret_cast<uint32_t*>(_fetchShopface);

                        for (int i = 0; i < _fetchCount; i++)
                        {
                            auto _fetchEntryId = *reinterpret_cast<uint16_t*>(_fetchShopface + 0x10 + 0x10 * i);

                            if (_fetchEntryId == _fetchPartId)
                            {
                                sprintf_s(buff, 0x28, "menu/shopface/%s.bin", _fetchShopface + 0x12 + 0x10 * i);
                                return;
                            }
                        }
                    }

                    sprintf_s(buff, 0x28, "menu/shopface/p_ex020.bin");
                }

            private:
                static bool _init()
                {
                    RedirectFunction("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xDA\x8B\xF9\x48\x8B\xCB\x48\x8D\x15\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????x????", reinterpret_cast<uint64_t>(GetFaceFileNmae), 0xFA);
                    return true;
                }

                #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
                static inline bool _doInit = _init();
                #endif
		};
	}
}