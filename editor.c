#include "editor.h"

void add_datastructure_to_window(WINDOW *win, data_structure *ds) {
	int i = 0, maxx, maxy, n, y = 0;
	getmaxyx(win, maxy, maxx);
	n = maxx;
	node *cursor;
	cursor = ds->head;
	while(cursor) {
		i += 1;
		if(i > maxy)
			break;
  		/* print at most first three characters (safe) */
		wprintw(win, "%.*s", n - 1, cursor->l->string);
		if(cursor->l->length >= n - 1) {
			scrollok(win, FALSE);
			wprintw(win, "$");
			scrollok(win,TRUE);	
		}
		wmove(win, y + 1, 0);
		y++;
		ds->mid = cursor;
		cursor = cursor->next;
	}
	/* set currentx currenty to 0, 0*/
	ds->current = ds->head;
	ds->currentx = ds->currenty = 0;
	wmove(win, 0, 0);
}

int moveup(WINDOW *win, data_structure *ds) {
	int x, y, maxy, maxx;
	getmaxyx(win, maxy, maxx);
	getyx(win, y, x);
	if(ds->current->prev) {
		/* update the current line */
		delete_current_line_from_window(win, ds);
		winsdelln(win, 1);
		wprintw(win, "%.*s", maxx - 1, ds->current->l->string);
		if(ds->current->l->length >= maxx - 1) {
			scrollok(win, FALSE);
			wprintw(win, "$");
			scrollok(win,TRUE);	
		}
		/* go up */
		ds_move_up(ds);
		if(y == 0) {
			y++;
			wmove(win, 0, 0);
			winsdelln(win, 1);
			wprintw(win, "%.*s", maxx - 1, ds->current->l->string, y, maxy);
			if(ds->current->l->length >= maxx - 1) {
				scrollok(win,FALSE);
				wprintw(win, "$");
				scrollok(win,TRUE);
			}
			/* update ds->mid NOTE : it will be next in if condn not prev */
			if(ds->mid->next)
				ds->mid = ds->mid->prev;
		}
		else {
		
		} 
		wmove(win, y - 1, ds->currentx % (maxx - 1));
		return 1;
	}

	return 0;
}


int movedown(WINDOW *win, data_structure *ds) {
	int x, y, maxy, maxx;
	getmaxyx(win, maxy, maxx);
	getyx(win, y, x);
	
	if(ds->current->next) {
		/* update the current line */
		delete_current_line_from_window(win, ds);
		winsdelln(win, 1);
		wprintw(win, "%.*s", maxx - 1, ds->current->l->string, y, maxy);
		if(ds->current->l->length >= maxx - 1) {
			scrollok(win,FALSE);
			wprintw(win, "$");
			scrollok(win,TRUE);
		}
		/* go to next line */
		ds_move_down(ds);
				
		if(y + 1 != maxy) {
			y++;
			wmove(win, y, 0);
			delete_current_line_from_window(win, ds);
			winsdelln(win, 1);
		}
		else {
			wmove(win, y, 0);
			wscrl(win, 1);
			/* update ds->mid */
			if(ds->mid->next)
				ds->mid = ds->mid->next;
		}
		/* Insert   line */
		wprintw(win, "%.*s", maxx - 1, ds->current->l->string, y, maxy);
		if(ds->current->l->length >= maxx - 1) {
			scrollok(win,FALSE);
			wprintw(win, "$");
			scrollok(win,TRUE);
		}
		wmove(win, y, ds->currentx % (maxx - 1));
		return 1;
	}
	return 0;
}

int moveleft(WINDOW *win, data_structure *ds) {
	int x, y, maxx, maxy, startpos;
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	if(ds_move_left(ds)) {
		if(x-1 < 0) {
			wmove(win, y, 0);
			startpos = (ds->currentx/(maxx - 1)) * (maxx - 1);
			wprintw(win, "%.*s", maxx - 1, (ds->current->l->string + startpos));
			wprintw(win, "$");
			wmove(win, y, maxx - 2);
		}	
		else	
			wmove(win, y, x-1);
		return 1;	
	}
	return 0;
}

int moveright(WINDOW *win, data_structure *ds) {
	int x, y, maxx, maxy;
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	
	if(ds_move_right(ds)) {
		if(x+1 >= maxx - 1) {
			wmove(win, y, 0);
			wprintw(win, "%.*s", maxx - 1, (ds->current->l->string + ds->currentx));
			if(ds->current->l->length/(maxx - 1) != ds->currentx/(maxx - 1)) {
				scrollok(win,FALSE);
				wprintw(win, "$");
				scrollok(win,TRUE);
			}
			else {	
				/* delete remaining part till end of line */
				wclrtoeol(win);
			}
			wmove(win, y, 0);
		}	
		else	
			wmove(win, y, x+1);
		return 1;	
	}
	return 0;
}


/*
 * deletes current_line_from_window where cursor is placed
 */
void delete_current_line_from_window(WINDOW *win, data_structure *ds) {
	int y, x, maxx, maxy;	
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	wmove(win, y, 0);
	/* delete  line*/
	winsdelln(win, -1);
}

/*
 * adds current line to window where cursor is placed
 */
void add_current_line_to_window(WINDOW *win, data_structure *ds) {
	int maxy, maxx, y, x, startpos;
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	startpos = (ds->currentx/(maxx - 1)) * (maxx - 1);
	wmove(win, y, 0);
	wprintw(win, "%.*s", maxx - 1, (ds->current->l->string + startpos));
	if(ds->current->l->length/(maxx - 1) != ds->currentx/(maxx - 1)) {
		scrollok(win,FALSE);
		wprintw(win, "$");
		scrollok(win,TRUE);
	}	
	wmove(win, y, x);
}


void editor_enter_key(WINDOW *win, data_structure *ds) {
	int maxx, maxy, x, y;
	char temp2[1028];
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	//clrtoeol();
	strcpy(temp2, ds_get_current_string(ds) + ds->currentx);
	/* end the line */
	ds_append_ch(ds, '\0');
	
	/* update the current line */
	delete_current_line_from_window(win, ds);
	winsdelln(win, 1);
	wprintw(win, "%.*s", maxx - 1, ds->current->l->string, y, maxy);
	if(ds->current->l->length >= maxx - 1) {
		scrollok(win,FALSE);
		wprintw(win, "$");
		scrollok(win,TRUE);
	}
	/* Update current line length */
	ds->current->l->length = strlen(ds->current->l->string);
	if(y == maxy - 1) {
		wscrl(win, 1);
		wmove(win, y, 0);
	}
	else {
		wmove(win, y + 1, 0);
		winsdelln(win, 1);
	}
	/* add new line to ds */
	ds_append_line(ds);
	strcpy(ds->current->l->string, temp2);
	/* update current line length */
	ds->current->l->length = strlen(temp2);
	add_current_line_to_window(win, ds);
	/* update ds->mid */
	if(y == maxy - 1 || ds->current == ds->tail) {
		ds->mid = ds->current;
	}
	else {
		if(ds->mid->next)
			ds->mid = ds->mid->prev;
	}
}



void editor_add_default_char(WINDOW *win, data_structure *ds, int ch) {
	int y, x, maxx, maxy;
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	/* delete current line from window */				
	delete_current_line_from_window(win, ds);
	ds_append_ch(ds, ch);
	/* insert 1 blank line */
	winsdelln(win, 1);					
	add_current_line_to_window(win, ds);
	if(x + 1 != maxx - 1) 
		wmove(win, y, x + 1);
}


void editor_backspace_key(WINDOW *win, data_structure *ds) {
	int x, y, maxx, maxy, temp_winx, temp_winy, temp_dsx, temp;
	char *string;
	getyx(win, y, x);
	getmaxyx(win, maxy, maxx);
	if(ds->currenty == 0 && ds->currentx == 0)
		return;
	else if(ds->currentx == 0) {					
		/* get current string */
		string = ds_get_current_string(ds);
		/* delete current line */
		delete_current_line_from_window(win, ds);
		temp = ds_delete_current_line(ds);
		/* if last line is deleted move up */
		if(temp == 2)
			wmove(win, y - 1, x);
		else if(temp == 1)
			moveup(win, ds);
		/* concat current line with string */
		while(moveright(win, ds));
		getyx(win, temp_winy, temp_winx);
		temp_dsx = ds->currentx;
		while(*string) {
			ds_append_ch(ds, *string);
			string++; 
		};
		ds->currentx = temp_dsx;
		delete_current_line_from_window(win, ds);
		winsdelln(win, 1);					
		add_current_line_to_window(win, ds);
		/* update last line */
		if(ds->mid->next) {
			ds->mid = ds->mid->next;
			wmove(win, maxy - 1, 0);
			wprintw(win, "%.*s", maxx - 1, ds->mid->l->string);
			if(ds->mid->l->length/(maxx - 1) != 0) {
				scrollok(win,FALSE);
				wprintw(win, "$");
				scrollok(win,TRUE);
			}
		}
		wmove(win, temp_winy, temp_winx);		
		
	}
	else {
		delete_current_line_from_window(win, ds);
		ds_delete_char(ds);
		winsdelln(win, 1);
		add_current_line_to_window(win, ds);
		if(x == 0) {
			wmove(win, y, maxx - 2);
		}	
		else	
			wmove(win, y, x - 1);
	}
}						

