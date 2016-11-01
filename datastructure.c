#include <stdio.h>
#include "datastructure.h"
#include <stdlib.h>
#include <errno.h>
#include <ncurses.h>

void ds_init(data_structure *ds) {
	ds->mid = ds->head = ds->current = ds->tail = NULL;
	ds->currentx = ds->currenty = ds->length = 0;	
}

/* adds line at pos, sets ds->current to that line node and returns 1 if successful else returns 0 */
int ds_add_line(data_structure *ds, int pos) {
	int i = 0;
	node *temp, *cursor;
	/* initializing line */
	line *l;
	l = (line*)malloc(sizeof(l));
	line_init(l);
	if(pos < 0 || pos > ds->length)
		return 0;
	temp = (node*)malloc(sizeof(node));
	temp->next = NULL;
	temp->prev = NULL;
	temp->l = l;
	/* if empty */
	if(ds->head == NULL) {
		ds->head = temp;
		ds->tail = temp;
	}
	else if(pos == 0) {
		temp->next = ds->head;
		ds->head->prev = temp;
		ds->head = temp;
	}
	/* if insert at last pos*/
	else if(pos == ds->length) {
		cursor = ds->tail;
		temp->prev = cursor;
		cursor->next = temp;
		ds->tail = temp;
	}	
	else {
		cursor = ds->head;
		while(i < pos - 1) {
			cursor = cursor->next;
			i++;
		}
		cursor->next->prev = temp;	
		temp->next = cursor->next;
		temp->prev = cursor;
		cursor->next = temp;
	}
	/* setting current to that line node */
	ds->current = temp;
	/* setting currenty to pos */
	ds->currenty = pos;
	/* setting current x to 0*/
	ds->currentx = 0;	
	/* incrementing ds length */
	(ds->length)++;
	return 1;
		
}


void ds_append_line(data_structure *ds) {
	ds_add_line(ds, ds->currenty + 1);
}


int ds_delete_line(data_structure *ds, int pos) {
	node *cursor;
	
	if(!ds->head)
		return 0;
	if(pos < 0 || pos >= ds->length)
		return 0;
			
	if(pos == 0) {
		cursor = ds->head;
		if(cursor->next) {
			cursor->next->prev = NULL;
			ds->head = cursor->next;
		}
		else {
			ds->head = NULL;
			ds->tail = NULL;
		}
	}
	else if(pos == ds->length - 1) {
		cursor = ds->tail;
		cursor->prev->next = NULL;
		ds->tail = cursor->prev;
	}
	else {
		int i = 0;
		while(i < pos) {
			cursor = cursor->next;
			i++;
		}
		cursor->prev->next = cursor->next;
		cursor->next->prev = cursor->prev;
		
	}
	/* freeing memory */
	free(cursor->l->string);
	free(cursor->l);
	free(cursor);
	(ds->length)--;	
	return 1;
}

int ds_delete_current_line(data_structure *ds) {
	node *temp;
	temp = ds->current;
	if(ds->current->next) {
		ds->current->prev->next = ds->current->next;
		ds->current->next->prev = ds->current->prev;
		ds->current = ds->current->next;
		ds->currentx = 0;
		free(temp);
		return 1;
	}
	
	ds->current->prev->next = NULL;
	ds->current = ds->current->prev;
	ds->currentx = 0;
	free(temp);
	ds->currenty--;
	ds->length--;
	return 2;
}

/* prints the content of datastructure */
void ds_print(data_structure *ds) {
	node *cursor;
	cursor = ds->head;
	while(cursor) {
		printf("%s\n", cursor->l->string);
		cursor = cursor->next;
	}
}

/* appends char ch to current pos */
void ds_append_ch(data_structure *ds, char ch) {
	line_add_char(ds->current->l, ds->currentx, ch);
	if(ch == '\0')
		ds->current->l->length = strlen(ds->current->l->string);
	(ds->currentx)++;
}

int ds_delete_char(data_structure *ds) {
	if(ds->currentx < 1)
		return 0;
	line_del_char(ds->current->l, ds->currentx - 1);
	ds->currentx--;
	return 1;
}




/* loads a text_file into data structure */
data_structure *ds_load_file(char *filename) {
	char ch;
	int i;
	data_structure *ds;
	FILE *fp;
	ds = (data_structure*)malloc(sizeof(data_structure));
	ds_init(ds);
	ds_add_line(ds, ds->length);
	if(ds == NULL) {
		printf("Failed to malloc data_structure\n");
		exit(5);
	}
	
	if(!filename)
		return ds;
	
	fp = fopen(filename, "r");
	if(fp == NULL) 
		return ds;
	
	while(1) {
		ch = fgetc(fp);
		if(ch == EOF)
			break;
		if(ch == '\t') {
			for(i = 0; i < TAB; i++)
				ds_append_ch(ds, ' ');
		}	
		else if(ch != '\n') 	
			ds_append_ch(ds, ch);
		else
			ds_append_line(ds);
	
	}
	ds->current = ds->head;
	ds->currentx = ds->currenty = 0;
	/* if file is not empty */
	if(ds->length != 1)
		/* delete extra '\n' */
		ds_delete_line(ds, ds->length - 1);
	fclose(fp);
	return ds;	
}

int ds_save_file(data_structure *ds, char *filename) {
	node *cursor;
	cursor = ds->head;
	FILE *fp;
	/* TODO make sure we have permisions to write to file */
	fp = fopen(filename, "w");
	if(fp == NULL) {
		endwin();
		perror("Error");
		exit(errno);
	}
	while(cursor) {
		fprintf(fp, "%s\n", cursor->l->string);
		cursor = cursor->next;
	}
	return 1;
}

/* returns 1 if it successfully moves cursor to left */
int ds_move_left(data_structure *ds) {
	if(ds->currentx == 0)
		return 0;		
	(ds->currentx)--;
	return 1;
}


/* returns 1 if it successfully moves cursor to right */
int ds_move_right(data_structure *ds) {
	if(ds->currentx == ds->current->l->length)
		return 0;
	(ds->currentx)++;
		return 1;	
}

/* returns 1 if it successfully moves current upword */
int ds_move_up(data_structure *ds) {
	if(ds->current->prev == NULL)
		return 0;
	ds->current = ds->current->prev;
	(ds->currenty)--;
	
	/* if require change currentx*/
	if(ds->current->l->length < ds->currentx)
		ds->currentx = ds->current->l->length;
		
	return 1;	
}

/* returns 1 if it successfully moves current downward */
int ds_move_down(data_structure *ds) {
	if(ds->current->next == NULL)
		return 0;
	ds->current = ds->current->next;
	(ds->currenty)++;
	
	/* if require change currentx*/
	if(ds->current->l->length < ds->currentx)
		ds->currentx = ds->current->l->length;
			
	return 1;	
}


char *ds_get_current_string(data_structure *ds) {
	return ds->current->l->string;
}

int ds_get_current_line_length(data_structure *ds) {
	return ds->current->l->length;
}


