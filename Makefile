# for Mac OS X

glut.so: glut.cpp
	g++ -ansi -Wall -framework OpenGL -framework GLUT -framework Foundation -dynamiclib -o glut.so glut.cpp -llua 
