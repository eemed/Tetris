#include "menu.h"
#include "tetris.h"
#include "utils.h"
using namespace utils;
namespace sdl_games
{
    Menu::Menu(TTF_Font * fnt, SDL_Renderer * renderer)
        : selected(0), size_m(0), fnt(fnt),
        textColor( { 80, 80, 80, 255 }),
        backgroundColor( { 20, 20, 20, 255 }),
        selectedColor( { 255, 255, 255, 255 } ), x(0), y(0), renderer(renderer)
    {
        games.push_back("Tetris");
        games.push_back("Quit");
        size_m += 2;
        updateTextures();
    }

    Menu::~Menu()
    {
        for(std::vector< SDL_Texture * >::size_type i = 0;
                i < textures.size(); ++i)
        {
            SDL_DestroyTexture( textures[i].first );
        }
    }

    void Menu::press()
    {
        games::Tetris  *t = NULL;
        //std::cout << selected << std::endl;
        switch( selected )
        {
            case 0:
                t = new(games::Tetris)(renderer, fnt, 200);
                t->runGame();
                delete t;
                break;

            default:
                break;
        }
    }

    void Menu::down()
    {
        if( selected + 1 < size_m )
        {
            selected += 1;
        }
        updateTextures();
    }

    void Menu::up()
    {
        if( selected != 0 )
        {
            selected -= 1;
        }
        updateTextures();
    }

    bool Menu::add(std::string g) // add to end
    {
        games.push_back( g );
        size_m += 1;
        updateTextures();
        return true;
    }

    bool Menu::add(std::string g, unsigned int i) // add to index i
    {
        if( i >= size_m )
        {
            return false;
        }
        else
        {
            games.push_back( g );
            size_m += 1;
            return true;
        }
    }

    bool Menu::del(std::string s) // Delete this menu entry
    {
        for(std::vector< std::string >::size_type i = 0; i < games.size(); ++i)
        {
            if( s == games[i] )
            {
                games.erase( games.begin() + i );
                return true;
            }
        }
        return false;
    }

    bool Menu::del(unsigned int i) //delete entry in this index
    {
        if( i >= size_m )
        {
            return false;
        }
        else
        {
            games.erase( games.begin() + i );
            return true;
        }
    }

    bool Menu::del() // Delete currently selected
    {
        games.erase( games.begin() + selected);
        return true;
    }

    unsigned int Menu::getSelected() const
    {
        return selected;
    }

    unsigned int Menu::size() const
    {
        return size_m;
    }

    bool Menu::updateTextures()
    {
        std::vector< std::pair< SDL_Texture *, SDL_Rect> > newTextures;
        for(std::vector< std::string >::size_type i = 0; i < games.size(); ++i)
        {
            SDL_Rect shadedRect;
            SDL_Surface* shaded;
            if( selected == i )
            {
                shaded = TTF_RenderText_Shaded( fnt, games[i].c_str(), selectedColor, backgroundColor );
            }
            else
            {
                shaded = TTF_RenderText_Shaded( fnt, games[i].c_str(), textColor, backgroundColor );
            }
            SDL_Texture* shadedTexture = surfaceToTexture( shaded, renderer);

            SDL_QueryTexture( shadedTexture , NULL, NULL, &shadedRect.w, &shadedRect.h );
            shadedRect.x = 100;
            //std::cout << games[i] << "\n";
            if( i == 0 )
            {
                shadedRect.y = 0;
            }
            else
            {
                shadedRect.y = newTextures[i - 1].second.y + newTextures[i - 1].second.h + 20;
            }
            newTextures.push_back( std::pair< SDL_Texture *, SDL_Rect>(shadedTexture, shadedRect));

        }
            //
            //
            //Free old
            //
            //
        textures = newTextures;
        return true;
    }
    void Menu::render() const
    {
        SDL_SetRenderDrawColor( renderer, 20, 20, 20, 255 );
        SDL_RenderClear( renderer );

        // Render here
        for( std::vector< SDL_Texture * >::size_type i = 0; i < textures.size(); ++i)
        {
            SDL_RenderCopy( renderer, textures[i].first, nullptr, &(textures[i].second) );
        }
        SDL_RenderPresent( renderer );
    }

    std::ostream & operator<<(std::ostream & out, const Menu & menu)
    {
        for(unsigned int i = 0; i < menu.size_m; ++i)
        {
            out << i+1 << ": " << menu.games[i] << std::endl;
        }
        return out;
    }
}
