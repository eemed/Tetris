#ifndef _GAME_
#define _GAME_
#include <string>
#include <iostream>
namespace utils
{
    class Game
    {
        protected:
            std::string name;

        public:
            Game(std::string name) : name(name) {};
            virtual ~Game() {};
            virtual void runGame() {};
            virtual std::string getEntry() const 
            { 
                //std::cout << name << std::endl;
                return name; 
            };
    };
}
#endif
