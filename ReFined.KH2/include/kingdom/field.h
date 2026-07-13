#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT  FIELD
		{
		public:
			static inline void(*CreateThread)(int id, int priority, void(*entryFunc)(char*), int stack_size)= reinterpret_cast<void(*)(int, int, void(*)(char*), int stack_size)>(FetchFunctionFromCall<char*>("\x40\x53\x48\x83\xEC\x20\x0F\xB6\xD9\xE8\x00\x00\x00\x00\x84\xC0", "xxxxxxxxxx????xx", 0xA2));
			static inline char* TaskManager = FetchRelativePointer<char*>(reinterpret_cast<char*>(YS::FIELD::CreateThread), 0x28);

			static bool TaskExists(void(*task)(char*))
			{
				void(*_funcItr)(char*) = nullptr;

				for (int i = 0; i < 1000; i++)
				{
					_funcItr = _funcItr ? reinterpret_cast<void(*)(char*)>(*reinterpret_cast<char**>(reinterpret_cast<char*>(_funcItr) + 0x78)) : reinterpret_cast<void(*)(char*)>(*reinterpret_cast<char**>(YS::FIELD::TaskManager + 16));

					if (!_funcItr || reinterpret_cast<char*>(_funcItr) < moduleInfo.moduleStart)
						break;

					if (*_funcItr == task)
						return true;
				}

				return false;
			}
		};
	}
}