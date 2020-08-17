#include<stdlib.h>
#include "scheduler.h"

void scheduler_create_task(void (*func)(void*), void* arg)
{
   static int id = 1;
   struct task* task = malloc(sizeof(task));
   task->status = ST_CREATED;
   task->func = func;
   task->arg = arg;
   task->id = id++;
   task->stack_size = 16 * 1024;
   task->stack_bottom = malloc(task->stack_size);
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

void scheduler_run(void) {
  switch((setjmp(priv.buf))) {
    case EXIT_TASK:
      scheduler_free_current_task();
    //let both fall through to the same "action"
    case INIT:
    case SCHEDULE:
      schedule();
      return;
    default:
      fprintf(stderr, "scheduler err")


  }
}

static schedule void() {
  struct task *next = schedule_choose_task();

  if (!next)
          return;
  
  priv.current = next;

  if(next->stats == ST_CREATED) {
          register void* top = next->stack_top;
          asm volatile(
            "mov %[rs], %%[rsp] \n"
           : [ rs ] "+r" (top) ::

          );
  }

}
