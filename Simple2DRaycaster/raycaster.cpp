#include "raycaster.h"
#include <algorithm>
#include <vector>
#include <cstdlib>

Raycaster::Raycaster() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    playerX = 3.0f;
    playerY = 3.0f;
    playerAngle = 0.0f;

    map = std::vector<int>(mapWidth * mapHeight, 0);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    timeOfDay = 0.0f;
}

Raycaster::~Raycaster() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Raycaster::render() {
    timeOfDay += 0.0002f;
    if (timeOfDay >= 1.0f) timeOfDay = 0.0f;

    float red = (1.0f - timeOfDay) * 135 + timeOfDay * 25;
    float green = (1.0f - timeOfDay) * 206 + timeOfDay * 25;
    float blue = (1.0f - timeOfDay) * 250 + timeOfDay * 50;

    SDL_SetRenderDrawColor(renderer, (int)red, (int)green, (int)blue, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_Rect floorRect = { 0, screenHeight / 2, screenWidth, screenHeight / 2 };
    SDL_RenderFillRect(renderer, &floorRect);

    if (timeOfDay > 0.5f) {
        for (int i = 0; i < 100; ++i) {
            static std::vector<std::pair<int, int>> stars;
            if (stars.empty()) {
                for (int i = 0; i < 100; ++i) {
                    int starX = rand() % screenWidth;
                    int starY = rand() % (screenHeight / 2);
                    stars.push_back({ starX, starY });
                }
            }

            int starIndex = rand() % stars.size();
            float starX = stars[starIndex].first;
            float starY = stars[starIndex].second;
            int brightness = rand() % 255;
            SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);
            SDL_RenderDrawPoint(renderer, starX, starY);
        }
    }

    for (int x = 0; x < screenWidth; ++x) {
        float rayAngle = (playerAngle - 0.5f) + ((float)x / screenWidth);
        float rayX = cos(rayAngle);
        float rayY = sin(rayAngle);

        float distance = 0.0f;
        bool hit = false;

        while (!hit && distance < 16.0f) {
            distance += 0.1f;
            int testX = int(playerX + rayX * distance);
            int testY = int(playerY + rayY * distance);

            if (testX >= mapWidth || testY >= mapHeight || testX < 0 || testY < 0) {
                hit = true;
                continue;
            }

            if (map[testY * mapWidth + testX] == 1) hit = true;
        }

        int lineHeight = (int)(screenHeight / (distance + 0.0001f));
        int drawStart = std::max(-lineHeight / 2 + screenHeight / 2, 0);
        int drawEnd = std::min(lineHeight / 2 + screenHeight / 2, screenHeight - 1);

        int brightness = (int)(255.0f / (distance + 0.0001f));
        brightness = std::min(brightness, 255);

        SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);

        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }

    SDL_RenderPresent(renderer);
}

void Raycaster::update() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) playerAngle -= 0.05f;
    if (keys[SDL_SCANCODE_RIGHT]) playerAngle += 0.05f;

    if (keys[SDL_SCANCODE_UP]) {
        float moveX = cos(playerAngle) * 0.1f;
        float moveY = sin(playerAngle) * 0.1f;

        playerX += moveX;
        playerY += moveY;
    }

    if (keys[SDL_SCANCODE_DOWN]) {
        float moveX = cos(playerAngle) * 0.1f;
        float moveY = sin(playerAngle) * 0.1f;

        playerX -= moveX;
        playerY -= moveY;
    }
}

void Raycaster::renderCircle(int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}