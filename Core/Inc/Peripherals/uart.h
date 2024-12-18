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

#include <stdio.h>
#include <string.h>

#include "stm32f429xx.h"
#include "USART_registers.h"

#define BUFFER_SIZE_USART2										512
#define BUFFER_SIZE_USART3										128
#define BUFFER_SIZE_UART5										64

/******************************* DATA STRUCTURE *************************************************/

/*! USART data structure */
typedef struct uart_ds {

	/* DMA reception buffer */
	uint8_t uart_rx_dma_buffer[BUFFER_SIZE_USART2];

	size_t old_pos;

	/* Pointer to destination buffer for data processing */
	uint8_t *dst_buffer;

} uart_ds;

#define GET_DMA_DATA_LENGTH_UART5()									(DMA1_Stream0->NDTR)
#define GET_DMA_DATA_LENGTH_USART2()								(DMA1_Stream5->NDTR)
#define GET_DMA_DATA_LENGTH_USART3()								(DMA1_Stream1->NDTR)

/********************************* BAUDRATE *****************************************************/

#define USART2_BAUDRATE 								9600
#define USART3_BAUDRATE									9600
#define UART5_BAUDRATE									9600


/********************************** USART CLOCK ACCESS ******************************************/

#define RCC_UART5_EN												(	1U << 20	)

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

/* UART3 RX buffer */
void dma1_stream1_rx_config(uint32_t dst_buffer);

/* UART3 TX buffer */
void dma1_stream3_tx_config(uint32_t source_buffer, uint32_t length);

/* UART2 RX buffer */
void dma1_stream5_rx_config(uint32_t rx_buffer);

/* UART5 RX buffer */
void dma1_stream0_rx_config(uint32_t rx_buffer);

/* UART5 TX buffer */
void dma1_stream7_tx_config(uint32_t source_buffer, uint32_t length );

/* DMA check & process data */
void usart2_dma_check_buffer ( uart_ds *ptr, char *message );

void usart3_process_data ( uint8_t *ptr, size_t length, char *response );

int usart3_dma_check_buffer( uart_ds *ptr, char *response );

int usart3_dma_check_sim800l_answer( uart_ds *ptr );

int usart3_process_sim800l_answer( uint8_t *ptr, size_t length );

/*!
 * 	\brief
 *
 * 	\par
 */
void usart2_process_data ( uint8_t *ptr, size_t length, char *message );


/*************************** RETARGETING PRINTF *************************************************/

int __io_putchar(int ch);

/******************************* INTERRUPTS *****************************************************/

/* static void uart_callback(void);*/



#endif /* UART_H_ */
