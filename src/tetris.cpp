#include "tetris.h"
#include "utils.h"
#include <iostream>
using namespace utils;
namespace games
{
    Tetris::Tetris(SDL_Renderer * rend, TTF_Font * fnt, int xPadding)
        : Game("Tetris"), renderer(rend), red(255), green(255), blue(255),
        speed(50), font(fnt), board( my_tetris::TetrisBoard(10, 22)),
        blockSize(40), sidebar(xPadding)
    {
        block.w = blockSize;
        block.h = blockSize;
    }

    Tetris::~Tetris()
    {
        // Free all resources
    }

    std::string Tetris::getEntry() const
    {
        std::cout << "Fetched " << name << std::endl;
        return name;
    }

    void Tetris::runGame()
    {

        // Audio options
        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8 *wavBuffer;
        SDL_AudioDeviceID deviceId;

        SDL_LoadWAV("./res/tetris.wav", &wavSpec, &wavBuffer, &wavLength);
        deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);

        unsigned int stage = 0;
        unsigned int score = 0;
        unsigned int linesBroken = 0;
        // Merge first shape
        setRandomColor(red, green, blue);
        board.mergeShape(my_tetris::getRandom(), 2,5);
        bool cont = true;
        unsigned int time = 0;
        while( cont )
        {
            SDL_Event event;
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                    cont = false;
                }
                else if( event.type == SDL_KEYDOWN )
                {
                    // Handle keypresses
                    switch ( event.key.keysym.sym )
                    {
                        case SDLK_q:
                            cont = false;
                            break;

                        case SDLK_SPACE:
                            board.rotateShape();
                            break;
                        case SDLK_DOWN:
                            if( !board.pushDown() )
                            {
                                setRandomColor(red, green, blue);
                                board.mergeShape(my_tetris::getRandom(), 2,5);
                            }
                            break;
                        case SDLK_LEFT:
                            board.moveLeft();
                            break;
                        case SDLK_RIGHT:
                            board.moveRight();
                            break;
                        default:
                            break;
                    }
                }
            }

            if( time >= speed )
            {
                if( !board.pushDown() )
                {
                    setRandomColor(red, green, blue);
                    board.mergeShape(my_tetris::getRandom(), 2,5);
                }

                if( ( board.checkRow()) != 0  )
                {
                  SDL_QueueAudio(deviceId, wavBuffer, wavLength);
                  SDL_PauseAudioDevice( deviceId, 0);
                }

                if( board.lost() )
                {
                    cont = false;
                }
                time = 0;
            }

            SDL_Delay( 8 );

            render();
            ++time;
            if( linesBroken >= 10 )
            {
                std::cout << "[Tetris] Stage " << stage + 2 << "!\n";
                stage += 1;
                //Leave the lines that went over 10
                linesBroken = linesBroken % 10;
                speed -= 5;
            }
        }
        std::cout << "[Tetris] Lines: " << stage * 10 + linesBroken <<
            ", Score: " << score << std::endl;
    }

    void Tetris::render()
    {
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor( renderer, 200, 200, 200, 255 );

        // Iterate over the board
        for(std::vector< std::vector<my_tetris::Square> >::size_type i = 2; i < board.board.size(); ++i)
        {
            for(std::vector<my_tetris::Square>::size_type j = 0; j < board.board[i].size(); ++j)
            {
                block.x = blockSize * j + j + sidebar;
                block.y = blockSize * (i-2) + (i-2);
                if( board.board[i][j].getType() == my_tetris::ACTIVE )
                {
                    if( board.board[i][j].getStuck() == true )
                    {
                        SDL_SetRenderDrawColor( renderer, 50, 50, 255, 255 );
                        SDL_RenderFillRect(renderer, &block);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor( renderer, red,
                                green, blue, 255 );
                        SDL_RenderFillRect(renderer, &block);
                    }
                }
                else if( board.board[i][j].getType() == my_tetris::PIVOT )
                {
                    SDL_SetRenderDrawColor( renderer, red, green, blue, 255 );
                    SDL_RenderFillRect(renderer, &block);
                }
                else if( board.board[i][j].getType() == my_tetris::DISABLED )
                {
                    SDL_SetRenderDrawColor( renderer, 20, 20, 20, 255 );
                    SDL_RenderFillRect(renderer, &block);
                }
            }
        }
        // Render the changes above
        SDL_RenderPresent( renderer );
    }
}
