SOURCES :=  $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)

TARGET := ./Neody
SOURCE_DIR := .

DIR_OBJ := .
COMPILER := g++

FLAGS = -std=c++17  -pthread -Wall -Wextra -pedantic
OBJ := $(SOURCES:.cpp=.o)
DEPS := $(SOURCES:.cpp=.d)

$(TARGET) : $(OBJ)
	$(COMPILER) $(OBJ) $(FLAGS) -o $(TARGET)

$(DIR_OBJ)/%.o : $(SOURCE_DIR)/%.cpp
	$(COMPILER) -c -MD $(FLAGS) $< -o $@

-include $(DIR_OBJ)/*.d

.PHONY: clean install all
clean:
	@rm -rf *.o $(TARGET) $(OBJ) $(DEPS)
	@rm -f $(TARGET)

install:
	sudo apt-get install -y g++ make

all:
	echo "init"