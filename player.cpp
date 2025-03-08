#include "imgui_log.cpp"

const float PLAYER_JUMP_SPEED = 700.0f;
const float PLAYER_MOVE_SPEED = 400.0f;
const float PLAYER_MOVE_SPEED_SLOW = 100.0f;

typedef struct Player {
  Vector2 position;
  Vector2 size;
  Vector2 speed;

  float rotation;
  bool dash;

  bool hitObstacle;
} Player;


float playerSize = round(screenWidth*0.041666);

Player player = { { margin*2, screenHeight/2 - playerSize/2},
                  { playerSize, playerSize },
                  { 0.0, 0.0},
                  0.0, false, false };

Vector2 direction = { 0.0, 0.0 };

Vector2 playerCenter;
Vector2 boxCenter;
Vector2 centerDistance;
Vector2 halfsPlayer;
Vector2 halfsItem;
float minDistX;
float minDistY;

Vector2 nextPosition;

void MaxAbsValue(Vector2 v, float *value, char *component) {
    if (fabsf(v.x) > fabsf(v.y)) {
        *value = v.x;
        *component = 'x';
    } else {
        *value = v.y;
        *component = 'y';
    }
}
void UpdatePlayer(float delta) {

  player.hitObstacle = false;

  // Input keys
  float speed = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)
    ? PLAYER_MOVE_SPEED_SLOW : PLAYER_MOVE_SPEED;
  direction.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
  direction.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

  // Calc player next Position according to its key presses
  nextPosition.x = round(player.position.x + direction.x * speed * delta);
  nextPosition.y = round(player.position.y + direction.y * speed * delta);

  // Check and avoid exiting stage borders
  if (nextPosition.x < leftBorder) nextPosition.x = leftBorder;
  if (nextPosition.y < topBorder) nextPosition.y = topBorder;
  if (nextPosition.y > bottomBorder - playerSize)
    nextPosition.y = bottomBorder - playerSize;
  
  // Loop obstables and check collisions
  for (int i=0; i<10; i++) {
    
    Rectangle rItem = boxes[i].rect;

    Rectangle playerRect = { nextPosition.x, nextPosition.y, player.size.x, player.size.y };

    
    if (CheckCollisionRecs(playerRect, rItem)) {

      player.hitObstacle = true;
      AddLog("-----");
      AddLogVector2("playerPosition", player.position);
      AddLogVector2("nextPosition  ", nextPosition);

      // Calc of centers of rectangles
      playerCenter = { round(playerRect.x + playerRect.width / 2),
                       round(playerRect.y + playerRect.height / 2) };
      AddLogVector2("   playerCenter", playerCenter);

      boxCenter = { round(rItem.x + rItem.width / 2),
                    round(rItem.y + rItem.height / 2) };
      AddLogVector2("   boxCenter   ", boxCenter);

      
      // Calc of the distance vector between the centers of the rectangles
      centerDistance = Vector2Subtract(playerCenter, boxCenter);
      AddLogVector2("   centerDistance", centerDistance);

      // Calc of half-widths and half-heights of rectangles
      halfsPlayer = { round(playerRect.width*.5f), round(playerRect.height*.5f) };
      halfsItem = { round(rItem.width*.5f), round(rItem.height*.5f) };
      // AddLogVector2("   halfsPlayer", halfsPlayer);
      // AddLogVector2("   halfsItem  ", halfsItem);

      // Calc of the minimum distance at which the two rectangles can be separated
      minDistX = halfsPlayer.x + halfsItem.x - fabsf(centerDistance.x);
      minDistY = halfsPlayer.y + halfsItem.y - fabsf(centerDistance.y);
      AddLog("minDistX=%.1f, minDistY=%.1f\n", minDistX, minDistY);
      
      // Adjusted object position based on minimum distance
      if (minDistX < minDistY) {
        AddLog("minDistX < minDistY\n");
        
        float copysignfDistX = copysignf(minDistX, centerDistance.x);
        nextPosition.x += copysignfDistX;
        AddLog("copysignfDistX:%.1f", copysignfDistX);
        AddLogVector2("finalPosition", nextPosition);
        if (abs(copysignfDistX) == 0.0) player.hitObstacle = false;

        player.speed.x = Clamp(100*copysignfDistX, 100, 500);
      } else {
        AddLog("minDistX >= minDistY\n");

        float copysignfDistY = copysignf(minDistY, centerDistance.y);
        nextPosition.y += copysignfDistY;
        AddLog("copysignfDistY:%.1fd", copysignfDistY);
        AddLogVector2("finalPosition", nextPosition);
        if (abs(copysignfDistY) == 0.0) player.hitObstacle = false;

        player.speed.y = Clamp(100*copysignfDistY, 100, 500);
      }

      if (player.hitObstacle)
        break;
    }
  }
  
  player.position.x = nextPosition.x;
  player.position.y = nextPosition.y; 

  if (IsKeyReleased(KEY_SPACE) && !player.dash) {
    player.speed.y = -PLAYER_JUMP_SPEED;
    player.dash = true;
  }
}


void DrawPlayer() {
  Rectangle rec = { player.position.x,
                    player.position.y,
                    player.size.x, player.size.y };
  DrawRectangleRec(rec, BLUE);

  ImGui::Begin("Player Information");
  ImGui::SeparatorText("Basic data");

  ImGui::Text("x"); ImGui::SameLine(150); ImGui::Text("y");

  ImGui::DragFloat2("position", (float*)&player.position, 1.0f, 0.0f, screenWidth);
  ImGui::DragFloat2("speed", (float*)&player.speed, 1.0f, 0.0f, 200.0f);
  ImGui::DragFloat2("direction", (float*)&direction, 1.0f, -1.0f, 1.0f);
  ImGui::Text("hitObstacle:"); ImGui::SameLine();
  if (player.hitObstacle) {
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "True");
  }
  else {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "False");
  }

  ImGui::SeparatorText("Collision stuff");

  ImGui::DragFloat2("centerDistance", (float*)&centerDistance, 1.0f, 0.0f, screenWidth);
  
  ImGui::Checkbox("Logs Enabled", &logsEnabled);
  ImGui::SameLine();
  ImGui::Checkbox("View Log Window", &viewLogWindow);
  
  ImGui::End();

  if (viewLogWindow) ShowLogWindow();
}
