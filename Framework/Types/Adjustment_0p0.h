/*
 * Adjustment.h
 *
 *  Created on: 06.05.2020
 *      Author: harald
 */

#ifndef SYSTEM_ADJUSTMENT_H_
#define SYSTEM_ADJUSTMENT_H_

#include <stdint.h>
#include "Fixed.h"
#include <float.h>
#include <math.h>

class Adjustment_0p0: public Fixed {
public:
	Adjustment_0p0(
			int32_t value = 0,
			float   minVal = static_cast<float>(INT32_MIN),
			float   maxVal = static_cast<float>(INT32_MAX)
	): Fixed(value, 1u),
	_fineFlag{false}
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

	virtual ~Adjustment_0p0() {};

	virtual void increment(void) {
		if (_fineFlag == true) {
			set(get() + 1);
		}
		else {
			set(get() / 10);
			set(get() * 10);
			set(get() + 10);
		}
	};

	virtual void decrement(void) {
		if (_fineFlag == true) {
				set(get() - 1);
		}
		else {
				set(get() /  10);
				set(get() *  10);
				set(get() -  10);
		}
	};

	bool isFineFlag(void) const { return _fineFlag; };
	void toggleFineFlag(void) {
		if (_fineFlag == true)
			_fineFlag = false;
		else
			_fineFlag = true;
	};


protected:
	bool 			_fineFlag;

};

#endif /* SYSTEM_ADJUSTMENT_H_ */
