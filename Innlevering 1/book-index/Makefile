binary = bookindex-test
folder = book-index
objects = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
cxxflags = -std=c++11 -g3

run: $(binary)
	./$(binary)

$(binary): $(objects)
	g++ $(cxxflags) -o $@ $^

%.o: %.cpp
	g++ $(cxxflags) -c -o $@ $<

clean:
	rm -f *.o

clear: clean
	rm -f *.zip $(binary) *.bmp

zip: clean
	zip $(folder) Makefile *.cpp *.h *.pxl
