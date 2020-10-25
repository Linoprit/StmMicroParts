/*
 * Eeprom_93C86_socket.h
 *
 *  Created on: 25.03.2018
 *      Author: harald
 *  Dummy class to mock functionality of 93C86 EEPROM
 *  for Testing.
 */

#ifndef DEVICES_EEPROM_93C86_EEPROM_93C86_SOCKET_H_
#define DEVICES_EEPROM_93C86_EEPROM_93C86_SOCKET_H_

#include <stdint.h>

class Eeprom_93C86_socket
{
public:
  Eeprom_93C86_socket();
  virtual ~Eeprom_93C86_socket() {};

  void strobe_sck(void);
  void put_opcode(uint8_t code);
  void put_address(uint16_t address);
  void erase(uint16_t address);
  void eraseall(void);
  void write_disable(void);
  void write_enable(void);
  uint8_t read(uint16_t address);
  void readBlock(uint16_t address, uint8_t *data, uint8_t len);
  void write(uint16_t address, uint8_t data);
  void writeBlock(uint16_t address, uint8_t* data, uint8_t len);
  void writeall(uint8_t data);

  inline bool is_busy(void)  		{	  return false; };
  inline void put_to_standby(void)  { 	  return;   	};
  uint16_t getMaxAddress(void);

  // only for testing
  uint8_t* get_buffer(void) 		{ return _buffer;		};
  uint16_t get_actAddress(void)		{ return _actAddress;	};
  bool     isWritingDisabled(void)  { return _writeIsDisabled;};

private:
  uint8_t* _buffer;
  uint16_t _actAddress;
  bool	   _writeIsDisabled;

};

#endif /* DEVICES_EEPROM_93C86_EEPROM_93C86_SOCKET_H_ */
