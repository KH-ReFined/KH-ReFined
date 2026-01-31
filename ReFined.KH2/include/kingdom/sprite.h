#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Sprite
		{
		public:
			using setNum_t = void(*)(char* Sprite, int num);
			using create_t = void(*)(char* Sprite, int priority, char* seqd, char* image, int num, int group, int offset16x9);
			using setPos_t = void(*)(char* Sprite, int x, int y);

			static setNum_t setNum;
			static create_t create;
			static setPos_t setPos;
		};
	}
}