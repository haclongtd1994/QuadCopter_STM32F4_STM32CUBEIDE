/*
 * Ring_Buffer.c
 *
 *  Created on: Sep 6, 2020
 *      Author: PC
 */

#include <Ring_Buffer.h>
#include <string.h>

/******************************************************************************
* Function Name: InitialiseRingBuffer
* Description  : This function initialize ring buffer.
* Arguments    : Struct data ring buffer.
* Return Value : None
******************************************************************************/
void InitialiseRingBuffer(RingBuffer *_this) {
	/*
	The following clears:
		-> buf
		-> head
		-> tail
		-> count
		and sets head = tail
	*/
	memset(_this, 0, sizeof (*_this));	// Clear struct data this
}

/******************************************************************************
* Function Name: modulo_inc
* Description  : This function increase value.
* Arguments    : Value and max value.
* Return Value : New value.
******************************************************************************/
unsigned int modulo_inc (const unsigned int value, const unsigned int modulus)
{
    unsigned int my_value = value + 1;	// Increase value
    if (my_value >= modulus)			// Check value and modulus
    {
      my_value  = 0;					// Reset value
    }
    return my_value;
}

/******************************************************************************
* Function Name: modulo_dec
* Description  : This function decrease value.
* Arguments    : Value and min value.
* Return Value : New value or return error (max value - 1)
******************************************************************************/
unsigned int modulo_dec (const unsigned int value, const unsigned int modulus)
{
	// Check value to know decrease value or modulus
    return (0 == value) ? (modulus - 1) : (value - 1);
}

/******************************************************************************
* Function Name: RingBufferIsEmpty
* Description  : This function check buffer is empty.
* Arguments    : Struct buffer ring data.
* Return Value : True (Empty), False (Not)
******************************************************************************/
int RingBufferIsEmpty(RingBuffer *_this) {
	return 0 == _this->count;					// Return result of compare
}

/******************************************************************************
* Function Name: RingBufferIsFull
* Description  : This function check buffer is full.
* Arguments    : Struct buffer ring data.
* Return Value : True (Full), False (Not).
******************************************************************************/
int RingBufferIsFull(RingBuffer *_this) {
	// Return check count of data in ring buffer
	return _this->count >= RING_BUFFER_SIZE;
}

/******************************************************************************
* Function Name: RingBufferPop
* Description  : This function pop data at tail.
* Arguments    : Struct buffer ring data.
* Return Value : None
******************************************************************************/
uint16_t RingBufferPop(RingBuffer *_this) {
	// Check data is right or not ?
	if (_this->count <= 0) {
		return -1;
	}

	// Get tail buffer
	uint16_t value = _this->buffer[_this->tail];
	// Increase tail to next data can insert
	_this->tail = modulo_inc(_this->tail, RING_BUFFER_SIZE);
	// Decrease count
	--_this->count;
	return value;
}

/******************************************************************************
* Function Name: RingBufferPut
* Description  : This function put new data at head.
* Arguments    : Struct buffer ring data.
* Return Value : None
******************************************************************************/
void RingBufferPut(RingBuffer *_this, uint16_t value) {
	// Check count with max size buffer
	if (_this->count < RING_BUFFER_SIZE) {
	  // Add new value
	  _this->buffer[_this->head] = value;
	  // Increase head
	  _this->head = modulo_inc(_this->head, RING_BUFFER_SIZE);
	  // Increase count
	  ++_this->count;
	}
}
