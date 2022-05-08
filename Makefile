#$(info $(OS))
ifeq ($(OS), Windows_NT)
LIB_LINKS := -lmingw32 -lfreeglut -lopengl32 -lglu32
else
LIB_LINKS := -lfreeglut -lopengl32 -lglu32
endif

CXX := g++
CXXFLAGS := -Wall -g
CPPFLAGS := -I include -L lib

NAME := epv_c
SRC := src
OBJ := obj
BIN := bin
LIB := lib

EXT := cpp
EXTH := h

SRC_FILES := $(wildcard $(SRC)/**.$(EXT))
#SRC_FILES += $(wildcard $(SRC)/*.$(EXTH))
OBJ_FILES := $(patsubst %.$(EXT), %.o, $(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC)/%, $(OBJ)/%, $(OBJ_FILES))
#EXECUTABLE_FILES = $(NAME:%=$(BIN)/%)
EXECUTABLE_FILES = $(BIN)/$(NAME)

build: $(EXECUTABLE_FILES)

clean:
	rm -r -f $(EXECUTABLE_FILES)
	rm -r -f $(OBJ_FILES)

.PHONY: build clean

$(OBJ_FILES): $(OBJ)/%.o : $(SRC)/%.$(EXT)
	$(CXX) $(CPPFLAGS) $(LIB_LINKS) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE_FILES): $(OBJ_FILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(EXECUTABLE_FILES) $(OBJ_FILES) $(LIB_LINKS)
