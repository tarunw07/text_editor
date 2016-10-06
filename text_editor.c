#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "datastructure.h"

void add_datastructure_to_window(WINDOW *win, data_structure *ds);
void moveup(WINDOW *win, data_structure *ds);
void movedown(WINDOW *win, data_structure *ds);
void moveleft(WINDOW *win, data_structure *ds);
void moveright(WINDOW *win, data_structure *ds);

#define TAB 8

int main(int argc, char *argv[]) {
	int row, col, y, x, temp;
	int ch;
	char temp2[1024];
	data_structure *ds;
	if(argc < 2) {	
		printf("Ussage ./editor filename\n");
		return 1;
	}	
	ds = ds_load_file(argv[1]);	
	initscr();
	keypad(stdscr,TRUE);
	noecho();
	raw();
	
	
	add_datastructure_to_window(stdscr, ds);			
	while(1) {
		ch = getch();
		switch (ch) {
			case KEY_LEFT:
				moveleft(stdscr, ds);
				break;
			case KEY_RIGHT:
				moveright(stdscr, ds);
				break;
			case KEY_UP:
				moveup(stdscr, ds);
				break;
			case KEY_DOWN:
				movedown(stdscr, ds);
				break;
			case KEY_DL:
				addch('d');
				break;
			case KEY_DC:
				addch('c');
				break;
			case KEY_SF:
				addch('s');
				break;
			case KEY_BACKSPACE:
				//move(y, x-1);
				getyx(stdscr, y, x);
				mvdelch(y, x-1);
				break;
			/* TAB KEY....TODO use tab as 4 spaces */	
			case 9:
				insch(ch);
				getyx(stdscr, y, x);
				move(y, x + TAB - 2);
				break;	
			/* ENTER KEY */		
			case 10:
				addch('\n');
				//TODO
				strcpy(temp2, ds->current->l->string + ds->currentx);
				/* end the line */
				ds_append_ch(ds, '\0');
				ds_add_line(ds, ds->currenty + 1);
				//TODO
				//
				
				break;					
			default:
				//move(y, x+3);
				insch(ch);
				ds_append_ch(ds, ch);
				getyx(stdscr, y, x);
				move(y, x+1);

				//move(y, x+2);
				//printf("moved\n");						
		}
	
		//addch(ch);
		if(ch == 'q') {
			//insertln();
			break;
		}
		refresh();
	}
	ds_save_file(ds, "output.txt");
	endwin();
	ds_print(ds);

}


void add_datastructure_to_window(WINDOW *win,data_structure *ds) {
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
	int x, y;
	getyx(win, y, x);
	if(ds_move_up(ds))
		move(y-1, ds->currentx);
	
}


void movedown(WINDOW *win, data_structure *ds) {
	int x, y;
	getyx(win, y, x);
	if(ds_move_down(ds))
		move(y+1, ds->currentx); 
}

void moveleft(WINDOW *win, data_structure *ds) {
	int x, y;
	getyx(win, y, x);
	if(ds_move_left(ds))
		move(y, x-1); 
}

void moveright(WINDOW *win, data_structure *ds) {
	int x, y;
	getyx(win, y, x);
	if(ds_move_right(ds))
		move(y, x+1);
}







