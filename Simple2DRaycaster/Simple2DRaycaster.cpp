#include <SDL.h>
#include "raycaster.h"

int main(int argc, char* argv[]) {
    Raycaster raycaster;

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        raycaster.update();
        raycaster.render();

        SDL_Delay(16); // 60 fps
    }

    return 0;
}
