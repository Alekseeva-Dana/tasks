CXX = g++
CXXFLAGS = -Wall -g
TARGET = sql
OBJS = main.o scanner.o parser.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp scanner.h parser.h
	$(CXX) $(CXXFLAGS) -c main.cpp

scanner.o: scanner.cpp scanner.h
	$(CXX) $(CXXFLAGS) -c scanner.cpp

parser.o: parser.cpp parser.h scanner.h
	$(CXX) $(CXXFLAGS) -c parser.cpp

clean:
	 rm -f $(OBJS) $(TARGET)
