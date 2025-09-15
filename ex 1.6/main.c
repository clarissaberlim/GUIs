#include <SDL2/SDL.h>
#include <stdio.h>

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

// Protótipo da função de reinício
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
    // Posições e tamanhos iniciais dos retângulos
    SDL_Rect r_tempo = { 0, 20, 10, 10 }; // Vermelho
    SDL_Rect r_teclado = { 0, 80, 10, 10 }; // Azul
    SDL_Rect r_mouse = { 0, 140, 10, 10 }; // Verde
    
    // Linha de chegada
    const int LINHA_CHEGADA_X = 350;

    int vencedor = 0;
    int chegados = 0;

    // Velocidade dos retângulos
    float velocidade_tempo = 0.5;
    const int VELOCIDADE_TECLADO = 5;
    
    Uint32 tempo_espera_tempo = 16; // Tempo inicial de espera para o retângulo de tempo
    
    SDL_Event evt;

    while (1) {
        int evento_ocorreu = AUX_WaitEventTimeout(&evt, &tempo_espera_tempo);
        
        if (evento_ocorreu) {
            if (evt.type == SDL_QUIT) {
                break;
            }
            // Lógica de movimento para os retângulos de teclado e mouse
            if (chegados < 3) {
                if (evt.type == SDL_KEYDOWN) {
                    if (evt.key.keysym.sym == SDLK_SPACE) {
                        r_teclado.x += VELOCIDADE_TECLADO;
                    }
                }
                
                if (evt.type == SDL_MOUSEMOTION) {
                    r_mouse.x = evt.motion.x - (r_mouse.w / 2);
                }
            }
        } else {
            // Lógica do timeout para o retângulo de tempo
            if (chegados < 3) {
                r_tempo.x += velocidade_tempo;
            }
            tempo_espera_tempo = 16; // Tempo para 60 FPS
        }

        // Lógica de verificação de chegada
        if (chegados < 3) {
            if (r_tempo.x >= LINHA_CHEGADA_X) {
                if (vencedor == 0) vencedor = 1;
                chegados |= 1;
            }
            
            if (r_teclado.x >= LINHA_CHEGADA_X) {
                if (vencedor == 0) vencedor = 2;
                chegados |= 2;
            }
            
            if (r_mouse.x >= LINHA_CHEGADA_X) {
                if (vencedor == 0) vencedor = 3;
                chegados |= 4;
            }
        }

        // Se todos os retângulos chegaram, reinicia a corrida
        if ((chegados & 1) && (chegados & 2) && (chegados & 4)) {
            reiniciar_corrida(&r_tempo, &r_teclado, &r_mouse, &vencedor, &chegados);
        }

        // Renderização
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
    
    SDL_Delay(3000);

    r1->x = 0;
    r2->x = 0;
    r3->x = 0;
    
    *vencedor = 0;
    *chegados = 0;
}
