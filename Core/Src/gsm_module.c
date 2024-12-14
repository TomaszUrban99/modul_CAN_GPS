/*
 * gsm_module.c
 *
 *  Created on: Dec 2, 2024
 *      Author: tomasz
 */

#include "gsm_module.h"

extern SemaphoreHandle_t GSM_receiver;
extern SemaphoreHandle_t TX_cplt;

void send_at_command ( const char *message ) {

	/* Configure DMA controller for transmission */
	dma1_stream3_tx_config((uint32_t) message, strlen(message));
	/* Wait until transmission completes */
	xSemaphoreTake(TX_cplt, portMAX_DELAY);

}

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

int receive_at_command (SemaphoreHandle_t sim_module,
		uart_ds *uart3, char *response,  int r_n_number ){

	int length = 0;
	memset(response,'\0',64);

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

void wait_until_registration(SemaphoreHandle_t sim_module, uart_ds *uart3){

	char response[32];

		/* Send and check network registration status */
		send_at_command("AT+CREG?\r\n");
		receive_at_command(sim_module,uart3,response,3);

		vTaskDelay(1000);

	return;

}

int start_tcpip_connection(SemaphoreHandle_t sim_module,
		uart_ds *uart3, char *ip_dst_address, char *dst_port){

	char message[64];
	char response[128];

	/* Number of attempts */
	uint8_t i = 0;



	do {

		/* Prepare message */
		sprintf(message,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",ip_dst_address,dst_port);
		send_at_command(message);

		GPIOB->ODR |= ODR_PB14;
		receive_at_command(sim_module, uart3, response, 3);
		GPIOB->ODR &= ~ODR_PB14;
		i++;

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}


		for ( size_t i = 0; i < strlen(response); ++i ){

			UART5->DR = response[i];

			while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}

		}


	} while ((strstr(response,"CONNECT OK") == NULL) && (i < NUMBER_OF_ATTEMPTS_TCPIP) );

	if ( i == NUMBER_OF_ATTEMPTS_TCPIP){
		return 1;
	}

	return 0;
}

int activate_wireless_conn(char *msg){



	return 0;
}

int configure_module( SemaphoreHandle_t sim_module,
		uart_ds *uart3 ){

	char response[64];

	/* Handshake - check hardware */
	send_at_command("AT\r\n");
	receive_at_command(sim_module,uart3,response,2);



	send_at_command("AT+CSQ\r\n");
	receive_at_command(sim_module,uart3,response,2);

	if ( strstr(response,"OK") == NULL ){
		return 1;
	}

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}


	/* Very important-> enter status IP INITIAL */
	send_at_command("AT+CIPSHUT\r\n");
	receive_at_command(sim_module,uart3,response,2);

	if ( strstr(response,"SHUT OK") == NULL ){
		return 1;
	}

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}

	/* Enable full functionality of the modem */
	send_at_command("AT+CFUN=1\r\n");
	receive_at_command(sim_module,uart3,response,2);

	if ( strstr(response,"OK") == NULL ){
		return 1;
	}

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}

	/* Check if SIM is ready to action */
	send_at_command("AT+CPIN?\r\n");
	receive_at_command(sim_module,uart3,response,3);

	if ( strstr(response,": READY") == NULL ){
		return 1;
	}


	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}


	send_at_command("AT+CREG?\r\n");
	receive_at_command(sim_module,uart3,response,2);

	if ( strstr(response,"OK") == NULL ){
		return 1;
	}

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}



	if ( strstr(response,"0,1") == NULL ){
		return 1;
	}

	if ( strstr(response,"OK") == NULL ){
		return 1;
	}


	send_at_command("AT+CGATT?\r\n");
	receive_at_command(sim_module,uart3,response,3);

	if ( strstr(response,": 1") == NULL ){
		return 1;
	}

	GPIOB->ODR &= ~ODR_PB14;


	/* Set APN name */
	send_at_command("AT+CSTT=\"internet\",\"\",\"\"\r\n");
	receive_at_command(sim_module,uart3,response,2);

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}


		for ( size_t i = 0; i < strlen(response); ++i ){

			UART5->DR = response[i];

			while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}

		}

	if ( strstr(response,"internet") == NULL ){
		return 1;
	}


	/* Start wireless connection */
	send_at_command("AT+CIICR\r\n");
	receive_at_command(sim_module,uart3,response,2);

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

			UART5->DR = response[i];

			while ( !(UART5->SR & SR_TXE) ) {}

			while(	!(UART5->SR & SR_TC )) {}

		}

	if ( strstr(response,"OK") == NULL){
		return 1;
	}


	send_at_command("AT+CIFSR\r\n");
	receive_at_command(sim_module,uart3,response,2);

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}

	return 0;
}

int start_wireless_connection(){
	return 0;
}

int send_tcpip_message ( char *message_to_send, SemaphoreHandle_t sim_module,
		uart_ds *uart3 ){

	char resp[128];

	/* Request TCPIP data transmission */
	send_at_command("AT+CIPSEND\r\n");

	/* Wait until ">" sign from module arrives */
	xSemaphoreTake(sim_module,portMAX_DELAY);

	/* Receive ">" sign */
	usart3_dma_check_buffer(uart3, resp);

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(resp); ++i ){

		UART5->DR = resp[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}


	/* ">" sign not found */
	if ( strstr(resp,">") == NULL ){
		return 1;
	}


	send_at_command(message_to_send);

	receive_at_command(sim_module,uart3,resp,2);

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(resp); ++i ){

		UART5->DR = resp[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}

	/* If answer is other than SEND OK - try to connect once again */
	if ( strstr(resp,"SEND OK") == NULL ){
		return 1;
	}

	GPIOB->ODR |= ODR_PB0;

	return 0;
}

/* Check IP connection status */
int check_conn_status ( SemaphoreHandle_t sim_module, uart_ds *uart3 ){

	char response[64];

	/* Send AT command- check connection status */
	send_at_command("AT+CIPSTATUS\r\n");

	/* Receive response */
	receive_at_command(sim_module,uart3,response,3);

	while ( !(UART5->SR & SR_TXE) ) {}

	while(	!(UART5->SR & SR_TC )) {}


	for ( size_t i = 0; i < strlen(response); ++i ){

		UART5->DR = response[i];

		while ( !(UART5->SR & SR_TXE) ) {}

		while(	!(UART5->SR & SR_TC )) {}

	}

	if ( strstr(response,"INITIAL") != NULL){
		return IP_INITIAL;
	} else if ( strstr(response, "IP START") != NULL ){
		return IP_START;
	} else if ( strstr(response, "IP CONFIG") != NULL){
		return IP_CONFIG;
	} else if ( strstr(response, "IP GPRSACT") != NULL){
		return IP_GPRSACT;
	} else if ( strstr(response, "IP STATUS") != NULL){
		return IP_STATUS;
	} else if ( strstr(response, "TCP CONNECTING") != NULL){
		return TCP_CONNECTING;
	} else if ( strstr(response, "CONNECT OK") != NULL){
		return CONNECT_OK;
	}  else if ( strstr(response, "TCP CLOSING") != NULL){
		return TCP_CLOSING;
	}  else if ( strstr(response, "TCP CLOSED") != NULL){
		return TCP_CLOSED;
	}  else if ( strstr(response, "PDP DEACT") != NULL){
		return PDP_DEACT;
	} else {
		return -1;
	}

}
