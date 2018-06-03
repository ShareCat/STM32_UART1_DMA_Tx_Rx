

#include "bsp_usart1.h"

uint8_t SendBuff[SENDBUFF_SIZE];

/**
  * @brief  USART1 GPIO conf. 115200 8-N-1
  * @param  null
  * @retval null
  */
void USARTx_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config USART1 clock */
    macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
    macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  macUSART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(macUSARTx, &USART_InitStructure);

    USART_Cmd(macUSARTx, ENABLE);
}


/* UART DMA RX buffer len */
#define UART_RX_LEN     128
/* UART DMA RX buffer */
uint8_t Uart_Rx[UART_RX_LEN] = {0};

/**
  * @brief  USARTx TX DMA config
  * @param  null
  * @retval null
  */
void USARTx_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 
        part1: UART DMA TX config
    */
    /* enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* config DMA src */
    //DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(& (macUSARTx->DR));

    /* config RAM addr. */
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

    /* config direction */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

    /* config DMA_BufferSize */
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* DMA mode */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    /* DMA_Priority */
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

    /* DMA_M2M disable */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    /* DMA1 channel4 */
    DMA_Init(macUSART_TX_DMA_CHANNEL, &DMA_InitStructure);

    /* enable DMA */
    DMA_Cmd(macUSART_TX_DMA_CHANNEL, ENABLE);

    /* DMA interrupt when tx done */
    //DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);



    /* 
        part2: UART DMA TX config
    */
    /* enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* DMA1 channel6 */
    DMA_DeInit(DMA1_Channel5);
    /* peri. addr */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
    /* RAM addr */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart_Rx;
    /* DMA dir */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    /* DMA RX buffer len */
    DMA_InitStructure.DMA_BufferSize = UART_RX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* config DMA mode */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /* DMA_Priority */
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    /* enable DMA channel5 */
    DMA_Cmd(DMA1_Channel5, ENABLE);


    /* 
        part3: UART interrupts
    */
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


int fputc(int ch, FILE *f)
{
    USART_SendData(macUSARTx, (uint8_t) ch);
    while(USART_GetFlagStatus(macUSARTx, USART_FLAG_TXE) == RESET);

    return (ch);
}

int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(macUSARTx, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(macUSARTx);
}
/*********************************************END OF FILE**********************/
