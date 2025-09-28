#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

// Função auxiliar de espera com timeout
int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms) {
    if (*ms <= 0) return 0;

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

    SDL_Window* win = SDL_CreateWindow("Click, Drag ou Cancel",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 480, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // Quadrado inicial
    SDL_Rect quadrado = {200, 200, 80, 80};
    SDL_Rect pos_inicial = quadrado;

    bool rodando = true;
    SDL_Event evt;

    enum Estado { INATIVO, PRESSIONADO, ARRASTANDO };
    enum Estado estado = INATIVO;

    int offsetX = 0, offsetY = 0; // para drag
    int startX = 0, startY = 0;   // posição do mouse no início
    const int DRAG_THRESHOLD = 5; // pixels para distinguir clique de drag

    while (rodando) {
        Uint32 espera = 16; // ~60fps
        int tem_evento = AUX_WaitEventTimeout(&evt, &espera);

        if (tem_evento) {
            switch (evt.type) {
                case SDL_QUIT:
                    rodando = false;
                    break;

                case SDL_KEYDOWN:
                    if (evt.key.keysym.sym == SDLK_ESCAPE) {
                        if (estado == PRESSIONADO || estado == ARRASTANDO) {
                            quadrado = pos_inicial; // volta à posição original
                            estado = INATIVO;
                            printf("Cancelado com ESC\n");
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (evt.button.button == SDL_BUTTON_LEFT) {
                        if (SDL_PointInRect(&(SDL_Point){evt.button.x, evt.button.y}, &quadrado)) {
                            estado = PRESSIONADO;
                            startX = evt.button.x;
                            startY = evt.button.y;
                            offsetX = evt.button.x - quadrado.x;
                            offsetY = evt.button.y - quadrado.y;
                            pos_inicial = quadrado; // salva posição original
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (estado == PRESSIONADO) {
                        int dx = evt.motion.x - startX;
                        int dy = evt.motion.y - startY;
                        if (dx*dx + dy*dy > DRAG_THRESHOLD*DRAG_THRESHOLD) {
                            estado = ARRASTANDO;
                            printf("Entrou em ARRASTANDO\n");
                        }
                    }
                    if (estado == ARRASTANDO) {
                        quadrado.x = evt.motion.x - offsetX;
                        quadrado.y = evt.motion.y - offsetY;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (evt.button.button == SDL_BUTTON_LEFT) {
                        if (estado == PRESSIONADO) {
                            printf("Clique detectado!\n");
                        } else if (estado == ARRASTANDO) {
                            printf("Quadrado solto em nova posição\n");
                        }
                        estado = INATIVO;
                    }
                    break;
            }
        }

        // Renderização
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_RenderFillRect(ren, &quadrado);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
