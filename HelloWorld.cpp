#include <iostream>
#include "raylib.h"

using namespace std;

// Color palette 
const Color tealBackground = {13, 115, 120, 255};      // Main background
const Color orangeAccent   = {255, 140, 0, 255};       // Paddles & center line
const Color creamAccent    = {245, 235, 200, 255};     // Ball & score

int playerScore = 0;
int cpuScore = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, creamAccent);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) {
            cpuScore++;
            resetBall();
        }
        if (x - radius <= 0) {
            playerScore++;
            resetBall();
        }
    }

    void resetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speedChoice[2] = { -1, 1};
        speed_x *= speedChoice[GetRandomValue(0, 1)];
        speed_y *= speedChoice[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void limitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.5f, 12, orangeAccent);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        limitMovement();
    }
};

class cpuPaddle: public Paddle {
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }
        limitMovement();
    }
};

Ball ball;
Paddle player;
cpuPaddle cpu;

// Draw a dotted center line
void DrawDottedLine(int x, int yStart, int yEnd, int segmentLength, int gap, Color color) {
    for (int y = yStart; y < yEnd; y += segmentLength + gap) {
        DrawLine(x, y, x, y + segmentLength, color);
    }
}

int main() {
    cout << " Starting the game " << endl;

    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Ping Pong Game!");
    SetTargetFPS(70);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false) {
        BeginDrawing();

        // Update game objects
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Collision detection
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }

        // Drawing
        ClearBackground(tealBackground);

        // Dotted center line
        DrawDottedLine(screen_width / 2, 40, screen_height - 40, 20, 20, orangeAccent);

        ball.Draw();
        cpu.Draw();
        player.Draw();

        // Score
        DrawText(TextFormat("%i", cpuScore), screen_width / 4 - 20, 40, 80, creamAccent);
        DrawText(TextFormat("%i", playerScore), 3 * screen_width / 4 - 20, 40, 80, creamAccent);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}