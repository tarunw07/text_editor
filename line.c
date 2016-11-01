#include "line.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void line_init(line *l) {
	l->string = (char*)malloc(MAX_STRING_LENGTH);
	l->string[0] = '\0';
	l->length = 0;
	l->max_length = MAX_STRING_LENGTH;
}


void line_add_char(line *l, int pos, char ch) {
	int j;
	/* Handle errors */
	if(pos < 0 || pos > length(l)) {
		err(1, "Invalid pos in line_add_char");		
	}
		
	if(length(l) == l->max_length) {
		//TODO relloc
		l->max_length *= 2;
		l->string = (char*)realloc(l->string, l->max_length);
		if(l->string == NULL) {
			printf("Failed allocationg memory for line\n");
			exit(3);
		}
		
	}
	
	/* insert char at pos */
	for(j = length(l) + 1; j > pos; j--) {
		l->string[j] = l->string[j - 1];
	}
	l->string[j] = ch;
	l->length++;
	
}
void line_append_char(line *l, char ch) {
	l->string[l->length] = ch;
	l->string[l->length + 1] = '\0';
	l->length++;
}

void line_del_char(line *l, int pos) {
	int j, len;
	len = length(l);
	if(pos < 0 || pos > len) {
		err(1, " %d Invalid pos in line_dek_char", pos);		
	}
	/* delete char */
	for(j = pos; j < len; j++) {
		l->string[j] = l->string[j + 1];
	}
	l->length--;
}

void print_line(line *l) {
	printf("%s\n", l->string);
}	

int length(line *l) {
	return l->length;
}

line *create_line() {
	line *l;
	l = (line*)malloc(sizeof(line));
	line_init(l);
	return l;
}
