/*
 * CAN_registers.h
 *
 *  Created on: Nov 8, 2024
 *      Author: tomasz
 */

#ifndef INC_PERIPHERALS_CAN_REGISTERS_H_
#define INC_PERIPHERALS_CAN_REGISTERS_H_

/* CAN control and status registers */
#define CAN_MCR_DBF							(1U << 16)
#define CAN_MCR_RESET						(1U << 15)

#define CAN_MCR_TTCM						(1U << 7)
#define CAN_MCR_ABOM						(1U << 6)
#define CAN_MCR_AWUM						(1U << 5)
#define CAN_MCR_NART						(1U << 4)
#define CAN_MCR_RFLM						(1U << 3)
#define CAN_MCR_TXFP						(1U << 2)
#define CAN_MCR_SLEEP						(1U << 1)
#define CAN_MCR_INRQ						(1U << 0)

/* CAN master status register */

#define CAN_MSR_RX							(1U << 11)
#define CAN_MSR_SAMP						(1U << 10)
#define CAN_MSR_RXM							(1U << 9) /* CAN Receiver mode */
#define CAN_MSR_TXM							(1U << 8) /* CAN Transmitter mode */
#define CAN_MSR_SLAKI						(1U << 4)
#define CAN_MSR_WKUI						(1U << 3)
#define CAN_MSR_ERRI						(1U << 2)
#define CAN_MSR_SLAK						(1U << 1)
#define CAN_MSR_INAK						(1U << 0) /* Initialization mode */

/* CAN transmit status register */

#define CAN_TSR_LOW2						(1U << 31)
#define CAN_TSR_LOW1						(1U << 30)
#define CAN_TSR_LOW0						(1U << 29)
#define CAN_TSR_TME2						(1U << 28)
#define CAN_TSR_TME1						(1U << 27)
#define CAN_TSR_TME0						(1U << 26)
#define CAN_TSR_CODE_1						(1U << 25)
#define CAN_TSR_CODE_0						(1U << 24)
#define CAN_TSR_ABRQ2						(1U << 23)

#define CAN_TSR_TERR2						(1U << 19)
#define CAN_TSR_ALST2						(1U << 18)
#define CAN_TSR_TXOK2						(1U << 17)
#define CAN_TSR_RQCO2						(1U << 16)
#define CAN_TSR_ABRQ1						(1U << 15)

#define CAN_TSR_TERR1						(1U << 11)
#define CAN_TSR_ALST1						(1U << 10)
#define CAN_TSR_TXOK1						(1U << 9)
#define CAN_TSR_RQCO1						(1U << 8)
#define CAN_TSR_ABRQ0						(1U << 7)

#define CAN_TSR_TERR0						(1U << 3)
#define CAN_TSR_ALST0						(1U << 2)
#define CAN_TSR_TXOK0						(1U << 1)
#define CAN_TSR_RQCO0						(1U << 0)

/* CAN receive FIFO 0 register (CAN_RF0R) */
#define CAN_RF0R_RFOM0						(1U << 5)
#define CAN_RF0R_FOVR0						(1U << 4)
#define CAN_RF0R_FULL0						(1U << 3)
#define CAN_FMP0_1							(1U << 1)
#define CAN_FMP0_0							(1U << 0)

/* CAN receive FIFO 1 register (CAN_RF1R) */
#define CAN_RF1R_RFOM1						(1U << 5)
#define CAN_RF1R_FOVR1						(1U << 4)
#define CAN_RF1R_FULL1						(1U << 3)
#define CAN_FMP1_1							(1U << 1)
#define CAN_FMP1_0							(1U << 0)

/* CAN interrupt enable register (CAN_IER) */
#define CAN_IER_SLKIE						(1U << 17) /* Sleep interrupt */
#define CAN_IER_WKUIE						(1U << 16) /* Wake up */
#define CAN_IER_ERRIE						(1U << 15) /* Error interrupt enable */
#define CAN_IER_LECIE						(1U << 11)
#define CAN_IER_BOFIE						(1U << 10) /* Buss-off interrupt */
#define CAN_IER_EPVIE						(1U << 9) /* Error passive */
#define CAN_IER_EWGIE						(1U << 8)
#define CAN_IER_FOVIE1						(1U << 6)
#define CAN_IER_FFIE1						(1U << 5)
#define CAN_IER_FMPIE1						(1U << 4)
#define CAN_IER_FOVIE0						(1U << 3)
#define CAN_IER_FFIE0						(1U << 2)
#define CAN_IER_FMPIE0						(1U << 1)
#define CAN_IER_TMEIE						(1U << 0) /* Transmit mailbox empty */

/* CAN bit timing register (CAN_BTR) */
#define CAN_BTR_SLIM						(1U << 31)
#define CAN_BTR_LBKM						(1U << 30)

#define CAN_BTR_SJW_1						(1U << 25)
#define CAN_BTR_SJW_0						(1U << 24)

#define CAN_BTR_TS2_2						(1U << 22)
#define CAN_BTR_TS2_1						(1U << 21)
#define CAN_BTR_TS2_0						(1U << 20)

#define CAN_BTR_TS1_3						(1U << 19)
#define CAN_BTR_TS1_2						(1U << 18)
#define CAN_BTR_TS1_1						(1U << 17)
#define CAN_BTR_TS1_0						(1U << 16)

/* BRP[9:0] baud rate prescaler */

#endif /* INC_PERIPHERALS_CAN_REGISTERS_H_ */
