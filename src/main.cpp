
#include "raylib.h"

#include "constants.hpp"
#include "functions.hpp"

#include "structures/block.hpp"
#include "structures/player.hpp"
#include "structures/ball.hpp"
#include "structures/label.hpp"

#include <string>

Player player;
Ball ball;
Block blocks[5][10];
Label displayedPoints;
Label startGameText;
Label gameOverText;

int points = 0;

Font font;
Sound gameOverSound;
Sound bounceSound;
Sound bounceOnBlockSound;

bool start = false;
bool gameOver = false;

int counter = 0;

void InitGame();
void Update();
void Draw();
void GameOverDraw();

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkan game albanese diocane");
    font = LoadFontEx("../src/resources/papyrus.ttf", BIG_FONTSIZE, nullptr, 250);
    InitAudioDevice();
    gameOverSound = LoadSound("../src/resources/lo_porto_io.wav");
    bounceSound= LoadSound("../src/resources/pluh2.wav");
    bounceOnBlockSound = LoadSound("../src/resources/ia_cut.wav");
    SetSoundVolume(bounceOnBlockSound,0.5);
    InitGame();

    SetTargetFPS(240);

    while (!WindowShouldClose()) {
        if(!gameOver){
            displayedPoints.location = {getCenter(SCREEN_WIDTH, MeasureTextEx(font, displayedPoints.text.c_str(), BIG_FONTSIZE, FONT_SPACING).x), 0};
            Update();
            Draw();
        } else {
            displayedPoints.location = {getCenter(SCREEN_WIDTH, MeasureTextEx(font, displayedPoints.text.c_str(), SMALL_FONTSIZE, FONT_SPACING).x), 200};
            GameOverDraw();
            InitGame();
            if(GetKeyPressed() == KEY_SPACE){
                gameOver = false;
                points = 0;
            }
        }
    }

    UnloadFont(font);
    UnloadSound(gameOverSound);
    UnloadSound(bounceSound);
    UnloadSound(bounceOnBlockSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void InitGame() {
    float tempX, tempY;

    start = false;

    ball.resetBouncesFlags();

    player.location = {getCenter(SCREEN_WIDTH, player.DIMENSIONS.x), SCREEN_HEIGHT - player.DIMENSIONS.y - 50};
    ball.location = {getCenter(SCREEN_WIDTH, ball.DIAMETER), player.location.y - ball.RADIUS - 5};
    ball.speed = {0, 0};

    int i,j;
    tempY = 70;// y location for the first block
    for( i = 0; i<BLOCK_ROWS; i++){
        tempX = 0;// x location for the first block
        for( j = 0; j < BLOCKS_PER_ROW; j++){
            blocks[i][j].location = {tempX, tempY};
            blocks[i][j].isActive = true;
            tempX = tempX + blocks[i][j].DIMENSIONS.x;
        }
        tempY = tempY + blocks[i][j-1].DIMENSIONS.y;
    }

    displayedPoints.text = "freaky points : " + std::to_string(points);
    displayedPoints.location = {getCenter(SCREEN_WIDTH, MeasureTextEx(font, displayedPoints.text.c_str(), BIG_FONTSIZE, FONT_SPACING).x), 0};
    startGameText.text = "press space to play";
    startGameText.location = {getCenter(SCREEN_WIDTH, MeasureTextEx(font, startGameText.text.c_str(), SMALL_FONTSIZE, FONT_SPACING).x), 250};

    gameOverText.text = "Game Over";
    gameOverText.location = {getCenter(SCREEN_WIDTH, MeasureTextEx(font, gameOverText.text.c_str(), BIG_FONTSIZE, FONT_SPACING).x), 50};
}

void Draw() {

    BeginDrawing();
    ClearBackground(WHITE);

    DrawRectangle(player.location.x, player.location.y, player.DIMENSIONS.x, player.DIMENSIONS.y, BLACK);
    DrawCircle(ball.location.x, ball.location.y, ball.RADIUS, BLUE);

    int i, j;
    for (i = 0; i < BLOCK_ROWS; i++) {
        for (j = 0; j < BLOCKS_PER_ROW; j++) {
            if (blocks[i][j].isActive) {
                if (i % 2 == 0) {
                    if (j % 2 == 0) {
                        DrawRectangle(blocks[i][j].location.x, blocks[i][j].location.y, blocks[i][j].DIMENSIONS.x,
                                      blocks[i][j].DIMENSIONS.y, DARKGRAY);
                    } else {
                        DrawRectangle(blocks[i][j].location.x, blocks[i][j].location.y, blocks[i][j].DIMENSIONS.x,
                                      blocks[i][j].DIMENSIONS.y, GRAY);
                    }
                } else {
                    if (j % 2 == 0) {
                        DrawRectangle(blocks[i][j].location.x, blocks[i][j].location.y, blocks[i][j].DIMENSIONS.x,
                                      blocks[i][j].DIMENSIONS.y, GRAY);
                    } else {
                        DrawRectangle(blocks[i][j].location.x, blocks[i][j].location.y, blocks[i][j].DIMENSIONS.x,
                                      blocks[i][j].DIMENSIONS.y, DARKGRAY);
                    }
                }
            }

        }
    }

    DrawTextEx(font, displayedPoints.text.c_str(), displayedPoints.location, BIG_FONTSIZE, FONT_SPACING, BLACK);
    DrawTextEx(font, startGameText.text.c_str(), startGameText.location, SMALL_FONTSIZE, FONT_SPACING, BLACK);
    EndDrawing();
}

void Update(){

    displayedPoints.text = "freaky points : " + std::to_string(points);

    //start condition
    if (GetKeyPressed() == KEY_SPACE) {
        if (ball.speed.x == 0 && ball.speed.y == 0) {
            ball.setInitSpeed();
            startGameText.text = "";
            start = true;
        }
    }
    if(!start) ball.location.x = player.location.x + player.DIMENSIONS.x/2;

    //player movement
    if (GetMousePosition().x - (player.DIMENSIONS.x / 2) < 0){
        player.location.x = 0;
    } else if(GetMousePosition().x + (player.DIMENSIONS.x / 2) > SCREEN_WIDTH){
        player.location.x = SCREEN_WIDTH - player.DIMENSIONS.x;
    } else {
        player.location.x = GetMousePosition().x - player.DIMENSIONS.x / 2;
    }

    ball.move();//ball movement


    //collision check with walls
    if (((ball.location.x + ball.RADIUS) >= SCREEN_WIDTH)) {
        if(!ball.hasBouncedOnWallRx) {
            ball.speed.x *= -1;
            PlaySound(bounceSound);
            ball.setBouncesFlags(Ball::onWallRx);
        }
    } else if (((ball.location.x - ball.RADIUS) <= 0)){
        if(!ball.hasBouncedOnWallLx) {
            ball.speed.x *= -1;
            PlaySound(bounceSound);
            ball.setBouncesFlags(Ball::onWallLx);
        }
    }
    if ((ball.location.y - ball.RADIUS) <= 0) {
        if(!ball.hasBouncedOnWallTop) {
            ball.speed.y *= -1;
            PlaySound(bounceSound);
            ball.setBouncesFlags(Ball::onWallTop);
        }
    }

    //collision check with player
    if (CheckCollisionCircleRec(ball.location, ball.RADIUS, (Rectangle){player.location.x, player.location.y, player.DIMENSIONS.x, player.DIMENSIONS.y})) {
        if (!ball.hasBouncedOnPlayer) {
            ball.speed.x = (ball.location.x - (player.location.x + player.DIMENSIONS.x/2))/(player.DIMENSIONS.x/2)*4;
            //ball.speed.y /= fabs(distance(ball.location.x, ball.location.y + ball.RADIUS, player.location.x + player.DIMENSIONS.x/2, player.location.y) + 1) / 10;
            ball.speed.y *= -1;
            PlaySound(bounceSound);
            ball.setBouncesFlags(Ball::onPlayer);
        }
    }

    //collision check with blocks
    ball.hasBouncedOnBlock = false;
    for (int i = 0; i < BLOCK_ROWS && !ball.hasBouncedOnBlock; i++) {
        for (int j = 0; j < BLOCKS_PER_ROW && !ball.hasBouncedOnBlock; j++) {
            if (blocks[i][j].isActive) {

                switch (ball.collisionWithBlock(blocks[i][j])) {
                    case Block::topCollision:
                        ball.speed.y *= -1;
                        blocks[i][j].isActive = false;
                        points++;
                        displayedPoints.text = "freaky points : " + std::to_string(points);
                        PlaySound(bounceOnBlockSound);
                        ball.setBouncesFlags(Ball::onBlock);
                        break;
                    case Block::bottomCollision:
                        ball.speed.y *= -1;
                        blocks[i][j].isActive = false;
                        points++;
                        displayedPoints.text = "freaky points : " + std::to_string(points);
                        PlaySound(bounceOnBlockSound);
                        ball.setBouncesFlags(Ball::onBlock);
                        break;
                    case Block::leftCollision:
                        ball.speed.x *= -1;
                        blocks[i][j].isActive = false;
                        points++;
                        displayedPoints.text = "freaky points : " + std::to_string(points);
                        PlaySound(bounceOnBlockSound);
                        ball.setBouncesFlags(Ball::onBlock);
                        break;
                    case Block::rightCollision:
                        ball.speed.x *= -1;
                        blocks[i][j].isActive = false;
                        points++;
                        displayedPoints.text = "freaky points : " + std::to_string(points);
                        PlaySound(bounceOnBlockSound);
                        ball.setBouncesFlags(Ball::onBlock);
                        break;
                    default:
                        break;
                }
            }
        }
    }


    //game over condition
    //TODO: win condition
    if(ball.location.y > SCREEN_HEIGHT || points >= 50) {
        gameOver = true;
        PlaySound(gameOverSound);
    }

}

void GameOverDraw(){
    BeginDrawing();
    ClearBackground(WHITE);

    DrawTextEx(font, displayedPoints.text.c_str(), displayedPoints.location, SMALL_FONTSIZE, FONT_SPACING, BLACK);
    DrawTextEx(font, gameOverText.text.c_str(), gameOverText.location, BIG_FONTSIZE, FONT_SPACING, BLACK);
    DrawTextEx(font, startGameText.text.c_str(), startGameText.location, SMALL_FONTSIZE, FONT_SPACING, BLACK);

    EndDrawing();
}