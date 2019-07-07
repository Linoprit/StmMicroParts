/*
 * ManualMachineScheduler.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINESCHEDULER_H_
#define HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINESCHEDULER_H_

#include <HeizStateMachine/ManualMachine/ManualMachineList.h>
#include <HeizStateMachine/StateMachineInterface.h>


class ManualMachineScheduler {
public:
	ManualMachineScheduler(uint8_t threadCount): _manualMachineList(threadCount) { };
	virtual ~ManualMachineScheduler() {} ;

	void switch_on(int8_t threadNo) {
		ManualMachineBasis* manualMachineBasis = _manualMachineList.get(threadNo);
		manualMachineBasis->switch_on();
	};

	void switch_off(int8_t threadNo) {
		ManualMachineBasis* manualMachineBasis = _manualMachineList.get(threadNo);
		manualMachineBasis->switch_off();
	};

	StateMachineInterface::enm_states get_state(int8_t threadNo) {
		ManualMachineBasis* manualMachineBasis = _manualMachineList.get(threadNo);
		return manualMachineBasis->get_state();
	}

	uint8_t  get_threadListlen(void) { return _manualMachineList.len(); };

private:
	ManualMachineList _manualMachineList;

};

#endif /* HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINESCHEDULER_H_ */
