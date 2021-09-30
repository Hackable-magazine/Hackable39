.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global g_pfnVectors

.section .text.Reset_Handler
.weak   Reset_Handler
.type   Reset_Handler, %function

Reset_Handler:
	ldr sp, =_estack	/* Set stack pointer */


// clock enable
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR RCC_BASE + 0x4c
#define GPIOBEN (1 << 1)

// port B
#define GPIOB_BASE 0x48000400
#define GPIOB_MODER GPIOB_BASE + 0
#define GPIOB_MODER3 0b01 << 6
#define GPIOB_MODER3_MASK !(0b11 << 6)

#define GPIOB_ODR GPIOB_BASE + 0x14
#define LED (1 << 3)

_start:
	bl gpio_init
mloop:
	ldr r0,=GPIOB_ODR
	ldr r1,[r0]
	orr r1,r1,#LED
	str r1,[r0]

	ldr r0,=0x10000
	push {r0}		// push instruction is a shorthand for stmdb -> arg = liste de registers -> donc {}
	bl wait

	ldr r0,=GPIOB_ODR
	ldr r1,[r0]
	bic r1,r1,#LED
	str r1,[r0]

	ldr r0,=0x38000
	push {r0}
	bl wait

	ldr r0,=GPIOB_ODR
	ldr r1,[r0]
	orr r1,r1,#LED
	str r1,[r0]

	ldr r0,=0x10000
	push {r0}
	bl wait

	ldr r0,=GPIOB_ODR
	ldr r1,[r0]
	bic r1,r1,#LED
	str r1,[r0]

	ldr r0,=0x200000
	push {r0}
	bl wait
	b mloop


wait:
	pop {r2}
loop:
	subs r2,r2,#1
	bne loop
	bx lr


gpio_init:
	// Conf clock
	ldr r0,=RCC_AHB2ENR  // RCC_AHB2ENR dans r0
	// LDR pseudo-instruction : https://developer.arm.com/documentation/dui0041/c/Assembler/ARM-pseudo-instructions/LDR-ARM-pseudo-instruction
	ldr r1,[r0]          // contenu de load r1 from address r0
	orr r1,r1,#GPIOBEN	// OR r1 with literal GPIOBEN (aka 1 << 1) -> idem orr r1,#GPIOBEN
	str r1,[r0]		// store r1 to address r0

	// PB3 direction
	ldr r0,=GPIOB_MODER
	ldr r1,[r0]
	and r1,r1,#GPIOB_MODER3_MASK
	orr r1,r1,#GPIOB_MODER3
	str r1,[r0]

	bx lr

Default_Handler:
Infinite_Loop:
	b Infinite_Loop


/******************
* Vector table
******************/
	.section .isr_vector,"a",%progbits
	.type g_pfnVectors, %object
	.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
	.word	_estack
	.word	Reset_Handler
	.word	Default_Handler // NMI_Handler
	.word	Default_Handler // HardFault_Handler
	.word	Default_Handler // MemManage_Handler
	.word	Default_Handler // BusFault_Handler
	.word	Default_Handler // UsageFault_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // SVC_Handler
	.word	Default_Handler // DebugMon_Handler
	.word	Default_Handler
	.word	Default_Handler // PendSV_Handler
	.word	Default_Handler // SysTick_Handler
	.word	Default_Handler // WWDG_IRQHandler
	.word	Default_Handler // PVD_PVM_IRQHandler
	.word	Default_Handler // TAMP_STAMP_IRQHandler
	.word	Default_Handler // RTC_WKUP_IRQHandler
	.word	Default_Handler // FLASH_IRQHandler
	.word	Default_Handler // RCC_IRQHandler
	.word	Default_Handler // EXTI0_IRQHandler
	.word	Default_Handler // EXTI1_IRQHandler
	.word	Default_Handler // EXTI2_IRQHandler
	.word	Default_Handler // EXTI3_IRQHandler
	.word	Default_Handler // EXTI4_IRQHandler
	.word	Default_Handler // DMA1_Channel1_IRQHandler
	.word	Default_Handler // DMA1_Channel2_IRQHandler
	.word	Default_Handler // DMA1_Channel3_IRQHandler
	.word	Default_Handler // DMA1_Channel4_IRQHandler
	.word	Default_Handler // DMA1_Channel5_IRQHandler
	.word	Default_Handler // DMA1_Channel6_IRQHandler
	.word	Default_Handler // DMA1_Channel7_IRQHandler
	.word	Default_Handler // ADC1_IRQHandler
	.word	Default_Handler // CAN1_TX_IRQHandler
	.word	Default_Handler // CAN1_RX0_IRQHandler
	.word	Default_Handler // CAN1_RX1_IRQHandler
	.word	Default_Handler // CAN1_SCE_IRQHandler
	.word	Default_Handler // EXTI9_5_IRQHandler
	.word	Default_Handler // TIM1_BRK_TIM15_IRQHandler
	.word	Default_Handler // TIM1_UP_TIM16_IRQHandler
	.word	Default_Handler // TIM1_TRG_COM_IRQHandler
	.word	Default_Handler // TIM1_CC_IRQHandler
	.word	Default_Handler // TIM2_IRQHandler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // I2C1_EV_IRQHandler
	.word	Default_Handler // I2C1_ER_IRQHandler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // SPI1_IRQHandler
	.word	Default_Handler
	.word	Default_Handler // USART1_IRQHandler
	.word	Default_Handler // USART2_IRQHandler
	.word	Default_Handler
	.word	Default_Handler // EXTI15_10_IRQHandler
	.word	Default_Handler // RTC_Alarm_IRQHandler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // SPI3_IRQHandler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // TIM6_DAC_IRQHandler
	.word	Default_Handler // TIM7_IRQHandler
	.word	Default_Handler // DMA2_Channel1_IRQHandler
	.word	Default_Handler // DMA2_Channel2_IRQHandler
	.word	Default_Handler // DMA2_Channel3_IRQHandler
	.word	Default_Handler // DMA2_Channel4_IRQHandler
	.word	Default_Handler // DMA2_Channel5_IRQHandler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // COMP_IRQHandler
	.word	Default_Handler // LPTIM1_IRQHandler
	.word	Default_Handler // LPTIM2_IRQHandler
	.word	Default_Handler // USB_IRQHandler
	.word	Default_Handler // DMA2_Channel6_IRQHandler
	.word	Default_Handler // DMA2_Channel7_IRQHandler
	.word	Default_Handler // LPUART1_IRQHandler
	.word	Default_Handler // QUADSPI_IRQHandler
	.word	Default_Handler // I2C3_EV_IRQHandler
	.word	Default_Handler // I2C3_ER_IRQHandler
	.word	Default_Handler // SAI1_IRQHandler
	.word	Default_Handler
	.word	Default_Handler // SWPMI1_IRQHandler
	.word	Default_Handler // TSC_IRQHandler
	.word	Default_Handler
	.word	Default_Handler
	.word	Default_Handler // RNG_IRQHandler
	.word	Default_Handler // FPU_IRQHandler
	.word	Default_Handler // CRS_IRQHandler
