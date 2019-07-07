/*
 * TCManualMachine.cpp
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#include <HeizStateMachine/ManualMachine/ManualMachineScheduler.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <string.h>

#include <Instances/Common.h>


using ::testing::Expectation;


TEST(TC_ManualMachine, simple_on_off) {
	Common::set_tick_faking(true);
	Common::set_fakeTick(100u);

	const uint8_t nrOfInstances = 3;
	ManualMachineScheduler manMachScheduler(nrOfInstances);
	EXPECT_EQ(manMachScheduler.get_threadListlen(), nrOfInstances);

	for (uint8_t i=0; i < nrOfInstances; i++) {
		EXPECT_EQ(manMachScheduler.get_state(i), StateMachineInterface::pause);
	}

	for (uint8_t i=0; i < nrOfInstances; i++) {
		manMachScheduler.switch_on(i);
	}

	for (uint8_t i=0; i < nrOfInstances; i++) {
		EXPECT_EQ(manMachScheduler.get_state(i), StateMachineInterface::active);
	}

	manMachScheduler.switch_off(1);
	EXPECT_EQ(manMachScheduler.get_state(0), StateMachineInterface::active);
	EXPECT_EQ(manMachScheduler.get_state(1), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(2), StateMachineInterface::active);

	manMachScheduler.switch_off(0);
	EXPECT_EQ(manMachScheduler.get_state(0), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(1), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(2), StateMachineInterface::active);

	manMachScheduler.switch_off(2);
	EXPECT_EQ(manMachScheduler.get_state(0), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(1), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(2), StateMachineInterface::pause);

	manMachScheduler.switch_on(2);
	EXPECT_EQ(manMachScheduler.get_state(0), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(1), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(2), StateMachineInterface::active);

	manMachScheduler.switch_on(0);
	EXPECT_EQ(manMachScheduler.get_state(0), StateMachineInterface::active);
	EXPECT_EQ(manMachScheduler.get_state(1), StateMachineInterface::pause);
	EXPECT_EQ(manMachScheduler.get_state(2), StateMachineInterface::active);

	manMachScheduler.switch_on(1);
	EXPECT_EQ(manMachScheduler.get_state(0), StateMachineInterface::active);
	EXPECT_EQ(manMachScheduler.get_state(1), StateMachineInterface::active);
	EXPECT_EQ(manMachScheduler.get_state(2), StateMachineInterface::active);
}
