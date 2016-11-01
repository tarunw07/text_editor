#include <stdio.h>
#include <stdlib.h>
#include "editor.h"


//TODO handle resize add search option
void bomb(void);
void initialize_header_window(WINDOW *win); 
void initialize_footer_window(WINDOW *win);
void help();

int main(int argc, char *argv[]) {
	int i, ch, maxx, maxy;
	data_structure *ds;
	WINDOW *header_window, *footer_window, *editor_window;
	char filename[128], temp[128];
	filename[0] = '\0';
	if(argc >= 2) {
		if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			help();
			return 0;	
		}	
		strcpy(filename, argv[1]);
	}
	
	ds = ds_load_file(argv[1]);
				
	initscr();
	noecho();
	raw();
	getmaxyx(stdscr, maxy, maxx);
	
	if( (header_window = newwin(2, maxx, 0, 0)) == NULL) bomb();
	if( (editor_window = newwin(maxy - 4,maxx, 2, 0)) == NULL) bomb();
	if( (footer_window = newwin(2, maxx, maxy - 2, 0)) == NULL) bomb();
	
	wrefresh(footer_window);
	wrefresh(header_window);
	wrefresh(editor_window);
	
	initialize_header_window(header_window);
	initialize_footer_window(footer_window);
	
	getmaxyx(header_window, maxy, maxx);
	keypad(editor_window, TRUE);
	scrollok(editor_window, TRUE);
	add_datastructure_to_window(editor_window, ds);
	wrefresh(editor_window);
	while(1) {
		ch = wgetch(editor_window);
		switch (ch) {
			case KEY_LEFT:
				moveleft(editor_window, ds);
				break;
				
			case KEY_RIGHT:
				moveright(editor_window, ds);
				break;
				
			case KEY_UP:
				moveup(editor_window, ds);
				break;
				
			case KEY_DOWN:
				movedown(editor_window, ds);
				break;
				
			case KEY_DL:
				break;
			case KEY_DC:
				if(moveright(editor_window, ds))
					editor_backspace_key(editor_window, ds);
				break;
			case KEY_SF:				
				break;
				
			case KEY_BACKSPACE:
				editor_backspace_key(editor_window, ds);
				break;
				
			/* TAB KEY */	
			case 9:
				for(i = 0; i < TAB; i++)
					editor_add_default_char(editor_window, ds, ' ');
				break;
					
			/* ENTER KEY */		
			case 10:
				editor_enter_key(editor_window, ds);						
				break;
			
			/* Ctrl + X */	
			case 24:				
				wmove(footer_window, 0, 0);
				wprintw(footer_window, "File name to write : %s", filename);
				echo();
				wrefresh(footer_window);
				wgetstr(footer_window, temp);
				strcat(filename, temp);
				ds_save_file(ds, filename);
				noecho();
				wrefresh(editor_window);
				endwin();
				return 0;
				break;
			/* Ctrl + C */
			case 3:
				endwin();
				return 0;
			
			case KEY_RESIZE:
				getmaxyx(stdscr, maxy, maxx);
				if(maxy < 6 || maxx < 3)
					break;
				delwin(header_window);
				delwin(footer_window);
				delwin(editor_window);
				if( (header_window = newwin(2, maxx, 0, 0)) == NULL) bomb();
				if( (editor_window = newwin(maxy - 4,maxx, 2, 0)) == NULL) bomb();
				if( (footer_window = newwin(2, maxx, maxy - 2, 0)) == NULL) bomb();
				keypad(editor_window, TRUE);
				scrollok(editor_window, TRUE);
				add_datastructure_to_window(editor_window, ds);
				initialize_header_window(header_window);
				initialize_footer_window(footer_window);
				break;
											
			default:
				editor_add_default_char(editor_window, ds, ch);				
				break;
									
		}
		wrefresh(editor_window);
	}

}


void bomb(void) {
	addstr("Unable to allocate memory for new window.\n");
	endwin();
}

void initialize_footer_window(WINDOW *win) {
	wmove(win, 1, 0);
	wprintw(win, "   ");
	wattron(win, A_REVERSE);
	wprintw(win, "^X");
	wattroff(win, A_REVERSE);
	wprintw(win, " Save");
	wprintw(win, "   ");
	wattron(win, A_REVERSE);
	wprintw(win, "^C");
	wattroff(win, A_REVERSE);
	wprintw(win, " Exit");
	wrefresh(win);	
}


void initialize_header_window(WINDOW *win) {
	int y, x, i;
	char str[] = "Text Editor";
	getmaxyx(win, y, x);
	wattron(win, A_REVERSE);
	for(i = 0; i < x; i++) {
		waddch(win, ' ');
	}
	wmove(win, 0, (x - strlen(str))/2);
	waddstr(win, str);
	wattroff(win, A_REVERSE);
	wrefresh(win);	
}

void help() {
	printf("Usage : ./project [filename]\n");
	printf("^ means Ctrl\n");
	printf("For exit without saving use ^C\n");
	printf("For save use ^X\n");
}
