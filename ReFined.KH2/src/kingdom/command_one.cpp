#include "command_one.h"

void dk::COMMAND_ONE::draw(char* Command)
{ 
	dk::Sprite::draw(Command);
	dk::SpriteMessage::drawMessage(Command);

	if (*reinterpret_cast<int*>(Command + 0x03C0) > 1)
	{
		auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
		auto _offsetValue = 85;

		if (_fetchMemory)
			memcpy(&_offsetValue, _fetchMemory, 0x04);

		auto _applyAspect = (_offsetValue * -1);

		*reinterpret_cast<int*>(Command + 0x018C + 0x0220) = YI::SEQUENCE::GetActiveX(Command + 0x0020) + *reinterpret_cast<int*>(Command + 0x01C0) + YI::SEQUENCE::GetParamX(Command + 0x0020) + _applyAspect;
		*reinterpret_cast<int*>(Command + 0x0190 + 0x0220) = YI::SEQUENCE::GetActiveY(Command + 0x0020) + *reinterpret_cast<int*>(Command + 0x01C4) + YI::SEQUENCE::GetParamY(Command + 0x0020) ;

		YI::SEQUENCE::Draw(Command + 0x0220);
	}
}

dk::COMMAND_ONE::staticInitializer initialize;