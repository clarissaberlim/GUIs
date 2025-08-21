#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* args[])
{
 /* INICIALIZACAO */
 SDL_Init(SDL_INIT_EVERYTHING);
 SDL_Window* win = SDL_CreateWindow("Exercício 2",
SDL_WINDOWPOS_UNDEFINED,
SDL_WINDOWPOS_UNDEFINED,
600, 400, SDL_WINDOW_SHOWN
 );
 SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

 /* EXECUÇÃO */
 SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00); //cor de fundo branco
 SDL_RenderClear(ren);

 //circulo verde preenchido
 filledCircleRGBA(ren, 100, 100, 50, 0, 255, 0, 255); // (x, y, raio, R, G, B, A)

 //retangulo preenchido vermelho com a borda preta
 boxRGBA(ren, 200, 80, 300, 150, 255, 0, 0, 255); // preenchido
 rectangleRGBA(ren, 200, 80, 300, 150, 0, 0, 0, 255); // borda
 
 //linha espessa azul
 thickLineRGBA(ren, 50, 200, 300, 250, 5, 0, 0, 255, 255); // linha de (x1, y1) até (x2, y2) com espessura 5

 //elipse
 ellipseRGBA(ren, 500, 100, 60, 30, 255, 165, 0, 255); // laranja

 //triangulo
 Sint16 vx[3] = { 400, 500, 450 }; // coordenadas x dos vértices
 Sint16 vy[3] = { 300, 300, 200 }; // coordenadas y dos vértices
 filledPolygonRGBA(ren, vx, vy, 3, 128, 0, 128, 255); // roxo escuro

 SDL_RenderPresent(ren);
 SDL_Delay(7000);

 /* FINALIZACAO */
 SDL_DestroyRenderer(ren);
 SDL_DestroyWindow(win);
 SDL_Quit();

 return 0;
}
