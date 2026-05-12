#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <theme.h>
#include <httplib.h>
#include <string.h>

//-----------------------------------------------------------------------------------------
// Main entry point
//-----------------------------------------------------------------------------------------
int main() {
    // Initialization
    //-------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 540;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "FS-ID");
    SetWindowIcon(LoadImage("resources/ideaCenterLogo.png"));
    SetTargetFPS(60);
    GuiLoadStyleGenesis();

    // Variables
    //--------------------------------------------------------------------------------------

    // Basic
    int fontSize = 20;
    float bufferSize = 10.0f;

    // Search Bar
    bool searchButtonPressed = false;

    // Textbox Class
    //-----------------------------------------------------------------------------------------

    class Textbox {
        public:
            char text[128];
            char pastText[128];
            bool focus;
            Rectangle rect;
            int charNum;
            float bufferSize;
            int fontSize;

            Textbox(int charNum, Rectangle rect, float bufferSize, int fontSize) {
                this->rect = rect;
                this->charNum = charNum;
                this->bufferSize = bufferSize;
                this->fontSize = fontSize;
                memset(text, 0, 128);
                memset(pastText, 0, 128);
            }

            void Update() {
                if (text != NULL) strcpy(pastText, text);
                rect = {bufferSize, bufferSize, (float)GetScreenWidth()-((bufferSize*2)+(fontSize+20)), (float)fontSize+10};
            }
    };

    Textbox searchBarInfo((int) 128, {bufferSize, bufferSize, (float)GetScreenWidth()-((bufferSize*2)+(fontSize+10)), (float)fontSize+10}, bufferSize, fontSize);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        //----------------------------------------------------------------------------------
        // Drawing
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);


        // Main Textbox Focusing Statement
        //----------------------------------------------------------------------------------
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), searchBarInfo.rect)) {
                searchBarInfo.focus = true;
            } else {
                searchBarInfo.focus = false;
            }
        }


        if (GuiTextBox(searchBarInfo.rect, searchBarInfo.text, searchBarInfo.charNum, searchBarInfo.focus)) {
            searchBarInfo.focus = false;
            searchButtonPressed = true;
        }


        // Main Textbox Searching
        //----------------------------------------------------------------------------------
        if (GuiButton({GetScreenWidth()-(bufferSize+(fontSize+10)), bufferSize, (float)fontSize+10, (float)fontSize+10}, ">")) {
            searchButtonPressed = true;
        } if (strcmp(searchBarInfo.pastText, searchBarInfo.text) != 0 && searchButtonPressed == true) {
            searchButtonPressed = false;
        } if (searchButtonPressed) {
            GuiLabel({bufferSize+5, (bufferSize*2)+(fontSize+10), GetScreenWidth()-(bufferSize*2), (float)fontSize+10}, searchBarInfo.text);
        }

        EndDrawing();

        searchBarInfo.Update();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}   
