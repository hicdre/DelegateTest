#pragma once
#include <stdint.h>

#ifdef MODELCORE_EXPORTS
#define MODELCORE_API __declspec(dllexport)
#else
#define MODELCORE_API __declspec(dllimport)
#endif

namespace ModelInternal
{
	struct MODELCORE_API auto_refcount {
		uint32_t value;
		auto_refcount();
	};
}