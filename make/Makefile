#shell
MKDIR_P = mkdir -p

#compiler
CC = g++

#cpp flags
FLAGS      = -std=c++11
CPPFLAGS   = -fPIC -Wall -Wextra 
LDFLAGS    = -shared

#directories
SRC_DIR 	= ../src
INC_DIR 	= ../inc
LIB_DIR 	= ../lib
OBJ_DIR 	= ../obj
DEV_OBJ_DIR = ../obj/dev
DEV_SRC_DIR = ../src/device

#common
SRCS = $(shell echo $(SRC_DIR)/*.cpp)
HDRS = $(shell echo $(INC_DIR)/*.h)
#replace ../src/NAME.cpp by ../obj/NAME.o
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(notdir $(SRCS:.cpp=.o)))


#device
DEV_SRCS = $(shell echo $(SRC_DIR)/device/*.cpp)
DEV_HDRS = $(shell echo $(INC_DIR)/device/*.h)
DEV_OBJS = $(patsubst %,$(DEV_OBJ_DIR)/%,$(notdir $(DEV_SRCS:.cpp=.o))) 

#shared library
LIB_SO = libowonparse.so
TARGET = $(LIB_DIR)/$(LIB_SO)


#line break
define n
endef

#default target
error :
	@echo "Please choose on of the following targets: 'compile', 'link', 'clean'."
	@exit 2


.PHONY: directories


compile : directories $(TARGET)

directories : $(OBJ_DIR) $(LIB_DIR) $(DEV_OBJ_DIR)

$(LIB_DIR):
	$(MKDIR_P) $(LIB_DIR)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(DEV_OBJ_DIR):
	$(MKDIR_P) $(DEV_OBJ_DIR)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS) $(DEV_HDRS)
	$(CC) -c $(FLAGS) -I$(INC_DIR) -I$(INC_DIR)/device $(CPPFLAGS) -o $@ $<

$(DEV_OBJ_DIR)/%.o: $(DEV_SRC_DIR)/%.cpp $(HDRS) $(DEV_HDRS)
	$(CC) -c $(FLAGS) -I$(INC_DIR) -I$(INC_DIR)/device $(CPPFLAGS) -o $@ $<


$(TARGET) : $(OBJS) $(DEV_OBJS)
	$(info $nShared library compilation...)
	$(CC) $(LDFLAGS) -o $@ $^


.PHONY: link
link :
	$(info $nLinking library...)
	cp $(TARGET) /usr/lib
	chmod 0755 /usr/lib/$(LIB_SO)
	ldconfig
	ldconfig -p | grep $(LIB_SO) 


.PHONY: clean
clean:
	$(info $nCleaning...)
	rm -rf $(OBJ_DIR)
