#include "raylib.h"
#include "raymath.h"

#define G 900
#define PLAYER_JUMP_SPD 500.0f
#define PLAYER_HOR_SPD 200.0f

typedef struct Player {
  Vector2 position;
  Vector2 speed;
  bool canJump;
  float k;
} Player;

typedef struct EnvItem {
  Rectangle rect;
  int blocking;
  Color color;
} EnvItem;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
int foo(int x, int y);
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);

void UpdateCameraCenter(Camera2D *camera, Player *player, int width, int height, float, EnvItem *, int );

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, int width, int height, float delta, EnvItem *envItems, int envItemsLength);
void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, int width, int height, float delta, EnvItem *envItems, int envItemsLength);
void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, int width, int height, float delta, EnvItem *envItems, int envItemsLength);
void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, int width, int height, float delta, EnvItem *envItems, int envItemsLength);


bool timing = false;
double startTime = 0.0;
double elapsedTime = 0.0;

float EaseOutQuad(float k) {
  return 1.0f - (1.0f - k) * (1.0f - k);
}

float EaseInOutSmooth(float k) {
  return k * k * (3 - 2 * k);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "delta");

  Player player = { 0 };
  player.position = (Vector2){ 400, 280 };
  player.speed = { 0.0, 0.0 };
  player.canJump = false;
  EnvItem envItems[] = {
    {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
    {{ -1000, 400, 2000, 200 }, 1, GREEN },
    {{ 300, 200, 400, 10 }, 1, GRAY },
    {{ 250, 300, 100, 10 }, 1, GRAY },
    {{ 650, 300, 100, 10 }, 1, GRAY }
  };

  int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

  Camera2D camera = { 0 };
  camera.target = player.position;
  camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // Store pointers to the multiple update camera functions
  void (*cameraUpdaters[])(Camera2D*, Player*, int, int, float, EnvItem*, int) = {
    UpdateCameraCenter,
    UpdateCameraCenterInsideMap,
    UpdateCameraCenterSmoothFollow,
    UpdateCameraEvenOutOnLanding,
    UpdateCameraPlayerBoundsPush
  };

  int cameraOption = 0;
  int cameraUpdatersLength = sizeof(cameraUpdaters)/sizeof(cameraUpdaters[0]);

  char *cameraDescriptions[] = {
    (char*)"Follow player center",
    (char*)"Follow player center, but clamp to map edges",
    (char*)"Follow player center; smoothed",
    (char*)"Follow player center horizontally; update player center vertically after landing",
    (char*)"Player push camera on getting too close to screen edge"
  };

  SetTargetFPS(60);
  //--------------------------------------------------------------------------------------

  float wobbleTime = 0.0f;
  // Main game loop
  while (!WindowShouldClose())
    {
      // Update
      //----------------------------------------------------------------------------------
      float deltaTime = GetFrameTime();
      wobbleTime += deltaTime;
      UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

      camera.zoom += ((float)GetMouseWheelMove()*0.05f);

      if (camera.zoom > 3.0f) camera.zoom = 3.0f;
      else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

      if (IsKeyPressed(KEY_R))
        {
          camera.zoom = 1.0f;
          player.position = (Vector2){ 400, 280 };
        }

      if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1)%cameraUpdatersLength;

      // Call update camera function by its pointer
      cameraUpdaters[cameraOption](&camera, &player, screenWidth, screenHeight, deltaTime, envItems, envItemsLength);
      //----------------------------------------------------------------------------------

      if (timing) {
        elapsedTime = GetTime() - startTime;
      }
        
      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

      ClearBackground(LIGHTGRAY);

      BeginMode2D(camera);

      for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);

      float PWIDTH = 40.0f;
      float PWIDTH_MIN = 20.0f;
      float PWIDTH_MAX = 60.0f;
      float PHEIGHT = 40.0f;
      float PHEIGHT_MIN = 20.0f;
      float PHEIGHT_MAX = 60.0f;
      float rectWidth = PWIDTH;
      float rectHeight = PHEIGHT;
                
      if (player.speed.y != 0.0) {
                  
        if (player.speed.y < 0.0) {
          player.k = (PLAYER_JUMP_SPD + player.speed.y)/PLAYER_JUMP_SPD;
          // k is 0.0 when jump starts and 1.0 when speed is 0.0 (top of the jump)
          player.k = EaseOutQuad(player.k);
          /* player.k = EaseInOutSmooth(player.k); */
          rectWidth = PWIDTH - PWIDTH_MIN*player.k;
          rectHeight = PHEIGHT + (PHEIGHT_MAX - PHEIGHT)*player.k;

        }
        if (player.speed.y > 0.0) {
          player.k = (PLAYER_JUMP_SPD - player.speed.y)/PLAYER_JUMP_SPD;
          player.k = EaseOutQuad(player.k);
          if (player.k < 0.15) {
            rectWidth = PWIDTH_MAX - rectWidth*player.k;
            rectHeight = PHEIGHT_MIN + (PHEIGHT - PHEIGHT_MIN)*player.k;
          }
          else {
            rectWidth = PWIDTH - PWIDTH_MIN*player.k;
            rectHeight = PHEIGHT_MIN + (PHEIGHT_MAX - PHEIGHT_MIN)*player.k;
          }
        }
      }

      if (player.speed.x != 0.0f) {
        float wobbleWidth = 3.0f;
        float wobbleOffset = Clamp(wobbleWidth*sin(wobbleTime * 3.0f), -wobbleWidth, wobbleWidth);  // Wobble effect
        rectWidth += EaseOutQuad(wobbleOffset); 
      }
      Rectangle playerRect = {
        player.position.x - rectWidth/2.0f, player.position.y - rectHeight,
        rectWidth, rectHeight
      };
      
      DrawRectangleRec(playerRect, RED);
      DrawLine(player.position.x, player.position.y, player.position.x, player.position.y - 40.0f, BLUE);
                
      DrawCircleV(player.position, 5.0f, GOLD);



      EndMode2D();

      DrawText("Controls:", 20, 20, 10, BLACK);
      DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
      DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
      DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
      DrawText("- C to change camera mode", 40, 100, 10, DARKGRAY);
      DrawText("Current camera mode:", 20, 120, 10, BLACK);
      DrawText(cameraDescriptions[cameraOption], 40, 140, 10, DARKGRAY);
            
      DrawText(TextFormat("Delta: %.2f", deltaTime), 20, 160, 10, RED);
      DrawText(TextFormat("JumpTime: %.2f", elapsedTime), 20, 180, 10, RED);
      DrawText(TextFormat("Speed: %.2f", player.speed.y), 20, 200, 10, RED);
      DrawText(TextFormat("Position: [%.2f, %.2f]", player.position.x, player.position.y), 20, 220, 10, RED);
      DrawText(TextFormat("k: %.2f", player.k), 20, 240, 10, RED);


      EndDrawing();
      //----------------------------------------------------------------------------------
    }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();        // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
  if (IsKeyDown(KEY_A)) {
    player->speed.x = PLAYER_HOR_SPD;
    player->position.x -= player->speed.x*delta;
  }
  if (IsKeyDown(KEY_D)) {
    player->speed.x = PLAYER_HOR_SPD;
    player->position.x += player->speed.x*delta;
  }
  if (IsKeyReleased(KEY_A)) {
    player->speed.x = 0.0f;    
  }
  if (IsKeyReleased(KEY_D)) {
    player->speed.x = 0.0f;    
  }

  
  if (IsKeyDown(KEY_SPACE) && player->canJump)
    {
      player->speed.y = -PLAYER_JUMP_SPD;
      player->canJump = false;
        
      timing = true;
      startTime = GetTime();
    }

  bool hitObstacle = false;
  for (int i = 0; i < envItemsLength; i++)
    {
      EnvItem *ei = envItems + i;
      Vector2 *p = &(player->position);
      if (ei->blocking &&
          ei->rect.x <= p->x &&
          ei->rect.x + ei->rect.width >= p->x &&
          ei->rect.y >= p->y &&
          ei->rect.y <= p->y + player->speed.y*delta)
        {
          hitObstacle = true;
          player->speed.y = 0.0f;
          p->y = ei->rect.y;
          break;
        }
    }

  if (!hitObstacle)
    {
      player->position.y += player->speed.y*delta;
      player->speed.y += G*delta;
      player->canJump = false;
    }
  else {
    if (timing) {
      elapsedTime = GetTime() - startTime;
      timing = false;
    }
    player->canJump = true;
  }
}

int foo(int x, int) {
  return 5*x;
}
void UpdateCameraCenter(Camera2D *camera, Player *player, int width, int height, float , EnvItem*, int)
{
  camera->offset = (Vector2){ width/2.0f, height/2.0f };
  camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, int width, int height, float , EnvItem *envItems, int envItemsLength)
{
  camera->target = player->position;
  camera->offset = (Vector2){ width/2.0f, height/2.0f };
  float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

  for (int i = 0; i < envItemsLength; i++)
    {
      EnvItem *ei = envItems + i;
      minX = fminf(ei->rect.x, minX);
      maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
      minY = fminf(ei->rect.y, minY);
      maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

  Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
  Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

  if (max.x < width) camera->offset.x = width - (max.x - width/2);
  if (max.y < height) camera->offset.y = height - (max.y - height/2);
  if (min.x > 0) camera->offset.x = width/2 - min.x;
  if (min.y > 0) camera->offset.y = height/2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, int width, int height, float delta, EnvItem *, int )
{
  static float minSpeed = 30;
  static float minEffectLength = 10;
  static float fractionSpeed = 0.8f;

  camera->offset = (Vector2){ width/2.0f, height/2.0f };
  Vector2 diff = Vector2Subtract(player->position, camera->target);
  float length = Vector2Length(diff);

  if (length > minEffectLength)
    {
      float speed = fmaxf(fractionSpeed*length, minSpeed);
      camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}

void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, int width, int height, float delta, EnvItem *, int )
{
  static float evenOutSpeed = 700;
  static int eveningOut = false;
  static float evenOutTarget;

  camera->offset = (Vector2){ width/2.0f, height/2.0f };
  camera->target.x = player->position.x;

  if (eveningOut)
    {
      if (evenOutTarget > camera->target.y)
        {
          camera->target.y += evenOutSpeed*delta;

          if (camera->target.y > evenOutTarget)
            {
              camera->target.y = evenOutTarget;
              eveningOut = 0;
            }
        }
      else
        {
          camera->target.y -= evenOutSpeed*delta;

          if (camera->target.y < evenOutTarget)
            {
              camera->target.y = evenOutTarget;
              eveningOut = 0;
            }
        }
    }
  else
    {
      if (player->canJump && (player->speed.y == 0) && (player->position.y != camera->target.y))
        {
          eveningOut = 1;
          evenOutTarget = player->position.y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, int width, int height, float , EnvItem *, int )
{
  static Vector2 bbox = { 0.2f, 0.2f };

  Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
  Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
  camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

  if (player->position.x < bboxWorldMin.x) camera->target.x = player->position.x;
  if (player->position.y < bboxWorldMin.y) camera->target.y = player->position.y;
  if (player->position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
  if (player->position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}
