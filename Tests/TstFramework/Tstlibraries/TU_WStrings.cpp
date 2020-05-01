/*
 * TU_WStrings.cpp
 *
 *  Created on: 02.11.2019
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <libraries/Arduino/WString.h>

using ::testing::Expectation;


TEST(TU_WString, simple) {
	String someString;
	someString += "blahblah";

	printf("someString: %s\n", someString.c_str());

}

