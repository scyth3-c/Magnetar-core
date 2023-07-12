SOURCES :=  $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)

TARGET := ./binary
SOURCE_DIR := .

DIR_OBJ := .
COMPILER := g++

FLAGS = -std=c++17 - -pthread
OBJ := $(SOURCES:.cpp=.o)
DEPS := $(SOURCES:.cpp=.d)

$(TARGET) : $(OBJ)
	$(COMPILER) $(OBJ) $(FLAGS) -o $(TARGET)


$(DIR_OBJ)/%.o : $(SOURCE_DIR)/%.cpp 
	$(COMPILER) -c -MD $(FLAGS) $< -o $@

-include $(DIR_OBJ)/*.d

.PHONY: clean
clean:
	@rm -rf *.o $(TARGET) $(OBJ) $(DEPS)