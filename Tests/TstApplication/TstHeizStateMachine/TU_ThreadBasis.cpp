/*
 * TU_ThreadBasis.cpp
 *
 *  Created on: 10.06.2019
 *      Author: harald
 */


#include <HeizStateMachine/WinterMachine/ThreadBasis.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <Instances/Common.h>
#include <libraries/SimpleQueue.h>


using ::testing::Expectation;

#define MIN2MS(x) (x*60*1000)

TEST(TU_ThreadBasis, fiveCycles) {
	uint32_t tick = 100;
	Common::set_tick_faking(true);
	Common::set_fakeTick(tick);
	SimpleQueue<int8_t>* threadQueue = new SimpleQueue<int8_t>(2);
	ThreadBasis thread(threadQueue, 5);

	EXPECT_EQ(thread.get_taskID(), 5);
	EXPECT_EQ(thread.get_state(), ThreadBasis::init);
	thread.set_tactiveMax(MIN2MS(4u));
	thread.set_tpauseMax(MIN2MS(10u));

	for (uint8_t i=0; i < 5; i++) {
		Common::set_fakeTick(++tick);
		thread.cycle(); // entry
		thread.cycle(); // first cycle
		EXPECT_EQ(thread.get_state(), ThreadBasis::ready);
		EXPECT_EQ(threadQueue->dequeue(), 5);

		thread.set_go();
		Common::set_fakeTick(++tick);
		thread.cycle();	// transition ready => active
		thread.cycle(); // entry active
		EXPECT_EQ(thread.get_state(), ThreadBasis::active);
		EXPECT_EQ(thread.get_tactive(), MIN2MS(4u));

		tick = tick + MIN2MS(4u);
		Common::set_fakeTick(++tick);
		thread.cycle(); // transition ready => pause
		thread.cycle(); // first cycle
		EXPECT_EQ(thread.get_state(), ThreadBasis::pause);
		EXPECT_EQ(thread.get_tpause(), MIN2MS(10u));

		tick = tick + MIN2MS(5u);
		Common::set_fakeTick(tick);
		thread.cycle();
		EXPECT_EQ(thread.get_state(), ThreadBasis::pause);
		EXPECT_EQ(thread.get_tpause(), MIN2MS(5u));

		tick = tick + MIN2MS(5);
	}
}

TEST(TU_ThreadBasis, DISABLED_preemption) {
	uint32_t tick = 100;
	Common::set_tick_faking(true);
	Common::set_fakeTick(tick);
	SimpleQueue<int8_t> threadQueue(2);
	ThreadBasis thread(&threadQueue, 15);

	EXPECT_EQ(thread.get_taskID(), 15);
	EXPECT_EQ(thread.get_state(), ThreadBasis::init);
	thread.set_tactiveMax(10*60*1000u);
	thread.set_tpauseMax(20*60*1000u);

	for (uint8_t i=0; i < 5; i++) {
		Common::set_fakeTick(++tick);
		thread.cycle(); // entry
		thread.cycle(); // first cycle
		EXPECT_EQ(thread.get_state(), ThreadBasis::ready);
		EXPECT_EQ(threadQueue.dequeue(), 15);

		thread.set_go();
		Common::set_fakeTick(++tick);
		thread.cycle();	// transition ready => active
		thread.cycle(); // entry active
		EXPECT_EQ(thread.get_state(), ThreadBasis::active);
		EXPECT_EQ(thread.get_tactive(), 10*60*1000u);

		tick = tick + (1*60*1000u);
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.reset_go(); // preempt
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.cycle();
		EXPECT_EQ(thread.get_state(), ThreadBasis::ready);
		EXPECT_EQ(threadQueue.dequeue(), 15);
		EXPECT_EQ(thread.get_tactive(), 9*60*1000u - 2u);

		tick = tick + (1*60*1000u);
		Common::set_fakeTick(tick);
		thread.cycle();
		EXPECT_EQ(thread.get_tactive(), 9*60*1000u - 2u);

		thread.set_go();
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.cycle();
		EXPECT_EQ(thread.get_state(), ThreadBasis::active);

		tick = tick + (10*60*1000u);
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.cycle();
		EXPECT_EQ(thread.get_state(), ThreadBasis::pause);
		EXPECT_EQ(thread.get_tpause(), 20*60*1000u);

		tick = tick + (20*60*1000u);
	}
}

TEST(TU_ThreadBasis, realTime) {
	uint32_t i;
	uint32_t tick = 100;
	Common::set_tick_faking(true);
	Common::set_fakeTick(tick);
	SimpleQueue<int8_t> threadQueue(2);
	ThreadBasis thread(&threadQueue, 5);

	EXPECT_EQ(thread.get_taskID(), 5);
	EXPECT_EQ(thread.get_state(), ThreadBasis::init);
	thread.set_tactiveMax(5*60*1000u);
	thread.set_tpauseMax(10*60*1000u);

	thread.cycle(); // entry
	EXPECT_EQ(thread.get_state(), ThreadBasis::ready);
	EXPECT_EQ(thread.get_tactive(), 5*60*1000u);

	Common::set_fakeTick(++tick);
	thread.cycle();
	thread.set_go();
	Common::set_fakeTick(++tick);
	thread.cycle(); // ready => active
	Common::set_fakeTick(++tick);
	thread.cycle(); // active - entry
	EXPECT_EQ(thread.get_state(), ThreadBasis::active);

	for (uint8_t j=0; j < 5; j++) {

	    EXPECT_EQ(thread.get_state(), ThreadBasis::active);
		for (i=0; i < 5*60*1000u; i++) {
			Common::set_fakeTick(++tick);
			thread.cycle();
		}
		EXPECT_EQ(thread.get_state(), ThreadBasis::pause);
		EXPECT_EQ(thread.get_tactive(), 0u);
		Common::set_fakeTick(++tick);
		thread.cycle(); // pause - entry

		for (i=0; i < 10*60*1000u; i++) {
			Common::set_fakeTick(++tick);
			thread.cycle();
		}
		EXPECT_EQ(thread.get_state(), ThreadBasis::ready);
		EXPECT_EQ(thread.get_tpause(), 0u);

		for (i=0; i < 2*60*1000u; i++) {
			Common::set_fakeTick(++tick);
			thread.cycle();
		}
		EXPECT_EQ(thread.get_state(), ThreadBasis::ready);

		EXPECT_EQ(threadQueue.dequeue(), 5);
		thread.set_go();
		Common::set_fakeTick(++tick);
		thread.cycle(); // active - entry
		Common::set_fakeTick(++tick);
		thread.cycle(); // active - state
	}
}


