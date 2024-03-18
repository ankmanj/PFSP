#ifndef __INTERRUPT_STM32F2XX_EMBOS_H__
#define __INTERRUPT_STM32F2XX_EMBOS_H__

/*
* Include Files
*/

/*
* Public Definition
*/

typedef enum {
	eISR_TIMER0 = 0,
	eISR_TIMER1,
	eISR_TIMER2,
	eISR_UART0_TX,
	eISR_UART0_RX,
	eISR_UART1_TX,
	eISR_UART1_RX,
	eISR_UART2_TX,
	eISR_UART2_RX,
    eISR_UART3_TX,
    eISR_UART3_RX,
//	eISR_SPI0_TX,
//	eISR_SPI0_RX,
//	eISR_SPI1_TX,
//	eISR_SPI1_RX,
    eISR_EXTI0,
    eISR_EXTI1,
    eISR_EXTI2,
    eISR_EXTI3,
    eISR_ADC1,
    eISR_ADC2,
    eISR_ADC3,
//  eISR_DMA1_2,    // Gyro (UART4 DMA)
    eISR_RTC_Alarm,
	eISR_ALL,
    eISR_NONE = -1,
} eInterrupt_t;

#ifdef __cplusplus
extern "C" {
#endif
	
/*
* Public Member Functions
*/

int16_t IRQ_Register(const eInterrupt_t nInterruptId, void (*pfnIsr)(void* pvArgs), void* pvArgs, void (*pfnInit)(FunctionalState, void*));
int16_t IRQ_Enable(const eInterrupt_t nInterruptId, void* pOption);
int16_t IRQ_Disable(const eInterrupt_t nInterruptId, void* pOption);

#ifdef __cplusplus
}
#endif
#endif //__INTERRUPT_STM32F2XX_EMBOS_H__
