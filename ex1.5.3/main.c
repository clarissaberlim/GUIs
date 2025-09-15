// O retângulo só se move com o movimento do mouse.
// Ao clicar com o botão esquerdo do mouse em qualquer lugar da janela, a direção do retângulo é invertida.
// Se ele estiver indo para a direita, passará a ir para a esquerda, e vice-versa. 

#include <SDL2/SDL.h>

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Orientada a Eventos",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       200, 100, SDL_WINDOW_SHOWN
                                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 0, 45, 10, 10 };                  // Posição inicial
    int direcao = 1;                                 // 1 para direita, -1 para esquerda
    int passo = 5;                                   // Pixels a mover por evento
                                                       
    
    SDL_Event evt;
    int rodando = 1;

    while (rodando) {
        SDL_WaitEvent(&evt);
        
        switch (evt.type) {
            case SDL_QUIT:
                rodando = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (evt.button.button == SDL_BUTTON_LEFT) {
                    // Inverte a direcao da animacao ao clicar
                    direcao *= -1;
                }
                break;
        }
        r.x += direcao * passo;

        if (direcao == 1 && r.x + r.w >= 200) {
            direcao = -1;
        } else if (direcao == -1 && r.x <= 0) {
            direcao = 1;
        }

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
