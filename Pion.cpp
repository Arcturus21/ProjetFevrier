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

Pion::Pion(const Pion& p) : _caseX(p._caseX), _caseY(p._caseY), _color(p._color), _shape(p._shape), _alive(true), _plateau(p._plateau)
{
    setOrigin(RAYON_PION,RAYON_PION);
    setPosition(p.getPosition());
}

Pion::~Pion()
{
    //dtor
}

std::vector<Case*>* Pion::GetCaseDeplacement(bool manger)
{
    vector<Case*>* retour=new vector<Case*>();
    Case *cTemp=NULL, *cTemp2=NULL;

    EtatCase pionAdverse;
    int enFace=0;
    if(_color==sf::Color::White)
    {
        pionAdverse=pionNoir;
        enFace=-1;
    }
    else
    {
        pionAdverse=pionBlanc;
        enFace=1;
    }

    ///Vérifie si on peut manger un pion dans les 4 directions
    ///BAS-DROITE
    cTemp=_plateau->GetCase(_caseX+2,_caseY+2); ///Case sur laquelle on place le pion
    cTemp2=_plateau->GetCase(_caseX+1,_caseY+1);    ///Case potentielle du pion adverse
    if(cTemp!=NULL && cTemp2!=NULL) ///Au cas ou on soit hors plateau ou en cas d'erreur
    {
        if(cTemp2->GetEtatCase()==pionAdverse && cTemp->GetEtatCase()==vide)
        {
            retour->push_back(cTemp);
            manger=true;                ///si le pion peut manger un autre pion
        }
    }

    ///BAS-GAUCHE
    cTemp=_plateau->GetCase(_caseX-2,_caseY+2); ///Case sur laquelle on place le pion
    cTemp2=_plateau->GetCase(_caseX-1,_caseY+1);    ///Case potentielle du pion adverse
    if(cTemp!=NULL && cTemp2!=NULL) ///Au cas ou on soit hors plateau ou en cas d'erreur
    {
        if(cTemp2->GetEtatCase()==pionAdverse && cTemp->GetEtatCase()==vide)
        {
            retour->push_back(cTemp);
            manger=true;                ///si le pion peut manger un autre pion
        }
    }

    ///HAUT-DROITE
    cTemp=_plateau->GetCase(_caseX+2,_caseY-2); ///Case sur laquelle on place le pion
    cTemp2=_plateau->GetCase(_caseX+1,_caseY-1);    ///Case potentielle du pion adverse
    if(cTemp!=NULL && cTemp2!=NULL) ///Au cas ou on soit hors plateau ou en cas d'erreur
    {
        if(cTemp2->GetEtatCase()==pionAdverse && cTemp->GetEtatCase()==vide)
        {
            retour->push_back(cTemp);
            manger=true;                ///si le pion peut manger un autre pion
        }
    }

    ///HAUT-GAUCHE
    cTemp=_plateau->GetCase(_caseX-2,_caseY-2); ///Case sur laquelle on place le pion
    cTemp2=_plateau->GetCase(_caseX-1,_caseY-1);    ///Case potentielle du pion adverse
    if(cTemp!=NULL && cTemp2!=NULL) ///Au cas ou on soit hors plateau ou en cas d'erreur
    {
        if(cTemp2->GetEtatCase()==pionAdverse && cTemp->GetEtatCase()==vide)
        {
            retour->push_back(cTemp);
            manger=true;                ///si le pion peut manger un autre pion
        }
    }

    ///Si on a pas l'occasion de manger un pion, on peut déplacer librement
    if(!manger)
    {
        ///GAUCHE
        cTemp=_plateau->GetCase(_caseX-1,_caseY+enFace); ///Case sur laquelle on place le pion
        if(cTemp!=NULL)                   ///Au cas ou on soit hors plateau ou en cas d'erreur
        {
            if(cTemp->GetEtatCase()==vide)
            {
                retour->push_back(cTemp);
            }
        }

        ///DROITE
        cTemp=_plateau->GetCase(_caseX+1,_caseY+enFace); ///Case sur laquelle on place le pion
        if(cTemp!=NULL)                   ///Au cas ou on soit hors plateau ou en cas d'erreur
        {
            if(cTemp->GetEtatCase()==vide)
            {
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
