#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* args[]) {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Exercício 3 - Orientado a Eventos",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        250, 150, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = {0, 0, 10, 10}; // retângulo 10x10
    float centro_x = 125, centro_y = 75;  // centro da tela
    float raio = 50;  // raio do círculo
    float angulo = 0.0;  // inicializa o ângulo

    float velocidade = 10.0; // velocidade linear (px/s)
    float omega = velocidade / raio; // velocidade angular (rad/s)

    Uint32 ultimo_tempo = SDL_GetTicks();
    Uint32 inicio = SDL_GetTicks();  // tempo inicial

    SDL_Event e;
    int rodando = 1;

    // Cor inicial (rosa)
    Uint8 r_cor = 255, g_cor = 203, b_cor = 219;

    while (rodando) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                rodando = 0;
            }
            else (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    rodando = 0; // fecha no ESC
                }
                else if (e.key.keysym.sym == SDLK_SPACE) {
                    // Evento novo: alternar cor ao pressionar espaço
                    if (r_cor == 255) { // estava rosa
                        r_cor = 100; g_cor = 255; b_cor = 100; // verde
                    } else {
                        r_cor = 255; g_cor = 203; b_cor = 219; // rosa
                    }
                }
            }
        }

        Uint32 tempo_atual = SDL_GetTicks();
        float delta_t = (tempo_atual - ultimo_tempo) / 1000.0f; // em segundos
        ultimo_tempo = tempo_atual;

        if ((tempo_atual - inicio) >= 10000) { // limite 10s
            rodando = 0;
        }

        angulo += omega * delta_t;
        if (angulo >= 2 * M_PI) {
            angulo -= 2 * M_PI;
        }

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // fundo preto
        SDL_RenderClear(ren);

        r.x = centro_x + raio * cos(angulo) - r.w / 2;
        r.y = centro_y + raio * sin(angulo) - r.h / 2;

        SDL_SetRenderDrawColor(ren, r_cor, g_cor, b_cor, 255);
        SDL_RenderFillRect(ren, &r);

        SDL_RenderPresent(ren);

        SDL_Delay(16); // ~60FPS
    }

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
