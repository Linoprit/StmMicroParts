/*
 * TC_HeizScheduler.cpp
 *
 *  Created on: 30.05.2019
 *      Author: harald
 */

#include <HeizStateMachine/StateWinter/SchedulerWinter.h>
#include <HeizStateMachine/StateMachineInterface.h>
#include "HeizMachineCommons.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <string.h>

#include <Instances/Common.h>

using ::testing::Expectation;

//#define PRINT


void do_loop(SchedulerWinter& scheduler, uint32_t time_ms) {
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

TEST(TC_HeizScheduler, many_cycles) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	SchedulerWinter heizScheduler(3);
	heizScheduler.set_tactiveMax(0, MIN2MS(2));
	heizScheduler.set_tpauseMax (0, MIN2MS(10));
	heizScheduler.set_tactiveMax(1, MIN2MS(3));
	heizScheduler.set_tpauseMax (1, MIN2MS(9));
	heizScheduler.set_tactiveMax(2, MIN2MS(4));
	heizScheduler.set_tpauseMax (2, MIN2MS(8));

	EXPECT_EQ(heizScheduler.get_threadListlen(), 3);

	// we need one loop in advance, so states are changed, when we check them
	do_loop(heizScheduler, 1);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	for(int j=0; j < 5; j++) {  // run many complete cycles
		do_loop(heizScheduler, MIN2MS(2));
		EXPECT_EQ(heizScheduler.get_active_thread(), 1);

		do_loop(heizScheduler, MIN2MS(3));
		EXPECT_EQ(heizScheduler.get_active_thread(), 2);

		do_loop(heizScheduler, MIN2MS(4));
		EXPECT_EQ(heizScheduler.get_active_thread(), -1);

		do_loop(heizScheduler, MIN2MS(3));
		EXPECT_EQ(heizScheduler.get_active_thread(), 0);
	}
}


TEST(TC_HeizScheduler, requeuing) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	SchedulerWinter heizScheduler(2);
	heizScheduler.set_tactiveMax(0, MIN2MS(2));
	heizScheduler.set_tpauseMax (0, MIN2MS(2));
	heizScheduler.set_tactiveMax(1, MIN2MS(2));
	heizScheduler.set_tpauseMax (1, MIN2MS(4));

	EXPECT_EQ(heizScheduler.get_threadListlen(), 2);

	do_loop(heizScheduler, 1);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), 1);

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), -1);

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), 1); // thread 0 requed

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), -1);

	do_loop(heizScheduler, MIN2MS(2));
	EXPECT_EQ(heizScheduler.get_active_thread(), 1); // thread 0 requed
}

TEST(TC_HeizScheduler, values) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	SchedulerWinter heizScheduler(2);
	heizScheduler.set_tactiveMax(0, MIN2MS(4u));
	heizScheduler.set_tpauseMax (0, MIN2MS(10u));
	heizScheduler.set_tactiveMax(1, MIN2MS(2u));
	heizScheduler.set_tpauseMax (1, MIN2MS(5u));

	EXPECT_EQ(heizScheduler.get_threadListlen(), 2u);

	do_loop(heizScheduler, 1);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(4u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(3u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(2u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(1u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_active_thread(), 1);

	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(9u) );
	EXPECT_EQ(heizScheduler.get_tactive(1), MIN2MS(1u) );

	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(8u) );
	EXPECT_EQ(heizScheduler.get_tpause(1), MIN2MS(5u) );

	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(7u) );
	EXPECT_EQ(heizScheduler.get_tpause(1), MIN2MS(4u) );

	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(6u) );
	EXPECT_EQ(heizScheduler.get_tpause(1), MIN2MS(3u) );
}
