
TRGS = bfs

LIBS += -lpthread -lrt

CXXFLAGS = -O3
CXXFLAGS += -Wall

all: $(TRGS)

bfs: bfs.cc
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TRGS) *.o *.dot
	rm -f myfile.txt

