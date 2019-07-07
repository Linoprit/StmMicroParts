/*
 * HeizScheduler.h
 *
 *  Created on: 29.05.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_HEIZSCHEDULER_H_
#define HEIZSTATEMACHINE_HEIZSCHEDULER_H_

#include "ThreadBasis.h"
#include "ThreadList.h"
#include "libraries/SimpleQueue.h"


class ThreadingScheduler {
public:
	ThreadingScheduler(uint8_t threadCount);
	virtual ~ThreadingScheduler() {};

	void cycle(void);
	int8_t get_last_active_thread(void) { return _lastActiveThread; };
	int8_t get_active_thread(void);
	void set_tactiveMax (int8_t threadNo, uint32_t tactive);
	void set_tpauseMax  (int8_t threadNo, uint32_t tpause);
	uint32_t get_tactiveMax (int8_t threadNo);
	uint32_t get_tpauseMax  (int8_t threadNo);
	uint32_t get_tactive (int8_t threadNo);
	uint32_t get_tpause  (int8_t threadNo);
	StateMachineInterface::enm_states get_state(int8_t threadNo);
	uint8_t  get_threadListlen(void) { return _threadList.len(); };

private:
	ThreadList _threadList;
	int8_t _lastActiveThread;

};

#endif /* HEIZSTATEMACHINE_HEIZSCHEDULER_H_ */
