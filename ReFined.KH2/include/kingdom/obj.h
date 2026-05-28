#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "collision_data.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJ
		{
		private:
			static bool _init()
			{
				RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x33\xDB\x48\x8B\xF2\x48\x8B\xF9\x48\x39\x99", "xxxxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(get_top_matrix), 0x65);
				return true;
			}

			#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
			static inline bool _doInit = _init();
			#endif

		public:
			static inline bool(*is_exist)(char* obj) = FetchFunctionFromCall<bool(*)(char*)>("\x48\x89\x5C\x24\x10\x57\x48\x81\xEC\xB0\x00\x00\x00\x48\x8B\xF9", "xxxxxxxxxxxxxxxx", 0x17);
			static inline uint16_t(*get_part_num)(char* obj) = FetchFunctionFromCall<uint16_t(*)(char*)>("\x40\x56\x48\x81\xEC\xC0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x88\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxx", 0x388);
			static inline bool(*is_enable_collision)(char* obj, uint32_t group) = FindSignature<bool(*)(char*, uint32_t)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x83\xB9\xB8\x0A\x00\x00\x00\x8B\xDA\x48\x8B\xF9\x74\x78", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline float(*get_bone_matrix)(char* obj, uint32_t bone, char* matrix) = FindSignature<float(*)(char*, uint32_t, char*)>("\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x70\x10\x48\x89\x78\x18\x55\x48\x8D\x68\xE8\x48\x81\xEC\x10\x01\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
            
            static float get_top_matrix(char* obj, char* matrix)
			{
				if (obj)
				{
					auto _fetchData = *reinterpret_cast<char**>(obj + 0x0AB8);
					char* _fetchElem = YS::COLLISION_DATA::next_elem(_fetchData, 15, nullptr);

					if (_fetchData)
					{
						while (_fetchElem && !YS::OBJ::is_enable_collision(obj, *_fetchElem))
							_fetchElem = YS::COLLISION_DATA::next_elem(_fetchData, 15, _fetchElem);
					}

					YS::OBJ::get_bone_matrix(obj, *reinterpret_cast<short*>(_fetchElem + 0x06), matrix);
					return static_cast<float>(*reinterpret_cast<short*>(_fetchElem + 0x08));
				}
			}
		};
	}
}