#include<stdlib.h>
#include "scheduler.h"
#include "task.h"
#include<stdio.h>

#define STACK_SIZE 16384
void scheduler_create_task(void (*func)(void*), void* arg)
{
   static int id = 1;
   struct task* task = malloc(sizeof(struct task));
   task->status = ST_CREATED;
   task->func = func;
   task->arg = arg;
   task->id = id++;
   task->stack_size = STACK_SIZE;
   task->stack_bottom = malloc(STACK_SIZE * sizeof(char));
   task->stack_top = task->stack_bottom + task->stack_size;
   sc_list_insert_end(&priv.task_list, &task->task_list);
   
}

void scheduler_init(void)
{
  priv.current = NULL;
  sc_list_init(&priv.task_list);
}

enum {
   INIT = 0,
   SCHEDULE,
   EXIT_TASK
};

void scheduler_free_current_task() {
   struct task* task = priv.current;
   priv.current = NULL;

   free(task->stack_bottom);
   free(task);

}

void scheduler_run(void) {
  //first setjmp returns 0, so try to init or schedule a new task
  //when task jumps to scheduler with EXIT_TASK as the longjmp code,
  //clean it up
  switch((setjmp(priv.buf))) {
    case EXIT_TASK:
      scheduler_free_current_task();
    //let both fall through to the same "action"
    case INIT:
    case SCHEDULE:
      schedule();
      return;
    default:
      fprintf(stderr, "scheduler err");


  }
}

static struct task* schedule_choose_next_task() {
  struct task* task;
  
  sc_list_for_each_entry(task, &priv.task_list, task_list, struct task)
  {
        if(task->status == ST_RUNNING || task->status == ST_CREATED) {
            sc_list_remove(&task->task_list);
            sc_list_insert_end(&priv.task_list, &task->task_list);
            return task; 
        } 
  } 

  return NULL;
}

static void schedule (void) {
  struct task *next = schedule_choose_next_task();

  if (!next)
    return;
  
  priv.current = next;

  //This is the **key** behind coroutines to have stacks for tasks that are actually
  //allocated on the heap and managed by the scheduler


  //create_task sets status to created
  //we need to switch the stack pointer to a manually managed stack
  //so that the stack for the task can be switched to
  if(next->status == ST_CREATED) {

    register void* top = next->stack_top;
     asm volatile(
      "mov %[rs], %%rsp \n"
      ::[ rs ] "r" (top):);
   
    //The above mentioned inline asm uses GCC's extended asm syntax.
    //Check out this awesome document on how it works
    //https://www.felixcloutier.com/documents/gcc-asm.html
    next->status = ST_RUNNING;
    next->func(next->arg);    

    scheduler_exit_current_task();
  } else {
    longjmp(next->buf, 1); 

  }

}



void scheduler_relinquish(void) {
    if(setjmp(priv.current->buf))
            return;
    else
        longjmp(priv.buf, SCHEDULE);
}

void scheduler_exit_current_task() {

  struct task* task = priv.current;
  sc_list_remove(&task->task_list);
  longjmp(priv.buf, EXIT_TASK);

}


