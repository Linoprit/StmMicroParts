/*
 * StateMachineInterface.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_STATEMACHINEINTERFACE_H_
#define HEIZSTATEMACHINE_STATEMACHINEINTERFACE_H_

#include <stdint.h>


class StateMachineInterface	{
public:
	enum enm_states {init, active, pause, ready};

	virtual ~StateMachineInterface(void) { };

	virtual void	cycle(void) 		= 0;
	enm_states 		get_state(void)    {	return _act_state;	};
	void 	   		set_go(void)	   { _go = true; 		};
	void 	   		reset_go(void)	   { _go = false; 		};

protected:
	enm_states _act_state;
	bool       _entry;
	bool       _go;

	void  switch_state(enm_states state){
		_entry = true;
		_act_state = state;
	};

	bool  check_entry(void)	{
		if(_entry == true)	{
			_entry = false;
			return true;
		}
		return false;
	};
};


#endif /* HEIZSTATEMACHINE_STATEMACHINEINTERFACE_H_ */
