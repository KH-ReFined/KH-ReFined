#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "obj.h"
#include "fade.h"
#include "sora.h"
#include "member.h"
#include "friend.h"
#include "member_table.h"
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SACRIFICE
		{
		public:
			static inline int(*GetFormStatus)(int form) = FindSignature<int(*)(int)>("\x40\x56\x57\x41\x56\x48\x83\xEC\x20\x8B\xF9\x45\x33\xF6\x33\xF6", "xxxxxxxxxxxxxxxx");
			static inline void(*StartPart)(int part, float time) = FindSignature<void(*)(int, float)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x30\x0F\x29\x74\x24\x20\x8B\xF9\x0F\x28\xF1\xE8", "xxxxxxxxxxxxxxxxxxxxx");

			static char* Return(int part)
			{
				auto _fetchMemberPart = YS::MEMBER::PartToMemberPart(part);

				if (!YS::OBJ::is_exist(*YS::SORA::Sora))
					return nullptr;

				auto _fetchMemberTable = YS::MEMBER::Get(UINT32_MAX);

				for (int i = 0x00; i <= 0x03; i++)
				{
					if (i == 0x03)
						return nullptr;

					auto _fetchCurrMember = *reinterpret_cast<uint8_t*>(_fetchMemberTable + i);

					if ((_fetchCurrMember & 0x1F) == _fetchMemberPart)
						break;
				}

				auto _fetchFriendId = reinterpret_cast<uint16_t*>(YS::MEMBER_TABLE::MemberTable + 0x02 * _fetchMemberPart);
				auto _fetchFriend = YS::FRIEND::AppearNearPlayer(*_fetchFriendId, nullptr, 100.0, 200.0, 300.0);

				if (_fetchFriend)
				{
					auto _fetchFadeFlag = *reinterpret_cast<uint32_t*>(_fetchFriend + 0x09B8) & 0x10;

					if (!_fetchFadeFlag)
						YS::FADE::start(_fetchFriend + 0x0A08, 0.0, 1.0, 15.0);
				}

				return _fetchFriend;
			}
		};
	}
}