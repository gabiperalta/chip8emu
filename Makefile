main: main.c video.c cpu.c
	gcc main.c video.c cpu.c -o chip8emu -lSDL2
