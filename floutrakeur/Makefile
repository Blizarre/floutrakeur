CXXFLAGS+=-O3 -std=c++11 -fopenmp
INCLUDES_DIR=
LIBS=-lgomp -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_contrib -lboost_system -lboost_filesystem
CXX=g++

all: s

s: main
	./main testSharp/  out.dummy -1

b: main
	./main testBlurry/  out.dummy 1

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES_DIR) $< -o $@

main: main.o fileFinder.o csvWriter.o filtersMetrics.o
	$(CXX) $^ $(LIBS) -o $@

clean:
	rm *.o main

