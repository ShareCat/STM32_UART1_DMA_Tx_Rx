

// DMA  data memory access

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
static void Delay(__IO u32 nCount);

/**
  * @brief  main func.
  * @param  null
  * @retval null
  */
int main(void)
{
    /* USART1 config 115200 8-N-1 */
    USARTx_Config();

    USARTx_DMA_Config();

    LED_GPIO_Config();

    printf("\r\n usart1 DMA Tx&Rx test \r\n");

    {
        uint16_t i;

        /* fill buffers before send */
        for(i = 0; i < SENDBUFF_SIZE; i++) {
            SendBuff[i]  = 'A';
        }
    }

    /* USART1 DMA TX request */
    USART_DMACmd(macUSARTx, USART_DMAReq_Tx, ENABLE);

    /* using DMA RX */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

    /* UART DMA is sending, but CPU is free now */

    /* ctrl. the LED */
    for(;;) {
        LED1(ON);
        Delay(0xFFFFF);
        LED1(OFF);
        Delay(0xFFFFF);

        LED1(ON);
        Delay(0xFFFFF);
        LED1(OFF);
        Delay(0xFFFFF);
        LED1(ON);
        Delay(0xFFFFF);
        LED1(OFF);
        Delay(0xFFFFF);

        /* ---------------UART DMA resend------------------------- */
        DMA_Cmd(DMA1_Channel4, DISABLE);
        /* setting the TX buffer length */
        DMA_SetCurrDataCounter(DMA1_Channel4, 10);
        /* enable DMA */
        DMA_Cmd(DMA1_Channel4, ENABLE);
        /* ---------------UART DMA resend------------------------- */


    }
}

static void Delay(__IO uint32_t nCount)  /* delay func. */
{
    for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
