#pragma once
#include <vector>
#include <SDL3/SDL.h>

class SDL_Wizard
{
public:
	SDL_Wizard(int y, int x) : height(y), width(x) {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Pixel Buffer", width, height, 0);
        renderer = SDL_CreateRenderer(window, NULL);
        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            width, height
        );
	}

	~SDL_Wizard() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
	}

    void OverwriteBuffer(const std::vector<uint32_t>* buffer) {
        SDL_UpdateTexture(texture, NULL, buffer->data(), width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

private:
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};