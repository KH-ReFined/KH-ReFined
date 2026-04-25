#include "itemshop.h"

void OvlShop::ItemShop::GetFaceFileNmae(int part, char* buff)
{
	auto _fetchShopface = YS::PANACEA_ALLOC::Get("00shopface.bin");

	if (_fetchShopface)
	{
		auto _fetchPartId = YS::MEMBER::PartToEntryID(part);
		auto _fetchCount = *reinterpret_cast<uint32_t*>(_fetchShopface);

		for (int i = 0; i < _fetchCount; i++)
		{
			auto _fetchEntryId = *reinterpret_cast<uint16_t*>(_fetchShopface + 0x10 + 0x10 * i);

			if (_fetchEntryId == _fetchPartId)
			{
				sprintf(buff, "menu/shopface/%s.bin", _fetchShopface + 0x12 + 0x10 * i);
				return;
			}
		}
	}
}

OvlShop::ItemShop::staticInitializer OvlShop::ItemShop::initialize;