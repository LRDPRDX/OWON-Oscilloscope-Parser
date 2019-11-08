#compiler
CC = g++

#cpp flags
FLAGS 		= -std=c++11
CPPFLAGS 	= -Wall -Wextra

#libs
LIB     	= -lowonparse

#directories
MAIN_DIR 	= ../../../../
INC_DIR     = $(MAIN_DIR)/inc
DEV_DIR 	= $(INC_DIR)/device

#choose the executable name
EXE      	= basic 

SRC     	= $(shell echo *.cpp)


$(EXE) : $(SRC)
	$(CC) $(SRC)  $(FLAGS) -I$(INC_DIR) -I$(DEV_DIR) $(LIB) -o $(EXE) 


.PHONY: clean
clean :
	rm $(EXE)
