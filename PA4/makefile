CXX = g++
CXX_FLAGS = -w -std=c++14

TARGET = main
HEADERS = table.h
SRCS = main.cpp 

OBJECTS := $(patsubst %.cpp,%.o,$(SRCS))

#Rule that states that default all and clean are make commands and not associated with any files
.PHONY: default all clean

#Rule that defers make all to the TARGET rule
all: $(TARGET)

#Rule to compile a single object file
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

#Rule that makes all object files in the OBJECTS list, then links them all together to produce TARGET executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(OBJECTS) $(LIBS) -o $@

#Rule to clean up the build (removes iteratively all object files .o and the execitable TARGET)
clean:
	-rm -f *.o
	-rm -f $(TARGET)