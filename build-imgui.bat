rmdir /s /q obj
mkdir obj
g++ -c ./imgui/imgui_draw.cpp ./imgui/imgui_tables.cpp ./imgui/imgui_widgets.cpp ./imgui/imgui_demo.cpp ./imgui/imgui.cpp ./imgui/backends/imgui_impl_opengl3.cpp -I ./imgui -I ./imgui/backends
rem g++ -c ./imgui/imgui_draw.cpp ./imgui/imgui_tables.cpp ./imgui/imgui_widgets.cpp ./imgui/imgui_demo.cpp ./imgui/imgui.cpp ./imgui/backends/imgui_impl_glfw.cpp -I ./imgui -I ./imgui/backends -I./raylib/src/external/glfw/include/
move *.o obj

