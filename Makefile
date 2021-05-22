FLAGS= -std=c99 -D_GNU_SOURCE -Iincludes -Wall -O3
COMPILER=gcc
INCLUDES=includes
SRC=src
OBJ=obj
PROG=program
MODEL=model
VIEW=view
CONTROLLER=controller

server: 
	$(COMPILER) $(SRC)/$(PROG)/aurrasd.c $(SRC)/$(MODEL)/reading.c $(SRC)/$(MODEL)/structs.c $(SRC)/$(MODEL)/request.c $(SRC)/$(MODEL)/filter.c $(SRC)/$(MODEL)/task.c $(FLAGS) -o server

client: 
	$(COMPILER) $(SRC)/$(PROG)/aurras.c $(SRC)/$(MODEL)/request.c   $(FLAGS) -o client


clean:
	rm server
	rm client
	rm 7*
	rm 8*