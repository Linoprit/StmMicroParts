/*
 * HeizScheduler.cpp
 *
 *  Created on: 29.05.2019
 *      Author: harald
 */

#include "ThreadingScheduler.h"

ThreadingScheduler::ThreadingScheduler(uint8_t threadCount) :
_threadList(threadCount)
{
	_lastActiveThread = -1;
}

void ThreadingScheduler::cycle(void) {
	// cycle all tasks
	bool oneTaskIsActive = false;
	for (uint8_t i=0; i < _threadList.len(); i++) {
		ThreadBasis* thread = _threadList.get(i);
		thread->cycle();
		if(thread->get_state() == ThreadBasis::active)
			oneTaskIsActive = true;
	}

	if((!_threadList.get_queue()->isEmpty()) && (!oneTaskIsActive)) {
		// select next active
		int8_t ID = _threadList.get_queue()->front();
		if(ID == _lastActiveThread) { // requeue
			ID = _threadList.get_queue()->dequeue();
			_threadList.get_queue()->enqueue(ID);

			//printf("requeuing thread %i\n", ID);
		}

		ID = _threadList.get_queue()->dequeue();
		ThreadBasis* thread = _threadList.get(ID);
		thread->set_go();
		_lastActiveThread = ID;
		thread->cycle();
	}
}

int8_t ThreadingScheduler::get_active_thread(void)
{
	int8_t result = -1;

	for (uint8_t i=0; i < _threadList.len(); i++) {
		ThreadBasis* thread = _threadList.get(i);

		if(thread->get_state() == ThreadBasis::active)
		{
			result = thread->get_taskID();
		}
		//printf("thread %i=%i ", thread->get_taskID(), thread->get_state());
	}
	//printf("\n");
	return result;
}

void ThreadingScheduler::set_tactiveMax (int8_t threadNo, uint32_t tactive){
	ThreadBasis* thread = _threadList.get(threadNo);
	thread->set_tactiveMax(tactive);
}

void ThreadingScheduler::set_tpauseMax  (int8_t threadNo, uint32_t tpause){
	ThreadBasis* thread = _threadList.get(threadNo);
	thread->set_tpauseMax(tpause);
}

uint32_t ThreadingScheduler::get_tactiveMax (int8_t threadNo) {
	ThreadBasis* thread = _threadList.get(threadNo);
	return thread->get_tactiveMax();
}

uint32_t ThreadingScheduler::get_tpauseMax  (int8_t threadNo){
	ThreadBasis* thread = _threadList.get(threadNo);
	return thread->get_tpauseMax();
}


uint32_t ThreadingScheduler::get_tactive (int8_t threadNo){
	ThreadBasis* thread = _threadList.get(threadNo);
	return thread->get_tactive();
}

uint32_t ThreadingScheduler::get_tpause  (int8_t threadNo){
	ThreadBasis* thread = _threadList.get(threadNo);
	return thread->get_tpause();
}

StateMachineInterface::enm_states ThreadingScheduler::get_state(int8_t threadNo) {
	ThreadBasis* thread = _threadList.get(threadNo);
	return thread->get_state();
}



