#include <stdio.h>
#include "raylib.h"

void drawFPS(){
    int fps = GetFPS();
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "%d", fps);
    DrawText(fpsText, 0, 0, 20, RAYWHITE);
}

bool checkCollision(int x1, int y1, float width1, float height1, int x2, int y2, float width2, float height2){
    return 
    (x1 < x2 + width2) && 
    (x1 + width1 > x2) && 
    (y1 < y2 + height2) && 
    (y1 + height1 > y2);
}

struct Attack{
    Texture2D texture;
    float scale;
    float x;
    float y;
    int damage;
    int speed;
    float width;
    float height;
};

struct GameObject{
    int health;
    Texture2D texture;
    float scale;
    float x;
    float y;
    float width;
    float height;
    int speed;
};

int main(void)
{   
    // Metadata
    const int screenWidth = 1200;
    const int screenHeight = 900;
    const int PADDING = 100;
    const int maxAttacks = 100;
    const int chickenCount = 150;
    SetTargetFPS(144);
    bool victory = true;

    InitWindow(screenWidth, screenHeight, "Lemke2 Invaders");

    // Ship
    const Texture2D shipTex = LoadTexture("assets/spaceship.png");
    const float shipScale = 0.5f;
    const float shipWidth = shipTex.width * shipScale;
    const float shipHeight = shipTex.height * shipScale;
    GameObject SHIP = {
        .health = 100,
        .texture = shipTex, 
        .scale = shipScale,
        .x = screenWidth/2, 
        .y = screenHeight - shipHeight, 
        .width = shipWidth,
        .height = shipHeight, 
        .speed = 5
    };

    // Attacks
    Attack attacks[maxAttacks] = {0};
    const int attackDamage = 25;
    const int attackSize = 2;
    const int attackSpeed = 10;
    int currentAttack = 0;
    for (int i = 0; i < maxAttacks; i++) {
        attacks[i].scale = 1.0f;
        attacks[i].damage = attackDamage;
        attacks[i].speed = attackSpeed;
        attacks[i].width = attackSize;
        attacks[i].height = attackSize;
        attacks[i].x = -100;
    }

    // Chickens
    const Texture2D chickenTex = LoadTexture("assets/chicken.png");
    GameObject chickens[chickenCount] = {0};
    const float chickenScale = 0.1f;
    const float chickenWidth = chickenTex.width * chickenScale;
    const float chickenHeight = chickenTex.height * chickenScale;
    const int chickenSpeed = 1;
    int direction = 1;

    int currX = PADDING;
    int currY = 0;
    int rightBorderChicken = -1;
    for(int i = 0; i < chickenCount; i++){
        chickens[i].texture = chickenTex;
        chickens[i].scale = chickenScale;
        chickens[i].width = chickenWidth;
        chickens[i].height = chickenHeight;
        chickens[i].health = 100;
        chickens[i].x = currX;
        chickens[i].y = chickenHeight * currY;
        chickens[i].speed = chickenSpeed;
        
        currX += chickens[i].width;
        if(currX >= screenWidth - PADDING){
            rightBorderChicken = i;
            currX = PADDING;
            currY++;
        }
    }

    // Chicken attacks
    const Texture2D eggTex = LoadTexture("assets/egg.png");
    Attack chickenAttacks[maxAttacks] = {0};
    for (int i = 0; i < maxAttacks; i++) {
        chickenAttacks[i].texture = eggTex;
        chickenAttacks[i].scale = 0.1f;
        chickenAttacks[i].damage = 10;
        chickenAttacks[i].speed = 1;
        chickenAttacks[i].width = eggTex.width * 0.1f;
        chickenAttacks[i].height = eggTex.height * 0.1f;
        chickenAttacks[i].x = -100;
    }
    int currentChickenAttack = 0;

    // Game loop
    while (!WindowShouldClose())
    {
        victory = true;

        // Move ship
        if (IsKeyDown(KEY_LEFT)) SHIP.x -= SHIP.speed;
        if (IsKeyDown(KEY_RIGHT)) SHIP.x += SHIP.speed;
        if (IsKeyDown(KEY_DOWN)) SHIP.y += SHIP.speed;
        if (IsKeyDown(KEY_UP)) SHIP.y -= SHIP.speed;

        if (SHIP.x < 0) SHIP.x = 0;
        if (SHIP.x > screenWidth - SHIP.width) SHIP.x = screenWidth - SHIP.width;
        if (SHIP.y > screenHeight - (SHIP.height/2)) SHIP.y = screenHeight - (SHIP.height/2);
        if (SHIP.y < 0) SHIP.y = 0;
        
        if (IsKeyDown(KEY_SPACE)){
            if(currentAttack >= maxAttacks) currentAttack = 0;
            attacks[currentAttack].x = SHIP.x + (shipWidth/2);
            attacks[currentAttack].y = SHIP.y;

            currentAttack++;
        }

        // Move chickens
        if(chickens[0].x <= chickenWidth || chickens[rightBorderChicken].x >= screenWidth - chickenWidth){
            direction*=-1;
        }
        for(int i = 0; i < chickenCount; i++){
            chickens[i].x = chickens[i].x + (direction * chickens[i].speed);
        }
        
        // Attack collisions
        for(int i = 0; i < maxAttacks; i++){
            for(int j = 0; j < chickenCount; j++){
                if(checkCollision(attacks[i].x, attacks[i].y, attacks[i].width, attacks[i].height, 
                                    chickens[j].x, chickens[j].y, chickens[j].width, chickens[j].height)){
                    attacks[i].x = -100;
                    attacks[i].y = -100;
                    chickens[j].health -= attacks[i].damage;
                    if(chickens[j].health <= 0){
                        chickens[j].y = screenHeight + 200;
                    }
                    break;
                }
            }

            if(checkCollision(SHIP.x, SHIP.y, SHIP.width, SHIP.width, 
                                chickenAttacks[i].x, chickenAttacks[i].y, chickenAttacks[i].width, chickenAttacks[i].height)){
                SHIP.health-=10;
                chickenAttacks[i].x = - 50;
                chickenAttacks[i].y = -50;
                printf("DAMAAGE TAKEN! CURRENT HEALTH: %d\n", SHIP.health);
            }

            attacks[i].y -= attacks[i].speed;
            chickenAttacks[i].y += chickenAttacks[i].speed;
        }

        // Generate chicken attacks
        for(int i = 0; i < chickenCount; i++){
            if(chickens[i].health > 0){
                int x = GetRandomValue(1, 10000);
                if(x == 500){
                    if(currentChickenAttack >= maxAttacks) currentChickenAttack = 0;
                    chickenAttacks[currentChickenAttack].x = chickens[i].x;
                    chickenAttacks[currentChickenAttack].y = chickens[i].y;
                    currentChickenAttack++;
                }
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            drawFPS();
            
            DrawTextureEx(SHIP.texture, (Vector2){(float)SHIP.x, (float)SHIP.y}, 0.0f, SHIP.scale, WHITE);

            for (int i = 0; i < maxAttacks; i++){
                DrawRectangle(attacks[i].x, attacks[i].y, attackSize, attackSize, RED);
                DrawTextureEx(eggTex, (Vector2){chickenAttacks[i].x, chickenAttacks[i].y}, 0.0f, chickenAttacks[i].scale, WHITE);
            }

            for(int i = 0; i < chickenCount; i++){
                if(chickens[i].health > 0){
                    victory = false;
                    DrawTextureEx(chickenTex, (Vector2){chickens[i].x, chickens[i].y}, 0.0f, chickens[i].scale, WHITE);
                }
            }

            DrawRectangle(0, screenHeight-20, ((SHIP.health % 100) * screenWidth * 0.01f), 20, RED);

            if(SHIP.health <= 0) DrawRectangle(screenHeight/2, screenWidth/2, 600, 200, RED);

            if(victory) DrawRectangle(screenHeight/2, screenWidth/2, 600, 200, BLUE);
            
        EndDrawing();
    }

    UnloadTexture(shipTex);
    CloseWindow();

    return 0;
}