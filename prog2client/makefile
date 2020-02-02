prog2client.out: client.o clientFunctions.o 
	gcc -g -Wall -Werror -o prog2client.out client.o clientFunctions.o 

client.o: client.c packets.h clientFunctions.h
	gcc -g -Wall -Werror -c client.c

clientFunctions.o: clientFunctions.c packets.h 
	gcc -g -Wall -Werror -c clientFunctions.c

clean:
	rm -fr *.o *.out