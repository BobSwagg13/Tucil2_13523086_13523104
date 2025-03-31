# === CONFIG ===
CXX = g++
CXXFLAGS = -std=c++17 -I.        
TARGET = ../bin/main.exe         
SRC = main.cpp FileProcessing.cpp EMM.cpp              

# === BUILD RULE ===
all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p ../bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# === RUN RULE ===
run: all
	$(TARGET)

# === CLEAN RULE ===
clean:
	@rm -f $(TARGET)

# Cara Penggunaan:
# 1. make untuk membuat main.exe pada folder bin
# 2. make run untuk menjalankan main.exe
# 3. make clean	untuk menghapus main.exe dari bin
