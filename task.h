#ifndef _TASK_H
#define _TASK_H
#include<setjmp.h>
#include "sc-list.h"

struct task {
   enum {
      ST_CREATED,
      ST_RUNNING,
      ST_WAITING, 
   } status;

   int id;

   //resuming to the task is bascially doing a `longjmp` to this buf
   jmp_buf buf;

   void (*func)(void*);
   void *arg;

   struct sc_list_head task_list;

   void* stack_bottom;
   void* stack_top;
   int stack_size;
};
#endif
