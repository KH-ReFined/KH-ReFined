#include "enemy.h"

char* YS::ENEMY::LastAttacker = ResolveRelativeAddress<char*>("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x89\x38\x0D\x00\x00\xE8", "xxxxxxxxxxxxxxxx", 0x34);
char*(*ENEMYBASE_OVR_DEAD)(char* enemy) = ResolveRelativeAddress<char*(*)(char*)>("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x89\x38\x0D\x00\x00\xE8", "xxxxxxxxxxxxxxxx", 0x94);

YS::ENEMY::staticInitializer YS::ENEMY::initialize;

char* YS::ENEMY::_OVR__dead(char* enemy)
{
	auto _enemyPartNum = YS::OBJ::get_part_num(enemy);

	auto _attackerObjPtr = *reinterpret_cast<uint32_t*>(enemy + 0xD38);
	auto _attackerObj = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_attackerObjPtr));

	if (YS::OBJ::is_exist(_attackerObj))
	{
		YS::ENEMY::LastAttacker = _attackerObj;
		auto _attackerFlags = *reinterpret_cast<uint32_t*>(_attackerObj + 0x6C8);

		if (_attackerFlags & 0x80)
			YS::FORM_LEVEL::CheckWisdomAndFinal(_enemyPartNum, _attackerObj);
	}

	YS::SAVERAM_BATTLE::inc_kill_count(AREA::SaveData + 0x24F0, _enemyPartNum);
	return ENEMYBASE_OVR_DEAD(enemy);
}