/*
 * TU_ThreadList.cpp
 *
 *  Created on: 10.06.2019
 *      Author: harald
 */

#include <HeizStateMachine/StateMachineList.h>
#include <HeizStateMachine/StateWinter/SchedulerWinter.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <Instances/Common.h>


using ::testing::Expectation;

//#define MIN2MS(x) (uint32_t)(x*60*1000)

TEST(TU_ThreadList, eins) {
	StateMachineList::MachineList<StateMachineWinter>  threadList(3);

	StateMachineWinter* thread = threadList.get(0);
	thread->set_tactiveMax(MIN2MS(2));
	thread->set_tpauseMax(MIN2MS(10));

	thread = threadList.get(1);
	thread->set_tactiveMax(MIN2MS(3));
	thread->set_tpauseMax(MIN2MS(11));

	thread = threadList.get(2);
	thread->set_tactiveMax(MIN2MS(4));
	thread->set_tpauseMax(MIN2MS(12));

	thread = threadList.get(0);
	EXPECT_EQ(thread->get_tactiveMax(), MIN2MS(2u));
	EXPECT_EQ(thread->get_tpauseMax(),  MIN2MS(10u));

	thread = threadList.get(1);
	EXPECT_EQ(thread->get_tactiveMax(), MIN2MS(3u));
	EXPECT_EQ(thread->get_tpauseMax(),  MIN2MS(11u));

	thread = threadList.get(2);
	EXPECT_EQ(thread->get_tactiveMax(), MIN2MS(4u));
	EXPECT_EQ(thread->get_tpauseMax(),  MIN2MS(12u));


}




