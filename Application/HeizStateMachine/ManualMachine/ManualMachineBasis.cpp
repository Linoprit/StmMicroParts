/*
 * ManualMachine.cpp
 *
 *  Created on: 07.07.2019
 *      Author: harald
 */

#include <HeizStateMachine/ManualMachine/ManualMachineBasis.h>

ManualMachineBasis::ManualMachineBasis() {
	_act_state = pause;
}

void ManualMachineBasis::cycle() {

}

void ManualMachineBasis::switch_on(void) {
	switch_state(active);
}

void ManualMachineBasis::switch_off(void) {
	switch_state(pause);
}
