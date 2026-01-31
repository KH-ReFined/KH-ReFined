#include "fvector.h"

uint32_t* kn::FVector::Init(uint32_t* _firstPtr, uint32_t* _secondPtr)
{
    if (!_firstPtr || !_secondPtr || reinterpret_cast<uint64_t>(_firstPtr) > 0x7FFF00000000 || reinterpret_cast<uint64_t>(_secondPtr) > 0x7FFF00000000)
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
