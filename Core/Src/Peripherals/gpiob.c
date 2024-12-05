/*
 * gpiob.c
 *
 *  Created on: Oct 25, 2024
 *      Author: tomasz
 */
#include "gpiob.h"

void gpiob_init(void){

	RCC->AHB1ENR |= RCC_GPIOB_EN;

	GPIOB->MODER &= ~MODE_PB14_1;
	GPIOB->MODER |= MODE_PB14_0;

	GPIOB->MODER &= ~MODE_PB7_1;
	GPIOB->MODER |= MODE_PB7_0;

	GPIOB->MODER &= ~MODE_PB0_1;
	GPIOB->MODER |= MODE_PB0_0;

	GPIOB->MODER &= ~MODE_PB1_1;
	GPIOB->MODER |= MODE_PB1_0;

}

