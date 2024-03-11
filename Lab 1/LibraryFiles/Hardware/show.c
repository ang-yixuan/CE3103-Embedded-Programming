#include "show.h"
uint8_t seconds = 0;uint8_t bufferA[10];
uint8_t mins=0;uint8_t bufferB[10];

void show_task(void *pvParameters){
   u32 lastWakeTime = getSysTickCnt();   while(1)
    { 
   vTaskDelayUntil(&lastWakeTime, F2T(RATE_50_HZ));   seconds++;
   vTaskDelay(1000);   if(seconds==60)
   {    mins++;
    seconds=0;    OLED_Clear();
   }   
   oled_show();        }
}  
void oled_show(void){  
     //To DO  
	unsigned char name[] = "Ang Yi Xuan";
  OLED_Display_On();  OLED_ShowString(0,0,name);
  sprintf(bufferA,"%d",seconds);
 sprintf(bufferB,"%d",mins);  OLED_ShowString(20,40,bufferA);
  OLED_ShowString(20,20,bufferB);  //call some function in oled to display something
  OLED_Refresh_Gram(); //refresh the OLED RAM
       
 }