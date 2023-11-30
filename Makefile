TARGET=a.out
CXX=g++
DEBUG=-g
OPT=-O0
WARN=-wall
NCURSES=-lncurses -ltinfo
CxxFLAGS=$(DEBUG) $(OPT) $(WARN) $(NCURSES) -pipe
LD=g++
LDFLAGS=$(NCURSES)
OBJS= main.o mte.o
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@rm =rf *.o

main.o: main.cpp
	$(CXX) -c $(CxxFLAGS) main.cpp -o main.o

mte.o: mte.cpp
	$(CXX) -c $(CxxFLAGS) mte.cpp -o mte.o

clean:
	rm -f *.o

