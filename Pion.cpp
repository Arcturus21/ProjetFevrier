#include "Pion.h"

Pion::Pion(int caseX, int caseY, int tailleCase, sf::Color color):
    _caseX(caseX), _caseY(caseY), _color(color), _shape(RAYON_PION)
{
    //ctor
    _shape.setFillColor(color);
    setPosition(caseX*tailleCase,caseY*tailleCase);
}

Pion::~Pion()
{
    //dtor
}
