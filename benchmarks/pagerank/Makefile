
TRGS = pagerank 

LIBS += -lpthread -lrt

CXXFLAGS =  --std=c++0x -O3
CXXFLAGS += -Wall -Werror

all: $(TRGS)

pagerank: pagerank.cc read_graph.h read_graph.cpp
	$(CXX) $(CXXFLAGS) $< read_graph.cpp -o $@ $(LIBS)


clean:
	rm -f $(TRGS) *.o *.dot
	rm -f file.txt



