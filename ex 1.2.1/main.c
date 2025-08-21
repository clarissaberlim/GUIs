#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* args[])
{
 /* INICIALIZACAO */
 SDL_Init(SDL_INIT_EVERYTHING);
 SDL_Window* win = SDL_CreateWindow("Exercício 1",
SDL_WINDOWPOS_UNDEFINED,
SDL_WINDOWPOS_UNDEFINED,
250, 150, SDL_WINDOW_SHOWN
 );
 SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

 /* EXECUÇÃO */
 SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00); //cor de fundo branco
 SDL_RenderClear(ren);

 SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00); //cor azul
 
 SDL_Rect r = { 40,20, 20,10 }; //retangulo em (40,20)
 SDL_RenderFillRect(ren, &r);

 SDL_RenderDrawLine(ren, 120, 130, 220, 130);  //linha de (120, 130) a (220, 130) - horizontal

 SDL_RenderDrawPoint(ren, 220, 10);  //ponto em (220, 10)

 SDL_SetRenderDrawColor(ren, 0xFF,0xC0,0xCB,0xFF); //cor rosa
 
 SDL_Rect r2 = { 80,60, 50,60 }; //retangulo em (80,60)
 SDL_RenderFillRect(ren, &r2);

 SDL_RenderDrawLine(ren, 230, 20, 230, 120);  //linha de (230, 20) a (230, 120) - vertical

 SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF);  //cor vermelho

 SDL_Rect r3 = { 120,80, 40,30 }; //retangulo em (120,80)
 SDL_RenderFillRect(ren, &r3); 

 SDL_RenderDrawPoint(ren, 200, 100);  //ponto em (100, 100)

 SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x00, 0xFF);  //cor verde

 SDL_Rect r4 = { 200,10, 10,90 }; //retangulo em (240,120)
 SDL_RenderFillRect(ren, &r4);

 SDL_RenderDrawLine(ren, 50, 50, 150, 150);  //linha de (50, 50) a (150, 150) - inclinada

 SDL_RenderPresent(ren);
 SDL_Delay(5000);

 /* FINALIZACAO */
 SDL_DestroyRenderer(ren);
 SDL_DestroyWindow(win);
 SDL_Quit();
}
