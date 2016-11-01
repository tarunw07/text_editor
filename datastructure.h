#include <string.h>
#include "line.h"
#define TAB 8

typedef struct node {
	line *l;
	struct node *next;
	struct node *prev;
}node;

typedef struct data_structure {
	/* current points to current line(line where cursor is placed)
	 * mid points to last line displayed on window
	 */
	node *head, *current, *tail, *mid;
	/* currenty stores the line number and currentx  stores the position of cursor on that line 
	 */
	int currentx, currenty, length;
}data_structure;

/* intializes data_structure */
void ds_init(data_structure *ds);

/* adds a new line in data_structure */
int ds_add_line(data_structure *ds, int pos);

/* adds a new line below current line */
void ds_append_line(data_structure *ds);

int ds_delete_line(data_structure *ds, int pos);

int ds_delete_current_line(data_structure *ds);

/* prints the contents of data_structure */
void ds_print(data_structure *ds);

/* inserts ch at current position */
void ds_append_ch(data_structure *ds, char ch);

/* loads text file into data_structure */
data_structure *ds_load_file(char *filename);

/* saves text file into datastructure */
int ds_save_file(data_structure *ds, char *filename);

int ds_delete_char(data_structure *ds);

int ds_move_left(data_structure *ds);

int ds_move_right(data_structure *ds);

int ds_move_up(data_structure *ds);

int ds_move_down(data_structure *ds);

int ds_get_current_line_length(data_structure *ds);

char *ds_get_current_string(data_structure *ds);


