#ifndef _TETRIS_SHAPES_
#define _TETRIS_SHAPES_
#include <iostream>
#include <vector>
namespace my_tetris
{
    enum Style
    {
        style_L,       style_RL,      style_Light,
        style_RLight,  style_Single,  style_Block,
        style_Line2,   style_Line3,   style_Line4,
        style_T, NONE
    };

    enum square_type
    {
        DISABLED, ACTIVE, PIVOT
    };

    class Square
    {
        private:
            square_type squareType;
            bool square_stuck;
        public:
            Square(square_type type);
            void setType(square_type type);
            square_type getType() const;

            void setStuck(bool stuck);
            bool getStuck() const;

    };
    std::ostream & operator<<(std::ostream & out, const Square & sqr);

    class Shape
    {
        protected:
            Style style;
            bool shape_stuck;
            std::vector< std::vector<Square> > squares;
            int rotation;
        public:
            Shape(Style s);
            bool isStuck() const;
            void setStuck(bool stuck);
            void rotate();
            std::vector< std::vector<Square> > getSquares() const;
    };
    std::ostream & operator<<(std::ostream & out, const Shape & shp);

    class Block : public Shape
    {
        public:
            Block();
    };

    class L_Shape : public Shape
    {
        public:
            L_Shape();
    };
    class RL_Shape : public Shape
    {
        public:
            RL_Shape();
    };
    class T_Shape : public Shape
    {
        public:
            T_Shape();
    };
    class Line4 : public Shape
    {
        public:
            Line4();
    };
    class Lightning : public Shape
    {
        public:
            Lightning();
    };
    class RLightning : public Shape
    {
        public:
            RLightning();
    };

    Shape getRandom();
}
#endif // _TERTIS_SHAPES_
