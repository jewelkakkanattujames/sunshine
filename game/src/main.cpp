
// AI LAB 1


#include "rlImGui.h" // Includes the functionality provided by ImGui and Raylib libraries.

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    // Vector2 ballPosition = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };


    // moving ball 
    Vector2 ballSpeed = { 5.0f, 4.0f }; // speed definition 
    int ballRadius = 20; //size
    Vector2 ballPosition = { GetScreenWidth() - ballRadius * 2, GetScreenHeight() - ballRadius * 2 }; // position
    bool pause = 0;
    int framesCounter = 0;


    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sunshine");

    Vector2 start = { 0, 0 };
    Vector2 end = { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };


    //Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = WHITE;

    // circle that change colour on collision  - the circle that is stationed at the centre of the screen
    Vector2 stationaryCirclePos = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    const float stationaryCircleRadius = 50.0f;
    Color stationaryCircleColor = GREEN;

    // the circle that will move with the cursor
    Vector2 controlledCirclePos = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    const float controlledCircleRadius = 30.0f;
    Color controlledCircleColor = BLUE;


    Vector2 controlledBallPosition = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    Vector2 controlledBallSpeed = { 0.0f, 0.0f };
    int controlledBallRadius = 30;
    Color controlledBallColor = DARKGREEN;

    // Define circle properties .a colour filled circle 
    Vector2 circleCenter = { SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100 };
    float circleRadius = 100;
    Color circleColor = PINK;


    SetTargetFPS(60);// 60 frames per second 

    // InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - input mouse wheel");
    int boxPositionY = SCREEN_HEIGHT / 2 - 130;
    int scrollSpeed = 4;            // Scrolling speed in pixels




    // First Triangle .These are the properties if the first triangle
    Vector2 vertex1 = { 0, 0 };
    Vector2 vertex2 = { 0, 300 };
    Vector2 vertex3 = { 150, 0 };
    Color triangleColor = BLACK;

    // Second Triangle.These are the properties if the second triangle
    Vector2 vertex4 = { 200, 150 };
    Vector2 vertex5 = { 300, 100 };
    Vector2 vertex6 = { 100, 50 };

    // properties of the rectangle 
    Rectangle rectangle; 
    rectangle.x = 0;
    rectangle.y = SCREEN_HEIGHT - 100;  // Position the rectangle at the bottom of the screen
    rectangle.width = SCREEN_WIDTH;
    rectangle.height = 100;  // Set the height of the rectangle

    // Define hexagon properties
    Vector2 hexagonCenter = { SCREEN_WIDTH - 200, SCREEN_HEIGHT - 300 }; // Position at the center of the screen
    float hexagonRadius = 100;
    Color hexagonColor = BLUE;

    Color rectangleColor = VIOLET; // colour of the rectangle 


    while (!WindowShouldClose())
    {

        
        {
            // Update


            if (IsKeyPressed(KEY_SPACE)) pause = !pause;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) start = GetMousePosition();
            else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) end = GetMousePosition();

            // bouncing ball
            if (!pause)
            {
                ballPosition.x += ballSpeed.x;
                ballPosition.y += ballSpeed.y;

                // Check walls collision for bouncing
                if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
                if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;


            }
            else framesCounter++;

            boxPositionY -= (GetMouseWheelMove() * scrollSpeed);
            //----------------------------------------------------------------------------------


            controlledCirclePos = GetMousePosition();

            controlledCirclePos.x += (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * 2.0f;
            controlledCirclePos.y += (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP)) * 2.0f;

            // Check collision between circles
            bool isCollision = CheckCollisionCircles(stationaryCirclePos, stationaryCircleRadius,
                controlledCirclePos, controlledCircleRadius);

            // Change color based on collision
            if (isCollision)
            {
                stationaryCircleColor = BLUE;
                controlledCircleColor = BLACK;
            }
            else
            {
                stationaryCircleColor = RED;
                controlledCircleColor = GREEN;
            }

            DrawCircleV(stationaryCirclePos, stationaryCircleRadius, stationaryCircleColor);
            DrawCircleV(controlledCirclePos, controlledCircleRadius, controlledCircleColor);



            // /*  if (IsKeyPressed(KEY_SPACE)) pause = !pause;

              // if (!pause)
             //  {
                         //ballPosition.x += ballSpeed.x;
                //   ballPosition.y += ballSpeed.y;

                   // Check walls collision for bouncing
                 //  if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
                 //  if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;
              // }
            ////   else framesCounter++;


            // controlling the circle using the arrow keys.
            if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
            if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
            if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
            if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;

            BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(SCREEN_WIDTH / 2 - 40, boxPositionY, 80, 80, MAROON);

            DrawText("Use mouse wheel to move the cube up and down!", 10, 10, 20, GRAY);
            DrawText(TextFormat("Box position Y: %03i", boxPositionY), 20, 40, 20, LIGHTGRAY);

            DrawText("USE MOUSE LEFT-RIGHT CLICK to DEFINE LINE START and END POINTS", 15, 20, 20, GRAY);

            DrawLineBezier(start, end, 2.0f, RED);

            DrawCircleV(ballPosition, 40, ballColor);

            DrawCircleV(ballPosition, 50, MAROON);

            DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25, 20, LIGHTGRAY);

            // Draw the rectangle
            DrawRectangleRec(rectangle, rectangleColor);

            // On pause, we draw a blinking message
            if (pause && ((framesCounter / 30) % 2))
                DrawText("PAUSED", 350, 200, 30, GRAY);

            DrawFPS(10, 10);

            //  DrawText("move ball with mouse and click mouse button to change color", 10, 10, 20, DARKGRAY);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawCircleV(controlledCirclePos, controlledCircleRadius, controlledCircleColor);

            DrawCircleV(controlledCirclePos, stationaryCircleRadius, stationaryCircleColor);
            DrawCircleV(controlledCirclePos, controlledCircleRadius, controlledCircleColor);

            DrawCircleV(ballPosition, 50, MAROON);

            // Draw the first triangle
            DrawTriangle(
                vertex1,
                vertex2,
                vertex3,
                triangleColor
            );

            // Draw the second triangle
            DrawTriangle(
                vertex4,
                vertex5,
                vertex6,
                triangleColor
            );

            // Draw the color-filled circle
            DrawCircle(circleCenter.x, circleCenter.y, circleRadius, circleColor);

            // Draw the color-filled hexagon
            DrawPoly(hexagonCenter, 6, hexagonRadius, 0, hexagonColor);

          
            EndDrawing();

        }

    }

    CloseWindow();
    return 0;
}