#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <theme.h>
#include <string.h>
#include <vector>
#include <iostream>


//-----------------------------------------------------------------------------------------
// Person Class
//-----------------------------------------------------------------------------------------


struct Person {
        char name[128];
        bool checkedIn;
        bool paying;
};


//-----------------------------------------------------------------------------------------
// Person Class
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
            rect = {bufferSize, bufferSize, (float)GetScreenWidth()-((bufferSize*3)+(fontSize+10)+GetScreenWidth()/6), (float)fontSize+10};
        }
};
 
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
    int itemsInPersonStuct = 3;
    char checkedIn[128];
    char paying[128];
    int ogVSpace = (3*bufferSize)+(2*(fontSize+10));
    int vSpace = (3*bufferSize)+(2*(fontSize+10));

    // Search Bar
    bool searchButtonPressed = false;

    // People
    std::vector<Person> people = { {"Ethan Meyer", false, false}, {"Remy Eiser", false, false}, {"Logan Kocher", false, false} };

    Textbox searchBarInfo((int) 128, {bufferSize, bufferSize, (float)GetScreenWidth()-((bufferSize*3)+(fontSize+10)+GetScreenWidth()/6), (float)fontSize+10}, bufferSize, fontSize);

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
        }

        // Barcode Scanner
        //---------------------------------------------------------------------------------
        if (GuiButton({(bufferSize*2)+(GetScreenWidth()-((bufferSize*3)+(fontSize+10)+GetScreenWidth()/6)), bufferSize, (float)GetScreenWidth()/6-(bufferSize), (float)fontSize+10}, "BARCODE")) {

        }
        
        // People Handling
        //----------------------------------------------------------------------------------

        float totalBufferSizeX = bufferSize*(1+itemsInPersonStuct);

        GuiLabel({bufferSize+5, (bufferSize*2)+(fontSize+10), (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10}, "NAME");
        GuiLabel({(bufferSize*2)+5+((GetScreenWidth()-totalBufferSizeX)/3), (bufferSize*2)+(fontSize+10), (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10}, "PAID");
        GuiLabel({(bufferSize*3)+5+(((GetScreenWidth()-totalBufferSizeX)/3)*2), (bufferSize*2)+(fontSize+10), (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10}, "CHECKED IN");

        if (searchButtonPressed) {
            for ( Person& person:people ) {
                if (strcmp(searchBarInfo.text, person.name) == 0) {
                    DrawRectangleLines(bufferSize, (float)vSpace, (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10, WHITE);
                    GuiLabel({bufferSize+5, (float)vSpace, (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10}, person.name);
                    if (person.paying) {
                        strcpy(paying, "YES");
                    } else {
                        strcpy(paying, "NO");
                    }
                    if (person.checkedIn) {
                        strcpy(checkedIn, "YES");
                    } else {
                        strcpy(checkedIn, "NO");
                    }
                    DrawRectangleLines((bufferSize*2)+((GetScreenWidth()-totalBufferSizeX)/3), (float)vSpace, (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10, WHITE);
                    GuiLabel({(bufferSize*2)+5+((GetScreenWidth()-totalBufferSizeX)/3), (float)vSpace, (GetScreenWidth()-totalBufferSizeX)/3, (float)fontSize+10}, paying);
                    DrawRectangleLines((bufferSize*3)+(((GetScreenWidth()-totalBufferSizeX)/3)*2), (float)vSpace, ((GetScreenWidth()-totalBufferSizeX)/3)-(GetScreenWidth()/6), (float)fontSize+10, WHITE);
                    GuiLabel({(bufferSize*3)+5+(((GetScreenWidth()-totalBufferSizeX)/3)*2), (float)vSpace, ((GetScreenWidth()-totalBufferSizeX)/3)-(GetScreenWidth()/6), (float)fontSize+10}, checkedIn);
                    if (GuiButton({(bufferSize*3)+5+(((GetScreenWidth()-totalBufferSizeX)/3)*2)+((GetScreenWidth()-totalBufferSizeX)/3)-(GetScreenWidth()/6)+(bufferSize/2), (float)vSpace, ((GetScreenWidth()-totalBufferSizeX)/3)-(GetScreenWidth()/6), (float)fontSize+10}, "CHECK IN")) {
                        person.checkedIn = !person.checkedIn;
                    }
                }
            }
            vSpace = ogVSpace;
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