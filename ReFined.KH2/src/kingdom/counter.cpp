#include "counter.h"

// Trust me, this is easier than rewriting this function.
dk::COUNTER::forceLeave_t dk::COUNTER::forceLeave = ResolveRelativeAddress<dk::COUNTER::forceLeave_t>(reinterpret_cast<char*>(dk::INFORMATION::forceLeaveCounter), 0x1B);

void dk::COUNTER::update(char* counter)
{
	dk::INFO_BASE::update(counter);

	if (*reinterpret_cast<uint32_t*>(counter + 0x1CC0) == 0x01)
	{
		if (!dk::Obj2D::isExist(counter + 0x0300))
			dk::COUNTER::forceLeave(counter);
	}

	else
	{
		dk::Sprite::setPos(counter + 0x0300, *reinterpret_cast<int*>(counter + 0x0028), *reinterpret_cast<int*>(counter + 0x002C));

	}
}