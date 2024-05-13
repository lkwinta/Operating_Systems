#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define THREAD_COUNT 12

typedef struct {
	int row_start;
	int column_start;

	int row_end;
	int column_end;
	char** grid_ptr;
} thread_args_t;

void* thread_function(void* arg) {
	thread_args_t* args = (thread_args_t*)arg;
	int row = args->row_start;
	int column = args->column_start;

	while(row != args->row_end && column != args->column_end) {
		(*args->grid_ptr)[row * GRID_WIDTH + column] = is_alive(row, column, *args->grid_ptr);
		column++;
		if(column == GRID_WIDTH) {
			column = 0;
			row++;
		} else if(row == args->row_end && column == args->column_end) {
			break;
		}
	}
}

int main()
{
	pthread_t threads[THREAD_COUNT];
	thread_args_t args[THREAD_COUNT];

	int cells_per_thread = GRID_HEIGHT * GRID_WIDTH / THREAD_COUNT;
	int current_cell_column = 0;
	int current_cell_row = 0;
	int current_cells_count = 0;

	for (int i = 0; i < THREAD_COUNT; i++) {
		args[i].row_start = i * GRID_HEIGHT / THREAD_COUNT;
		pthread_create(&threads[i], NULL, thread_function, &args[i]);
	}

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	init_grid(foreground);

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation
		update_grid(foreground, background);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
