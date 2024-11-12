#include "can.h"

void can_filter_init ( uint16_t can_client_id, uint32_t can_filter_mask ){

	/* Enter filter initialization mode */
	CAN1->FMR |= CAN_FMR_FINIT;

	/*----------------- FILTER BANK 0 -----------------------*/

	/* Use filter bank 0 in IDENTIFIER MASK MODE 1 */
	CAN1->FM1R |= CAN_FM1R_FBM0;

	/* CAN filter scale - use 1 single 32 bit register */
	CAN1->FS1R |= CAN_FS1R_FSC0;

	/* assign to FIFO 0 */
	CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

	/* Filter activation register */
	CAN1->FA1R |= CAN_FA1R_FACT0;

	/*--------------- FILTER CONTENT ------------------------*/

	/* Register 1 - filter */
	CAN1->sFilterRegister[0].FR1 = (can_client_id << STDID_POS);

	/* Register 2 - filter mask */
	CAN1->sFilterRegister[0].FR2 = (can_filter_mask << STDID_POS);

	/*-------------------------------------------------------*/

	/*----------------- FILTER BANK 1 -----------------------*/

	/* Use filter bank 0 in IDENTIFIER MASK MODE 1 */
	CAN1->FM1R |= CAN_FM1R_FBM1;

	/* CAN filter scale - use 1 single 32 bit register */
	CAN1->FS1R |= CAN_FS1R_FSC1;

	/* assign to FIFO 0 */
	CAN1->FFA1R &= ~CAN_FFA1R_FFA1;

	/* Filter activation register */
	CAN1->FA1R |= CAN_FA1R_FACT1;

	/*--------------- FILTER CONTENT ------------------------*/

	/* Register 1 - filter */
	CAN1->sFilterRegister[1].FR1 = (can_client_id << STDID_POS);

	/* Register 2 - filter mask */
	CAN1->sFilterRegister[1].FR2 = (can_filter_mask << STDID_POS);

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

	/* Set CAN options */

	/*-------------------- Set bit timing --------------------*/
	/*
	 * Bit Rate - 500k
	 * SJW	-	1
	 * BS1	-	10
	 * BS2	-	1
	 * BRP	- 	5
	 */

	/* 15 brp 2 ph 1, 3 ph 2 */
	CAN1->BTR &= ~(1U << 25);
	CAN1->BTR |= (1U << 24);

	CAN1->BTR &= ~(1U << 22);
	CAN1->BTR &= ~(1U << 21);
	CAN1->BTR |= (1U << 20);

	CAN1->BTR &= ~(1U << 19);
	CAN1->BTR |= (1U << 18);
	CAN1->BTR |= (1U << 17);
	CAN1->BTR &= ~(1U << 16);

	CAN1->BTR |= (1U << 1);
	CAN1->BTR |= (1U << 2);
			/*( CAN_BR_PRESCALER << CAN_BTR_BDP_0);*/

	/*---------------------------------------------------------*/

	/*------------------- Set filters -------------------------*/
	can_filter_init(CAN_CLIENT_ID, CAN_FILTER_MASK);

	/* LEAVE initialization mode */
	CAN1->MCR &= ~CAN_MCR_INRQ;

	/* Wait until INAK bit in CAN_MSR is cleared */
	while ( (CAN1->MSR & CAN_MSR_INAK) ){}
}

int can_send ( struct can_frame *msg ){

	/* 	Select empty mailbox -
	 *  first step use only one mailbox No. 0
	 *  Wait until transmit mailbox 0 is empty */
	while(!(CAN1->TSR & CAN_TSR_TME0_M)){}

	/* Set up identifier */
	CAN1->sTxMailBox[0].TIR = ( msg->_can_id << STDID_POS );

	/* Set up DLC - [0:3] bits describe DLC */
	CAN1->sTxMailBox[0].TDTR |= ( msg->_dlc );

	/* Set up DATA */
	for ( int i = 0; i < msg->_dlc; ++i){

		if ( i < 4){
			/* Write least significant bytes to TDLR register
			 * TDLR stores first four bytes */
			CAN1->sTxMailBox[0].TDLR = ( msg->_data[i] << (i * 8));
		}
		else {
			/* TDHR stores last four bytes of CAN frame */
			CAN1->sTxMailBox[0].TDHR = ( msg->_data[i] << ((i%4) * 8));
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


