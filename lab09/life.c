// declare feature test macros to enable certain functions (sigaction, usleep)
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

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define THREAD_COUNT 12 // define number of threads to use - don't exceed number of threads on your CPU

/**
 * Define structure that will be passed to the thread function 
 * as an argument
*/
typedef struct {
	int cell_start; // index of the starting cell (inclusive)
	int cell_end; // index of the ending cell (exclusive)

	char** background; // pointer to the background array
	char** foreground; // pointer to the foreground array
} thread_args_t;

/* declare dummy signal handler so the signal doesn't crash program */
void dummy_handler(int signo) {__asm__("nop");}

void* thread_function(void* arg) {
	/* dereference arguments struct */
	thread_args_t* args = (thread_args_t*)arg;
	
	/* thread loop */
	while (true) {
		/* pause thread until any signal comes */
		pause();

		/* iterate over assigned cel */
		for (int i = args->cell_start; i < args->cell_end; i++) {
			int row = i / GRID_WIDTH; // translate index to row
			int col = i % GRID_WIDTH; // translate index to column

			/* update cell in background array */
			(*args->background)[i] = is_alive(row, col, *args->foreground);
		}
	}
}

int main()
{
	/**
	 * Set up signal handler for threads
	 * We need to set up a dummy signal handler so the signal doesn't crash the program
	 * We will use the signal to wake up the threads
	*/
	struct sigaction sa;
	sa.sa_handler = dummy_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	pthread_t threads[THREAD_COUNT];
	thread_args_t args[THREAD_COUNT];

	int cells_per_thread = (int)ceil(GRID_HEIGHT * GRID_WIDTH / THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; i++) {
		// Assign cells to threads
		args[i].cell_start = i * cells_per_thread;
		args[i].cell_end = MIN((i + 1) * cells_per_thread, GRID_HEIGHT * GRID_WIDTH); // exclusive, safety check for the last thread

		/**
		 * We use pointers to the 1D array to be able to shuffle pointers between frames
		 * instead of copying the whole array
		*/
		args[i].foreground = &foreground;
		args[i].background = &background;

		// create threads without any special attributes and with argument structure
		pthread_create(&threads[i], NULL, thread_function, &args[i]);
	}

	init_grid(foreground);
 
	while (true)
	{
		draw_grid(foreground);

		// Wake up threads with declared signals
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
