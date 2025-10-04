#pragma once
#include <safetyhook.hpp>
#include <LR2Typedefs.hpp>
#include <deque>

class SkinParsing {
public:
	SkinParsing() = default;
	
	void Init();
	void Reset(); /* should be called when a new skin is loaded to clear the current stack of filenames and offsets */
private:
	struct {
		uintptr_t read_dst = 0x49E8E0;
		uintptr_t start_include = 0x4A6B7A;
		uintptr_t finish_include = 0x4A6B7F;
		uintptr_t read_line = 0x4a1fd8; /* there are two fgets calls for some reason */
	} m_offsets;

	struct CSVOffset {
		int x = 0, y = 0;
		double w = 1, h = 1;
	};

	std::deque<CSVOffset> m_offset_stack;
	std::deque<std::string> m_filename_stack;

	bool m_include_flag = false;

	static int OnReadDST(LR2::DSTstruct* dst, LR2::CSVbuf* csv, int order);
	static void OnStartInclude(SafetyHookContext& ctx);
	static void OnFinishInclude(SafetyHookContext& ctx);
	static void OnReadLine(SafetyHookContext& ctx);	

	SafetyHookInline m_read_dst_hook;
	SafetyHookMid m_start_include_hook;
	SafetyHookMid m_finish_include_hook;
	SafetyHookMid m_first_line_hook;
	SafetyHookMid m_additional_lines_hook;
};