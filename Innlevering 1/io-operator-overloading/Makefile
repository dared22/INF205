binary = io-operator-overloading
compiler = g++ -g3 -c
data = 
folder = io-operator-overloading
input = 
linker = g++ -g3
objects = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
params = 


run: $(binary)
	./$(binary) $(params)

$(binary): $(objects)
	$(linker) -o $@ $^

%.o: %.cpp
	$(compiler) -o $@ $<

clean:
	rm -f *.o

clear: clean
	rm -f *.zip $(binary)

zip: clean
	zip $(folder) Makefile *.cpp *.h $(data)
