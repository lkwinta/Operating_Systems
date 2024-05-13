#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>

#define THREAD_COUNT 12

typedef struct {
	int cell_start;
	int cell_end;

	char** background;
	char** foreground;
} thread_args_t;

void dummy_handler(int signo) {__asm__("nop");}

void* thread_function(void* arg) {
	thread_args_t* args = (thread_args_t*)arg;
	
	while (true) {
		pause();

		for (int i = args->cell_start; i < args->cell_end; i++) {
			int row = i / GRID_WIDTH;
			int col = i % GRID_WIDTH;
			(*args->background)[i] = is_alive(row, col, *args->foreground);
		}
	}
}

int main()
{
	struct sigaction sa;
	sa.sa_handler = dummy_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);

	pthread_t threads[THREAD_COUNT];
	thread_args_t args[THREAD_COUNT];
	
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	int cells_per_thread = (int)ceil(GRID_HEIGHT * GRID_WIDTH / THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; i++) {
		args[i].cell_start = i * cells_per_thread;
		args[i].cell_end = (i + 1) * cells_per_thread;
		args[i].foreground = &foreground;
		args[i].background = &background;

		pthread_create(&threads[i], NULL, thread_function, &args[i]);
	}

	init_grid(foreground);
 
	while (true)
	{
		draw_grid(foreground);
		for(int i = 0; i < THREAD_COUNT; i++) {
			pthread_kill(threads[i], SIGUSR1);
		}

		usleep(500 * 1000);

		// Step simulation
		//update_grid(foreground, background);
		
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
