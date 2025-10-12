#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

// Função auxiliar para esperar evento com timeout
int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms) {
    if (*ms <= 0) return 0;

    Uint32 antes = SDL_GetTicks();
    int ocorreu = SDL_WaitEventTimeout(evt, *ms);
    Uint32 depois = SDL_GetTicks();

    Uint32 decorrido = depois - antes;
    if (*ms > decorrido) *ms -= decorrido;
    else *ms = 0;

    return ocorreu;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Evento de Múltiplos Cliques",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       500, 300, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event evt;

    enum Estado { IDLE, WAIT_CLICKS, CANCELLED };
    enum Estado estado = IDLE;

    int rodando = 1;
    int contador = 0;
    Uint32 timeout = 0;
    int x_inicial = 0, y_inicial = 0;

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
                        timeout = 250;
                        x_inicial = evt.button.x;
                        y_inicial = evt.button.y;
                    } else if (estado == WAIT_CLICKS) {
                        contador++;
                        timeout = 250;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (estado == WAIT_CLICKS) {
                        int dx = evt.motion.x - x_inicial;
                        int dy = evt.motion.y - y_inicial;
                        if (dx * dx + dy * dy > 4) {
                            estado = CANCELLED;
                            contador = 0;
                            printf("Movimento detectado — clique múltiplo cancelado.\n");
                        }
                    }
                    break;

                case SDL_USEREVENT:
                    printf("Clique múltiplo detectado: %d cliques!\n", evt.user.code);
                    break;
            }
        } else {
            if (estado == WAIT_CLICKS) {
                SDL_Event user_evt;
                SDL_zero(user_evt);
                user_evt.type = SDL_USEREVENT;
                user_evt.user.code = contador;
                SDL_PushEvent(&user_evt);

                estado = IDLE;
                contador = 0;
            }
        }

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        for (int i = 0; i < contador; i++) {
            SDL_Rect r = {50 + i * 60, 120, 50, 50};
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
