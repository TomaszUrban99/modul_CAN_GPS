/*
 * gsm_module.hh
 *
 *  Created on: Dec 2, 2024
 *      Author: tomasz
 */

#ifndef INC_GSM_MODULE_H_
#define INC_GSM_MODULE_H_

#include <string.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include "uart.h"

#define NUMBER_OF_ATTEMPTS_TCPIP			5
#define NUMBER_OF_ATTEMPTS					5

enum connection_state { IP_INITIAL, IP_START,
	IP_CONFIG, IP_GPRSACT,
	IP_STATUS, TCP_CONNECTING,
	CONNECT_OK, TCP_CLOSING,
	TCP_CLOSED, PDP_DEACT
};

void send_at_command ( const char *message );

int count_r_n ( char *response, int r_n_number );

int receive_at_command ( SemaphoreHandle_t sim_module,
		uart_ds *uart3, char *response, int r_n_number );

void wait_until_registration( SemaphoreHandle_t sim_module, uart_ds *uart3);

int configure_module( SemaphoreHandle_t sim_module,
		uart_ds *uart3 );

int start_tcpip_connection(SemaphoreHandle_t sim_module,
		uart_ds *uart3, char *ip_dst_address, char *dst_port);

int send_tcpip_message ( char *message_to_send, SemaphoreHandle_t sim_module,
		uart_ds *uart3 );

int activate_wireless_conn(char *msg);

int check_conn_status ( SemaphoreHandle_t sim_module, uart_ds *uart3 );

int wait_for_conn();


#endif /* INC_GSM_MODULE_H_ */
