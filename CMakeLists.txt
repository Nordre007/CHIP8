cmake_minimum_required(VERSION 3.23)

project(CHIP8)

find_package(SDL2 REQUIRED)

add_executable(chip8 chip8.c main.c)