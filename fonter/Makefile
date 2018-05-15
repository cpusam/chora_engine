CXX = g++
CXXFLAGS = -g --std=c++11
LDXXFLAGS = -I../ -L../ -lSDL2 -lSDL2_ttf ../libchora.a ./SDL_SavePNG/savepng.c -lpng

all: fonter fonterJSON

fonter: fonter.cpp
	$(CXX) -o fonter fonter.cpp $(LDXXFLAGS) $(CXXFLAGS)

fonterJSON: fonterJSON.cpp
	$(CXX) -o fonterJSON fonterJSON.cpp $(LDXXFLAGS) $(CXXFLAGS)
