#include "can.h"

/*!
 *  \brief Initialize can filter
 */
void can_filter_init ( uint16_t can_client_id, uint32_t can_filter_mask ){

	/* Enter filter initialization mode */
	CAN1->FMR |= CAN_FMR_FINIT;

	/* Deactivate filter bank */
	CAN1->FA1R &= ~CAN_FA1R_FACT0;

	/*----------------- FILTER BANK 0 -----------------------*/

	/* Use filter bank 0 in IDENTIFIER MASK MODE 1 */
	CAN1->FM1R &= ~CAN_FM1R_FBM0;

	/* CAN filter scale - use 1 single 32 bit register */
	CAN1->FS1R |= CAN_FS1R_FSC0;

	/* assign to FIFO 0 */
	CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

	/* Filter activation register
	CAN1->FA1R |= CAN_FA1R_FACT0; */

	/*--------------- FILTER CONTENT ------------------------*/

	/* Register 1 - filter */
	CAN1->sFilterRegister[0].FR1 = (can_client_id << STDID_POS);

	/* Register 2 - filter mask */
	CAN1->sFilterRegister[0].FR2 = (CAN_FILTER_MASK);

	/* Filter activation register */
	CAN1->FA1R |= CAN_FA1R_FACT0;

	/*-------------------------------------------------------*/

	/*----------------- FILTER BANK 1 -----------------------*/

	/* Use filter bank 0 in IDENTIFIER MASK MODE 1 */
	/*CAN1->FM1R |= CAN_FM1R_FBM1;*/

	/* CAN filter scale - use 1 single 32 bit register */
	/*CAN1->FS1R |= CAN_FS1R_FSC1;*/

	/* assign to FIFO 0 */
	/*CAN1->FFA1R |= CAN_FFA1R_FFA1;*/

	/* Filter activation register */
	/*CAN1->FA1R |= CAN_FA1R_FACT1;*/

	/*--------------- FILTER CONTENT ------------------------*/

	/* Register 1 - filter */
	/*CAN1->sFilterRegister[1].FR1 = (can_client_id << STDID_POS);*/

	/* Register 2 - filter mask */
	/*CAN1->sFilterRegister[1].FR2 = (can_filter_mask << STDID_POS);*/

	/*-------------------------------------------------------*/

	/* Leave filter initialization mode */
	CAN1->FMR &= ~CAN_FMR_FINIT;

}

void can_init (){

	/* Enable clock access to CAN */
	RCC->APB1ENR |= RCC_APB1ENR_CAN1;

	/*---------------------- GPIOD ---------------------*/

	/* Enable pins - port D */
	RCC->AHB1ENR |= RCC_GPIOD_EN;

	/* PD0 - CAN1_RD */
	GPIOD->MODER |= MODE_PD0_1;
	GPIOD->MODER &= ~MODE_PD0_0;

	/* Set PD0 alternate function to CAN1_RD (AF9) */
	GPIOD->AFR[0] |= AFRL0_3;
	GPIOD->AFR[0] &= ~AFRL0_2;
	GPIOD->AFR[0] &= ~AFRL0_1;
	GPIOD->AFR[0] |= AFRL0_0;

	/* PD1 - CAN1_TD */
	GPIOD->MODER |= MODE_PD1_1;
	GPIOD->MODER &= ~MODE_PD1_0;

	/* Set PD1 alternate function to CAN1_TD (AF9) */
	GPIOD->AFR[0] |= AFRL1_3;
	GPIOD->AFR[0] &= ~AFRL1_2;
	GPIOD->AFR[0] &= ~AFRL1_1;
	GPIOD->AFR[0] |= AFRL1_0;

	/* ------------------------------------------------------ */

	/* ENTER initialization mode - set INRQ in CAN_MC */
	CAN1->MCR |= CAN_MCR_INRQ;

	/* Wait until INAK bit in CAN_MSR is set */
	while ( ! (CAN1->MSR & CAN_MSR_INAK) ){}

	/* Leave sleep mode, after reset CAN is in SLEEP mode */
	CAN1->MCR &= ~CAN_MCR_SLEEP;

	/* Wait until SLAK bit in CAN_MSR is cleared */
	while ( CAN1->MSR & CAN_MSR_SLAK ) {}

	/* Set CAN options */

	/*-------------------- Set bit timing --------------------*/

	/* For debugging purposes */
	/*CAN1->BTR = (1U << 30);*/

	/*
	 * Bit Rate - 500k
	 * SJW	-	1
	 * BS1	-	11
	 * BS2	-	2
	 * BRP	- 	11
	 */

	/* Set BTR */
	CAN1->BTR = 0x001a0005;

	/* ------------------- Set receiver -----------------------*/
	CAN1->IER |= ( CAN_IER_FMPIE0 | CAN_IER_FMPIE1 );


	/*---------------------------------------------------------*/

	/*------------------- Set filters -------------------------*/
	can_filter_init(CAN_ECU_ID, CAN_FILTER_MASK);

	/* LEAVE initialization mode */
	CAN1->MCR &= ~CAN_MCR_INRQ;

	NVIC_SetPriority(CAN1_RX0_IRQn,15);
	NVIC_EnableIRQ(CAN1_RX0_IRQn);

	NVIC_SetPriority(CAN1_RX1_IRQn,15);
	NVIC_EnableIRQ(CAN1_RX1_IRQn);


	/* Wait until INAK bit in CAN_MSR is cleared */
	while ( (CAN1->MSR & CAN_MSR_INAK) ){}
}

int can_send_request ( struct can_frame *msg, uint8_t pid_code ){

	/* Set CAN ID */
	msg->_can_id = CAN_CLIENT_ID;

	msg->_dlc = DATA_FRAME_LENGTH; /* Set data length */

	msg->_data[0] = 0x02;
	msg->_data[1] = PID_01;
	msg->_data[2] = pid_code;

	for ( int i = 3; i < DATA_FRAME_LENGTH; ++i ){
		msg->_data[i] = 0xaa;
	}

	return can_send(msg);

	/* Alternative

	msg->_dlc = 0x03;

	msg->_data[0] = 0x02;
	msg->_data[1] = PID_01;
	msg->_data[2] = pid_code;
	*/

}

int can_process ( struct can_frame *msg, struct can_data_collection *collection ){

	switch ( msg->_data[2] ){

		case CAN_ENGINE_RPM:

			if ( msg->_data[1] == PID_01_POSITIVE_RESPONSE ){
				collection->_engine_rpm = engine_rpm_receive(msg->_data);
			} else {
				collection->_engine_rpm = -1;
				return -1;
			}

		break;

		case CAN_ENGINE_LOAD:

			if ( msg->_data[1] == PID_01_POSITIVE_RESPONSE ){
				collection->_engine_load = engine_load_receive(msg->_data);
			} else {
				collection->_engine_load = -1;
				return -1;
			}

		break;

		case CAN_TANK_LEVEL:

			if ( msg->_data[1] == PID_01_POSITIVE_RESPONSE ){
				collection->_tank_level = tank_level_receive(msg->_data);
			} else {
				collection->_tank_level = -1;
				return -1;
			}

		break;
	}

	return 0;
}

/*
 *
 */
uint16_t engine_rpm_receive ( uint8_t *can_data ){
	return (256 * can_data[3] + can_data[4])/4;
}

uint8_t engine_load_receive ( uint8_t *can_data ){
	return (100 * can_data[3])/255;
}

uint8_t tank_level_receive ( uint8_t *can_data ){
	return (100 * can_data[3])/255;
}

int can_receive(struct can_frame *msg, uint8_t FIFO_No ){

		/* Read message can id */
		msg->_can_id = ((CAN1->sFIFOMailBox[FIFO_No].RIR & 0xffe00000) >> 21);

		/* Read message length */
		msg->_dlc = (CAN1->sFIFOMailBox[FIFO_No].RDTR & 0xf);

		for ( uint8_t i = 0; i < msg->_dlc; ++i ){

			if ( i < 4 ){
				msg->_data[i] = (CAN1->sFIFOMailBox[FIFO_No].RDLR >> (8 * i));
			}
			else {
				msg->_data[i] = ( CAN1->sFIFOMailBox[FIFO_No].RDHR >> ((8*(i)) % 32));
			}
		}

		if ( FIFO_No == 0){
			CAN1->RF0R |= (1U << 5);
		} else {
			CAN1->RF1R |= (1U << 5);
		}

		return 0;
}

int can_send ( struct can_frame *msg ){

	/* 	Select empty mailbox -
	 *  first step use only one mailbox No. 0
	 *  Wait until transmit mailbox 0 is empty */
	while(!(CAN1->TSR & CAN_TSR_TME0_M)){}

	/* Set up identifier */
	CAN1->sTxMailBox[0].TIR = ( msg->_can_id << STDID_POS );

	/* Set up DLC - [0:3] bits describe DLC */
	CAN1->sTxMailBox[0].TDTR = ( msg->_dlc );

	CAN1->sTxMailBox[0].TDLR = 0;
	CAN1->sTxMailBox[0].TDHR = 0;

	/* Set up DATA */
	for ( int i = 0; i < msg->_dlc; ++i){

		if ( i < 4){
			/* Write least significant bytes to TDLR register
			 * TDLR stores first four bytes */
			CAN1->sTxMailBox[0].TDLR |= ( msg->_data[i] << (i * 8));
		}
		else {
			/* TDHR stores last four bytes of CAN frame */
			CAN1->sTxMailBox[0].TDHR |= ( msg->_data[i] << ((i%4) * 8));
		}
	}

	/* Request transmission- set TXRQ */
	CAN1->sTxMailBox[0].TIR |= CAN_TIxR_TXRQ;

	/* Wait until transmission takes place
	 * Meanwhile check possible transmission errors */
	while ( !(CAN1->TSR & (CAN_TSR_RQCP0 | CAN_TSR_TXOK0)) ){

		/* Check possible errors - Arbitration Lost ALST0 or
		 * Transmission Error Detection TERR0
		 */
		if ( (CAN1->TSR & CAN_TSR_ALST0) || (CAN1->TSR & CAN_TSR_TERR0) ){
			return 1;
		}
	}

	return 0;

}


