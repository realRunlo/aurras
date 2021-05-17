FLAGS= -std=c99 -Wall -o2 
COMPILER=gcc
INCLUDES=includes
SRC=src
OBJ=obj

server: 
	$(COMPILER) $(SRC)/aurrasd.c $(SRC)/reading.c $(SRC)/structs.c $(SRC)/request.c $(SRC)/filter.c $(SRC)/task.c $(FLAGS) -o server

client: 
	$(COMPILER) $(SRC)/aurras.c $(SRC)/request.c   $(FLAGS) -o client


clean:
	rm server
	rm client
	rm 7*
	rm 8*