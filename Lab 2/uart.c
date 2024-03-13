#include "uart.h"

void uart3_init(u32 bound)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Enable the gpio clock
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Enable the Usart clock
GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART2);
GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART2);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
GPIO_Init(GPIOC, &GPIO_InitStructure);
//UsartNVIC configuration
 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//Preempt priority
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
//Preempt priority
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//Enable the IRQ channel
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 //Initialize the VIC register with the specified parameters
NVIC_Init(&NVIC_InitStructure);
 //USART Initialization Settings
USART_InitStructure.USART_BaudRate = bound; //Baud rate
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //The word length is 8 bit data
USART_InitStructure.USART_StopBits = USART_StopBits_1; //A stop bit
USART_InitStructure.USART_Parity = USART_Parity_No; //No parity bits
//No hardware data flow control
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//Sending and receiving mode
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART3, &USART_InitStructure); //Initialize serial port 3
 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //Open the serial port to accept interrupts
 USART_Cmd(USART3, ENABLE); //Enable serial port 3
}

void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);
}

const u8* p;

int USART3_IRQHandler(void)
{
	static u8 Count=0,i;
	u8 Usart_Receive;
	static char s[10] = {'\0'};
	
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		// USART_Receive is the received data
		Usart_Receive = USART_ReceiveData(USART3);
		 
		// show on oled
		if (Count == 10)
			Count = 0;
		s[Count] = Usart_Receive;
		OLED_ShowString(0, 0, s);
		Count++;
		if (Usart_Receive <= '9' && Usart_Receive >= '0'){
			i = Usart_Receive - '0';
			xTimerChangePeriod( xAutoTimer, i*1000, 1000);
			xTimerStart(xAutoTimer, 0);
		}
		
		OLED_Refresh_Gram();

	}
	return 0;
}

void uart_task(void *pvParameters)
{
	u8 data[17] = " Ang Yi Xuan ";
	u8 i = 0;
	while (1) {
		for (i = 0; i < 17; i++){
			usart3_send(data[i]);
		}
		delay_ms(1000);
	}
} 