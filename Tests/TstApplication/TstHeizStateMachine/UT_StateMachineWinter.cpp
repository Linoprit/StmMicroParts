/*
 * TU_ThreadBasis.cpp
 *
 *  Created on: 10.06.2019
 *      Author: harald
 */


#include <HeizStateMachine/StateWinter/StateMachineWinter.h>
#include <HeizStateMachine/StateMachineInterface.h>
#include "HeizMachineCommons.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <Instances/Common.h>
#include <libraries/SimpleQueue.h>


using ::testing::Expectation;

//#define PRINT

void do_loop(StateMachineWinter& thread, uint32_t time_ms) {
	uint32_t tick = Common::get_tick() + 1;
	const uint32_t step = 1;

	for (uint32_t i=0; i < time_ms; i+=step ) {
		Common::set_fakeTick(tick + i);
		thread.cycle();

#ifdef PRINT
		if (i == 0)
			printf("cyc \t state \t tactiv \t tpaus \t tactMax \t tpausMax \n");

		printf("%i \t %s \t %i \t %i \t %i \t %i \n",
				i,
				state2str(thread.get_state()).c_str(),
				thread.get_tactive(),
				thread.get_tpause(),
				thread.get_tactiveMax(),
				thread.get_tpauseMax()
		);
#endif
	}
}


TEST(TU_ThreadBasis, fiveCycles) {
	uint32_t tick = 100;
	Common::set_tick_faking(true);
	Common::set_fakeTick(tick);
	SimpleQueue<int8_t>* threadQueue = new SimpleQueue<int8_t>(2);
	StateMachineWinter thread(threadQueue, 5);

	EXPECT_EQ(thread.get_taskID(), 5);
	EXPECT_EQ(thread.get_state(), StateMachineWinter::init);
	thread.set_tactiveMax(MIN2MS(4u));
	thread.set_tpauseMax(MIN2MS(10u));

	for (uint8_t i=0; i < 5; i++) {
		Common::set_fakeTick(++tick);
		thread.cycle(); // entry
		thread.cycle(); // first cycle
		EXPECT_EQ(thread.get_state(), StateMachineWinter::ready);
		EXPECT_EQ(threadQueue->dequeue(), 5);

		thread.set_go();
		Common::set_fakeTick(++tick);
		thread.cycle();	// transition ready => active
		thread.cycle(); // entry active
		EXPECT_EQ(thread.get_state(), StateMachineWinter::active);
		EXPECT_EQ(thread.get_tactive(), MIN2MS(4u));

		tick = tick + MIN2MS(4u);
		Common::set_fakeTick(++tick);
		thread.cycle(); // transition ready => pause
		thread.cycle(); // first cycle
		EXPECT_EQ(thread.get_state(), StateMachineWinter::pause);
		EXPECT_EQ(thread.get_tpause(), MIN2MS(10u));

		tick = tick + MIN2MS(5u);
		Common::set_fakeTick(tick);
		thread.cycle();
		EXPECT_EQ(thread.get_state(), StateMachineWinter::pause);
		EXPECT_EQ(thread.get_tpause(), MIN2MS(5u));

		tick = tick + MIN2MS(5);
	}
}

TEST(TU_ThreadBasis, DISABLED_preemption) {
	uint32_t tick = 100;
	Common::set_tick_faking(true);
	Common::set_fakeTick(tick);
	SimpleQueue<int8_t> threadQueue(2);
	StateMachineWinter thread(&threadQueue, 15);

	EXPECT_EQ(thread.get_taskID(), 15);
	EXPECT_EQ(thread.get_state(), StateMachineWinter::init);
	thread.set_tactiveMax(10*60*1000u);
	thread.set_tpauseMax(20*60*1000u);

	for (uint8_t i=0; i < 5; i++) {
		Common::set_fakeTick(++tick);
		thread.cycle(); // entry
		thread.cycle(); // first cycle
		EXPECT_EQ(thread.get_state(), StateMachineWinter::ready);
		EXPECT_EQ(threadQueue.dequeue(), 15);

		thread.set_go();
		Common::set_fakeTick(++tick);
		thread.cycle();	// transition ready => active
		thread.cycle(); // entry active
		EXPECT_EQ(thread.get_state(), StateMachineWinter::active);
		EXPECT_EQ(thread.get_tactive(), 10*60*1000u);

		tick = tick + (1*60*1000u);
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.reset_go(); // preempt
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.cycle();
		EXPECT_EQ(thread.get_state(), StateMachineWinter::ready);
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
		EXPECT_EQ(thread.get_state(), StateMachineWinter::active);

		tick = tick + (10*60*1000u);
		Common::set_fakeTick(++tick);
		thread.cycle();
		thread.cycle();
		EXPECT_EQ(thread.get_state(), StateMachineWinter::pause);
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
	StateMachineWinter thread(&threadQueue, 5);

	EXPECT_EQ(thread.get_taskID(), 5);
	EXPECT_EQ(thread.get_state(), StateMachineWinter::init);
	thread.set_tactiveMax(MIN2MS(5u));
	thread.set_tpauseMax(MIN2MS(10u));

	do_loop(thread, 1); // init => ready
	threadQueue.dequeue(); // avoid queue overflow
	EXPECT_EQ(thread.get_state(), StateMachineWinter::ready);
	EXPECT_EQ(thread.get_tactive(), MIN2MS(5u));

	thread.set_go(); // next cycle: ready => active

	for (i=0; i < 5; i++) {
		do_loop(thread, MIN2MS(5u) + 1u);
		EXPECT_EQ(thread.get_state(), StateMachineWinter::pause);
		EXPECT_EQ(thread.get_tactive(), 0u);  //MIN2MS(5u));
		EXPECT_EQ(thread.get_tpause(), MIN2MS(10u));

		do_loop(thread, MIN2MS(10));
		EXPECT_EQ(thread.get_state(), StateMachineWinter::ready);
		EXPECT_EQ(thread.get_tpause(), MIN2MS(10u));//0u);
		EXPECT_EQ(thread.get_tactive(), MIN2MS(5u));
		threadQueue.dequeue(); // avoid queue overflow

		thread.set_go();
	}
}


