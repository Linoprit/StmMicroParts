/*
 * TestQueue.cpp
 *
 *  Created on: 13.05.2019
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../../../Framework/libraries/SimpleQueue.h"

using ::testing::Expectation;


TEST(SimpleQueue, enqueue) {

	uint16_t queueSize = 5;
	SimpleQueue<uint16_t> q(queueSize);

	EXPECT_TRUE(q.isEmpty());

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);
	q.enqueue(400);
	q.enqueue(500);

	EXPECT_EQ(100, q.dequeue());
	EXPECT_EQ(200, q.dequeue());
	EXPECT_EQ(300, q.dequeue());
	EXPECT_EQ(400, q.dequeue());
	EXPECT_EQ(500, q.dequeue());
}


TEST(SimpleQueue, front) {

	uint16_t queueSize = 10;
	SimpleQueue<uint16_t> q(queueSize);

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);
	q.enqueue(400);
	q.enqueue(500);

	EXPECT_EQ(100, q.front());
	EXPECT_EQ(100, q.dequeue());
	EXPECT_EQ(200, q.front());
	EXPECT_EQ(200, q.dequeue());
}

TEST(SimpleQueue, enqueueAgain) {

	uint16_t queueSize = 3;
	SimpleQueue<uint16_t> q(queueSize);

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);

	EXPECT_EQ(100, q.dequeue());
	EXPECT_EQ(200, q.dequeue());

	q.enqueue(400);
	q.enqueue(500);

	EXPECT_EQ(300, q.front());
	EXPECT_EQ(300, q.dequeue());
}

TEST(SimpleQueue, asArray) {
	uint16_t queueSize = 5;
	SimpleQueue<uint16_t> q(queueSize);
	uint8_t i;

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);
	q.enqueue(400);
	q.enqueue(500);

	uint16_t* result = new uint16_t[queueSize];
	q.asArray(result);

	for(i=0; i < queueSize; i++) {
		uint16_t expect = 100 + i*100;
		uint16_t content = result[i];
		EXPECT_EQ(expect, content);
	}

	EXPECT_EQ(100, q.dequeue());
	q.asArray(result);
	for(i=0; i < queueSize-1; i++) {
		uint16_t expect = 200 + i*100;
		uint16_t content = result[i];
		EXPECT_EQ(expect, content);
	}

	q.enqueue(600);
	q.asArray(result);
	for(i=0; i < queueSize; i++) {
		uint16_t expect = 200 + i*100;
		uint16_t content = result[i];
		EXPECT_EQ(expect, content);
	}
}

TEST(SimpleQueue, reset) {
	uint16_t queueSize = 5;
	SimpleQueue<uint16_t> q(queueSize);

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);
	q.enqueue(400);
	q.enqueue(500);

	q.reset();

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);
	q.enqueue(400);
	q.enqueue(500);

	EXPECT_EQ(100, q.dequeue());
	EXPECT_EQ(200, q.dequeue());
	EXPECT_EQ(300, q.dequeue());
	EXPECT_EQ(400, q.dequeue());
	EXPECT_EQ(500, q.dequeue());
}

TEST(SimpleQueue, exceptions) {

	uint16_t queueSize = 4;
	SimpleQueue<uint16_t> q(queueSize);

	q.enqueue(100);
	q.enqueue(200);
	q.enqueue(300);
	q.enqueue(400);

	EXPECT_THROW(q.enqueue(500), QueueOverFlowException);
}

TEST(SimpleQueue, classtype) {
	class MyClass {
	public:
		MyClass (void) {
			value_a = 0;
			value_b = 0;
			value_c = 0;
		}
		MyClass(int a, uint8_t b, uint16_t c) {
			value_a = a;
			value_b = b;
			value_c = c;
		}
		int 	 get_a (void){return value_a;}
		uint8_t  get_b (void){return value_b;}
		uint16_t get_c (void){return value_c;}
	private:
		int 		value_a;
		uint8_t 	value_b;
		uint16_t 	value_c;
	};

	MyClass myClass_1(-40, 33, 2048);
	MyClass myClass_2(-6589, 254, 3021);
	MyClass myClass_3(2056, 127, 30302);
	MyClass myClass_4(456, 5, 28);

	uint16_t queueSize = 4;
	SimpleQueue<MyClass> q(queueSize);

	q.enqueue(myClass_1);
	q.enqueue(myClass_2);
	q.enqueue(myClass_3);
	q.enqueue(myClass_4);

	MyClass myClass_11 = q.front();
	EXPECT_EQ(-40,  myClass_11.get_a());
	EXPECT_EQ(33,   myClass_11.get_b());
	EXPECT_EQ(2048, myClass_11.get_c());

	MyClass myClass_12 = q.dequeue();
	MyClass myClass_21 = q.dequeue();
	MyClass myClass_31 = q.dequeue();
	MyClass myClass_41 = q.dequeue();

	EXPECT_EQ(-40,   myClass_12.get_a());
	EXPECT_EQ(33,    myClass_12.get_b());
	EXPECT_EQ(2048,  myClass_12.get_c());
	EXPECT_EQ(-6589, myClass_21.get_a());
	EXPECT_EQ(254,   myClass_21.get_b());
	EXPECT_EQ(3021,  myClass_21.get_c());
	EXPECT_EQ(2056,  myClass_31.get_a());
	EXPECT_EQ(127,   myClass_31.get_b());
	EXPECT_EQ(30302, myClass_31.get_c());
	EXPECT_EQ(456,   myClass_41.get_a());
	EXPECT_EQ(5,     myClass_41.get_b());
	EXPECT_EQ(28,    myClass_41.get_c());
}

