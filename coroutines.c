#include <stdlib.h>
#include <stdio.h>

#include "scheduler.h"

struct tester_args {
    char *name;
    int iters;
};

struct mytask {
   enum {
      AST_CREATED,
      AST_RUNNING,
      AST_WAITING,
   } astatus;

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
void tester(void *arg)
{
    int i;
    struct tester_args *ta = (struct tester_args *)arg;
    for (i = 0; i < ta->iters; i++) {
        printf("task %s: %d\n", ta->name, i);
        scheduler_relinquish();
    }
    free(arg);
}

void create_test_task(char *name, int iters)
{
    struct tester_args *ta = malloc(sizeof(struct tester_args));
    ta->name = name;
    ta->iters = iters;
    scheduler_create_task(tester, ta);
}

int main(int argc, char **argv)
{
    scheduler_init();
    create_test_task("first", 10);
    create_test_task("second", 10);
    scheduler_run();
    printf("\nFinished running all tasks!\n");
    return EXIT_SUCCESS;
}

