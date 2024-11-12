#include "tasks.h"

/*void usart2_dma_rx_task ( void *queuePtr ){

	QueueHandle_t ptr = (QueueHandle_t) queuePtr;

	int d;


	while(1){

		GPIOB->ODR ^= ODR_PB0;
		xQueueReceive(ptr, (void *) &d, portMAX_DELAY);
		GPIOB->ODR ^= ODR_PB0;

	}

}*/


void GPSmodule ( void *parameters ){


	while(1){

	}
}

void PCconnector ( void *parameters ){

	while(1){

	}
}
