/*
 * TU_Adjustment_0p0.cpp
 *
 *  Created on: 11.05.2020
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../../Framework/Types/Adjustment_0p0.h"

using ::testing::Expectation;

TEST(Adjustment, increment) {

	Adjustment_0p0 adj(0u, -20.2f, 40.1f);
	EXPECT_EQ(0, adj.get());
	EXPECT_EQ(401, adj.getMax());
	EXPECT_EQ(-202, adj.getMin());

	adj.increment(); // 1.0
	adj.toggleFineFlag();
	adj.increment(); // 1.1
	adj.increment(); // 1.2
	EXPECT_EQ((int32_t) 12u, adj.get());
	EXPECT_EQ(true, adj.isFineFlag());

	adj.toggleFineFlag();
	EXPECT_EQ(false, adj.isFineFlag());
	adj.increment(); // 2.0
	EXPECT_EQ((int32_t) 20u, adj.get());

	adj.set((int32_t) 0u);
	// coarse increment to max
	for (uint i=0; i < 42u; i++) {
		adj.increment();
	}
	EXPECT_EQ((int32_t) 401u, adj.get());

	// fine increment to max
	adj.set((int32_t) 390u);
	adj.toggleFineFlag();
	for (uint i=0; i < 40u; i++) {
		adj.increment();
	}
	EXPECT_EQ((int32_t) 401u, adj.get());
}

TEST(Adjustment, decrement) {
	Adjustment_0p0 adj(0u, -20.2f, 40.1f);
	EXPECT_EQ(0, adj.get());

	adj.set((int32_t) 0u);
	adj.decrement(); // -1
	adj.toggleFineFlag();
	adj.decrement(); // -1.1
	EXPECT_EQ((int32_t) -11u, adj.get());
	EXPECT_EQ(true, adj.isFineFlag());

	adj.toggleFineFlag();
	EXPECT_EQ(false, adj.isFineFlag());
	adj.decrement(); // -2.0
	EXPECT_EQ((int32_t) -20u, adj.get());

	// coarse decrement to min
	for (uint i=0; i < 40u; i++) {
		adj.decrement();
	}
	EXPECT_EQ((int32_t) -202u, adj.get());

	// fine decrement to min
	adj.set((int32_t) -190u);
	adj.toggleFineFlag();
	for (uint i=0; i < 40u; i++) {
		adj.decrement();
	}
	EXPECT_EQ((int32_t) -202u, adj.get());

}


