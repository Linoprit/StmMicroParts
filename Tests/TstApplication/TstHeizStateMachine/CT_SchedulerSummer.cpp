/*
 * TC_SchedulerSummer.cpp
 *
 *  Created on: 21.07.2019
 *      Author: harald
 */

#include <HeizStateMachine/StateSummer/SchedulerSummer.h>
#include <HeizStateMachine/StateMachineInterface.h>
#include "HeizMachineCommons.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <string.h>

#include <Instances/Common.h>


using ::testing::Expectation;
using ::SummerMachine::SchedulerSummer;

//#define PRINT

void do_loop(SchedulerSummer& scheduler, uint32_t time_ms) {
	uint32_t tick = Common::get_tick() + 1;
	const uint32_t step = 1;

	for (uint32_t i=0; i < time_ms; i+=step ) {
		Common::set_fakeTick(tick + i);
		scheduler.cycle();

#ifdef PRINT
		if (i == 0)
			printf("cyc \t thr \t state \t tactiv \t tpaus \t tactMax \t tpausMax \n");

		for (uint8_t j=0; j < scheduler.get_threadListlen(); j++) {
			printf("%i \t %i \t %s \t %i \t %i \t %i \t %i \n",
					i,
					j,
					state2str(scheduler.get_state(j)).c_str(),
					scheduler.get_tactive(j),
					scheduler.get_tpause(j),
					scheduler.get_tactiveMax(j),
					scheduler.get_tpauseMax(j)
			);
		}
#endif
	}
}


TEST(TC_SummerScheduler, many_cycles) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	RtcSocket* rtcSocket = new RtcSocket();
	SchedulerSummer summerScheduler(rtcSocket, 3u);
	EXPECT_EQ(summerScheduler.get_threadListlen(), 3);

	// nothing should happen
	do_loop(summerScheduler, 3);
	EXPECT_EQ(summerScheduler.get_active_thread(), -1); // no active thread

	// set time-date to next service point
	RTC_TimeTypeDef sTime = { 1,  0, 0}; // same as in SchedulerSummer.cpp
	RTC_DateTypeDef sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);

	do_loop(summerScheduler, 3);
	EXPECT_EQ(summerScheduler.get_active_thread(), 0);

	do_loop(summerScheduler, MIN2MS(5) + 1);
	sTime = { 1,  5, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	EXPECT_EQ(summerScheduler.get_active_thread(), 1);

	do_loop(summerScheduler, MIN2MS(5) + 1);
	sTime = { 1,  10, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	EXPECT_EQ(summerScheduler.get_active_thread(), 2);

	do_loop(summerScheduler, MIN2MS(5) + 1);
	sTime = { 1,  15, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	EXPECT_EQ(summerScheduler.get_active_thread(), -1);

	// some days later...

	Common::set_fakeTick(Common::get_tick() + (3 * 24 * 60 * 1000));
	do_loop(summerScheduler, MIN2MS(5));
	EXPECT_EQ(summerScheduler.get_active_thread(), -1);

	// set time-date to next service point
	sTime = { 1,  0, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);

	do_loop(summerScheduler, 3);
	EXPECT_EQ(summerScheduler.get_active_thread(), 0);

	do_loop(summerScheduler, MIN2MS(5) + 1);
	sTime = { 1,  5, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	EXPECT_EQ(summerScheduler.get_active_thread(), 1);

	do_loop(summerScheduler, MIN2MS(5) + 1);
	sTime = { 1,  10, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	EXPECT_EQ(summerScheduler.get_active_thread(), 2);

	do_loop(summerScheduler, MIN2MS(5) + 1);
	sTime = { 1,  15, 0}; // same as in SchedulerSummer.cpp
	sDate = { RTC_WEEKDAY_WEDNESDAY, 1, 2 ,3 };
	rtcSocket->set_time_date(sTime, sDate);
	EXPECT_EQ(summerScheduler.get_active_thread(), -1);

}

