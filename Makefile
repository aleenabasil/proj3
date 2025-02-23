BIN_DIR = bin 
OBJ_DIR = obj  
TARGET = $(BIN_DIR)/test

all: directories runtests

build g++ codefile.cpp

runtests: $(TARGET)
	$(TARGET)

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)