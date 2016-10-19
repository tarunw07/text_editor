#include "editor.h"

void add_datastructure_to_window(WINDOW *win, data_structure *ds) {
	node *cursor;
	cursor = ds->head;
	while(cursor) {
		wprintw(win, "%s\n", cursor->l->string);
		cursor = cursor->next;
	}
	/* set currentx currenty to 0, 0*/
	ds->currentx = ds->currenty = 0;
	move(0, 0);
}

void moveup(WINDOW *win, data_structure *ds) {
	int x, y, maxy, maxx, temp, prev_line_length, prevx;
	getmaxyx(stdscr, maxy, maxx);
	prevx = ds->currentx;
	prev_line_length = ds->current->prev->l->length;
	temp = prev_line_length/maxx + 1;
	getyx(win, y, x);
	if(ds_move_up(ds)) {
		if(prevx > ds->currentx) 
			temp = temp - prevx/maxx + ds->currentx/maxx;
		move(y-temp, ds->currentx % maxx);
	}
}


void movedown(WINDOW *win, data_structure *ds) {
	int x, y, maxy, maxx, temp, current_line_length, prevx;
	getmaxyx(stdscr, maxy, maxx);
	current_line_length = ds->current->l->length;
	prevx = ds->currentx;
	temp = current_line_length/maxx + 1;
	getyx(win, y, x);
	if(ds_move_down(ds)) {
		if(prevx > ds->currentx) 
			temp = temp - prevx/maxx + ds->currentx/maxx;
		wmove(win, y+temp, ds->currentx % maxx);
	} 
}

void moveleft(WINDOW *win, data_structure *ds) {
	int x, y, maxx, maxy;
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	if(ds_move_left(ds)) {
		if(x-1 < 0)
			move(y-1, maxx - 1);
		else	
			move(y, x-1);
	}
}

void moveright(WINDOW *win, data_structure *ds) {
	int x, y, maxx, maxy;
	getyx(win, y, x);
	getmaxyx(stdscr, maxy, maxx);
	if(ds_move_right(ds)) {
		if(x+1 >= maxx)
			move(y+1, 0);
		else	
			move(y, x+1);
	}
}
