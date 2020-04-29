CXX = g++ -std=c++17
LDFLAGS = -pthread -O3 #-fopenmp -ftree-vectorize -fopt-info-vec # -fopt-info-vec-missed
TARGETS = main

.PHONY: all clean cleanall
.SUFFIXES: .cpp

%: %.cpp
	$(CXX) -o $@ $< $(LDFLAGS)

all : $(TARGETS)
clean :
	rm -f $(TARGETS)
cleanall : clean
	\rm -f *.o *~ *.txt
