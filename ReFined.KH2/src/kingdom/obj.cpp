#include "obj.h"

YS::OBJ::is_exist_t YS::OBJ::is_exist = ResolveFunctionFromCall<YS::OBJ::is_exist_t>("\x48\x89\x5C\x24\x10\x57\x48\x81\xEC\xB0\x00\x00\x00\x48\x8B\xF9", "xxxxxxxxxxxxxxxx", 0x17);
YS::OBJ::get_part_num_t YS::OBJ::get_part_num = ResolveFunctionFromCall<YS::OBJ::get_part_num_t>("\x40\x56\x48\x81\xEC\xC0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x88\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxx", 0x388);
YS::OBJ::is_enable_collision_t YS::OBJ::is_enable_collision = SignatureScan<YS::OBJ::is_enable_collision_t>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x83\xB9\xB8\x0A\x00\x00\x00\x8B\xDA\x48\x8B\xF9\x74\x78", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
YS::OBJ::get_bone_matrix_t YS::OBJ::get_bone_matrix = SignatureScan<YS::OBJ::get_bone_matrix_t>("\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x70\x10\x48\x89\x78\x18\x55\x48\x8D\x68\xE8\x48\x81\xEC\x10\x01\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");

YS::OBJ::staticInitializer initialize;

float YS::OBJ::get_top_matrix(char* obj, char* matrix)
{
	if (obj)
	{
		auto _fetchData = *reinterpret_cast<char**>(obj + 0x0AB8);
		char* _fetchElem = YS::COLLISION_DATA::next_elem(_fetchData, 15, nullptr);

		if (_fetchData)
		{
			while (_fetchElem && !YS::OBJ::is_enable_collision(obj, *_fetchElem))
				_fetchElem = YS::COLLISION_DATA::next_elem(_fetchData, 15, _fetchElem);
		}

		YS::OBJ::get_bone_matrix(obj, *reinterpret_cast<short*>(_fetchElem + 0x06), matrix);
		return static_cast<float>(*reinterpret_cast<short*>(_fetchElem + 0x08));
	}
}