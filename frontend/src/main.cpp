#include "C:/raylib/raylib/src/raylib.h"

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main() {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 400;
  const int screenHeight = 400;

  InitWindow(screenWidth, screenHeight, "raylib");
  //--------------------
  //------------------------------------------------------------------
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(GRAY);

    DrawRectangleRounded({10, 10, 380, 380}, 0.2, 0, BLUE);
    DrawRectangleRoundedLines({10, 10, 380, 380}, 0.2, 0, 5, DARKBLUE);

    EndDrawing();
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
