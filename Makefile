# for Mac OS X
CFLAGS = -ansi -Wall
LFLAGS = -framework OpenGL -framework GLUT -framework Foundation -dynamiclib -llua

all: glut.so gl.so

glut.so: glut.cpp
	g++ $(CFLAGS) $(LFLAGS) -o glut.so glut.cpp

gl.so: gl.cpp
	g++ $(CFLAGS) $(LFLAGS) -o gl.so gl.cpp
