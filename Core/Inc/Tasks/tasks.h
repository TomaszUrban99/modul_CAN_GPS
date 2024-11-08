/*
 * tasks.h
 *
 *  Created on: Oct 29, 2024
 *      Author: tomasz
 */

#ifndef INC_TASKS_TASKS_H_
#define INC_TASKS_TASKS_H_

#include <FreeRTOS.h>
#include <queue.h>
#include <string.h>

#include "gpiob.h"
#include "uart.h"
#include "stm32f4xx_hal.h"

void usart2_dma_rx_task ( void *queuePtr );

void GPSmodule ( void *parameters );

void PCconnector ( void *parameters );

#endif /* INC_TASKS_TASKS_H_ */

