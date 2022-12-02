outDir := out/
linkTarget := rtc
pchSrc := pch.hpp
pchOut := pch.hpp.gch

CXX := clang++
CXXFLAGS := -std=c++17 -g -Wall -include $(pchSrc)
LDFLAGS := -lSDL2 -lSDL2_image

objects := $(patsubst %.cpp,%.o,$(wildcard ./*.cpp))
rebuildables := $(objects) $(outDir)* 

# link object files
$(outDir)$(linkTarget): $(objects) $(outDir)
	$(CXX) -o $(outDir)$(linkTarget) $(objects) $(LDFLAGS) $(CXXFLAGS)
	
# pre-compiled header
$(pchOut): $(pchSrc)
	$(CXX) -o $@ $< $(CXXFLAGS)

# compile each cpp file
%.o: %.cpp %.hpp $(pchOut)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# ensure there's a directory to put executable
$(outDir):
	mkdir -p $(outDir)

.PHONEY:
clean:
	rm $(rebuildables)
cleanall:
	rm $(rebuildables) $(pchOut)

release: CXXFLAGS += -Ofast
release: $(outDir)$(linkTarget)
