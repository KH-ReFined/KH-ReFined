#include "friendsaveram.h"

int (*kn::FriendSaveRam::item2index)(int) = SignatureScan<int(*)(int)>("\x48\x83\xEC\x28\x45\x33\xC0\x48\x8D\x15\x00\x00\x00\x00\x4C\x8D\x0D", "xxxxxxxxxx????xxx");