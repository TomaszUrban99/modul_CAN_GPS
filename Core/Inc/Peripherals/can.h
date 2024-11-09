
#ifndef INC_PERIPHERALS_CAN_H_
#define INC_PERIPHERALS_CAN_H_

#include "stm32f429xx.h"
#include "gpiob.h"

#define RCC_APB1ENR_CAN1							(1U << 25)

#define CAN_BTR_BDP_0								0U
#define CAN_BR_PRESCALER							5

/* CAN filter mode register */
#define CAN_FM1R_FBM0								(1U << 0)
#define CAN_FM1R_FBM1								(1U << 1)

/* CAN filter scale register */
#define CAN_FS1R_FSC0								(1U << 0)
#define CAN_FS1R_FSC1								(1U << 1)

/* CAN FIFO filter assignment */
#define CAN_FFA1R_FFA0								(1U << 0)
#define CAN_FFA1R_FFA1								(1U << 1)

/* CAN filter activation */
#define CAN_FA1R_FACT0								(1U << 0)
#define CAN_FA1R_FACT1								(1U << 1)

typedef struct can_frame {


} can_frame;

void can_filter_init ();

void can_init ();



#endif /* INC_PERIPHERALS_CAN_H_ */
