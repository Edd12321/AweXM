#include "util.hpp"

namespace AweXM {

string byte2hexstr(byte b) {
	constexpr char hex[] = "0123456789ABCDEF";
	string ret;
	ret += hex[b / 16];
	ret += hex[b % 16];
	return ret;
}
string note2str(byte note) {
	static const char names[][3] = {
		"C-", "C#", "D-", "D#", "E-", "F-",
		"F#", "G-", "G#", "A-", "A#", "B-"
	};
	if (note == 0)
		return "---";
	if (note == 97)
		return "OFF";
	if (note > 97)
		return "???";

	--note;
	return string(names[note % 12]) + std::to_string(note / 12);
}
string effect2str(byte effect_type, byte effect_parameter) {
	string ret;
	if (!effect_type && !effect_parameter)
		ret = "...";
	else {
		if (effect_type <= 0x09)
			ret += static_cast<char>(effect_type + '0');
		else if (effect_type >= 0x0A && effect_type <= 0x21)
			ret += static_cast<char>(effect_type - 0xA + 'A');
		else ret = "?";
		ret += byte2hexstr(effect_parameter);
	}
	return ret;
}

}
