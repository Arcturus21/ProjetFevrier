#include "Pion.h"
#include "Case.h"
#include "Plateau.h"

using namespace std;

Pion::Pion(int caseX, int caseY, int tailleCase, sf::Color color):
    _caseX(caseX), _caseY(caseY), _color(color), _shape(RAYON_PION), _alive(true)
{
    //ctor
    setOrigin(RAYON_PION,RAYON_PION);
    _shape.setFillColor(color);
    _shape.setOutlineColor(sf::Color(128,128,128));
    _shape.setOutlineThickness(3);

    int moitTailleCase=tailleCase/2;
    setPosition(caseX*tailleCase+moitTailleCase,caseY*tailleCase+moitTailleCase);
}

Pion::~Pion()
{
    //dtor
}

///A SIMPLIFIER
std::vector<Case*>* Pion::GetCaseDeplacement()
{
    vector<Case*>* retour=new vector<Case*>();
    Case* cTemp=NULL;

    if(_color==sf::Color::White)
    {
        ///EN FACE A SA GAUCHE
        cTemp=_plateau->GetCase(_caseX-1,_caseY-1);
        if(cTemp!=NULL)
        {
            if(cTemp->GetEtatCase()==vide)
                retour->push_back(cTemp);
            else if(cTemp->GetEtatCase()==pionNoir)
            {
                cTemp=_plateau->GetCase(_caseX-2,_caseY-2);
                if(cTemp!=NULL && cTemp->GetEtatCase()==vide)
                    retour->push_back(cTemp);
            }
        }
        ///EN FACE A SA DROITE
        cTemp=_plateau->GetCase(_caseX+1,_caseY-1);
        if(cTemp!=NULL)
        {
            if(cTemp->GetEtatCase()==vide)
                retour->push_back(cTemp);
            else if(cTemp->GetEtatCase()==pionNoir)
            {
                cTemp=_plateau->GetCase(_caseX+2,_caseY-2);
                if(cTemp!=NULL && cTemp->GetEtatCase()==vide)
                    retour->push_back(cTemp);
            }
        }
    }
    else
    {
        ///EN FACE A SA GAUCHE
        cTemp=_plateau->GetCase(_caseX-1,_caseY+1);
        if(cTemp!=NULL)
        {
            if(cTemp->GetEtatCase()==vide)
                retour->push_back(cTemp);
            else if(cTemp->GetEtatCase()==pionBlanc)
            {
                cTemp=_plateau->GetCase(_caseX-2,_caseY+2);
                if(cTemp!=NULL && cTemp->GetEtatCase()==vide)
                    retour->push_back(cTemp);
            }
        }
        ///EN FACE A SA DROITE
        cTemp=_plateau->GetCase(_caseX+1,_caseY+1);
        if(cTemp!=NULL)
        {
            if(cTemp->GetEtatCase()==vide)
                retour->push_back(cTemp);
            else if(cTemp->GetEtatCase()==pionBlanc)
            {
                cTemp=_plateau->GetCase(_caseX+2,_caseY+2);
                if(cTemp!=NULL && cTemp->GetEtatCase()==vide)
                    retour->push_back(cTemp);
            }
        }
    }

    return retour;
}

void Pion::setCase(int caseX,int caseY)
{
    _plateau->GetCase(_caseX,_caseY)->SetPion(NULL);

    _caseX=caseX;
    _caseY=caseY;

    int moitTailleCase=TAILLE_CASE/2;
    setPosition(caseX*TAILLE_CASE+moitTailleCase,caseY*TAILLE_CASE+moitTailleCase);

    _plateau->GetCase(_caseX,_caseY)->SetPion(this);
}

Case* Pion::getCase()
{
    if(_plateau==NULL)
        return NULL;
    return _plateau->GetCase(_caseX,_caseY);
}
