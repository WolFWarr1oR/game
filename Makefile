CC=gcc
CC-WIN=x86_64-w64-mingw32-gcc
CC-WEB=/mnt/DataFirst/programing/probe/repos/emsdk/upstream/emscripten/emcc
CFLAGS=-I./libs/raylib-5.5_linux_amd64/include -L./libs/raylib-5.5_linux_amd64/lib -l:libraylib.a -lm

all: build/ linux windows web

linux: build/linux/owolfwarrior_linux_x86-64

windows: build/windows/owolfwarrior_windows_x86-64.exe

web: build/web/game.wasm

build/:
	mkdir -p build/
	mkdir -p build/linux
	mkdir -p build/windows
	mkdir -p build/web

build/linux/owolfwarrior_linux_x86-64: main.c build/
	$(CC) -o build/linux/owolfwarrior_linux_x86-64 main.c $(CFLAGS)

build/windows/owolfwarrior_windows_x86-64.exe: main.c build/
	 $(CC-WIN) main.c \
	-I./libs/raylib-5.5_win64_mingw-w64/include \
	-L./libs/raylib-5.5_win64_mingw-w64/lib \
	-l:libraylib.a -lm -lopengl32 -lgdi32 -lwinmm \
	-o build/windows/owolfwarrior_windows_x86-64

build/web/game.wasm: main.c assets/shell.html
	$(CC-WEB) main.c \
	  ./libs/raylib-5.5_webassembly/lib/libraylib.a \
	  -I./libs/raylib-5.5_webassembly/include \
	  -Os -Wall \
	  -DPLATFORM_WEB \
	  -s USE_GLFW=3 \
	  --shell-file assets/shell.html \
	  -o build/web/game.html


clean:
	rm -fr build/*

