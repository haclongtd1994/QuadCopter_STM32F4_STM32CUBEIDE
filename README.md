					Project Quadcopter DIY by me =]]]
					
Requirement device:
	1 x Kit STM32F4 Discovery.
	1 x GY-85.
	1 x F450 Frame for Quad.
	4 x Esc and Brushless motor.
	1 x battery have BET can supply power for ST device.
	1 x Schematic design.
	1 x Tranfer and Reciever MC6C it's OK
Requirement software:
	Just need install STM32CubeIde and related software and library.
Limitation:
	Just code for throttle.

Implementaition Current in Project:
	+ Module:				Status:		Function:
	- Accelerometer			Done		i2c_write_accelerometer
										i2c_read_accelerometer
										InitialiseAccelerometer
										ReadAccelerometer
	- Analytics				Not Yet
	- Angular_Position		60%			InitialiseAngularPosition(Lack Magnet)
										ReadAngularPosition(Need improvement)
										ResetToAngularZeroPosition
	- Delay(DWDGT)			Done		EnableTiming
										TimingDelay
										WaitASecond
										WaitAMillisecond
										WaitAFewMillis
	- Gyroscope				Done		i2c_write_gyroscope
										i2c_read_gyroscope
										InitialiseGyroscope
										ReadGyroscope
	- I2C					Not Yet
	- Magnetometer			Not Yet
	- Main					Not Yet
	- On_Board_Leds			Done
	- Panic					Not Yet
	- PID					Not Yet
	- PWM					Done		InitialisePWM
										SetTim3Channel1
										SetTim3Channel2
										SetTim3Channel3
										SetTim3Channel4
										UpdateTim3Channel1
										UpdateTim3Channel2
										UpdateTim3Channel3
										UpdateTim3Channel4
										ReadTim3Channel1Pulse
										ReadTim3Channel2Pulse
										ReadTim3Channel3Pulse
										ReadTim3Channel4Pulse
										InitialisePWMChannel
	- PWM_Input				Not Yet
	- Remote_Control		Not Yet
	- Ring_Buffer			Not Yet
	- Serial_Output			Not Yet
	- Systick				Not Yet