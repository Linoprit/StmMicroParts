/*
 * TU_ThreadList.cpp
 *
 *  Created on: 10.06.2019
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../../../Application/HeizScheduler/HeizScheduler.h"
#include "../../../Framework/Instances/Common.h"

using ::testing::Expectation;

#define MIN2MS(x) (uint32_t)(x*60*1000)

TEST(TU_ThreadList, eins) {
	ThreadList threadList(3);

	ThreadBasis* thread = threadList.get(0);
	thread->set_tactiveMax(MIN2MS(2));
	thread->set_tpauseMax(MIN2MS(10));

	thread = threadList.get(1);
	thread->set_tactiveMax(MIN2MS(3));
	thread->set_tpauseMax(MIN2MS(11));

	thread = threadList.get(2);
	thread->set_tactiveMax(MIN2MS(4));
	thread->set_tpauseMax(MIN2MS(12));

	thread = threadList.get(0);
	EXPECT_EQ(thread->get_tactiveMax(), MIN2MS(2));
	EXPECT_EQ(thread->get_tpauseMax(),  MIN2MS(10));

	thread = threadList.get(1);
	EXPECT_EQ(thread->get_tactiveMax(), MIN2MS(3));
	EXPECT_EQ(thread->get_tpauseMax(),  MIN2MS(11));

	thread = threadList.get(2);
	EXPECT_EQ(thread->get_tactiveMax(), MIN2MS(4));
	EXPECT_EQ(thread->get_tpauseMax(),  MIN2MS(12));


}




