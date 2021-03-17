OBJDIR=obj/
SRCDIR=./
RM=rm -f 
SRCS=test.cpp pagetrack.cpp
OBJS=$(addprefix $(OBJDIR),$(subst .cpp,.o,$(SRCS)))
CXXFLAGS=-g -O0

all: test

test: $(OBJS) 
	$(CXX) $(CXXFLAGS)  -o test $(OBJS) 

$(OBJDIR)test.o: test.cpp pagetrack.h
	$(CXX) $(CXXFLAGS) -c test.cpp -o $(OBJDIR)test.o

$(OBJDIR)pagetrack.o: pagetrack.cpp pagetrack.h
	$(CXX) $(CXXFLAGS) -c pagetrack.cpp -o $(OBJDIR)pagetrack.o


clean:
	${RM} ${OBJS}
