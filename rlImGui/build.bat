imgui.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I../rlImGui -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/rlImGui/imgui.o" -MF "obj/x64/Debug/rlImGui/imgui.d" -c "../imgui-master/imgui.cpp"

imgui_demo.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I../rlImGui -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/rlImGui/imgui_demo.o" -MF "obj/x64/Debug/rlImGui/imgui_demo.d" -c "../imgui-master/imgui_demo.cpp"

imgui_draw.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I../rlImGui -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/rlImGui/imgui_draw.o" -MF "obj/x64/Debug/rlImGui/imgui_draw.d" -c "../imgui-master/imgui_draw.cpp"

imgui_tables.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I../rlImGui -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/rlImGui/imgui_tables.o" -MF "obj/x64/Debug/rlImGui/imgui_tables.d" -c "../imgui-master/imgui_tables.cpp"

imgui_widgets.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I../rlImGui -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/rlImGui/imgui_widgets.o" -MF "obj/x64/Debug/rlImGui/imgui_widgets.d" -c "../imgui-master/imgui_widgets.cpp"

rlImGui.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I../rlImGui -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/rlImGui/rlImGui.o" -MF "obj/x64/Debug/rlImGui/rlImGui.d" -c "../rlImGui.cpp"

Linking rlImGui
ar -rcs "../bin/Debug/rlImGui.lib"  obj/x64/Debug/rlImGui/imgui.o obj/x64/Debug/rlImGui/imgui_demo.o obj/x64/Debug/rlImGui/imgui_draw.o obj/x64/Debug/rlImGui/imgui_tables.o obj/x64/Debug/rlImGui/imgui_widgets.o obj/x64/Debug/rlImGui/rlImGui.o



simple.cpp
g++   -MMD -MP -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_WIN32 -I../raylib-master/src -I../raylib-master/src/external -I../raylib-master/src/external/glfw/include -I.. -I../imgui -I../imgui-master -m64 -g -std=c++17  -o "obj/x64/Debug/simple/simple.o" -MF "obj/x64/Debug/simple/simple.d" -c "../examples/simple.cpp"
Linking simple
g++ -o "../bin/Debug/simple.exe"  obj/x64/Debug/simple/simple.o   -L../bin/Debug -L/usr/lib64 -m64 ../bin/Debug/raylib.lib ../bin/Debug/rlImGui.lib -lwinmm -lgdi32
