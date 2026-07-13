#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "sora.h"
#include "friend.h"
#include "converter.h"
#include "member.h"
#include "weapon_entry.h"
#include "weapon_mset.h"
#include "weapon.h"
#include "objentry.h"
#include "panacea_alloc.h"
#include "sheet.h"
#include "cache_buff.h"
#include "file.h"
#include "pax.h"
#include "area.h"
#include "field.h"
#include "item.h"
#include "sound.h"
#include "memorymgr.h"
#include "member_table.h"

extern "C"
{ 
	namespace YS
	{
		class DLL_EXPORT PARTY
		{
		protected:
			static inline bool THREAD_RUNNING;
			static inline uint8_t CHECK_VSB_THROUGHPUT;

			static inline char* ALLOC_VSB = nullptr;
			static inline char* ACTIVE_VOICE_COUNT = FetchRelativePointer<char*>("\x40\x56\x57\x41\x56\x48\x83\xEC\x40\x48\xC7\x44\x24\x30\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x60", "xxxxxxxxxxxxxxxxxxxxxxx", 0x171);

		public:
			static inline void(*SetWeapon)(char* party_ptr, int entry_id, int hand) = FindSignature<void(*)(char*, int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x8B\x81\xC8\x06\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*ExecuteCommand)(char* playerObject, char* command) = FindSignature<void(*)(char*, char*)>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x30\x81\xA1\x24\x01\x00\x00", "xxxxxxxxxxxxxxxx");
			static inline char* (*get_partram)(char* party_ptr) = FindSignature<char*(*)(char*)>("\x48\x8B\x81\xC0\x05\x00\x00\x8B\x88\x50\x02\x00\x00\xE9", "xxxxxxxxxxxxxx");

			static inline char* KeybladePAX = FetchRelativePointer<char*>("\x48\x8B\xD1\x4C\x8D\x05\x00\x00\x00\x00\x4C\x8D\x0D\x00\x00\x00\x00\x49\x8B\x00\x48\x85\xC0\x74\x18\x0F\x1F\x80\x00\x00\x00\x00\x48\x39\x10\x74\x1B\x48\x8B\x48\x20\x48\x8B\xC1\x48\x85\xC9", "xxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x10A);

			static void ChangeWeapon(char* task, int part, bool hand_secondary, int item, bool playPax = true)
			{
				// If the function was NOT called by the task manager. This is to ensure that it executes the queue system.
				if (task == nullptr)
				{
					auto _taskPtr = reinterpret_cast<void(*)(char*)>(&YS::PARTY::ChangeWeapon);

					// Loop through the queue system. 
					// Basically, within CHANGE_WEAPON_QUEUE we can have 0x14 commands that the task will iterate through.
					// The task will execute all of them whenever possible.

					for (int i = 0; i <= 0x14; i++)
					{
						// If the code reaches this point, it means the queue is full and we cannot fill it anymore.
						// The code cannot run.
						if (i == 0x14)
							return;

						// Push all arguments to a vector.
						vector<int> _argArray{ part, hand_secondary, item, playPax };

						// Fetch the pointer to the first argument within the queue.
						auto _partPtr = YS::PANACEA_ALLOC::Get("CHANGE_WEAPON_QUEUE") + (i * 0x10);

						// If it is empty (PART argument CANNOT be 0x00):
						if (*_partPtr == 0x00)
						{
							// Copy the arguments to the queue in the appropriate place.
							memcpy(YS::PANACEA_ALLOC::Get("CHANGE_WEAPON_QUEUE") + (i * 0x10), _argArray.data(), 0x10);
							break;
						}
					}

					// If the ChangeWeapon task does not exist within FIELD::TaskManager, make it exist.
					if (!YS::FIELD::TaskExists(_taskPtr))
						YS::FIELD::CreateThread(0, 140000, _taskPtr, 8096);
				}

				// If the function WAS called by a task manager, we get into the fun stuff: 
				else
				{
					// Through the entire queue;
					for (int _queueIterator = 0x00; _queueIterator < 0x14; _queueIterator++)
					{
						// Fetch the first element of the element within the queue.
						auto _partPtr = YS::PANACEA_ALLOC::Get("CHANGE_WEAPON_QUEUE") + (0x10 * _queueIterator);

						// If it is empty, carry on.
						if (*_partPtr == 0x00)
							continue;

						// Copy the arguments to the current stack to be processed.
						vector<int> _currentStack(0x04);
						memcpy(_currentStack.data(), YS::PANACEA_ALLOC::Get("CHANGE_WEAPON_QUEUE") + (0x10 * _queueIterator), 0x10);

						bool _wpnParsed = false;

						char* _charPtr = nullptr;
						bool is_hide = false;

						int _wpnBank = 0;
						int _wpnPart = 0;
						int _wpnPriority = 0;

						bool _wpnHide = false;

						// For every possible slot that *may* or *may not* contain a party member;
						for (int _charIterator = 0; _charIterator <= 3; _charIterator++)
						{
							if (_charIterator == 0x03 && _currentStack[0] == 0x01)
								_charPtr = *YS::SORA::Sora;

							// Else, break out. We will parse the character we need another way.
							else if (_charIterator == 0x03)
								break;

							// Fetch the pointer in which the party member is initialized.
							_charPtr = _charIterator == 0x00 ? *YS::SORA::Sora : *reinterpret_cast<char**>(YS::FRIEND::Friend + 0x08 * (_charIterator - 1));

							// If the pointer does not exist, kindly continue.
							if (_charPtr == nullptr)
								continue;

							// Fetch the WEAPON_PART variable.
							_wpnPart = *reinterpret_cast<int8_t*>(PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_charPtr + 0x08)) + 0x4C);

							// If the WEAPON_PART parsed matches the WEAPON_PART requested, we have our target party member.
							if (_wpnPart == _currentStack[0])
								break;
						}

						// If we have a hit on the party member;
						if (_charPtr != nullptr)
						{
							// Fetch the pointers for the weapon and its function space.
							auto _weaponPtr = *reinterpret_cast<char**>(_charPtr + 0x08 * _currentStack[1] + 0x0D60);
							int* _weaponInt = reinterpret_cast<int*>(_weaponPtr);

							// If the int version of the weapon pointer does not exist (this also means the weapon pointer doesn't exist): Continue.
							if (_weaponInt == nullptr)
								continue;

							char* _weaponAddr = PC::CONVERTER::INTPTR_TO_POINTER(*_weaponInt);
							uint64_t* _weaponAddrPtr = reinterpret_cast<uint64_t*>(_weaponAddr);

							// If the pointers do not exist (Part 2!) or are flat out wrong: Continue.
							if (_weaponPtr == nullptr || reinterpret_cast<int64_t>(_weaponPtr) == -1)
								continue;

							// Fetch the priority and see if it should be hidden or not.
							_wpnPriority = *reinterpret_cast<uint32_t*>(_weaponPtr + 0x0C0C);
							_wpnHide = (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x124) & 0x180) != 0x00 || (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x6C8) & 0x44) == 0x00 || (*reinterpret_cast<uint64_t*>(_weaponPtr + 0x120) & 0x400) != 0x00;

							// Fetch the weapon bank, set to -1 if it is not defined or otherwise can't be accessed.

							auto _wpnBankAddr = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_weaponPtr + 0x0BB0));

							// If the weapon bank address exists, get the weapon bank ID.
							if (_wpnBankAddr != 0x00)
								_wpnBank = *reinterpret_cast<uint16_t*>(_wpnBankAddr);

							// Else, default to -1.
							else
								_wpnBank = 0xFFFF;

							// Prepare the OBJ for destruction.
							reinterpret_cast<void(*)(char*, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x48))(_weaponAddr, _weaponInt); // Compresses the VIF packages(?).

							// This essentially reimplements YS::OBJ::destroy(). Why? Why not.
							if ((*reinterpret_cast<uint64_t*>(_weaponPtr + 0x9B8) & 0x40) == 0x00)
							{
								reinterpret_cast<void(*)(char*, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x30))(_weaponAddr, _weaponInt);
								*reinterpret_cast<uint64_t*>(_weaponPtr + 0x9B8) |= 0x40;
							}

							// Erase the weapon pointer.
							*reinterpret_cast<uint64_t*>(_charPtr + 0x08 * _currentStack[1] + 0x0D60) = 0x00;

							// Parse the current weapon part from the parsed character.
							_wpnPart = *reinterpret_cast<int8_t*>(PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_charPtr + 0x08)) + 0x4E);

							// Check if the weapon targeted is Sora's MAIN weapon;
							if (_wpnPart != 0x35 && !_currentStack[1] && (*reinterpret_cast<uint8_t*>(_charPtr + 0x06CB) & 0x01) != 0x00)
							{
								// Parse Sora's current costume from the objentry, then parse the WEAPON_PART accordingly.
								auto _object = YS::SORA::GetEntryID(0x00);
								_wpnPart = *reinterpret_cast<uint16_t*>(YS::OBJENTRY::Get(_object) + 0x4E);
							}
						}

						// Otherwise;
						else
						{
							// Fetch the current object from WEAPON_PART and its priority.
							auto _object = YS::MEMBER::PartToEntryID(_currentStack[0]);
							auto _priority = YS::OBJENTRY::GetFriendPriority(_object);

							// If the priority is larger than 0x096A;
							if (_priority > 0x096A)
							{
								// If it is *specifically* 0x0974, we return as it is not a correct parse.
								if (_priority != 0x0974)
									continue;

								// Otherwise, Weapon Bank is 0x06.
								_wpnBank = 0x06;
							}

							// Otherwise;
							else
							{
								// If it is *smaller* than 0x0969, we return as it is not a correct parse.
								if (_priority <= 0x0969)
									continue;

								// Otherwise, Weapon Bank is 0x05.
								_wpnBank = 0x05;
							}

							// Apply the WEAPON_PART and Priority accordingly. A Party member's priority is always their own + 1.
							_wpnPriority = _priority + 1;
							_wpnPart = *reinterpret_cast<uint16_t*>(YS::OBJENTRY::Get(_object) + 0x4E);
						}

						// Destroy all files with the given priority from the CACHE_BUFF.
						YS::CACHE_BUFF::DestroyPriority(_wpnPriority);

						auto _fetchWeaponID = YS::WEAPON_ENTRY::Get(_wpnPart, _currentStack[2]);

						// Request reading the files necessary for the current weapon. 
						// This will wait until the flush is complete.

						YS::OBJENTRY::ReadRequestWeapon(_wpnPart, _currentStack[1], _fetchWeaponID, _wpnPriority, _wpnBank);
						YS::CACHE_BUFF::Flush(task);

						if (!*AREA::IsInMap)
							return;

						if (!_wpnHide && _currentStack[3])
						{
							auto _sizeVSB = YS::FILE::GetSize("se/zz00_keyswitch.win32.scd");

							if (_sizeVSB != 0x00)
							{
								if (!ALLOC_VSB)
								{
									ALLOC_VSB = (char*)malloc(_sizeVSB);
									YS::FILE::Read("se/zz00_keyswitch.win32.scd", ALLOC_VSB);
								}

								if (*ACTIVE_VOICE_COUNT == 0x0F)
									SOUND::StreamAllStop(true);

								SOUND::PlayVSB(ALLOC_VSB, _sizeVSB, 0x3FAC, 0x00);
							}
						}

						// Get the target weapon's objentry and initialize swap.
						auto _wpnEntry = YS::WEAPON_ENTRY::Get(_wpnPart, _currentStack[2]);
						YS::PARTY::SetWeapon(_charPtr, _wpnEntry, _currentStack[1]);

						// Fetch the necessary pointers.

						auto _weaponPtr = *reinterpret_cast<char**>(_charPtr + 0x08 * _currentStack[1] + 0x0D60);
						int* _weaponInt = reinterpret_cast<int*>(_weaponPtr);

						if (_weaponInt == nullptr)
							continue;

						char* _weaponAddr = PC::CONVERTER::INTPTR_TO_POINTER(*_weaponInt);
						uint64_t* _weaponAddrPtr = reinterpret_cast<uint64_t*>(_weaponAddr);

						// If the weapon needs to be shown, set the parameter.
						if (!_wpnHide)
							reinterpret_cast<void(*)(char*, int*)>(*reinterpret_cast<uint64_t*>(*_weaponAddrPtr + 0x88))(_weaponAddr, _weaponInt);

						if (_currentStack[3])
						{
							auto _parsePaxPtr = *reinterpret_cast<char**>(ryj::PAX::PaxList) - 0x80;
							ryj::PAX::Begin(_parsePaxPtr, 0x00, 0x01, 0x00, 0x00);
						}

						// I do not know how to execute this for others, so it is for Sora only at this moment.

						if (_currentStack[0] == 0x01)
						{
							YS::SHEET::Save(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308);
							YS::SHEET::Load(YS::MEMBER_TABLE::MemberStatsAnchor + 0xC308, nullptr);
						}

						// The queued request has successfully been processed. Clear it.
						fill(YS::PANACEA_ALLOC::Get("CHANGE_WEAPON_QUEUE") + (0x10 * _queueIterator), YS::PANACEA_ALLOC::Get("CHANGE_WEAPON_QUEUE") + (0x10 * _queueIterator) + 0x10, 0x00);
					}
				}
			}
		};
	}
}