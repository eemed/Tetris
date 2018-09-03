#ifndef _TETRIS_SDL_
#define _TETRIS_SDL_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "game.h"
#include <string>
namespace games
{
    class Tetris : public utils::Game
    {
        private:
            SDL_Renderer * renderer;
            // Colors
            int red;
            int green;
            int blue;

            unsigned int speed; // DECREASE TO INCREASE SPEED
            TTF_Font * font;
            SDL_Rect block;
            my_tetris::TetrisBoard board;
            int blockSize;
            int sidebar;
        public:
            Tetris(SDL_Renderer *, TTF_Font *, int xPadding);
            ~Tetris();
            void runGame();
            void render();
            std::string getEntry() const;
    };
}
#endif
