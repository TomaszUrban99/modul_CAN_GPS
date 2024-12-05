/*
 * USART_registers.h
 *
 *  Created on: Nov 2, 2024
 *      Author: tomasz
 */

#ifndef INC_PERIPHERALS_USART_REGISTERS_H_
#define INC_PERIPHERALS_USART_REGISTERS_H_

/********* Enable Clock Access ***************/

#define RCC_USART2_EN				(1U << 17)
#define RCC_USART3_EN				(1U << 18)

#define RCC_DMA1_EN					(1U << 21)

/***************** GPIO **********************/
/************** PIN CONFIGURATION ************/

/**************** CLOCK ACCESS ***************/
#define RCC_GPIOB_EN			(1U << 1)
#define RCC_GPIOD_EN			(1U << 3)

/***************** USART 2 *******************/

#define MODE_PD6_1					(1U << 13)
#define MODE_PD6_0					(1U << 12)

#define MODE_PD5_1					(1U << 11)
#define MODE_PD5_0					(1U << 10)

/***************** USART 3 *******************/

/* USART3 RX */
#define MODE_PB11_1					(1U << 23)
#define MODE_PB11_0					(1U << 22)

/* USART3 TX */
#define MODE_PB10_1					(1U << 21)
#define MODE_PB10_0					(1U << 20)

/*************** GPIOx_AFRL ******************/

/********** PD6 ******************************/
#define AFRL6_3						(1U << 27)
#define AFRL6_2						(1U << 26)
#define AFRL6_1						(1U << 25)
#define AFRL6_0						(1U << 24)

/********** PD5 ******************************/
#define AFRL5_3						(1U << 23)
#define AFRL5_2						(1U << 22)
#define AFRL5_1						(1U << 21)
#define AFRL5_0						(1U << 20)

/********** PB11 ******************************/
#define AFRH11_3						(1U << 15)
#define AFRH11_2						(1U << 14)
#define AFRH11_1						(1U << 13)
#define AFRH11_0						(1U << 12)

/********** PB10 ******************************/
#define AFRH10_3						(1U << 11)
#define AFRH10_2						(1U << 10)
#define AFRH10_1						(1U << 9)
#define AFRH10_0						(1U << 8)

/*************** USART_SR ********************/

#define SR_CTS						(1U << 9)
#define SR_LBD						(1U << 8)
#define SR_TXE						(1U << 7) /* Transmit data energy empty */
#define SR_TC						(1U << 6) /* Transmission complete */
#define SR_RXNE						(1U << 5) /* Read DR not empty */
#define SR_IDLE						(1U << 4)
#define SR_ORE						(1U << 3)
#define SR_NF						(1U << 2)
#define SR_FE						(1U << 1)
#define SR_PE						(1U << 0)

/*************** USART_CR ********************/

/***************** CR1 ***********************/

#define CR1_OVER8					(1U << 15)
#define CR1_UE						(1U << 13) /* USART enable */
#define CR1_M						(1U << 12) /* Word length  */
#define CR1_WAKE					(1U << 11)
#define CR1_PCE						(1U << 10)
#define CR1_PS						(1U << 9) /* Parity selection */
#define CR1_PEIE					(1U << 8) /* PE Interrupt enable */
#define CR1_TXEIE					(1U << 7) /* TXE (tranm interrupt enable */
#define CR1_TCIE					(1U << 6) /* Transmission complete interrupt enable */
#define CR1_RXNEIE					(1U << 5) /* RXNE interrupt enable */
#define CR1_IDLEIE					(1U << 4) /* IDLE interrupt enable */
#define CR1_TE						(1U << 3) /* Transmitter enable */
#define CR1_RE						(1U << 2) /* Receiver enable */
#define CR1_RWU						(1U << 1) /* Receiver wake-up */
#define CR1_SBK						(1U << 0) /* Send break */

/***************** CR2 ***********************/

#define CR2_LINEN					(1U << 14)
#define CR2_STOP_1					(1U << 13)
#define CR2_STOP_0					(1U << 12)
#define CR2_CLKEN					(1U << 11)
#define CR2_CPOL					(1U << 10)
#define CR2_CPHA					(1U << 9)
#define CR2_LBCL					(1U << 8)
#define CR2_LBDIE					(1U << 6) /* Lin break detection */
#define CR2_LBDL					(1U << 5) /* Lin break detection length */
#define CR2_ADD_3					(1U << 3)
#define CR2_ADD_2					(1U << 2)
#define CR2_ADD_1					(1U << 1)
#define CR2_ADD_0					(1U << 0)

/***************** CR3 ************************/

#define CR3_ONEBIT					(1U << 11)
#define CR3_CTSIE					(1U << 10)
#define CR3_CTSE					(1U << 9)
#define CR3_RTSE					(1U << 8) /* RTS enable */
#define CR3_DMAT					(1U << 7) /* Enable DMA TX */
#define CR3_DMAR					(1U << 6) /* Enable DMA RX */
#define CR3_SCEN					(1U << 5) /* Smartcard mode enable */
#define CR3_NACK					(1U << 4) /* Smartcard NACK enable */
#define CR3_HDSEL					(1U << 3) /* Half-duplex selection */
#define CR3_IRLP					(1U << 2) /* IrDA low-power */
#define CR3_IREN					(1U << 1) /* IrDA mode enable */
#define CR3_EIE						(1U << 0) /* Error interrupt enable */

/************* DMA REGISTERS *****************/

/***************** LIFCR *********************/

/* Stream 0 UART5 RX */
#define LIFCR_CTCIF0				(1U << 5)
#define LIFCR_CHTIF0				(1U << 4)
#define LIFCR_CTEIF0				(1U << 3)
#define LIFCR_CDMEIF0				(1U << 2)
#define LIFCR_CFEIF0				(1U << 0)

/* Stream 1 USART3 RX */
#define LIFCR_CTCIF1				(1U << 11)
#define LIFCR_CHTIF1				(1U << 10)
#define LIFCR_CTEIF1				(1U << 9)
#define LIFCR_CDMEIF1				(1U << 8)
#define LIFCR_CFEIF1				(1U << 6)

/* Stream 3 USART3 TX */
#define LIFCR_CTCIF3				(1U << 27)
#define LIFCR_CHTIF3				(1U << 26)
#define LIFCR_CTEIF3				(1U << 25)
#define LIFCR_CDMEIF3				(1U << 24)
#define LIFCR_CFEIF3				(1U << 22)

/***************** LIFSR **********************/

/* Stream 0 UART5 RX */
#define LIFSR_CTCIF0				(1U << 5)
#define LIFSR_CHTIF0				(1U << 4)
#define LIFSR_CTEIF0				(1U << 3)
#define LIFSR_CDMEIF0				(1U << 2)
#define LIFSR_CFEIF0				(1U << 0)


/* Stream 1 USART3 RX */
#define LIFSR_CTCIF1				(1U << 11)
#define LIFSR_CHTIF1				(1U << 10)
#define LIFSR_CTEIF1				(1U << 9)
#define LIFSR_CDMEIF1				(1U << 8)
#define LIFSR_CFEIF1				(1U << 6)

/* Stream 3 USART3 TX */
#define LIFSR_CTCIF3				(1U << 27)
#define LIFSR_CHTIF3				(1U << 26)
#define LIFSR_CTEIF3				(1U << 25)
#define LIFSR_CDMEIF3				(1U << 24)
#define LIFSR_CFEIF3				(1U << 22)

/***************** HIFCR **********************/

/* Stream 5 USART2 RX */
#define HIFCR_CTCIF5				(1U << 11)
#define HIFCR_CHTIF5				(1U << 10)
#define HIFCR_CTEIF5				(1U << 9)
#define HIFCR_CDMEIF5				(1U << 8)
#define HIFCR_CFEIF5				(1U << 6)

/* Stream 6 USART2 TX */
#define HIFCR_CTCIF6				(1U << 21)
#define HIFCR_CHTIF6				(1U << 20)
#define HIFCR_CTEIF6				(1U << 19)
#define HIFCR_CDMEIF6				(1U << 18)
#define HIFCR_CFEIF6				(1U << 16)

/* Stream 7 UART5 TX */
#define HIFCR_CTCIF7				(1U << 27)
#define HIFCR_CHTIF7				(1U << 26)
#define HIFCR_CTEIF7				(1U << 25)
#define HIFCR_CDMEIF7				(1U << 24)
#define HIFCR_CFEIF7				(1U << 22)

/****************** HIFSR ***********************/

/* Stream 5 USART2 RX */
#define HIFSR_CTCIF5				(1U << 11)
#define HIFSR_CHTIF5				(1U << 10)
#define HIFSR_CTEIF5				(1U << 9)
#define HIFSR_CDMEIF5				(1U << 8)
#define HIFSR_CFEIF5				(1U << 6)

/* Stream 6 USART2 TX */
#define HIFSR_CTCIF6				(1U << 21)
#define HIFSR_CHTIF6				(1U << 20)
#define HIFSR_CTEIF6				(1U << 19)
#define HIFSR_CDMEIF6				(1U << 18)
#define HIFSR_CFEIF6				(1U << 16)

/* Stream 7 UART5 TX */
#define HIFSR_CTCIF7				(1U << 27)
#define HIFSR_CHTIF7				(1U << 26)
#define HIFSR_CTEIF7				(1U << 25)
#define HIFSR_CDMEIF7				(1U << 24)
#define HIFSR_CFEIF7				(1U << 22)

/******************** DMA_SxCR ********************/

/* Channel select */
#define DMA_SXCR_CHSEL_2			(1U << 27)
#define DMA_SXCR_CHSEL_1			(1U << 26)
#define DMA_SXCR_CHSEL_0			(1U << 25)

/* MBURST */
#define DMA_SXCR_MBURST_1			(1U << 24)
#define DMA_SXCR_MBURST_0			(1U << 23)

#define DMA_SXCR_PBURST_1			(1U << 22)
#define DMA_SXCR_PBURST_0			(1U << 21)

#define DMA_SXCR_CT					(1U << 19)
#define DMA_SXCR_DBM				(1U << 18)
#define DMA_SXCR_PL_1				(1U << 17)
#define DMA_SXCR_PL_0				(1U << 16)
#define DMA_SXCR_PINCOS				(1U << 15)

#define DMA_SXCR_MSIZE_1			(1U << 14)
#define DMA_SXCR_MSIZE_0			(1U << 13)

#define DMA_SXCR_PSIZE_1			(1U << 12)
#define DMA_SXCR_PSIZE_0			(1U << 11)

#define DMA_SXCR_MINC				(1U << 10)
#define DMA_SXCR_PINC				(1U << 9)
#define DMA_SXCR_CRC				(1U << 8) /* Circular mode */

/* Transfer direction */
#define DMA_SXCR_DIR_1				(1U << 7)
#define DMA_SXCR_DIR_0				(1U << 6)

#define DMA_SXCR_PFCTRL				(1U << 5)
#define DMA_SXCR_TCIE				(1U << 4)
#define DMA_SXCR_HTIE				(1U << 3)
#define DMA_SXCR_TEIE				(1U << 2)
#define DMA_SXCR_DMEIE				(1U << 1)
#define DMA_SXCR_EN					(1U << 0)

#endif /* INC_PERIPHERALS_USART_REGISTERS_H_ */
