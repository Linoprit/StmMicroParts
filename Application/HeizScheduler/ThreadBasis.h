/*
 * ThreadBasis.h
 *
 *  Created on: 13.05.2019
 *      Author: harald
 */

#ifndef THREADBASIS_H_
#define THREADBASIS_H_

#include <stdint.h>
#include "libraries/SimpleQueue.h"


#define ONE_TICK 1


class ThreadBasis {
public:
	enum enm_states {init, active, pause, ready};

	ThreadBasis(void);
	ThreadBasis(SimpleQueue<int8_t>* threadQueue_in, int8_t taksID);
	virtual ~ThreadBasis() { };

	void 	   cycle(void);
	enm_states get_state(void)    { return _act_state; 	};
	void 	   set_go(void)	      { _go = true; 		};
	void 	   reset_go(void)	  { _go = false; 		};

	void set_tpauseMax(uint32_t value)  { _t_pauseMax  = value; }; // ms
	void set_tactiveMax(uint32_t value) { _t_activeMax = value; }; // ms
	uint32_t get_tpauseMax(void)  { return _t_pauseMax; }; // ms
	uint32_t get_tactiveMax(void) { return _t_activeMax; }; // ms
	uint32_t get_tactive (void) { return _t_active; };
	uint32_t get_tpause  (void) { return _t_pause;  };

	void initialize(SimpleQueue<int8_t>* threadQueue_in, int8_t taksID);

	int8_t get_taskID(void)			{ return _taskID; };

private:
	enm_states _act_state;
	uint32_t _my_time;   // [ms]
	uint32_t _t_active;  // [ms]
	uint32_t _t_pause;   // [ms]
	bool     _entry;
	uint8_t  _taskID;
	uint32_t _t_activeMax; // [ms]
	uint32_t _t_pauseMax;  // [ms]
	bool     _go;
	SimpleQueue<int8_t>* threadQueue;

	void initialize(void); // called by constructor

	void st_active(void);
	void st_pause(void);
	void st_ready(void);
	void st_init(void);

	void tr_active_pause(void);
	void tr_active_ready(void);
	void tr_pause_ready(void);
	void tr_ready_active(void);

	uint32_t time_diff(void);
	void switch_state(enm_states state);
	bool check_entry(void);
};

#endif /* THREADBASIS_H_ */
