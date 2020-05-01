/*
 * Error_Handler.cpp
 *
 *  Created on: 07.10.2019
 *      Author: harald
 */

#include <System/Error_Handler.h>
#include <System/uart_printf.h>
#include <stdint.h>
#include <stdio.h>



void error_handler(const char* file, uint32_t line)
{
	printf("Exception in %s, line: %i\n", file, line);

	throw ErrorHandlerException();
}

