/*
 * TC_HeizScheduler.cpp
 *
 *  Created on: 30.05.2019
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <string.h>

#include "../../../Application/HeizScheduler/HeizScheduler.h"
#include "../../../Framework/Instances/Common.h"

using ::testing::Expectation;

#define MIN2MS(x) (x*60*1000)

void do_cycle(HeizScheduler& scheduler, uint16_t count)
{
	for(uint16_t i=0; i < count; i++) {
		scheduler.cycle();
	}
}

void do_loop(HeizScheduler& scheduler, uint32_t time_ms) {
	uint32_t tick = Common::get_tick();

	for (uint32_t i=0; i < time_ms; i++) {
		Common::set_fakeTick(++tick);
		scheduler.cycle();
	}
}

void dump_times(HeizScheduler& scheduler) {
	for (uint8_t i=0; i < scheduler.get_threadListlen(); i++) {

		std::string state;
		switch (scheduler.get_state(i)) {
		case ThreadBasis::init:
			state = "init";
			break;
		case ThreadBasis::active:
			state = "active";
			break;
		case ThreadBasis::pause:
			state = "pause";
			break;
		case ThreadBasis::ready:
			state = "ready";
			break;
		default:
			state = "*error*";
		}

		printf("thread %i: state=%s tact=%i tpaus=%i tactMax=%i tpausMax=%i\n",
				i,
				state.c_str(),
				scheduler.get_tactive(i),
				scheduler.get_tpause(i),
				scheduler.get_tactiveMax(i),
				scheduler.get_tpauseMax(i)
				);
	}
}

TEST(TC_HeizScheduler, many_cycles) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	HeizScheduler heizScheduler(3);
	heizScheduler.set_tactiveMax(0, MIN2MS(2));
	heizScheduler.set_tpauseMax (0, MIN2MS(10));
	heizScheduler.set_tactiveMax(1, MIN2MS(3));
	heizScheduler.set_tpauseMax (1, MIN2MS(9));
	heizScheduler.set_tactiveMax(2, MIN2MS(4));
	heizScheduler.set_tpauseMax (2, MIN2MS(8));

	EXPECT_EQ(heizScheduler.get_threadListlen(), 3);

	// first cycle: all threads init => ready, two cycles needed for sync
	do_cycle(heizScheduler, 3);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	for(int j=0; j < 5; j++) {  // run many complete cycles
		do_loop(heizScheduler, MIN2MS(2));
		do_cycle(heizScheduler, 2);
		EXPECT_EQ(heizScheduler.get_active_thread(), 1);

		do_loop(heizScheduler, MIN2MS(3));
		do_cycle(heizScheduler, 2);
		EXPECT_EQ(heizScheduler.get_active_thread(), 2);

		do_loop(heizScheduler, MIN2MS(4));
		do_cycle(heizScheduler, 2);
		EXPECT_EQ(heizScheduler.get_active_thread(), -1);

		do_loop(heizScheduler, MIN2MS(3));
		do_cycle(heizScheduler, 2);
		EXPECT_EQ(heizScheduler.get_active_thread(), 0);
	}
}

TEST(TC_HeizScheduler, requeuing) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	HeizScheduler heizScheduler(2);
	heizScheduler.set_tactiveMax(0, MIN2MS(2));
	heizScheduler.set_tpauseMax (0, MIN2MS(2));
	heizScheduler.set_tactiveMax(1, MIN2MS(2));
	heizScheduler.set_tpauseMax (1, MIN2MS(4));

	EXPECT_EQ(heizScheduler.get_threadListlen(), 2);

	// one cycle: all threads init => ready, two cycles needed for sync
	do_cycle(heizScheduler, 3);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), 1);

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), -1);

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), 1); // thread 0 requed

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), -1);

	do_loop(heizScheduler, MIN2MS(2));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), 1); // thread 0 requed
}

TEST(TC_HeizScheduler, values) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	HeizScheduler heizScheduler(2);
	heizScheduler.set_tactiveMax(0, MIN2MS(4u));
	heizScheduler.set_tpauseMax (0, MIN2MS(10u));
	heizScheduler.set_tactiveMax(1, MIN2MS(2u));
	heizScheduler.set_tpauseMax (1, MIN2MS(5u));

	EXPECT_EQ(heizScheduler.get_threadListlen(), 2u);

	// one cycle: all threads init => ready, two cycles needed for sync
	do_cycle(heizScheduler, 3);
	EXPECT_EQ(heizScheduler.get_active_thread(), 0);

	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(4u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(3u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(2u) );
	do_loop(heizScheduler, MIN2MS(1));
	EXPECT_EQ(heizScheduler.get_tactive(0), MIN2MS(1u) );
	do_loop(heizScheduler, MIN2MS(1));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_active_thread(), 1);

	dump_times(heizScheduler);

	do_loop(heizScheduler, MIN2MS(1));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(9u) );
	EXPECT_EQ(heizScheduler.get_tactive(1), MIN2MS(1u) );

	do_loop(heizScheduler, MIN2MS(1));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(8u) );
	EXPECT_EQ(heizScheduler.get_tpause(1), MIN2MS(5u) );

	do_loop(heizScheduler, MIN2MS(1));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(7u) );
	EXPECT_EQ(heizScheduler.get_tpause(1), MIN2MS(4u) );

	do_loop(heizScheduler, MIN2MS(1));
	do_cycle(heizScheduler, 2);
	EXPECT_EQ(heizScheduler.get_tpause(0), MIN2MS(6u) );
	EXPECT_EQ(heizScheduler.get_tpause(1), MIN2MS(3u) );

	dump_times(heizScheduler);

}
