#include "task.h"
void scheduler_init(void);

//A task is a function that takes a single pointer argument
void scheduler_create_task(void (*func)(void*), void* arg);

void scheduler_run(void);
static void schedule(void);

void scheduler_exit_current_task(void);
void scheduler_relinquish(void);

struct scheduler_private {
 jmp_buf buf;
 struct task* current;
 struct sc_list_head task_list;
} priv;


