SRC_DIR := .
OBJ_DIR := .
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDES := /opt/local/include
LDFLAGS := -L /opt/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CPPFLAGS := -g -Wall -fpermissive -std=c++17
CXXFLAGS :=
TARGET := Particle.out

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -I $(INCLUDES) -c -o $@ $<

run:
	./$(TARGET)

clean:
	rm $(TARGET) *.o
