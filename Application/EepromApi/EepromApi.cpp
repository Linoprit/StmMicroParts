/*
 * EepromApi.cpp
 *
 *  Created on: 12.10.2020
 *      Author: harald
 */

#include <EepromApi/EepromApi.h>
#include <Devices/Eeprom_93C86/Eeprom_93C86_socket.h>

namespace eeprom {

EepromApi::EepromApi() {
	_eepromSocket = Eeprom_93C86_socket();
	_eeprom = Eeprom_93C86(&_eepromSocket);
}

bool EepromApi::init(bool eraseData) {
	if (eraseData) {
		_eeprom.eraseEeprom();
	}
	return _eeprom.init();
}

bool EepromApi::readStartValues(StartValType& startVal) {
	return _eeprom.readData(ValueID::StartVal,
			reinterpret_cast<uint8_t*>(&startVal));
}

bool EepromApi::readMemValues(MemValType& memVal) {
	return _eeprom.readData(ValueID::MemVal,
			reinterpret_cast<uint8_t*>(&memVal));
}

bool EepromApi::readCalibValues(CalibValType &calibVal) {
	return _eeprom.readData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&calibVal));
}

bool EepromApi::writeStartValues(StartValType startVal) {
	return _eeprom.writeData(ValueID::StartVal,
			reinterpret_cast<uint8_t*>(&startVal), sizeof(StartValType));
}

bool EepromApi::writeMemValues(MemValType memVal) {
	return _eeprom.writeData(ValueID::MemVal,
			reinterpret_cast<uint8_t*>(&memVal), sizeof(MemValType));
}

bool EepromApi::writeCalibValues(CalibValType calibVal) {
	return _eeprom.writeData(ValueID::CalibVal,
			reinterpret_cast<uint8_t*>(&calibVal), sizeof(CalibValType));
}

} // namespace eeprom
