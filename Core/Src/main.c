#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void GPIO_Init(void);
static void NEO_UART1_Init(void);
static void SIM800L_UART2_Init(void);

uint8_t gpsBuffer[128];

void GPS_Init(void);
void GPS_Read(void);

uint8_t rxBuffer[100];
uint8_t txBuffer[50];

void SIM800_SendString(char *cmd);
void SIM800_ReadResponse(void);
void SIM800_Init(void);

BaseType_t status;

TaskHandle_t SpeedMonitorHandle;
TaskHandle_t GpsHandle;
TaskHandle_t Sim800lHandle;

void SpeedMonitorHandler(void* paramters);
void GpsHandler(void* paramters);
void Sim800lHandler(void* paramters);

void CapChargeTaskHandler(void* parameters);


int main(void)
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	NEO_UART1_Init();
	SIM800L_UART2_Init();
	GPS_Init();
	SIM800_Init();

	status = xTaskCreate(SpeedMonitorHandler, "SpeedMonitorTask", 200, NULL, 3, &SpeedMonitorHandle);
	configASSERT(status == pdPASS);

	status = xTaskCreate(GpsHandler, "GpsTask", 200, NULL, 3, &GpsHandle);
	configASSERT(status == pdPASS);

	status = xTaskCreate(Sim800lHandler, "Sim800lTask", 200, NULL, 3, &Sim800lHandle);
	configASSERT(status == pdPASS);

	vTaskStartScheduler();


}

void SpeedMonitorHandler(void* paramters)
{
	while(1)
	{
		// will implement after components are available.
	}
}
void GpsHandler(void* paramters)
{
	while(1)
	{
		// will implement after components are available.
	}
}
void Sim800lHandler(void* paramters)
{
	while(1)
	{
		// will implement after components are available.

	}
}


void GPS_Init(void) {
    HAL_UART_Receive(&huart1, gpsBuffer, sizeof(gpsBuffer)-1, 2000);
    printf("GPS Data: %s\r\n", gpsBuffer);
}

void GPS_Read(void) {
    memset(gpsBuffer, 0, sizeof(gpsBuffer));
    HAL_UART_Receive(&huart1, gpsBuffer, sizeof(gpsBuffer)-1, 2000);
    printf("GPS Data: %s\r\n", gpsBuffer);
}


void SIM800_SendString(char *cmd) {
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
}

void SIM800_ReadResponse(void) {
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Receive(&huart2, rxBuffer, sizeof(rxBuffer)-1, 2000);
    printf("SIM800 Response: %s\r\n", rxBuffer);
}

void SIM800_Init(void) {
    SIM800_SendString("AT\r\n");
    HAL_Delay(1000);
    SIM800_ReadResponse();

    SIM800_SendString("ATE0\r\n");
    HAL_Delay(1000);
    SIM800_ReadResponse();

    SIM800_SendString("AT+CPIN?\r\n");
    HAL_Delay(1000);
    SIM800_ReadResponse();

    SIM800_SendString("AT+CREG?\r\n");
    HAL_Delay(1000);
    SIM800_ReadResponse();

    SIM800_SendString("AT+CSQ\r\n");
    HAL_Delay(1000);
    SIM800_ReadResponse();
}




void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void NEO_UART1_Init(void)
{


  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }


}


static void SIM800L_UART2_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}


static void GPIO_Init(void)
{

  __HAL_RCC_GPIOA_CLK_ENABLE();


}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM5)
  {
    HAL_IncTick();
  }

}


void Error_Handler(void)
{
   while (1);
}


