all: sc-list.c coroutines.c scheduler.c
	gcc coroutines.c scheduler.c sc-list.c -o coroutines
clean:
	rm coroutines
