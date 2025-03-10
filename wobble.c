typedef struct WobbleTest {
    int option;
    float duration;  // Wobble cycle duration (seconds)
    float amount; // How much it moves left & right
    bool forward;
    float time;

    float startX;
    float offset;
} WobbleTest;

WobbleTest wt = {
    .option = 0,
    .duration = 0.5f,
    .amount = 50.0f,
    .forward = true,
    .time = 0.0f,
    
    .startX = 350.0f,
    .offset = 0.0f,
};

const char* easingsText[] = { "None", "SineIn", "SineOut", "SineInOut","CircIn", "CircOut", "CircInOut", "CubicIn", "CubicOut", "CubicInOut", "QuadIn", "QuadOut", "QuadInOut", "ExpoIn", "ExpoOut", "ExpoInOut", "BackIn", "BackOut", "BackInOut", "BounceIn", "BounceOut", "BounceInOut", "ElasticIn", "ElasticOut", "ElasticInOut" };

typedef float (*EaseFunction)(float, float, float, float);
EaseFunction easingFunctions[] = {
    NULL, // No easing for index 0 (to match 1-based option)
    EaseSineIn, EaseSineOut, EaseSineInOut,
    EaseCircIn, EaseCircOut, EaseCircInOut,
    EaseCubicIn, EaseCubicOut, EaseCubicInOut,
    EaseQuadInOut, EaseQuadInOut, EaseQuadInOut, // Same function repeated
    EaseExpoIn, EaseExpoOut, EaseExpoInOut,
    EaseBackIn, EaseBackOut, EaseBackInOut,
    EaseBounceIn, EaseBounceOut, EaseBounceInOut,
    EaseElasticIn, EaseElasticOut, EaseElasticInOut
};

float wobbleTest(float delta) {

    wt.time += (wt.forward ? delta : -delta);  // Move forward or backward

    if (wt.time >= wt.duration) {
        wt.time = wt.duration;    // Reverse at end
        wt.forward = false;
    }
    if (wt.time <= 0.0f) {
        wt.time = 0.0f;    // Reverse at start
        wt.forward = true;
    }
    
    if (IsKeyPressed(KEY_F1)) { wt.option = 0; }
    if (IsKeyPressed(KEY_F2)) { wt.option = 3; }
    if (IsKeyPressed(KEY_F3)) { wt.option = 21; }
    if (IsKeyPressed(KEY_F4)) { wt.option = 24; }

    ImGui::Begin("Wobble Test");

    ImGui::Text("option:"); ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), TextFormat("%d", wt.option));
    ImGui::Combo("Easing", &wt.option, easingsText, IM_ARRAYSIZE(easingsText));
            
    ImGui::DragFloat("duration:", &wt.duration, 0.1, 0.1, 5.0);
    ImGui::DragFloat("moveAmount:", &wt.amount, 0.1, 0.1, 100.0);
    ImGui::End();

    if (wt.option < 1 || wt.option > 24) { return 0.0; }

    return easingFunctions[wt.option](wt.time, 0.0f, wt.amount, wt.duration);
}

void DrawWobbleTest() {
    DrawRectangle(wt.startX + wt.offset, 250, 100, 100, RED);  // Wobbling rectangle
}
