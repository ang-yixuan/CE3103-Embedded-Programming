#include "button.h"
#include "LED.h"
#include "playsong.h"
#include "show.h"

// Practice A

//Buzzer initialization
void Button_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //Enable APB Clock
	
	GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;				//Buzzer Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	// input - internal pull up
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);			//Initialize Buzzer GPIO
	GPIO_SetBits(BUTTON_PORT,BUTTON_PIN);
}

void button_task(void *pvParameters)
{
	u8* song = (uint8_t*)
	"e2,d2,e2,d2,e2,B2,d2,c2,A2_C2,E2,A2,B2_E2,G2,B2,c4_E2,e2,d2,e2,d2,e2,B2,d2,c2,A2_C2,E2,A2,B2_E2,c2,B2,A4";
	
	int period = 2000;
	
	while(1)
	{
		if(!(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_PIN))
		{
			//OLED_ShowString(0, 0, p1);
			playSong(song);
			
			period = (period == 500) ? 2000 : 500;
			changePeriod(period);
		}
		else
		{
			//OLED_ShowString(0, 0, p2);	
		}
		
		//OLED_Refresh_Gram(); //refresh the OLED RAM
	}
}  