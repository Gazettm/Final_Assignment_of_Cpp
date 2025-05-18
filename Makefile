CXX = g++
CXXFLAGS = -std=c++11 -Wall -fPIC
QT_INCLUDE = -I/usr/include/x86_64-linux-gnu/qt5/ \
             -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
             -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
             -I/usr/include/x86_64-linux-gnu/qt5/QtCore
QT_LIBS = -lQt5Widgets -lQt5Gui -lQt5Core

SRC = main.cpp gamewindow.cpp gomokuboard.cpp
OBJ = $(SRC:.cpp=.o) moc_gamewindow.o  # 添加 moc 生成的目标文件
TARGET = gomoku

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(QT_LIBS)

# 生成 moc 文件
moc_%.cpp: %.h
	moc $< -o $@

# 编译 moc 文件
moc_%.o: moc_%.cpp
	$(CXX) $(CXXFLAGS) $(QT_INCLUDE) -c $< -o $@

# 编译普通源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(QT_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) moc_*
