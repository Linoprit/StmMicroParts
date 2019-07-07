/*
 * ThreadBasis.cpp
 *
 *  Created on: 13.05.2019
 *      Author: harald
 */

#include "ThreadBasis.h"
#include <Instances/Common.h>

ThreadBasis::ThreadBasis()
{
	initialize();
}

ThreadBasis::ThreadBasis(SimpleQueue<int8_t>* threadQueue_in, int8_t taskID)
{
	initialize(threadQueue_in, taskID);
}

void ThreadBasis::initialize(void) {
	_act_state 		= init;
	_my_time   		= 0;
	_t_active  		= 0;
	_t_pause   		= 0;
	_entry			= true;
	_taskID			= -1;
	_t_activeMax	= 0;
	_t_pauseMax		= 0;
	_go				= false;
	threadQueue     = NULL;
}

void ThreadBasis::initialize(SimpleQueue<int8_t>* threadQueue_in, int8_t taskID)
{
	initialize();
	_taskID			= taskID;
	threadQueue		= threadQueue_in;
}


void ThreadBasis::cycle(void)
{
	if (get_state() == init)
		st_init();
	else if (get_state() == active)
		st_active();
	else if (get_state() == pause)
		st_pause();
	else if (get_state() == ready)
		st_ready();
}

void ThreadBasis::st_init(void)
{
	if(check_entry() == true) {
		if((_taskID == -1) || (threadQueue == NULL)) {
			switch_state(init);
		}
		else {
			_t_active = _t_activeMax;
			switch_state(ready);
		}
	}
}

void ThreadBasis::st_active(void)
{
	if(check_entry() == true) {
		_my_time = Common::get_tick();
	}

	uint32_t tdiff = time_diff();
	if (tdiff >= ONE_TICK) {
		if (_t_active > tdiff)
			_t_active -= tdiff;
		else
			_t_active = 0u;

		_my_time = Common::get_tick();

		tr_active_ready();
		tr_active_pause(); // higher prio
	}
}

void ThreadBasis::st_ready(void)
{
	if(check_entry() == true) {
		threadQueue->enqueue(_taskID);
		_go = false;
	}

	tr_ready_active();
}

void ThreadBasis::st_pause(void)
{
	if(check_entry() == true) {
		_t_active = _t_activeMax;
		_t_pause  = _t_pauseMax;
		_my_time  = Common::get_tick();
	}

	uint32_t tdiff = time_diff();
	if (tdiff >= ONE_TICK) {
		if (_t_pause > tdiff)
			_t_pause -= tdiff;
		else
			_t_pause = 0u;

		_my_time  = Common::get_tick();
		tr_pause_ready();
	}
}

void ThreadBasis::tr_active_pause(void)
{
	if (_t_active == 0 )
		switch_state(pause);
}

void ThreadBasis::tr_active_ready(void)
{
	if (_go == false)
		switch_state(ready);
}
void ThreadBasis::tr_pause_ready(void)
{
	if (_t_pause == 0)
		switch_state(ready);
}
void ThreadBasis::tr_ready_active(void)
{
	if (_go == true)
		switch_state(active);
}

uint32_t ThreadBasis::time_diff(void)
{
	if(_my_time > Common::get_tick())
		return (UINT32_MAX - _my_time + Common::get_tick());
	return Common::get_tick() - _my_time;
}

void ThreadBasis::switch_state(enm_states state)
{
	_entry = true;
	_act_state = state;
}

bool ThreadBasis::check_entry(void)
{
	if(_entry == true)	{
		_entry = false;
		return true;
	}
	return false;
}


