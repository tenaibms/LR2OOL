#pragma once
#include <stdint.h>

/*#define ciu const inline uintptr_t

namespace greennumber {
	namespace Offsets {
		ciu skin_struct = 0x108358;
		ciu draw_ptr_p1 = 0x89CC;
		ciu draw_ptr_p2 = 0x89F8;
		ciu y = 0x4;
		ciu h = 0xC;

		ciu hispeed = 0xFF838;
		ciu cover = 0xFF858;
		ciu basespeed = 0xFF8D0;

		ciu adjust_y1 = 0x111F88;
		ciu adjust_y2 = 0x111F90;
		ciu adjust_h = 0x111F80;

		ciu max_bpm = 0x19F520;
		ciu min_bpm = 0x19F528;

		ciu current_bpm = 0x197188;

		ciu hs_fix = 0xFF880;
	}

	double GetGreenNumber();
	double GetMinGreenNumber();
	double GetMaxGreenNumber();
	double GetWhiteNumber();
	double GetLiftNumberP1();
	double GetLiftNumberP2();

	void UpdateVariables();
}

#undef ciu*/

class GreenNumber {
public:
	double GetGreenNumber(int hsfix, int player);
	double GetWhiteNumber(int player);
	double GetLiftNumber(int player);
};