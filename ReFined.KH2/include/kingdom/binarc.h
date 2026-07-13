#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define _CRT_SECURE_NO_WARNINGS

#include "axa.h"
#include "area.h"
#include "file.h"
#include "region.h"
#include "converter.h"
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT BINARC
		{
		public:
			static inline void (*modelInitVB)(char* readAddr, bool doProcess) = FindSignature<void(*)(char*, bool)>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8D\xB9\x90\x00\x00\x00", "xxxxxxxxxxxxxxxxx");
            static inline void (*modelCreateVB)(char* readAddr, int kind, char* param) = FindSignature<void(*)(char*, int, char*)>("\x83\xFA\xFF\x0F\x84\xA5\x00\x00\x00\x48\x89\x6C\x24\x18", "xxxxxxxxxxxxxx");

			static inline char* (*get_info_by_tag)(char* binarc, uint32_t type, uint32_t tag, int n) = FindSignature<char* (*)(char*, uint32_t, uint32_t, int)>("\x48\x89\x5C\x24\x08\x48\x89\x7C\x24\x10\x48\x63\x79\x04\x33\xDB\x48\x85\xFF\x7E\x36", "xxxxxxxxxxxxxxxxxxxxx");
			static inline uint32_t (*get_file_num)(char* binarc, uint32_t type) = FindSignature<uint32_t(*)(char*, uint32_t)>("\x4C\x63\x49\x04\x33\xC0\x44\x8B\xD2\x4D\x85\xC9\x7E\x1D\x4C\x8D", "xxxxxxxxxxxxxxxx");

            static void preSetup(char* binarc)
			{
				auto _fetchInit = *reinterpret_cast<uint32_t*>(binarc + 0x08);

				if (_fetchInit == 0x00)
				{
					auto _fetchItemCount = *reinterpret_cast<int*>(binarc + 0x04);

					if (_fetchItemCount > 0x00)
					{
						char* _paramThingie = 0x00;

						for (int i = 0; i < _fetchItemCount; i++)
						{
							auto _fetchSize = *reinterpret_cast<uint32_t*>(binarc + 0x1C + 0x10 * i);
							auto _fetchType = *reinterpret_cast<uint16_t*>(binarc + 0x10 + 0x10 * i);
							auto _fetchOffset = *reinterpret_cast<uint32_t*>(binarc + 0x18 + 0x10 * i);
							auto _fetchDuplicate = (*reinterpret_cast<uint16_t*>(binarc + 0x12 + 0x10 * i) & 1) == 0x00;

							if (_fetchSize && _fetchType == 0x05 && _fetchDuplicate)
								_paramThingie = binarc + _fetchOffset;
						}

						for (int i = 0; i < _fetchItemCount; i++)
						{
							auto _fetchType = *reinterpret_cast<uint16_t*>(binarc + 0x10 + 0x10 * i);
							auto _fetchSize = *reinterpret_cast<uint32_t*>(binarc + 0x1C + 0x10 * i);
							auto _fetchOffset = *reinterpret_cast<uint32_t*>(binarc + 0x18 + 0x10 * i);
							auto _fetchDuplicate = (*reinterpret_cast<uint16_t*>(binarc + 0x12 + 0x10 * i) & 1) != 0x00;

							if (_fetchSize != 0x00 && !_fetchDuplicate)
							{
								auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

								string _encodedRegion = _fetchConfig & 0x0004 ? "jp" :
									(_fetchConfig & 0x0008 ? "es" :
										(_fetchConfig & 0x0010 ? "de" :
											(_fetchConfig & 0x0020 ? "bg" : "us")));

								auto _fetchAbsolute = reinterpret_cast<char*>(binarc + _fetchOffset);

								switch (_fetchType)
								{
								case 0x01: // Binary Archive [BAR]
								case 0x11: // Animation Binary Archive [ANB] 
								case 0x14: // Motion Set [MSET]
								case 0x1E: // Map Archive [MAP]
								case 0x2E: // Binary Archive [???]
									YS::BINARC::preSetup(_fetchAbsolute);
									break;
								case 0x04: // VIF Packets [VIF]
								{
									YS::BINARC::modelInitVB(_fetchAbsolute, (*(binarc + 0x03) & 0xC0) == 0x80);
									YS::BINARC::modelCreateVB(_fetchAbsolute, 0x00, *reinterpret_cast<uint32_t*>(binarc + 0x10 * i + 0x14) == 0x50414D ? _paramThingie : nullptr);
								} break;
								case 0x1F: // Sound Effect Block [SEB]
								case 0x22: // IopVoice Sound [VSB]
								case 0x30: // SONY Audio Format [VAG]
								{
									char _readOrigin[0x07];
									char _readPath[0x80];

									memcpy(_readOrigin, _fetchAbsolute, 0x07);

									if (strcmp(_readOrigin, "ORIGIN") == 0x00)
									{
										auto _fetchName = string(_fetchAbsolute + 0x10);

										if (_fetchName.find("voice/battle") != string::npos)
										{
											auto _fetchSlash = strrchr(_fetchName.c_str(), '/');

											sprintf(_readPath, "voice/%s/battle/%s.win32.scd", _encodedRegion.c_str(), _fetchSlash + 0x01);
											auto _fetchSize = YS::FILE::GetSize(_readPath);

											if (_fetchSize)
											{
												auto _allocMemory = (char*)malloc(_fetchSize);
												auto _fetchLoad = YS::FILE::Read(_readPath, _allocMemory);

												*reinterpret_cast<char**>(_fetchAbsolute + 0x20) = _allocMemory;
												*reinterpret_cast<size_t*>(_fetchAbsolute + 0x28) = _fetchSize;

												*reinterpret_cast<char*>(_fetchAbsolute + 0x1F) = 0x01;
											}

											else
											{
												sprintf(_readPath, "voice/us/battle/%s.win32.scd", _fetchSlash + 0x01);
												_fetchSize = YS::FILE::GetSize(_readPath);

												if (_fetchSize)
												{
													auto _allocMemory = (char*)malloc(_fetchSize);
													auto _fetchLoad = YS::FILE::Read(_readPath, _allocMemory);

													*reinterpret_cast<char**>(_fetchAbsolute + 0x20) = _allocMemory;
													*reinterpret_cast<size_t*>(_fetchAbsolute + 0x28) = _fetchSize;

													*reinterpret_cast<char*>(_fetchAbsolute + 0x1F) = 0x01;
												}

												else
												{
													*reinterpret_cast<char**>(_fetchAbsolute + 0x20) = nullptr;
													*reinterpret_cast<size_t*>(_fetchAbsolute + 0x28) = 0x00;
												}
											}
										}

										else
										{
											auto _fetchSlash = strrchr(_fetchName.c_str(), '/');
											auto _findSlash = _fetchName.find(_fetchSlash);

											sprintf(_readPath, "C:/hd28/%s/juefigs/KH2Replace/KH2-Sound/little/%s/", IsLibraryLinked(L"steam_api64.dll") ? "STEAM" : "EPIC", _fetchName.substr(0x00, _findSlash).c_str());

											auto _fetchResourceEntry = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(binarc + 0x0C) ^ (*reinterpret_cast<uint32_t*>(binarc + 0x0C) ^ (*reinterpret_cast<uint8_t*>(binarc + 0x03) >> 0x04)) & 0x03);
											auto _fetchReplaceEntry = Axa::ResourceEntry::ReadDataLocal(_fetchResourceEntry, _fetchResourceEntry + 0x620, _readPath, _fetchAbsolute + 0x10);

											if (_fetchReplaceEntry)
											{
												*reinterpret_cast<char**>(_fetchAbsolute + 0x20) = *reinterpret_cast<char**>(_fetchReplaceEntry + 0x60);
												*reinterpret_cast<int*>(_fetchAbsolute + 0x28) = *reinterpret_cast<int*>(_fetchReplaceEntry + 0x50);

												*reinterpret_cast<char*>(_fetchAbsolute + 0x1F) = 0x01;
											}

											else
											{
												*reinterpret_cast<char**>(_fetchAbsolute + 0x20) = nullptr;
												*reinterpret_cast<int*>(_fetchAbsolute + 0x28) = 0x00;
											}
										}
									}
								} break;
								}
							}
						}
					}
				}
			}

			private:
				static bool _init()
				{
					RedirectFunction("\x41\x56\x48\x81\xEC\x00\x01\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxx", reinterpret_cast<uint64_t>(preSetup), 0x34A);
					return true;
				}

				#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
				static inline bool _doInit = _init();
				#endif
		};
	}
}