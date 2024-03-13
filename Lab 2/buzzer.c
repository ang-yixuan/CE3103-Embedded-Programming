#include "buzzer.h"

void Buzzer_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //Enable APB Clock
	
  GPIO_InitStructure.GPIO_Pin =  BUZZER_PIN;                  //Buzzer Pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //Push pull output
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);                    //Initialize BUZZER GPIO
	GPIO_SetBits(BUZZER_PORT,BUZZER_PIN);
}

void buzzer_task(void *pvParameters)
{
	int delay = 7000;
    while(1)
    {
      BUZZER=~BUZZER;        //0 on, 1 off   		
			delay_ms(delay);
      //vTaskDelay(Buzzer_Count); //Delay 
    }
}