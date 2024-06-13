#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAME_WIDTH = 300;
const int FRAME_HEIGHT = 400;
const int PICTURE_WIDTH = FRAME_WIDTH - 45;
const int PICTURE_HEIGHT = FRAME_HEIGHT - 80;
const SDL_Color GREY_COLOR = { 192, 192, 192, 255 };

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Texture* loadTexture(const std::string& path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, GREY_COLOR.r, GREY_COLOR.g, GREY_COLOR.b, GREY_COLOR.a);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    std::vector<SDL_Texture*> backgroundTextures;
    backgroundTextures.push_back(loadTexture("./Assets/openGL.png"));
    backgroundTextures.push_back(loadTexture("./Assets/blender.png"));
    backgroundTextures.push_back(loadTexture("./Assets/frame.png"));

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, GREY_COLOR.r, GREY_COLOR.g, GREY_COLOR.b, GREY_COLOR.a);
        SDL_RenderClear(gRenderer);

        // Frame for the tent picture
        SDL_Rect tentFrame = { SCREEN_WIDTH / 4 - FRAME_WIDTH / 2, (SCREEN_HEIGHT - FRAME_HEIGHT) / 2, FRAME_WIDTH, FRAME_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundTextures[2], NULL, &tentFrame);

        // Frame for the night picture
        SDL_Rect nightFrame = { 3 * SCREEN_WIDTH / 4 - FRAME_WIDTH / 2, (SCREEN_HEIGHT - FRAME_HEIGHT) / 2, FRAME_WIDTH, FRAME_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundTextures[2], NULL, &nightFrame);

        // Picture of the tent inside the frame (centered with 40 pixels less on each side)
        SDL_Rect tentDest = { tentFrame.x + (FRAME_WIDTH - PICTURE_WIDTH) / 2, tentFrame.y + (FRAME_HEIGHT - PICTURE_HEIGHT) / 2, PICTURE_WIDTH, PICTURE_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundTextures[0], NULL, &tentDest);

        // Picture of the night inside the frame (centered with 40 pixels less on each side)
        SDL_Rect nightDest = { nightFrame.x + (FRAME_WIDTH - PICTURE_WIDTH) / 2, nightFrame.y + (FRAME_HEIGHT - PICTURE_HEIGHT) / 2, PICTURE_WIDTH, PICTURE_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundTextures[1], NULL, &nightDest);

        SDL_RenderPresent(gRenderer);
    }

    for (SDL_Texture* texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }

    closeSDL();

    return 0;
}
