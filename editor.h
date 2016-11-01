#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "datastructure.h"


void add_datastructure_to_window(WINDOW *win, data_structure *ds);
int moveup(WINDOW *win, data_structure *ds);
int movedown(WINDOW *win, data_structure *ds);
int moveleft(WINDOW *win, data_structure *ds);
int moveright(WINDOW *win, data_structure *ds);

/* Deletes the full line where cursor is placed. */
void delete_current_line_from_window(WINDOW *win, data_structure *ds);

void editor_backspace_key(WINDOW *win, data_structure *ds);
void editor_add_default_char(WINDOW *win, data_structure *ds, int ch);
void editor_enter_key(WINDOW *win, data_structure *ds);
