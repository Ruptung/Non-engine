#pragma once
#include <SDL3/SDL.h>
#include <vector>

#include "BasicStructures.h"
/*
*
1. 먼저 웹에서 **《씹어먹는 C++》**의 아래 3개 파트를 가볍게 훑어보세요:
• RAII 패턴과 스마트 포인터 (std::unique_ptr, std::shared_ptr)
• 우측값 참조와 이동 시맨틱 (std::move)
• constexpr와 enum class
2. 그 다음 **《Effective Modern C++》**을 사서 정독해 보세요.
3. 책을 읽으면서 지금 작성하신 VirtualScreen, Painter, SDL_Wizard 프로젝트를 모던 C++ 스타일로 하나씩 리팩토링해
보시면 실력이 수직 상승할 겁니다.
*/
class SDL_Wizard
{
public:
	SDL_Wizard(Vector2 VirtualVector, Vector2 ScreenVector)
	: VirtualVector(VirtualVector), ScreenVector(ScreenVector),

	window(SDL_CreateWindow("Pixel Buffer", ScreenVector.x, ScreenVector.y, 0)),
	renderer(SDL_CreateRenderer(window, nullptr)){

        SDL_Init(SDL_INIT_VIDEO);

	    // 텍스처는 가상 해상도(32x32) 크기로 생성
	    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            VirtualVector.x, VirtualVector.y
        );

	    // 픽셀이 뭉개지지 않도록 최근접 점(Nearest-neighbor) 필터링 설정
	    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	}

	~SDL_Wizard() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
	}

    void OverwriteBuffer(const std::vector<uint32_t>* buffer) {
        SDL_UpdateTexture(texture, NULL, buffer->data(), VirtualVector.x * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

private:
    Vector2 VirtualVector, ScreenVector;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
