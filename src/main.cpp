#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Renderer *renderer;
SDL_Texture *image;

bool main_loop()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {

#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif

        return false;
    }
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, image, nullptr, nullptr);

    SDL_RenderPresent(renderer);
    return true;
}

void run_main_loop() {
    main_loop();
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
            "Getting Started",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800, 600,
            SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 255, 69, 69, 255);
    image = IMG_LoadTexture(renderer, "assets/images/whale.png");
    if (image == nullptr) {
        printf("Error loading image: %s\n", IMG_GetError());
    }
    else {
        printf("Image loaded successfully!\n");
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(run_main_loop, 0, true);
#else
    while(main_loop()) {
        SDL_Delay(0);
    };
#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}