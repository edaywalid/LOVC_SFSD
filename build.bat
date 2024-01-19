@REM gcc -c abstract.c -o object/abstract.o && gcc -c main.c -o object/main.o && gcc object/abstract.o object/main.o -o Debug/myprogram &&  .\Debug\myprogram
@REM gcc -o project main.c -Iinclude  -LWlib -lraylib -lopengl32 -lgdi32 -lwinmm
gcc -o Debug/myprogram main.c abstract.c -Iinclude -LWlib -lraylib -lopengl32 -lgdi32 -lwinmm && .\Debug\myprogram
@REM project.exe