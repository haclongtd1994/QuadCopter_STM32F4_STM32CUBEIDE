# Project Quadcopter DIY by me =]]]
					
## Requirement device:

| N | Name:                                            |
|---|--------------------------------------------------|
| 1 | Kit STM32F4 Discovery.                           |
| 1 | GY-85.                                           |
| 1 | F450 Frame for Quad.                             |
| 4 | Esc and Brushless motor.                         |
| 1 | Battery have BET can supply power for ST device. |
| 1 | Schematic design.                                |
| 1 | Tranfer and Reciever MC6C it's OK.               |

## Requirement software:
	Just need install STM32CubeIde and related software and library.
## Limitation:
	Just code for throttle.
## Refer:
	https://www.technik-consulting.eu/en/optimizing/drone_PID-optimizing.html
	https://www.youtube.com/watch?v=ZRHFQlmI0zE

## Implementaition Current in Project:
| Module:		|Status:	|Function:                             |
|-----------------------|---------------|--------------------------------------|
|- Accelerometer	|Done		|i2c_write_accelerometer               |
|			|		|i2c_read_accelerometer                |
|			|		|InitialiseAccelerometer               |
|			|		|ReadAccelerometer                     |
|- Analytics		|Not Yet        |                                      |
|- Angular_Position	|60%		|InitialiseAngularPosition(Lack Magnet)|
|                       |               |ReadAngularPosition(Need improvement) |
|			|		|ResetToAngularZeroPosition            |
|- Delay(DWDGT)		|Done		|EnableTiming                          |
|			|		|TimingDelay                           |
|			|		|WaitASecond                           |
|			|		|WaitAMillisecond                      |
|			|		|WaitAFewMillis                        |
|- Gyroscope		|Done		|i2c_write_gyroscope                   |
|			|		|i2c_read_gyroscope                    |
|			|		|InitialiseGyroscope                   |
|			|		|ReadGyroscope                         |
|- I2C			|Don't need     |                                      |
|- Magnetometer		|Implement later|                                      |
|- Main			|Not Yet        |                                      |
|- On_Board_Leds	|Done		|TurnOn                                |
|					|TurnOff                               |
|- Panic		|Not Yet	|				       |
|- PID			|Not Yet	|                                      |
|- PWM			|Done		|InitialisePWM			       |
|			|		|SetTim3Channel1		       |
|			|		|SetTim3Channel2		       |
|			|		|SetTim3Channel3		       |
|			|		|SetTim3Channel4		       |
|			|		|UpdateTim3Channel1		       |
|			|		|UpdateTim3Channel2		       |
|			|		|UpdateTim3Channel3		       |
|			|		|UpdateTim3Channel4		       |
|			|		|ReadTim3Channel1Pulse		       |
|			|		|ReadTim3Channel2Pulse		       |
|			|		|ReadTim3Channel3Pulse		       |
|			|		|ReadTim3Channel4Pulse		       |
|			|		|InitialisePWMChannel		       |
|- PWM_Input		|Done		|HAL_TIM_IC_CaptureCallback            |
|- Remote_Control	|Done		|InitialiseRemoteControls	       |
|			|		|CalculatePercentageOfMaximum	       |
|			|		|ReadRemoteThrottle	               |
|			|		|ReadRemotePidProportional	       |
|			|		|ReadRemotePidIntegral	               |
|			|		|ReadResetAngularPositions	       |
|- Ring_Buffer		|Not Yet	|                                      |
|- Serial_Output	|Not Yet	|	   			       |
|- Systick		|Not Yet	|	                               |