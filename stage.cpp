const float margin = round(screenWidth*0.021875f);
const float leftBorder = margin;
const float topBorder = margin;
const float bottomBorder = screenHeight - margin;

struct Wobble {
  float time;
  float duration;
  bool forward;  // right direction
};

struct Box {
  Rectangle rect;
  int direction;
  float speed;
};

struct Stage {
  Rectangle left;  
  Rectangle top;  
  Rectangle bottom;  
  Rectangle finish;

  Box boxes[10] = {};
};

Stage stage = {
  .left =   { 0, 0, margin, screenHeight },
  .top =    { 0, 0, screenWidth, margin },
  .bottom = { 0, screenHeight - margin, screenWidth, screenHeight - margin },
  .finish = { screenWidth - 3*margin, margin, margin, screenHeight-2*margin + 1},
  .boxes = { }
};

Box *boxes = stage.boxes;
float boxSize = round(screenWidth*0.033333f);
float boxesBegin = margin*5;
float boxesEnd = screenWidth - margin*5;

void InitStage() {

  float extent = boxesEnd - boxesBegin;
  for (int i=0; i<10; i++) {

    int direction = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    boxes[i].direction = (float) direction;

    float speed = 10.0f + (float)GetRandomValue(0, 19000) / 100.0f;
    if (i == 0) speed = 0.5f;
    boxes[i].speed = speed;
    TRACELOGD("box[%d].speed = %.1f", i, speed);
    
    Rectangle& rect = boxes[i].rect;
    rect.x = boxesBegin + i*0.1f*extent;
    rect.y = GetRandomValue((int)margin, (int) (screenHeight - margin - boxSize));
    rect.width = boxSize;
    rect.height = boxSize;
    // TraceLog(LOG_DEBUG, "box[%d].y = %f", i, rect.y);
    TRACELOGD("box[%d].y = %.1f", i, rect.y);
  }
}

void UpdateStage(float delta) {
  for (int i=0; i<10; i++) {
    Rectangle& rect = boxes[i].rect;

    // Change direction at the borders
    float distance = boxes[i].direction * boxes[i].speed * delta;
    if ((rect.y + distance < margin) || rect.y + distance > screenHeight - margin - boxSize) {
      boxes[i].direction = -boxes[i].direction;
      distance = boxes[i].direction * boxes[i].speed * delta;
    }
     
    rect.y += distance;

    
  }
}

void DrawStage() {
  Rectangle left = stage.left;
  Rectangle top = stage.top;
  Rectangle bottom = stage.bottom;
  Rectangle finish = stage.finish;
  
  DrawRectangleRec(left, GRAY);
  DrawRectangle(top.x, top.y, top.width, top.height, GRAY);
  DrawRectangle(bottom.x, bottom.y, bottom.width, bottom.height, GRAY);
  DrawRectangle(finish.x, finish.y, finish.width, finish.height, GREEN);

  DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, RED);
  DrawLine(boxesBegin, margin, boxesBegin, screenHeight - margin, RED);
  DrawLine(boxesEnd, margin, boxesEnd, screenHeight - margin, RED);

  for (int i=0; i<10; i++) {
    Rectangle rect = boxes[i].rect;
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLACK);
  }
}
