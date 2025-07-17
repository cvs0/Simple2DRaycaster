#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SDL.h>
#include <cmath>
#include <vector> 

class Raycaster {
public:
    Raycaster();
    ~Raycaster();

    void render();
    void update();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    const int screenWidth = 640;
    const int screenHeight = 480;

    float playerX, playerY, playerAngle;
    const int mapWidth = 8, mapHeight = 8;
    const int tileSize = 64;
    std::vector<int> map;

    float timeOfDay;

    void drawMap();
    void renderCircle(int centerX, int centerY, int radius);
};

#endif
