/*
 * Analytics.h
 *
 *  Created on: Sep 6, 2020
 *      Author: PC
 */

#ifndef INC_ANALYTICS_H_
#define INC_ANALYTICS_H_

#include "Config_Types_Vars.h"
#include "main.h"
#include "Ring_Buffer.h"

void InitialiseAnalytics(void);

void RecordIntegerMetric(uint8_t type, uint8_t loopReference, uint32_t value);

void RecordFloatMetric(uint8_t type, uint8_t loopReference, float value);

void RecordPanicMessage(char *message);
void RecordWarningMessage(char *message);

void FlushMetrics(void);
void FlushAllMetrics(void);

#endif /* INC_ANALYTICS_H_ */
