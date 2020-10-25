/*
 * TstEeprom_93C86_socket.cpp
 *
 *  Created on: 27.09.2020
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../Framework/Devices/Eeprom_93C86/Eeprom_93C86_socket.h"

using ::testing::Expectation;

TEST(Eeprom_93C86_socket, writingToDummy) {
	Eeprom_93C86_socket sock = Eeprom_93C86_socket();

	uint8_t* buffer = sock.get_buffer();
	for (uint16_t i=0; i < sock.getMaxAddress(); i++)
		EXPECT_EQ(buffer[i], 0xFF); // brand new EE

	uint16_t i=0;
	uint8_t  j=20;

	const uint16_t address = 50;
	const uint8_t  blkLen  = 100;
	uint8_t wData[blkLen];
	uint8_t rData[blkLen];
	// prepare Datablock, that will be written to EE
	for(i=0; i < blkLen; i++) {
		wData[i] = j + i;
	}

	// write disabled
	sock.writeBlock(address, &wData[0], blkLen);
	sock.readBlock(address, &rData[0], blkLen);
	int result = memcmp( wData, rData,  sock.getMaxAddress()+1u );
	EXPECT_FALSE(result == 0);

	// write enabled
	sock.write_enable(); // never forget!
	sock.writeBlock(address, &wData[0], blkLen);
	sock.readBlock(address, &rData[0], blkLen);
	result = memcmp( wData, rData,  blkLen );
	EXPECT_EQ(result, 0);
}

TEST(Eeprom_93C86_socket, writeall) {
	Eeprom_93C86_socket sock = Eeprom_93C86_socket();

	sock.write_enable();
	sock.writeall(123);

	uint8_t* buffer = sock.get_buffer();
	for (uint16_t i=0; i < sock.getMaxAddress(); i++)
		EXPECT_EQ(buffer[i], 123u);

	sock.eraseall();
	for (uint16_t i=0; i < sock.getMaxAddress(); i++)
		EXPECT_EQ(buffer[i], 0xFF); // brand new EE

	sock.write_disable();
	sock.write(250, 45u);
	uint8_t result = sock.read(250);
	EXPECT_EQ(result, 255u);
}


