CC=g++
CFLAGS= -g -fPIC -D_DEBUG_ -DGL_GLEXT_PROTOTYPES -Wall -DILUT_USE_OPENGL -I/usr/include/freetype2
LDFLAGS= -pg -lSDL -lGL -lGLU -lGLEW -lIL -lILU -lILUT -lfreetype -llua -lluabindd -lexpat

SRCDIR= src

SRC= $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp) $(wildcard $(SRCDIR)/*/*/*.cpp)
OBJS= $(patsubst %.cpp, %.o, $(SRC)) 

PROG= test

current: $(PROG)

$(PROG): $(OBJS) lua
	$(CC) -g $(OBJS) src/test.o $(LDFLAGS) -o $(PROG) 


%.o: %.cpp %.h 
	$(CC) $(CFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)

lua:
	cd src/; swig -c++ -lua test.i
	$(CC) $(CFLAGS) -c src/test_wrap.cxx -o src/test.o
	$(CC) $(LDFLAGS) -shared src/*.o src/lua/*.o src/gui/*.o src/renderer/ogl/*.o -o test.so

run:
	./$(PROG)

debug:
	gdb $(PROG)

clean:
	rm */*.o */*/*.o */*/*/*.o
