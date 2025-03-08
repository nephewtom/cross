:: g++ jump-ray.cpp -o jump-ray.exe -I raylib/src -L raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wextra -Wno-missing-field-initializers -g
del cross.exe
g++ cross.cpp obj/*.o -o cross.exe -I ./imgui -I ./imgui/backends -I ./raylib/src -L ./raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wextra -Wno-missing-field-initializers -g
.\cross.exe

:: del delta.exe
:: g++ delta.c -o delta.exe -I raylib/src -L raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wextra -Wno-missing-field-initializers -g
::.\delta.exe

