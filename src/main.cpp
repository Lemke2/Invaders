#include <stdio.h>
#include "raylib.h"

void drawFPS(){
    int fps = GetFPS();
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "%d", fps);
    DrawText(fpsText, 0, 0, 20, RAYWHITE);
}

bool checkCollision(int x1, int y1, int size1, int x2, int y2, int size2){
    bool collision = false;
    if ((x1 < (x2 + size2) && (x1 + size1) > x2) && (y1 < (y2 + size2) && (y1 + size1) > y2))
        collision = true;

    return collision;
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
    const int screenWidth = 1200;
    const int screenHeight = 900;
    const int PADDING = 100;
    const int maxAttacks = 100;
    const int chickenCount = 300;
    SetTargetFPS(144);
    bool victory = true;

    int health = 100;
    int shipWidth = 75;
    int shipHeight = 60;
    int x = screenWidth/2;
    int y = screenHeight - shipHeight;
    const int shipSpeed = 5;

    Position attacks[maxAttacks] = {0};
    int currentAttack = 0;
    const int attackDamage = 25;
    const int attackSize = 2;
    const int attackSpeed = 10;

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
        
        if (IsKeyDown(KEY_SPACE)){
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
            for(int j = 0; j < chickenCount; j++){
                if(checkCollision(attacks[i].x, attacks[i].y, attackSize, chickens[j].x, chickens[j].y, chickenSize)){
                    attacks[i].x = -100;
                    attacks[i].y = -100;
                    chickens[j].Health -=attackDamage;
                    if(chickens[j].Health <= 0){
                        chickens[j].x = -200;
                        chickens[y].x = -200;
                    }
                    break;
                }
            }

            if(checkCollision(x, y, shipHeight, chickenAttacks[i].x, chickenAttacks[i].y, attackSize)){
                health-=10;
                chickenAttacks[i].x = - 50;
                chickenAttacks[i].y = -50;
                printf("DAMAAGE TAKEN! CURRENT HEALTH: %d\n", health);
            }

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

            DrawRectangle(0, screenHeight-20, ((health/100) * screenWidth), 20, RED);

            if(health <= 0) DrawRectangle(screenHeight/2, screenWidth/2, 600, 200, RED);

            if(victory) DrawRectangle(screenHeight/2, screenWidth/2, 600, 200, BLUE);
            
        EndDrawing();
    }

    UnloadTexture(shipTex);
    CloseWindow();

    return 0;
}