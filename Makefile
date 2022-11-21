outDir := out/
linkTarget := rtc
pchSrc := pch.hpp
pchOut := pch.hpp.gch

CXX := g++
CXXFLAGS := -std=c++17 -Ofast -Wall -include $(pchSrc)
LDFLAGS := -lSDL2 -lSDL2_image

objects = $(patsubst %.cpp,%.o,$(wildcard ./*.cpp))

$(outDir)$(linkTarget): $(objects) $(outDir)
	$(CXX) -o $(outDir)$(linkTarget) $(objects) $(LDFLAGS) $(CXXFLAGS)
	
$(pchOut): $(pchSrc)
	$(CXX) -o $@ $< $(CXXFLAGS)

%.o: %.cpp %.hpp $(pchOut)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(outDir):
	mkdir -p $(outDir)

rebuildables := $(objects) $(outDir)* 

.PHONEY:
clean:
	rm $(rebuildables)
cleanall:
	rm $(rebuildables) $(pchOut)
