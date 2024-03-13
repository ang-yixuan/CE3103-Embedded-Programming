#ifndef __BUTTON_H
#define __BUTTON_H
#include "sys.h"
#include "system.h"

//Buzzer Port definition
#define BUTTON_TASK_PRIO		2     
#define BUTTON_STK_SIZE 		128   

#define BUTTON_PORT GPIOD
#define BUTTON_PIN GPIO_Pin_8
#define BUTTON PDin(8)
/*----------------------------------*/

void Button_Init(void);
void button_task(void *pvParameters);

#endif