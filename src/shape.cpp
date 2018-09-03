#include "shape.h"
#include <iostream>
#include <random>
#include <ctime>
using namespace std;
namespace my_tetris
{
    //-- SQUARE --
    Square::Square(square_type type)
        : squareType(type), square_stuck(false)
    {
    }

    void Square::setType(square_type type)
    {
        squareType = type;
    }

    square_type Square::getType() const { return squareType; }

    void Square::setStuck(bool stuck)
    {
        square_stuck = stuck;
    }

    bool Square::getStuck() const { return square_stuck; }

    ostream & operator<<(ostream & out, const Square & sqr)
    {
        switch(sqr.getType())
        {
            case DISABLED:
                out << "-";
                return out;

            case ACTIVE:
                out << "x";
                return out;

            case PIVOT:
                out << "o";
                return out;

            default:
                return out;
        }
    }

    // -- Shape parser --
    vector< vector<Square> > parseTable(const vector< vector<char> > & t)
    {
        vector< vector<Square> > parsed;
        for(vector< vector<Square> >::size_type i = 0; i < t.size(); ++i)
        {
            vector<Square> row;
            for(vector<char>::size_type j = 0; j < t[i].size(); ++j)
            {
                switch( t[i][j] )
                {
                    case 0:
                        row.push_back(Square(DISABLED));
                        break;

                    case 1:
                        row.push_back(Square(ACTIVE));
                        break;

                    case 2:
                        row.push_back(Square(PIVOT));
                        break;
                    default:
                        cout << "Error occured in parser!";
                }
            }
            parsed.push_back(row);
        }
        return parsed;
    }
    // -- Determine ShapeStyle --
    vector< vector<Square> > getShape(Style style)
    {
        vector< vector<char> > shape;
        switch( style )
        {
            case style_L:
                shape = { { 0, 0, 0, 0, 0},
                           { 0, 0, 1, 0, 0},
                           { 0, 0, 2, 0, 0},
                           { 0, 0, 1, 1, 0},
                           { 0, 0, 0, 0, 0}  };
                break;

            case style_RL:
                shape = { { 0, 0, 0, 0, 0},
                           { 0, 0, 1, 0, 0},
                           { 0, 0, 2, 0, 0},
                           { 0, 1, 1, 0, 0},
                           { 0, 0, 0, 0, 0}  };
                break;

            case style_Light:
                shape = { { 0, 0, 0, 0, 0},
                           { 0, 0, 1, 0, 0},
                           { 0, 1, 2, 0, 0},
                           { 0, 1, 0, 0, 0},
                           { 0, 0, 0, 0, 0}  };
                break;

            case style_RLight:
                shape = { { 0, 0, 0, 0, 0},
                          { 0, 0, 1, 0, 0},
                          { 0, 0, 2, 1, 0},
                          { 0, 0, 0, 1, 0},
                          { 0, 0, 0, 0, 0}  };
                break;

            case style_Single:
                shape = { { 0, 0, 0, 0, 0},
                          { 0, 0, 0, 0, 0},
                          { 0, 0, 2, 0, 0},
                          { 0, 0, 0, 0, 0},
                          { 0, 0, 0, 0, 0}  };
                break;

            case style_Block:
                shape = { { 0, 0, 0, 0, 0},
                          { 0, 0, 0, 0, 0},
                          { 0, 1, 2, 0, 0},
                          { 0, 1, 1, 0, 0},
                          { 0, 0, 0, 0, 0}  };
                break;

            case style_Line4:
                shape = { { 0, 0, 1, 0, 0},
                          { 0, 0, 1, 0, 0},
                          { 0, 0, 2, 0, 0},
                          { 0, 0, 1, 0, 0},
                          { 0, 0, 0, 0, 0}  };
                break;

            case style_T:
                shape = { { 0, 0, 0, 0, 0},
                          { 0, 0, 0, 0, 0},
                          { 0, 1, 2, 1, 0},
                          { 0, 0, 1, 0, 0},
                          { 0, 0, 0, 0, 0}  };
                break;

            default:
                break;

        }
        return parseTable(shape);
    }

    // -- Shape Rotator --
    void Shape::rotate()
    {
        if( rotation == 4 )
        {
            rotation = 1;
        }
        else
        {
            rotation += 1;
        }
        vector< vector<Square> > rotated;
        for(vector< vector<Square> >::size_type i = 0; i < squares.size(); ++i)
        {
            vector<Square> row;
            for(vector<Square>::size_type j = squares[i].size(); j > 0; --j)
            {
                row.push_back(squares[j-1][i]);
            }
            rotated.push_back(row);
        }
        squares = rotated;
    }

    //-- SHAPE --
    Shape::Shape(Style s) : style(s), shape_stuck(false),
    squares(getShape(style)), rotation(1)
    {
    }

    vector< vector<Square> > Shape::getSquares() const
    {
        return squares;
    }

    bool Shape::isStuck() const
    {
        //Check all squares and if all unstuck keep unstuck
        return shape_stuck;
    }

    ostream & operator<<(ostream & out, const Shape & shp)
    {
        for(vector< vector<Square> >::size_type i = 0; i < shp.getSquares().size(); ++i)
        {
            for(vector<Square>::size_type j = 0; j < shp.getSquares()[i].size(); ++j)
            {
                out << " " << shp.getSquares()[i][j];
            }
            out << "\n";
        }
        return out;
    }


    //-- BLOCK --
    Block::Block()
        : Shape(style_Block)
    {
    }

    // -- L-Shape --
    L_Shape::L_Shape()
        : Shape(style_L)
    {
    }

    // -- RL-Shape --
    RL_Shape::RL_Shape()
        : Shape(style_RL)
    {
    }

    // -- T-Shape --
    T_Shape::T_Shape()
        : Shape(style_T)
    {
    }

    // -- Line4  --
    Line4::Line4()
        : Shape(style_Line4)
    {
    }

    // -- Lightning  --
    Lightning::Lightning()
        : Shape(style_Light)
    {
    }

    // -- RLightning  --
    RLightning::RLightning()
        : Shape(style_RLight)
    {
    }

    Shape getRandom()
    {
        std::srand(std::time(nullptr));
        int dice_roll = std::rand() % 7;
        switch( dice_roll )
        {
            case 0:
                return RLightning();

            case 1:
                return Block();

            case 2:
                return L_Shape();

            case 3:
                return RL_Shape();

            case 4:
                return T_Shape();

            case 5:
                return Line4();

            case 6:
                return Lightning();

            default:
                return L_Shape();
        }
    }
}
