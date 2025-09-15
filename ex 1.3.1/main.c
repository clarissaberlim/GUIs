#include <SDL2/SDL.h>

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao em Ciclo",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       200, 100, SDL_WINDOW_SHOWN
                                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 0, 45, 10, 10 }; 		// Inicia o retângulo na borda esquerda
    float posX = r.x; 				// Posição do retângulo como float para maior precisão
    int direcao = 1; 				// 1 para direita, -1 para esquerda
    float velocidade = 10.0f; 			// 10 pixels por segundo

    Uint32 tempo_anterior = SDL_GetTicks();
    
    SDL_Event evt;
    int rodando = 1; 				// Flag para controlar o loop principal

    while (rodando) {
        if (SDL_WaitEventTimeout(&evt, 16)) { 	// Espera por até 16ms
            if (evt.type == SDL_QUIT) {
                rodando = 0;
            }
        }        
        // Calcula o tempo decorrido desde o último frame
        Uint32 tempo_atual = SDL_GetTicks();
        float delta_time = (tempo_atual - tempo_anterior) / 1000.0f;
        tempo_anterior = tempo_atual;

        posX += direcao * velocidade * delta_time;

        // Mudança de direção
        if (direcao == 1 && posX + r.w >= 200) {
            direcao = -1;
            posX = 200 - r.w; 			// Ajusta a posição para não ultrapassar a borda
        } else if (direcao == -1 && posX <= 0) {
            direcao = 1;
            posX = 0; 				// Ajusta a posição para não ultrapassar a borda
        }

        r.x = (int)posX; 			// Converte para int para o SDL_Rect

        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}
