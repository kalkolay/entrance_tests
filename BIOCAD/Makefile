CXX=g++
CXXFLAGS=-std=c++17 -I.
DEPS = sources.h
OBJ = main.o
LIBS=-lOpenCL

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

biocad_test_task: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f biocad_test_task main.o