#pragma once
#ifdef MODELCORE_EXPORTS
#define MODELCORE_API __declspec(dllexport)
#else
#define MODELCORE_API __declspec(dllimport)
#endif

#include "ModelCore/Ptr.h"
#include "ModelCore/IObject.h"
#include "ModelCore/Weak.h"

