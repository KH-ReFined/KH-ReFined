#include "treasure_info.h"
#include "SigScan.h"
#include <Windows.h>

dk::TREASURE_INFO::openPrizeWindow_t dk::TREASURE_INFO::openPrizeWindow = SignatureScan<dk::TREASURE_INFO::openPrizeWindow_t>("\x40\x53\x48\x83\xEC\x20\x48\x8B\x15\x00\x00\x00\x00\x48\x8B\xD9\x4C\x63\x82\x00\x00\x00\x00", "xxxxxxxxx????xxxxxx????");

void dk::TREASURE_INFO::openBoxWindow(const char* message, uint16_t image)
{
	using _processItemInfo = void(*)();

	auto PROCESS_ITEMINFO = SignatureScan<_processItemInfo>("\x48\x63\x15\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0C\x52\xFF\xC2\x81\xE2\x07\x00\x00\x80\x7D\x07", "xxx????xxx????xxxxxxxxxxxxxx");
	char* ITEMINFO_QUEUE = ResolveRelativeAddress<char*>("\x48\x63\x15\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0C\x52\xFF\xC2\x81\xE2\x07\x00\x00\x80\x7D\x07", "xxx????xxx????xxxxxxxxxxxxxx", 0x03) - 0x04;

	YS::ITEMPIC::ReadImage(image);

	uint8_t _currentCount = *reinterpret_cast<const uint8_t*>(ITEMINFO_QUEUE + 0x04);
	uint8_t _treasureCount = _currentCount + 0x01;

	if (_currentCount >= 0x07)
	{
		uint8_t _emptyCount = 0x00;
		vector<uint8_t> _emptyArray(0x18);

		memcpy(ITEMINFO_QUEUE, &_emptyCount, 0x01);
		memcpy(ITEMINFO_QUEUE + 0x04, &_emptyCount, 0x01);

		for (int i = 0; i < 0x07; i++)
			memcpy(ITEMINFO_QUEUE + (0x08 + 0x18 * i), _emptyArray.data(), 0x18);

		_currentCount = 0x00;
		_treasureCount = 0x01;
	}

	uint64_t _messageAddr = reinterpret_cast<uint64_t>(message);
	uint64_t _messageOffset = _messageAddr + 0x20;

	memcpy(ITEMINFO_QUEUE, &_treasureCount, 0x01);
	memcpy(ITEMINFO_QUEUE + 0x04, &_currentCount, 0x01);

	memcpy(ITEMINFO_QUEUE + (0x08 + 0x18 * _currentCount), &_messageAddr, 0x08);
	memcpy(ITEMINFO_QUEUE + (0x08 + 0x18 * _currentCount) + 0x08, &_messageOffset, 0x08);

	memcpy(ITEMINFO_QUEUE + (0x08 + 0x18 * _currentCount) + 0x10, &image, 0x02);

	PROCESS_ITEMINFO();

	YS::ITEMPIC::FreeImageData(YS::ITEMPIC::ImageBuff);
}