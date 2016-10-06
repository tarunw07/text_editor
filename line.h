#define MAX_STRING_LENGTH 1024

typedef struct {
	char *string;
	/* max_length -> maximum possible string length */
	int length, max_length;
}line;


void line_init(line *l);
void line_add_char(line *l, int pos, char ch);
void line_append_char(line *l, char ch);
void line_del_char(line *l, int pos);
int length(line *l) ;
void print_line(line *l);
line *create_line();
