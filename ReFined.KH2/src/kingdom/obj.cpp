#include "obj.h"

YS::OBJ::is_exist_t YS::OBJ::is_exist = ResolveFunctionFromCall<YS::OBJ::is_exist_t>("\x48\x89\x5C\x24\x10\x57\x48\x81\xEC\xB0\x00\x00\x00\x48\x8B\xF9", "xxxxxxxxxxxxxxxx", 0x17);
YS::OBJ::get_part_num_t YS::OBJ::get_part_num = ResolveFunctionFromCall<YS::OBJ::get_part_num_t>("\x40\x56\x48\x81\xEC\xC0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x88\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxx", 0x388);