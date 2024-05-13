#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>


char *create_grid()
{
    return malloc(sizeof(char) * GRID_WIDTH * GRID_HEIGHT);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            if (grid[i * GRID_WIDTH + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= GRID_HEIGHT || c < 0 || c >= GRID_WIDTH)
            {
                continue;
            }
            if (grid[GRID_WIDTH * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * GRID_WIDTH + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            dst[i * GRID_WIDTH + j] = is_alive(i, j, src);
        }
    }
}