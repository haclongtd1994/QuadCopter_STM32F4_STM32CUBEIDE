/*
 * Analytics.c
 *
 *  Created on: Sep 6, 2020
 *      Author: PC
 */

#include "Analytics.h"
#include "delay.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

void InitialiseAnalytics() {
	InitialiseRingBuffer(&metricsRingBuffer);
}

void WriteStringToBuffer(char* value) {
	char* letter = value;

	while(*letter) {
		RingBufferPut(&metricsRingBuffer, *letter++);
	}
}

void WriteCharacterToBuffer(uint16_t value) {
	RingBufferPut(&metricsRingBuffer, value);
}
uint8_t numDigits(uint32_t number)
{
	uint8_t digits = 0;
	while (number) {
			number /= 10;
			digits++;
	}
	return digits;
}

void WriteCharacterToBufferNum(uint32_t value) {
	int count=0;
	int digits = numDigits(value);
	char data[100];
	sprintf(data, "%lu", value);
	while(count < digits){
		RingBufferPut(&metricsRingBuffer, data[count]);
		count++;
	}
}

void RecordWarningMessage(char *message) {
	WriteStringToBuffer("info.warn:W:");
	WriteStringToBuffer(message);
	WriteCharacterToBuffer(13);
	WriteCharacterToBuffer(10);
}

void RecordPanicMessage(char *message) {
	WriteStringToBuffer("info.err-:E:");
	WriteStringToBuffer(message);
	WriteCharacterToBuffer(13);
	WriteCharacterToBuffer(10);
}

void RecordIntegerMetric(uint8_t type, uint8_t loopReference, uint32_t value) {
	WriteCharacterToBuffer('S');
	WriteCharacterToBufferNum(type);
	WriteCharacterToBuffer(':');
	WriteCharacterToBufferNum(loopReference);
//	WriteCharacterToBuffer(':');
//
//	uint8_t valueHighest = (value >> 24) & 0xFF;
//	uint8_t valueHigh = (value >> 16) & 0xFF;
//	uint8_t valueLow = (value >> 8) & 0xFF;
//	uint8_t valueLowest = (value >> 0) & 0xFF;
//
//	WriteCharacterToBufferNum(valueHighest);
//	WriteCharacterToBufferNum(valueHigh);
//	WriteCharacterToBufferNum(valueLow);
//	WriteCharacterToBufferNum(valueLowest);
	WriteCharacterToBuffer(':');
	WriteCharacterToBufferNum(value);
	WriteCharacterToBuffer(13);
	WriteCharacterToBuffer(10);
}

void RecordFloatMetric(uint8_t type, uint8_t loopReference, float value) {
	WriteCharacterToBuffer('S');
	WriteCharacterToBufferNum(type);
	WriteCharacterToBuffer(':');
	WriteCharacterToBufferNum(loopReference);
//	WriteCharacterToBuffer(':');
//
//	int32_t roundedValue = (value * 1000000);
//	uint8_t valueHighest = (roundedValue >> 24) & 0xFF;
//	uint8_t valueHigh = (roundedValue >> 16) & 0xFF;
//	uint8_t valueLow = (roundedValue >> 8) & 0xFF;
//	uint8_t valueLowest = (roundedValue >> 0) & 0xFF;
//
//	WriteCharacterToBufferNum(valueHighest);
//	WriteCharacterToBufferNum(valueHigh);
//	WriteCharacterToBufferNum(valueLow);
//	WriteCharacterToBufferNum(valueLowest);
	WriteCharacterToBuffer(':');
	WriteCharacterToBufferNum(value);
	WriteCharacterToBuffer(13);
	WriteCharacterToBuffer(10);
}

void FlushMetrics() {
	uint8_t metricsFlushed = 0;
	uint8_t data_send[1]={0};

	while(metricsFlushed < ANALYTICS_CHARACTERS_TO_SEND_PER_FLUSH && metricsRingBuffer.count > 0) {
		data_send[0] = RingBufferPop(&metricsRingBuffer);
		HAL_UART_Transmit(&huart4, (uint8_t *)data_send, 1, 1000);
		metricsFlushed++;
	}
}

void FlushAllMetrics() {
	uint8_t data_send[1]={0};
	while(metricsRingBuffer.count > 0) {
		data_send[0] = RingBufferPop(&metricsRingBuffer);
		HAL_UART_Transmit(&huart4, (uint8_t *)data_send, 1, 1000);
		WaitAFewMillis(10);
	}
}

