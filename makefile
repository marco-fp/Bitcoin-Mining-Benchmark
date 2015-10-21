#FLAGS
CXX = g++
CPPFLAGS= -Wall -O2 -std=c++11

INC=include
SRC=src
BIN=bin
RESOURCES=resources


.PHONY: all
all: $(BIN)/ $(INC)/sha256.h $(BIN)/sha256_benchmark

$(BIN)/sha256_benchmark: $(SRC)/sha256.cpp $(SRC)/main.cpp
	$(CXX) -o $@ $^  -I $(INC)/ $(CPPFLAGS)
	cp $(RESOURCES)/* $(BIN)

$(BIN)/:
	mkdir $(BIN)

#cleans the project
.PHONY: clean
clean:
	rm -r -f $(BIN)  $(SRC)/*~ $(INC)/*~
