#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include <cstdint>

extern "C"
{
	namespace kn
	{
		class DLL_EXPORT FVector
		{
		    public:
			    static uint32_t* Init(uint32_t* _firstPtr, uint32_t* _secondPtr)
                {
                    if (!_firstPtr || !_secondPtr || reinterpret_cast<char*>(_firstPtr) > moduleInfo.moduleEnd || reinterpret_cast<char*>(_secondPtr) > moduleInfo.moduleEnd)
                        return nullptr;

                    if (_firstPtr != _secondPtr)
                    {
                        *_firstPtr = *_secondPtr;
                        _firstPtr[1] = _secondPtr[1];
                        _firstPtr[2] = _secondPtr[2];
                        _firstPtr[3] = _secondPtr[3];
                    }

                    return _firstPtr;
                }

            private:
                static bool _init()
                {
                    RedirectFunction("\x48\x3B\xCA\x74\x16\x8B\x02\x89\x01", "xxxxxxxxx", reinterpret_cast<uint64_t>(Init), 0x1F);
                    return true;
                }

                #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
                static inline bool _doInit = _init();
                #endif
		};
	}
}