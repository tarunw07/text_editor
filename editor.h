#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "datastructure.h"

#define TAB 8

void add_datastructure_to_window(WINDOW *win, data_structure *ds);
void moveup(WINDOW *win, data_structure *ds);
void movedown(WINDOW *win, data_structure *ds);
void moveleft(WINDOW *win, data_structure *ds);
void moveright(WINDOW *win, data_structure *ds);
