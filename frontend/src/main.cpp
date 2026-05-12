#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "resources/theme.h"

//-----------------------------------------------------------------------------------------
// Main entry point
//-----------------------------------------------------------------------------------------
int main() {
    // Initialization
    //-------------------------------------------------------------------------------------
    const int screenWidth = 400;
    const int screenHeight = 400;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);
    GuiLoadStyleGenesis();

    // Variables
    //--------------------------------------------------------------------------------------

    // Color Scheme Defnition
    char mainTextBoxText[128] = { 0 };
    bool mainTextBoxFocus = false;
    bool showText = false;
    int fontSize = 20;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Variables that Require Updtae
        //----------------------------------------------------------------------------------
        Rectangle mainTextBoxRect = { 10, 10, GetScreenWidth() - 20.0, 30 };
        int mainTextBox = GuiTextBox(mainTextBoxRect, mainTextBoxText, 128, mainTextBoxFocus);


        // Drawing
        //----------------------------------------------------------------------------------
        BeginDrawing();
        GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

        if (mainTextBox) {
            mainTextBoxFocus = !mainTextBoxFocus;
        }

        if (IsKeyPressed(KEY_ENTER) && mainTextBox) {
            showText = !showText;
        }
        
        if (showText) {
            DrawText(mainTextBoxText, 15, 50, fontSize, WHITE);
        }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}   
