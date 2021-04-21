OBJDIR=obj/
SRCDIR=./
TESTSRC=../testCPP/
RM=rm -f 
SRCS=test.cpp pagetrack.cpp
OBJS=$(addprefix $(OBJDIR),$(subst .cpp,.o,$(SRCS))) $(TESTSRC)testCPP.o
CXXFLAGS= -g  -pthread -I$(TESTSRC)


all: test

pt: pagetrack.cpp pagetrack.h
	$(CXX) -c pagetrack.cpp -o $(OBJDIR)pagetrack.o

test: $(OBJS) 
	$(CXX) -std=c++17 $(CXXFLAGS)  -o test $(OBJS) 

$(OBJDIR)test.o: test.cpp pagetrack.h $(TESTSRC)testCPP.h
	$(CXX) -std=c++17 $(CXXFLAGS) -c test.cpp -o $(OBJDIR)test.o

$(OBJDIR)pagetrack.o: pagetrack.cpp pagetrack.h
	$(CXX) $(CXXFLAGS) -c pagetrack.cpp -o $(OBJDIR)pagetrack.o

$(TESTSRC)testCPP.o: $(TESTSRC)testCPP.cpp $(TESTSRC)testCPP.h
	$(CXX) $(CXXFLAGS) -c $(TESTSRC)testCPP.cpp -o $(TESTSRC)testCPP.o

clean:
	${RM} ${OBJS}
