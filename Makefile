CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: rainforest

rainforest: rainforest.cpp log.cpp mrussell.cpp smerino.cpp dpham.cpp crodriguez3.cpp
	g++ $(CFLAGS) rainforest.cpp mrussell.cpp smerino.cpp crodriguez3.cpp dpham.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rainforest

clean:
	rm -f rainforest
	rm -f *.o

