/*
 *  Time.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 27.11.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Time.h"
#include <chrono>

auto start_time = std::chrono::high_resolution_clock::now();

void startTimeCount()
{
    start_time = std::chrono::high_resolution_clock::now();
}

unsigned millisecondsSinceStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
}