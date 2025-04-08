# === CONFIG ===
CXX = g++
CXXFLAGS = -std=c++17 -I./src
TARGET = bin/main.exe
SRC = src/main.cpp src/FileProcessing.cpp src/EMM.cpp src/QuadTree.cpp

# === BUILD RULE ===
all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# === RUN RULE ===
run: all
	./$(TARGET)

# === CLEAN RULE ===
clean:
	@rm -f $(TARGET)

# Usage:
# 1. Run "make" to build main.exe in the bin folder.
# 2. Run "make run" to execute main.exe.
# 3. Run "make clean" to remove main.exe from bin.
