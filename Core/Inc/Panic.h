/*
 * Panic.h
 *
 *  Created on: Sep 6, 2020
 *      Author: PC
 */

#ifndef INC_PANIC_H_
#define INC_PANIC_H_

#include "Config_Types_Vars.h"
#include "main.h"

void InitialisePanicButton(void);
void ClearWarnings(void);

void panic(char* message);
void panicWithValue(char* message, uint32_t value);

void warning(char* message);
void warningWithValue(char* message, uint32_t value);


#endif /* INC_PANIC_H_ */
