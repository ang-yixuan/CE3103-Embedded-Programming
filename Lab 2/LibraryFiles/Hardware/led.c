#include "led.h"

// Practice B
TimerHandle_t ledTimer;

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LED_PORT, LED_PIN);
	
	// Practice B
	ledTimer = xTimerCreate(
		"BlinkLED1Sec", 			// name
		pdMS_TO_TICKS(1000),	// period
		pdTRUE,								// auto reload
		(void*) 0,						// timer ID
		led_task							// callback
	);
	xTimerStart(ledTimer, pdMS_TO_TICKS(0));
}

void led_task(void *pvParameters) 
{
	//int delay_amount = 5000; //ms
	
	//while (1) {
		LED=~LED;
		//delay_ms(delay_amount);
	//}
}

void changePeriod(int newPeriod)
{
	xTimerChangePeriod(ledTimer, pdMS_TO_TICKS(newPeriod), pdMS_TO_TICKS(0));
}