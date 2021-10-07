
TRGS = sssp

LIBS += -lpthread -lrt

##BOOST_PATH = /run/pkg/boost-/1.53.0-gcc_4.7.3

##BOOST_FLAGS = -I$(BOOST_PATH)/include -L$(BOOST_PATH)/lib -lboost_graph

CXXFLAGS = --std=c++0x -O3
CXXFLAGS += -Wall -Werror

all: $(TRGS)

sssp: sssp_outer_atomic.cc read_graph.cpp read_graph.h
	$(CXX) $(CXXFLAGS) $< read_graph.cpp -o $@ $(LIBS)

clean:
	rm -f $(TRGS) *.o *.dot
	rm -f myfile.txt

