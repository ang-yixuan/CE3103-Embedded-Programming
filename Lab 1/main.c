#include "system.h"
#include "led.h"
#include "buzzer.h"
#include "show.h"
#include "oled.h"

#define START_TASK_PRIO     4
#define START_STK_SIZE      256
TaskHandle_t StartTask_Handler;
void start_task(void* pvParameters);
void start_oled_task(void* pvParameters);

int main(void){
    systemInit();
    Buzzer_Init(); 
		OLED_Init();
    xTaskCreate((TaskFunction_t ) start_task,                
								(const char*    ) "start_task",
                (uint16_t       ) START_STK_SIZE,                
								(void*          ) NULL,
                (UBaseType_t    ) START_TASK_PRIO,                
								(TaskHandle_t*  ) &StartTask_Handler);
  vTaskStartScheduler();
  
  xTaskCreate((TaskFunction_t ) buzzer_task,
    (const char*    ) "buzzer_task",    
			(uint16_t       ) START_STK_SIZE,
    (void*          ) NULL,    
			(UBaseType_t    ) START_TASK_PRIO,
    (TaskHandle_t*  ) &StartTask_Handler);  
		
    vTaskStartScheduler();
  
	 xTaskCreate((TaskFunction_t ) show_task,    
		(const char*    ) "show_task",
			(uint16_t       ) START_STK_SIZE,    
		(void*          ) NULL,
			(UBaseType_t    ) START_TASK_PRIO,    
		(TaskHandle_t*  ) &StartTask_Handler);
			vTaskStartScheduler();
	 
}

void start_task(void* pvParameters)
{    
	taskENTER_CRITICAL();
  xTaskCreate(led_task, "led_task", LED_STK_SIZE, NULL, LED_TASK_PRIO, NULL);
  xTaskCreate(buzzer_task, "buzzer_task", BUZZER_STK_SIZE, NULL, BUZZER_TASK_PRIO, NULL);    
	xTaskCreate(show_task, "show_task", SHOW_STK_SIZE, NULL, SHOW_TASK_PRIO, NULL);   
    vTaskDelete(StartTask_Handler);    
    taskEXIT_CRITICAL();
}