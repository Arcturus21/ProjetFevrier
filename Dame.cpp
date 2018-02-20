#include "Dame.h"

Dame::Dame(const Pion& p) : Pion(p)
{
    //ctor
    _shape.setOutlineThickness(5);
    if(_color==sf::Color::White)
    {
        _shape.setOutlineColor(sf::Color(50,50,200));
    }
    else
    {
        _shape.setOutlineColor(sf::Color(200,50,50));
    }
}

Dame::~Dame()
{
    //dtor
}
