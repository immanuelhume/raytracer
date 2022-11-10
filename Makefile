outDir = out/
linkTarget = rtc
pchSrc=lib/pch.hpp
pchOut=lib/pch.hpp.gch

LIBS = -lSDL2

CFLAGS = -std=c++17 -Ofast -Wall -H

objects = main.o app.o $(patsubst %.cpp,%.o,$(wildcard ./lib/*.cpp))

rebuildables = $(objects) $(outDir)* 

$(linkTarget): $(objects)
	mkdir -p $(outDir)
	g++ -o $(outDir)$(linkTarget) $(objects) $(LIBS) $(CFLAGS)
	
$(pchOut) : $(pchSrc)
	g++ -o $@ $< $(CFLAGS)

%.o: %.cpp $(pchOut)
	g++ -o $@ -c $< $(CFLAGS) -include $(pchSrc)

.PHONEY:
clean:
	rm $(rebuildables)
cleanall:
	rm $(rebuildables) $(pchOut)
