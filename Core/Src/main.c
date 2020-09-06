/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
// Variable for I2C
I2C_HandleTypeDef hi2c1;

// Variable for Timer2
TIM_HandleTypeDef htim2;

// Variable for Timer3 control ESC
TIM_HandleTypeDef htim3;

// Variable for Timer4, 5, 9, 12 monitor RX controller
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim12;

// Variable for UART communicate with CC2530
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
// Declare function in stdio.h
int	sprintf (char *__restrict, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 2, 3)));
// Declare function configure below main function
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM12_Init(void);
static void MX_UART4_Init(void);
void HardFault_Handler(void);

// Declare function Delay polling
void EnableTiming(void);
void WaitASecond(void);
void WaitAFewMillis(int16_t millis);

// Declare function PWM control ESC with motor brushless
void InitialisePWM();
DutyCycle InitialisePWMChannel(uint8_t channel);

// Declare function ON/OFF Leds on board
void TurnOn(uint16_t);
void TurnOff(uint16_t);

// Declare function Remote control receive signal from RX controller
void InitialiseRemoteControls();
float ReadRemoteThrottle();
float ReadRemotePidProportional();
float ReadRemotePidIntegral();
float ReadResetAngularPosition();

// Declare function to read Angular position(Pitch, Roll, Yaw):
//   Current implemented using Accelerometer sensor (Position X(Pitch), Y(Roll))
//     Need improvement using Gyroscope sensor (Position X(P), Y(R), Z(Yaw))
void InitialiseAngularPosition();
void ReadAngularPosition();

// Declare function PID to calibration Quadcopter
struct Pid InitialisePid(float proportional, float integral, float differential);
float CalculatePidAdjustment(Pid* pid, float current, float target);

// Declare function for Panic
void InitialisePanicButton();
void ClearWarnings(void);

// Declare function for Analytics
void RecordIntegerMetric(uint8_t type, uint8_t loopReference, uint32_t value);
void RecordFloatMetric(uint8_t type, uint8_t loopReference, float value);
void FlushMetrics(void);
void FlushAllMetrics(void);

/* USER CODE BEGIN PFP */

// Interrupt call back for user implement process pwm input
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  // Initialize for Leds on board
  MX_GPIO_Init();
  // Initialize for I2C communicate with sensor GY-85
  MX_I2C1_Init();
  // Initialize for timer count mili second
  MX_TIM2_Init();
  // Initialize for Timer control ESC with brushless motor
  MX_TIM3_Init();
  //Initialize 4 channel Input_Capture signal from RX controller
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  // Initialize UART communicate with CC2530 transfer data to server Laptop
  MX_UART4_Init();
  // Enable timing to using delay function with DWDGT
  EnableTiming();

  /* USER CODE BEGIN 2 */
  // Set interrupt 4 timer for channel 1&2 with mode Input Capture
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_2);

  // Initialize panic
  InitialisePanicButton();
  TurnOn(ORANGE_LED);

  // Initialize timer tick 1 mili second
  HAL_TIM_Base_Start_IT(&htim2);

  // Initialize PWM control ESC
  // Initialize PWM channel control ESC
	DutyCycle aProp = InitialisePWMChannel(1);
	DutyCycle cProp = InitialisePWMChannel(2);
	DutyCycle eProp = InitialisePWMChannel(3);
	DutyCycle bProp = InitialisePWMChannel(4);
  InitialisePWM();
  WaitAFewMillis(3000);

  //InitialiseI2C();	// PB.08 (SCL), PB.09 (SDA)
  Pid xAxisPid = InitialisePid(3, 0, 0);
  Pid yAxisPid = InitialisePid(3, 0, 0);

  // Initialize remote control detail channel RC and pin refer to file
  InitialiseRemoteControls();

  /* USER CODE END 2 */

  // Initialize InitialiseAngularPosition
  InitialiseAngularPosition();

  // Identify change status initialize done, next step waiting arming quad
  TurnOff(ORANGE_LED);
  TurnOn(YELLOW_LED);

  /* turn the motors off, until we get the go ahead from the user */
  uint8_t armingSequenceStep = ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED;
  uint32_t armingSequenceTimeLastStepExecuted = 0;

  bProp.set(1000);
  eProp.set(1000);
  cProp.set(1000);
  aProp.set(1000);

  while (armingSequenceStep != ARMING_SEQUENCE_ARMED || ARMING_SEQUENCE_IS_DISABLED) {
    float thrust = ReadRemoteThrottle();
    if (secondsElapsed > armingSequenceTimeLastStepExecuted) {
      if ((armingSequenceStep == ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED) && (thrust == 0.0)) {
        armingSequenceStep = ARMING_SEQUENCE_HIGH_THROTTLE_REQUIRED;
        armingSequenceTimeLastStepExecuted = secondsElapsed;
      }
      else if (armingSequenceStep == ARMING_SEQUENCE_HIGH_THROTTLE_REQUIRED && thrust == 100.0) {
        armingSequenceStep = ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED_AGAIN;
        armingSequenceTimeLastStepExecuted = secondsElapsed;
      }
      else if (armingSequenceStep == ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED_AGAIN && thrust == 0.0) {
        armingSequenceStep = ARMING_SEQUENCE_ARMED;
        armingSequenceTimeLastStepExecuted = secondsElapsed;
        TurnOff(YELLOW_LED);
        TurnOn(BLUE_LED);
      }
     }
     WaitAFewMillis(10);
  }

  uint8_t data_recevie[3]={0};
  uint16_t loopsPerSecond = 0;
  uint32_t thisSecond = 0;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//	  // Wait 1s to initialize ESC
//	  WaitAFewMillis(3000);
//	  aProp.set(1230);
//	  bProp.set(1230);
//	  cProp.set(1230);
//	  dProp.set(1230);
//	  TurnOn(YELLOW_LED);
//	  WaitAFewMillis(3000);
//	  TurnOff(YELLOW_LED);
//	  WaitAFewMillis(3000);
//	  thrust = ReadRemoteThrottle();
//	  rudder = ReadRemotePidProportional();
//	  aileron = ReadRemotePidIntegral();
//	  elevator = ReadResetAngularPosition();
//	  HAL_UART_Receive(&huart4, data_recevie, sizeof(data_recevie), 10);
//	  if(data_recevie[0]=='O' && data_recevie[1]=='N')
//	  {
		  loopsPerSecond++;
			ReadAngularPosition();

			float thrust = ReadRemoteThrottle();
			float baseMotorSpeed = 0;
			float motorAdjustment = 0;
			float bMotorSpeed = 0.0;
			float eMotorSpeed = 0.0;
			float cMotorSpeed = 0.0;
			float aMotorSpeed = 0.0;
			float xAdjustment = 0.0;
			float yAdjustment = 0.0;

			if ((armingSequenceStep != ARMING_SEQUENCE_ARMED || ARMING_SEQUENCE_IS_DISABLED) && secondsElapsed > armingSequenceTimeLastStepExecuted) {
				if (armingSequenceStep == ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED && thrust == 0.0) {
					armingSequenceStep = ARMING_SEQUENCE_HIGH_THROTTLE_REQUIRED;
					armingSequenceTimeLastStepExecuted = secondsElapsed;

				} else if (armingSequenceStep == ARMING_SEQUENCE_HIGH_THROTTLE_REQUIRED && thrust == 100.0) {
					armingSequenceStep = ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED_AGAIN;
					armingSequenceTimeLastStepExecuted = secondsElapsed;

				} else if (armingSequenceStep == ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED_AGAIN && thrust == 0.0) {
					armingSequenceStep = ARMING_SEQUENCE_ARMED;
					armingSequenceTimeLastStepExecuted = secondsElapsed;

					TurnOff(YELLOW_LED);
					TurnOn(BLUE_LED);
				}
			} else if (armingSequenceStep == ARMING_SEQUENCE_ARMED) {
				xAdjustment = CalculatePidAdjustment(&xAxisPid, angularPosition.x, 0.0);
				yAdjustment = CalculatePidAdjustment(&yAxisPid, angularPosition.y, 0.0);

				if (xAdjustment < PID_MINIMUM_BOUND) { xAdjustment = PID_MINIMUM_BOUND; }
				if (xAdjustment > PID_MAXIMUM_BOUND) { xAdjustment = PID_MAXIMUM_BOUND; }
				if (yAdjustment < PID_MINIMUM_BOUND) { yAdjustment = PID_MINIMUM_BOUND; }
				if (yAdjustment > PID_MAXIMUM_BOUND) { yAdjustment = PID_MAXIMUM_BOUND; }

				if (thrust == 0.0) {
					/* always turn it off when the throttle is zero, independent of throttle constants */
					bProp.set(1000);
					eProp.set(1000);
					cProp.set(1000);
					aProp.set(1000);
				} else {
					/* throttle is converted to a range of -50 to +50 */
					baseMotorSpeed = MOTOR_SPEED_REQUIRED_FOR_LIFT + (THROTTLE_SENSITIVITY * (thrust - 50.0));

					bMotorSpeed = baseMotorSpeed + yAdjustment;
					eMotorSpeed = baseMotorSpeed - yAdjustment;
					cMotorSpeed = baseMotorSpeed + xAdjustment;
					aMotorSpeed = baseMotorSpeed - xAdjustment;

					/* adjust all motor speeds if one motor is outside motor speed bounds */
					/* this is a deliberate choice to prioritise desired angular position over desired thrust */
					float smallestMotorSpeed = MAXIMUM_MOTOR_SPEED;
					float largestMotorSpeed = MINIMUM_MOTOR_SPEED;

					if (bMotorSpeed < smallestMotorSpeed) { smallestMotorSpeed = bMotorSpeed; }
					if (bMotorSpeed > largestMotorSpeed) { largestMotorSpeed = bMotorSpeed; }
					if (eMotorSpeed < smallestMotorSpeed) { smallestMotorSpeed = eMotorSpeed; }
					if (eMotorSpeed > largestMotorSpeed) { largestMotorSpeed = eMotorSpeed; }
					if (cMotorSpeed < smallestMotorSpeed) { smallestMotorSpeed = cMotorSpeed; }
					if (cMotorSpeed > largestMotorSpeed) { largestMotorSpeed = cMotorSpeed; }
					if (aMotorSpeed < smallestMotorSpeed) { smallestMotorSpeed = aMotorSpeed; }
					if (aMotorSpeed > largestMotorSpeed) { largestMotorSpeed = aMotorSpeed; }

					if (smallestMotorSpeed < MINIMUM_MOTOR_SPEED) {
						motorAdjustment = MINIMUM_MOTOR_SPEED - smallestMotorSpeed;
					} else if (largestMotorSpeed > MAXIMUM_MOTOR_SPEED) {
						motorAdjustment = MAXIMUM_MOTOR_SPEED - largestMotorSpeed;
					}

					/* apply adjusted motor speeds to the motors */
					bMotorSpeed = bMotorSpeed + motorAdjustment;
					eMotorSpeed = eMotorSpeed + motorAdjustment;
					cMotorSpeed = cMotorSpeed + motorAdjustment;
					aMotorSpeed = aMotorSpeed + motorAdjustment;

					bProp.set(bMotorSpeed);
					eProp.set(eMotorSpeed);
					cProp.set(cMotorSpeed);
					aProp.set(aMotorSpeed);
				}
			}

			if (thisSecond != secondsElapsed) {
				uint8_t loopReference = rand() & 0xFF;

//				RecordIntegerMetric(METRIC_SECONDS_ELAPSED, loopReference, secondsElapsed);
//				RecordIntegerMetric(METRIC_LOOP_FREQUENCY, loopReference, loopsPerSecond);
//				RecordFloatMetric(METRIC_GYROSCOPE_X_POSITION, loopReference, gyroscopeReading.x);
//				RecordFloatMetric(METRIC_GYROSCOPE_Y_POSITION, loopReference, gyroscopeReading.y);
//				RecordFloatMetric(METRIC_GYROSCOPE_Z_POSITION, loopReference, gyroscopeReading.z);
//				RecordFloatMetric(METRIC_GYROSCOPE_TEMPERATURE, loopReference, gyroscopeReading.gyroscopeTemperature);
//				RecordIntegerMetric(METRIC_GYROSCOPE_SAMPLE_RATE, loopReference, gyroscopeReading.readings);
//				RecordFloatMetric(METRIC_PROPELLOR_B_SPEED, loopReference, bMotorSpeed);
//				RecordFloatMetric(METRIC_PROPELLOR_E_SPEED, loopReference, eMotorSpeed);
//				RecordFloatMetric(METRIC_PROPELLOR_C_SPEED, loopReference, cMotorSpeed);
//				RecordFloatMetric(METRIC_PROPELLOR_A_SPEED, loopReference, aMotorSpeed);
//				RecordFloatMetric(METRIC_PID_X_ADJUSTMENT, loopReference, xAdjustment);
//				RecordFloatMetric(METRIC_PID_Y_ADJUSTMENT, loopReference, yAdjustment);
//				RecordFloatMetric(METRIC_REMOTE_PID_PROPORTIONAL, loopReference, xAxisPid.proportional);
				RecordFloatMetric(METRIC_REMOTE_THROTTLE, loopReference, thrust);
				RecordFloatMetric(METRIC_ACCELEROMETER_X_POSITION, loopReference, accelerometerReading.x);
				RecordFloatMetric(METRIC_ACCELEROMETER_Y_POSITION, loopReference, accelerometerReading.y);
//				RecordFloatMetric(METRIC_ACCELEROMETER_Z_POSITION, loopReference, accelerometerReading.z);
//				RecordIntegerMetric(METRIC_ACCELEROMETER_SAMPLE_RATE, loopReference, accelerometerReading.readings);
//				RecordFloatMetric(METRIC_ANGULAR_X_POSITION, loopReference, angularPosition.x);
//				RecordFloatMetric(METRIC_ANGULAR_Y_POSITION, loopReference, angularPosition.y);
//				RecordFloatMetric(METRIC_ANGULAR_Z_POSITION, loopReference, angularPosition.z);
//				RecordIntegerMetric(METRIC_METRICS_BUFFER_SIZE, loopReference, metricsRingBuffer.count);
				RecordFloatMetric(METRIC_DEBUG_VALUE_1, loopReference, baseMotorSpeed);
				RecordFloatMetric(METRIC_DEBUG_VALUE_2, loopReference, motorAdjustment);

				loopsPerSecond = 0;
				accelerometerReading.readings = 0;
				gyroscopeReading.readings = 0;
				thisSecond = secondsElapsed;
				ClearWarnings();
			}

			if (intermediateMillis % ANALYTICS_FLUSH_FREQUENCY == 0) {
				FlushMetrics();
			}
//	  }
//	  else{
//	  	// Stop ESC
//			bProp.set(1000);
//			eProp.set(1000);
//			cProp.set(1000);
//			aProp.set(1000);
//	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief Callback function delay 1 mili second
  * @param stucture of htime
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Check correct timer 2
	if(htim->Instance == htim2.Instance){
		intermediateMillis++;					// Increase milisecond
		if (intermediateMillis % 1000 == 0) {
			secondsElapsed++;					// Increase second
		}

		// After 49 days, what to do? Kaboom! Until we require something better.
		// Check if milisecond max is 0xffffffff (UINT_LEAST32_MAX)
		if (intermediateMillis == UINT_LEAST32_MAX) {
			HardFault_Handler();				//Call function in stm32f4xx_it.h to monitor to user
		}
	}
}

/**
  * @brief Callback function for pwm input
  * @param stucture of htime
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint32_t IC1Value = 0;
	uint32_t IC2Value = 0;
	float updatedDutyCycle = 0;
	float updatedFrequency = 0;
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		IC1Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		if(IC1Value!=0)
		{
			IC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			updatedDutyCycle = ((IC2Value*100.0f)/IC1Value);
			if (htim->Instance == htim4.Instance){
				updatedFrequency = \
				  ((HAL_RCC_GetHCLKFreq() / (pwmInputTimer4.hclckDivisor)) \
				  /(IC1Value*1000));
			}
			else if (htim->Instance == htim5.Instance){
				updatedFrequency = \
				  ((HAL_RCC_GetHCLKFreq() / (pwmInputTimer5.hclckDivisor)) \
				  /(IC1Value*1000));
			}
			else if (htim->Instance == htim9.Instance){
				updatedFrequency = \
				  ((HAL_RCC_GetHCLKFreq() / (pwmInputTimer9.hclckDivisor)) \
				  /(IC1Value*1000));
			}
			else if (htim->Instance == htim12.Instance){
				updatedFrequency = \
				  ((HAL_RCC_GetHCLKFreq() / (pwmInputTimer12.hclckDivisor)) \
				  /(IC1Value*1000));
			}
			/* eliminate noise that is more than twice the previous duty cycle */
			if (isnan(updatedDutyCycle)
				|| isnan(updatedFrequency)
				|| updatedDutyCycle <= 0.0
				|| updatedFrequency <= 0.0
				|| IC1Value == IC2Value){
				return;
			}
			if (htim->Instance == htim4.Instance){
				pwmInputTimer4.dutyCycle = updatedDutyCycle;
				pwmInputTimer4.frequency = updatedFrequency;
			}
			else if (htim->Instance == htim5.Instance){
				pwmInputTimer5.dutyCycle = updatedDutyCycle;
				pwmInputTimer5.frequency = updatedFrequency;
			}
			else if (htim->Instance == htim9.Instance){
				pwmInputTimer9.dutyCycle = updatedDutyCycle;
				pwmInputTimer9.frequency = updatedFrequency*2;
			}
			else if (htim->Instance == htim12.Instance){
				pwmInputTimer12.dutyCycle = updatedDutyCycle;
				pwmInputTimer12.frequency = updatedFrequency;
			}
		}
		else
		{
			/* Do nothing */
		}
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}


/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8399;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 19999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xffff;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 999;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xffff;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim5, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 999;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 0xffff;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim9, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM12_Init(void)
{

  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 999;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 0xffff;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim12, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM12_Init 2 */

  /* USER CODE END TIM12_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
