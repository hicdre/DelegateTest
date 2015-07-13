// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "stdafx.h"
#include "IDelegateInstance.h"

namespace UE4Delegates_Private
{
	uint64_t GNextID = 1;
}

uint64_t FDelegateHandle::GenerateNewID()
{
	// Just increment a counter to generate an ID.
	// On the next-to-impossible event that we wrap round to 0, reset back to 1, because we reserve 0 for null delegates.
	uint64_t Result = UE4Delegates_Private::GNextID++;
	if (UE4Delegates_Private::GNextID == 0)
	{
		UE4Delegates_Private::GNextID = 1;
	}

	return Result;
}
