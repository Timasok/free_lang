STK_DIR = ./lib/stack/src/
ASM_DIR = ./lib/ASM/src/
CPU_DIR = ./lib/CPU/src/
LIB_DIR = ./lib/
LANG_DIR = ./src/

LOGS_DIR = ./logs/
GRAPH_DUMP_DIR = ./graph_dumps/
OBJ_DIR = ./obj/

I_FLAG := -I./lib/stack/include/ -I./lib/ASM/include/ -I./lib/CPU/include/ -I./lib/inc/ -I./lib/ -I./ -I./include/ -I./include/generators

CC := g++
CFLAGS := -Wno-format -g -fsanitize=address
CFLAGS_NO_SANITIZER := -Wno-format -g

LANG_EXE := free_lang
ASM_EXE  := assembler
CPU_EXE  := cpu

STK_SRC  := $(wildcard $(STK_DIR)*.cpp)
ASM_SRC  := $(wildcard $(ASM_DIR)*.cpp)
CPU_SRC  := $(wildcard $(CPU_DIR)*.cpp)
LIB_SRC  := $(wildcard $(LIB_DIR)*.cpp)
LANG_SRC := $(wildcard $(LANG_DIR)*.cpp)

STK_OBJ  := $(patsubst $(STK_DIR)%.cpp,  $(OBJ_DIR)%.o, $(STK_SRC))
ASM_OBJ  := $(patsubst $(ASM_DIR)%.cpp,  $(OBJ_DIR)%.o, $(ASM_SRC))
CPU_OBJ  := $(patsubst $(CPU_DIR)%.cpp,  $(OBJ_DIR)%.o, $(CPU_SRC))
LIB_OBJ  := $(patsubst $(LIB_DIR)%.cpp,  $(OBJ_DIR)%.o, $(LIB_SRC))
LANG_OBJ := $(patsubst $(LANG_DIR)%.cpp, $(OBJ_DIR)%.o, $(LANG_SRC))

all : $(ASM_EXE) $(CPU_EXE) $(LANG_EXE)

$(ASM_EXE) : $(ASM_OBJ) $(STK_OBJ) $(LIB_OBJ)
	@$(CC) $(CFLAGS_NO_SANITIZER) $(ASM_OBJ) $(STK_OBJ) $(LIB_OBJ) -o $(ASM_EXE)

$(CPU_EXE) : $(CPU_OBJ) $(STK_OBJ) $(LIB_OBJ)
	@$(CC) $(CFLAGS) $(CPU_OBJ) $(STK_OBJ) $(LIB_OBJ) -o $(CPU_EXE)

$(LANG_EXE) : $(LANG_OBJ) $(LIB_OBJ)
	@$(CC) $(CFLAGS) $(LANG_OBJ) $(LIB_OBJ) -o $(LANG_EXE)

$(OBJ_DIR)%.o : $(STK_DIR)%.cpp
	@$(CC) $(I_FLAG) -c $< -o $@

$(OBJ_DIR)%.o : $(ASM_DIR)%.cpp
	@$(CC) $(I_FLAG) -c $< -o $@

$(OBJ_DIR)%.o : $(CPU_DIR)%.cpp
	@$(CC) $(I_FLAG) -c $< -o $@

$(OBJ_DIR)%.o : $(LIB_DIR)%.cpp
	@$(CC) $(I_FLAG) -c $< -o $@

$(OBJ_DIR)%.o : $(LANG_DIR)%.cpp
	@$(CC) $(I_FLAG) -c $< -o $@


mkdir :
	@mkdir -p $(GRAPH_DUMP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LOGS_DIR)

clean :
	@rm $(LANG_EXE) $(OBJ_DIR)*.o 