/*
 * TC_MainMachine.cpp
 *
 *  Created on: 06.09.2019
 *      Author: harald
 */

// --gtest_filter=Test_Cases1* / --gtest_filter=TC_HeizScheduler.many_cycles_fixed
// --gtest_list_tests

#include <HeizStateMachine/MainMachine.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <string.h>

#include <Instances/Common.h>


using ::testing::Expectation;

#define PRINT
// set tab width in console wiht: tabs +15

void do_loop(MainMachine& mainMachine, uint32_t time_ms, bool doPrint) {
	uint32_t tick = Common::get_tick() + 1;
	const uint32_t step = 1;

	for (uint32_t i=0; i < time_ms; i+=step ) {
		Common::set_fakeTick(tick + i);
		mainMachine.cycle();

#ifdef PRINT
		if (!doPrint)
			continue;

		if (i == 0)
			printf("cyc \t pmp \t on \t night \t st_winter \t tact \t tpaus \t tactMax \t tpausMax \t tactSum \t tpausSum \n");

		if((i > 4u) && (i <= time_ms-6u))
			continue;

		for (uint32_t j=0; j < NR_OF_INSTANCES; j++) {
			printf("%i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",
					i,
					j,
					mainMachine.isPumpActive(j),
					mainMachine.get_nightMode(),
					mainMachine.get_state_winter(j),
					mainMachine.get_tactive_winter(j),
					mainMachine.get_tpause_winter(j),
					mainMachine.get_tactiveMax_winter(j),
					mainMachine.get_tpauseMax_winter(j),
					mainMachine.get_tactive_summer(j),
					mainMachine.get_tpause_summer(j)
			);
		}
#endif
	}
}


TEST(TC_MainMachine, Winter) {
	const bool printThis = false;

	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	RtcSocket* rtcSocket = new RtcSocket();
	MainMachine mainMachine(rtcSocket);

	// startup with no settings
	do_loop(mainMachine, 3000u, printThis);
	mainMachine.set_tactiveMax_winter(0, MIN2MS(5));
	mainMachine.set_tactiveMax_winter(1, MIN2MS(5));
	mainMachine.set_tpauseMax_winter(0, MIN2MS(3));
	mainMachine.set_tpauseMax_winter(1, MIN2MS(7));

	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, 1u, printThis);

	// pump 0
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(5u), printThis);

	// pump 1
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);
	do_loop(mainMachine, MIN2MS(3u), printThis);
	EXPECT_EQ(mainMachine.get_state_winter(0), 3); // ready

	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);
	do_loop(mainMachine, MIN2MS(2u), printThis);

	// pump 0
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(5u), printThis);

	// all inactive
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	EXPECT_EQ(mainMachine.get_state_winter(0), 2); // pause
	EXPECT_EQ(mainMachine.get_state_winter(1), 2); // pause
	do_loop(mainMachine, MIN2MS(2u), printThis);

	// from beginning, but pump 1 starts, now this is the cycle
	for (int i=0; i<5; i++)
	{
	// pump 1
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);
	do_loop(mainMachine, MIN2MS(5u), printThis);
	EXPECT_EQ(mainMachine.get_state_winter(0), 1); // active

	// pump 0
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(5u), printThis);

	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(2u), printThis);

	// pump 1
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);
	do_loop(mainMachine, MIN2MS(5u), printThis);

	// pump 0
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(5u), printThis);

	// all inactive
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	EXPECT_EQ(mainMachine.get_state_winter(0), 2); // pause
	EXPECT_EQ(mainMachine.get_state_winter(1), 2); // pause
	do_loop(mainMachine, MIN2MS(2u), printThis);
	}
}

TEST(TC_MainMachine, WinterNight) {
	const bool printThis = false;

	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	RtcSocket* rtcSocket = new RtcSocket();
	MainMachine mainMachine(rtcSocket);

	// startup with no settings
	do_loop(mainMachine, 3000u, printThis);
	mainMachine.set_tactiveMax_winter(0, MIN2MS(5));
	mainMachine.set_tactiveMax_winter(1, MIN2MS(5));
	mainMachine.set_tpauseMax_winter(0, MIN2MS(3));
	mainMachine.set_tpauseMax_winter(1, MIN2MS(7));

	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, 1u, printThis);

	// pump 0
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(3u), printThis);

	mainMachine.set_nightMode(true);
	do_loop(mainMachine, MIN2MS(10u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	mainMachine.set_nightMode(false);

	// pump 1
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);
	do_loop(mainMachine, MIN2MS(3u), printThis);

	// pump 0
	do_loop(mainMachine, MIN2MS(3u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(2u), printThis);

	do_loop(mainMachine, MIN2MS(1u), printThis);
	mainMachine.set_nightMode(true);
	do_loop(mainMachine, MIN2MS(10u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	mainMachine.set_nightMode(false);

	do_loop(mainMachine, MIN2MS(1u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);
}

TEST(TC_MainMachine, Summer) {
	const bool printThis = false;

	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	RtcSocket* rtcSocket = new RtcSocket();
	MainMachine mainMachine(rtcSocket);
	RTC_TimeTypeDef sTime = { 1,  0, 0}; // same as in SchedulerSummer.cpp
	RTC_DateTypeDef sDate = { RTC_WEEKDAY_SUNDAY, 1, 2 ,3 };

	// startup with no settings
	do_loop(mainMachine, 3000u, false);
	mainMachine.set_tactiveMax_winter(0, MIN2MS(5));
	mainMachine.set_tactiveMax_winter(1, MIN2MS(5));
	mainMachine.set_tpauseMax_winter(0, MIN2MS(3));
	mainMachine.set_tpauseMax_winter(1, MIN2MS(7));

	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, 1u, printThis);

	// pump 0
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
	do_loop(mainMachine, MIN2MS(10u), printThis);

	// Night Mode must not be respected
	mainMachine.set_nightMode(true);

	mainMachine.set_state(MainMachine::summer);
	for(int i=0; i<5; i++)
	{
		sTime = { 1,  0, 0}; // same as in SchedulerSummer.cpp
		sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
		rtcSocket->set_time_date(sTime, sDate);
		do_loop(mainMachine, 1u, false);

		sTime = { 2,  0, 0}; // same as in SchedulerSummer.cpp
		sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
		rtcSocket->set_time_date(sTime, sDate);

		EXPECT_EQ(mainMachine.isPumpActive(0), true);
		EXPECT_EQ(mainMachine.isPumpActive(1), false);
		do_loop(mainMachine, MIN2MS(5u), printThis);

		sTime = { 3,  0, 0}; // same as in SchedulerSummer.cpp
		sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
		rtcSocket->set_time_date(sTime, sDate);

		EXPECT_EQ(mainMachine.isPumpActive(0), false);
		EXPECT_EQ(mainMachine.isPumpActive(1), true);
		do_loop(mainMachine, MIN2MS(5u), printThis);

		sTime = { 4,  0, 0}; // same as in SchedulerSummer.cpp
		sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
		rtcSocket->set_time_date(sTime, sDate);

		EXPECT_EQ(mainMachine.isPumpActive(0), false);
		EXPECT_EQ(mainMachine.isPumpActive(1), false);
		do_loop(mainMachine, MIN2MS(1u), printThis);

		// SummerState resceduling avoidance (see constructor)
		do_loop(mainMachine, MIN2MS(20u), false);
	}
}

TEST(TC_MainMachine, Manual) {
	const bool printThis = false;

	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	RtcSocket* rtcSocket = new RtcSocket();
	MainMachine mainMachine(rtcSocket);

	// startup with no settings
	do_loop(mainMachine, 3000u, false);
	mainMachine.set_tactiveMax_winter(0, MIN2MS(5));
	mainMachine.set_tactiveMax_winter(1, MIN2MS(5));
	mainMachine.set_tpauseMax_winter(0, MIN2MS(3));
	mainMachine.set_tpauseMax_winter(1, MIN2MS(7));

	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	mainMachine.set_state(MainMachine::manual);
	do_loop(mainMachine, MIN2MS(2u), printThis);

	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	mainMachine.switch_on_manual(1);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	mainMachine.switch_on_manual(0);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	mainMachine.switch_off_manual(0);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	mainMachine.switch_off_manual(1);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	mainMachine.switch_on_manual(0);
	mainMachine.switch_on_manual(1);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	// back to Winter
	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, MIN2MS(2u) +1, printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	do_loop(mainMachine, MIN2MS(3u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
}

TEST(TC_MainMachine, SwitchMainStates) {
	const bool printThis = false;

	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	RtcSocket* rtcSocket = new RtcSocket();
	MainMachine mainMachine(rtcSocket);
	RTC_TimeTypeDef sTime = { 1,  0, 0}; // same as in SchedulerSummer.cpp
	RTC_DateTypeDef sDate = { RTC_WEEKDAY_SUNDAY, 1, 2 ,3 };

	// startup with no settings
	do_loop(mainMachine, 3000u, false);
	mainMachine.set_tactiveMax_winter(0, MIN2MS(5));
	mainMachine.set_tactiveMax_winter(1, MIN2MS(5));
	mainMachine.set_tpauseMax_winter(0, MIN2MS(3));
	mainMachine.set_tpauseMax_winter(1, MIN2MS(7));

	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	mainMachine.set_state(MainMachine::summer);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	sTime = { 1,  0, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	do_loop(mainMachine, 1u, false);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	mainMachine.set_state(MainMachine::manual);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);

	mainMachine.switch_on_manual(0);
	mainMachine.switch_on_manual(1);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	mainMachine.set_state(MainMachine::winter);
	do_loop(mainMachine, MIN2MS(2u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), false);
	EXPECT_EQ(mainMachine.isPumpActive(1), true);

	do_loop(mainMachine, MIN2MS(5u), printThis);
	EXPECT_EQ(mainMachine.isPumpActive(0), true);
	EXPECT_EQ(mainMachine.isPumpActive(1), false);
}

