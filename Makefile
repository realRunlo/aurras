FLAGS= -std=c99 -D_GNU_SOURCE -I$(INCLUDES) -Wall -O3 -g
COMPILER=gcc
INCLUDES=includes
SRC=src
OUT=bin
OBJ=$(OUT)/obj
PROG=program
MODEL=model
VIEW=view
CONTROLLER=controller

SERVER=$(PROG)/aurrasd
CLIENT=$(PROG)/aurras

MODULESD=$(MODEL)/reading $(MODEL)/structs $(MODEL)/request $(MODEL)/filter $(MODEL)/task
MODULES =$(MODEL)/request

_HEADERSD=$(MODULESD)
HEADERSD =$(patsubst %,$(INCLUDES)/%.h,$(_HEADERSD))

_HEADERS=$(MODULES)
HEADERS =$(patsubst %,$(INCLUDES)/%.h,$(_HEADERS))

_OBJECTSD=$(SERVER) $(MODULESD)
OBJECTSD =$(patsubst %,$(OBJ)/%.o,$(_OBJECTSD))

_OBJECTS=$(CLIENT) $(MODULES)
OBJECTS =$(patsubst %,$(OBJ)/%.o,$(_OBJECTS))

_SRCFILESD=$(SERVER) $(MODULESD)
SRCFILESD =$(patsubst %,$(SRC)/%.c,$(_SRCFILESD))

_SRCFILES=$(SRC)/$(CLIENT) $(MODULES)
SRCFILES =$(patsubst %,$(SRC)/%.c,$(_SRCFILES))

# Defines the dependency between MODULE.c, MODULE.h and MODULE.c
$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) $(HEADERSD)
	$(CC) -c -o $@ $< $(FLAGS)

.PHONY: clean fullclean memcheck directories

all: directories server client

directories:
	mkdir -p $(OBJ)/$(CONTROLLER) $(OBJ)/$(VIEW) $(OBJ)/$(MODEL) $(OBJ)/$(PROG) $(OUT)/$(PROG)

server: $(OBJECTSD)
	$(COMPILER) -o $(OUT)/$(SERVER) $^ $(FLAGS)

client: $(OBJECTS)
	$(COMPILER) -o $(OUT)/$(CLIENT) $^ $(FLAGS)

memcheckd:
	valgrind --leak-check=yes $(OUT)/$(SERVER)

memcheck:
	valgrind --leak-check=yes $(OUT)/$(CLIENT)

clean:
	rm -rf $(OUT)/$(SERVER) $(OUT)/$(CLIENT) $(OUT)/$(OBJ)