#include "imgui_log.cpp"

typedef struct Player {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Vector2 direction;
    bool normalizeDir;
    
    float highSpeed;
    float lowSpeed;
    float acceleration;
    float friction;
    
    float rotation;
    bool dash; // TODO

    bool hitObstacle;
} Player;


float playerSize = round(screenWidth*0.041666);

Player player = {
    .position = { margin*2, screenHeight/2 - playerSize/2},
    .size = { playerSize, playerSize },
    .velocity = { 0.0, 0.0},
    .direction = { 0.0, 0.0},
    .normalizeDir = false,
    .highSpeed = 800.0f,
    .lowSpeed = 200.0f,
    .acceleration = 800.0f, // How fast to reach max speed
    .friction = 800.0f, // Deceleration when no input
    .rotation = 0.0,
    .dash = false,
    .hitObstacle = false
};

typedef struct CollisionBundle {
    Vector2 playerCenter;
    Vector2 boxCenter;
    Vector2 centerDistance;
    Vector2 halfsPlayer;
    Vector2 halfsItem;
    float minDistX;
    float minDistY;    
} CollisionBundle;

CollisionBundle collisionBundle;

Vector2 nextPosition;

void MaxAbsValue(Vector2 v, float *value, char *component)
{
    if (fabsf(v.x) > fabsf(v.y)) {
        *value = v.x;
        *component = 'x';
    } else {
        *value = v.y;
        *component = 'y';
    }
}

void CheckPlayerCollision()
{

    for (int i=0; i<numOfBoxes; i++) {

        Rectangle rItem = boxes[i].rect;

        Rectangle playerRect = { nextPosition.x, nextPosition.y, player.size.x, player.size.y };


        if (CheckCollisionRecs(playerRect, rItem)) {

            CollisionBundle &cb = collisionBundle;
            player.hitObstacle = true;
            AddLog("-----");
            AddLogVector2("playerPosition", player.position);
            AddLogVector2("nextPosition  ", nextPosition);

            // Calc of centers of rectangles
            cb.playerCenter = { round(playerRect.x + playerRect.width / 2),
                round(playerRect.y + playerRect.height / 2)
            };
            AddLogVector2("   playerCenter", cb.playerCenter);

            cb.boxCenter = { round(rItem.x + rItem.width / 2),
                round(rItem.y + rItem.height / 2)
            };
            AddLogVector2("   boxCenter   ", cb.boxCenter);


            // Calc of the distance vector between the centers of the rectangles
            cb.centerDistance = Vector2Subtract(cb.playerCenter, cb.boxCenter);
            AddLogVector2("   centerDistance", cb.centerDistance);

            // Calc of half-widths and half-heights of rectangles
            cb.halfsPlayer = { round(playerRect.width*.5f), round(playerRect.height*.5f) };
            cb.halfsItem = { round(rItem.width*.5f), round(rItem.height*.5f) };
            // AddLogVector2("   halfsPlayer", halfsPlayer);
            // AddLogVector2("   halfsItem  ", halfsItem);

            // Calc of the minimum distance at which the two rectangles can be separated
            cb.minDistX = cb.halfsPlayer.x + cb.halfsItem.x - fabsf(cb.centerDistance.x);
            cb.minDistY = cb.halfsPlayer.y + cb.halfsItem.y - fabsf(cb.centerDistance.y);
            AddLog("minDistX=%.1f, minDistY=%.1f\n", cb.minDistX, cb.minDistY);

            // Adjusted object position based on minimum distance
            if (cb.minDistX < cb.minDistY) {
                AddLog("minDistX < minDistY\n");

                float copysignfDistX = copysignf(cb.minDistX, cb.centerDistance.x);
                nextPosition.x += copysignfDistX;
                AddLog("copysignfDistX:%.1f", copysignfDistX);
                AddLogVector2("finalPosition", nextPosition);
                if (abs(copysignfDistX) == 0.0) {
                    player.hitObstacle = false;
                } else {
                    player.velocity.x = 0.0;
                }
            } else {
                AddLog("minDistX >= minDistY\n");

                float copysignfDistY = copysignf(cb.minDistY, cb.centerDistance.y);
                nextPosition.y += copysignfDistY;
                AddLog("copysignfDistY:%.1fd", copysignfDistY);
                AddLogVector2("finalPosition", nextPosition);
                if (abs(copysignfDistY) == 0.0) {
                    player.hitObstacle = false;
                } else {
                    player.velocity.y = 0.0;
                }
            }

            if (player.hitObstacle) {
                break;
            }

        }
    }
}

void UpdatePlayer(float delta)
{

    player.hitObstacle = false;
    Vector2 &direction = player.direction;
    // Input keys
    float targetSpeed = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)
        ? player.lowSpeed : player.highSpeed;
    direction.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    direction.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

    // Normalize direction (to avoid diagonal speed boost)
    if (player.normalizeDir) {
        float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }
    }

    // Apply acceleration if moving
    if (direction.x != 0) {
        player.velocity.x += direction.x * player.acceleration * delta;
        if (fabsf(player.velocity.x) > targetSpeed)
            player.velocity.x = direction.x * targetSpeed;
    } else {
        // Apply friction (deceleration when no input)
        if (fabsf(player.velocity.x) > 0) {
            player.velocity.x -= player.friction * delta * (player.velocity.x > 0 ? 1 : -1);
            if (fabsf(player.velocity.x) < 5.0f) player.velocity.x = 0; // Stop completely at low speed
        }
    }

    if (direction.y != 0) {
        player.velocity.y += direction.y * player.acceleration * delta;
        if (fabsf(player.velocity.y) > targetSpeed)
            player.velocity.y = direction.y * targetSpeed;
    } else {
        if (fabsf(player.velocity.y) > 0) {
            player.velocity.y -= player.friction * delta * (player.velocity.y > 0 ? 1 : -1);
            if (fabsf(player.velocity.y) < 5.0f) player.velocity.y = 0;
        }
    }
    
// Calc player next Position according to its key presses
// nextPosition.x = round(player.position.x + direction.x * speed * delta);
// nextPosition.y = round(player.position.y + direction.y * speed * delta);

    nextPosition.x = round(player.position.x + player.velocity.x * delta);
    nextPosition.y = round(player.position.y + player.velocity.y * delta);

// Check and avoid exiting stage borders
    if (nextPosition.x < leftBorder) {
        nextPosition.x = leftBorder;
        player.velocity.x = 0;
    }
    if (nextPosition.y < topBorder) {
        nextPosition.y = topBorder;
        player.velocity.y = 0;
    }
    if (nextPosition.y > bottomBorder - playerSize) {
        nextPosition.y = bottomBorder - playerSize;
        player.velocity.y = 0;
    }

    CheckPlayerCollision();

    player.position.x = nextPosition.x;
    player.position.y = nextPosition.y;
}

void DrawPlayerImgui() {
    ImGui::Begin("Player Information");
    ImGui::SeparatorText("Position & velocity");

// ImGui::Text("x");
// ImGui::SameLine(150);
// ImGui::Text("y");

    ImGui::DragFloat2("position", (float*)&player.position, 1.0f, 0.0f, screenWidth);
    ImGui::DragFloat2("velocity", (float*)&player.velocity, 1.0f, 0.0f, 200.0f);
    ImGui::DragFloat2("direction", (float*)&player.direction, 1.0f, -1.0f, 1.0f);
    ImGui::Checkbox("normalize direction", &player.normalizeDir);
    ImGui::Spacing();
    ImGui::SeparatorText("Speed & acceleration");
    ImGui::DragFloat("high speed", (float*)&player.highSpeed, 1.0f, 200.0f, 2000.0f);
    ImGui::DragFloat("low speed", (float*)&player.lowSpeed, 1.0f, 50.0f, 200.0f);
    ImGui::DragFloat("acceleration", (float*)&player.acceleration, 1.0f, 700.0f, 1200.0f);
    ImGui::DragFloat("friction", (float*)&player.friction, 1.0f, 300.0f, 1000.0f);

    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SeparatorText("Collision stuff");
    ImGui::Text("hitObstacle:");
    ImGui::SameLine();
    if (player.hitObstacle) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "True");
    } else {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "False");
    }


    ImGui::DragFloat2("centerDistance", (float*)&collisionBundle.centerDistance, 1.0f, 0.0f, screenWidth);

    ImGui::Checkbox("Logs Enabled", &logsEnabled);
    ImGui::SameLine();
    ImGui::Checkbox("View Log Window", &viewLogWindow);

    ImGui::End();

    if (viewLogWindow) {
        ShowLogWindow();
    }

}

void DrawPlayer()
{
    Rectangle rec = { player.position.x,
        player.position.y,
        player.size.x, player.size.y
    };
    DrawRectangleRec(rec, BLUE);
    DrawPlayerImgui();
}
