
TARGETS=executable/main.exe

OBJECTS=src/GLFWInput.o \
src/ParticleManager.o \
src/ParticleSystem.o \
src/ParticleTexture.o \
src/ShaderManager.o \
src/XMLParser.o \
src/tga.o

CC=gcc
AR=ar
#GDB=-g

CPPFLAGS=-std=c++11 -Iinclude/ -Wall -s
LFLAGS=-lglfw3 -lglew32 -lopengl32 -lgdi32 -lxerces-c -lstdc++

all: src/main.cpp $(OBJECTS)
	$(CC) $(GDB) $(CPPFLAGS) -o $(TARGETS) $< $(OBJECTS) $(LFLAGS)
	
%.o: %.cpp
	$(CC) $(GDB) $(CPPFLAGS) -c $< -o $@
	
clean:
	rm $(subst /,\,$(OBJECTS))
	rm $(subst /,\,$(TARGETS))