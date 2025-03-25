#include "defs.h"
#include "player.h"
#include "graphic.h"

int main( int argc, char* args[] )
{
	Graphic graphic;
    graphic.init();
    Tank tank1(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
    Tank tank2(SCREEN_WIDTH / 2, 0);

    bool quit = false;
    // game loop
    while(!quit){
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT) quit = true;
        }
    }

}