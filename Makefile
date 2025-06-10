CXX = g++
CXXFLAGS = -std=c++11 -Wall -fPIC
QT_INCLUDE = -I/usr/include/x86_64-linux-gnu/qt5/ \
             -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
             -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
             -I/usr/include/x86_64-linux-gnu/qt5/QtCore
QT_LIBS = -lQt5Widgets -lQt5Gui -lQt5Core

SRC = main.cpp gamewindow.cpp gomokuboard.cpp aiplayer.cpp Rating.cpp
OBJ = $(SRC:.cpp=.o) moc_gamewindow.o
TARGET = gomoku

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(QT_LIBS)

moc_%.cpp: %.h
	moc $< -o $@

moc_%.o: moc_%.cpp
	$(CXX) $(CXXFLAGS) $(QT_INCLUDE) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(QT_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) moc_*
