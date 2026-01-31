#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Obj2D
		{
		public:
			using isExist_t = bool(*)(char* obj);
			using commitSuicide_t = void(*)(char* obj, int unk1);

			static isExist_t isExist;
			static commitSuicide_t commitSuicide;
		};
	}
}