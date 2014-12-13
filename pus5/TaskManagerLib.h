/*
* TaskManagerLib.h
*
* Created: 2014-11-12 17:55:47
*  Author: Michał
*/
#ifndef TASKMANAGERLIB_H_
#define TASKMANAGERLIB_H_
#define MAX_NUMBER_OF_TASKS 8

typedef enum { false = 0, true = 1 } bool;
typedef void (*TaskPointer)(void* params);

typedef struct {
	int period;
	TaskPointer func_ptr;
	void* params;
	int current;
	bool ready;
} Task;

void schedule();
void execute();
void AddTask(int priority, int period, TaskPointer func_ptr, void* params);




#endif /* TASKMANAGERLIB_H_ */