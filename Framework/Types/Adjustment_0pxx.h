/*
 * Adjustment_0pxx.h
 *
 *  Created on: 14.05.2020
 *      Author: harald
 */

#ifndef TYPES_ADJUSTMENT_0PXX_H_
#define TYPES_ADJUSTMENT_0PXX_H_

#include <stdint.h>
#include "Fixed.h"
#include <float.h>
#include <math.h>

/* Initially thought for the Rshunt adjustment. Use four places after dec point. */
class Adjustment_0pxx: public Fixed {
public:
	Adjustment_0pxx(
			int32_t value  = 0,
			uint8_t exp    = 4,
			float   minVal = static_cast<float>(INT32_MIN),
			float   maxVal = static_cast<float>(INT32_MAX)
	): Fixed(value, exp),
	_actualPlace { 0 }
	{
		// to avoid overflow in increment / decrement functions,
		// set the limits smaller
		if(maxVal < static_cast<float>(INT32_MAX - 10))
			setMax(maxVal);
		else
			setMax(static_cast<float>(INT32_MAX - 10));

		if(minVal > static_cast<float>(INT32_MIN + 10))
			setMin(minVal);
		else
			setMin(static_cast<float>(INT32_MIN + 10));
	};

	virtual ~Adjustment_0pxx() {};

	virtual void increment(void) {
		int32_t valToAdd = pow(10, getExp() - _actualPlace);
		set(get() + valToAdd);
	}

	virtual void decrement(void) {
		int32_t valToSub = pow(10, getExp() - _actualPlace);
		set(get() - valToSub);
	}

	uint8_t getActualPlace(void) { return _actualPlace; };
	void	incrementActualPlace(void) {
		if (_actualPlace < getExp())
			_actualPlace++;
		else
			_actualPlace = 0;
	}

protected:
	uint8_t 	_actualPlace;

};



#endif /* TYPES_ADJUSTMENT_0PXX_H_ */
