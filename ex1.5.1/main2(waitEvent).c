#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Retângulos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 200, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect r2 = { 60,50, 10,20 };
    SDL_Rect r3 = { 20,80, 20,10 };
    
    SDL_Event evt;
    int espera = 500;
    
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00); //fundo branco
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);

        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r2);

        SDL_SetRenderDrawColor(ren, 0x00,0xFF,0x00,0x00);
        SDL_RenderFillRect(ren, &r3);

        SDL_RenderPresent(ren);
        
        SDL_WaitEvent(&evt);
        Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);
        
        if(isevt){
            espera -= (SDL_GetTicks()-antes);
            if (evt.type == SDL_QUIT) {
            	break;
            }
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                      if (r.y - 5 >= 0) {
                	      r.y -= 5;
                      }
                    break;
                    case SDLK_DOWN:
                	    if (r.y + r.h + 5 <= 100) {
                		    r.y += 5;
                	    }
                        break;
                    case SDLK_LEFT:
                	    if (r.x - 5 >= 0) {
                    		r.x -= 5;
                    	}
                        break;
                    case SDLK_RIGHT:
                        if (r.x + r.w + 5 <= 200) {
                	    r.x += 5;
                	    }
                        break;
                }
            }
            if(evt.type == SDL_MOUSEMOTION){
                int mX = 0;
                int mY = 0;
                SDL_GetMouseState(&mX, &mY);
                r2.x = mX;
                r2.y = mY;
            }
        } else {
          espera = 500;
          r3.x +=2;
          r3.y +=2;
      }
  }       

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
