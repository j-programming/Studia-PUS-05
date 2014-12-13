/*
 * TaskManagerLib.c
 *
 * Created: 2014-11-19 13:22:08
 *  Author: Michał
 */
#include "TaskManagerLib.h"

static Task tasks[MAX_NUMBER_OF_TASKS];

void schedule(){
	for(int i = 0; i<MAX_NUMBER_OF_TASKS; i++){
		tasks[i].current++;
		if(tasks[i].current == tasks[i].period){
			tasks[i].ready = true;
			tasks[i].current = 0;
		}
	}
};

void execute(){
	int position = 0;
	while(true){
		if(tasks[position].ready == true){
			tasks[position].ready = false;
			tasks[position].func_ptr(tasks[position].params);
		}
		position++;
		if(position>MAX_NUMBER_OF_TASKS){
			position = 0;
		}
	}
};

void AddTask(int priority, int period, TaskPointer func_ptr, void* params){
	tasks[priority].period = period;
	tasks[priority].current = 0;
	tasks[priority].ready = false;
	tasks[priority].func_ptr = func_ptr;
	tasks[priority].params = params;
};
