editor: text_editor.o datastructure.o line.o
	cc text_editor.o datastructure.o line.o -o editor -lncurses
		
text_editor.o: text_editor.c datastructure.h
	cc -c text_editor.c
	
datastructure.o: datastructure.c datastructure.h line.o 
	cc -c datastructure.c
	
line.o: line.c line.h
	cc -c line.c	

			
