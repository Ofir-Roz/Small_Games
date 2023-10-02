#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include "DynamicScreen.h"
#include <string>
#define SKYBLUEE    CLITERAL(Color){ 125, 210, 255, 255 }   // Sky Blue

int main()
{
    // window size in pixels 
    const int windowWidth{682};
    const int windowHeight{576};
    InitWindow(windowWidth, windowHeight, "Arachisya");
    InitAudioDevice();

    // some of the sound assets 
    Sound gameMusic = LoadSound("nature_tileset/Desecrated Cave ver.1.wav");
    Sound openingMusic = LoadSound("nature_tileset/A town without hope (no loop).wav");
    Sound defeatSound = LoadSound("nature_tileset/gameover_loud.mp3");
    SetSoundVolume(defeatSound, 0.2f);
    SetSoundPitch(defeatSound, 2.7f);
    bool played {false};

    Texture2D map = LoadTexture("nature_tileset/DesertWorldMap_2_24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    DynamicScreen openScreen;
    Texture2D openScreenBackground = LoadTexture("nature_tileset/Space_Background_fit.png");

    Character knight(windowWidth, windowHeight);

    Prop props[6]{
        Prop{Vector2{800.f, 1200.f}, LoadTexture("nature_tileset/Rock.png"), 7.f},
        Prop{Vector2{1450.f, 950.f}, LoadTexture("nature_tileset/sign.png"), 4.f},
        Prop{Vector2{1800.f, 1650.f}, LoadTexture("nature_tileset/Rock.png"), 10.f},
        Prop{Vector2{600.f, 700.f}, LoadTexture("nature_tileset/Log.png"), 6.f},
        Prop{Vector2{1500.f, 300.f}, LoadTexture("nature_tileset/Log.png"), 5.f},
        Prop{Vector2{2100.f, 500.f}, LoadTexture("nature_tileset/Rock.png"), 6.5f}};

    Enemy enmies[]{
        Enemy{Vector2{1250.f, 350.f}, LoadTexture("characters/goblin_idle_spritesheet.png"),
              LoadTexture("characters/goblin_run_spritesheet.png"), 3.f},
        Enemy{Vector2{2700.f, 2500.f}, LoadTexture("characters/goblin_idle_spritesheet.png"),
              LoadTexture("characters/goblin_run_spritesheet.png"), 3.9f},
        Enemy{Vector2{3200.f, 250.f}, LoadTexture("characters/goblin_idle_spritesheet.png"),
              LoadTexture("characters/goblin_run_spritesheet.png"), 2.8f},
        Enemy{Vector2{1900.f, 3000.f}, LoadTexture("characters/slime_idle_spritesheet.png"),
              LoadTexture("characters/slime_run_spritesheet.png"), 1.7f},
        Enemy{Vector2{1330.f, 2330.f}, LoadTexture("characters/slime_idle_spritesheet.png"),
              LoadTexture("characters/slime_run_spritesheet.png"), 2.9f},
        Enemy{Vector2{3600.f, 2900.f}, LoadTexture("characters/goblin_idle_spritesheet.png"),
              LoadTexture("characters/goblin_run_spritesheet.png"), 3.2f},
        Enemy{Vector2{2670.f, 2900.f}, LoadTexture("characters/IntellectDevourerIdleSide_spritesheet.png"),
              LoadTexture("characters/IntellectDevourerIdleSide_spritesheet.png"), 2.2f}};

    Enemy *enemiesP[]{
        &enmies[0],
        &enmies[1],
        &enmies[2],
        &enmies[3],
        &enmies[4],
        &enmies[5],
        &enmies[6]};

    SetTargetFPS(60);
    PlaySound(openingMusic);

    while (IsKeyUp(KEY_ENTER) && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextureEx(openScreenBackground, mapPos, 0.0, 1.f, WHITE);
        openScreen.Tick(GetFrameTime(), mapPos);

        if (!IsSoundPlaying(openingMusic))
            PlaySound(openingMusic);

        EndDrawing();
    }

    StopSound(openingMusic);

    for (auto enemy : enemiesP)
        enemy->setTarget(&knight);

    PlaySound(gameMusic);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUEE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // draw the props
        for (auto prop : props)
            prop.Render(knight.getWorldPos());

        // health managment
        if (!knight.getAlive())
        {
            if (!IsSoundPlaying(defeatSound) && !played)
            {
                PlaySound(defeatSound);
                played = true;
            }
            else
                ResumeSound(defeatSound);
            
            DrawText("Game Over!", 220.f, windowHeight / 2.3f, 48, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightHealth = "Health: ";
            knightHealth.append(std::to_string(knight.getHealth()), 0, 5);

            // draw health color based on value
            if (knight.getHealth() >= 75.f)
                DrawText(knightHealth.c_str(), 55.f, 45.f, 36, LIME);
            else if (knight.getHealth() >= 45.f)
                DrawText(knightHealth.c_str(), 55.f, 45.f, 36, YELLOW);
            else
                DrawText(knightHealth.c_str(), 55.f, 45.f, 36, MAROON);
        }

        knight.tick(GetFrameTime());

        // check map bounds
        if (knight.getWorldPos().x < -130.f ||
            knight.getWorldPos().y < -100.f ||
            (knight.getWorldPos().x + windowHeight) * 0.99f > map.width * mapScale ||
            (knight.getWorldPos().y + windowHeight) * 0.99f > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // check prop collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
                knight.undoMovement();

            for (auto enemy : enemiesP)
                if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), enemy->getCollisionRec()))
                    enemy->undoMovement();
        }

        for (auto enemy : enemiesP)
        {
            enemy->tick(GetFrameTime());

            if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getCollisionRec()))
                enemy->undoMovement();
        }

        if (IsKeyPressed(KEY_SPACE))
            for (auto enemy : enemiesP)
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                    enemy->setAlive(false);

        if (!IsSoundPlaying(gameMusic))
            PlaySound(gameMusic);

        EndDrawing();
    }
    StopSound(gameMusic);

    CloseAudioDevice();
    CloseWindow();
}