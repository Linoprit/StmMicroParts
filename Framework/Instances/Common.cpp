/*
 * Common.cpp
 *
 *  Created on: 20.05.2019
 *      Author: harald
 */

#include <Instances/Common.h>

#include <iostream>

bool	 Common::initIsDone 	= false;
bool	 Common::isFakeTick		= false;
uint32_t Common::fakeTick		= 0;
std::chrono::milliseconds Common::startTime;

using namespace std;
using namespace std::chrono;

void Common::init(void)
{
	if (Common::initIsDone == true)
		return;

	initIsDone 		= true;

	startTime = duration_cast< milliseconds >(
		    system_clock::now().time_since_epoch() );

	isFakeTick = false;
	fakeTick   = static_cast<uint32_t>(startTime.count());
}

uint32_t Common::get_tick(void)
{
	if (isFakeTick == false) {
		milliseconds now = duration_cast< milliseconds >(
				system_clock::now().time_since_epoch() );
		milliseconds elapsed_ms = now - startTime;
		return static_cast<uint32_t>(elapsed_ms.count());
	}
	else
		return fakeTick;
}
