
#ifndef INC_PERIPHERALS_CAN_H_
#define INC_PERIPHERALS_CAN_H_

#include "stm32f429xx.h"
#include "gpiob.h"

#define RCC_APB1ENR_CAN1							(1U << 25)

#define CAN_BTR_BDP_0								0U
#define CAN_BR_PRESCALER							6

#define CAN_TIxR_TXRQ								(1U << 0)

#define CAN_CLIENT_ID								0x7e8
#define STDID_POS									20

#define CAN_FILTER_MASK								0xffe00000

#define DATA_FRAME_LENGTH							8

#define CAN_TSR_TME0_M								(1U << 26)

typedef struct can_frame {

	uint16_t _can_id; /* CAN frame id */

	uint8_t _dlc; /* Data length code */

	uint8_t _data[DATA_FRAME_LENGTH]; /* Data */

} can_frame;

void can_filter_init ( uint16_t can_client_id, uint32_t can_filter_mask);

void can_init ();

int can_send ( struct can_frame *msg );

#endif /* INC_PERIPHERALS_CAN_H_ */
