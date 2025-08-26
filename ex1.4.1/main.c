#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    
    SDL_Rect retFixos[10];
    int numRet = 0;
    
    SDL_Event evt;
    
    while (1) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        
        for (int i = 0; i < numRet; i++) {
        	SDL_SetRenderDrawColor(ren, (i*40)%256, (i*80)%256, (i*120)%256, 0xFF);
        	SDL_RenderFillRect(ren, &retFixos[i]);
        }

        SDL_RenderPresent(ren);
        SDL_WaitEvent(&evt);
        
        if (evt.type == SDL_QUIT) {
        	break;
        }
        
        if (evt.type == SDL_MOUSEBUTTONDOWN) {
        	if (evt.button.button == SDL_BUTTON_LEFT && numRet < 10) {
        		SDL_Rect novo = {evt.button.x, evt.button.y, 10, 10};
        		retFixos[numRet++] = novo;
        	}
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
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
