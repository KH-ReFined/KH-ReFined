#include "level_up.h"
 
// I am NOT making 4 seperate classes for what is essentially the same function.

void dk::LEVEL_UP::draw(char* levelUp)
{
	auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
	auto _offsetValue = 85;

	if (_fetchMemory)
		memcpy(&_offsetValue, _fetchMemory, 0x04);

	char _messageMemory[0x300];

	if (*reinterpret_cast<int*>(levelUp + 0x021C) == 0x00)
		dk::Sprite::draw(levelUp);

	dk::SpriteMessage::drawMessage(levelUp);

	auto _activeX = YI::SEQUENCE::GetActiveX(levelUp + 0x0020) + *reinterpret_cast<int*>(levelUp + 0x01C0);
	auto _activeY = YI::SEQUENCE::GetActiveY(levelUp + 0x0020) + *reinterpret_cast<int*>(levelUp + 0x01C4);

	auto _activeRGBA = *reinterpret_cast<uint32_t*>(levelUp + 0x0208) & 0xFFFFFF | YI::SEQUENCE::GetActiveRGBA(levelUp + 0x0020) & 0xFF000000;

	auto _msgSize = *reinterpret_cast<uint32_t*>(levelUp + 0x020C);

	auto _messageName = *reinterpret_cast<char**>(levelUp + 0x0220);
	auto _messageReward = *reinterpret_cast<char**>(levelUp + 0x0228);

	auto _paramEx = YI::SEQUENCE::GetParamEx(levelUp + 0x0020);

	YS::MESSAGEDRAW::_MESSAGEDRAW(_messageMemory, 0x01, _messageName, _msgSize, 0x01);
	YS::MESSAGEDRAW::draw(_messageMemory, _activeX + (_activeX > 255 ? _offsetValue : _offsetValue * -1), _activeY, _activeRGBA);

	if (*reinterpret_cast<uint32_t*>(levelUp + 0x0234) != 0x00 || _activeX > 255)
	{
		char _messageBuffer[0x10];
		auto _sprintMessage = YS::MESSAGE::Sprintf(_messageBuffer, "%2d", *reinterpret_cast<uint32_t*>(levelUp + 0x0230));

		YS::MESSAGEDRAW::_MESSAGEDRAW(_messageMemory + 0x0100, 0x01, _sprintMessage, _msgSize, 0x00);
		YS::MESSAGEDRAW::draw(_messageMemory + 0x0100, _activeX + _paramEx + (_activeX > 255 ? _offsetValue : _offsetValue * -1), _activeY, _activeRGBA);
	}

	if (_messageReward)
	{
		auto _msgFont = *reinterpret_cast<uint32_t*>(levelUp + 0x0210);

		auto _paramX = YI::SEQUENCE::GetParamX(levelUp + 0x0020);
		auto _paramY = YI::SEQUENCE::GetParamY(levelUp + 0x0020);

		YS::MESSAGEDRAW::_MESSAGEDRAW(_messageMemory + 0x0200, _msgFont, _messageReward, _msgSize, 0x00);
		YS::MESSAGEDRAW::draw(_messageMemory + 0x0200, _activeX + _paramX + (_activeX > 255 ? _offsetValue : _offsetValue * -1), _activeY + _paramY + 17, _activeRGBA);
	}
}

dk::LEVEL_UP::staticInitializer initialize;