#include "hooks/skinparsing.h"
#include "hooks/hooks.h"
#include <unordered_set>
#include <LR2Bindings.hpp>

static std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

static std::string unsplit(std::vector<std::string> strings, std::string delimiter)
{
	std::string new_string = {};
	for (auto string : strings) {
		new_string += string + delimiter;
	}
	return new_string;
}

void SkinParsing::Init()
{
	m_read_dst_hook = safetyhook::create_inline(m_offsets.read_dst, OnReadDST);
	m_start_include_hook = safetyhook::create_mid(m_offsets.start_include, OnStartInclude);
	m_finish_include_hook = safetyhook::create_mid(m_offsets.finish_include, OnFinishInclude);
	m_first_line_hook = safetyhook::create_mid(m_offsets.read_line, OnReadLine);
}

void SkinParsing::Reset()
{
	m_offset_stack = {};
	m_filename_stack = {};
}

int SkinParsing::OnReadDST(LR2::DSTstruct* dst, LR2::CSVbuf* csv, int order)
{
	SkinParsing& skin_parsing = hooks::skin_parsing;
	LR2::skstruct& sk = LR2::pGame->skstruct;
	
	int ret = skin_parsing.m_read_dst_hook.call<int>(dst, csv, order);
	
	/* Maybe move to another file? This makes this file unnecessarily long*/
	static std::unordered_set<void*> dst_blacklist = {
		/* #DST_BAR_TITLE */
		&sk.dst_BAR_TITLE[0],
		&sk.dst_BAR_TITLE[1],
		&sk.dst_BAR_TITLE[2],
		&sk.dst_BAR_TITLE[3],
		&sk.dst_BAR_TITLE[4],
		/* #DST_BAR_FLASH */
		&sk.dst_BAR_FLASH,
		/* #DST_BAR_LEVEL */
		&sk.dst_BAR_LEVEL[0],
		&sk.dst_BAR_LEVEL[1],
		&sk.dst_BAR_LEVEL[2],
		&sk.dst_BAR_LEVEL[3],
		&sk.dst_BAR_LEVEL[4],
		&sk.dst_BAR_LEVEL[5],
		&sk.dst_BAR_LEVEL[6],
		&sk.dst_BAR_LEVEL[7],
		&sk.dst_BAR_LEVEL[8],
		&sk.dst_BAR_LEVEL[9],
		/* #DST_BAR_LAMP */
		&sk.dst_BAR_LAMP[0],
		&sk.dst_BAR_LAMP[1],
		&sk.dst_BAR_LAMP[2],
		&sk.dst_BAR_LAMP[3],
		&sk.dst_BAR_LAMP[4],
		&sk.dst_BAR_LAMP[5],
		&sk.dst_BAR_LAMP[6],
		&sk.dst_BAR_LAMP[7],
		&sk.dst_BAR_LAMP[8],
		&sk.dst_BAR_LAMP[9],
		&sk.dst_BAR_MY_LAMP[0],
		&sk.dst_BAR_MY_LAMP[1],
		&sk.dst_BAR_MY_LAMP[2],
		&sk.dst_BAR_MY_LAMP[3],
		&sk.dst_BAR_MY_LAMP[4],
		&sk.dst_BAR_MY_LAMP[5],
		&sk.dst_BAR_MY_LAMP[6],
		&sk.dst_BAR_MY_LAMP[7],
		&sk.dst_BAR_MY_LAMP[8],
		&sk.dst_BAR_MY_LAMP[9],
		&sk.dst_BAR_RIVAL_LAMP[0],
		&sk.dst_BAR_RIVAL_LAMP[1],
		&sk.dst_BAR_RIVAL_LAMP[2],
		&sk.dst_BAR_RIVAL_LAMP[3],
		&sk.dst_BAR_RIVAL_LAMP[4],
		&sk.dst_BAR_RIVAL_LAMP[5],
		&sk.dst_BAR_RIVAL_LAMP[6],
		&sk.dst_BAR_RIVAL_LAMP[7],
		&sk.dst_BAR_RIVAL_LAMP[8],
		&sk.dst_BAR_RIVAL_LAMP[9],
		/* #DST_BAR_RANk */
		&sk.dst_BAR_RANK[0],
		&sk.dst_BAR_RANK[1],
		&sk.dst_BAR_RANK[2],
		&sk.dst_BAR_RANK[3],
		&sk.dst_BAR_RANK[4],
		&sk.dst_BAR_RANK[5],
		&sk.dst_BAR_RANK[6],
		&sk.dst_BAR_RANK[7],
		&sk.dst_BAR_RANK[8],
		&sk.dst_BAR_RANK[9],
		/* #DST_BAR_RIVAL */
		&sk.dst_BAR_RIVAL[0],
		&sk.dst_BAR_RIVAL[1],
		&sk.dst_BAR_RIVAL[2],
		&sk.dst_BAR_RIVAL[3],
		&sk.dst_BAR_RIVAL[4],
		&sk.dst_BAR_RIVAL[5],
		&sk.dst_BAR_RIVAL[6],
		&sk.dst_BAR_RIVAL[7],
		&sk.dst_BAR_RIVAL[8],
		&sk.dst_BAR_RIVAL[9],
		/* #DST_MOUSECURSOR */
		&sk.dst_MOUSECURSOR,
		/*# DST_NOWCOMBO */
		&sk.dst_NOWCOMBO_1P[0],
		&sk.dst_NOWCOMBO_1P[1],
		&sk.dst_NOWCOMBO_1P[2],
		&sk.dst_NOWCOMBO_1P[3],
		&sk.dst_NOWCOMBO_1P[4],
		&sk.dst_NOWCOMBO_1P[5],
		&sk.dst_NOWCOMBO_2P[0],
		&sk.dst_NOWCOMBO_2P[1],
		&sk.dst_NOWCOMBO_2P[2],
		&sk.dst_NOWCOMBO_2P[3],
		&sk.dst_NOWCOMBO_2P[4],
		&sk.dst_NOWCOMBO_2P[5],
	};

	/* some things are always defined relative to other skin objects, so they should not have their x and y offsets adjusted */
	if (dst_blacklist.contains(dst)) {
		for (CSVOffset& offset : skin_parsing.m_offset_stack) {
			/* w and h */
			dst->draw[dst->dstCount - 1].x *= offset.w;
			dst->draw[dst->dstCount - 1].y *= offset.h;
			dst->draw[dst->dstCount - 1].w *= offset.w;
			dst->draw[dst->dstCount - 1].h *= offset.h;
		}

		return ret;
	} else {
		for (CSVOffset& offset : skin_parsing.m_offset_stack) {
			/* x and y */
			dst->draw[dst->dstCount - 1].x += offset.x;
			dst->draw[dst->dstCount - 1].y += offset.y;
			/* w and h */
			dst->draw[dst->dstCount - 1].x *= offset.w;
			dst->draw[dst->dstCount - 1].y *= offset.h;
			dst->draw[dst->dstCount - 1].w *= offset.w;
			dst->draw[dst->dstCount - 1].h *= offset.h;
		}

		return ret;
	}	
}

void SkinParsing::OnStartInclude(SafetyHookContext& ctx)
{
	SkinParsing& skin_parsing = hooks::skin_parsing;

	std::string filename = std::string(*(const char**)(ctx.esp + 0x30));

	if(skin_parsing.m_include_flag) {
		skin_parsing.m_filename_stack.push_front(filename);
		skin_parsing.m_include_flag = false;
	}
}

void SkinParsing::OnFinishInclude(SafetyHookContext& ctx)
{
	SkinParsing& skin_parsing = hooks::skin_parsing;

	std::string filename = std::string(*(const char**)(ctx.esp + 0x30));

	if((skin_parsing.m_offset_stack.size() > 0)) {
		if (filename == skin_parsing.m_filename_stack.front()) {
			skin_parsing.m_offset_stack.pop_front();
			skin_parsing.m_filename_stack.pop_front();
		}
	}
}

void SkinParsing::OnReadLine(SafetyHookContext& ctx)
{
	SkinParsing& skin_parsing = hooks::skin_parsing;

	std::string line = std::string(*(const char**)(ctx.ecx));

	auto buf = split(line, ",");
	if (buf[0] == "#CSVOFFSET") {
		skin_parsing.m_include_flag = true;
		if(buf.size() >= 3) {
			CSVOffset offset = { std::stoi(buf[1]), std::stoi(buf[2]) };
			if (buf.size() >= 5) {
				offset.w = std::stod(buf[3]);
				offset.h = std::stod(buf[4]);
			}
			skin_parsing.m_offset_stack.push_front(offset);
		}
	}
}




