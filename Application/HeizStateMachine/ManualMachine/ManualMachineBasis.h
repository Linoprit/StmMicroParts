/*
 * ManualMachine.h
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#ifndef HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINEBASIS_H_
#define HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINEBASIS_H_

#include "../StateMachineInterface.h"


class ManualMachineBasis: public StateMachineInterface {
public:
	ManualMachineBasis();
	virtual ~ManualMachineBasis() {};

	void	cycle(void);

	void switch_on(void);
	void switch_off(void);
};

#endif /* HEIZSTATEMACHINE_MANUALMACHINE_MANUALMACHINEBASIS_H_ */
