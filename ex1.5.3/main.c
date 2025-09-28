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
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Orientada a Eventos",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       200, 100, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 0, 45, 10, 10 };  // posição inicial
    int direcao = -1;                // começa indo para a esquerda
    int passo = 5;                   // pixels a mover
    int rodando = 1;

    SDL_Event evt;
    Uint32 intervalo = 50;           // tempo em ms entre movimentos
    Uint32 ultimo_movimento = SDL_GetTicks();

    while (rodando) {
        Uint32 agora = SDL_GetTicks();
        Uint32 timeout = (intervalo > (agora - ultimo_movimento))
                          ? intervalo - (agora - ultimo_movimento)
                          : 0;

        int houve_evento = AUX_WaitEventTimeout(&evt, &timeout);

        if (houve_evento) {
            switch (evt.type) {
                case SDL_QUIT:
                    rodando = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (evt.button.button == SDL_BUTTON_LEFT) {
                        direcao *= -1;  // inverte direção no clique
                    }
                    break;
                default:
                    // Ignora eventos irrelevantes
                    break;
            }
        }

        // Calcula tempo desde o último movimento
        agora = SDL_GetTicks();
        Uint32 delta = agora - ultimo_movimento;

        // Move o retângulo em velocidade regular, sem alteração por conta de eventos
        while (delta >= intervalo) {
            r.x += direcao * passo;

            // Bateu nas bordas? Inverte direção
            if (direcao == 1 && r.x + r.w >= 200) {
                direcao = -1;
            } else if (direcao == -1 && r.x <= 0) {
                direcao = 1;
            }

            ultimo_movimento += intervalo;
            delta = agora - ultimo_movimento;
        }

        // Desenhar
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);
    }

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
