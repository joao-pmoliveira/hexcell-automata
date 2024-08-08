@echo off

IF not exist .\.build (
    echo "Creating '.build' directory"
    mkdir .\.build
) else ( echo "Build directory found!" ) 

gcc main.c -o .\.build\Hexcell
