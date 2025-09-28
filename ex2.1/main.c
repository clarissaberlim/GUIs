#include <SDL2/SDL.h>
#include <stdio.h>

// Função auxiliar de espera com timeout
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

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Multiplo Clique SDL",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       400, 200, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    int rodando = 1;
    SDL_Event evt;

    enum Estado {IDLE, WAIT_CLICKS};
    enum Estado estado = IDLE;

    int contador = 0;
    Uint32 timeout = 0;  // tempo restante até expirar (ms)

    while (rodando) {
        Uint32 tempo_espera = (estado == WAIT_CLICKS ? timeout : 1000);

        int tem_evento = AUX_WaitEventTimeout(&evt, &tempo_espera);

        if (tem_evento) {
            switch (evt.type) {
                case SDL_QUIT:
                    rodando = 0;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (estado == IDLE) {
                        estado = WAIT_CLICKS;
                        contador = 1;
                        timeout = 250; // inicia contagem
                    } else if (estado == WAIT_CLICKS) {
                        contador++;
                        timeout = 250; // reinicia janela
                    }
                    break;

                case SDL_USEREVENT:
                    printf("Evento de clique multiplo detectado: %d cliques\n", evt.user.code);
                    break;
            }
        } else {
            // Timeout expirou
            if (estado == WAIT_CLICKS) {
                // Envia evento personalizado
                SDL_Event user_evt;
                SDL_zero(user_evt);
                user_evt.type = SDL_USEREVENT;
                user_evt.user.code = contador;
                SDL_PushEvent(&user_evt);

                // Reinicia estado
                estado = IDLE;
                contador = 0;
            }
        }

        // Renderização simples
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        // Desenha quadrados conforme nº de cliques (enquanto forem multiplos, depois apaga)
        for (int i = 0; i < contador; i++) {
            SDL_Rect r = {50 + i * 50, 80, 40, 40};
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
            SDL_RenderFillRect(ren, &r);
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
