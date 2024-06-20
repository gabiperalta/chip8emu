cmake_minimum_required(VERSION 3.0)
project(chip8emu C)

set(CMAKE_C_STANDARD 11)

add_executable(chip8emu main.c
        video.h
        video.c
        cpu.c
        cpu.h)
