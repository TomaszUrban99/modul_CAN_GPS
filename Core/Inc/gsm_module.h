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

int count_r_n ( char *response, int r_n_number );

int receive_line_module ( SemaphoreHandle_t sim_module,
		uart_ds *uart3, char *response, int r_n_number );

int configure_module();

int activate_wireless_conn(char *msg);

int wait_for_conn();


#endif /* INC_GSM_MODULE_H_ */
