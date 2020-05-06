/*
 * Fixed.h
 *
 *  Created on: 06.05.2020
 *      Author: harald
 */

#ifndef TYPES_FIXED_H_
#define TYPES_FIXED_H_

#include <stdint.h>
#include <math.h>
#include <libraries/HelpersLib.h>


class Fixed {
public:
	Fixed(int32_t value = 0, uint8_t exp = 2)
{
		_value = value;
		_exp = exp;
};
	virtual ~Fixed();

	int32_t get(void) 				{ return _value; };
	void	set(const int32_t in) 	{ _value = in; }
	uint8_t getExp(void) 			{ return _exp; };
	int32_t getFactor(void)			{ return 10 * _exp; };
	float   toFloat(void) {
		float factor = static_cast<float>( getFactor() );
		return (static_cast<float>(_value) / factor );
	}
	void  set(const float in)	{
		float factor = static_cast<float>( getFactor() );
		_value = static_cast<uint32_t>( round(in * factor) );
	};

	void toString(char* buffer, const uint8_t buffLen){
		HelpersLib::value2char(buffer, buffLen, _value, _exp);
	};

protected:
	int32_t _value;
	uint8_t _exp;

};



#endif /* TYPES_FIXED_H_ */
