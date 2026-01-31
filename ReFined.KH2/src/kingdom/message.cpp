#include "message.h"
#include "SigScan.h"
#include <Windows.h>
#include <regex>
#include <iomanip> 

YS::MESSAGE::GetData_t YS::MESSAGE::GetData = SignatureScan<YS::MESSAGE::GetData_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x50\x45\x33\xF6\x48\x63\xE9\x33\xF6\x48\x8D\x3D\x00\x00\x00\x00\x4C\x8D\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????");
YS::MESSAGE::GetSize_t YS::MESSAGE::GetSize = SignatureScan<YS::MESSAGE::GetSize_t>("\x4C\x8B\xC1\x48\x8B\xD1\x48\x85\xC9\x74\x2E\x4C\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxx????");

char* YS::MESSAGE::SizeTable = ResolveRelativeAddress<char*>("\x4C\x8B\xC1\x48\x8B\xD1\x48\x85\xC9\x74\x2E\x4C\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxx????", 0x0E);

vector<char> YS::MESSAGE::EncodeKHSCII(string Input)
{
    // A dictionary of all the special characters, which
    // are hard to convert through a mathematical formula.
    static const map<char, uint8_t> _specialDict = {
        { ' ', 0x01 },
        { '\n', 0x02 },
        { '!', 0x48 },
        { '?', 0x49 },
        { '%', 0x4A },
        { '/', 0x4B },
        { '.', 0x4F },
        { ',', 0x50 },
        { ';', 0x51 },
        { ':', 0x52 },
        { '-', 0x54 },
        { 'ー', 0x55 },
        { '~', 0x56 },
        { '\'', 0x57 },
        { '(', 0x5A },
        { ')', 0x5B },
        { '[', 0x62 },
        { ']', 0x63 },
        { 'à', 0xB7 },
        { 'á', 0xB8 },
        { 'â', 0xB9 },
        { 'ä', 0xBA },
        { 'è', 0xBB },
        { 'é', 0xBC },
        { 'ê', 0xBD },
        { 'ë', 0xBE },
        { 'ì', 0xBF },
        { 'í', 0xC0 },
        { 'î', 0xC1 },
        { 'ï', 0xC2 },
        { 'ñ', 0xC3 },
        { 'ò', 0xC4 },
        { 'ó', 0xC5 },
        { 'ô', 0xC6 },
        { 'ö', 0xC7 },
        { 'ù', 0xC8 },
        { 'ú', 0xC9 },
        { 'û', 0xCA },
        { 'ü', 0xCB },
        { 'ç', 0xE8 },
        { 'À', 0xD0 },
        { 'Á', 0xD1 },
        { 'Â', 0xD2 },
        { 'Ä', 0xD3 },
        { 'È', 0xD4 },
        { 'É', 0xD5 },
        { 'Ê', 0xD6 },
        { 'Ë', 0xD7 },
        { 'Ì', 0xD8 },
        { 'Í', 0xD9 },
        { 'Î', 0xDA },
        { 'Ï', 0xDB },
        { 'Ñ', 0xDC },
        { 'Ò', 0xDD },
        { 'Ó', 0xDE },
        { 'Ô', 0xDF },
        { 'Ö', 0xE0 },
        { 'Ù', 0xE1 },
        { 'Ú', 0xE2 },
        { 'Û', 0xE3 },
        { 'Ü', 0xE4 },
        { '¡', 0xE5 },
        { '¿', 0xE6 },
        { 'Ç', 0xE7 }
    };

    vector<char> _outList;
    size_t _charCount = 0;
    const size_t length = Input.size();

    regex command_regex(R"(\{0x[a-fA-F0-9]{2}\})");

    while (_charCount < length)
    {
        char _char = Input[_charCount];

        // Simple character conversion through mathematics.

        if (_char >= 'a' && _char <= 'z')
        {
            _outList.push_back(_char + 0x39);
            _charCount++;
        }

        else if (_char >= 'A' && _char <= 'Z')
        {
            _outList.push_back(_char - 0x13);
            _charCount++;
        }

        else if (_char >= '0' && _char <= '9')
        {
            _outList.push_back(_char + 0x60);
            _charCount++;
        }

        // If it hits a "{", we will know it's a command, not a character.
        else if (_char == '{' && _charCount + 6 <= length)
        {
            string _command = Input.substr(_charCount, 6);

            if (regex_match(_command, command_regex))
            {
                string _value = _command.substr(3, 2); // "TT" part
                uint8_t byte_value = std::stoi(_value, nullptr, 16);

                _outList.push_back(byte_value);
                _charCount += 6;
            }

            else
            {
                // If not a valid command, treat as unknown character
                _outList.push_back(0x01);
                _charCount++;
            }
        }
        else
        {
            auto it = _specialDict.find(_char);

            if (it != _specialDict.end())
                _outList.push_back(it->second);

            else
                _outList.push_back(0x01);

            _charCount++;
        }
    }

    // When the list ends, we add a terminator and return the string.
    _outList.push_back(0x00);
    return _outList;
}

string YS::MESSAGE::DecodeKHSCII(const char* Input)
{
	map<uint8_t, char> _specialDict
	{
		{ 0x01, ' ' },
		{ 0x02, '\n' },
		{ 0x2C, '-' },
		{ 0x66, '-' },
		{ 0x48, '!' },
		{ 0x49, '?' },
		{ 0x4A, '%' },
		{ 0x4B, '/' },
		{ 0x4F, '.' },
		{ 0x50, ',' },
		{ 0x51, ';' },
		{ 0x52, ':' },
        { 0x54, '-' },
        { 0x55, 'ー'},
        { 0x56, '~' },
		{ 0x57, '\'' },
		{ 0x5A, '('},
		{ 0x5B, ')'},
		{ 0x62, '['},
		{ 0x63, ']'},
		{ 0xB7, 'à'},
		{ 0xB8, 'á'},
		{ 0xB9, 'â'},
		{ 0xBA, 'ä'},
		{ 0xBB, 'è'},
		{ 0xBC, 'é'},
		{ 0xBD, 'ê'},
		{ 0xBE, 'ë'},
		{ 0xBF, 'ì'},
		{ 0xC0, 'í'},
		{ 0xC1, 'î'},
		{ 0xC2, 'ï'},
		{ 0xC3, 'ñ'},
		{ 0xC4, 'ò'},
		{ 0xC5, 'ó'},
		{ 0xC6, 'ô'},
		{ 0xC7, 'ö'},
		{ 0xC8, 'ù'},
		{ 0xC9, 'ú'},
		{ 0xCA, 'û'},
		{ 0xCB, 'ü'},
		{ 0xE8, 'ç'},
		{ 0xD0, 'À'},
		{ 0xD1, 'Á'},
		{ 0xD2, 'Â'},
		{ 0xD3, 'Ä'},
		{ 0xD4, 'È'},
		{ 0xD5, 'É'},
		{ 0xD6, 'Ê'},
		{ 0xD7, 'Ë'},
		{ 0xD8, 'Ì'},
		{ 0xD9, 'Í'},
		{ 0xDA, 'Î'},
		{ 0xDB, 'Ï'},
		{ 0xDC, 'Ñ'},
		{ 0xDD, 'Ò'},
		{ 0xDE, 'Ó'},
		{ 0xDF, 'Ô'},
		{ 0xE0, 'Ö'},
		{ 0xE1, 'Ù'},
		{ 0xE2, 'Ú'},
		{ 0xE3, 'Û'},
		{ 0xE4, 'Ü'},
		{ 0xE5, '¡'},
		{ 0xE6, '¿'},
		{ 0xE7, 'Ç'}
	};

	vector<char> _outList;

	auto _charCount = 0;
	auto _currentChar = *reinterpret_cast<const uint8_t*>(Input + _charCount);

	auto _totalSize = YS::MESSAGE::GetSize(Input);

	// Throughout the text, do:
	while (_charCount != _totalSize)
	{
		// Simple character conversion through mathematics.
		if (_currentChar >= 0x9A && _currentChar <= 0xB3)
		{
			_outList.push_back(_currentChar - 0x39);
			_charCount++;
		}

		else if (_currentChar >= 0x2E && _currentChar <= 0x47)
		{
			_outList.push_back(_currentChar + 0x13);
			_charCount++;
		}

		else if (_currentChar >= 0x90 && _currentChar <= 0x99)
		{
			_outList.push_back(_currentChar - 0x60);
			_charCount++;
		}

        else if (_currentChar <= 0x20 && _currentChar >= 0x03)
        {
            auto _fetchSize = *(YS::MESSAGE::SizeTable + _currentChar);

            for (int i = 0; i < _fetchSize; i++)
            {
                auto _fetchChar = *reinterpret_cast<const uint8_t*>(Input + _charCount + i);

                stringstream _charToHex;
                _charToHex << "{0x" << std::hex << std::setw(2) << std::setfill('0') << (int)_fetchChar << "}";

                for (auto _chr : _charToHex.str())
                    _outList.push_back(_chr);
            }

            _charCount += _fetchSize;
        }

		else
		{
			if (_specialDict.find(_currentChar) != _specialDict.end())
				_outList.push_back(_specialDict.at(_currentChar));

			else
			{
				stringstream _charToHex;
				_charToHex << "{0x" << std::hex << std::setw(2) << std::setfill('0') << (int)_currentChar << "}";

				for (auto _chr : _charToHex.str())
					_outList.push_back(_chr);
			}
			_charCount++;
		}

		_currentChar = *reinterpret_cast<const uint8_t*>(Input + _charCount);
	}

	return string(_outList.begin(), _outList.end());
}