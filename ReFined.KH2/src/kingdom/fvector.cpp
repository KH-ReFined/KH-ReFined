#include "fvector.h"

uint32_t* kn::FVector::Init(uint32_t* _firstPtr, uint32_t* _secondPtr)
{
    if (!_firstPtr || !_secondPtr || reinterpret_cast<char*>(_firstPtr) > moduleInfo.endAddr || reinterpret_cast<char*>(_secondPtr) > moduleInfo.endAddr)
        return nullptr;

    if (_firstPtr != _secondPtr)
    {
        *_firstPtr = *_secondPtr;
        _firstPtr[1] = _secondPtr[1];
        _firstPtr[2] = _secondPtr[2];
        _firstPtr[3] = _secondPtr[3];
    }

    return _firstPtr;
}

kn::FVector::staticInitializer initialize;
