CC=gcc
CFLAGS=-I./raylib-5.5_linux_amd64/include -L./raylib-5.5_linux_amd64/lib -l:libraylib.a -lm

main: main.c
	$(CC) -o main main.c $(CFLAGS)

clean:
	rm -f main

main-windows: main.c
	 x86_64-w64-mingw32-gcc -o game main.c -I./raylib-5.5_win64_mingw-w64/include -L./raylib-5.5_win64_mingw-w64/lib -l:libraylib.a -lm -lopengl32 -lgdi32 -lwinmm

game.wasm: main.c
	/mnt/DataFirst/programing/probe/repos/emsdk/upstream/emscripten/emcc main.c \
	  ./raylib-5.5_webassembly/lib/libraylib.a \
	  -I./raylib-5.5_webassembly/include \
	  -Os -Wall \
	  -DPLATFORM_WEB \
	  -s USE_GLFW=3 \
	  --shell-file shell.html \
	  -o game.html
