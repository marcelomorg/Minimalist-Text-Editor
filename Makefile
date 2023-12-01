TARGET=a.out
CXX=g++
DEBUG=-g
OPT=-O0
WARN=-Wall -Wno-unknown-pragmas
NCURSES=-lncurses -ltinfo
CXXFLAGS=$(DEBUG) $(OPT) $(WARN) $(NCURSES) -pipe
LD=g++
LDFLAGS=$(NCURSES)
OBJS= main.o mte.o
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@rm -rf *.o

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o main.o

mte.o: mte.cpp
	$(CXX) -c $(CXXFLAGS) mte.cpp -o mte.o

clean:
	rm -f *.o

