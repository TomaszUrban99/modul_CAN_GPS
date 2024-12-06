/*
 * gsm_module.c
 *
 *  Created on: Dec 2, 2024
 *      Author: tomasz
 */

#include "gsm_module.h"

extern SemaphoreHandle_t GSM_receiver;
extern SemaphoreHandle_t TX_cplt;
extern uart_ds usart3;

int count_r_n ( char *response, int r_n_number ){

	char *ptr;
	int number_of_occurences = 0;

	ptr = strstr(response,"\r\n");

	while ( ptr != NULL ){

		ptr += 2;
		number_of_occurences++;
		ptr = strstr(ptr,"\r\n");

	}

	return number_of_occurences;
}

int receive_line_module (SemaphoreHandle_t sim_module,
		uart_ds *uart3, char *response,  int r_n_number ){

	int length = 0;
	response[0] = '\0';

	while (1){

		/* Wait until new message has come */
		xSemaphoreTake(sim_module,portMAX_DELAY);

		usart3_dma_check_buffer(uart3,response);

		length = count_r_n(response,r_n_number);

		if ( length == r_n_number ){
			return 0;
		} else if ( length > r_n_number ){
			return -1;
		}

	}

	return 0;
}

int activate_wireless_conn(char *msg){



	return 0;
}

int configure_module(){

	char messg[64];
	char response[64];

	/* Handshake - check hardware */
	strcpy(messg,"AT\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	receive_line_module(GSM_receiver,&usart3,response,2);

	/* Enable full functionality of the modem */
	do {
		strcpy(messg,"AT+CFUN=1\r\n");
		dma1_stream3_tx_config((uint32_t) messg, strlen(messg));
		xSemaphoreTake(TX_cplt, portMAX_DELAY);
		receive_line_module(GSM_receiver,&usart3,response,2);
		GPIOB->ODR ^= ODR_PB14;
	} while ( strstr(response,"OK") == NULL );

	GPIOB->ODR &= ~ODR_PB14;


	/* Check if SIM is ready to action */
	do {
		strcpy(messg,"AT+CPIN?\r\n");
		dma1_stream3_tx_config((uint32_t) messg, strlen(messg));
		xSemaphoreTake(TX_cplt, portMAX_DELAY);
		receive_line_module(GSM_receiver,&usart3,response,3);
		GPIOB->ODR ^= ODR_PB14;

		while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}


			for ( size_t i = 0; i < strlen(response); ++i ){

				UART5->DR = response[i];

				while ( !(UART5->SR & SR_TXE) ) {}

				while(	!(UART5->SR & SR_TC )) {}

		}
	} while ( strstr(response,": READY") == NULL );

	GPIOB->ODR &= ~ODR_PB14;

	while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}


		for ( size_t i = 0; i < strlen(response); ++i ){

			UART5->DR = response[i];

			while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}

	}

	/* Set APN name */
	do {
		strcpy(messg,"AT+CSTT=\"internet\",\"\",\"\"\r\n");
		dma1_stream3_tx_config((uint32_t) messg, strlen(messg));
		xSemaphoreTake(TX_cplt, portMAX_DELAY);
		receive_line_module(GSM_receiver,&usart3,response,2);
		GPIOB->ODR ^= ODR_PB14;

		while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}


			for ( size_t i = 0; i < strlen(response); ++i ){

				UART5->DR = response[i];

				while ( !(UART5->SR & SR_TXE) ) {}

				while(	!(UART5->SR & SR_TC )) {}

		}
	} while ( strstr(response,"internet") == NULL );

	GPIOB->ODR &= ~ODR_PB14;

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

}



	/* Start wireless connection */
	do {
		strcpy(messg,"AT+CIICR\r\n");
		dma1_stream3_tx_config((uint32_t) messg, strlen(messg));
		xSemaphoreTake(TX_cplt, portMAX_DELAY);
		receive_line_module(GSM_receiver,&usart3,response,2);
		GPIOB->ODR ^= ODR_PB14;
	} while ( strstr(response,"OK") == NULL);

	GPIOB->ODR &= ~ODR_PB14;

	while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}


		for ( size_t i = 0; i < strlen(response); ++i ){

			UART5->DR = response[i];

			while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}

	}


		strcpy(messg,"AT+CIFSR\r\n");
		dma1_stream3_tx_config((uint32_t) messg, strlen(messg));
		xSemaphoreTake(TX_cplt, portMAX_DELAY);
		receive_line_module(GSM_receiver,&usart3,response,2);
		GPIOB->ODR ^= ODR_PB14;

		while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}


			for ( size_t i = 0; i < strlen(response); ++i ){

				UART5->DR = response[i];

				while ( !(UART5->SR & SR_TXE) ) {}

				while(	!(UART5->SR & SR_TC )) {}

		}

	GPIOB->ODR &= ~ODR_PB14;

	char *ptr;
	ptr = strstr(response,"\r\n");
	ptr += 2;

	while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}


		for ( size_t i = 0; i < strlen(response); ++i ){

			UART5->DR = response[i];

			while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}

	}


	/* Wait until answer is received */


	/* Handshake */
	/*strcpy(messg,"ATI\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);
	usart3_dma_check_buffer(&usart3);


	strcpy(messg,"AT+CPIN?\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);
	usart3_dma_check_buffer(&usart3);

	strcpy(messg,"AT+CCID\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);
	usart3_dma_check_buffer(&usart3);

	strcpy(messg,"AT+CREG=1\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);
	usart3_dma_check_buffer(&usart3);*/

	/*strcpy(messg,"AT+CSQ\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt,portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);
	usart3_dma_check_buffer(&usart3);

	vTaskDelay(8000);*/

	/*strcpy(messg,"AT+CFUN?\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt,portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);
	usart3_dma_check_buffer(&usart3);

	vTaskDe	strcpy(messg,"AT+COPS=1,2,\"26002\"\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt,portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);

	GPIOB->ODR |= ODR_PB14;

	usart3_dma_check_buffer(&usart3);

	GPIOB->ODR ^= ODR_PB14;

	vTaskDelay(5000);
	lay(8000);*/


	/*strcpy(messg,"AT+COPS?\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt,portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);

	GPIOB->ODR |= ODR_PB14;

	usart3_dma_check_buffer(&usart3);

	GPIOB->ODR ^= ODR_PB14;

	vTaskDelay(5000);

	strcpy(messg,"AT+CGDCONT?\r\n");
	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);

	vTaskDelay(5000);
	usart3_dma_check_buffer(&usart3);

	vTaskDelay(2000);

	strcpy(messg, "AT+CREG?\r\n");

	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);

	vTaskDelay(5000);
	usart3_dma_check_buffer(&usart3);

	vTaskDelay(2000);

	strcpy(messg, "AT+CSTT=\"internet\"\r\n");

	dma1_stream3_tx_config((uint32_t) messg, strlen(messg));

	xSemaphoreTake(TX_cplt, portMAX_DELAY);
	xSemaphoreTake(GSM_receiver,portMAX_DELAY);

	vTaskDelay(5000);
	usart3_dma_check_buffer(&usart3);*/

	vTaskDelay(2000);






	return 0;
}
