/*
 * TU_Adjustment_0pxx.cpp
 *
 *  Created on: 14.05.2020
 *      Author: harald
 */



#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../../Framework/Types/Adjustment_0pxx.h"

using ::testing::Expectation;

TEST(Adustment_0pxx, incActualPlaces) {

	Adjustment_0pxx adj(1234, 4, 0.0010f, 0.2000f);

	EXPECT_EQ(0, adj.getActualPlace());
	adj.incrementActualPlace();
	EXPECT_EQ(1, adj.getActualPlace());
	adj.incrementActualPlace();
	EXPECT_EQ(2, adj.getActualPlace());
	adj.incrementActualPlace();
	EXPECT_EQ(3, adj.getActualPlace());
	adj.incrementActualPlace();
	EXPECT_EQ(4, adj.getActualPlace());
	adj.incrementActualPlace();
	EXPECT_EQ(0, adj.getActualPlace());
}

TEST(Adustment_0pxx, increment) {

	Adjustment_0pxx adj(1234, 4, 0.0010f, 0.2000f);

	adj.decrement();
	EXPECT_EQ(10, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(1, adj.getActualPlace());
	adj.increment();
	EXPECT_EQ(1010, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(2, adj.getActualPlace());
	adj.increment();
	adj.increment();
	adj.increment();
	EXPECT_EQ(1310, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(3, adj.getActualPlace());
	adj.increment();
	adj.increment();
	adj.increment();
	adj.increment();
	EXPECT_EQ(1350, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(4, adj.getActualPlace());
	adj.increment();
	EXPECT_EQ(1351, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(0, adj.getActualPlace());
	adj.increment();
	EXPECT_EQ(2000, adj.get());

}

TEST(Adustment_0pxx, decrement) {

	Adjustment_0pxx adj(1234, 4, 0.0010f, 0.2000f);
	adj.set(2000);

	adj.incrementActualPlace();
	EXPECT_EQ(1, adj.getActualPlace());
	adj.decrement();
	EXPECT_EQ(1000, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(2, adj.getActualPlace());
	adj.decrement();
	adj.decrement();
	adj.decrement();
	EXPECT_EQ(700, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(3, adj.getActualPlace());
	adj.decrement();
	adj.decrement();
	adj.decrement();
	adj.decrement();
	EXPECT_EQ(660, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(4, adj.getActualPlace());
	adj.decrement();
	EXPECT_EQ(659, adj.get());

	adj.incrementActualPlace();
	EXPECT_EQ(0, adj.getActualPlace());
	adj.decrement();
	EXPECT_EQ(10, adj.get());
}


