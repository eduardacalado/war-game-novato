cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.c
        war.c
        war.h
        torre.c
        torre.h
        tetris.c
        tetris.h
        detective.c
        detective.h)
