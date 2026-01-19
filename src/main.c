#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/emu.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

int main(int argc, char **argv) {
    struct emu* chip8 = malloc(sizeof(struct emu));
    FILE *rom = fopen("/home/chrisp/chip8/rom/IBMLogo.ch8", "rb");
    if (rom == NULL) {
        printf("Failed to open ROM\n");
        return 1;
    }
    read_rom(rom, chip8);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL couldn't create a window! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const int scale_x = SCREEN_WIDTH / DISPLAY_WIDTH;
    const int scale_y = SCREEN_HEIGHT / DISPLAY_HEIGHT;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }

        execute(chip8);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                int idx = y * DISPLAY_WIDTH + x;
                if (chip8->display[idx]) {
                    SDL_Rect r = { x * scale_x, y * scale_y, scale_x, scale_y };
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    fclose(rom);
    free(chip8);
    return 0;
}