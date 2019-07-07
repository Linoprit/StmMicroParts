/*
 * ManualMachineList.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINELIST_H_
#define HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINELIST_H_

#include <HeizStateMachine/ManualMachine/ManualMachineBasis.h>
#include <libraries/SimpleQueue.h>
#include <cstdlib>
#include <exception>


using namespace std;

class ManualMachineListOutOfBoundsExeption: public exception {
public:
	virtual const char* what() const throw() {
		return "Pos out of ManualMachineList bounds.";
	}
};


class ManualMachineList {
public:
	ManualMachineList(uint8_t length);

	virtual ~ManualMachineList();

	uint8_t len(void) { return _length; };
	ManualMachineBasis*  get (uint8_t pos);
	SimpleQueue<int8_t>* get_queue(void) { return _threadQueue; };


private:
	SimpleQueue<int8_t>* _threadQueue;
	uint8_t 			 _length;
	ManualMachineBasis*  _threads;
};



#endif /* HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINELIST_H_ */
