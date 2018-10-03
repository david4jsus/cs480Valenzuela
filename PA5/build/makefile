# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS=-lSDL2 -lGLEW -lGL -lassimp
# Mac
else
	CC=clang++
	LIBS=-lSDL2 -framework OpenGL -framework Cocoa -lGLEW -stdlib=libc++
endif

#Paths, use this to set Include directories, or library directories; uncomment in all: if used
#PATHI=-I/usr/local/include/ 
#PATHL=-L/usr/local/lib/

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o Compilation
O_FILES=main.o camera.o engine.o graphics.o object.o shader.o window.o imgui.o imgui_demo.o imgui_draw.o imgui_widgets.o imgui_impl_opengl3.o imgui_impl_sdl.o# gl3w.o

# Point to includes of local directories
INDLUDES=-I../include

all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o Tutorial $(O_FILES) $(LIBS) #$(PATHI) $(PATHL)

main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) -c ../src/main.cpp -o main.o $(INDLUDES)

camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/camera.cpp -o camera.o $(INDLUDES)

engine.o: ../src/engine.cpp
	$(CC) $(CXXFLAGS) -c ../src/engine.cpp -o engine.o $(INDLUDES)

graphics.o: ../src/graphics.cpp
	$(CC) $(CXXFLAGS) -c ../src/graphics.cpp -o graphics.o $(INDLUDES)

object.o: ../src/object.cpp
	$(CC) $(CXXFLAGS) -c ../src/object.cpp -o object.o $(INDLUDES)

shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader.cpp -o shader.o $(INDLUDES)

window.o: ../src/window.cpp
	$(CC) $(CXXFLAGS) -c ../src/window.cpp -o window.o $(INDLUDES)

imgui.o: ../src/imgui.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui.cpp -o imgui.o $(INDLUDES)
	
imgui_demo.o: ../src/imgui_demo.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_demo.cpp -o imgui_demo.o $(INDLUDES)

imgui_draw.o: ../src/imgui_draw.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_draw.cpp -o imgui_draw.o $(INDLUDES)

imgui_widgets.o: ../src/imgui_widgets.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_widgets.cpp -o imgui_widgets.o $(INDLUDES)
	
imgui_impl_opengl3.o: ../src/imgui_impl_opengl3.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_impl_opengl3.cpp -o imgui_impl_opengl3.o $(INDLUDES)
	
imgui_impl_sdl.o: ../src/imgui_impl_sdl.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_impl_sdl.cpp -o imgui_impl_sdl.o $(INDLUDES)
	
#gl3w.o: ../src/gl3w.c
#	$(CC) $(CXXFLAGS) -c ../src/gl3w.c -o gl3w.o $(INDLUDES)

clean:
	-@if rm *.o Tutorial 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
