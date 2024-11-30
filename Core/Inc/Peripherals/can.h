
#ifndef INC_PERIPHERALS_CAN_H_
#define INC_PERIPHERALS_CAN_H_

#include "stm32f429xx.h"
#include "gpiob.h"

#define RCC_APB1ENR_CAN1							(1U << 25)

#define CAN_BTR_BDP_0								0U
#define CAN_BR_PRESCALER							6

#define CAN_TIxR_TXRQ								(1U << 0)

#define CAN_ECU_ID									0x7df
#define CAN_CLIENT_ID								0x7e8
#define STDID_POS									21

#define CAN_FILTER_MASK								0xffe00000

#define DATA_FRAME_LENGTH							8

#define CAN_TSR_TME0_M								(1U << 26)

/* PID request code */
#define PID_01										0x01
#define PID_01_POSITIVE_RESPONSE					0x41

/* PID 0x01 reuest code */
#define CAN_ENGINE_RPM								0x0c
#define CAN_ENGINE_LOAD								0x04
#define CAN_TANK_LEVEL								0x2f

/* FIFO number */
#define CAN_FIFO_0									0x00
#define CAN_FIFO_1									0x01

/* CAN frame */
typedef struct can_frame {

	uint16_t _can_id; /* CAN frame id */

	uint8_t _dlc; /* Data length code */

	uint8_t _data[DATA_FRAME_LENGTH]; /* Data */

} can_frame;

/* Collection of received data from CAN */
typedef struct can_data_collection {

	uint16_t _engine_rpm;

	uint8_t _engine_load;

	uint8_t _tank_level;

} can_data_collection;

void can_filter_init ( uint16_t can_client_id, uint32_t can_filter_mask);

void can_init ();

int can_process ( struct can_frame *msg, struct can_data_collection *collection );

/*!
 * \brief Receive engine RPM from can message data part
 *
 * \param uint8_t* can_data - pointer to array with CAN frame's data part
 *
 */
uint16_t engine_rpm_receive ( uint8_t *can_data );

uint8_t engine_load_receive ( uint8_t *can_data );

uint8_t tank_level_receive ( uint8_t *can_data );

int can_receive(struct can_frame *msg, uint8_t FIFO_No );

int can_send ( struct can_frame *msg );

/*!
 *	\brief Send OBD request
 */
int can_send_request ( struct can_frame *msg, uint8_t pid_code );

#endif /* INC_PERIPHERALS_CAN_H_ */
