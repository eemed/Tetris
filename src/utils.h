#ifndef _UTILS_SDL_GAMES_
#define _UTILS_SDL_GAMES_
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
namespace utils
{
    void setRandomColor(int & red, int & green, int & blue);
    void setColor(int * a, int & red, int & green, int & blue);

    bool initSDL();
    SDL_Window * createWindow(const int & posX, const int & posY,
            const int & sizeX, const int & sizeY);
    SDL_Renderer * createRenderer(SDL_Window *);
    void setupRenderer(SDL_Renderer *,const int & sizeX, const int & sizeY );
    TTF_Font * setupTTF( const std::string &fontName );
    SDL_Texture* surfaceToTexture( SDL_Surface* surf, SDL_Renderer *);
}
#endif //_UTILS_SDL_GAMES_
