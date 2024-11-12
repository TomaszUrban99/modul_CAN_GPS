#include "main.h"

#include "FreeRTOS.h"
#include <queue.h>
#include <string.h>

#include "cmsis_os.h"
#include "gpiob.h"
#include "gps.h"
#include "task.h"
#include "uart.h"
#include "can.h"

#define QUEUE_LENGTH								10

/************** DATA STRUCTURES *********************/
uart_ds usart2;
gps gpsData;

/**************** SYNCHRONISE ***********************/
QueueHandle_t gpsReceiver;
TaskHandle_t ptr = NULL;
BaseType_t xHigherPriorityTaskWoken;

void SystemClock_Config(void);

void USART2_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);

void initTask ( void *parameters );
void usart2_dma_rx_task(void *parameters);
void can_task (void *parameters );

void blinkLed ( void *parameters ){

	while(1){
		GPIOB->ODR ^= ODR_PB14;
		vTaskDelay(1000);
	}
}

int main(void)
{

	/* HAL_Init contains NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4) */
	HAL_Init();
	SystemClock_Config();

	gpiob_init(); /* Init user's LED's */

	/* Create QUEUE's */
	if ( (gpsReceiver = xQueueCreate(QUEUE_LENGTH,sizeof(uint8_t))) == NULL ){
	  GPIOB->ODR |= ODR_PB14;
	}

	uart2_rx_tx_init();
	uart3_rx_tx_init();
	dma1_init();
	dma1_stream5_rx_config((uint32_t) usart2.uart_rx_dma_buffer);

	can_init();

	if ( pdPASS != xTaskCreate(usart2_dma_rx_task,"DMAU2",2048,NULL,configMAX_PRIORITIES-1,NULL)){
		GPIOB->ODR |= ODR_PB7;
		}

	if ( pdPASS != xTaskCreate(can_task,"CAN", 1000, NULL, configMAX_PRIORITIES-1, NULL)){
	 GPIOB->ODR |= ODR_PB7;
	}

	vTaskStartScheduler();

  while (1)
  {
  }
}

void USART2_IRQHandler(void){

	if (USART2->SR & SR_IDLE){

		uint8_t d = 1;

		xHigherPriorityTaskWoken = pdFALSE;
		USART2->DR;

		/* Check if message has been sent correctly */
		if( xQueueSendFromISR(gpsReceiver,&d,&xHigherPriorityTaskWoken) != pdTRUE ){
			GPIOB->ODR |= ODR_PB7;
		}

		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}

void DMA1_Stream5_IRQHandler(void){

	uint8_t d =1;
	xHigherPriorityTaskWoken = pdFALSE;

	if (((DMA1->HISR) & HIFSR_CTCIF5)){

		xQueueSendFromISR(gpsReceiver,&d,&xHigherPriorityTaskWoken);
		DMA1->HIFCR |= HIFCR_CTCIF5;
	}

	if (((DMA1->HISR) & HIFSR_CHTIF5)){

		xQueueSendFromISR(gpsReceiver,&d,&xHigherPriorityTaskWoken);
		DMA1->HIFCR |= HIFCR_CHTIF5;
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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

void usart2_dma_rx_task ( void *queuePtr ){

	uint8_t d = 1;

	while(1){

		GPIOB->ODR ^= ODR_PB0;

		/* Wait until there is any element from USART2 IRQ and
		 * DMA1_Stream5 IRQ handlers
		 */
		xQueueReceive(gpsReceiver, &d, portMAX_DELAY);

		/* Process message */
		usart2_dma_check_buffer(&usart2, &gpsData);
	}

}

void can_task ( void *parameters ){

	can_frame msg;
	msg._can_id = CAN_CLIENT_ID;
	msg._dlc = 8;

	for ( uint8_t i = 0; i < 8; ++i ){
		msg._data[i] = i;
	}

	while(1){

		GPIOB->ODR ^= ODR_PB14;
		can_send(&msg);
		vTaskDelay(2000);
	}
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
