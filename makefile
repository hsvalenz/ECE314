linkTarget = main

#Define libraries that we need
LIBS = -lSDL2main -lSDL2
INCLUDES = -ISDL2/src/lib

# define flags 
# CFLAGS = -std=c++17 -pg
CFLAGS = -std=c++17
SDLFLAGS = `sdl2-config --cflags --libs`

# define the object files that we need to use
objects = main.o \
		  CApp.o \
		  $(patsubst %.cpp, %.o,$(wildcard ./SpectLib/*.cpp)) \
		  $(patsubst %.cpp, %.o, $(wildcard ./LIB2D/*.cpp))


# define the rebuildables
rebuildables = $(objects) $(linkTarget)

#Rule to actually perform the build
$(linkTarget): $(objects)
	@echo "Linking..."
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS) $(SDLFLAGS)

#Rule to create the .o files
%.o: %.cpp
	@echo "Compiling $<..."
	g++ -o $@ -c $< $(CFLAGS) $(SDLFLAGS)

# Phoney Target to clean up
.PHONEY: clean
clean:
	rm $(rebuildables)