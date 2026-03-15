#include "newgame.h"


char** Title::NewGame::m_SeqTbl = ResolveRelativeAddress<char**>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x17);
int* Title::NewGame::m_pri = ResolveRelativeAddress<int*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x2D);
char** Title::NewGame::m_Lay = ResolveRelativeAddress<char**>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x20);
char* Title::NewGame::s_Plate2Seq = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x25D);
char** Title::NewGame::m_SelPtr = ResolveRelativeAddress<char**>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x273);
char** Title::NewGame::m_MenuPtr = reinterpret_cast<char**>(ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x17) - 0x28);

Title::NewGame::staticInitializer Title::NewGame::initialize;

char* Title::NewGame::SetupResult()
{
    auto _fetchSeqd = *m_SeqTbl + 0x440;
    Tz::MenuUtil::CreateMess(*m_SeqTbl + 0x440, *m_pri, 0, 0, *m_Lay, 28, 29, -1, 0, 0);

    *reinterpret_cast<uint32_t*>(_fetchSeqd + 0x010) &= ~0x10;
    *reinterpret_cast<uint32_t*>(_fetchSeqd + 0x010) &= ~0x08;

    *reinterpret_cast<uint32_t*>(_fetchSeqd + 0x214) = 2;
    auto _fetchParamX = YI::SEQUENCE::GetParamX(_fetchSeqd + 0x20);
    auto _fetchParamY = YI::SEQUENCE::GetParamY(_fetchSeqd + 0x20);

    *reinterpret_cast<float*>(_fetchSeqd + 0x200) = static_cast<float>(_fetchParamX);
    *reinterpret_cast<float*>(_fetchSeqd + 0x204) = static_cast<float>(_fetchParamY);

    // Create and render option selections.
    for (int i = 0x00; i < 0x04; i++)
    {
        auto _fetchIntro = YS::PANACEA_ALLOC::Get("INTRO_MEMORY");

        auto _offsetSqd = 0x660 + (0x220 * i);
        auto _fetchSeqd2 = *m_SeqTbl + _offsetSqd;
        fprintf(stdout, "0x%p\n", _fetchSeqd2);

        Tz::MenuUtil::CreateMess(_fetchSeqd2, *m_pri, 0, 0, *m_Lay, 26, -1, -1, 0, 0);

        *reinterpret_cast<uint32_t*>(_fetchSeqd2 + 0x010) &= ~0x10;
        *reinterpret_cast<uint32_t*>(_fetchSeqd2 + 0x010) &= ~0x08;
        *reinterpret_cast<uint32_t*>(_fetchSeqd2 + 0x010) &= ~0x08;

        *reinterpret_cast<char**>(_fetchSeqd2 + 0x01F8) = YS::MESSAGE::GetData(*reinterpret_cast<uint16_t*>(_fetchIntro + 0x08 + 0x2C * i));
        Tz::MenuUtil::SetSprtParent(_fetchSeqd2, _fetchSeqd); // Tz::MenuUtil::SetSprtParent

        auto _fetchParamX2 = YI::SEQUENCE::GetParamX(_fetchSeqd2 + 0x20);
        auto _fetchParamY2 = YI::SEQUENCE::GetParamY(_fetchSeqd2 + 0x20);

        auto _fetchSeqd3 = *m_SeqTbl + (0x660 + (0x220 * 0x04) + 0x220 * i);
        fprintf(stdout, "0x%p\n", _fetchSeqd3);

        Tz::MenuUtil::CreateMess(_fetchSeqd3, *m_pri, 0, 0, *m_Lay, 13, -1, -1, 0, 0);

        auto _fetchSelect = YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x200 + (0x04 * i);

        *reinterpret_cast<uint32_t*>(_fetchSeqd3 + 0x010) &= ~0x10;
        *reinterpret_cast<char**>(_fetchSeqd3 + 0x1F8) = YS::MESSAGE::GetData(*reinterpret_cast<uint16_t*>(_fetchIntro + 0x0C + (0x04 * *_fetchSelect) + 0x2C * i));

        Tz::MenuUtil::SetSprtParent(_fetchSeqd3, *m_SeqTbl + 0x660 + 0x220 * i);

        *reinterpret_cast<int*>(_fetchSeqd3 + 0x1C8) = _fetchParamX2;
        *reinterpret_cast<int*>(_fetchSeqd3 + 0x1CC) = _fetchParamY2;
    }

    // Set offsets of option selections.
    for (int i = 0x00; i < 0x04; i++)
    {
        auto _fetchBeginParam = YI::SEQUENCE::GetParamCr(*m_SeqTbl + 0x680);

        auto _offsetSqd = 0x660 + (0x220 * i);
        auto _fetchSeqd = *m_SeqTbl + _offsetSqd;

        *reinterpret_cast<int*>(_fetchSeqd + 0x1C8) = 0x00;
        *reinterpret_cast<int*>(_fetchSeqd + 0x1CC) = (_fetchBeginParam * -2) + _fetchBeginParam * i;
    }


    // Create and render the buttons.
    for (int i = 0x00; i < 0x02; i++)
    { 
        Tz::Select::SetItem(*m_SelPtr, i, 0, 0, *m_Lay, -1, s_Plate2Seq, s_Plate2Seq + 0x08);

        auto _fetchItemPtr = *reinterpret_cast<char**>(*reinterpret_cast<char**>(*m_SelPtr + 0x28) + i * 0x08);
        auto _itemParamEx = YI::SEQUENCE::GetParamEx(_fetchItemPtr + 0x20);
        auto _itemParamCr = YI::SEQUENCE::GetParamCr(_fetchItemPtr + 0x20);

        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

        *reinterpret_cast<int*>(_fetchItemPtr + 0x214) = 0x02;

        *reinterpret_cast<int*>(_fetchItemPtr + 0x1C8) = i * _itemParamCr;
        *reinterpret_cast<int*>(_fetchItemPtr + 0x1CC) = 0x00;

        Tz::MenuUtil::SetSprtParent(_fetchItemPtr, _fetchSeqd);

        *reinterpret_cast<float*>(_fetchItemPtr + 0x200) = (float)_itemParamEx;
        *reinterpret_cast<char**>(_fetchItemPtr + 0x1F8) = YS::MESSAGE::GetData(0xC384 - i); 
    }

    Tz::Select::SetCurrent(*m_SelPtr, 0x01, true, 0x00, 0x02);

    auto _menuPtr16 = reinterpret_cast<uint16_t*>(*m_MenuPtr);

    auto _selectResult = _menuPtr16[0x0B];
    auto _selectCalc = static_cast<uint32_t>(_menuPtr16[0x00] + _menuPtr16[0x09]);

    auto _selectPos = _selectCalc;

    if (_selectResult)
        _selectPos = _selectCalc % _selectResult;

    auto _mInfo = reinterpret_cast<char**>(reinterpret_cast<char*>(m_SeqTbl) - 0x08);
    auto _fetchText = 0xC32F;

        if (**_mInfo <= 1)
            _fetchText = *reinterpret_cast<int*>(YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x200 + 0x0B * **_mInfo + _selectPos * 0x04 + 0x1C);

    *reinterpret_cast<char**>(*m_SeqTbl + 0x638) = YS::MESSAGE::GetData(_fetchText);
    *reinterpret_cast<int*>(*_mInfo + 0x04) = 0x00;

    return *_mInfo;
}