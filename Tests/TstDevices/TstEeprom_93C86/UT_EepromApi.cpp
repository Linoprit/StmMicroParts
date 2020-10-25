/*
 * UT_EdpromApi.cpp
 *
 *  Created on: 18.10.2020
 *      Author: harald
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../Application/EepromApi/EepromApi.h"

using ::testing::Expectation;
using namespace eeprom;

class Eeprom_ApiFix: public ::testing::Test {
public:
	Eeprom_ApiFix() {
	}
	;
	~Eeprom_ApiFix() {
	}
	;
	// void SetUp() { };
	// void TearDown() { };

	void inrementMemValType(EepromApi::MemValType &val) {
		val.Usoll1++;
		val.Isoll1++;
		val.Usoll2++;
		val.Isoll2++;
	}
	void inrementStartValType(EepromApi::StartValType &val) {
		val.Usoll1++;
		val.Isoll1++;
		val.Usoll2++;
		val.Isoll2++;
		if (val.inSource < 15)
			val.inSource++;
		else
			val.inSource = 0;

		if (val.powActive < 15)
			val.powActive++;
		else
			val.powActive = 0;
	}
	void inrementCalibValType(EepromApi::CalibValType &val) {
		val.Rshunt++;
		val.T_TrafoMax++;
		val.T_Endstufe_Max++;
	}

	bool isEqMemValType(EepromApi::MemValType lval,
			EepromApi::MemValType rval) {
		return ((lval.Isoll1 == rval.Isoll1) && (lval.Usoll1 == rval.Usoll1)
				&& (lval.Usoll2 == rval.Usoll2) && (lval.Isoll2 == rval.Isoll2));
	}

	bool isEqStartValType(EepromApi::StartValType lval,
			EepromApi::StartValType rval) {
		return ((lval.powActive == rval.powActive)
				&& (lval.inSource == rval.inSource)
				&& (lval.Isoll1 == rval.Isoll1) && (lval.Usoll1 == rval.Usoll1)
				&& (lval.Usoll2 == rval.Usoll2) && (lval.Isoll2 == rval.Isoll2));
	}

	bool isEqCalibValType(EepromApi::CalibValType lval,
			EepromApi::CalibValType rval) {
		return ((lval.Rshunt == rval.Rshunt)
				&& (lval.T_TrafoMax == rval.T_TrafoMax)
				&& (lval.T_Endstufe_Max == rval.T_Endstufe_Max));
	}

	EepromApi eeApi = EepromApi();
};

TEST_F(Eeprom_ApiFix, basisTest) {
	EXPECT_EQ(eeApi.init(false), false);
	EXPECT_EQ(eeApi.init(true), true);

	EepromApi::MemValType memVal { 1220u, 220u, 2145u, 120u, 0 };
	EepromApi::StartValType startVal { 1220u, 220u, 2145u, 120u, 1u, 2u, 0u };
	EepromApi::CalibValType calibVal { 1021u, 32u, 64u, 0u };

	for (int i = 0; i < 20; i++) {
		inrementMemValType(memVal);
		inrementStartValType(startVal);
		inrementCalibValType(calibVal);

		EXPECT_EQ(eeApi.writeMemValues(memVal), true); // 17 Bytes
		EXPECT_EQ(eeApi.writeStartValues(startVal), true); // 18 Bytes
		EXPECT_EQ(eeApi.writeCalibValues(calibVal), true); // 7 Bytes
	}
	EXPECT_EQ(eeApi.getEeprom().getFreeJournalEntries(), 40u);
	EXPECT_EQ(eeApi.getEeprom().getFreeDataSpace(), 1438u - 840u);

	// init and read in again
	EXPECT_EQ(eeApi.init(false), true);
	EXPECT_EQ(eeApi.getEeprom().getFreeJournalEntries(), 40u);
	EXPECT_EQ(eeApi.getEeprom().getFreeDataSpace(), 1438u - 840u);

	EepromApi::MemValType rd_memVal;
	EepromApi::StartValType rd_startVal;
	EepromApi::CalibValType rd_calibVal;

	EXPECT_EQ(eeApi.readStartValues(rd_startVal), true);
	EXPECT_EQ(eeApi.readMemValues(rd_memVal), true);
	EXPECT_EQ(eeApi.readCalibValues(rd_calibVal), true);

	EXPECT_EQ(isEqCalibValType(rd_calibVal, calibVal), true);
	EXPECT_EQ(isEqMemValType(rd_memVal, memVal), true);
	EXPECT_EQ(isEqStartValType(rd_startVal, startVal), true);
}

