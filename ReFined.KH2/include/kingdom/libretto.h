#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "region.h"
#include "cache_buff.h"
#include "area.h"
#include "file.h"

extern "C"
{
	namespace worldmap
	{
		class DLL_EXPORT Libretto
		{
		private:
			static bool _init()
			{

				vector<uint8_t> _injectInstructionSet =
				{
					0x48, 0x8B, 0xCE, // mov rcx, rsi
					0xE8, 0x00, 0x00, 0x00, 0x00, // call [function]
					0xE9, 0x00, 0x00, 0x00, 0x00  // jmp [offset]
				};

				auto _parseLibrettoFunc = FindSignature<char*>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x55\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x60\x48\x8B\x41", "xxxxxxxxxxxxxxxxxxxxxxxx");
				auto _librettoFlushOriginal = FindSignature<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\xE8\x00\x00\x00\x00\x85\xC0\x74\x0E\xE8\x00\x00\x00\x00\x83\xF8\x07\x74\x04\x32\xC0\xEB\x02\xB0\x01\x44\x8B\x0F\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxx????xxxxx????xxxxxxxxxxxxxxxxx????");

				uint32_t _calcCallOffset = _librettoFlushOriginal - (_parseLibrettoFunc + 0x136);
				uint32_t _calcJumpOffset = (_parseLibrettoFunc + 0x62C) - (_parseLibrettoFunc + 0x13B);

				memset(_parseLibrettoFunc + 0x12E, 0x90, 0x5D);

				memcpy(_injectInstructionSet.data() + 0x04, &_calcCallOffset, 0x04);
				memcpy(_injectInstructionSet.data() + 0x09, &_calcJumpOffset, 0x04);
				memcpy(_parseLibrettoFunc + 0x12E, _injectInstructionSet.data(), _injectInstructionSet.size());

				RedirectFunction(_librettoFlushOriginal, reinterpret_cast<uint64_t>(flush), 0x0116);

				return true;
			}

			#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
			static inline bool _doInit = _init();
			#endif

		public:
			static void flush(char* libretto)
			{
				auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

				string _constructPath = _fetchConfig & 0x0004 ? "libretto/jp/wm%d.bar" :
									   (_fetchConfig & 0x0008 ? "libretto/es/wm%d.bar" :
									   (_fetchConfig & 0x0010 ? "libretto/de/wm%d.bar" :
									   (_fetchConfig & 0x0020 ? "libretto/bg/wm%d.bar" : "libretto/us/wm%d.bar")));

				sprintf(libretto + 0x74, _constructPath.c_str(), *reinterpret_cast<uint32_t*>(libretto));

				if (!YS::FILE::GetSize(libretto + 0x74))
					sprintf(libretto + 0x74, "libretto/us/wm%d.bar", *reinterpret_cast<uint32_t*>(libretto));

				fprintf(stdout, "[worldmap::Libretto::flush] | Requested LIBRETTO read for: \"%s\"\n", libretto + 0x74);

				YS::CACHE_BUFF::ReadRequestSub(libretto + 0x74, -200, 0x00);
				YS::CACHE_BUFF::Flush(nullptr);
			}
		};
	}
}