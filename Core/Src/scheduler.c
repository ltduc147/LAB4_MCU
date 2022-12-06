/*
 * scheduler.c
 *
 *  Created on: Nov 29, 2022
 *      Author: Admin
 */


#include "scheduler.h"

#define CYCLE 10
sTasks SCH_tasks_G[SCH_MAX_TASKS];

void SCH_Init(void){
	unsigned char i;
	for (i = 0; i < SCH_MAX_TASKS; i++){
		SCH_Delete_Task(i);
	}

}

void SCH_Add_Task(void (*pFuntion)(), uint32_t DELAY, uint32_t PERIOD){
	unsigned char index = 0;

	while ((SCH_tasks_G[index].pTask != 0) && (index < SCH_MAX_TASKS)){
		index++;
	}

	SCH_tasks_G[index].pTask = pFuntion;
	SCH_tasks_G[index].Delay = DELAY / CYCLE;
	SCH_tasks_G[index].Period = PERIOD / CYCLE;
	SCH_tasks_G[index].RunMe = 0;

}

void SCH_Update(void){
	unsigned char index;

	for (index = 0 ; index < SCH_MAX_TASKS; index ++){
		if (SCH_tasks_G[index].pTask){
			if (SCH_tasks_G[index].Delay == 0){
				SCH_tasks_G[index].RunMe += 1;
				if (SCH_tasks_G[index].Period){
					SCH_tasks_G[index].Delay = SCH_tasks_G[index].Period;
				}
			} else {
				SCH_tasks_G[index].Delay --;
			}
		}
	}
}

void SCH_Dispatch_Tasks(void){
	unsigned char index;
	for (index = 0; index < SCH_MAX_TASKS; index++){
		if (SCH_tasks_G[index].RunMe > 0){
			(*SCH_tasks_G[index].pTask)();
			SCH_tasks_G[index].RunMe--;

			if (SCH_tasks_G[index].Period == 0){
				SCH_Delete_Task(index);
			}

		}
	}

}

void SCH_Delete_Task(uint32_t TaskID){

	if (SCH_tasks_G[TaskID].pTask != 0){
		SCH_tasks_G[TaskID].pTask = 0x0000;
		SCH_tasks_G[TaskID].Delay = 0;
		SCH_tasks_G[TaskID].Period = 0;
		SCH_tasks_G[TaskID].RunMe = 0;
	}
}
