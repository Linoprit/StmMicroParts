/*
 * EepromApi.h
 *
 *  Created on: 12.10.2020
 *      Author: harald
 */

#ifndef EEPROMAPI_EEPROMAPI_H_
#define EEPROMAPI_EEPROMAPI_H_

#include <stdint.h>
#include <Devices/Eeprom_93C86/Eeprom_93C86.h>

namespace eeprom {

class EepromApi {
public:

	EepromApi(void);
	virtual ~EepromApi(void) {
	}
	;
	class MemValType {
	public:
		uint32_t Usoll1;
		uint32_t Isoll1;
		uint32_t Usoll2;
		uint32_t Isoll2;
		uint8_t crc;
	} __attribute__ ((packed));

	class StartValType {
	public:
		uint32_t Usoll1;
		uint32_t Isoll1;
		uint32_t Usoll2;
		uint32_t Isoll2;
		char inSource :4;
		char powActive :4;
		uint8_t crc;
	} __attribute__ ((packed));

	class CalibValType {
	public:
		uint32_t Rshunt;
		uint8_t T_TrafoMax;
		uint8_t T_Endstufe_Max;
		uint8_t crc;
	} __attribute__ ((packed));

	bool init(bool eraseData);

	bool writeStartValues(StartValType startVal);
	bool writeMemValues(MemValType memVal);
	bool writeCalibValues(CalibValType calibVal);

	bool readStartValues(StartValType &startVal);
	bool readMemValues(MemValType &memVal);
	bool readCalibValues(CalibValType &calibVal);

	Eeprom_93C86 getEeprom(void) { return _eeprom; };

private:
	Eeprom_93C86_socket _eepromSocket;
	Eeprom_93C86 _eeprom;

};

} // namespace eeprom

#endif /* EEPROMAPI_EEPROMAPI_H_ */
