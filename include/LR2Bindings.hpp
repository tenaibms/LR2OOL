#include "LR2Typedefs.hpp"

typedef unsigned char undefined;

namespace LR2 {
	inline bool isInit = false;
	inline std::uintptr_t stackOffset = 0;

	inline game* pGame = 0;
	inline void* pSqlite = 0;

	void Init();
}