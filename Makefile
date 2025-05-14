# 定义编译器和编译选项
CXX = g++
CXXFLAGS = -Wall -g

# 定义目标可执行文件
TARGET = 123

# 定义源代码文件
SRC = 1.cpp

# 默认规则：如何从源代码生成可执行文件
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
