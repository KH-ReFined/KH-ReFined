#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "menu.h"
#include "obj2d.h"
#include "sequence.h"
#include "memorymgr.h"
#include "panacea_alloc.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Sprite
		{
			protected:
				static inline char* VTABLE_SPRITE = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x80);
				static inline char* VIEWPORT_ADDR = FetchRelativePointer<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

			public:
				static inline void(*setNum)(char* Sprite, int num) = FindSignature<void(*)(char*, int)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x89\x91\xD4\x01\x00\x00\x48", "xxxxxxxxxxxxxxxx");
				static inline void(*setPos)(char* Sprite, int x, int y) = FetchFunctionFromCall<void(*)(char*, int, int)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xBB);
				static inline void(*update)(char* Sprite) = FindSignature<void(*)(char*)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x8B\x49\x10\x8B\xC1\xC1\xE8", "xxxxxxxxxxxxxxxx");

				static inline void(*initSqd)(char* Sprite, char* sqd, char* image, int num);

				static void _Sprite(char* self)
				{
					*reinterpret_cast<uint64_t*>(self) = reinterpret_cast<uint64_t>(VTABLE_SPRITE);
				}

				static void initWork(char* Sprite)
				{
					memset(Sprite + 0x1C0, 0x00, 0x10);
					*reinterpret_cast<float*>(Sprite + 0x1D0) = 1.0;

					*reinterpret_cast<uint64_t*>(Sprite + 0x1C0) = 0x00;
					*reinterpret_cast<uint64_t*>(Sprite + 0x1C8) = 0x00;

					*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
					*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

					if (*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
					{
						*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
						*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

						while (*reinterpret_cast<uint64_t*>(Sprite + 0x1E8))
						{
							*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
							*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

							if (!*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
								break;

							*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
							*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;
						}
					}

					*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
					*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

					if (*reinterpret_cast<uint64_t*>(Sprite + 0x1E8))
					{
						*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
						*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

						if (*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
						{
							*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
							*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

							while (*reinterpret_cast<uint64_t*>(Sprite + 0x1E8))
							{
								*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
								*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

								if (!*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
									break;

								*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
								*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;
							}
						}
					}

					*(Sprite + 0x1F1) = 0x00;
				}
			
				static void draw(char* Sprite)
				{
					auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
					auto _isHudDraw = true;

					if (_fetchHudDraw)
						memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

					if (!_isHudDraw && !*MENU::IsMenu)
						return;

					int _activeX = 0;
					int _activeY = 0;

					auto _fetchObject = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01E0));
					auto _fetchSequence = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01E8));

					auto _didObjectInit = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0800;
					auto _canObjectDraw = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x1000;

					if (_didObjectInit != 0x0000)
					{
						if (_fetchObject)
							if (!dk::Obj2D::isExist(_fetchObject))
								return;

						if (_fetchSequence)
						{
							auto _canSequenceDraw = *reinterpret_cast<uint32_t*>(_fetchSequence + 0x0140) != 0x00;

							if (!_canSequenceDraw)
								return;
						}
					}

					if (_fetchObject)
					{
						_activeX = YI::SEQUENCE::GetActiveX(_fetchObject + 0x0020) + *reinterpret_cast<int*>(_fetchObject + 0x01C8) + *reinterpret_cast<int*>(_fetchObject + 0x01C0);
						_activeY = YI::SEQUENCE::GetActiveY(_fetchObject + 0x0020) + *reinterpret_cast<int*>(_fetchObject + 0x01CC) + *reinterpret_cast<int*>(_fetchObject + 0x01C4);

						*reinterpret_cast<int*>(Sprite + 0x01C0) = _activeX;
						*reinterpret_cast<int*>(Sprite + 0x01C4) = _activeY;
					}

					if (_fetchSequence)
					{
						_activeX = YI::SEQUENCE::GetActiveX(_fetchSequence) + *reinterpret_cast<int*>(_fetchSequence + 0x018C);
						_activeY = YI::SEQUENCE::GetActiveY(_fetchSequence) + *reinterpret_cast<int*>(_fetchSequence + 0x0190);

						*reinterpret_cast<int*>(Sprite + 0x01C0) = _activeX;
						*reinterpret_cast<int*>(Sprite + 0x01C4) = _activeY;
					}

					if ((_fetchObject || _fetchSequence) && (*reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0200) != 0x0000)
					{
						auto _activeRGBA = YI::SEQUENCE::GetActiveRGBA(_fetchObject ? _fetchObject + 0x0020 : _fetchSequence);

						auto _factorA = static_cast<float>((_activeRGBA & 0xFF000000) >> 0x18) * 0.0078125;

						auto _factorR = *reinterpret_cast<float*>(Sprite + 0x017C);
						auto _factorG = *reinterpret_cast<float*>(Sprite + 0x0180);
						auto _factorB = *reinterpret_cast<float*>(Sprite + 0x0184);

						if ((*reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0400) == 0x0000)
						{
							_factorB = ((_activeRGBA & 0x00FF0000) >> 0x10) * 0.0078125;
							_factorG = ((_activeRGBA & 0x0000FF00) >> 0x08) * 0.0078125;
							_factorR = (_activeRGBA & 0x000000FF) * 0.0078125;
						}

						YI::SEQUENCE::SetColorRate(Sprite + 0x0020, _factorR, _factorG, _factorB, _factorA);
					}

					if (_canObjectDraw == 0x0000)
					{
						auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
						auto _offsetValue = 85;

						if (_fetchMemory)
							memcpy(&_offsetValue, _fetchMemory, 0x04);

						auto _fetchAspect = *reinterpret_cast<int*>(Sprite + 0x1DC);
						auto _applyAspect = _fetchAspect == 0x00 ? 0x00 : (_fetchAspect > 0x00 ? _offsetValue : _offsetValue * -1);

						auto _objectStart = Sprite + 0x0020;

						auto _fetchCurrentX = *reinterpret_cast<int*>(Sprite + 0x01C8) + *reinterpret_cast<int*>(Sprite + 0x01C0);
						auto _fetchCurrentY = *reinterpret_cast<int*>(Sprite + 0x01CC) + *reinterpret_cast<int*>(Sprite + 0x01C4);

						*reinterpret_cast<int*>(_objectStart + 0x018C) = _fetchCurrentX + _applyAspect;
						*reinterpret_cast<int*>(_objectStart + 0x0190) = _fetchCurrentY;

						YI::SEQUENCE::Draw(Sprite + 0x0020);
					}

				}

				static void create(char* Sprite, int priority, char* seqd, char* image, int num, int group, int offset16x9)
				{
					dk::Obj2D::create(Sprite, priority, group);
					dk::Sprite::initWork(Sprite);

					if (seqd && image)
					{
						YI::SEQUENCE::Init(Sprite + 0x20, seqd, image);

						*reinterpret_cast<uint32_t*>(Sprite + 0x1D4) = num;
						YI::SEQUENCE::SetNumberForce(Sprite + 0x20, num);

						*(Sprite + 0x1F0) = 0x00;
						*reinterpret_cast<uint32_t*>(Sprite + 0x1DC) = offset16x9;
						*reinterpret_cast<uint32_t*>(Sprite + 0x1D8) = UINT32_MAX;
					}
				}

			private:
				static bool _init()
				{
					RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x4D\x8B\xF0\x49\x8B\xE9", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(create), 0x106);
					RedirectFunction("\x40\x53\x48\x83\xEC\x30\x8B\x41\x10\x48\x8B\xD9\xC1\xE8\x0B\xA8\x01", "xxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(draw), 0x1FD);
					return true;
				}

				#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
				static inline bool _doInit = _init();
				#endif
		};
	}
}