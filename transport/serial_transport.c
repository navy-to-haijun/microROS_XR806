# include"microros_xr806.h"

#if defined MICROROS_SERIAL

#define UARTID 				UART1_ID
#define micro_rollover_useconds 4294967295

int clock_gettime(clockid_t unused, struct timespec *tp)
{
    (void)unused;
    static uint32_t rollover = 0;
    static uint32_t last_measure = 0;

    uint32_t m = OS_GetTicks() * 1000;
    

    rollover += (m < last_measure) ? 1 : 0;

    uint64_t real_us = (uint64_t) (m + rollover * micro_rollover_useconds);

    tp->tv_sec = real_us / 1000000;
    tp->tv_nsec = (real_us % 1000000) * 1000;
    last_measure = m;
    return 0;
}

/*串口1初始化*/
static int uart_init(void)
{
	UART_InitParam param;

	param.baudRate = 115200;
	param.dataBits = UART_DATA_BITS_8;
	param.stopBits = UART_STOP_BITS_1;
	param.parity = UART_PARITY_NONE;
	param.isAutoHwFlowCtrl = 0;

	if(HAL_UART_Init(UARTID, &param) != HAL_OK)
		return -1;
	/*使能DMA*/
	if (HAL_UART_EnableTxDMA(UARTID) != HAL_OK)
		return -2;
	if (HAL_UART_EnableRxDMA(UARTID) != HAL_OK)
		return -3;
	
	return 0;
}

bool serial_transport_open(uxrCustomTransport* transport){
    HAL_Status status = HAL_ERROR;
    status = uart_init();
    if(status == HAL_OK){
        printf("open uart%d successful!\n", UARTID);
         return 1;
    }
    else{
        printf("open uart%d fail!\n", UARTID);
        return 0;
    }
        
}

bool serial_transport_close(uxrCustomTransport* transport){
    HAL_Status status = HAL_ERROR;
    HAL_UART_DisableTxDMA(UARTID);
    HAL_UART_DisableRxDMA(UARTID);
    status = HAL_UART_DeInit(UARTID);
    if(status == HAL_OK)
        return 1;
    else
        return 0;
}

size_t serial_transport_write(uxrCustomTransport* transport,const uint8_t* buffer,
                            size_t length,uint8_t* errcode){

    return  HAL_UART_Transmit_DMA(UARTID, (uint8_t *)buffer, length);
}

size_t serial_transport_read(uxrCustomTransport* transport,uint8_t* buffer,
                            size_t length,int timeout,uint8_t* errcode){
    
    return HAL_UART_Receive_DMA(UARTID, buffer, length, timeout); 
}

#endif

