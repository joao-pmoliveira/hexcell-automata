@echo off

IF not exist .\.build (
    echo "Creating '.build' directory"
    mkdir .\.build
) else ( echo "Build directory found!" ) 

gcc main.c -o .\.build\Hexcell -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
