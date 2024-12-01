#include "main.h"

#include "FreeRTOS.h"

#include <event_groups.h>
#include <queue.h>
#include <string.h>
#include <semphr.h>

#include "cmsis_os.h"
#include "gpiob.h"
#include "gps.h"
#include "task.h"
#include "uart.h"
#include "can.h"
#include "packet.h"

#define QUEUE_LENGTH								10


/************** DATA STRUCTURES *********************/
uart_ds usart2;
uart_ds usart5;

struct packet packetData;

/**************** SYNCHRONISE ***********************/
QueueHandle_t gpsReceiver;
QueueHandle_t CAN_receiver;

SemaphoreHandle_t GSM_receiver;

/* Event group */
EventGroupHandle_t dataReceived;

/* Event bits */
#define OBD_MODULE									0x01
#define GPS_MODULE									0x02
#define PACKET_PREPARED								0x04

TaskHandle_t ptr = NULL;
BaseType_t xHigherPriorityTaskWoken;

can_frame msg_receive;

/******************************************************/
#define MESSAGE_LENGTH								128

void SystemClock_Config(void);

void USART2_IRQHandler(void);
void USART5_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);

void CAN1_RX0_IRQHandler(void);
void CAN1_RX1_IRQHandler(void);


void usart2_dma_rx_task(void *parameters);
void obd_module(void *parameters);
void send_task(void *parameters );


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

	if ( (CAN_receiver = xQueueCreate(QUEUE_LENGTH,sizeof(uint8_t))) == NULL ){
	  GPIOB->ODR |= ODR_PB14;
	}

	/* Create SEMAPHORES */
	if ( (GSM_receiver = xSemaphoreCreateBinary()) == NULL ){
		GPIOB->ODR |= ODR_PB14;
	}


	/* Create event group */
	if ( (dataReceived = xEventGroupCreate()) == NULL ){
		GPIOB->ODR |= ODR_PB14;
	}

	/* Event group description
	 *
	 * Bit 0 - received and processed data from OBD
	 * Bi1 1 - received and processed data from GPS
	 * Bit 2 - created packet to send
	 *
	 */

	uart2_rx_tx_init();
	uart3_rx_tx_init();

	dma1_init();
	dma1_stream5_rx_config((uint32_t) usart2.uart_rx_dma_buffer);
	dma1_stream0_rx_config((uint32_t) usart5.uart_rx_dma_buffer);

	can_init();

	if ( pdPASS != xTaskCreate(usart2_dma_rx_task,"DMAU2",256,NULL,configMAX_PRIORITIES-1,NULL)){
		GPIOB->ODR |= ODR_PB7;
		}

	if ( pdPASS != xTaskCreate(obd_module,"CAN", 256, NULL, configMAX_PRIORITIES-1, NULL)){
	 GPIOB->ODR |= ODR_PB7;
	}

	if ( pdPASS != xTaskCreate(send_task,"SEND", 256, NULL, configMAX_PRIORITIES -1, NULL)){
		GPIOB->ODR |= ODR_PB7;
	}

	vTaskStartScheduler();

  while (1)
  {
  }
}

void CAN1_RX0_IRQHandler(void){

	if ( CAN1->RF0R & CAN_RF0R_FMP0 ){

		/* FIFO No */
		uint8_t d = 0;

		xHigherPriorityTaskWoken = pdFALSE;

		if ( xQueueSendFromISR(CAN_receiver,&d,&xHigherPriorityTaskWoken) != pdPASS ){
			GPIOB->ODR |= ODR_PB7;
		}

		/* Receive message - read CAN frame */
		can_receive(&msg_receive, d);

		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);

	}

}

void CAN1_RX1_IRQHandler(void){

	if ( CAN1->RF1R & CAN_RF1R_FMP1){

			uint8_t d = 1;
			xHigherPriorityTaskWoken = pdFALSE;

			if ( xQueueSendFromISR(CAN_receiver,&d,&xHigherPriorityTaskWoken) == errQUEUE_FULL ){
				GPIOB->ODR |= ODR_PB7;
			}

			portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}

void USART2_IRQHandler(void){

	if (USART2->SR & SR_IDLE){

		uint8_t d = 1;

		xHigherPriorityTaskWoken = pdFALSE;
		USART2->DR;

		/* Check if message has been sent correctly */
		if( xQueueSendFromISR(gpsReceiver,&d,&xHigherPriorityTaskWoken) != pdPASS ){
			GPIOB->ODR |= ODR_PB7;
		}

		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}

void USART5_IRQHandler (void){

	if ( UART5->SR & SR_IDLE ){

		xHigherPriorityTaskWoken = pdFALSE;
		UART5->DR;

		if ( xSemaphoreGiveFromISR(GSM_receiver,&xHigherPriorityTaskWoken) != pdPASS ){
			GPIOB->ODR |= ODR_PB7;
		}

		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}

}

void DMA1_Stream0_IRQHandler(void){

	xHigherPriorityTaskWoken = pdFALSE;

	if (((DMA1->HISR) & LIFSR_CTCIF0)){

		xSemaphoreGiveFromISR(GSM_receiver,&xHigherPriorityTaskWoken);
		DMA1->HIFCR |= LIFCR_CTCIF0;
	}

	if (((DMA1->HISR) & LIFSR_CHTIF0)){

		xSemaphoreGiveFromISR(gpsReceiver,&xHigherPriorityTaskWoken);
		DMA1->HIFCR |= LIFCR_CHTIF0;
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
		usart2_dma_check_buffer(&usart2, &packetData._gps_data);

		/* Set bits */
		xEventGroupSetBits(dataReceived,GPS_MODULE);
	}

}

void usart5_dma_rx_tx_task ( void *queuePtr ){

	while(1){

	}
}

void send_task ( void *parameters ){

	/* Initialize array for message */
	/*uint8_t message[MESSAGE_LENGTH];
	uint8_t message_length;*/

	while (1) {

		/*xEventGroupWaitBits(dataReceived,3, pdTRUE, pdTRUE, portMAX_DELAY);*/
		/* message_length = prepare_json( &packetData,message); */

		xSemaphoreTake(GSM_receiver,portMAX_DELAY);
		uart5_dma_check_buffer(&usart5);

		/*xEventGroupSetBits(dataReceived,PACKET_PREPARED);*/
	}
}

void obd_module ( void *parameters ){

	uint8_t request_code[] = { CAN_ENGINE_RPM, CAN_ENGINE_LOAD, CAN_TANK_LEVEL};
	can_frame msg_transmit;

	/* Request code array length */
	uint8_t request_code_array_length = sizeof(request_code)/sizeof(uint8_t);
	uint8_t d = 0;

	while(1){


		for ( int i = 0; i < request_code_array_length; ++i){
			can_send_request(&msg_transmit,request_code[i]);
		}

		for ( int i = 0; i < request_code_array_length; ++i){

			/* Wait until new message incomes */
			xQueueReceive(CAN_receiver,&d, portMAX_DELAY);

			/* Receive and process message */

			can_process(&msg_receive, &packetData._obd_data);
		}

		/* Send notification that all responses have been received ? Event group ?*/
		xEventGroupSetBits(dataReceived,OBD_MODULE);
		xEventGroupWaitBits(dataReceived,PACKET_PREPARED, pdTRUE, pdTRUE, portMAX_DELAY);

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
