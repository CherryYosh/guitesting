CC=g++
CFLAGS= -g -D_DEBUG_ -Wall -DILUT_USE_OPENGL -I/usr/include/freetype2
LDFLAGS= -pg -lSDL -lGL -lGLU -lGLEW -lIL -lILU -lILUT -lfreetype

SRCDIR= src

SRC= $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp) $(wildcard $(SRCDIR)/*/*.c)
OBJS= $(patsubst %.cpp, %.o, $(SRC)) 

PROG= test

current: $(PROG)

$(PROG): $(OBJS)
	$(CC) -g $(OBJS) $(LDFLAGS) -o $(PROG) 


%.o: %.cpp %.h 
	$(CC) $(CFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)

run:
	./$(PROG)

debug:
	gdb $(PROG)

clean:
	rm */*.o */*/*.o
