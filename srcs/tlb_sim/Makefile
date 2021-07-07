CXX=g++
RM=rm -f
SRCS=test.cpp tlb.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
CXXFLAGS= -g
all: test

test: $(OBJS) 
	$(CXX) $(CXXFLAGS) -o test $(OBJS) 

test.o: test.cpp tlb.o

tlb.o: tlb.cpp tlb.h

clean:
	${RM} ${OBJS}
