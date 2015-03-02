
CC = g++
CFLAGS = -g -Wall
GLUTF = -lglut -l GLU 

SOURCE = src
.PHONY : lib
lib:
	$(MAKE) -C $(SOURCE) lib
cube14 :
	${CC} ${CFLAGS} cube14.cpp src/*.o -o exec/cube14
rcube :
	${CC} ${CFLAGS} randomCubes.cpp src/*.o -o exec/rcube
polygon:
	${CC} ${CFLAGS} polygon.cpp src/*.o -o exec/polygon
opagl:  pb1.cpp
	${CC} ${CFLAGS} pb1.cpp ${GLUTF} -o opagl
glplay: glplay.cpp
	${CC} ${CFLAGS} glplay.cpp ${GLUTF} -o glplay

clean:
	rm src/*.o
	rm exec/*
