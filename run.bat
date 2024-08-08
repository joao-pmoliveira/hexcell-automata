@echo off

IF not exist .\.build (
    echo "Expected directory '.build' not found at root."
    exit 1
)

IF not exist .\.build\Hexcell.exe (
    echo "Executable not found: 'Hexcell.exe'"
    exit 1
)

.\.build\Hexcell.exe

