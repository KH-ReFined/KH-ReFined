#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT IMAGE
		{
		public:
			using IMAGE_t = void(*)(char* imageBuff);
			static IMAGE_t _IMAGE;

			using MakePacket_t = void(*)(char* imageBuff);
			static MakePacket_t MakePacket;

			using Init_t = void(*)(char* imageBuff, char* imd);
			static Init_t Init;

			using InitLoadImage_t = void(*)(char* imageBuff);
			static InitLoadImage_t InitLoadImage;

			using LoadTexture_t = void(*)(char* imageBuff);
			static LoadTexture_t LoadTexture;

			using ReleaseImage_t = char*(*)(char* imd);
			static ReleaseImage_t ReleaseImage;

			using CreateImage_t = char*(*)(char* imd);
			static CreateImage_t CreateImage;
		};
	}
}