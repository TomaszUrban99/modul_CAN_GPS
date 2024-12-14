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
	USART3->CR1 |= CR1_IDLEIE;

	/* Enable UART */
	USART3->CR1 |= CR1_UE;

	/* Enable interrupt in NVIC */
	NVIC_SetPriority(USART3_IRQn,15);
	NVIC_EnableIRQ(USART3_IRQn);
}

void uart5_rx_tx_init(void){

	/* Configure GPIO pins (PORTD, PORTC) */

	/* Enable access to clock for pins PORTD and PORTC */
	RCC->AHB1ENR |= RCC_GPIOC_EN;
	RCC->AHB1ENR |= RCC_GPIOD_EN;

	/* Set GPIO PC12 mode to alternate mode */
	GPIOC->MODER |= MODE_PC12_1;
	GPIOC->MODER &= ~MODE_PC12_0;

	/* Set alternate function to usart tx */
	GPIOC->AFR[1] |= AFRH12_3;
	GPIOC->AFR[1] &= ~AFRH12_2;
	GPIOC->AFR[1] &= ~AFRH12_1;
	GPIOC->AFR[1] &= ~AFRH12_0;

	/* Set GPIO PD2 mode to alternate mode */
	GPIOD->MODER |= MODE_PD2_1;
	GPIOD->MODER &= ~MODE_PD2_0;

	/* Set alternate function to usart rx AF8 */
	GPIOD->AFR[0] |= AFRL2_3;
	GPIOD->AFR[0] &= ~AFRL2_2;
	GPIOD->AFR[0] &= ~AFRL2_1;
	GPIOD->AFR[0] &= ~AFRL2_0;

	/* Configure UART */
	RCC->APB1ENR |= RCC_UART5_EN;

	/* Set baudrate */
	set_uart_baudrate(UART5, APB1_CLK, UART5_BAUDRATE);

	/* Enable DMA */
	UART5->CR3 = CR3_DMAT | CR3_DMAR;

	/* Set transfer direction */
	UART5->CR1 = CR1_TE | CR1_RE;

	/* Clear all flags */
	UART5->SR &= ~SR_TC;
	UART5->SR &= ~SR_IDLE;

	/* Enable IDLE line interrupt */
	UART5->CR1 |= CR1_IDLEIE;

	/* Enable UART */
	UART5->CR1 |= CR1_UE;

	/* Enable UART5 interrupt in NVIC */
	NVIC_SetPriority(UART5_IRQn,15);
	NVIC_EnableIRQ(UART5_IRQn);
}

void dma1_init(void){

	/* Enable clock access to DMA1 */
	RCC->AHB1ENR |= RCC_DMA1_EN;

	/* Enable DMA interrupt in NVIC */
	/*NVIC_EnableIRQ(DMA1_Stream3_IRQn);*/

	/* Enable DMA Stream3 interrupt in NVIC */
	NVIC_SetPriority(DMA1_Stream3_IRQn,15);
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);

	/* Enable DMA Stream7 interrupt in NVIC */
	NVIC_SetPriority(DMA1_Stream7_IRQn,15);
	NVIC_EnableIRQ(DMA1_Stream7_IRQn);
}

void dma1_stream0_rx_config(uint32_t rx_buffer){

	/* Disable DMA1 stream 0 */
	DMA1_Stream0->CR &= ~DMA_SXCR_EN;

	/* Wait until EN bit is set to 0 */
	while ( DMA1_Stream0->CR & DMA_SXCR_EN ){}

	/* Clear all interrupt flags related to DMA1 stream 0 */
	DMA1->LIFCR = LIFCR_CTCIF0 | LIFCR_CHTIF0 | LIFCR_CTEIF0 |
			LIFCR_CDMEIF0 | LIFCR_CFEIF0;

	/* Set peripheral address */
	DMA1_Stream0->PAR = (uint32_t)(&(UART5->DR));

	/* Set memory address */
	DMA1_Stream0->M0AR = (uint32_t)(rx_buffer);

	/* Set number of transfers */
	DMA1_Stream0->NDTR = (uint16_t) BUFFER_SIZE_USART2;

	/* Select channel (4) */
	DMA1_Stream0->CR &= ~DMA_SXCR_CHSEL_0;
	DMA1_Stream0->CR &= ~DMA_SXCR_CHSEL_1;
	DMA1_Stream0->CR |= DMA_SXCR_CHSEL_2;

	/* Enable memory address increment */
	DMA1_Stream0->CR |= DMA_SXCR_MINC;

	/* Enable transfer and half transfer complete interrupt */
	DMA1_Stream0->CR |= DMA_SXCR_TCIE | DMA_SXCR_HTIE;

	/* Enable circular mode */
	DMA1_Stream0->CR |= DMA_SXCR_CRC;

	/* Set transfer direction (peripheral to memory) */
	DMA1_Stream0->CR &= ~DMA_SXCR_DIR_1;
	DMA1_Stream0->CR &= ~DMA_SXCR_DIR_0;

	/* Enable DMA stream */
	DMA1_Stream0->CR |= DMA_SXCR_EN;

	NVIC_SetPriority(DMA1_Stream0_IRQn,15);
	/* Enable DMA Stream5 interrupt in NVIC */
	NVIC_EnableIRQ(DMA1_Stream0_IRQn);
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
	DMA1_Stream1->NDTR = (uint16_t) BUFFER_SIZE_USART2;

	/* Select channel (4) */
	DMA1_Stream1->CR &= ~DMA_SXCR_CHSEL_0;
	DMA1_Stream1->CR &= ~DMA_SXCR_CHSEL_1;
	DMA1_Stream1->CR |= DMA_SXCR_CHSEL_2;

	/* Enable memory address increment */
	DMA1_Stream1->CR |= DMA_SXCR_MINC;

	/* Enable transfer complete interrupt */
	DMA1_Stream1->CR |= DMA_SXCR_TCIE | DMA_SXCR_HTIE;

	/* Enable circular mode */
	DMA1_Stream1->CR |= DMA_SXCR_CRC;

	/* Set transfer direction (peripheral to memory) */
	DMA1_Stream1->CR &= ~DMA_SXCR_DIR_1;
	DMA1_Stream1->CR &= ~DMA_SXCR_DIR_0;

	/* Enable DMA stream */
	DMA1_Stream1->CR |= DMA_SXCR_EN;

	/* Enable DMA Stream1 interrupt in NVIC */
	NVIC_SetPriority(DMA1_Stream1_IRQn,15);
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

void dma1_stream7_tx_config(uint32_t source_buffer, uint32_t length){

	/*Disable DMA1 stream 3*/
	DMA1_Stream7->CR &= ~DMA_SXCR_EN;

	/* Wait until DMA1 stream3 enable bit is 0 */
	while((DMA1_Stream7->CR & DMA_SXCR_EN)) {}

	/* Clear all interrupt flags related to DMA1 stream 3 */
	DMA1->LIFCR = HIFCR_CTCIF7 | HIFCR_CHTIF7 | HIFCR_CTEIF7 | HIFCR_CDMEIF7 | HIFCR_CFEIF7;

	/* Set peripheral address */
	DMA1_Stream7->PAR = (uint32_t) (&(UART5->DR));

	/* Set memory address */
	DMA1_Stream7->M0AR = (uint32_t)(source_buffer);

	/* Set number of transfers */
	DMA1_Stream7->NDTR = length;

	/* Select channel (4) */
	DMA1_Stream7->CR &= ~DMA_SXCR_CHSEL_0;
	DMA1_Stream7->CR &= ~DMA_SXCR_CHSEL_1;
	DMA1_Stream7->CR |= DMA_SXCR_CHSEL_2;

	/* Enable memory address increment */
	DMA1_Stream7->CR |= DMA_SXCR_MINC;

	/* Set transfer direction (memory to peripheral 01) */
	DMA1_Stream7->CR &= ~DMA_SXCR_DIR_1;
	DMA1_Stream7->CR |= DMA_SXCR_DIR_0;

	DMA1_Stream7->CR |= DMA_SXCR_TCIE;

	/* Enable DMA stream */
	DMA1_Stream7->CR |= DMA_SXCR_EN;
}

void usart2_process_data ( uint8_t *ptr, size_t length, char *message ){

	strncat(message,(char*) ptr, length);

	/*if ( !parseMessage(gpsPtr, (char *) ptr, length )){

	}*/

}

void usart3_process_data ( uint8_t *ptr, size_t length, char *response ){

	/* Copy length bytes to ptr */
	strncat(response,(char*)ptr, length);

}

void usart2_dma_check_buffer ( uart_ds *ptr, char *message ){

	size_t pos = BUFFER_SIZE_USART2 - (size_t) GET_DMA_DATA_LENGTH_USART2();

	if ( pos >= 0 ){

		if ( pos != ptr->old_pos ){

		if ( pos > ptr->old_pos ){
			usart2_process_data(&(ptr->uart_rx_dma_buffer[ptr->old_pos]),pos - ptr->old_pos, message);
		}
		else{
			usart2_process_data(&(ptr->uart_rx_dma_buffer[ptr->old_pos]),
					BUFFER_SIZE_USART2 - ptr->old_pos, message);
			if ( pos > 0 ){
				usart2_process_data(&(ptr->uart_rx_dma_buffer[0]),pos, message);
			}
		}

		ptr->old_pos = pos;
	}
	}
}

int usart3_dma_check_buffer( uart_ds *ptr, char *response ){

	size_t pos = BUFFER_SIZE_USART2 - (size_t) GET_DMA_DATA_LENGTH_USART3();

	if ( pos >= 0 ){

		if ( pos != ptr->old_pos ){

		if ( pos > ptr->old_pos ){
			usart3_process_data(&(ptr->uart_rx_dma_buffer[ptr->old_pos]),pos - ptr->old_pos,response);
		}
		else{
			usart3_process_data(&(ptr->uart_rx_dma_buffer[ptr->old_pos]),BUFFER_SIZE_USART2 - ptr->old_pos,response);
			if ( pos > 0 ){
				usart3_process_data(&(ptr->uart_rx_dma_buffer[0]),pos, response);
			}
		}

		ptr->old_pos = pos;
	}

	}

	return (int) strlen(response);
}



