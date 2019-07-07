/*
 * ThreadList.cpp
 *
 *  Created on: 29.05.2019
 *      Author: harald
 */

#include <HeizScheduler/ThreadList.h>



ThreadList::ThreadList(uint8_t length) {
	_length 	 = length;
	_threadQueue = new SimpleQueue<int8_t>(_length);
	_threads  	 = new ThreadBasis[_length];

	for(uint8_t i=0; i < _length; i++)
	{
		_threads[i].initialize(_threadQueue, i);
	}
}

ThreadBasis* ThreadList::get (uint8_t pos) {
	if((pos < _length) && (pos >= 0))
		return &_threads[pos];
	else
		throw ThreadListOutOfBoundsExeption();


}
