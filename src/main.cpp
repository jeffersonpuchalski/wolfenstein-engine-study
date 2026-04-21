#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    // Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error when init SDL2: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* windown = SDL_CreateWindow(
        "wolf3d", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        800, 600,
        SDL_WINDOW_SHOWN
    );
    if(!windown){
         SDL_Log("Erro ao criar janela: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(windown, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr) {
        SDL_Log("Erro ao criar o render: %s", SDL_GetError());
    }

    bool running = true;
    SDL_Event event;
    uint64_t frameTime, startTime, deltaTime = 0;

    while (running)
    {
        startTime = SDL_GetTicks64();
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }
    
        frameTime =  SDL_GetTicks64() - startTime;
        // Define backgound color - blue
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        // Clear The Render
        SDL_RenderClear(renderer);
        
        // Swap or Present Render
        SDL_RenderPresent(renderer);

        if(frameTime < 16) { 
            
            SDL_Delay(16 - frameTime);
        }
        deltaTime= SDL_GetTicks64() - startTime;

        // debug
        std::cout << "DeltaTime: " << deltaTime << "\b\r";
    }



    //Cleanup
    SDL_DestroyWindow(windown);
    SDL_Quit();
    
    return 0;
}