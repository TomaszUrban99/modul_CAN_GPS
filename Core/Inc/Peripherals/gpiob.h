/*
 * gpioa.h
 *
 *  Created on: Oct 15, 2024
 *      Author: tomasz
 */

#ifndef GPIOB_H_
#define GPIOB_H_

#include <config.h>
#include <stm32f429xx.h>

#define RCC_GPIOB_EN			(1U << 1)
#define RCC_GPIOD_EN			(1U << 3)

#define MODE_PB14_1				(1U << 29)
#define MODE_PB14_0				(1U << 28)

#define MODE_PB7_1				(1U << 15)
#define MODE_PB7_0				(1U << 14)

#define MODE_PB0_1				(1U << 1)
#define MODE_PB0_0				(1U << 0)

#define ODR_PB0					(1U << 0)
#define ODR_PB7					(1U << 7)
#define ODR_PB14				(1U << 14)

/* POrt D */
#define MODE_PD0_1				(1U << 1)
#define MODE_PD0_0				(1U << 0)

#define MODE_PD1_1				(1U << 3)
#define MODE_PD1_0				(1U << 2)

#define AFRL1_3					(1U << 7)
#define AFRL1_2					(1U << 6)
#define AFRL1_1					(1U << 5)
#define AFRL1_0					(1U << 4)

#define AFRL0_3					(1U << 3)
#define AFRL0_2					(1U << 2)
#define AFRL0_1					(1U << 1)
#define AFRL0_0					(1U << 0)

void gpiob_init(void);


#endif /* GPIOB_H_ */
