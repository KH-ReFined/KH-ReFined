#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "obj2d.h"
#include "sound.h"
#include "sequence.h"
#include "memorymgr.h"
#include "messagedraw.h"
#include "panacea_alloc.h"

extern "C"
{
	namespace dk {

		class DLL_EXPORT Scis 
		{
        private:
            static bool _init()
            {
                RedirectFunction("\x66\x89\x54\x24\x08\x66\xFF\xCA\x66\x44\x89\x44\x24\x0C\x66\x41", "xxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(setScis), 0x21);
                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

			public:
				static void setScis(char* scissor, short x, short y, short width, short height)
                {
                    auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
                    auto _offsetValue = 85;

                    if (_fetchMemory)
                        memcpy(&_offsetValue, _fetchMemory, 0x04);

                    auto _applyAspect = (_offsetValue * -1) + 85;

                    long long _calcScis = 0x00;

                    _calcScis |= (static_cast<long long>(x + (_offsetValue > 85 ? _applyAspect : 0x00)) & 0x000000000000FFFF);
                    _calcScis |= (static_cast<long long>(width + (x + _applyAspect) - 1) & 0x000000000000FFFF) << 16;
                    _calcScis |= (static_cast<long long>(y) & 0x000000000000FFFF) << 32;
                    _calcScis |= (static_cast<long long>(height + y - 1) & 0x000000000000FFFF) << 48;

                    *reinterpret_cast<long long*>(scissor + 0x10) = _calcScis;
                }
		};
	}
}