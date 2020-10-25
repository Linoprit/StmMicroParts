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
	Fixed(	int32_t value = 0,
			uint8_t exp = 2,
			float   minVal = static_cast<float>(INT32_MIN),
			float   maxVal = static_cast<float>(INT32_MAX) )
{
		_exp = exp;
		setMax(maxVal);
		setMin(minVal);
		set(value);
};
	virtual ~Fixed() {};

	int32_t get(void) 				{ return _value; };
	void	set(const int32_t in) 	{
		if(in > _maxVal)
			_value = _maxVal;
		else if(in < _minVal)
			_value = _minVal;
		else
			_value = in;
	}
	uint8_t getExp(void) 			{ return _exp; };
	int32_t getFactor(void)			{ return std::pow(10, _exp); };
	float   getFlt(void) {
		float factor = static_cast<float>( getFactor() );
		return (static_cast<float>(_value) / factor );
	}
	void  set(const float in)	{
		float factor = static_cast<float>( getFactor() );
		set(static_cast<int32_t>( round(in * factor) ));
	};

	void toString(char* buffer, const uint8_t buffLen){
		HelpersLib::value2char(buffer, buffLen, _value, _exp);
	};

	void setMax(float in) {
		float factor = static_cast<float>(getFactor());
		float tmpMax = (in * factor);
		if (tmpMax <= static_cast<float>(INT32_MAX))
			_maxVal = static_cast<int32_t>(round(tmpMax));
		else
			_maxVal = INT32_MAX;
	};
	void setMin(float in) {
		float tmpMin = (in * static_cast<float>(getFactor()));
		if (tmpMin >= static_cast<float>(INT32_MIN))
			_minVal = static_cast<int32_t>(round(tmpMin));
		else
			_minVal = INT32_MIN;
	};
	int32_t getMax(void) const { return _maxVal; };
	int32_t getMin(void) const { return _minVal; };

	float getMaxFlt(void) {
		float factor = static_cast<float>( getFactor() );
		return (static_cast<float>(_maxVal) / factor ); };
	float getMinFlt(void) {
		float factor = static_cast<float>( getFactor() );
		return (static_cast<float>(_minVal) / factor );};

private:
	int32_t 	_value;
	uint8_t 	_exp;
	int32_t		_maxVal;
	int32_t		_minVal;

};



#endif /* TYPES_FIXED_H_ */
