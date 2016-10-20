#include <stdio.h>
#include "editor.h"



int main(int argc, char *argv[]) {
	int row, col, y, x, temp, current_line_length, maxx, maxy;
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
				//TODO
				getyx(stdscr, y, x);
				getmaxyx(stdscr, maxy, maxx);
				current_line_length = ds->current->l->length;
				temp = current_line_length/maxx + 1;
				
				
				//move to start of line
				move(y - ds->currentx/maxx, 0);
				/* delete temp lines*/
				insdelln(-1*temp);
				
				
				strcpy(temp2, ds->current->l->string + ds->currentx);
				/* end the line */
				ds_append_ch(ds, '\0');
				current_line_length = ds->current->l->length;
				temp = current_line_length/maxx + 1;
				insdelln(temp);
				move(y - (ds->currentx - 1)/maxx, 0);
				wprintw(stdscr, "%s\n", ds->current->l->string);
				
				ds_append_line(ds);
				strcpy(ds->current->l->string, temp2);
				ds->current->l->length = strlen(temp2);
				current_line_length = ds->current->l->length;
				temp = current_line_length/maxx + 1;
				insdelln(temp);
				move(y + 1 - (ds->currentx - 1)/maxx, 0);
				wprintw(stdscr, "%s\n", ds->current->l->string);
				move(y + 1, 0);
				//
				
				break;					
			default:
				//insch(ch);
				getyx(stdscr, y, x);
				getmaxyx(stdscr, maxy, maxx);
				current_line_length = ds->current->l->length;
				temp = current_line_length/maxx + 1;
				
				//move to start of line
				move(y - ds->currentx/maxx, 0);
				/* delete temp lines*/
				insdelln(-1*temp);
				ds_append_ch(ds, ch);
				current_line_length = ds->current->l->length;
				temp = current_line_length/maxx + 1;
				insdelln(temp);
				move(y - (ds->currentx - 1)/maxx, 0);
				//printf("%d %d\n", x, maxx);
				wprintw(stdscr, "%s\n", ds->current->l->string);
				if(x+1 >= maxx)
					move(y+1, 0);
				else	
					move(y, x+1);
				//getyx(stdscr, y, x);
				//moveright(stdscr, ds);

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
