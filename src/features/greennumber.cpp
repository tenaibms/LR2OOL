#include "greennumber.h"
#include <LR2Bindings.hpp>

double GreenNumber::GetGreenNumber(int hsfix, int player)
{
	double line_h = LR2::pGame->skstruct.dst_LINE[player - 1].draw->h;
	double line_y = LR2::pGame->skstruct.dst_LINE[player - 1].draw->y;

	double adjust_x = player == 1 ? LR2::pGame->skstruct.adjust.note_1p_x : LR2::pGame->skstruct.adjust.note_2p_x;
	double adjust_y = player == 1 ? LR2::pGame->skstruct.adjust.note_1p_y : LR2::pGame->skstruct.adjust.note_2p_y;
	
	double hispeed = player == 1 ? LR2::pGame->config.play.hiSpeed[0] : LR2::pGame->config.play.hiSpeed[1];
	double basespeed = LR2::pGame->config.play.basespeed;

	double cover = player == 1 ? LR2::pGame->config.play.p1_lanecoverv : LR2::pGame->config.play.p2_lanecoverv;

	/* max, min, current */
	double bpm_user[] = {
		LR2::pGame->gameplay.maxBPM,
		LR2::pGame->gameplay.minBPM,
		LR2::pGame->gameplay.BPM
	};

	if(LR2::pGame->config.play.hsfix != 4) /* constant */
		hispeed *= bpm_user[hsfix] * LR2::pGame->gameplay.speedmultiplier / 150.0;

	return ((2173.f / 725.f) * 10000.f) * ((line_h + line_y + adjust_x + adjust_y) / (hispeed * basespeed)) * (1 - (cover / 100.f));
}

double GreenNumber::GetWhiteNumber(int player)
{
	return (player == 1 ? LR2::pGame->config.play.p1_lanecoverv : LR2::pGame->config.play.p2_lanecoverv) * 10;
}

double GreenNumber::GetLiftNumber(int player)
{
	double line_y = LR2::pGame->skstruct.dst_LINE[0].draw->y;
	double adjust_y = player == 1 ? LR2::pGame->skstruct.adjust.note_1p_y : LR2::pGame->skstruct.adjust.note_2p_y;

	double lift = (-1 * adjust_y) / line_y;

	return lift * 1000;
}
