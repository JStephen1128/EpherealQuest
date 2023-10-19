 #include "raylib.h"
#include <time.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 120
#define FIRST_WAVE 15
#define SECOND_WAVE 30
#define THIRD_WAVE 55
#define FOURTH_WAVE 80
#define FIFTH_WAVE 95
#define SIXTH_WAVE 120

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { FIRST = 0, SECOND, THIRD, FOURTH, FIFTH, SIXTH } EnemyWave;

typedef struct Player{
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

typedef struct Enemy{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

typedef struct Shoot{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

//Window Size
static const int SCREEN_W_G = 960;
static const int SCREEN_W_T = 600;

//Game Screen
static const int screenWidth = 742;
static const int screenHeight = 433;

//Border
static Texture2D hudBorder;
static Texture2D hudBorder2;

//Backgrounds
static Texture2D forest;
static Texture2D beach;

//Gersemy's Sprite
Texture2D playerTexture;

//Title screen
Texture2D titleScreen;

//Game States
static bool gameOver = false;
static bool pause =  false;
static bool isTitleScreen = true;
static bool victory = false;

//Player Stats
static int score = 0; //EXP
static int money = 0; //Currency
static int max_hp = 16; //Maximum HP
static int curr_hp = 16; //Current HP
static int level = 1;
static int def = 1;
static int firerate = 3; //Attack
static int agi = 5; //Agility
static int points = 0; //Points

//Enemy Stats
static int enemAtk = 4;
static int stageMod = 1;

//Equipment Stats
static int armor = 0;
static int attack = 0;
static int fireH = 10;
static int fireW = 5;
static int as = 7;

//Intermission
static bool StageClear = false;
static bool StageClear2 = false;
static bool StageClear3 = false;
static bool StageClear4 = false;
static bool StageClear5 = false;
static bool StageClear6 = false;
static int a_presses = 0;

static Player player = { 0 };
static Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
static Shoot shoot[NUM_SHOOTS] = { 0 };
static EnemyWave wave = { 0 };

//Meta
static int shootRate = 0;
static float alpha = 0.0f;
bool textVisible = true;

static int activeEnemies = 0;
static int enemiesKill = 0;
static bool smooth = false;

static int framesCounter = 0;


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void InitGame2(void);         // Initialize Stage 2
static void InitGame3(void);         // Initialize Stage 3
static void InitGame4(void);         // Initialize Stage 4
static void InitGame5(void);         // Initialize Stage 5
static void InitGame6(void);         // Initialize Stage 6

static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(SCREEN_W_G, SCREEN_W_T, "EpherealQuest: Sabre Shuffling Scramble");
    

    titleScreen = LoadTexture("title.png");




//    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game stages

//Rom Forest
void InitGame(void)
{
    hudBorder = LoadTexture("GameBorder.png");
    hudBorder2 = LoadTexture("IntermissionBorder.png");
    forest = LoadTexture("forest.png");
    beach = LoadTexture("beach.png");
    
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    StageClear = false;
    victory = false;
    smooth = false;
    wave = FIRST;
    activeEnemies = FIRST_WAVE;
    enemiesKill = 0;
    score = 0;
    money = 0;
    max_hp = 16;
    curr_hp = 16;
    level = 1;
    def = 1;
    firerate = 3;
    agi = 5;
    alpha = 0;
    stageMod = 1;
    framesCounter = 0;
    playerTexture = LoadTexture("Gersemy.png");
    
    DrawTexture(forest, 0, 0, WHITE);

    // Initialize player
    player.rec.x =  10;
    player.rec.y = 100;
    player.rec.width = 39;
    player.rec.height = 41;
    player.speed.x = agi;
    player.speed.y = agi;
    player.color = BLACK;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth+ 1000);
//        enemy[i].rec.x = GetRandomValue(0, screenWidth - enemy[i].rec.width);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = fireH;
        shoot[i].rec.height = fireW;
        shoot[i].speed.x = as;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

//Seaside Scrander
void InitGame2(void)
{

    
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    StageClear = false;
    victory = false;
    smooth = false;
    wave = SECOND;
    activeEnemies = SECOND_WAVE;
    alpha = 0;
    framesCounter = 0;
    stageMod = 2;


    // Initialize player
//    player.rec.x =  20;
//    player.rec.y = 50;
//    player.rec.width = 20;
//    player.rec.height = 20;
//    player.speed.x = 5;
//    player.speed.y = 5;
//    player.color = BLACK;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = RED;
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

//Wasteful Wasteland
void InitGame3(void)
{

    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    StageClear = false;
    victory = false;
    smooth = false;
    wave = THIRD;
    activeEnemies = THIRD_WAVE;
    alpha = 0;
    framesCounter = 0;


    // Initialize player
    player.rec.x =  20;
    player.rec.y = 50;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 5;
    player.speed.y = 5;
    player.color = BLACK;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

//Longarce Longland
void InitGame4(void)
{
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    StageClear = false;
    victory = false;
    smooth = false;
    wave = SECOND;
    activeEnemies = SECOND_WAVE;
    alpha = 0;
    framesCounter = 0;


    // Initialize player
    player.rec.x =  20;
    player.rec.y = 50;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 5;
    player.speed.y = 5;
    player.color = BLACK;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

//Hazelnut Heights
void InitGame5(void)
{
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    StageClear = false;
    victory = false;
    smooth = false;
    wave = SECOND;
    activeEnemies = SECOND_WAVE;
    alpha = 0;
    framesCounter = 0;


    // Initialize player
    player.rec.x =  20;
    player.rec.y = 50;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 5;
    player.speed.y = 5;
    player.color = BLACK;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

//Casa de Thalavin
void InitGame6(void)
{
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    StageClear = false;
    victory = false;
    smooth = false;
    wave = SECOND;
    activeEnemies = SECOND_WAVE;
    alpha = 0;
    framesCounter = 0;


    // Initialize player
    player.rec.x =  20;
    player.rec.y = 50;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 5;
    player.speed.y = 5;
    player.color = BLACK;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

// Update game (one frame)
void UpdateGame(void)
{
    
    if (!gameOver && !StageClear && !isTitleScreen)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
            
        {
            framesCounter++;
                    
            switch (wave)
            {
                case FIRST:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

//                    if (enemiesKill == activeEnemies)
                    if (((float)framesCounter/60) == 10.0f)
                    {
                        enemiesKill = 0;
                        framesCounter = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        StageClear = true;

                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = THIRD_WAVE;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies) victory = true;

                } break;
                default: break;
            }

            // Player movement
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

            // Player collision with enemy
            for (int i = 0; i < activeEnemies; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)){
                    if ((curr_hp - ((enemAtk + (stageMod * 4)) - def)) <= 0){
                        
                        gameOver = true;
                        
                    }
                    else{
                        
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                        curr_hp -= ((enemAtk + (stageMod * 4)) - def);
                        
                    }
                }
            }

             // Enemy behaviour
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.x -= enemy[i].speed.x;

                    if (enemy[i].rec.x < 0)
                    {
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                    }
                }
            }

            // Wall behaviour
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

            // Shoot initialization
            if (IsKeyDown(KEY_SPACE))
            {
                shootRate += firerate;

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate%20 == 0)
                    {
                        shoot[i].rec.x = player.rec.x;
                        shoot[i].rec.y = player.rec.y + player.rec.height/4;
                        shoot[i].active = true;
                        break;
                    }
                }
            }

            // Shoot logic
            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movement
                    shoot[i].rec.x += shoot[i].speed.x;

                    // Collision with enemy
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec))
                            {
                                shoot[i].active = false;
                                enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                                enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
                                shootRate = 0;
                                enemiesKill++;
                                score += (1+(20-level)/3); //EXP Formula
                                money += (3 + ((enemAtk/2) + (stageMod))); //Taler Formula
                            }

                            if (shoot[i].rec.x + shoot[i].rec.width >= screenWidth)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                    }
                }
            }
            
            //Level Up Function
            
            if (score >= 100)
            {
                score = 0;
                level++;
                
                //Max HP increases by 3
                max_hp+=3;
                
                //Attack increases by 1 every eight levels
                if(level == 8 || level == 16)
                {
                    firerate++;
                }
                
                //Defense increases by 1 every three levels
                if(level%3 == 0)
                {                
                    def++;
                }
                
                //Agility increases by 1 every three levels
                if(level%2 == 0)
                {                
                    agi++;
                }
            }
        }
    }
    
    else if (isTitleScreen)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            isTitleScreen = false;
        }
    }
    
    else if (StageClear)
    {
        if (IsKeyPressed(KEY_ENTER)) {a_presses++;}
        if (a_presses == 4)
        {
            InitGame2();
            StageClear = false;
            a_presses = 0;
        }
    }

    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
//            InitGame();
            isTitleScreen = true;
        }
    }
}

char* getRandomMessage() {

    char* messages[] = {
        "You probably should've stayed in bed today...",
        "Ugh! Diddums!",
        "You're supposed to dodge the enemies.",
        "XD3R-B8HH-9ZR2-FL16",
        "Gersemy! Get up! The fate of the world rests on thou shoulders!",
        "GERSEMY fainted!",
        "...I'll go call the mortician.",
        "Check please!"
    };
    
    int index = rand() % 8;
    return messages[index];
}

// Draw game (one frame)
void DrawGame(void)
{
    
    BeginDrawing();

        ClearBackground(RAYWHITE);
        
        double startTime = GetTime();
        int blinkDuration = 3; // Blink every 3 seconds
        bool textVisible = true;

        if (isTitleScreen) {
            DrawTexture(titleScreen, 0, 0, WHITE);
            
            double currentTime = GetTime() - startTime;
            int currentFrame = (int)(currentTime * 60); // Assuming 60 FPS
            
            if (currentFrame % (2 * blinkDuration * 60) < blinkDuration * 60) {
                textVisible = true;
            } else {
                textVisible = false;
            }
            
            if (textVisible) {
                DrawText(
                    "Press [ENTER] to start",
                    SCREEN_W_G / 2 - MeasureText("PRESS [ENTER] TO START", 30) / 2,
                    SCREEN_W_T * 0.65 + 39,
                    30, WHITE
                );
            }
        }


        else if (!gameOver && !StageClear)
        {
            DrawTexture(hudBorder, 0, 0, WHITE);



            //DrawRectangleRec(player.rec, player.color);


            if (wave == FIRST){ 
                        DrawTexture(forest, 0, 0, WHITE);
                        DrawText("Stage 1:", (screenWidth/2 - MeasureText("Stage 1:", 40)/2) - 25, (screenHeight/2 - 40) - 25, 20, Fade(WHITE, alpha));
                        DrawText("Rom Forest", screenWidth/2 - MeasureText("Rom Forest", 40)/2, screenHeight/2 - 40, 40, Fade(WHITE, alpha));
                        }
            else if (wave == SECOND){ 
                        DrawTexture(beach, 0, 0, WHITE);
                        DrawText("Stage 2:", (screenWidth/2 - MeasureText("Stage 1:", 40)/2) - 25, (screenHeight/2 - 40) - 25, 20, Fade(WHITE, alpha));
                        DrawText("Seaside Scrander", screenWidth/2 - MeasureText("Seaside Scrander", 40)/2, screenHeight/2 - 40, 40, Fade(WHITE, alpha));
                        }
            else if (wave == THIRD) DrawText("Wasteful Wasteland", screenWidth/2 - MeasureText("Wasteful Wasteland", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == FOURTH) DrawText("Longarce Longland", screenWidth/2 - MeasureText("Longarce Longland", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == FIFTH) DrawText("Hazelnut Heights", screenWidth/2 - MeasureText("Hazelnut Heights", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == SIXTH) DrawText("Casa de Thalavin", screenWidth/2 - MeasureText("Casa de Thalavin", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));

            DrawTexture(playerTexture, player.rec.x, player.rec.y, WHITE);
            
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
            }

            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
            }

            DrawText(TextFormat("Gersemy"), 780, 30, 20, WHITE);
            DrawText(TextFormat("Swordfighter"), 780, 55, 20, WHITE);
            DrawText(TextFormat("Level: %02i", level), 780, 80, 20, WHITE);
            DrawText(TextFormat("HP: %02i / %02i", curr_hp, max_hp), 780, 105, 20, WHITE);

            
            DrawText(TextFormat("Str: %02i", firerate), 780, 160, 20, WHITE);
            DrawText(TextFormat("Def: %02i", def), 780, 200, 20, WHITE);
            DrawText(TextFormat("Spd: %02i", agi), 780, 240, 20, WHITE);
            DrawText(TextFormat("EXP: %03i", score), 780, 280, 20, WHITE);
            DrawText(TextFormat("Taler: %04i", money), 780, 320, 20, WHITE);
            DrawText(TextFormat("Time: %.02f", (float)framesCounter/60), 780, 360, 20, WHITE);
  
            DrawText(TextFormat("Equipment"), 40, 480, 20, WHITE);
            DrawText(TextFormat("Shot Width: %02i", fireH), 560, 510, 20, WHITE);
            DrawText(TextFormat("Shot Height: %02i", fireW), 560, 535, 20, WHITE);           
            DrawText(TextFormat("Sword: Iron Sword"), 290, 510, 20, WHITE);
            DrawText(TextFormat("Shield: Buckler"), 290, 535, 20, WHITE);           
            DrawText(TextFormat("Attack Speed: %02i", as), 70, 510, 20, WHITE);
            DrawText(TextFormat("Armor Class: %02i", def + armor), 70, 535, 20, WHITE); 

 
            if (score >= 100) DrawText("Level Up!", screenWidth/2 - MeasureText("Level Up!", 40)/2, screenHeight/2 - 40, 30, Fade(WHITE, alpha));
            
            if (victory) DrawText("YOU WIN", screenWidth/2 - MeasureText("YOU WIN", 40)/2, screenHeight/2 - 40, 40, BLACK);

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }

                
        else if (StageClear){
            
            DrawTexture(hudBorder2, 0, 0, WHITE);
            
            DrawText(TextFormat("Gersemy"), 780, 30, 20, WHITE);
            DrawText(TextFormat("Swordfighter"), 780, 55, 20, WHITE);
            DrawText(TextFormat("Level: %02i", level), 780, 80, 20, WHITE);
            DrawText(TextFormat("HP: %02i / %02i", curr_hp, max_hp), 780, 105, 20, WHITE);

            
            DrawText(TextFormat("Str: %02i", firerate), 780, 160, 20, WHITE);
            DrawText(TextFormat("Def: %02i", def), 780, 200, 20, WHITE);
            DrawText(TextFormat("Spd: %02i", agi), 780, 240, 20, WHITE);
            DrawText(TextFormat("EXP: %03i", score), 780, 280, 20, WHITE);
            DrawText(TextFormat("Taler: %04i", money), 780, 320, 20, WHITE);
            DrawText(TextFormat("Time: N/A"), 780, 360, 20, WHITE);
  
            DrawText(TextFormat("Equipment"), 40, 480, 20, WHITE);
            DrawText(TextFormat("Shot Width: %02i", fireH), 560, 510, 20, WHITE);
            DrawText(TextFormat("Shot Height: %02i", fireW), 560, 535, 20, WHITE);           
            DrawText(TextFormat("Sword: Iron Sword"), 290, 510, 20, WHITE);
            DrawText(TextFormat("Shield: Buckler"), 290, 535, 20, WHITE);           
            DrawText(TextFormat("Attack Speed: %02i", as), 70, 510, 20, WHITE);
            DrawText(TextFormat("Armor Class: %02i", def + armor), 70, 535, 20, WHITE); 
            DrawText(TextFormat("Press [Enter] to Continue"), 780, 510, 10, WHITE);
            
            if (a_presses == 0){
            DrawText("After a tough battle, Gersemy arrives at the quaint /n hamlet of Rom Village.", screenWidth/2 - MeasureText("After a tough battle, Gersemy arrives at the quaint hamlet of Rom Village.", 20)/2, screenHeight/2 - 100, 20, WHITE);
            }
            else if (a_presses == 1){
                DrawText("She opts to immerse herself in the town's goods and services before continuing", screenWidth/2 - MeasureText("She opts to immerse herself in the town's goods and services before continuing", 20)/2, screenHeight/2 - 80, 20, WHITE);
                }
            //DrawText("to embark on her journey to the seaside, where her next waypoint awaits.", screenWidth/2 - MeasureText("to embark on her journey to the seaside, where her next waypoint awaits.", 20)/2, screenHeight/2 - 60, 20, WHITE);

        }
        else {

            srand((time(NULL) / 36));
            char* message = getRandomMessage();
            
            DrawText(message, GetScreenWidth()/2 - MeasureText(message, 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
            DrawText("Game Over", GetScreenWidth()/2 - MeasureText("Game Over", 20)/2, GetScreenHeight()/2 - 30, 20, GRAY);
            DrawText("Press [Enter] to play again", GetScreenWidth()/2 - MeasureText("Press [Enter] to play again", 20)/2, GetScreenHeight()/2 - 10, 20, GRAY);
        }

    EndDrawing();
}


// Unload game variables
void UnloadGame(void)
{
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}