#include "raylib.h"

#define screenWidth 800
#define screenHeight 460

void drawSoftRectangle(int position_x, int position_y, int size_x, int size_y, float roundness, Color color) {
    DrawRectangleRounded(
        (Rectangle){ position_x, position_y, size_x, size_y },
        roundness,
        0,
        color
    );
}
void drawLineBetweenRectangles(Rectangle rect1, Rectangle rect2) {
    // Calculate the center points of the rectangles
    Vector2 point1 = {rect1.x + rect1.width , rect1.y + rect1.height/2 };
    Vector2 point2 = {rect2.x , rect2.y + rect2.height/2 };

    // Draw a line between the centers of the rectangles
    DrawLineV(point1, point2, RED);
}
void search_visualisation(int x) {
    // Initialization
    InitWindow(screenWidth, screenHeight, "Search Visualization");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update

        // Draw
        BeginDrawing();

        ClearBackground(SKYBLUE);  // Clear the background
        int y=20;
        Rectangle rect[5];
        
        for (int i = 0; i < x; i++) {
            rect[i] =(Rectangle) { (i * screenWidth / 5)+10, 460/2, 100, 100 };
            y++;
            //DrawRectangleRec(rect[i], WHITE);
            drawSoftRectangle((i * screenWidth / 5)+10,460/2,100,100,0.5f,RED);
        }
        drawLineBetweenRectangles(rect[0], rect[1]);
        drawLineBetweenRectangles(rect[1], rect[2]);
        drawLineBetweenRectangles(rect[2], rect[3]);
        drawLineBetweenRectangles(rect[3], rect[4]);
        
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();  // Close window and OpenGL context
}




int main(void) {
   search_visualisation(5);  // You can change the parameter as needed
      InitWindow(screenWidth, screenHeight, "Line Between Rectangles Example");

    SetTargetFPS(60);

    
    // while (!WindowShouldClose()) {
    //     // Update

    //     // Draw
    //     BeginDrawing();
        
    //     ClearBackground(RAYWHITE);  // Clear the background

    //     // Draw rectangles
       
    //     // Draw line between rectangles
        

    //     EndDrawing();
    // }

    // De-Initialization
    CloseWindow();  // Close window and OpenGL context

    return 0;
}
   
