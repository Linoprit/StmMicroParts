/*
 * ThreadList.h
 *
 *  Created on: 29.05.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_THREADLIST_H_
#define HEIZSTATEMACHINE_THREADLIST_H_

#include "ThreadBasis.h"
#include <stdint.h>


class ThreadListOutOfBoundsExeption: public exception {
public:
	virtual const char* what() const throw() {
		return "Pos out of ThreadList bounds.";
	}
};

class ThreadList {
public:
	ThreadList(uint8_t length);
	virtual ~ThreadList();

	uint8_t len(void) { return _length; };
	ThreadBasis* get (uint8_t pos);
	SimpleQueue<int8_t>* get_queue(void) { return _threadQueue; };


private:
	SimpleQueue<int8_t>* _threadQueue;
	uint8_t 			 _length;
	ThreadBasis* 		 _threads;
};

#endif /* HEIZSTATEMACHINE_THREADLIST_H_ */
