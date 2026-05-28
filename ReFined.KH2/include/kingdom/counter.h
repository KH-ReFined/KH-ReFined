#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "obj2d.h"
#include "limit.h"
#include "sprite.h"
#include "sequence.h"
#include "memorymgr.h"
#include "info_base.h"
#include "cache_buff.h"
#include "information.h"
#include "mission_count.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT COUNTER
		{
		public:
			static void(*forceLeave)(char* counter);

			static void update(char* counter)
			{
				dk::INFO_BASE::update(counter);

				auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
				auto _offsetValue = 85;

				if (_fetchMemory)
					memcpy(&_offsetValue, _fetchMemory, 0x04);

				auto _fetchMission = YS::CACHE_BUFF::SearchByName("msn/us/HB33_FM_LEX.bar", -1) && *reinterpret_cast<int*>(counter + 0x1CB8) == 0x05;

				if (_fetchMission && _offsetValue != 85)
					_offsetValue = 85;

				auto _applyAspect = 0x00;

				auto _fetchActiveX = YI::SEQUENCE::GetActiveX(counter + 0x160);
				auto _fetchNumber = *reinterpret_cast<uint32_t*>(counter + 0x160 + 0x158);

				switch (_fetchNumber)
				{
				case 0x24:
					_applyAspect = _offsetValue * -1;
					break;
				case 0x25:
					_applyAspect = _offsetValue;
					break;
				default:
					_applyAspect = 0x00;
					break;
				}

				*reinterpret_cast<int*>(counter + 0x0028) = _applyAspect + _fetchActiveX;

				if (*reinterpret_cast<uint32_t*>(counter + 0x1CC0) == 0x01)
				{
					if (!dk::Obj2D::isExist(counter + 0x0300))
						dk::COUNTER::forceLeave(counter);
				}

				else
				{
					dk::Sprite::setPos(counter + 0x0300, *reinterpret_cast<int*>(counter + 0x0028), *reinterpret_cast<int*>(counter + 0x002C));

					auto _comboCurrent = **reinterpret_cast<int**>(counter + 0x1C98);
					auto _maxComboCurrent = **reinterpret_cast<int**>(counter + 0x1CA8);
					*reinterpret_cast<int*>(counter + 0x1CA0) = _comboCurrent;
					*reinterpret_cast<int*>(counter + 0x1CB0) = _maxComboCurrent;

					auto _comboDisplay = *reinterpret_cast<int*>(counter + 0x1CA4);
					auto _maxComboDisplay = *reinterpret_cast<int*>(counter + 0x1CB4);

					if (_comboDisplay != _comboCurrent)
					{
						auto _articleCount = *reinterpret_cast<int*>(counter + 0x1CB8) - 1;

						char _numSeqDictionary[0x0A] = { 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };
						char _arraySequenceNum[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

						sprintf_s(_arraySequenceNum, 0x06, "%d", _comboCurrent);

						vector<char> _arrayFinalSequence;

						for (int i = 0; i < 6; i++)
						{
							if (!_arraySequenceNum[i])
								_arrayFinalSequence.push_back(0x0C);

							else
							{
								auto _fetchArr = _numSeqDictionary[_arraySequenceNum[i] - 0x30];
								_arrayFinalSequence.insert(_arrayFinalSequence.begin(), _fetchArr);
							}
						}

						for (int i = _articleCount; i >= 0; i--)
						{
							*reinterpret_cast<uint32_t*>(counter + 0x04D4 + 0x01F8 * (i + 2)) = _arrayFinalSequence[i];
							YI::SEQUENCE::SetNumberForce(counter + 0x0320 + 0x01F8 * (i + 2), _arrayFinalSequence[i]);

							*(counter + 0x04F0 + 0x01F8 * (i + 2)) = 0x00;

							*reinterpret_cast<uint32_t*>(counter + 0x0310 + 0x01F8 * (i + 2)) &= ~0x40;
						}

						if (_articleCount >= 1)
						{
							for (int i = _articleCount; i >= 1; i--)
							{
								if (_arrayFinalSequence[i] != 0x0C)
									break;

								if (*reinterpret_cast<int*>(counter + 0x1CBC) == 0x02)
									*reinterpret_cast<uint32_t*>(counter + 0x0310 + 0x01F8 * (i + 2)) |= 0x40;

								else
								{
									*reinterpret_cast<uint32_t*>(counter + 0x04D4 + 0x01F8 * (i + 2)) = 0x16;
									YI::SEQUENCE::SetNumberForce(counter + 0x0320 + 0x01F8 * (i + 2), 0x16);

									*(counter + 0x04F0 + 0x01F8 * (i + 2)) = 0x00;
								}
							}
						}
					}

					if (_maxComboDisplay != _maxComboCurrent)
					{
						auto _articleCount = *reinterpret_cast<int*>(counter + 0x1CB8) - 1;

						char _subNumSeqDictionary[0x0A] = { 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21 };
						char _arraySequenceSubNum[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

						sprintf_s(_arraySequenceSubNum, 0x06, "%d", _maxComboCurrent);

						vector<char> _arrayFinalSequence;

						for (int i = 0; i < 6; i++)
						{

							if (!_arraySequenceSubNum[i])
								_arrayFinalSequence.push_back(0x18);

							else
							{
								auto _fetchArr = _subNumSeqDictionary[_arraySequenceSubNum[i] - 0x30];
								_arrayFinalSequence.insert(_arrayFinalSequence.begin(), _fetchArr);
							}
						}

						for (int i = _articleCount; i >= 0; i--)
						{
							*reinterpret_cast<uint32_t*>(counter + 0x04D4 + 0x01F8 * (i + 7)) = _arrayFinalSequence[i];
							YI::SEQUENCE::SetNumberForce(counter + 0x0320 + 0x01F8 * (i + 7), _arrayFinalSequence[i]);

							*(counter + 0x04F0 + 0x01F8 * (i + 2)) = 0x00;
						}

						if (_articleCount >= 1)
						{
							for (int i = _articleCount; i >= 1; i--)
							{
								if (_arrayFinalSequence[i] != 0x18)
									break;

								*reinterpret_cast<uint32_t*>(counter + 0x04D4 + 0x01F8 * (i + 7)) = 0x22;
								YI::SEQUENCE::SetNumberForce(counter + 0x0320 + 0x01F8 * (i + 7), 0x22);

								*(counter + 0x04F0 + 0x01F8 * (i + 7)) = 0x00;
							}
						}
					}
					*reinterpret_cast<int*>(counter + 0x1CA4) = *reinterpret_cast<int*>(counter + 0x1CA0);
					*reinterpret_cast<int*>(counter + 0x1CB4) = *reinterpret_cast<int*>(counter + 0x1CB0);
				}
			}
		
			private:
				static bool _init()
				{
					YS::LIMIT::destroy = FindSignature<void(*)(char*)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x09\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8", "xxxxxxxxxxxxxxxx????xxxx");

					YS::MISSION_COUNT::destroy = FetchRelativePointer<void(*)(char*)>(reinterpret_cast<char*>(YS::LIMIT::destroy), 0xF9);
					dk::INFORMATION::forceLeaveCounter = FetchRelativePointer<void(*)(char*)>(reinterpret_cast<char*>(YS::MISSION_COUNT::destroy), 0x13);
					dk::COUNTER::forceLeave = FetchRelativePointer<void(*)(char*)>(reinterpret_cast<char*>(dk::INFORMATION::forceLeaveCounter), 0x1B);

					RedirectFunction("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x55\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x50\x48\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(update), 0x38E);
					return true;
				}

				#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
				static inline bool _doInit = _init();
				#endif
		};
	}
}