/*
 * SimpleQueue.h
 * taken from:
 * http://sourcetricks.com/2008/07/c-queues.html#.W4T0NWbwCHs
 *
 *  Created on: 13.05.2019
 *      Author: harald
 */

#ifndef SIMPLEQUEUE_H_
#define SIMPLEQUEUE_H_

#include <iostream>
#include <cstdlib>
#include "../../Framework/System/uart_printf.h"


using namespace std;


class QueueOverFlowException: public exception {
public:
	virtual const char* what() const throw() {
		return "Queue overflow";
	}
};

class QueueEmptyException: public exception {
public:
	virtual const char* what() const throw() {
		return "Queue empty";
	}
};

template <class T> class SimpleQueue {
public:
	/*explicit SimpleQueue(void) {
		_data 	= new T[1];
		_maxSize = 1;
		reset();
	}*/

	SimpleQueue(uint16_t maxSize) {
		_data 	= new T[maxSize];
		_maxSize = maxSize;
		reset();
	};

	virtual ~SimpleQueue(void) {
		delete[] _data;
	};

	void enqueue(T element) { // put element to back of queue
		if (size() == _maxSize)
			throw QueueOverFlowException();

		_size++;
		increment(_rear);
		_data[_rear] = element;
	};

	T dequeue(void) { // return element and remove it from queue
		if ( isEmpty() )
			throw QueueEmptyException();

		_size--;
		T ret = _data[_front];
		increment(_front);
		return ret;
	};

	void asArray(T* data) { // returns all elements as array, w/o dequeueing
		int16_t lclFront = _front;
		for (uint8_t i=0; i < _size; i++) {
			data[i] = _data[lclFront];
			increment(lclFront);
		}
	}

	void reset(void) {
		_front 	= 0;
		_rear 	= -1;
		_size   = 0;
	}

	T front(void) { // look at first element w/o dequeueing
		if ( isEmpty() )
			throw QueueEmptyException();

		return _data[_front];
	};

	int16_t size(void) { return _size;	};
	bool isEmpty(void) { return (_size == 0); };
	bool isFull(void)  { return (_size == _maxSize); };

	int16_t getFront() { return _front;	};
	int16_t getRear()  { return _rear; 	};

private:
	T* _data;
	int16_t _front;
	int16_t _rear;
	int16_t _size;
	uint8_t _maxSize;

	void increment(int16_t &x) {
		if (x == _maxSize - 1)
			x = 0;
		else
			x++;
	};
};

#endif /* SIMPLEQUEUE_H_ */
