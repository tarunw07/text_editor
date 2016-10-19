project: main.o editor.o datastructure.o line.o
	cc  main.o editor.o datastructure.o line.o -o project -lncurses
				
main.o: main.c editor.h
	cc -c main.c
		
editor.o: editor.c editor.h datastructure.h
	cc -c editor.c
	
datastructure.o: datastructure.c datastructure.h line.o 
	cc -c datastructure.c
	
line.o: line.c line.h
	cc -c line.c
			
