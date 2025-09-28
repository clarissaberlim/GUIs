#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define CHEGADA   500

// Função auxiliar para contagem de tempo
int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms) {
    if (*ms <= 0) {
        return 0;
    }

    Uint32 tempo_antes = SDL_GetTicks();
    int evento_ocorreu = SDL_WaitEventTimeout(evt, *ms);
    Uint32 tempo_depois = SDL_GetTicks();
    Uint32 tempo_decorrido = tempo_depois - tempo_antes;

    if (*ms > tempo_decorrido) {
        *ms -= tempo_decorrido;
    } else {
        *ms = 0;
    }

    return evento_ocorreu;
}

int main(int argc, char* args[]) {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Corrida",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        600, 200,
                        SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect r1 = { 20, 40, 20, 20 };   // Azul: só pelo tempo
    SDL_Rect r2 = { 20, 90, 20, 20 };   // Vermelho: teclado
    SDL_Rect r3 = { 20, 140, 20, 20 };  // Verde: mouse

    bool quit = false;
    SDL_Event e;

    srand(time(NULL));

    while (!quit) {/
        // Reset posição inicial
        r1.x = r2.x = r3.x = 20;
        bool chegou1 = false, chegou2 = false, chegou3 = false;
        bool vencedorAnunciado = false;
        int vencedor = 0;

        // Velocidade fixa de r1
        int speed1 = rand() % 5 + 1;

        // Controle de tempo do r1
        Uint32 lastTick = SDL_GetTicks();
        Uint32 intervalo = 30; // ms entre passos do r1

        while (!(chegou1 && chegou2 && chegou3) && !quit) {
            Uint32 ms = 30;
            int hasEvent = AUX_WaitEventTimeout(&e, &ms);

            // Trata eventos
            if (hasEvent) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_RIGHT) {
                        if (!chegou2) r2.x += 5;
                    }
                }
                else if (e.type == SDL_MOUSEMOTION) {
                    if (!chegou3) r3.x = e.motion.x;
                }
            }

            // Movimento de r1 controlado por tempo real
            Uint32 now = SDL_GetTicks();
            if (!chegou1 && now - lastTick >= intervalo) {
                r1.x += speed1;
                lastTick = now;
            }

            // Checagem de chegada
            if (r1.x + r1.w >= CHEGADA) {
                chegou1 = true;
                if (!vencedorAnunciado) { vencedor = 1; vencedorAnunciado = true; }
            }
            if (r2.x + r2.w >= CHEGADA) {
                chegou2 = true;
                if (!vencedorAnunciado) { vencedor = 2; vencedorAnunciado = true; }
            }
            if (r3.x + r3.w >= CHEGADA) {
                chegou3 = true;
                if (!vencedorAnunciado) { vencedor = 3; vencedorAnunciado = true; }
            }

            // Renderização
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderClear(ren);

            // Linha de chegada
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_Rect linha = { CHEGADA, 0, 5, 200 };
            SDL_RenderFillRect(ren, &linha);

            // Retângulos
            SDL_SetRenderDrawColor(ren, 0, 0, 255, 255); // Azul
            SDL_RenderFillRect(ren, &r1);

            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Vermelho
            SDL_RenderFillRect(ren, &r2);

            SDL_SetRenderDrawColor(ren, 0, 255, 0, 255); // Verde
            SDL_RenderFillRect(ren, &r3);

            SDL_RenderPresent(ren);
        }

        if (!quit) {
            printf("O vencedor foi o objeto %d!\n", vencedor);
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
