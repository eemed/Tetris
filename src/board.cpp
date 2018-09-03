#include "board.h"
namespace my_tetris
{
    TetrisBoard::TetrisBoard(unsigned int x, unsigned int y)
        : board(std::vector< std::vector<Square> >(
            y, std::vector<Square>(x, Square(DISABLED)))), currentShape(Shape(NONE))
    {
    }

    //There should be only one pivot block on board at a time
    std::pair<unsigned int, unsigned int> TetrisBoard::getPivot()
    {
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( board[i][j].getType() == PIVOT )
                    return std::pair<unsigned int, unsigned int>(i,j);
            }
        }
        return std::pair<unsigned int, unsigned int>(0,0);
    }

    std::pair<int, int> TetrisBoard::getXBound()
    {
        int left = board[0].size();
        int right = 0;
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( ( board[i][j].getType() == ACTIVE or
                      board[i][j].getType() == PIVOT ) and
                      board[i][j].getStuck() == false )
                {
                    if( left > (int)j )
                    {
                        left = (int)j;
                    }
                    if( right < (int)j )
                    {
                        right = (int)j;
                    }
                }
            }
        }
        return std::pair<int, int>(left,right);
    }

    std::pair<int, int> TetrisBoard::getYBound()
    {
        int up = (int) board.size();
        int down = 0;
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( ( board[i][j].getType() == ACTIVE or
                      board[i][j].getType() == PIVOT ) and
                      board[i][j].getStuck() == false )
                {
                    if( up > (int)i )
                    {
                        up = (int)i;
                    }
                    if( down < (int)i )
                    {
                        down = (int)i;
                    }
                }
            }
        }
        return std::pair<int, int>(up,down);
    }
    /*
     * @param s,  shape to merge
     * @param x,  x coord of pivot block
     * @param y,  y coord of pivot block
     * */
    bool TetrisBoard::mergeShape(Shape s, unsigned int x, unsigned int y)
    {
        currentShape = s;
        //std::cout << "Merge to >> " << x << "," << y << std::endl << s;
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < s.getSquares().size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < s.getSquares()[i].size(); ++j)
            {
                if( s.getSquares()[i][j].getType() == ACTIVE )
                {
                    if ( board[i+x-2][j+y-2].getStuck() == true )
                    {
                        //std::cout << "x >> " << i+x-2;
                        //std::cout << "y >> " << j+y-2 << std::endl;
                        return false;
                    }
                }
            }
        }
        board[x][y].setType(PIVOT);
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < s.getSquares().size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < s.getSquares()[i].size(); ++j)
            {
                if( s.getSquares()[i][j].getType() == ACTIVE )
                {
                    /*
                    std::cout << "x: " << i + x - 2 << ", y: " <<
                        j + y - 2 << std::endl;
                    */
                    board[i+x-2][j+y-2].setType(ACTIVE);
                }
            }
        }
        return true;
    }

    bool TetrisBoard::clearActive()
    {
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( ( board[i][j].getType() == ACTIVE or
                      board[i][j].getType() == PIVOT ) and
                      board[i][j].getStuck() == false )
                {
                    board[i][j].setType(DISABLED);
                }
            }
        }
        return true;
    }

    std::pair<unsigned int, unsigned int> TetrisBoard::checkRotate()
    {
        std::pair<int, int> bound = getYBound();
        std::pair<unsigned int, unsigned int> piv = getPivot();
        int posX = piv.first;
        int posY = piv.second;
        if( piv.second < 2 and ( posY <= (int)(bound.second - piv.first) ))
        {
            //std::cout << "first: " << piv.first << ", second: " << piv.second << ", boundF: " << bound.first << ", BSecond: " << bound.second << std::endl;
            posY = bound.second - piv.first;
        }
        if( piv.second > board[0].size() - 2 )
        {
            //std::cout << "first: " << piv.first << ", second: " << piv.second << ", boundF: " << bound.first << ", BSecond: " << bound.second << std::endl;
            posY -= piv.first - bound.first;
        }
        //std::cout << "first: " << piv.first << ", second: " << piv.second << ", boundF: " << bound.first << ", BSecond: " << bound.second << std::endl;
        //std::cout << "posX " << posX << ", posY: " << posY << std::endl;
        return std::pair<int, int>(posX, posY);
    }

    bool TetrisBoard::rotateShape()
    {
        std::pair<int, int> original = getPivot();
        std::pair<int, int> piv = checkRotate();
        //std::cout << "x: " << piv.first << ", y: " << piv.second << std::endl;
        currentShape.rotate();
        clearActive();
        if( !mergeShape(currentShape, piv.first, piv.second) )
        {
            clearActive();
            currentShape.rotate();
            currentShape.rotate();
            currentShape.rotate();
            //std::cout << "Couldnt fit merge back to place" << std::endl;
            mergeShape(currentShape, original.first, original.second);
            return false;
        }
        return true;
    }

    unsigned int TetrisBoard::lowestActiveRow()
    {
        unsigned int low = 0;
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( board[i][j].getType() == ACTIVE or
                    board[i][j].getType() == PIVOT )
                {
                    low = i;
                }
            }
        }
        return low;

    }

    bool TetrisBoard::lockAll()
    {
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( board[i][j].getType() == ACTIVE or
                    board[i][j].getType() == PIVOT )
                {
                    board[i][j].setStuck(true);
                    board[i][j].setType(ACTIVE);
                }
                else
                {
                    board[i][j].setStuck(false);
                }
            }
        }
        return true;
    }

    bool TetrisBoard::checkStuck()
    {
        for(std::vector<Square>::size_type j = 0; j < board[board.size() - 1].size(); ++j)
        {
            if( ( board[board.size() - 1][j].getType() == ACTIVE or
                  board[board.size() - 1][j].getType() == PIVOT ) and
                  board[board.size() - 1][j].getStuck() == false )
            {
                //std::cout << "Row index 19!" << std::endl;
                return lockAll();
            }
        }
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size() - 1; ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( ( board[i][j].getType() == ACTIVE or
                       board[i][j].getType() == PIVOT ) and
                    ( board[i+1][j].getStuck() == true and
                      board[i][j].getStuck() == false ) )
                {
                    //std::cout << "Shape stuck!" << std::endl;
                    return lockAll();;
                }
            }
        }
        return false;
    }

    bool TetrisBoard::pushDown()
    {
        if( !checkStuck() )
        {
            for(std::vector< std::vector<Square> >::size_type i = lowestActiveRow();
                    i > 0; --i)
            {
                for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
                {
                    if ( i != board.size() -1 and
                            board[i][j].getStuck() == false )
                    {
                        if (  board[i][j].getType() == ACTIVE )
                        {
                            board[i][j].setType(DISABLED);
                            board[i+1][j].setType(ACTIVE);
                        }
                        else if ( board[i][j].getType() == PIVOT )
                        {
                            board[i][j].setType(DISABLED);
                            board[i+1][j].setType(PIVOT);
                        }
                    }
                    //Need special check for row 0
                    if( i == 1 )
                    {
                        if ( i != board.size() -1 and
                                board[i-1][j].getStuck() == false )
                        {
                            if (  board[i-1][j].getType() == ACTIVE )
                            {
                                board[i-1][j].setType(DISABLED);
                                board[i][j].setType(ACTIVE);
                            }
                            else if ( board[i-1][j].getType() == PIVOT )
                            {
                                board[i-1][j].setType(DISABLED);
                                board[i][j].setType(PIVOT);
                            }
                        }
                    }
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TetrisBoard::checkLeft()
    {
        //std::pait<unsigned int, unsigned int> piv;
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 1; j < board[i].size() ; ++j)
            {
                if( ( board[i][j].getType() == ACTIVE or
                      board[i][j].getType() == PIVOT ) and
                      board[i][j].getStuck() == false and
                      board[i][j-1].getStuck() == true )
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool TetrisBoard::checkRight()
    {
        for(std::vector< std::vector<Square> >::size_type i = 0;
                i < board.size(); ++i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size() - 1; ++j)
            {
                if( ( board[i][j].getType() == ACTIVE or
                      board[i][j].getType() == PIVOT ) and
                      board[i][j].getStuck() == false and
                      board[i][j+1].getStuck() == true )
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool TetrisBoard::moveLeft()
    {
        //std::cout << "Left" << std::endl;
        std::pair<int, int> bound = getXBound();
        //std::cout << "x: " << bound.first << ", y: " << bound.second << std::endl;
        if( bound.first - 1 >= 0 and checkLeft() )
        {
            std::pair<unsigned int, unsigned int> piv = getPivot();
            clearActive();
            mergeShape(currentShape, piv.first, piv.second - 1);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TetrisBoard::moveRight()
    {
        //std::cout << "Right" << std::endl;
        std::pair<int, int> bound = getXBound();
        //std::cout << "x: " << bound.first << ", y: " << bound.second << std::endl;
        if( bound.second + 1 <= (int)board[0].size() - 1  and checkRight() )
        {
            std::pair<unsigned int, unsigned int> piv = getPivot();
            clearActive();
            mergeShape(currentShape, piv.first, piv.second + 1);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TetrisBoard::pushStuck(unsigned int n)
    {
        if( n == 0 )
        {
            for(std::vector<Square>::size_type j = 0; j < board[n].size(); ++j)
            {
                board[n][j].setType(DISABLED);
                board[n][j].setStuck(false);
            }
            return true;
        }
        for(std::vector< std::vector<Square> >::size_type i = n;
                i > 0; --i)
        {
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if ( board[i-1][j].getStuck() == true )
                {
                    board[i][j].setType(ACTIVE);
                    board[i][j].setStuck(true);
                }
                if ( board[i-1][j].getType() == DISABLED and
                     board[i][j].getStuck() == true )
                {
                    board[i][j].setType(DISABLED);
                    board[i][j].setStuck(false);
                }
                //Need special check for row 0
                if( i == 1 )
                {
                    if (  board[i-1][j].getStuck() == true )
                    {
                        board[i-1][j].setType(DISABLED);
                        board[i-1][j].setStuck(false);
                    }
                }
            }
        }
        return true;
    }

    int TetrisBoard::checkRow()
    {
        int deleted = 0;
        for(std::vector< std::vector<Square> >::size_type i = board.size() - 1;
                i != 0; --i)
        {
            bool allStuck = true;
            for(std::vector<Square>::size_type j = 0; j < board[i].size(); ++j)
            {
                if( board[i][j].getStuck() == false )
                {
                    allStuck = false;
                }
            }
            if( allStuck )
            {
                pushStuck(i);
                ++i;
                deleted += 1;
            }
        }
        return deleted;
    }
    bool TetrisBoard::lost()
    {
        for(std::vector<Square>::size_type j = 0; j < board[1].size(); ++j)
        {
            if( board[1][j].getStuck() == true )
            {
                return true;
            }
        }
        return false;
    }
}
