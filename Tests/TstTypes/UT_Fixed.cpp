/*
 * TU_Fixed.cpp
 *
 *  Created on: 12.05.2020
 *      Author: harald
 */


#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../../Framework/Types/Fixed.h"

using ::testing::Expectation;

TEST(Fixed, setters_exp1) {
	Fixed fxd(0u, 1, -20.2f, 40.1f);
	EXPECT_FLOAT_EQ(-20.2f, fxd.getMinFlt());
	EXPECT_FLOAT_EQ(40.1f, fxd.getMaxFlt());

	fxd.set(20.4f);
	EXPECT_EQ((int32_t) 204u, fxd.get());

	fxd.set(20.36f);
	EXPECT_EQ((int32_t) 204u, fxd.get());

	fxd.set(20.34f);
	EXPECT_EQ((int32_t) 203u, fxd.get());
	EXPECT_FLOAT_EQ(20.3f, fxd.getFlt());

	fxd.set(200.0f);
	EXPECT_EQ((int32_t) 401u, fxd.get());

	fxd.set(-200.0f);
	EXPECT_EQ((int32_t) -202u, fxd.get());
}

TEST(Fixed, setters_exp2) {
	Fixed fxd(0u, 3, -20.234f, 40.123f);
	EXPECT_FLOAT_EQ(-20.234f, fxd.getMinFlt());
	EXPECT_FLOAT_EQ(40.123f, fxd.getMaxFlt());

	fxd.set(20.456f);
	EXPECT_EQ((int32_t) 20456u, fxd.get());

	fxd.set(20.4563f);
	EXPECT_EQ((int32_t) 20456u, fxd.get());

	fxd.set(20.4566f);
	EXPECT_EQ((int32_t) 20457u, fxd.get());
	EXPECT_FLOAT_EQ(20.457f, fxd.getFlt());

	fxd.set(200000.0f);
	EXPECT_EQ((int32_t) 40123u, fxd.get());

	fxd.set(-20000.0f);
	EXPECT_EQ((int32_t) -20234u, fxd.get());
}

