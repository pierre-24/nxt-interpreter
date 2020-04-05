/*
 *  Time.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 27.11.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Time.h"

// Includes
#if defined(_WIN32)
#include <windows.h>

#elif defined(__APPLE_CC__)
#include <CoreFoundation/CoreFoundation.h>

#elif defined(ANDROID_NDK)  || defined(__linux__)
#include <time.h>

#endif /* Platform */

// Constants
#if defined(_WIN32)
static DWORD startTime;

#elif defined(__APPLE_CC__)
static CFAbsoluteTime startTime;

#elif defined(ANDROID_NDK) || defined(__linux__)
static long startTime;

#endif

void startTimeCount()
{
#if defined(_WIN32)
	startTime = GetTickCount();
#elif defined(__APPLE_CC__)
	startTime = CFAbsoluteTimeGetCurrent();
#elif defined(ANDROID_NDK) || defined(__linux__)
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	startTime = (now.tv_sec*1000 + (unsigned int) (now.tv_nsec/1000000.f));
#endif
}

unsigned millisecondsSinceStart()
{
#if defined(_WIN32)
	unsigned now = GetTickCount();
	if (now < startTime)
		return (((DWORD) ~0) - startTime) + now;
	else
		return now - startTime;
#elif defined(__APPLE_CC__)
	CFAbsoluteTime now = CFAbsoluteTimeGetCurrent();
	return (unsigned) ((now - startTime)*1000.0f);
#elif defined(ANDROID_NDK) || defined(__linux__)
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec*1000 + (unsigned int) (now.tv_nsec/1000000.f)) - startTime;
#endif
}