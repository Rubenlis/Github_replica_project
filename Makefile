all : prog myGit

myGit: part1.o part2.o part3.o myGit.o
	gcc -o myGit part1.o part2.o part3.o myGit.o

prog: part1.o part2.o part3.o main.o
	gcc -o prog part1.o part2.o part3.o main.o

myGit.o : myGit.c
	gcc -c -Wall myGit.c

main.o : main.c
	gcc -c -Wall main.c

part1.o : part1.c
	gcc -c -Wall part1.c

part2.o : part2.c
	gcc -c -Wall part2.c

part3.o : part3.c
	gcc -c -Wall part3.c

clean : 
	rm -f prog *.o


