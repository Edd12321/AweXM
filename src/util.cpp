#include "util.hpp"

namespace AweXM {

array<double(*)(size_t), 3> wave = {
	[](size_t i) { return sin(2 * M_PI * static_cast<double>(i) / 64); },
	[](size_t i) { return 1 - static_cast<double>(((i + 32) % 64)) / 32.; },
	[](size_t i) { return i < 32 ? 1. : -1.; }
};

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
		else if (effect_type <= 0x21)
			ret += static_cast<char>(effect_type - 0xA + 'A');
		else ret = "?";
		ret += byte2hexstr(effect_parameter);
	}
	return ret;
}

double note2period(byte note, int finetune, bool linear) {
	// all of this is from xm-form.txt
	static constexpr double period_tab[] = {
		907, 900, 894, 887, 881, 875, 868, 862, 856, 850, 844, 838, 832, 826, 820, 814,
		808, 802, 796, 791, 785, 779, 774, 768, 762, 757, 752, 746, 741, 736, 730, 725,
		720, 715, 709, 704, 699, 694, 689, 684, 678, 675, 670, 665, 660, 655, 651, 646,
		640, 636, 632, 628, 623, 619, 614, 610, 604, 601, 597, 592, 588, 584, 580, 575,
		570, 567, 563, 559, 555, 551, 547, 543, 538, 535, 532, 528, 524, 520, 516, 513,
		508, 505, 502, 498, 494, 491, 487, 484, 480, 477, 474, 470, 467, 463, 460, 457
	};
	auto ft = finetune / 16.;
	int ift = static_cast<int>(std::floor(ft));
	if (linear)
		return 7680. - note * 64. - finetune / 2.;

	// https://github.com/danielepiccone/xm_file/blob/main/xm.txt
	// versiunea din README are un bug
	return (period_tab[note % 12 * 8 + ift] * (1 - (ft - ift))
	      + period_tab[note % 12 * 8 + ift + 1] * (ft - ift))
	     * 16. / std::pow(2., note / 12);
}

double period2freq(double period, bool linear) {
	if (linear)
		return 8363. * std::pow(2., (4608 - period) / 768.);
	return 8363. * 1712. / period;
}

}
