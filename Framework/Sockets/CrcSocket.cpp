/*
 * CrcSocket.cpp
 *
 *  Created on: 03.10.2020
 *      Author: harald
 */

#include <Sockets/CrcSocket.h>


uint8_t CrcSocket::calc_chksum(uint8_t* data, uint8_t dataLen) {
  uint8_t i, crc = 0;

  // we must exclude the chk-sum byte at the end!
  for (i=0; i < (dataLen-1); i++) {
      crc = _crc_ibutton_update(crc, *data);
      data++;
    }
  return crc;
}

uint8_t CrcSocket::_crc_ibutton_update(uint8_t crc, uint8_t data) {
  uint8_t i;

  crc = crc ^ data;
  for (i = 0; i < 8; i++)  {
      if (crc & 0x01)
        crc = (crc >> 1) ^ 0x8C;
      else
        crc >>= 1;
    }

  return crc;
  }
