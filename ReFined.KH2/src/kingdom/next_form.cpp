#include "next_form.h"
#include <area.h>

char* dk::NEXT_FORM::instance = nullptr;

char* dk::NEXT_FORM::VTABLE_CLASS = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x154);
char* dk::NEXT_FORM::VTABLE_SPRITE = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x80);

void dk::NEXT_FORM::create(int nextExp, int offset16x9)
{
    auto _mainObject = dk::NEXT_FORM::instance + 0x38;
    auto _formID = *(YS::AREA::SaveData + 0x3524);

    if (dk::Obj2D::isExist(_mainObject))
    {
        uint32_t _nextEXP = *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0xDAC);

        if (_nextEXP != nextExp)
        {
            *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0xDAC) = nextExp;
            dk::Sprite::setNum(_mainObject, (_formID == 0x00 ? 0x12 : 0x51 + _formID));
            dk::NEXT_FORM::initNumber(offset16x9);
        }
    }

    else
    {
        *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0xDAC) = nextExp;

        auto _fetchImage = dk::LockOn::getYIImage();
        auto _fetchSeqd = dk::Field2DD::getSqd(0);

        dk::Sprite::create(_mainObject, 4500, _fetchSeqd, _fetchImage, (_formID == 0x00 ? 0x12 : 0x51 + _formID), 7, offset16x9);

        auto _posX = 0x00;
        auto _posY = *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0x2C);

        dk::Obj2D::commitSuicide(_mainObject, 0x00);
        dk::Sprite::setPos(_mainObject, _posX, _posY);

        char* _seqdChain = dk::NEXT_FORM::instance + 0x230;

        for (int i = 0; i < 7; i++)
        {
            YI::SEQUENCE::Init(_seqdChain, _fetchSeqd, _fetchImage);
            YI::SEQUENCE::SetNumber(_seqdChain, 0x26);

            _seqdChain += 0x1A0;
        }

        dk::NEXT_FORM::initNumber(offset16x9);

        auto _seqdParam = YI::SEQUENCE::GetParamCr(dk::NEXT_FORM::instance + 0x58);
        dk::INFO_BASE::create(dk::NEXT_FORM::instance, 0x13, _seqdParam, 4500, 7, 0);
    }
}

void dk::NEXT_FORM::initNumber(int offset16x9)
{
    auto _mainObject = dk::NEXT_FORM::instance + 0x38;

    int _numArray[10] = { 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25 };

    int _outArray[0x08];
    int _fetchArray[0x08];

    uint32_t _nextEXP = *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0xDAC);

    auto _fetchArticles = dk::Exp::getSeqNumber(reinterpret_cast<char*>(_numArray), _nextEXP, _outArray);
    *reinterpret_cast<uint32_t*>(dk::NEXT_FORM::instance + 0xDB0) = _fetchArticles;

    for (int i = 0; i < 0x08; i++)
    {
        auto _formVar = *(YS::AREA::SaveData + 0x3524);

        if (_outArray[i] == 0xCCCCCCCC)
            break;

        _fetchArray[i] = _outArray[i] + 0x3B + (_formVar == 0x00 ? 0x03 : _formVar - 0x01) * 0x0A;
    }

    if (_fetchArticles > 0)
    {
        uint32_t _seqdParam = 0;
        int* _backChain = _outArray;
        int* _backChainFetch = _fetchArray;
        char* _seqdChain = dk::NEXT_FORM::instance + 0x230;

        for (int i = 0; i < _fetchArticles; i++)
        {
            YI::SEQUENCE::SetNumberForce(_seqdChain, *_backChain);
            _seqdParam += YI::SEQUENCE::GetParamEx(_seqdChain);

            YI::SEQUENCE::SetNumberForce(_seqdChain, *_backChainFetch);
            _seqdChain += 0x1A0;

            _backChain++;
            _backChainFetch++;

            *reinterpret_cast<uint32_t*>((uint64_t)_backChain + reinterpret_cast<uint64_t>(dk::NEXT_FORM::instance) - (uint64_t)_outArray + 0xD8C) = _seqdParam + offset16x9;
        }
    }
}