#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

struct SnakeSegment {
    int x, y;
};

class SnakeGame {
public:
    int screenWidth, screenHeight;
    int gridWidth, gridHeight, gridSize;
    int speed;
    int counter = 0;

    SnakeGame() {
        screenWidth = 400;
        screenHeight = 460;
        gridWidth = 80;
        gridHeight = 45;
        gridSize = screenWidth / gridWidth;
        speed = 15;

        snakeLength = 3;
        snakeDirection = RIGHT;
        nextDirection = RIGHT;
        score = 0;

        snake.push_back({80, 45});
        snake.push_back({79, 45});
        snake.push_back({78, 45});

        spawnApple();

        InitWindow(screenWidth, screenHeight, "Snake");
        SetTargetFPS(60);
    }

    ~SnakeGame() {
        CloseWindow();
    }

    void PromptForSettings() {
        bool done = false;
        bool blink = false;

        std::string screenWidthStr = std::to_string(1600);
        std::string screenHeightStr = std::to_string(900);
        std::string gridWidthStr = std::to_string(gridWidth);
        std::string gridHeightStr = std::to_string(gridHeight);
        std::string speedStr = std::to_string(speed);

        int selectedField = 0;

        char result[32];

        while (!done && !WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
    
            DrawText("Set up your game:", 20, 20, 30, RAYWHITE);

            DrawText("Screen Width (160-1920):", 20, 70, 20, (selectedField == 0) ? MAROON : RAYWHITE);
            strcpy(result, screenWidthStr.c_str());
            strcat(result, ((blink && (selectedField == 0))) ? "_" : "");
            DrawText(result, 20, 100, 20, WHITE);
    
            DrawText("Screen Height (90-1080):", 20, 140, 20, (selectedField == 1) ? MAROON : RAYWHITE);
            strcpy(result, screenHeightStr.c_str());
            strcat(result, ((blink && (selectedField == 1))) ? "_" : "");
            DrawText(result, 20, 170, 20, WHITE);
    
            DrawText("Grid Width (16-320):", 20, 210, 20, (selectedField == 2) ? MAROON : RAYWHITE);
            strcpy(result, gridWidthStr.c_str());
            strcat(result, ((blink && (selectedField == 2))) ? "_" : "");
            DrawText(result, 20, 240, 20, WHITE);
    
            DrawText("Grid Height (9-180):", 20, 280, 20, (selectedField == 3) ? MAROON : RAYWHITE);
            strcpy(result, gridHeightStr.c_str());
            strcat(result, ((blink && (selectedField == 3))) ? "_" : "");
            DrawText(result, 20, 310, 20, WHITE);

            DrawText("Speed (1-20):", 20, 350, 20, (selectedField == 4) ? MAROON : RAYWHITE);
            strcpy(result, speedStr.c_str());
            strcat(result, ((blink && (selectedField == 4))) ? "_" : "");
            DrawText(result, 20, 380, 20, WHITE);
    
            DrawText("Press ENTER to start!", 20, 420, 20, MAROON);

            if (IsKeyPressed(KEY_DOWN)) selectedField = (selectedField + 1) % 5;
            if (IsKeyPressed(KEY_UP)) selectedField = (selectedField + 4) % 5;
    
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (selectedField == 0 && screenWidthStr.length() > 0) screenWidthStr.pop_back();
                if (selectedField == 1 && screenHeightStr.length() > 0) screenHeightStr.pop_back();
                if (selectedField == 2 && gridWidthStr.length() > 0) gridWidthStr.pop_back();
                if (selectedField == 3 && gridHeightStr.length() > 0) gridHeightStr.pop_back();
                if (selectedField == 4 && speedStr.length() > 0) speedStr.pop_back();
            }

            for (int key = KEY_ZERO; key <= KEY_NINE; ++key) {
                if (IsKeyPressed(key)) {
                    char numChar = key - KEY_ZERO + '0';
                    if (selectedField == 0) screenWidthStr.size() < 31 ? screenWidthStr += numChar : screenWidthStr;
                    if (selectedField == 1) screenHeightStr.size() < 31 ? screenHeightStr += numChar : screenHeightStr;
                    if (selectedField == 2) gridWidthStr.size() < 31 ? gridWidthStr += numChar : gridWidthStr;
                    if (selectedField == 3) gridHeightStr.size() < 31 ? gridHeightStr += numChar : gridHeightStr;
                    if (selectedField == 4) speedStr.size() < 31 ? speedStr += numChar : speedStr;
                }
            }
    
            if (IsKeyPressed(KEY_ENTER)) {
                screenWidth = std::stoi(screenWidthStr);
                screenHeight = std::stoi(screenHeightStr);
                gridWidth = std::stoi(gridWidthStr);
                gridHeight = std::stoi(gridHeightStr);
                speed = std::stoi(speedStr);
                done = true;
            }
    
            EndDrawing();
 
            if (counter % 30 == 0) blink = !blink;
            counter++;
        }
    
        screenWidth = screenWidth > 1920 ? 1920 : screenWidth;
        screenHeight = screenHeight > 1080 ? 1080 : screenHeight;
        gridWidth = gridWidth > 320 ? 320 : gridWidth;
        gridHeight = gridHeight > 180 ? 180 : gridHeight;
        speed = speed > 20 ? 20 : speed;
        speed = 20 - speed;
        if (speed == 0) speed = 1;
        gridSize = screenWidth / gridWidth;
        counter = 0;
        spawnApple();
    }    

    void ProcessInput() {
        if (IsKeyPressed(KEY_W) && snakeDirection != DOWN) nextDirection = UP;
        if (IsKeyPressed(KEY_S) && snakeDirection != UP) nextDirection = DOWN;
        if (IsKeyPressed(KEY_A) && snakeDirection != RIGHT) nextDirection = LEFT;
        if (IsKeyPressed(KEY_D) && snakeDirection != LEFT) nextDirection = RIGHT;

        if (nextDirection != snakeDirection) {
            if (nextDirection == UP && snakeDirection != DOWN) snakeDirection = UP;
            else if (nextDirection == DOWN && snakeDirection != UP) snakeDirection = DOWN;
            else if (nextDirection == LEFT && snakeDirection != RIGHT) snakeDirection = LEFT;
            else if (nextDirection == RIGHT && snakeDirection != LEFT) snakeDirection = RIGHT;
        }
    }

    void Update() {        
        SnakeSegment newHead = snake.front();
        switch (snakeDirection) {
            case UP: newHead.y--; break;
            case DOWN: newHead.y++; break;
            case LEFT: newHead.x--; break;
            case RIGHT: newHead.x++; break;
        }
    
        if (newHead.x < 0) newHead.x = gridWidth - 1;
        if (newHead.x >= gridWidth) newHead.x = 0;
        if (newHead.y < 0) newHead.y = gridHeight - 1;
        if (newHead.y >= gridHeight) newHead.y = 0;
    
        for (const auto& segment : snake) {
            if (newHead.x == segment.x && newHead.y == segment.y) {
                ResetGame();
                return;
            }
        }

        snake.insert(snake.begin(), newHead);

        if (newHead.x == appleX && newHead.y == appleY) {
            score++;
            spawnApple();
        } else {
            snake.pop_back();
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);

        for (const auto& segment : snake) {
            DrawRectangle(segment.x * gridSize, segment.y * gridSize, gridSize, gridSize, WHITE);
        }

        DrawRectangle(appleX * gridSize, appleY * gridSize, gridSize, gridSize, RED);

        EndDrawing();
    }

private:
    void ResetGame() {
        snake.clear();
        snake.push_back({80, 45});
        snake.push_back({79, 45});
        snake.push_back({78, 45});
        score = 0;
        snakeDirection = RIGHT;
        spawnApple();
    }

    void spawnApple() {
        appleX = GetRandomValue(0, gridWidth - 1);
        appleY = GetRandomValue(0, gridHeight - 1);
    }

    int snakeLength, appleX, appleY, score;
    std::vector<SnakeSegment> snake;
    enum Direction { UP, DOWN, LEFT, RIGHT } snakeDirection;
    enum Direction nextDirection;
};

int main() {
    SnakeGame game;
    game.PromptForSettings();
    SetWindowSize(game.screenWidth, game.screenHeight);

    while (!WindowShouldClose()) {
        game.ProcessInput();
        if (game.counter % game.speed == 0) game.Update();
        game.counter++;
        game.Draw();
    }

    return 0;
}
