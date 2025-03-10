#include "raylib.h"
#define SUPPORT_TRACELOG
#define SUPPORT_TRACELOG_DEBUG
#include "utils.h"
#include "raymath.h"
#include "reasings.h"  // Include Raylib's built-in easing functions

#include "imgui.h"
#include "rlImGui.h"

#include <math.h>
#include <stdio.h>
#include <stdarg.h>

Vector2 fullHD = { 1920, 1080 };
float screenWidth = fullHD.x;
float screenHeight = fullHD.y;
bool fullScreen = false;
const float ratio = screenWidth/screenHeight;

#include "stage.cpp"
#include "player.cpp"
#include "wobble.c"

Font font;
const char* menu_paused = "    MENU PAUSED\nWaiting for input!\n    0123456789!";
typedef enum CurrentScreen { MENU = 0, GAME = 1} CurrentScreen;
CurrentScreen currentScreen = GAME;
Vector2 fontPosition = { 420, 400 };

typedef struct ImGuiConf {
    bool space; // Extra space to locate ImGui windows
    float spaceAmount;
    bool show_demo_window;
} ImGuiConf;

ImGuiConf imguiConf = {
    .space = true,
    .spaceAmount = 600,
    .show_demo_window = false,
};

void CheckResize()
{
    if (!fullScreen && IsKeyPressed(KEY_F9)) {
        TRACELOGD("CheckResize => F9");
        imguiConf.space  = !imguiConf.space;
        if (imguiConf.space) {
            SetWindowSize(fullHD.x+imguiConf.spaceAmount, fullHD.y);
        } else {
            SetWindowSize(fullHD.x, fullHD.y);
        }
    }
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)GetScreenWidth(), (float)GetScreenHeight());
}

void PrintFloatValues()
{
    TRACELOGD("screen:[width=%.1f, height=%.1f]", screenWidth, screenHeight);
    TRACELOGD("topBorder=%.1f | bottomBorder=%.1f", topBorder, bottomBorder);
    TRACELOGD("boxSize=%.1f", boxSize);
    TRACELOGD("playerSize=%.1f", playerSize);
}


void MenuScreen();
void GameScreen();
void UpdateGame();

int main()
{
    SetTraceLogLevel(LOG_DEBUG);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(imguiConf.space ? screenWidth + imguiConf.spaceAmount : screenWidth, screenHeight, "Cross!");
    SetWindowPosition(25, 50);
    PrintFloatValues();

    SetTargetFPS(60);
    SetRandomSeed(time(nullptr));

    font = LoadFontEx("caveman.ttf", 48, 0, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    rlImGuiSetup(true);

    InitStage();

    while (!WindowShouldClose()) {

        CheckResize();

        if (IsKeyPressed(KEY_F5)) {
            if (currentScreen == GAME) {currentScreen = MENU;}
            else {currentScreen = GAME;}
        }
        if (currentScreen == MENU) {
            MenuScreen();
            continue;
        }
        
        GameScreen();
    }
    rlImGuiShutdown();

    CloseWindow();
    return 0;
}

void MenuScreen() {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawTextEx(font, menu_paused, fontPosition, font.baseSize*2.0f, 0, BLUE);
    EndDrawing();
}

void GameScreen() {
    BeginDrawing();
    rlImGuiBegin();

    UpdateGame();

    ClearBackground(RAYWHITE);
    DrawPlayer();
    DrawStage();

    rlImGuiEnd();
    EndDrawing();

}

void UpdateGame() {

    if (IsKeyPressed(KEY_F10)) {
        imguiConf.show_demo_window = !imguiConf.show_demo_window;
    }
    if (imguiConf.show_demo_window) {
        ImGui::ShowDemoWindow(&imguiConf.show_demo_window);
    }
    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
        fullScreen != fullScreen;
    }

    if (IsKeyPressed(KEY_B)) {
        ClearStage();
        numOfBoxes = GetRandomValue(3, 12);
        InitStage();
    }
            
    float delta = GetFrameTime();

    wt.offset = wobbleTest(delta);
        
    UpdateStage(delta);
    UpdatePlayer(delta);
}
