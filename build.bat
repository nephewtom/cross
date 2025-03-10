del cross.exe
g++ main.cpp obj/*.o -o cross.exe -I./rlImGui/src -I ./rlImGui/imgui  -I ./rlImGui/imgui/backends -I ./raylib/src -L./rlImGui/src -L./raylib/src -lrlImGui -lraylib -lgdi32 -lwinmm -Wall -Wextra -Wno-missing-field-initializers -g
.\cross.exe

:: del delta.exe
:: g++ delta.c -o delta.exe -I raylib/src -L raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -Wextra -Wno-missing-field-initializers -g
::.\delta.exe

