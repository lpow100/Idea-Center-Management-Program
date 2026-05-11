#include "C:/raylib/raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

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

    char buf[64] = "hello";
    bool editing = false;

    if (GuiTextBox({10, 10, 380, 13}, buf, 10, editing)) {
        editing = !editing;
    }

    EndDrawing();
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
