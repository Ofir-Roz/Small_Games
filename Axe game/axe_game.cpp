/**
 * @file axe_game.cpp
 * @author Ofir Rozanes (ofir19923@gmail.com)
 * @brief a very simple axe game for working a little with raylib game library
 * @version 0.1
 * @date 2022-01-01
 */
#include "raylib.h"

void runGame();

int main()
{
    runGame();
}

void runGame()
{
    // window dimensions
    int width{800};
    int height{450};
    InitWindow(width, height, "Ofir's Window!");

    // circle coordinates
    int circle_x{200};
    int circle_y{200};
    int circle_radius{25};
    // circle edges
    int l_circle_x{circle_x - circle_radius};
    int r_circle_x{circle_x + circle_radius};
    int u_circle_y{circle_y - circle_radius};
    int b_circle_y{circle_y + circle_radius};

    // axe coordinates
    int axe_x{400};
    int axe_y{0};
    int axe_length{50};
    // axe edges
    int l_axe_x{axe_x};
    int r_axe_x{axe_x + axe_length};
    int u_axe_y{axe_y};
    int b_axe_y{axe_y + axe_length};

    int direction{10};
    bool collision_with_axe = 
                        (b_axe_y >= u_circle_y) &&
                        (u_axe_y <= b_circle_y) &&
                        (l_axe_x <= r_circle_x) && 
                        (r_axe_x >= l_circle_x);


    SetTargetFPS(60);
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if (collision_with_axe)
        {
            DrawText("Game Over!", 250, 200, 60, RED);
        }
        else
        {   // game logic begins

            // update the edges
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            b_circle_y = circle_y + circle_radius;
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_length;
            // update collision with axe
            collision_with_axe = 
                        (b_axe_y >= u_circle_y) &&
                        (u_axe_y <= b_circle_y) &&
                        (l_axe_x <= r_circle_x) && 
                        (r_axe_x >= l_circle_x);

            DrawCircle(circle_x, circle_y, circle_radius, BLUE);
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);

            // move the axe
            axe_y += direction;
            if (axe_y > height || axe_y < 0)
            {
                direction = -direction;
            }
            

            if (IsKeyDown(KEY_D) && circle_x < width)
            {
                circle_x += 10;
            }
            if (IsKeyDown(KEY_A) && circle_x > 0)
            {
                circle_x -= 10;
            }
            
        }// game logic ends

        EndDrawing();
    }
}