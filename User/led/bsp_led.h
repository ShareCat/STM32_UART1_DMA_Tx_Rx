#ifndef __LED_H
#define __LED_H


#include "stm32f10x.h"



#define macLED1_GPIO_PORT       GPIOB
#define macLED1_GPIO_CLK        RCC_APB2Periph_GPIOB
#define macLED1_GPIO_PIN        GPIO_Pin_9



/** the macro definition to trigger the led on or off
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1


#define LED1(a) if (a)  \
                    GPIO_SetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN)




#define digitalHi(p,i)          {p->BSRR=i;}
#define digitalLo(p,i)          {p->BRR=i;}
#define digitalToggle(p,i)      {p->ODR ^=i;}



#define LED1_TOGGLE     digitalToggle(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_OFF        digitalHi(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_ON         digitalLo(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */

