#include <SDL2/SDL.h>
#include <stdio.h>

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

void reiniciar_corrida(SDL_Rect *r1, SDL_Rect *r2, SDL_Rect *r3, int *vencedor, int *chegados);

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Corrida de Retângulos",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       400, 200, SDL_WINDOW_SHOWN
                                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r_tempo = { 0, 20, 10, 10 }; // Vermelho
    SDL_Rect r_teclado = { 0, 80, 10, 10 }; // Azul
    SDL_Rect r_mouse = { 0, 140, 10, 10 }; // Verde
    
    const int LINHA_CHEGADA_X = 350;

    int vencedor = 0; // 0: Ninguém, 1: Tempo, 2: Teclado, 3: Mouse
    int chegados = 0;

    float velocidade_tempo = 0.5;
    int move_teclado_x = 0;
    int move_mouse_x = 0;
    
    Uint32 tempo_espera_tempo = 500;
    
    SDL_Event evt;

    while (1) {
        int evento_ocorreu = AUX_WaitEventTimeout(&evt, &tempo_espera_tempo);
        
        if (evento_ocorreu) {
            if (evt.type == SDL_QUIT) {
                break;
            }
            if (evt.type == SDL_KEYDOWN) {
                if (chegados < 3) {
                    if (evt.key.keysym.sym == SDLK_SPACE) {
                        move_teclado_x = 1;
                    }
                }
            } else {
                if (move_teclado_x > 0) {
                    move_teclado_x = 0;
                }
            }
            
            if (evt.type == SDL_MOUSEMOTION) {
                if (chegados < 3) {
                    r_mouse.x = evt.motion.x - (r_mouse.w / 2);
                }
            }
        } else {
            if (chegados < 3) {
                r_tempo.x += velocidade_tempo;
            }
            tempo_espera_tempo = 1000 / 60; // 60 FPS
        }

        if (chegados < 3) {
            if (r_tempo.x >= LINHA_CHEGADA_X && !(chegados & 1)) {
                if (vencedor == 0) vencedor = 1;
                chegados |= 1;
            }
            if (move_teclado_x) {
                r_teclado.x += 2;
                if (r_teclado.x >= LINHA_CHEGADA_X && !(chegados & 2)) {
                    if (vencedor == 0) vencedor = 2;
                    chegados |= 2;
                }
            }
            if (r_mouse.x >= LINHA_CHEGADA_X && !(chegados & 4)) {
                if (vencedor == 0) vencedor = 3;
                chegados |= 4;
            }
        }

        // Se todos os retângulos chegaram, reinicia a corrida
        if (chegados == 7) { // 1+2+4 = 7, todos chegaram
            reiniciar_corrida(&r_tempo, &r_teclado, &r_mouse, &vencedor, &chegados);
        }

        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderDrawLine(ren, LINHA_CHEGADA_X, 0, LINHA_CHEGADA_X, 200);

        SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0x00); // Vermelho (tempo)
        SDL_RenderFillRect(ren, &r_tempo);

        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 0x00); // Azul (teclado)
        SDL_RenderFillRect(ren, &r_teclado);

        SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x00, 0x00); // Verde (mouse)
        SDL_RenderFillRect(ren, &r_mouse);

        SDL_RenderPresent(ren);
    }
    
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}

// Função de reinício da corrida
void reiniciar_corrida(SDL_Rect *r1, SDL_Rect *r2, SDL_Rect *r3, int *vencedor, int *chegados) {
    printf("Corrida finalizada! O vencedor é: ");
    if (*vencedor == 1) {
        printf("Retângulo Vermelho (Tempo)!\n");
    } else if (*vencedor == 2) {
        printf("Retângulo Azul (Teclado)!\n");
    } else if (*vencedor == 3) {
        printf("Retângulo Verde (Mouse)!\n");
    } else {
        printf("Nenhum.\n");
    }
    
    SDL_Delay(3000); // Aguarda 3 segundos para exibir o resultado

    r1->x = 0;
    r2->x = 0;
    r3->x = 0;
    *vencedor = 0;
    *chegados = 0;
}
