#include "system.h"
#include "led.h"
#include "buzzer.h"
#include "show.h"
#include "oled.h"
#include "button.h"
#include "uart.h"

#define START_TASK_PRIO     4
#define START_STK_SIZE      256
TaskHandle_t StartTask_Handler;
TimerHandle_t xAutoTimer;
void start_task(void* pvParameters);
void start_oled_task(void* pvParameters);

int main(void){
    systemInit();
    Buzzer_Init();
		//BUZZER = 0;
		OLED_Init();
	  Button_Init(); // Exercise A
	  uart3_init(115200); // Exercise C

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
		
	 xTaskCreate((TaskFunction_t ) button_task,
    (const char*    ) "button_task",    
			(uint16_t       ) START_STK_SIZE,
    (void*          ) NULL,    
			(UBaseType_t    ) START_TASK_PRIO,
    (TaskHandle_t*  ) &StartTask_Handler);  
    vTaskStartScheduler();
		
		xTaskCreate((TaskFunction_t ) uart_task,
    (const char*    ) "uart_task",    
			(uint16_t       ) START_STK_SIZE,
    (void*          ) NULL,    
			(UBaseType_t    ) START_TASK_PRIO,
    (TaskHandle_t*  ) &StartTask_Handler);  
    vTaskStartScheduler();
}

void start_task(void* pvParameters)
{    
	taskENTER_CRITICAL();
  //xTaskCreate(led_task, "led_task", LED_STK_SIZE, NULL, LED_TASK_PRIO, NULL);
  //xTaskCreate(buzzer_task, "buzzer_task", BUZZER_STK_SIZE, NULL, BUZZER_TASK_PRIO, NULL);    
	//xTaskCreate(show_task, "show_task", SHOW_STK_SIZE, NULL, SHOW_TASK_PRIO, NULL); 
	xTaskCreate(button_task, "button_task", SHOW_STK_SIZE, NULL, SHOW_TASK_PRIO, NULL); // Exercise A
  xTaskCreate(uart_task, "uart_task", SHOW_STK_SIZE, NULL, SHOW_TASK_PRIO, NULL); 	// Exercise C
  vTaskDelete(StartTask_Handler);    
  taskEXIT_CRITICAL();
}