#include "can.h"

void can_filter_init (){

	/* Enter filter initialization mode */
	CAN1->FMR |= CAN_FMR_FINIT;

	/* Use filter bank 0 in IDENTIFIER LIST MODE 1 */
	CAN1->FM1R |= CAN_FM1R_FBM0;

	/* CAN filter scale - use dual 16 bit filters 0 */
	CAN1->FS1R &= ~
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
	CAN1->BTR &= ~CAN_BTR_SJW_1;
	CAN1->BTR |= CAN_BTR_SJW_0;

	CAN1->BTR &= ~CAN_BTR_TS2_2;
	CAN1->BTR &= ~CAN_BTR_TS2_1;
	CAN1->BTR |= CAN_BTR_TS2_0;

	CAN1->BTR |= CAN_BTR_TS1_3;
	CAN1->BTR &= ~CAN_BTR_TS1_0;
	CAN1->BTR |= CAN_BTR_TS1_0;
	CAN1->BTR &= ~CAN_BTR_TS1_0;

	CAN1->BTR |= ( CAN_BR_PRESCALER << CAN_BTR_BDP_0);

	/*---------------------------------------------------------*/

	/*------------------- Set filters -------------------------*/

	/* Enter filter initialization */
	CAN1->FMR |= CAN_FMR_FINIT;

	/* LEAVE initialization mode */
	CAN1->MCR &= ~CAN_MCR_INRQ;

	/* Wait until INAK bit in CAN_MSR is cleared */
	while ( (CAN1->MSR & CAN_MSR_INAK) ){}
}


