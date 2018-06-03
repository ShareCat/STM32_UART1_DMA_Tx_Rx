

#include "bsp_led.h"

/**
 * @brief  config LED GPIO
 * @param  null
 * @retval null
 */
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(macLED1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(macLED1_GPIO_PORT, macLED1_GPIO_PIN);
}
/*********************************************END OF FILE**********************/



