/*
 * HeizMachineCommons.cpp
 *
 *  Created on: 08.09.2019
 *      Author: harald
 */

#include "HeizMachineCommons.h"


std::string state2str(StateMachineInterface::enm_states state) {
	std::string state_str;

	switch (state) {
	case StateMachineInterface::init:
		state_str = "init_";
		break;
	case StateMachineInterface::active:
		state_str = "activ";
		break;
	case StateMachineInterface::pause:
		state_str = "pause";
		break;
	case StateMachineInterface::ready:
		state_str = "ready";
		break;
	default:
		state_str = "*error*";
	}
	return state_str;
}


