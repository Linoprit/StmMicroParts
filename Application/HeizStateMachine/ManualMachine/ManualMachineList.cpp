/*
 * ManualMachineList.cpp
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#include "ManualMachineList.h"


ManualMachineList::ManualMachineList(uint8_t length) {
	_length 	 = length;
	_threadQueue = new SimpleQueue<int8_t>(_length);
	_threads  	 = new ManualMachineBasis[_length];
}

ManualMachineList::~ManualMachineList() {
	delete[] _threads;
	delete _threadQueue;
}

ManualMachineBasis* ManualMachineList::get (uint8_t pos) {
	if((pos < _length) && (pos >= 0))
		return &_threads[pos];
	else
		throw ManualMachineListOutOfBoundsExeption();
}
