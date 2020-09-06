/*
 * Ring_Buffer.h
 *
 *  Created on: Sep 6, 2020
 *      Author: PC
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include "main.h"
#include "Config_Types_Vars.h"

void  InitialiseRingBuffer(RingBuffer *_this);				// InitialiseRingBuffer
int RingBufferIsEmpty(RingBuffer *_this);					// Check buffer empty
int RingBufferIsFull(RingBuffer *_this);					// Check buffer full
uint16_t RingBufferPop(RingBuffer *_this);					// Pop data which don't need anymore
void RingBufferPut(RingBuffer *_this, uint16_t value);		// Put new data

#endif /* INC_RING_BUFFER_H_ */
