#include "Comm485_2.h"
#include "cmsis_os.h"
#include "usart.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t Comm485_2_Rx_Buff[1024];
uint8_t Comm485_2_Tx_Buff[1024];


void Comm485_2_Task(void)
{
    for(int i = 0; i < 1024; i++)
    {
        Comm485_2_Tx_Buff[i] = i;
    }


    




    while(1)
    {
        osDelay(1000);
        // printf("__FUNC__: %s\r\n", __func__);
        // HAL_UART_Transmit(&huart1, Comm485_2_Tx_Buff, 1024, HAL_MAX_DELAY);
        UART_Tx_And_Rx(Comm485_2_Tx_Buff, 20, Comm485_2_Rx_Buff, 1024);


        //  dcache_clean(Comm485_2_Tx_Buff, 100);
        //  HAL_UART_Transmit_DMA(&huart1, Comm485_2_Tx_Buff, 500);
        // HAL_UART_Transmit_IT(&huart1, Comm485_2_Tx_Buff, 100);

    }
}

