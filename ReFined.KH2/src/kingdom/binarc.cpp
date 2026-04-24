#include "binarc.h"

YS::BINARC::modelInitVB_t YS::BINARC::modelInitVB = SignatureScan<YS::BINARC::modelInitVB_t>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8D\xB9\x90\x00\x00\x00", "xxxxxxxxxxxxxxxxx");
YS::BINARC::modelCreateVB_t YS::BINARC::modelCreateVB = SignatureScan<YS::BINARC::modelCreateVB_t>("\x83\xFA\xFF\x0F\x84\xA5\x00\x00\x00\x48\x89\x6C\x24\x18", "xxxxxxxxxxxxxx");

YS::BINARC::get_info_by_tag_t YS::BINARC::get_info_by_tag = SignatureScan<YS::BINARC::get_info_by_tag_t>("\x48\x89\x5C\x24\x08\x48\x89\x7C\x24\x10\x48\x63\x79\x04\x33\xDB\x48\x85\xFF\x7E\x36", "xxxxxxxxxxxxxxxxxxxxx");

void YS::BINARC::preSetup(char* binarc)
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

					string _encodedRegion =  _fetchConfig & 0x0004 ? "jp" :
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
						case 0x04:
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

									sprintf(_readPath, "C:/hd28/STEAM/juefigs/KH2Replace/KH2-Sound/little/%s/", _fetchName.substr(0x00, _findSlash).c_str());

									auto _fetchResourceEntry = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(binarc + 0x0C) ^ (*reinterpret_cast<uint32_t*>(binarc + 0x0C) ^ (*reinterpret_cast<uint8_t*>(binarc + 0x03) >> 0x04)) & 0x03));
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

YS::BINARC::staticInitializer initialize;