/*
 * uart.h
 *
 *  Created on: Oct 15, 2024
 *      Author: tomasz
 */

#ifndef UART_H_
#define UART_H_

#include "config.h"
#include "gpiob.h"
#include "gps.h"

#include <stdio.h>
#include <string.h>

#include "stm32f429xx.h"
#include "USART_registers.h"

#define BUFFER_SIZE_USART2										512
#define BUFFER_SIZE_USART3										128

/******************************* DATA STRUCTURE *************************************************/

/*! USART data structure */
typedef struct uart_ds {

	/* DMA reception buffer */
	uint8_t uart_rx_dma_buffer[BUFFER_SIZE_USART2];

	size_t old_pos;

	/* Pointer to destination buffer for data processing */
	uint8_t *dst_buffer;

} uart_ds;

#define GET_DMA_DATA_LENGTH_USART2()								(DMA1_Stream5->NDTR)

/********************************* BAUDRATE *****************************************************/

#define USART2_BAUDRATE 								9600
#define USART3_BAUDRATE									9600

uint16_t compute_uart_div ( uint32_t peripheralClock, uint32_t baudRate );

void set_uart_baudrate(USART_TypeDef *USART, uint32_t peripheralClock, uint32_t baudRate);

/*************************************************************************************************/

/**************************** INITIALIZE FUNCTIONS ***********************************************/

void uart2_rx_tx_init();

void uart3_rx_tx_init(void);

void uart5_rx_tx_init(void);

/************************************************************************************************/

/************************************ DMA *******************************************************/

void dma1_init(void);

void dma1_stream1_rx_config(uint32_t dst_buffer);

void dma1_stream3_tx_config(uint32_t source_buffer, uint32_t length);

void dma1_stream5_rx_config(uint32_t rx_buffer);

void dma1_stream0_rx_config(uint32_t rx_buffer);

void dma1_stream7_tx_config(uint32_t source_buffer, uint32_t length );

/* DMA check & process data */
void usart2_dma_check_buffer ( uart_ds *ptr, gps *gpsPtr );

void usart2_process_data ( gps *gpsPtr, uint8_t *ptr, size_t length );
/*void uart_read(USART_TypeDef *USART, int *ch);*/


/*************************** RETARGETING PRINTF *************************************************/

int __io_putchar(int ch);

/******************************* INTERRUPTS *****************************************************/

/* static void uart_callback(void);*/

void DMA1_Stream1_IRQHandler(void);

void DMA1_Stream3_IRQHandler(void);


#endif /* UART_H_ */
