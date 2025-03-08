#include "external/glad.h"

#include "raylib.h"
#define SUPPORT_TRACELOG
#define SUPPORT_TRACELOG_DEBUG
#include "utils.h"
#include "raymath.h"
#include "reasings.h"  // Include Raylib's built-in easing functions

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <math.h>
#include <stdio.h>
#include <stdarg.h>

Vector2 fullHD = { 1920, 1080 };
bool imguiSpace = true;
float imguiSpaceAmount = 600;

float screenWidth = fullHD.x;
float screenHeight = fullHD.y;

const float ratio = screenWidth/screenHeight;

#include "stage.cpp"
#include "player.cpp"

typedef unsigned int GLuint;

void InitImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard
  io.DisplaySize = ImVec2((float)GetScreenWidth(), (float)GetScreenHeight());

  // ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)GetWindowHandle(), true);
  
  ImGui_ImplOpenGL3_Init("#version 130");    
}

void ProcessImGuiInput() {
  ImGuiIO& io = ImGui::GetIO();
    
  // Mouse
  io.MousePos = ImVec2(GetMouseX(), GetMouseY());
  io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
  io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
  io.MouseWheel += GetMouseWheelMove();  // Add mouse wheel movement to ImGui

  // Keyboard
  io.KeyCtrl = (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL));
  io.KeyShift = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
  io.KeyAlt = (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT));

  for (int i = 32; i < 127; i++) {  // Capture printable keys
    if (IsKeyPressed(i)) io.AddInputCharacter(i);
  }
}

void CheckResize() {

  if (IsKeyPressed(KEY_F9)) {
    TRACELOGD("CheckResize => F9");
    imguiSpace  = !imguiSpace;
    if (imguiSpace) SetWindowSize(fullHD.x+imguiSpaceAmount, fullHD.y);
    else SetWindowSize(fullHD.x, fullHD.y);
  }
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)GetScreenWidth(), (float)GetScreenHeight());
}

void PrintFloatValues() {
  TRACELOGD("screen:[width=%.1f, height=%.1f]", screenWidth, screenHeight);
  TRACELOGD("topBorder=%.1f | bottomBorder=%.1f", topBorder, bottomBorder);
  TRACELOGD("boxSize=%.1f", boxSize);
  TRACELOGD("playerSize=%.1f", playerSize);
}

int getWobble(int option, float time, float amount, float duration) {
  float wobbleOffset = 0.0;
  switch(option) {
  case 1: wobbleOffset = EaseSineIn(time, 0.0f, amount, duration); break;
  case 2: wobbleOffset = EaseSineOut(time, 0.0f, amount, duration); break;
  case 3: wobbleOffset = EaseSineInOut(time, 0.0f, amount, duration); break;
  case 4: wobbleOffset = EaseCircIn(time, 0.0f, amount, duration); break;
  case 5: wobbleOffset = EaseCircOut(time, 0.0f, amount, duration); break;
  case 6: wobbleOffset = EaseCircInOut(time, 0.0f, amount, duration); break;
  case 7: wobbleOffset = EaseCubicIn(time, 0.0f, amount, duration); break;
  case 8: wobbleOffset = EaseCubicOut(time, 0.0f, amount, duration); break;
  case 9: wobbleOffset = EaseCubicInOut(time, 0.0f, amount, duration); break;
  case 10: wobbleOffset = EaseQuadInOut(time, 0.0f, amount, duration); break;
  case 11: wobbleOffset = EaseQuadInOut(time, 0.0f, amount, duration); break;
  case 12: wobbleOffset = EaseQuadInOut(time, 0.0f, amount, duration); break;
  case 13: wobbleOffset = EaseExpoIn(time, 0.0f, amount, duration); break;
  case 14: wobbleOffset = EaseExpoOut(time, 0.0f, amount, duration); break;
  case 15: wobbleOffset = EaseExpoInOut(time, 0.0f, amount, duration); break;
  case 16: wobbleOffset = EaseBackIn(time, 0.0f, amount, duration); break;
  case 17: wobbleOffset = EaseBackOut(time, 0.0f, amount, duration); break;
  case 18: wobbleOffset = EaseBackInOut(time, 0.0f, amount, duration); break;
  case 19: wobbleOffset = EaseBounceIn(time, 0.0f, amount, duration); break;
  case 20: wobbleOffset = EaseBounceOut(time, 0.0f, amount, duration); break;
  case 21: wobbleOffset = EaseBounceInOut(time, 0.0f, amount, duration); break;
  case 22: wobbleOffset = EaseElasticIn(time, 0.0f, amount, duration); break;
  case 23: wobbleOffset = EaseElasticOut(time, 0.0f, amount, duration); break;
  case 24: wobbleOffset = EaseElasticInOut(time, 0.0f, amount, duration); break;
  }
    const char* easings[] = { "None", "SineIn", "SineOut", "SineInOut","CircIn", "CircOut", "CircInOut", "CubicIn", "CubicOut", "CubicInOut", "QuadIn", "QuadOut", "QuadInOut", "ExpoIn", "ExpoOut", "ExpoInOut", "BackIn", "BackOut", "BackInOut", "BounceIn", "BounceOut", "BounceInOut", "ElasticIn", "ElasticOut", "ElasticInOut" };  
  return wobbleOffset;
}

int main() {

  SetTraceLogLevel(LOG_DEBUG);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow(screenWidth+imguiSpaceAmount, screenHeight, "Cross!");
  SetWindowPosition(25, 50);

  SetTargetFPS(60);
  SetRandomSeed(GetTime());

  InitImGui();

  // Wobble Rectangle
  float startX = 350.0f;   // Initial X position
  float wobbleDuration = 0.5f;  // Wobble cycle duration (seconds)
  float wobbleAmount = 50.0f; // How much it moves left & right
  bool wForward = true;
  float wTime = 0.0f;


  int option = 0;
  bool show_demo_window = false;

  PrintFloatValues();
    
  InitStage();
  
  while (!WindowShouldClose()) {
    ProcessImGuiInput();  // Handle ImGui input
    CheckResize();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    
    if (IsKeyPressed(KEY_F10)) show_demo_window = !show_demo_window;
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
    if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
    
    float delta = GetFrameTime();

    // Wobble stuff
    wTime += (wForward ? delta : -delta);  // Move forward or backward

    if (wTime >= wobbleDuration) { wTime = wobbleDuration; wForward = false; }  // Reverse at end
    if (wTime <= 0.0f) { wTime = 0.0f; wForward = true; }  // Reverse at start

    if (IsKeyPressed(KEY_ZERO)) option = 0;
    if (IsKeyPressed(KEY_ONE)) option = 1;
    if (IsKeyPressed(KEY_TWO)) option = 2;
    if (IsKeyPressed(KEY_THREE)) option = 3;

    ImGui::Begin("Wobble Test");
    const char* easings[] = { "None", "SineIn", "SineOut", "SineInOut","CircIn", "CircOut", "CircInOut", "CubicIn", "CubicOut", "CubicInOut", "QuadIn", "QuadOut", "QuadInOut", "ExpoIn", "ExpoOut", "ExpoInOut", "BackIn", "BackOut", "BackInOut", "BounceIn", "BounceOut", "BounceInOut", "ElasticIn", "ElasticOut", "ElasticInOut" };

    ImGui::Combo("Easing", &option, easings, IM_ARRAYSIZE(easings));
    
    ImGui::DragFloat("duration:", &wobbleDuration, 0.1, 0.1, 5.0);
    ImGui::DragFloat("moveAmount:", &wobbleAmount, 0.1, 0.1, 100);
    ImGui::End();

    float wobbleOffset = getWobble(option, wTime, wobbleAmount, wobbleDuration);

    UpdateStage(delta);
    UpdatePlayer(delta);
    
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    DrawRectangle(startX + wobbleOffset, 250, 100, 100, RED);  // Wobbling rectangle
    DrawPlayer();
    DrawStage();


    // ImGui::SetCurrentContext(ImGui::GetCurrentContext());
    // ImGui window with player controls


    ImGui::Render(); // Render ImGui
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    EndDrawing();
  }

  ImGui_ImplOpenGL3_Shutdown();

  ImGui::DestroyContext();
  CloseWindow();
  return 0;
}
