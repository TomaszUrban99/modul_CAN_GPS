/*
 * uart.c
 *
 *  Created on: Oct 15, 2024
 *      Author: tomasz
 */
#include "uart.h"


/********************* USART INTERRUPT FLAGS ************************************/

/***************************** USART 2 ******************************************/

uint16_t compute_uart_div ( uint32_t peripheralClock, uint32_t baudRate ){
	return ((peripheralClock + (baudRate/2U))/baudRate);
	/*return peripheralClock / ( 8 * 2 * baudRate );*/

}

void set_uart_baudrate(USART_TypeDef *USART, uint32_t peripheralClock, uint32_t baudRate){
	USART->BRR = compute_uart_div(peripheralClock,baudRate);
}

/*!
 * 	Configure uart2 peripheral
 *
 * 	Transmit - use DMA controller
 * 	Receive PIN - do not use DMA controller, use interrupt
 *
 *
 ************************** GPS MODULE ******************************
 */
void uart2_rx_tx_init(){

	/************** Configure UART2 GPIO PIN (PORT D) ***************/

	/* Enable access to PORTD */
	RCC->AHB1ENR |= RCC_GPIOD_EN;

	/************************ TRANSMIT PIN ******************************/

	/* Set PD5 to alternate function */
	GPIOD->MODER |= MODE_PD5_1;
	GPIOD->MODER &= ~(MODE_PD5_0);

	/* Set PD5 alternate function to TX */
	GPIOD->AFR[0] &= ~(AFRL5_3);
	GPIOD->AFR[0] |= AFRL5_2;
	GPIOD->AFR[0] |= AFRL5_1;
	GPIOD->AFR[0] |= AFRL5_0;

	/************************* RECEIVE PIN *****************************/

	/* Set PD6 to alternate function */
	GPIOD->MODER |= MODE_PD6_1;
	GPIOD->MODER &= ~(MODE_PD6_0);

	/* Set PD6 alternate function to RX (AF7) */
	GPIOD->AFR[0] &= ~(AFRL6_3);
	GPIOD->AFR[0] |= AFRL6_2;
	GPIOD->AFR[0] |= AFRL6_1;
	GPIOD->AFR[0] |= AFRL6_0;

	/************************ Configure UART ***************************/

	/* Enable clock access to UART2 */
	RCC->APB1ENR |= RCC_USART2_EN;

	set_uart_baudrate(USART2,APB1_CLK,USART2_BAUDRATE);

	/* Enable DMA for TX */
	USART2->CR3 = CR3_DMAT | CR3_DMAR;

	/* Configure transfer direction */
	USART2->CR1 = CR1_TE | CR1_RE;

	/* Clear all flags */
	USART2->SR &= ~SR_TC;
	USART2->SR &= ~SR_IDLE;

	/* Enable receive and transfer interrupt */
	/*
	 * -> Receive interrupt enable
	 * -> TCIE transmission complete interrupt
	 *
	 */
	USART2->CR1 |= CR1_IDLEIE;

	/* Enable UART */
	USART2->CR1 |= CR1_UE;


	/* Enable USART2 interrupt in NVIC */
	NVIC_SetPriority(USART2_IRQn,15);
	NVIC_EnableIRQ(USART2_IRQn);

}

void uart3_rx_tx_init(void){

	/************** Configure UART3 GPIO PIN (PORT B) ***************/

	/* Enable access to PORTB */
	RCC->AHB1ENR |= RCC_GPIOB_EN;

	/************************ TRANSMIT PIN ******************************/

	/* Set PB10 to alternate function */
	GPIOB->MODER |= MODE_PB10_1;
	GPIOB->MODER &= ~MODE_PB10_0;

	/* Set PB10 alternate function to TX */
	GPIOB->AFR[1] &= ~AFRH10_3;
	GPIOB->AFR[1] |= AFRH10_2;
	GPIOB->AFR[1] |= AFRH10_1;
	GPIOB->AFR[1] |= AFRH10_0;

	/************************* RECEIVE PIN *****************************/

	/* Set PB11 to alternate function */
	GPIOB->MODER |= MODE_PB11_1;
	GPIOB->MODER &= ~MODE_PB11_0;

	/* Set PB11 alternate function to RX */
	GPIOB->AFR[1] &= ~AFRH11_3;
	GPIOB->AFR[1] |= AFRH11_2;
	GPIOB->AFR[1] |= AFRH11_1;
	GPIOB->AFR[1] |= AFRH11_0;

	/************************* Configure UART **************************/

	/* Enable UART3 access to clock */
	RCC->APB1ENR |= RCC_USART3_EN;

	/* Set baudrate */
	set_uart_baudrate(USART3,APB1_CLK,USART3_BAUDRATE);

	/* Select to use DMA module for UART */
	USART3->CR3 = CR3_DMAT | CR3_DMAR;

	/* Configure transfer direction */
	USART3->CR1 = CR1_TE | CR1_RE;

	/* Clear all flags */
	USART3->SR &= ~SR_TC;
	USART3->SR &= ~SR_IDLE;

	/* Enable receive and transfer interrupt */
	/* USART3->CR1 |= CR1_TCIE | CR1_RXNEIE;*/

	/* Enable UART */
	USART3->CR1 |= CR1_UE;

	/* Enable interrupt in NVIC */
	NVIC_EnableIRQ(USART3_IRQn);
}

void dma1_init(void){

	/* Enable clock access to DMA1 */
	RCC->AHB1ENR |= RCC_DMA1_EN;

	/* Enable DMA interrupt in NVIC */
	/*NVIC_EnableIRQ(DMA1_Stream3_IRQn);*/

	/* Enable DMA Stream3 interrupt in NVIC */
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
}

void dma1_stream1_rx_config(uint32_t dst_buffer){

	/* Disable DMA1 stream 1 */
	DMA1_Stream1->CR &= ~DMA_SXCR_EN;

	/* Wait until EN bit is set to 0 */
	while ( DMA1_Stream1->CR & DMA_SXCR_EN ){}

	/* Clear all interrupt flags related to stream 1 */
	DMA1->LIFCR = LIFCR_CTCIF1 | LIFCR_CHTIF1 | LIFCR_CTEIF1 |
			LIFCR_CDMEIF1 | LIFCR_CFEIF1;
	/* Writing 1 to each of those bits clears
	 * corresponding bit in interrupt register */

	/* Set peripheral address */
	DMA1_Stream1->PAR = (uint32_t)(&(USART3->DR));

	/* Set memory address */
	DMA1_Stream1->M0AR = (uint32_t)(dst_buffer);

	/* Set number of transfers */
	DMA1_Stream1->NDTR = (uint16_t) BUFFER_SIZE_USART3;

	/* Select channel (4) */
	DMA1_Stream1->CR &= ~DMA_SXCR_CHSEL_0;
	DMA1_Stream1->CR &= ~DMA_SXCR_CHSEL_1;
	DMA1_Stream1->CR |= DMA_SXCR_CHSEL_2;

	/* Enable memory address increment */
	DMA1_Stream1->CR |= DMA_SXCR_MINC;

	/* Enable transfer complete interrupt */
	DMA1_Stream1->CR |= DMA_SXCR_TCIE;

	/* Enable circular mode */
	DMA1_Stream1->CR |= DMA_SXCR_CRC;

	/* Set transfer direction (peripheral to memory) */
	DMA1_Stream1->CR &= ~DMA_SXCR_DIR_1;
	DMA1_Stream1->CR &= ~DMA_SXCR_DIR_0;

	/* Enable DMA stream */
	DMA1_Stream1->CR |= DMA_SXCR_EN;

	/* Enable DMA Stream1 interrupt in NVIC */
	NVIC_EnableIRQ(DMA1_Stream1_IRQn);
}

void dma1_stream3_tx_config(uint32_t source_buffer, uint32_t length){


	/*Disable DMA1 stream 3*/
	DMA1_Stream3->CR &= ~DMA_SXCR_EN;

	/* Wait until DMA1 stream3 enable bit is 0 */
	while((DMA1_Stream3->CR & DMA_SXCR_EN)) {}

	/* Clear all interrupt flags related to DMA1 stream 3 */
	DMA1->LIFCR = LIFCR_CTCIF3 | LIFCR_CHTIF3 | LIFCR_CTEIF3 | LIFCR_CDMEIF3 | LIFCR_CFEIF3;

	/* Set peripheral address */
	DMA1_Stream3->PAR = (uint32_t) (&(USART3->DR));

	/* Set memory address */
	DMA1_Stream3->M0AR = (uint32_t)(source_buffer);

	/* Set number of transfers */
	DMA1_Stream3->NDTR = length;

	/* Select channel (4) */
	DMA1_Stream3->CR &= ~DMA_SXCR_CHSEL_0;
	DMA1_Stream3->CR &= ~DMA_SXCR_CHSEL_1;
	DMA1_Stream3->CR |= DMA_SXCR_CHSEL_2;

	/* Enable memory address increment */
	DMA1_Stream3->CR |= DMA_SXCR_MINC;

	/* Set transfer direction (memory to peripheral 01) */
	DMA1_Stream3->CR &= ~DMA_SXCR_DIR_1;
	DMA1_Stream3->CR |= DMA_SXCR_DIR_0;

	DMA1_Stream3->CR |= DMA_SXCR_TCIE;

	/* Enable DMA stream */
	DMA1_Stream3->CR |= DMA_SXCR_EN;

}

/********************** USART2 DMA STREAMS ***************************/
void dma1_stream5_rx_config(uint32_t rx_buffer){

	/* Disable DMA1 stream 5 */
		DMA1_Stream5->CR &= ~ DMA_SXCR_EN;

		/* Wait until EN bit is set to 0 */
		while ( DMA1_Stream5->CR & DMA_SXCR_EN ){}

		/* Clear all interrupt flags related to stream 1 */
		DMA1->HIFCR = HIFCR_CTCIF5 | HIFCR_CHTIF5 |
					HIFCR_CTEIF5 | HIFCR_CDMEIF5 | HIFCR_CFEIF5;

		/* Writing 1 to each of those bits clears
		 * corresponding bit in interrupt register */

		/* Set peripheral address */
		DMA1_Stream5->PAR = (uint32_t)(&(USART2->DR));

		/* Set memory address */
		DMA1_Stream5->M0AR = (uint32_t)(rx_buffer);

		/* Set number of transfers */
		DMA1_Stream5->NDTR = (uint16_t) BUFFER_SIZE_USART2;

		/* Select channel (4) */
		DMA1_Stream5->CR |= DMA_SXCR_CHSEL_2;
		DMA1_Stream5->CR &= ~DMA_SXCR_CHSEL_1;
		DMA1_Stream5->CR &= ~DMA_SXCR_CHSEL_0;

		/* Enable memory address increment */
		DMA1_Stream5->CR |= DMA_SXCR_MINC;

		/* Enable transfer complete interrupt */
		DMA1_Stream5->CR |= DMA_SXCR_TCIE | DMA_SXCR_HTIE;

		/* Enable circular mode */
		DMA1_Stream5->CR |= DMA_SXCR_CRC;

		/* Set transfer direction (peripheral to memory) */
		DMA1_Stream5->CR &= ~DMA_SXCR_DIR_1;
		DMA1_Stream5->CR &= ~DMA_SXCR_DIR_0;

		/* Enable DMA stream */
		DMA1_Stream5->CR |= DMA_SXCR_EN;

		NVIC_SetPriority(DMA1_Stream5_IRQn,15);
		/* Enable DMA Stream5 interrupt in NVIC */
		NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void usart2_process_data ( gps *gpsPtr, uint8_t *ptr, size_t length ){

	if ( !parseMessage(gpsPtr, (char *) ptr, length )){

		sprintf(gpsPtr->gps_buffer,"Location: %f %c, %f %c\n", gpsPtr->_latitude,
				gpsPtr->_latitude_attitude, gpsPtr->_longitude, gpsPtr->_longitude_attitude);


		for ( size_t i = 0; i < strlen(gpsPtr->gps_buffer); ++i){
			USART3->DR = gpsPtr->gps_buffer[i];
			GPIOB->ODR ^= ODR_PB7;
			while(!(USART3->SR & SR_TXE)){}
		}

		while(!(USART3->SR & SR_TC)) {}

	}

}

void usart2_dma_check_buffer ( uart_ds *ptr, gps *gpsPtr ){

	size_t pos = BUFFER_SIZE_USART2 - (size_t) GET_DMA_DATA_LENGTH_USART2();

	if ( pos >= 0 ){

		if ( pos != ptr->old_pos ){

		if ( pos > ptr->old_pos ){
			usart2_process_data(gpsPtr, &(ptr->uart_rx_dma_buffer[ptr->old_pos]),pos - ptr->old_pos);
		}
		else{
			usart2_process_data(gpsPtr, &(ptr->uart_rx_dma_buffer[ptr->old_pos]),BUFFER_SIZE_USART2 - ptr->old_pos);
			if ( pos > 0 ){
				usart2_process_data(gpsPtr,&(ptr->uart_rx_dma_buffer[0]),pos);
			}
		}

		ptr->old_pos = pos;
	}
	}
}

void DMA1_Stream1_IRQHandler(void){

	/* Transfer complete interrupt */
	if ((DMA1->LISR) & LIFSR_CTCIF1){

		DMA1->LIFCR |= LIFCR_CTCIF1;
	}
}

void DMA1_Stream3_IRQHandler(void){

	/* Transfer complete interrupt */
	if ((DMA1->LISR) & LIFSR_CTCIF3){

		GPIOB->ODR |= ODR_PB14;
		DMA1->LIFCR |= LIFSR_CTCIF3;
	}
}

void DMA1_Stream6_IRQHandler(void){

}

void uart2_callback(void){

}


void USART3_IRQHandler(void){

}

/**************************** READ & WRITE BUFFER ***********************************/

/* Store character in buffer */
/*void store_char(ring_buffer *ptr, char ch){

	 int i = (unsigned int) (ptr->_head + 1) & BUFFER_SIZE;

	 if ( i != ptr->_tail ){
		 ptr->_buffer[ptr->_head] = ch;
		 ptr->_head = i;
	 }
}*/

/* Read from RX uart buffer
int uart_read(ring_buffer *ptr){

	 if ( ptr->_head == ptr->_tail ){
		 return -1;
	 }
	 else {
		 uint8_t c = ptr->_buffer[ptr->_tail];
		 ptr->_tail = (uint16_t)(ptr->_tail + 1) % BUFFER_SIZE;
		 return c;
	 }
}

void uart_write(ring_buffer *ptr, int ch){

	if (ch>=0)
		{
			int i = (ptr->_head + 1) % BUFFER_SIZE;

			while (i == ptr->_tail);

			ptr->_buffer[ptr->_head] = (uint8_t)ch;
			ptr->_head = i;

		}
}*/

