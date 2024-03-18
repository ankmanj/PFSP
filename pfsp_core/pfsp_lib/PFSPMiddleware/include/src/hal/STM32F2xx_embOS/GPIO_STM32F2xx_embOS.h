#ifndef __GPIO_STM32F2XX_EMBOS_H__
#include "stm32f2xx_gpio.h"
#endif

#ifndef __GPIO_STM32F2XX_EMBOS_H__
#define __GPIO_STM32F2XX_EMBOS_H__

#ifdef __cplusplus
extern "C" {
#endif

void GPIO_Enable(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

void GPIO_Disable(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif //__GPIO_STM32F2XX_EMBOS_H__
