#ifndef _TETRIS_BOARD_
#define _TETRIS_BOARD_

#include <vector>
#include <utility>
#include "shape.h"
namespace my_tetris
{
    class TetrisBoard
    {

        public:
            std::vector< std::vector<Square> > board;
            Shape currentShape;
            TetrisBoard(unsigned int x = 10, unsigned int y = 22);
            //Get pivot point of current shape
            std::pair<unsigned int, unsigned int> getPivot();
            std::pair<int, int> getXBound();
            std::pair<int, int> getYBound();
            bool mergeShape(Shape s, unsigned int x, unsigned int y);
            bool clearActive();
            bool rotateShape();
            //Push all squares down execpt stuck ones
            unsigned int lowestActiveRow();
            bool pushDown();
            bool checkStuck();
            bool lockAll();
            bool checkLeft();
            bool checkRight();
            std::pair<unsigned int, unsigned int> checkRotate();
            bool moveLeft();
            bool moveRight();
            int checkRow();
            bool pushStuck(unsigned int n);
            bool lost();
    };
}

#endif //_TETRIS_BOARD_
