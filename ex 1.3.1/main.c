#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char* args[]) {
  /* INICIALIZACAO */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* win = SDL_CreateWindow("Exercício 3",
  SDL_WINDOWPOS_UNDEFINED,
  SDL_WINDOWPOS_UNDEFINED,
  250, 150, SDL_WINDOW_SHOWN
  );
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

  /* EXECUÇÃO */
  SDL_Rect r = {0,0,10,10}; //retangulo 10x10
  float centro_x = 125, centro_y = 75;  //centro da tela
  float raio = 50;  //raio do círculo
  float angulo = 0.0;  //inicializa o ângulo
 
  float velocidade = 10.0; //velocidade linear (px/s)
  float omega = velocidade / raio; //velocidade angular (rad/s)

  Uint32 ultimo_tempo = SDL_GetTicks();
  Uint32 inicio = SDL_GetTicks();  //guarda o tempo inicial

  while (1) {
    Uint32 tempo_atual = SDL_GetTicks();
    float delta_t = (tempo_atual - ultimo_tempo) / 1000.0f; //em segundos
    ultimo_tempo = tempo_atual;

    if ((tempo_atual - inicio) >= 10000) { //limita o tempo da animação em 10s
        break;
      }

    angulo += omega * delta_t;  //atualiza de acordo com tempo real
    if (angulo >= 2 * M_PI) {
        angulo -= 2 * M_PI;
      }
 
    SDL_SetRenderDrawColor(ren,0,0,0,255); //cor de fundo preto
    SDL_RenderClear(ren);
    
    r.x = centro_x + raio * cos(angulo) - r.w / 2;
    r.y = centro_y + raio * sin(angulo) - r.h / 2;

    SDL_SetRenderDrawColor(ren, 255, 203, 219, 255);
    SDL_RenderFillRect(ren, &r);
    SDL_RenderPresent(ren);
  
    SDL_Delay(16); //~60FPS
    }
  /* FINALIZACAO */
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}
