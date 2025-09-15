#include <SDL2/SDL.h>

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

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Retângulos",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       400, 200, SDL_WINDOW_SHOWN
                                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    // r (Azul) - Move com o teclado
    SDL_Rect r = { 40,20, 10,10 };
    // r2 (Vermelho) - Acompanha o mouse
    SDL_Rect r2 = { 60,50, 10,20 };
    // r3 (Verde) - Move pelo tempo
    SDL_Rect r3 = { 20,80, 20,10 };
    
    SDL_Event evt;
    Uint32 tempo_espera_r3 = 500; // Tempo inicial de espera para o retângulo verde

    while (1) {
        int evento_ocorreu = AUX_WaitEventTimeout(&evt, &tempo_espera_r3);
        
        if (evento_ocorreu) {
            // Lógica de evento para os retângulos r (teclado) e r2 (mouse)
            if (evt.type == SDL_QUIT) {
                break;
            }
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        if (r.y - 5 >= 0) {
                            r.y -= 5;
                        }
                        break;
                    case SDLK_DOWN:
                        if (r.y + r.h + 5 <= 200) {
                            r.y += 5;
                        }
                        break;
                    case SDLK_LEFT:
                        if (r.x - 5 >= 0) {
                            r.x -= 5;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (r.x + r.w + 5 <= 400) {
                            r.x += 5;
                        }
                        break;
                }
            }
            if (evt.type == SDL_MOUSEMOTION) {
                // Posiciona r2 na coordenada do mouse
                r2.x = evt.motion.x;
                r2.y = evt.motion.y;
            }
        } else {
            // Lógica do timeout para o retângulo r3
            tempo_espera_r3 = 500;
            r3.x += 2;
            r3.y += 2;
            
            // Lógica para o retângulo verde não sair da tela
            if (r3.x > 400) r3.x = 0;
            if (r3.y > 200) r3.y = 0;
        }
        
        // Renderização
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 0x00); // Azul
        SDL_RenderFillRect(ren, &r);

        SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0x00); // Vermelho
        SDL_RenderFillRect(ren, &r2);

        SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x00, 0x00); // Verde
        SDL_RenderFillRect(ren, &r3);

        SDL_RenderPresent(ren);
    }
    
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
