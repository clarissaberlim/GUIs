#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

int main(int argc, char* args[])
{
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* win = SDL_CreateWindow("Animacao com SpriteSheet",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       400, 200, SDL_WINDOW_SHOWN);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* img = IMG_Load("image.png");
    if (!img) {
        printf("Erro ao carregar imagem: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, img);
    SDL_FreeSurface(img);

    int frame_largura = 128; // ajuste ao tamanho do sprite
    int frame_altura  = 128;
    int total_frames  = 4;
    int frame_atual   = 0;

    SDL_Rect src = {0, 0, frame_largura, frame_altura};
    SDL_Rect dst = {0, 50, frame_largura, frame_altura};

    int direcao = 1;                                 // 1 direita, -1 esquerda
    int velocidade = 100;                            // pixels por segundo

    Uint32 ultimo_tempo_anim = SDL_GetTicks();
    Uint32 intervalo_anim = 120;                     // ms entre troca de frames

    Uint32 ultimo_tempo_mov = SDL_GetTicks();

    int rodando = 1;
    SDL_Event evt;

    while (rodando) {
        // --- intervalo de espera até o próximo frame ---
        Uint32 tempo_espera = 16; // ~60fps

        // Usa a função auxiliar
        int tem_evento = AUX_WaitEventTimeout(&evt, &tempo_espera);

        if (tem_evento) {
            switch (evt.type) {
                case SDL_QUIT:
                    rodando = 0;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (evt.button.button == SDL_BUTTON_LEFT) {
                        direcao *= -1; // inverte direção
                    }
                    break;

                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        // redesenho forçado
                    }
                    break;
            }
        }

        // --- atualização da lógica ---
        Uint32 agora = SDL_GetTicks();
        float delta = (agora - ultimo_tempo_mov) / 1000.0f; // segundos
        ultimo_tempo_mov = agora;

        dst.x += (int)(direcao * velocidade * delta);

        if (direcao == 1 && dst.x + dst.w >= 400) {
            direcao = -1;
        } else if (direcao == -1 && dst.x <= 0) {
            direcao = 1;
        }

        if (agora - ultimo_tempo_anim >= intervalo_anim) {
            frame_atual = (frame_atual + 1) % total_frames;
            src.x = frame_atual * frame_largura;
            ultimo_tempo_anim = agora;
        }

        // --- desenho ---
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, &src, &dst);
        SDL_RenderPresent(ren);
    }

    /* FINALIZAÇÃO */
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
