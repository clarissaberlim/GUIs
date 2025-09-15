#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define FINISH_LINE   500
#define SPEED1 3
#define SPEED2 2
#define SPEED3 4

int main(int argc, char* args[]) {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Corrida",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        600, 200,
                        SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // Retângulos (corredores)
    SDL_Rect r1 = { 20, 40, 20, 20 };
    SDL_Rect r2 = { 20, 90, 20, 20 };
    SDL_Rect r3 = { 20, 140, 20, 20 };

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Reset corrida
        r1.x = r2.x = r3.x = 20;
        bool chegou1 = false, chegou2 = false, chegou3 = false;
        bool vencedorAnunciado = false;
        int vencedor = 0;

        while (!(chegou1 && chegou2 && chegou3) && !quit) {
            // Espera evento OU timeout (30 ms)
            int hasEvent = SDL_WaitEventTimeout(&e, 30);

            if (hasEvent) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            } else {
                // Timeout → mover os objetos
                if (!chegou1) r1.x += SPEED1;
                if (!chegou2) r2.x += SPEED2;
                if (!chegou3) r3.x += SPEED3;
            }

            // Checar chegada
            if (r1.x + r1.w >= FINISH_LINE) {
                chegou1 = true;
                if (!vencedorAnunciado) { vencedor = 1; vencedorAnunciado = true; }
            }
            if (r2.x + r2.w >= FINISH_LINE) {
                chegou2 = true;
                if (!vencedorAnunciado) { vencedor = 2; vencedorAnunciado = true; }
            }
            if (r3.x + r3.w >= FINISH_LINE) {
                chegou3 = true;
                if (!vencedorAnunciado) { vencedor = 3; vencedorAnunciado = true; }
            }

            // Renderização
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderClear(ren);

            // Linha de chegada
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_Rect linha = { FINISH_LINE, 0, 5, SCREEN_HEIGHT };
            SDL_RenderFillRect(ren, &linha);

            // Corredores
            SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
            SDL_RenderFillRect(ren, &r1);

            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
            SDL_RenderFillRect(ren, &r2);

            SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
            SDL_RenderFillRect(ren, &r3);

            SDL_RenderPresent(ren);
        }

        if (!quit) {
            printf("O vencedor foi o objeto %d!\n", vencedor);
            SDL_Delay(2000); // esperar um pouco antes de reiniciar
        }
    }

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
