prog2server.out: server.o serverFunctions.o 
	gcc -g -Wall -Werror -o prog2server.out server.o serverFunctions.o 

server.o: server.c packets.h serverFunctions.h
	gcc -g -Wall -Werror -c server.c

serverFunctions.o: serverFunctions.c packets.h 
	gcc -g -Wall -Werror -c serverFunctions.c

clean:
	rm -fr *.o *.out