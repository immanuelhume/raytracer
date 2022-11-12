outDir := out/
linkTarget := rtc
pchSrc := lib/pch.hpp
pchOut := lib/pch.hpp.gch

CXX := g++
CXXFLAGS := -std=c++17 -Ofast -Wall -include $(pchSrc)
LDFLAGS := -lSDL2 -lSDL2_image

objects = main.o app.o $(patsubst %.cpp,%.o,$(wildcard ./lib/*.cpp))

$(outDir)$(linkTarget): $(objects) $(outDir)
	$(CXX) -o $(outDir)$(linkTarget) $(objects) $(LDFLAGS) $(CXXFLAGS)
	
$(pchOut): $(pchSrc)
	$(CXX) -o $@ $< $(CXXFLAGS)

%.o: %.cpp %.hpp $(pchOut)
	$(CXX) -o $@ -c $< $(CXXFLAGS) -include $(pchSrc)

$(outDir):
	mkdir -p $(outDir)

rebuildables := $(objects) $(outDir)* 

.PHONEY:
clean:
	rm $(rebuildables)
cleanall:
	rm $(rebuildables) $(pchOut)
