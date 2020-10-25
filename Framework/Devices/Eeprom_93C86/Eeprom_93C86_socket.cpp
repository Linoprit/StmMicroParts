/*
 * Eeprom_93C86_socket.cpp
 *
 *  Created on: 25.03.2018
 *     Author: harald
 *     based on Bob Ryans 93C46B EEprom Driver, (c) 2010
 */

#include <string.h>
#include <Devices/Eeprom_93C86/Eeprom_93C86_socket.h>

// on 93C86, 8-bit mode, we have eleven address-bits
//#define ADDRESS_MASK 1024 // the 11th bit
//#define ADDRESS_BITS 11
//#define LAST_DATABIT 128
#define MAX_ADDRESS 2047 // 93C86 highest address

Eeprom_93C86_socket::Eeprom_93C86_socket ():
_actAddress { 0 },
_writeIsDisabled { true }
{
	_buffer = new uint8_t[MAX_ADDRESS + 1U];
	memset((void*) _buffer, 0xFF, MAX_ADDRESS + 1U);
}

void Eeprom_93C86_socket::strobe_sck(void)			{ return;	}
void Eeprom_93C86_socket::put_opcode(uint8_t code)	{ return;	}

void Eeprom_93C86_socket::put_address(uint16_t address)
{
	if (address > MAX_ADDRESS)
		address = MAX_ADDRESS;

	_actAddress = address;
}

void Eeprom_93C86_socket::erase(uint16_t address)
{
	if (_writeIsDisabled)
		return;

	_buffer[_actAddress] = 0xFF;
}

void Eeprom_93C86_socket::eraseall(void)
{
	if (_writeIsDisabled)
		return;

	memset((void*) _buffer, 0xFF, MAX_ADDRESS + 1U);
}

void Eeprom_93C86_socket::write_disable(void)
{
	_writeIsDisabled = true;
}

void Eeprom_93C86_socket::write_enable(void)
{
	_writeIsDisabled = false;
}

uint8_t Eeprom_93C86_socket::read(uint16_t address)
{
  put_address(address);
  return _buffer[_actAddress];
}

void Eeprom_93C86_socket::readBlock(uint16_t address, uint8_t *data, uint8_t len)
{
  put_address(address);

  for (uint8_t j=0; j < len; j++) {
	  data[j]  = _buffer[_actAddress];
	 _actAddress++;
  }
}

void Eeprom_93C86_socket::write(uint16_t address, uint8_t data)
{
  put_address(address);
  if (_writeIsDisabled)
	  return;

  _buffer[_actAddress] = data;
}

void Eeprom_93C86_socket::writeBlock(uint16_t address, uint8_t* data, uint8_t len) {
	uint16_t tmpAddress = address;
	for (uint8_t i=0; i < len; i++) {
		write(tmpAddress, data[i]);
		tmpAddress++;
	}
}

void Eeprom_93C86_socket::writeall(uint8_t data)
{
	if (_writeIsDisabled)
		return;

	memset((void*) _buffer, data, MAX_ADDRESS + 1U);
}

uint16_t Eeprom_93C86_socket::getMaxAddress(void) { return MAX_ADDRESS; }


