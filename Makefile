outDir = out/
linkTarget = rtc

LIBS = -lSDL2

CFLAGS = -std=c++17 -Ofast

objects = main.o app.o $(patsubst %.cpp,%.o,$(wildcard ./lib/*.cpp))

rebuildables = $(objects) $(outDir)*

$(linkTarget): $(objects)
	mkdir -p $(outDir)
	g++ -o $(outDir)$(linkTarget) $(objects) $(LIBS) $(CFLAGS)

%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)	
	
.PHONEY:
clean:
	rm $(rebuildables)
