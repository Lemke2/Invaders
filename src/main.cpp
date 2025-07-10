#include <stdio.h>
#include "raylib.h"

void drawFPS(){
    int fps = GetFPS();
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "%d", fps);
    DrawText(fpsText, 0, 0, 20, RAYWHITE);
}

struct Position{
    int x;
    int y;
};

struct Chicken{
    int x;
    int y;
    int Health;
};

int main(void)
{   
    // metadata
    const int screenWidth = 1200;
    const int screenHeight = 900;
    const int PADDING = 100;
    const int maxAttacks = 100;
    const int chickenCount = 300;
    SetTargetFPS(144);
    bool victory = true;

    // Spaceship
    int health = 100;
    int shipWidth = 75;
    int shipHeight = 60;
    int x = screenWidth/2;
    int y = screenHeight - shipHeight;
    const int shipSpeed = 5;

    // Attacks
    Position attacks[maxAttacks] = {0};
    int currentAttack = 0;
    const int attackDamage = 25;
    const int attackSize = 2;
    const int attackSpeed = 10;

    // Chickens
    int chickenSize = 30;
    const int chickenSpeed = 1;
    const int chickenAttackSpeed = 2;
    Chicken chickens[chickenCount] = {0};
    int direction = 1;
    Position chickenAttacks[maxAttacks] = {0};
    int currentChickenAttack = 0;

    int currX = PADDING;
    int currY = 0;
    int rightBorderChicken = -1;
    for(int i = 0; i < chickenCount; i++){
        chickens[i].Health = 100;
        chickens[i].x = currX;
        chickens[i].y = chickenSize * currY;
        currX += chickenSize;
        if(currX >= screenWidth - PADDING){
            rightBorderChicken = i;
            currX = PADDING;
            currY++;
        }
    }

    InitWindow(screenWidth, screenHeight, "Lemke2 Invaders");
    Texture2D shipTex = LoadTexture("assets/spaceship.png");

    while (!WindowShouldClose())
    {
        victory = true;

        if (IsKeyDown(KEY_LEFT)) x -= shipSpeed;
        if (IsKeyDown(KEY_RIGHT)) x += shipSpeed;
        if (IsKeyDown(KEY_DOWN)) y += shipSpeed;
        if (IsKeyDown(KEY_UP)) y -= shipSpeed;

        if (x < 0) x = 0;
        if (x > screenWidth) x = screenWidth;
        if (y > screenHeight) y = screenHeight;
        if (y < 0) y = 0;
        
        if (IsKeyPressed(KEY_SPACE)){
            if(currentAttack >= maxAttacks) currentAttack = 0;
            attacks[currentAttack] = {x, y};
            currentAttack++;
        }

        if(chickens[0].x <= chickenSize || chickens[rightBorderChicken].x >= screenWidth - chickenSize){
            direction*=-1;
        }

        for(int i = 0; i < chickenCount; i++){
            chickens[i].x = chickens[i].x + (direction * chickenSpeed);
        }
        
        for(int i = 0; i < maxAttacks; i++){
            attacks[i].y -= attackSpeed;
            chickenAttacks[i].y += chickenAttackSpeed;
        }

        for(int i = 0; i < chickenCount; i++){
            int x = GetRandomValue(1, 10000);
            if(x == 500){
                if(currentChickenAttack >= maxAttacks) currentChickenAttack = 0;
                chickenAttacks[currentChickenAttack++] = {chickens[i].x, chickens[i].y};
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            drawFPS();
            
            DrawTextureEx(shipTex, (Vector2){(float)x - (shipWidth/2), (float)y - (shipHeight/2)}, 0.0f, 0.5f, WHITE);

            for (int i = 0; i < maxAttacks; i++){
                DrawRectangle(attacks[i].x, attacks[i].y, attackSize, attackSize, RED);
                DrawRectangle(chickenAttacks[i].x, chickenAttacks[i].y, attackSize, attackSize, RED);
            }

            for(int i = 0; i < chickenCount; i++){
                if(chickens[i].Health > 0){
                    victory = false;
                    DrawRectangle(chickens[i].x, chickens[i].y, chickenSize, chickenSize, RED);
                }
            }

            if(victory) DrawRectangle(screenHeight/2, screenWidth/2, 600, 200, BLUE);
            
        EndDrawing();
    }

    UnloadTexture(shipTex);
    CloseWindow();

    return 0;
}