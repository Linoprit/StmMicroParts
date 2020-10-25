/*
 * UT_Eeprom_93C86.cpp
 *
 *  Created on: 18.10.2020
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../Framework/Devices/Eeprom_93C86/Eeprom_93C86_socket.h"
#include "../Framework/Devices/Eeprom_93C86/Eeprom_93C86.h"

using ::testing::Expectation;
using namespace eeprom;

class Eeprom_93C86Fix: public ::testing::Test {
public:
	class byte8TestType {
	public:
		uint8_t uintValue;
		uint16_t shortValue;
		int32_t intValue;
		CrcType crc; // must be defined
	}__attribute__ ((packed));

	class byte20TestType {
	public:
		uint8_t uintValue;
		uint16_t shortValue;
		int32_t intValue0;
		int32_t intValue1;
		int32_t intValue2;
		int32_t intValue3;
		CrcType crc; // must be defined
	}__attribute__ ((packed));

	Eeprom_93C86Fix() {
	}
	;
	~Eeprom_93C86Fix() {
	}
	;
	// void SetUp() {};
	// void TearDown() {};

	void printBuffer(uint16_t address, uint8_t size) {
		uint8_t *buffer = sock.get_buffer();
		for(uint16_t i=0; i < size; i++) {
			printf("%i ", buffer[address + i]);
		}
		printf("\n");
	}

	void checkResetParams(Eeprom_93C86 eeprom) {
		EXPECT_EQ(eeprom.getNextDataAddress(), 600U + 9U);
		EXPECT_EQ(eeprom.getFreeDataSpace(), (2048U - 10U - 600U));
		EXPECT_EQ(eeprom.getNumOfJournalEntries(), 0U);
		EXPECT_EQ(eeprom.getFreeJournalEntries(), 100U);

		EXPECT_EQ(eeprom.isDataWritable(1438U), true);
		EXPECT_EQ(eeprom.isDataWritable(1439U), false);
	}

	class Eeprom_93C86_TST: public Eeprom_93C86 {
	public:
		Eeprom_93C86_TST(Eeprom_93C86_socket *socket) :
				Eeprom_93C86::Eeprom_93C86(socket) {
		}

		bool _readJournalEntry(uint8_t entryNumber,
				JournalEntryType &journalEntry) {
			return readJournalEntry(entryNumber, journalEntry);
		}
	}
	;
	Eeprom_93C86_socket sock = Eeprom_93C86_socket();
	Eeprom_93C86_TST eeprom = Eeprom_93C86_TST(&sock);

};

TEST_F(Eeprom_93C86Fix, basisTest) {
	bool result = eeprom.init();
	EXPECT_EQ(result, false);

	eeprom.eraseEeprom();
	EXPECT_EQ(sock.isWritingDisabled(), false);
	result = eeprom.init();
	EXPECT_EQ(result, true);

	checkResetParams(eeprom);
}

TEST_F(Eeprom_93C86Fix, writeAndRead) {
	eeprom.eraseEeprom();
	eeprom.init();

	bool result = false;
	byte8TestType write_tt { 125u, 1225u, -64438, 0 };
	result = eeprom.writeData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte8TestType));
	EXPECT_EQ(result, true);
	EXPECT_EQ(eeprom.getFreeJournalEntries(), 99U);
	EXPECT_EQ(eeprom.getFreeDataSpace(), (1430U));

	byte8TestType read_tt;
	result = eeprom.readData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&read_tt));
	EXPECT_EQ(result, true);
	EXPECT_EQ(read_tt.uintValue, 125u);
	EXPECT_EQ(read_tt.shortValue, 1225u);
	EXPECT_EQ(read_tt.intValue, -64438);

	// do some writing
	for (int i = 0; i < 20; i++) {
		write_tt.uintValue++;
		write_tt.shortValue++;
		write_tt.intValue--;
		result = eeprom.writeData(ValueID::CalibVal,
				reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte8TestType));
		EXPECT_EQ(result, true);
	}
	EXPECT_EQ(eeprom.getFreeJournalEntries(), 79u);
	EXPECT_EQ(eeprom.getFreeDataSpace(), (1430u - 160u));

	result = eeprom.readData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&read_tt));
	EXPECT_EQ(result, true);
	EXPECT_EQ(read_tt.uintValue, 145u);
	EXPECT_EQ(read_tt.shortValue, 1245u);
	EXPECT_EQ(read_tt.intValue, -64458);
}

TEST_F(Eeprom_93C86Fix, journalFull) {
	eeprom.eraseEeprom();
	eeprom.init();

	bool result = false;
	byte8TestType write_tt { 125u, 1225u, -64438, 0 };

	// do some writing
	for (int i = 0; i < 100; i++) {
		write_tt.uintValue++;
		write_tt.shortValue++;
		write_tt.intValue--;
		EXPECT_EQ(eeprom.isDataWritable(sizeof(byte8TestType)), true);
		result = eeprom.writeData(ValueID::CalibVal,
				reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte8TestType));
		EXPECT_EQ(result, true);

		// Look at data at the end of Journal-Space
		// printf("%i ", eeprom.getFreeJournalEntries());
		// printBuffer(609 - 30, 35);
	}
	EXPECT_EQ(eeprom.isDataWritable(sizeof(byte8TestType)), false);
	result = eeprom.writeData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte8TestType));
	EXPECT_EQ(result, false);

	eeprom.eraseEeprom();
	checkResetParams(eeprom);

	result = eeprom.writeData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte20TestType));
	EXPECT_EQ(result, true);

	byte8TestType read_tt;
	result = eeprom.readData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&read_tt));
	EXPECT_EQ(result, true);
	EXPECT_EQ(read_tt.uintValue, 125u + 100u);
	EXPECT_EQ(read_tt.shortValue, 1225u + 100u);
	EXPECT_EQ(read_tt.intValue, -64438 - 100);
}

TEST_F(Eeprom_93C86Fix, dataSpaceFull) {
	eeprom.eraseEeprom();
	eeprom.init();
	EXPECT_EQ(eeprom.getFreeDataSpace(), 1438U);
	bool result = false;
	byte20TestType write_tt { 125u, 1225u, -64438, 32468, -125236, 823456 };
	const uint8_t writeCount = 71;

	// do some writing
	for (int i = 0; i < writeCount; i++) {
		write_tt.uintValue++;
		write_tt.shortValue++;
		write_tt.intValue0--;
		write_tt.intValue1++;
		write_tt.intValue2--;
		write_tt.intValue3++;
		EXPECT_EQ(eeprom.isDataWritable(sizeof(byte8TestType)), true);
		result = eeprom.writeData(ValueID::CalibVal,
				reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte20TestType));
		EXPECT_EQ(result, true);

		//printf("%i \n", eeprom.getFreeDataSpace());
	}
	EXPECT_EQ(eeprom.isDataWritable(19U), false);

	eeprom.eraseEeprom();
	result = eeprom.writeData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte20TestType));
	EXPECT_EQ(result, true);

	byte20TestType read_tt;
	result = eeprom.readData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&read_tt));
	EXPECT_EQ(result, true);
	EXPECT_EQ(read_tt.uintValue, 125u + writeCount);
	EXPECT_EQ(read_tt.shortValue, 1225u + writeCount);
	EXPECT_EQ(read_tt.intValue0, -64438 - writeCount);
	EXPECT_EQ(read_tt.intValue1, 32468 + writeCount);
	EXPECT_EQ(read_tt.intValue2, -125236 - writeCount);
	EXPECT_EQ(read_tt.intValue3, 823456 + writeCount);
}

TEST_F(Eeprom_93C86Fix, reInitEeprom) {
	eeprom.eraseEeprom();
	eeprom.init();
	EXPECT_EQ(eeprom.getFreeDataSpace(), 1438U);
	bool result = false;

	// do some writing
	byte20TestType write_tt { 125u, 1225u, -64438, 32468, -125236, 823456 };
	for (int i = 0; i < 20; i++) {
		write_tt.uintValue++;
		write_tt.shortValue++;
		write_tt.intValue0--;
		write_tt.intValue1++;
		write_tt.intValue2--;
		write_tt.intValue3++;
		EXPECT_EQ(eeprom.isDataWritable(sizeof(byte8TestType)), true);
		result = eeprom.writeData(ValueID::CalibVal,
				reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte20TestType));
		EXPECT_EQ(result, true);
	}

	EXPECT_EQ(eeprom.init(), true);

	EXPECT_EQ(eeprom.getNextDataAddress(), 609U + 400U);
	EXPECT_EQ(eeprom.getFreeDataSpace(), (2048U - 610U - 400U));
	EXPECT_EQ(eeprom.getNumOfJournalEntries(), 20U);
	EXPECT_EQ(eeprom.getFreeJournalEntries(), 80U);

	byte20TestType read_tt;
	result = eeprom.readData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&read_tt));
	EXPECT_EQ(result, true);
	EXPECT_EQ(read_tt.uintValue, 145u);
	EXPECT_EQ(read_tt.shortValue, 1245u);
	EXPECT_EQ(read_tt.intValue0, -64458);
	EXPECT_EQ(read_tt.intValue1, 32488);
	EXPECT_EQ(read_tt.intValue2, -125256);
	EXPECT_EQ(read_tt.intValue3, 823476);
}

TEST_F(Eeprom_93C86Fix, invalidateJournal) {
	eeprom.eraseEeprom();
	eeprom.init();
	EXPECT_EQ(eeprom.getFreeDataSpace(), 1438U);
	EXPECT_EQ(eeprom.getNextDataAddress(), 9 + 6 * 100);

	bool result = false;
	uint8_t nrOfEntries = 50;
	byte8TestType write_tt { 125u, 1225u, -64438, 0 };

	// do some writing
	for (int i = 0; i < nrOfEntries; i++) {
		write_tt.uintValue++;
		write_tt.shortValue++;
		write_tt.intValue--;
		EXPECT_EQ(eeprom.isDataWritable(sizeof(byte8TestType)), true);
		result = eeprom.writeData(ValueID::CalibVal,
				reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte8TestType));
		EXPECT_EQ(result, true);
	}

	eeprom.eraseEeprom();

	Eeprom_93C86::JournalEntryType journalEntry;
	for (uint8_t i = 0; i < nrOfEntries; i++) {
		result = eeprom._readJournalEntry(i, journalEntry);
		EXPECT_EQ(result, false);
		EXPECT_EQ(journalEntry.entryID,
				static_cast<uint8_t>(ValueID::CalibVal));
		EXPECT_EQ(journalEntry.size, 8U);
		EXPECT_EQ(journalEntry.crc, 255U);
		EXPECT_EQ(journalEntry.startAddress, 609U + i * sizeof(byte8TestType));
	}
}

TEST_F(Eeprom_93C86Fix, bufferCheck) {
	bool result = false;
	uint8_t nrOfEntries = 3;
	byte8TestType write_tt { 125u, 1225u, -64438, 0 };
	uint8_t *buffer = sock.get_buffer();
	eeprom.eraseEeprom();
	eeprom.init();

	for (uint8_t i = 0; i < nrOfEntries; i++) {
		write_tt.uintValue++;
		write_tt.shortValue++;
		write_tt.intValue--;
		EXPECT_EQ(eeprom.isDataWritable(sizeof(byte8TestType)), true);
		result = eeprom.writeData(ValueID::CalibVal,
				reinterpret_cast<uint8_t*>(&write_tt), sizeof(byte8TestType));
		EXPECT_EQ(result, true);

		uint16_t address = 9 + (i * sizeof(Eeprom_93C86::JournalEntryType));
		EXPECT_EQ(buffer[address], static_cast<uint8_t>(ValueID::CalibVal));

		address = 609U + i * sizeof(byte8TestType);
		EXPECT_EQ(buffer[address], write_tt.uintValue);

		// watch Journal-Buffer and Data-Buffer
		// printf("Journal: ");
		// printBuffer(0U, nrOfEntries * 10);
		// printf("Data: ");
		// printBuffer(609U, nrOfEntries * 10);
	}
}
